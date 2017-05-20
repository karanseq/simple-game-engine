#include "Game\LevelData.h"

// external includes
#include "lua.hpp"

// engine includes
#include "Assert\Assert.h"
#include "Events\TimerEvent.h"
#include "Jobs\CreateActorFromFileAtPositionJob.h"
#include "Jobs\JobSystem.h"
#include "Logger\Logger.h"
#include "Time\Updater.h"
#include "Util\LuaHelper.h"

namespace game {

LevelData::LevelData() : on_loading_complete_(nullptr),
    on_loading_failed_(nullptr),
    actors_left_to_create_(0),
    jobs_left_to_finish_(0)
{}

LevelData::~LevelData()
{}

void LevelData::LoadLevel(const engine::util::FileUtils::FileData& i_level_file_data, const std::function<void(void)>& i_on_loading_complete, const std::function<void(void)>& i_on_loading_failed)
{
    ASSERT(i_on_loading_complete && i_on_loading_failed);
    on_loading_complete_ = i_on_loading_complete;
    on_loading_failed_ = i_on_loading_failed;

    ASSERT(i_level_file_data.file_contents);
    ASSERT(i_level_file_data.file_size > 0);

    // initialize lua state
    lua_State* lua_state = luaL_newstate();
    ASSERT(lua_state);

    luaL_openlibs(lua_state);

    // do initial buffer parsing
    int result = 0;
    result = luaL_loadbuffer(lua_state, reinterpret_cast<char*>(i_level_file_data.file_contents), i_level_file_data.file_size, nullptr);
    ASSERT(result == 0);
    lua_pcall(lua_state, 0, 0, 0);
    ASSERT(result == 0);

    int type = LUA_TNIL;

    // find the global variable named "Level" & push it onto stack
    // if not found, it pushes nil value instead
    result = lua_getglobal(lua_state, "Level");
    ASSERT(result == LUA_TTABLE);

    // extract the player fire rate
    level_.player_fire_rate_ = engine::util::LuaHelper::CreateFloat(lua_state, "player_fire_rate");

    // extract the enemy fire rate
    level_.enemy_fire_rate_ = engine::util::LuaHelper::CreateFloat(lua_state, "enemy_fire_rate");

    // extract the enemy move interval
    level_.enemy_move_interval_ = engine::util::LuaHelper::CreateFloat(lua_state, "enemy_move_interval");

    // extract the enemy move impulse
    level_.enemy_move_impulse_ = engine::util::LuaHelper::CreateFloat(lua_state, "enemy_move_impulse");

    // check if there are actors
    lua_pushstring(lua_state, "actors");
    result = LUA_TNIL;
    result = lua_gettable(lua_state, -2);
    ASSERT(result == LUA_TTABLE);

    size_t index = 0;
    static const engine::data::PooledString game_team("GameTeam");

    lua_pushnil(lua_state);

    // loop through all actors
    while (lua_next(lua_state, -2) != 0)
    {
        if (lua_type(lua_state, -1) == LUA_TTABLE)
        {
            std::lock_guard<std::mutex> lock(actors_created_mutex_);

            // update counters
            ++actors_left_to_create_;
            ++jobs_left_to_finish_;

            // extract this actor's asset file information
            char file_name[512];
            if (!engine::util::LuaHelper::CreateCString(lua_state, "file", 512, file_name))
            {
                LOG_ERROR("%s could not extract the file name at index %d", __FUNCTION__, index);
            }

            // fetch this actor's asset file from the file cache
            const engine::util::FileUtils::FileData file_data = engine::util::FileUtils::Get()->GetFileFromCache(engine::data::HashedString::Hash(file_name));

            // extract this actor's position in the level
            const engine::math::Vec3D position = engine::util::LuaHelper::CreateVec3D(lua_state, "position");

            // create a job to create this actor
            engine::jobs::CreateActorFromFileAtPositionJob* create_actor_job = new engine::jobs::CreateActorFromFileAtPositionJob(file_data, position, std::bind(&LevelData::OnActorCreated, this, std::placeholders::_1));
            engine::jobs::JobSystem::Get()->AddJob(create_actor_job, game_team);
        }
        else
        {
            LOG_ERROR("%s expected a table at index %d", __FUNCTION__, index);
        }

        // remove value
        lua_pop(lua_state, 1);
        ++index;
    }

    // pop "actors"
    lua_pop(lua_state, 1);

    // pop "Level" table
    lua_pop(lua_state, 1);

    // validate that the stack is clean
    int stack_items = lua_gettop(lua_state);
    ASSERT(stack_items == 0);

    lua_close(lua_state);
}

void LevelData::OnActorCreated(engine::memory::SharedPointer<engine::gameobject::Actor> i_actor_created)
{
    ASSERT(i_actor_created);

    static const engine::data::HashedString enemy_type("Enemy");
    static const engine::data::HashedString brick_type("Brick");

    std::lock_guard<std::mutex> lock(actors_created_mutex_);

    --jobs_left_to_finish_;

    if (i_actor_created)
    {
        --actors_left_to_create_;

        if (i_actor_created->GetType() == enemy_type)
        {
            level_.enemies_.push_back(i_actor_created);
            ++level_.num_enemies_;
        }
        else if (i_actor_created->GetType() == brick_type)
        {
            level_.bricks_.push_back(i_actor_created);
            ++level_.num_bricks_;
        }
        else
        {
            LOG_ERROR("%s found invalid type for actor with name %s", __FUNCTION__, i_actor_created->GetName().GetString());
        }
    }

    if (jobs_left_to_finish_ == 0)
    {
        // call the appropriate callback function based on whether all files were loaded
        // create a timer event so the callback is called from the main thread
        engine::time::Updater::Get()->AddTimerEvent(engine::events::TimerEvent::Create(actors_left_to_create_ == 0 ? on_loading_complete_ : on_loading_failed_, 0.001f, 0));
    }
}

} // namespace game
