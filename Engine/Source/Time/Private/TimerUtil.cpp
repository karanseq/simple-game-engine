#include "Time\TimerUtil.h"

namespace engine {
namespace time {

const float TimerUtil::DESIRED_FPS = 60.0f;
const float TimerUtil::DESIRED_FRAMETIME_MS = 1000.0f / TimerUtil::DESIRED_FPS;
const float TimerUtil::MAX_FRAMETIME_MS = 2 * TimerUtil::DESIRED_FRAMETIME_MS;

double TimerUtil::pc_frequency_ = 0;
double TimerUtil::last_frame_start_tick_ = 0;
float TimerUtil::last_frame_time_ms_ = 0.0f;

double TimerUtil::GetTick()
{
    return TimerUtil::last_frame_start_tick_;
}

float TimerUtil::GetLastFrameTime_ms()
{
#if defined(CONSTANT_FRAMETIME)
    return DESIRED_FRAMETIME_MS;
#elif defined(CLAMP_FRAMETIME)
    if (last_frame_time_ms_ > MAX_FRAMETIME_MS)
    {
        return MAX_FRAMETIME_MS;
    }
    else
    {
        return last_frame_time_ms_;
    }
#else
    return last_frame_time_ms_;
#endif
}

} // namespace time
} // namespace engine
