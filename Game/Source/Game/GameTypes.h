#ifndef GAME_TYPES_H_
#define GAME_TYPES_H_

namespace game {

enum class GameStates : uint8_t
{
    kGameStateNone = 0,
    kGameStateLoading,
    kGameStateBegin,
    kGameStateRunning,
    kGameStatePaused,
    kLevelFailed,
    kLevelComplete,
    kGameStateRestart,
    kGameStateQuit
};

} // namespace monsterchase

#endif // GAME_TYPES_H_
