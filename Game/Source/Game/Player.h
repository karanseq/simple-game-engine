#ifndef PLAYER_H_
#define PLAYER_H_

// library includes
#include <mutex>
#include <vector>

// engine includes
#include "GameObject\Actor.h"
#include "Events\KeyboardEvent.h"
#include "Events\TimerEvent.h"
#include "Memory\SharedPointer.h"
#include "Time\InterfaceTickable.h"

// forward declaration
namespace engine {
namespace data {
    class PooledString;
}
}

namespace game {

class Player : public engine::time::InterfaceTickable
{
public:
    Player(const engine::data::PooledString& i_lua_file_name);
    ~Player();

    // disable copy & move constructors & assignment operators
    Player(const Player& i_copy) = delete;
    Player& operator=(const Player& i_copy) = delete;
    Player(Player&& i_copy) = delete;
    Player& operator=(Player&& i_copy) = delete;

    // implement interface tickable
    void Tick(float i_dt) override;

    void FlyOut();
    void Die();

    // accessors
    inline const engine::memory::WeakPointer<engine::gameobject::Actor> GetActor() const
    { 
        return actor_;
    }

    inline const std::vector<engine::memory::SharedPointer<engine::gameobject::Actor>>& GetBulletPool() const
    {
        return bullet_pool_;
    }

private:
    void CreateActors(const engine::data::PooledString& i_lua_file_name);
    void FireBullet();

    // events
    void OnActorCreated(engine::memory::SharedPointer<engine::gameobject::Actor>);
    void OnAllActorsCreated();
    void OnKeyPressed(unsigned int i_key_id);
    void OnKeyReleased(unsigned int i_key_id);

public:
    // constants
    static const float                                                              DEFAULT_MASS;
    static const float                                                              DEFAULT_FORCE;
    static const size_t                                                             BULLET_POOL_SIZE;

private:
    engine::memory::SharedPointer<engine::gameobject::Actor>                        actor_;
    engine::memory::SharedPointer<engine::events::KeyboardEvent>                    keyboard_event_;
    engine::memory::SharedPointer<engine::events::TimerEvent>                       fire_timer_event_;

    std::vector<engine::memory::SharedPointer<engine::gameobject::Actor>>           bullet_pool_;
    mutable std::mutex                                                              bullet_pool_mutex_;

    float                                                                           fire_rate_;
    bool                                                                            is_left_pressed_;
    bool                                                                            is_right_pressed_;
    uint8_t                                                                         actors_left_to_create_;

}; // class Player

} // namespace game

#endif // PLAYER_H_
