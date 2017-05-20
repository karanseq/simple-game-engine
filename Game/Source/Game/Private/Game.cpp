#include "Game\Game.h"

// engine includes 
#include "Assert\Assert.h"
#include "Common\Engine.h"
#include "Common\HelperMacros.h"
#include "Events\EventDispatcher.h"
#include "GameObject\ActorCreator.h"
#include "Jobs\CreateActorFromFileJob.h"
#include "Jobs\FileLoadJob.h"
#include "Jobs\JobSystem.h"
#include "Logger\Logger.h"
#include "Time\Updater.h"
#include "Util\FileUtils.h"

// game includes
#include "Game\GameUtils.h"
#include "Game\LevelData.h"
#include "Game\Player.h"

namespace game {

bool StartUp()
{
    LOG("-------------------- Game StartUp --------------------");

    // create a team of workers for the game
    if (!engine::jobs::JobSystem::Get()->CreateTeam(engine::data::PooledString("GameTeam"), 10))
    {
        LOG_ERROR("Could not create a job team for the game!");
        return false;
    }

    // load the game config
    const engine::util::FileUtils::FileData config_file_data = engine::util::FileUtils::Get()->ReadFile(GameData::GAME_CONFIG_FILE);
    ASSERT(config_file_data.file_contents && config_file_data.file_size > 0);

    // create an instance of the game
    Game* game = Game::Create();
    if (game == nullptr)
    {
        LOG_ERROR("Could not create an instance of the game!");
        return false;
    }

    srand(static_cast<unsigned int>(time(0)));

    return true;
}

void Shutdown()
{
    Game::Destroy();
    LOG("-------------------- Game Shutdown --------------------");
}

// static member initialization
Game* Game::instance_ = nullptr;
const float Game::BULLET_IMPULSE = 12.0f;

Game* Game::Create()
{
    // create the singleton instance of Game
    if (!Game::instance_)
    {
        Game::instance_ = new Game();
    }
    return Game::instance_;
}

void Game::Destroy()
{
    // destroy the singleton instance of Game
    SAFE_DELETE(Game::instance_);
}

Game::Game() : game_state_(GameStates::kGameStateNone),
    init_successful_(false),
    level_number_(1),
    level_data_(nullptr),
    player_(nullptr),
    enemy_moves_(0),
    keyboard_event_(nullptr),
    move_enemies_event_(nullptr),
    fire_enemy_bullet_event_(nullptr)
{
    game_state_ = GameStates::kGameStateLoading;
    game_data_.LoadAssetsListedInConfig(std::bind(&Game::OnAssetLoadingComplete, this), std::bind(&Game::OnAssetLoadingFailed, this));
}

Game::~Game()
{
    Reset();
}

bool Game::Init()
{
    ASSERT(game_state_ == GameStates::kGameStateBegin);

    // make sure we don't init repeatedly
    if (init_successful_)
    {
        return false;
    }
    init_successful_ = true;

    LOG("%s", __FUNCTION__);

    // register for update events
    engine::time::Updater::Get()->AddTickable(this);

    // register for collision events
    engine::physics::Collider::Get()->SetCollisionListener(this);

    // create a new keyboard event
    keyboard_event_ = engine::events::KeyboardEvent::Create();
    ASSERT(keyboard_event_);

    // register for key events
    keyboard_event_->SetOnKeyPressed(std::bind(&Game::OnKeyPressed, this, std::placeholders::_1));
    engine::events::EventDispatcher::Get()->AddKeyboardEventListener(keyboard_event_);

    // create the level
    CreateLevel();

    // create the player
    CreatePlayer();

    // create a pool of enemy bullets
    CreateEnemyBullets();

    return true;
}

void Game::Reset()
{
    ASSERT(game_state_ == GameStates::kGameStateQuit || game_state_ == GameStates::kGameStateRestart);

    // make sure we don't reset repeatedly
    if (!init_successful_)
    {
        return;
    }
    init_successful_ = false;

    LOG("%s", __FUNCTION__);

    move_enemies_event_ = nullptr;
    fire_enemy_bullet_event_ = nullptr;
    engine::time::Updater::Get()->RemoveAllTimerEvents();

    DestroyEnemyBullets();

    DestroyPlayer();

    DestroyLevel();

    engine::events::EventDispatcher::Get()->RemoveKeyboardEventListener(keyboard_event_);
    keyboard_event_ = nullptr;

    // tell the engine we no longer want collision events
    engine::physics::Collider::Get()->SetCollisionListener(nullptr);

    // tell the engine we no longer want to be ticked
    engine::time::Updater::Get()->RemoveTickable(this);
}

void Game::CheckLevelComplete()
{
    if (level_data_->level_.num_enemies_ <= 0)
    {
        game_state_ = GameStates::kLevelComplete;
        DoLevelCompletedEffect();
    }
}

void Game::OnAssetLoadingComplete()
{
    LOG("%s", __FUNCTION__);
    game_state_ = GameStates::kGameStateBegin;
    Init();
}

void Game::OnAssetLoadingFailed()
{
    LOG("%s", __FUNCTION__);
    game_state_ = GameStates::kGameStateQuit;
    engine::InitiateShutdown();
}

void Game::OnLevelLoadingComplete()
{
    LOG("%s", __FUNCTION__);

    enemy_moves_ = 1;
    move_enemies_event_ = engine::events::TimerEvent::Create(std::bind(&Game::OnMoveEnemiesTimerElapsed, this), level_data_->level_.enemy_move_interval_, -1);
    engine::time::Updater::Get()->AddTimerEvent(move_enemies_event_);

    fire_enemy_bullet_event_ = engine::events::TimerEvent::Create(std::bind(&Game::OnFireEnemyBulletTimerElapsed, this), level_data_->level_.enemy_fire_rate_, -1);
    engine::time::Updater::Get()->AddTimerEvent(fire_enemy_bullet_event_);

    game_state_ = GameStates::kGameStateRunning;
}

void Game::OnLevelLoadingFailed()
{
    LOG("%s", __FUNCTION__);
    game_state_ = GameStates::kGameStateQuit;
    engine::InitiateShutdown();
}

void Game::OnKeyPressed(unsigned int i_key_id)
{
    if (game_state_ != GameStates::kGameStateRunning &&
        game_state_ != GameStates::kGameStatePaused)
    {
        return;
    }

    if (i_key_id == 'P')
    {
        if (engine::IsPaused())
        {
            engine::Resume();
            game_state_ = GameStates::kGameStateRunning;
        }
        else
        {
            engine::Pause();
            game_state_ = GameStates::kGameStatePaused;
        }
    }
    else if (i_key_id == 'R')
    {
        if (engine::IsPaused())
        {
            engine::Resume();
            game_state_ = GameStates::kGameStateRestart;
        }
    }
    else if (i_key_id == 'Q')
    {
        if (engine::IsPaused())
        {
            game_state_ = GameStates::kGameStateQuit;
            engine::InitiateShutdown();
        }
    }
}

void Game::OnMoveEnemiesTimerElapsed()
{
    for (const auto& enemy : level_data_->level_.enemies_)
    {
        enemy->GetPhysicsObject().Lock()->ApplyImpulse(engine::math::Vec3D(level_data_->level_.enemy_move_impulse_));
    }

    --enemy_moves_;
    level_data_->level_.enemy_move_impulse_ *= enemy_moves_ <= 0 ? -1 : 1;
    enemy_moves_ = enemy_moves_ <= 0 ? 2 : enemy_moves_;
}

void Game::OnFireEnemyBulletTimerElapsed()
{
    static const engine::data::PooledString enemy_types[3] = { "Enemy_01", "Enemy_02", "Enemy_03" };
    static const engine::data::PooledString bullet_types[3] = { "Bullet_01", "Bullet_02", "Bullet_03" };

    const uint8_t random_index = rand() % level_data_->level_.num_enemies_;
    const engine::data::PooledString& enemy_name = level_data_->level_.enemies_[random_index]->GetName();

    uint8_t bullet_type = 0;
    for (uint8_t i = 0; i < 3; ++i)
    {
        if (enemy_name == enemy_types[i])
        {
            bullet_type = i;
            break;
        }
    }

    for (const auto& bullet : enemy_bullet_pool_)
    {
        if (bullet->GetName() == bullet_types[bullet_type] && bullet->GetIsEnabled() == false)
        {
            bullet->SetIsEnabled(true);
            bullet->GetGameObject()->SetPosition(level_data_->level_.enemies_[random_index]->GetGameObject()->GetPosition());
            bullet->GetPhysicsObject().Lock()->ApplyImpulse(engine::math::Vec3D(0.0f, -Game::BULLET_IMPULSE));
            break;
        }
    }
}

void Game::OnEnemyBulletCreated(engine::memory::SharedPointer<engine::gameobject::Actor> i_actor)
{
    ASSERT(i_actor);

    i_actor->GetPhysicsObject().Lock()->SetDefaultCollisionResponseEnabled(false);
    std::lock_guard<std::mutex> lock(enemy_bullet_pool_mutex_);
    i_actor->SetIsEnabled(false);
    enemy_bullet_pool_.push_back(i_actor);
}

void Game::Tick(float dt)
{
    if (game_state_ == GameStates::kGameStateRestart)
    {
        Reset();
        game_state_ = GameStates::kGameStateBegin;
        Init();
        return;
    }
    else if (game_state_ == GameStates::kGameStateRunning)
    {
        // remove all enemies that died this tick
        level_data_->level_.num_enemies_ = static_cast<uint8_t>(DestroyDeadLevelActors(level_data_->level_.enemies_));

        // remove all bricks
        level_data_->level_.num_bricks_ = static_cast<uint8_t>(DestroyDeadLevelActors(level_data_->level_.bricks_));

        // disable all enemy bullets that have left the screen
        for (const auto& bullet : enemy_bullet_pool_)
        {
            bullet->SetIsEnabled(bullet->GetIsEnabled() && bullet->GetGameObject()->GetPosition().y() < -Game::SCREEN_HEIGHT * 0.5f ? false : bullet->GetIsEnabled());
        }

        // check if the level has completed
        CheckLevelComplete();
    }
}

void Game::OnCollision(const engine::physics::CollisionPair& i_collision_pair)
{
    if (game_state_ != GameStates::kGameStateRunning)
    {
        return;
    }

    static const engine::data::HashedString enemy_type("Enemy");
    static const engine::data::HashedString brick_type("Brick");
    static const engine::data::HashedString bullet_type("Bullet");
    static const engine::data::HashedString player_type("Player");

    const engine::memory::SharedPointer<engine::gameobject::Actor> actor_a = i_collision_pair.object_a.Lock()->GetGameObject().Lock()->GetOwner().Lock();
    const engine::memory::SharedPointer<engine::gameobject::Actor> actor_b = i_collision_pair.object_b.Lock()->GetGameObject().Lock()->GetOwner().Lock();

    const engine::memory::SharedPointer<engine::gameobject::Actor> enemy = actor_a->GetType() == enemy_type ? actor_a : (actor_b->GetType() == enemy_type ? actor_b : nullptr);
    const engine::memory::SharedPointer<engine::gameobject::Actor> brick = actor_a->GetType() == brick_type ? actor_a : (actor_b->GetType() == brick_type ? actor_b : nullptr);
    const engine::memory::SharedPointer<engine::gameobject::Actor> bullet = actor_a->GetType() == bullet_type ? actor_a : (actor_b->GetType() == bullet_type ? actor_b : nullptr);
    const engine::memory::SharedPointer<engine::gameobject::Actor> player = actor_a->GetType() == player_type ? actor_a : (actor_b->GetType() == player_type ? actor_b : nullptr);

    if (enemy)
    {
        enemy->SetHasDied(true);
    }

    if (brick)
    {
        brick->SetHasDied(true);
    }

    if (bullet)
    {
        bullet->SetIsEnabled(false);
    }

    if (player)
    {
        game_state_ = GameStates::kLevelFailed;
        DoLevelFailedEffect();
    }
}

void Game::CreateLevel()
{
    level_data_ = new LevelData();

    char buf[512];
    sprintf_s(buf, "%s%02d.lua", game_data_.GetLevelLuaFilePath().GetString(), level_number_);

    level_data_->LoadLevel(engine::util::FileUtils::Get()->GetFileFromCache(engine::data::HashedString::Hash(buf)), std::bind(&Game::OnLevelLoadingComplete, this), std::bind(&Game::OnLevelLoadingFailed, this));
}

void Game::DestroyLevel()
{
    SAFE_DELETE(level_data_);
}

void Game::CreatePlayer()
{
    player_ = new Player(game_data_.GetPlayerLuaFileName());
}

void Game::DestroyPlayer()
{
    SAFE_DELETE(player_);
}

void Game::CreateEnemyBullets()
{
    static const engine::data::PooledString job_team("GameTeam");

    for (uint8_t i = 0; i < BULLETS_PER_ENEMY_IN_POOL * 3; ++i)
    {
        char buf[512];
        sprintf_s(buf, "%s_%02d.lua", game_data_.GetBulletLuaFilePath().GetString(), (i % BULLETS_PER_ENEMY_IN_POOL) + 1);

        engine::jobs::JobSystem::Get()->AddJob(new engine::jobs::CreateActorFromFileJob(engine::util::FileUtils::Get()->GetFileFromCache(engine::data::HashedString::Hash(buf)), 
            std::bind(&Game::OnEnemyBulletCreated, this, std::placeholders::_1)), job_team);
    }
}

void Game::DestroyEnemyBullets()
{
    enemy_bullet_pool_.clear();
}

size_t Game::DestroyDeadLevelActors(std::vector<engine::memory::SharedPointer<engine::gameobject::Actor>>& i_actors) const
{
    if (!i_actors.empty())
    {
        auto end_it = std::remove_if(i_actors.begin(), i_actors.end(), [](const engine::memory::SharedPointer<engine::gameobject::Actor>& i_actor) { return i_actor->GetHasDied(); });
        auto diff_it = i_actors.erase(end_it, i_actors.end());
        return size_t(diff_it - i_actors.begin());
    }
    return 0;
}

void Game::DoLevelCompletedEffect()
{
    if (game_state_ != GameStates::kLevelComplete)
    {
        return;
    }

    engine::time::Updater::Get()->RemoveTimerEvent(move_enemies_event_);
    engine::time::Updater::Get()->RemoveTimerEvent(fire_enemy_bullet_event_);

    player_->FlyOut();
    engine::time::Updater::Get()->AddTimerEvent(engine::events::TimerEvent::Create(std::bind(&Game::OnLevelCompletedEffectComplete, this), 1.0f, 0));
}

void Game::DoLevelFailedEffect()
{
    if (game_state_ != GameStates::kLevelFailed)
    {
        return;
    }

    player_->Die();
    engine::time::Updater::Get()->AddTimerEvent(engine::events::TimerEvent::Create(std::bind(&Game::OnLevelFailedEffectComplete, this), 1.0f, 0));
}

void Game::OnLevelCompletedEffectComplete()
{
    if (game_state_ != GameStates::kLevelComplete)
    {
        return;
    }

    ++level_number_;
    level_number_ = level_number_ > game_data_.GetNumberOfLevels() ? 1 : level_number_;
    game_state_ = GameStates::kGameStateRestart;
}

void Game::OnLevelFailedEffectComplete()
{
    if (game_state_ != GameStates::kLevelFailed)
    {
        return;
    }

    game_state_ = GameStates::kGameStateRestart;
}

} // namespace game
