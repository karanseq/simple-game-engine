#ifndef GAME_UTILS_H_
#define GAME_UTILS_H_

// library includes
#include <stdint.h>
#include <stdlib.h>
#include <string>

// engine includes
#include "Math\Vec3D.h"

// game includes
#include "Game\GameTypes.h"

namespace game {

class GameUtils
{
private:
    GameUtils() = delete;
    GameUtils(const GameUtils& i_copy) = delete;
    GameUtils& operator=(const GameUtils& i_rhs) = delete;
    ~GameUtils() = delete;

public:
    inline static engine::math::Vec3D GetRandomVec3D(size_t i_max_x = 0, size_t i_max_y = 0, size_t i_max_z = 0) 
    { 
        return engine::math::Vec3D(i_max_x > 0 ? static_cast<float>(rand() % i_max_x) : 0.0f, 
                                   i_max_y > 0 ? static_cast<float>(rand() % i_max_y) : 0.0f, 
                                   i_max_z > 0 ? static_cast<float>(rand() % i_max_z) : 0.0f);
    }

}; // class GameUtils

} // namespace monsterchase

#endif // GAME_UTILS_H_
