#ifndef LEVEL_DATA_H_
#define LEVEL_DATA_H_

// library includes
#include <functional>
#include <mutex>
#include <vector>

// engine includes
#include "Data\PooledString.h"
#include "GameObject\Actor.h"
#include "Memory\SharedPointer.h"
#include "Util\FileUtils.h"

namespace game {

struct Level
{
    std::vector<engine::memory::SharedPointer<engine::gameobject::Actor>>       enemies_;
    std::vector<engine::memory::SharedPointer<engine::gameobject::Actor>>       bricks_;
    uint8_t                                                                     num_enemies_;
    uint8_t                                                                     num_bricks_;
    float                                                                       player_fire_rate_;
    float                                                                       enemy_fire_rate_;
    float                                                                       enemy_move_interval_;
    float                                                                       enemy_move_impulse_;

    Level() : 
        num_enemies_(0),
        num_bricks_(0),
        player_fire_rate_(0.0f),
        enemy_fire_rate_(0.0f),
        enemy_move_interval_(0.0f),
        enemy_move_impulse_(0.0f)
    {}

}; // struct Level

class LevelData
{
public:
    LevelData();
    ~LevelData();

    void LoadLevel(const engine::util::FileUtils::FileData& i_level_file_data, const std::function<void(void)>& i_on_loading_complete, const std::function<void(void)>& i_on_loading_failed);
    inline const Level& GetLevel() const { return level_; }

private:
    // disable copy constructor & copy assignment operator
    LevelData(const LevelData&) = delete;
    LevelData& operator=(const LevelData&) = delete;

    void OnActorCreated(engine::memory::SharedPointer<engine::gameobject::Actor>);

private:
    std::function<void(void)>                           on_loading_complete_;
    std::function<void(void)>                           on_loading_failed_;
    Level                                               level_;
    std::mutex                                          actors_created_mutex_;
    size_t                                              actors_left_to_create_;
    size_t                                              jobs_left_to_finish_;

    friend class Game;

}; // class LevelData

} // namespace game

#endif // LEVEL_DATA_H_
