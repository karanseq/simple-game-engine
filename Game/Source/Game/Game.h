#ifndef GAME_H_
#define GAME_H_

// library includes
#include <mutex>
#include <stdint.h>
#include <vector>

// engine includes
#include "Events\KeyboardEvent.h"
#include "Events\TimerEvent.h"
#include "GameObject\Actor.h"
#include "Memory\SharedPointer.h"
#include "Memory\WeakPointer.h"
#include "Physics\Collider.h"
#include "Time\InterfaceTickable.h"
#include "Util\FileUtils.h"

// game includes
#include "Game\GameData.h"
#include "Game\GameTypes.h"

namespace game {

// forward declarations
class LevelData;
class Player;

// global life-cycle functions
bool StartUp();
void Shutdown();

class Game : public engine::time::InterfaceTickable, public engine::physics::InterfaceCollisionListener
{
public:
    static Game* Create();
    static inline Game* GetInstance()                                               { return instance_; }
    static void Destroy();

    // game life-cycle functions
    bool Init();
    void Reset();
    void CheckLevelComplete();

    // implement InterfaceTickable
    virtual void Tick(float dt) override;

    // implement InterfaceCollisionListener
    virtual void OnCollision(const engine::physics::CollisionPair& i_collision_pair) override;

    // game elements
    void CreateLevel();
    void DestroyLevel();
    void CreatePlayer();
    void DestroyPlayer();
    void CreateEnemyBullets();
    void DestroyEnemyBullets();
    size_t DestroyDeadLevelActors(std::vector<engine::memory::SharedPointer<engine::gameobject::Actor>>& i_actors) const;

    // effects
    void DoLevelFailedEffect();
    void OnLevelFailedEffectComplete();
    void DoLevelCompletedEffect();
    void OnLevelCompletedEffectComplete();

    // job events
    void OnAssetLoadingComplete();
    void OnAssetLoadingFailed();
    void OnLevelLoadingComplete();
    void OnLevelLoadingFailed();
    void OnEnemyBulletCreated(const engine::memory::SharedPointer<engine::gameobject::Actor> i_actor);

    // key event
    void OnKeyPressed(unsigned int i_key_id);

    // timer events
    void OnMoveEnemiesTimerElapsed();
    void OnFireEnemyBulletTimerElapsed();

    inline GameStates GetState() const                                              { return game_state_; }
    inline const GameData& GetGameData() const                                      { return game_data_; }
    inline const LevelData* GetLevelData() const                                    { return level_data_; }

    // game constants
    static const uint16_t                                                           SCREEN_WIDTH = 1280;
    static const uint16_t                                                           SCREEN_HEIGHT = 800;
    static const uint8_t                                                            BULLETS_PER_ENEMY_IN_POOL = 3;
    static const float                                                              BULLET_IMPULSE;

private:
    Game();
    ~Game();
    static Game* instance_;

    // disable copy constructor & copy assignment operator
    Game(const Game& i_copy) = delete;
    Game& operator=(const Game& i_copy) = delete;

    // game state & data
    GameStates                                                                      game_state_;
    GameData                                                                        game_data_;
    bool                                                                            init_successful_;

    // game elements
    uint8_t                                                                         level_number_;
    LevelData*                                                                      level_data_;
    Player*                                                                         player_;
    std::vector<engine::memory::SharedPointer<engine::gameobject::Actor>>           enemy_bullet_pool_;
    std::mutex                                                                      enemy_bullet_pool_mutex_;
    uint8_t                                                                         enemy_moves_;

    // events
    engine::memory::SharedPointer<engine::events::KeyboardEvent>                    keyboard_event_;
    engine::memory::SharedPointer<engine::events::TimerEvent>                       move_enemies_event_;
    engine::memory::SharedPointer<engine::events::TimerEvent>                       fire_enemy_bullet_event_;


}; // class Game

} // namespace game

#endif // GAME_H_