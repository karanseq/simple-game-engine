#include "Game\GameData.h"

// external includes
#include "lua.hpp"

// engine includes
#include "Assert\Assert.h"
#include "Events\TimerEvent.h"
#include "Jobs\FileLoadJob.h"
#include "Jobs\JobSystem.h"
#include "Logger\Logger.h"
#include "Time\Updater.h"
#include "Util\LuaHelper.h"

namespace game {

// static member initializations
const char* GameData::GAME_CONFIG_FILE = "Data\\GameConfig.lua";

GameData::GameData() : on_loading_complete_(nullptr),
    on_loading_failed_(nullptr),
    files_left_to_load_(0),
    jobs_left_to_finish_(0),
    player_lua_file_path_(""),
    bullet_lua_file_path_(""),
    level_lua_file_path_(""),
    pause_overlay_file_path_(""),
    num_levels_(5)
{}

GameData::~GameData()
{}

void GameData::LoadAssetsListedInConfig(const std::function<void(void)>& i_on_loading_complete, const std::function<void(void)>& i_on_loading_failed)
{
    ASSERT(i_on_loading_complete && i_on_loading_failed);
    on_loading_complete_ = i_on_loading_complete;
    on_loading_failed_ = i_on_loading_failed;

    const auto config_file_data = engine::util::FileUtils::Get()->GetFileFromCache(GameData::GAME_CONFIG_FILE);

    ASSERT(config_file_data.file_contents);
    ASSERT(config_file_data.file_size > 0);

    // initialize lua state
    lua_State* lua_state = luaL_newstate();
    ASSERT(lua_state);

    luaL_openlibs(lua_state);

    // do initial buffer parsing
    int result = 0;
    result = luaL_loadbuffer(lua_state, reinterpret_cast<char*>(config_file_data.file_contents), config_file_data.file_size, nullptr);
    ASSERT(result == 0);
    lua_pcall(lua_state, 0, 0, 0);
    ASSERT(result == 0);

    int type = LUA_TNIL;

    // find the global variable named "GameConfig" & push it onto stack
    // if not found, it pushes nil value instead
    result = lua_getglobal(lua_state, "GameConfig");
    ASSERT(result == LUA_TTABLE);

    // extract the player lua file path
    player_lua_file_path_ = engine::util::LuaHelper::CreatePooledString(lua_state, "player_lua");

    // extract the bullet lua file path
    bullet_lua_file_path_ = engine::util::LuaHelper::CreatePooledString(lua_state, "bullet_lua");

    // extract the level lua file path
    level_lua_file_path_ = engine::util::LuaHelper::CreatePooledString(lua_state, "level_lua");

    // extract the number of levels
    num_levels_ = static_cast<uint8_t>(engine::util::LuaHelper::CreateInt(lua_state, "num_levels"));

    // check if there is an asset list
    lua_pushstring(lua_state, "asset_list");
    result = LUA_TNIL;
    result = lua_gettable(lua_state, -2);
    ASSERT(result == LUA_TTABLE);

    size_t index = 0;
    const engine::data::PooledString game_team("GameTeam");

    lua_pushnil(lua_state);

    // loop through all assets in the asset list
    while (lua_next(lua_state, -2) != 0)
    {
        if (lua_type(lua_state, -1) == LUA_TTABLE)
        {
            // update counters
            ++files_left_to_load_;
            ++jobs_left_to_finish_;

            // extract the file name for this asset
            const engine::data::PooledString asset_file_name = engine::util::LuaHelper::CreatePooledString(lua_state, "file_name");

            // create a job to load this asset
            engine::jobs::FileLoadJob* file_load_job = new engine::jobs::FileLoadJob(asset_file_name, std::bind(&GameData::OnFileLoaded, this, std::placeholders::_1), true);
            engine::jobs::JobSystem::Get()->AddJob(file_load_job, game_team);
        }
        else
        {
            LOG_ERROR("%s expected a table at index %d", __FUNCTION__, index);
        }

        // remove value
        lua_pop(lua_state, 1);
        ++index;
    }

    // pop asset list
    lua_pop(lua_state, 1);

    // pop "GameConfig" table
    lua_pop(lua_state, 1);

    // validate that the stack is clean
    int stack_items = lua_gettop(lua_state);
    ASSERT(stack_items == 0);

    lua_close(lua_state);
}

void GameData::OnFileLoaded(const engine::util::FileUtils::FileData& i_file_data)
{
    // update counters
    --jobs_left_to_finish_;
    files_left_to_load_ -= (i_file_data.file_contents && i_file_data.file_size > 0) ? 1 : 0;

    // check if all jobs have finished
    if (jobs_left_to_finish_ == 0)
    {
        // call the appropriate callback function based on whether all files were loaded
        // create a timer event so the callback is called from the main thread
        engine::time::Updater::Get()->AddTimerEvent(engine::events::TimerEvent::Create(files_left_to_load_ == 0 ? on_loading_complete_ : on_loading_failed_, 0.001f, 0));
    }
}

} // namespace monsterchase
