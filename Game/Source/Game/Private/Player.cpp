#include "Game\Player.h"

// library includes
#include <functional>

// engine includes
#include "Assert\Assert.h"
#include "Data\PooledString.h"
#include "Jobs\CreateActorFromFileJob.h"
#include "Jobs\JobSystem.h"
#include "Events\EventDispatcher.h"
#include "Logger\Logger.h"
#include "Time\Updater.h"

// game includes
#include "Game\Game.h"
#include "Game\GameData.h"
#include "Game\LevelData.h"

namespace game {

// static member initialization
const float Player::DEFAULT_MASS = 50.0f;
const float Player::DEFAULT_FORCE = 5.0f;
const size_t Player::BULLET_POOL_SIZE = 5;

Player::Player(const engine::data::PooledString& i_lua_file_name) : 
    actor_(nullptr),
    keyboard_event_(engine::events::KeyboardEvent::Create()),
    fire_timer_event_(nullptr),
    fire_rate_(0.0f),
    is_left_pressed_(false),
    is_right_pressed_(false),
    actors_left_to_create_(0)
{
    CreateActors(i_lua_file_name);
}

Player::~Player()
{
    if (keyboard_event_)
    {
        engine::events::EventDispatcher::Get()->RemoveKeyboardEventListener(keyboard_event_);
        keyboard_event_ = nullptr;
    }

    engine::time::Updater::Get()->RemoveTickable(this);
}

void Player::Tick(float i_dt)
{
    // apply an appropriate force if any of the keys are pressed
    if (is_left_pressed_ || is_right_pressed_)
    {
        engine::math::Vec3D force(is_left_pressed_ ? -Player::DEFAULT_FORCE : (is_right_pressed_ ? Player::DEFAULT_FORCE : 0.0f), 0.0f);
        actor_->GetPhysicsObject().Lock()->ApplyImpulse(force);
    }

    for (const auto& bullet : bullet_pool_)
    {
        bullet->SetIsEnabled(bullet->GetIsEnabled() && bullet->GetGameObject()->GetPosition().y() > Game::SCREEN_HEIGHT * 0.5f ? false : bullet->GetIsEnabled());
    }

    // wrap around the screen
    engine::math::Vec3D position = actor_->GetGameObject()->GetPosition();
    position.x((position.x() < -Game::SCREEN_WIDTH / 2) ? Game::SCREEN_WIDTH / 2 : (position.x() > Game::SCREEN_WIDTH / 2 ? -Game::SCREEN_WIDTH / 2 : position.x()));
    actor_->GetGameObject()->SetPosition(position);
}

void Player::FlyOut()
{
    // stop taking key input
    engine::events::EventDispatcher::Get()->RemoveKeyboardEventListener(keyboard_event_);
    keyboard_event_ = nullptr;
    is_left_pressed_ = false;
    is_right_pressed_ = false;

    // stop firing
    engine::time::Updater::Get()->RemoveTimerEvent(fire_timer_event_);
    fire_timer_event_ = nullptr;

    // fly off screen
    const engine::memory::SharedPointer<engine::physics::PhysicsObject> physics_object = actor_->GetPhysicsObject().Lock();
    physics_object->SetIsActive(false);
    physics_object->SetIsActive(true);
    physics_object->SetDrag(0.0f);
    physics_object->ApplyImpulse(engine::math::Vec3D(0.0f, Player::DEFAULT_FORCE * 15.0f));
}

void Player::Die()
{
    // stop taking key input
    engine::events::EventDispatcher::Get()->RemoveKeyboardEventListener(keyboard_event_);
    keyboard_event_ = nullptr;

    // stop firing
    engine::time::Updater::Get()->RemoveTimerEvent(fire_timer_event_);
    fire_timer_event_ = nullptr;

    // fly off screen
    const engine::memory::SharedPointer<engine::physics::PhysicsObject> physics_object = actor_->GetPhysicsObject().Lock();
    physics_object->SetIsActive(false);
    physics_object->SetIsActive(true);
    physics_object->ApplyImpulse(engine::math::Vec3D(0.0f, -Player::DEFAULT_FORCE * 20.0f));

    // spin off screen
    engine::time::Updater::Get()->AddTimerEvent(engine::events::TimerEvent::Create([&]() {
        engine::math::Vec3D rotation = actor_->GetGameObject()->GetRotation();
        rotation.z(rotation.z() + 10);
        actor_->GetGameObject()->SetRotation(rotation);
    }, 0.05f, 25));
}

void Player::CreateActors(const engine::data::PooledString& i_lua_file_name)
{
    static const engine::data::PooledString job_team("GameTeam");

    engine::jobs::CreateActorFromFileJob* create_actor_job = new engine::jobs::CreateActorFromFileJob(i_lua_file_name, std::bind(&Player::OnActorCreated, this, std::placeholders::_1));
    engine::jobs::JobSystem::Get()->AddJob(create_actor_job, job_team);
    ++actors_left_to_create_;

    // generate bullet's file name
    char buf[512];
    sprintf_s(buf, "%s.lua", Game::GetInstance()->GetGameData().GetBulletLuaFilePath().GetString());

    const engine::util::FileUtils::FileData bullet_file_data = engine::util::FileUtils::Get()->GetFileFromCache(engine::data::HashedString::Hash(buf));
    for (size_t i = 0; i < Player::BULLET_POOL_SIZE; ++i)
    {
        create_actor_job = new engine::jobs::CreateActorFromFileJob(bullet_file_data, std::bind(&Player::OnActorCreated, this, std::placeholders::_1));
        engine::jobs::JobSystem::Get()->AddJob(create_actor_job, job_team);
        ++actors_left_to_create_;
    }
}

void Player::FireBullet()
{
    for (const auto& bullet : bullet_pool_)
    {
        if (bullet->GetIsEnabled() == false)
        {
            bullet->SetIsEnabled(true);
            bullet->GetGameObject()->SetPosition(actor_->GetGameObject()->GetPosition());
            bullet->GetPhysicsObject().Lock()->ApplyImpulse(engine::math::Vec3D(0.0f, Game::BULLET_IMPULSE));
            break;
        }
    }
}

void Player::OnActorCreated(engine::memory::SharedPointer<engine::gameobject::Actor> i_actor)
{
    ASSERT(i_actor);

    if (i_actor->GetType() == "Player")
    {
        actor_ = i_actor;
        --actors_left_to_create_;

        // fly into screen
        actor_->GetPhysicsObject().Lock()->ApplyImpulse(engine::math::Vec3D(0.0f, Player::DEFAULT_FORCE * 17.5f));
    }
    else if (i_actor->GetType() == "Bullet")
    {
        i_actor->GetPhysicsObject().Lock()->SetDefaultCollisionResponseEnabled(false);
        std::lock_guard<std::mutex> lock(bullet_pool_mutex_);
        i_actor->SetIsEnabled(false);
        bullet_pool_.push_back(i_actor);
        --actors_left_to_create_;
    }
    else
    {
        LOG_ERROR("%s found unknown type for actor %s!", __FUNCTION__, i_actor->GetName().GetString());
    }

    if (actors_left_to_create_ <= 0)
    {
        OnAllActorsCreated();
    }
}

void Player::OnAllActorsCreated()
{
    // register for update events
    engine::time::Updater::Get()->AddTickable(this);

    // register for key events
    ASSERT(keyboard_event_);
    keyboard_event_->SetOnKeyPressed(std::bind(&Player::OnKeyPressed, this, std::placeholders::_1));
    keyboard_event_->SetOnKeyReleased(std::bind(&Player::OnKeyReleased, this, std::placeholders::_1));
    engine::events::EventDispatcher::Get()->AddKeyboardEventListener(keyboard_event_);

    fire_rate_ = Game::GetInstance()->GetLevelData()->GetLevel().player_fire_rate_;
    fire_timer_event_ = engine::events::TimerEvent::Create(std::bind(&Player::FireBullet, this), fire_rate_, -1);
    engine::time::Updater::Get()->AddTimerEvent(fire_timer_event_);
}

void Player::OnKeyPressed(unsigned int i_key_id)
{
    is_left_pressed_ = is_left_pressed_ || i_key_id == 37;
    is_right_pressed_ = is_right_pressed_ || i_key_id == 39;
}

void Player::OnKeyReleased(unsigned int i_key_id)
{
    is_left_pressed_ = is_left_pressed_ ? !(i_key_id == 37) : is_left_pressed_;
    is_right_pressed_ = is_right_pressed_ ? !(i_key_id == 39) : is_right_pressed_;
}

} // namespace monsterchase
