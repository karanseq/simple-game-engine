#ifndef TIMER_UTIL_H_
#define TIMER_UTIL_H_

// library includes
#include <stdint.h>

namespace engine {
namespace time {

class TimerUtil
{
private:
	TimerUtil() = delete;
	~TimerUtil() = delete;

	TimerUtil(const TimerUtil& i_copy) = delete;
	TimerUtil operator=(const TimerUtil& i_copy) = delete;

public:
	static float GetLastFrameTime_ms();
	static float CalculateLastFrameTime_ms();

	static void CustomSleep(uint32_t i_milli_seconds);

	static const float					DESIRED_FPS;
	static const float					DESIRED_FRAMETIME_MS;
	static const float					MAX_FRAMETIME_MS;

private:
	// TODO: These functions may only be required for win32
	static double GetCounter();
	static double GetFrequency();

	static double						pc_frequency_;
	static double						last_frame_start_tick_;
	static float						last_frame_time_ms_;

}; // class TimerUtil

} // namespace time
} // namespace engine

#endif // TIMER_UTIL_H_
