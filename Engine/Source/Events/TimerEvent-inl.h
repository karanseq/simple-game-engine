#include "TimerEvent.h"

namespace engine {
namespace events {

inline engine::memory::SharedPointer<TimerEvent> TimerEvent::Create()
{
    return engine::memory::SharedPointer<TimerEvent>(new TimerEvent());
}

inline engine::memory::SharedPointer<TimerEvent> TimerEvent::Create(const std::function<void(void)>& i_on_timer_elapsed, float i_duration_seconds, int32_t i_repeat)
{
    return engine::memory::SharedPointer<TimerEvent>(new TimerEvent(i_on_timer_elapsed, i_duration_seconds, i_repeat));
}

inline TimerEvent& TimerEvent::operator=(const TimerEvent& i_other)
{
    // cheeck for self assignment
    if (this != &i_other)
    {
        on_timer_elapsed_ = i_other.on_timer_elapsed_;
        duration_ms_ = i_other.duration_ms_;
        duration_elapsed_ = i_other.duration_elapsed_;
        repeat_ = i_other.repeat_;
        repeated_ = i_other.repeated_;
    }
    return *this;
}

inline void TimerEvent::SetOnTimerElapsed(const std::function<void(void)>& i_on_timer_elapsed)
{
    on_timer_elapsed_ = i_on_timer_elapsed;
}

inline float TimerEvent::GetDuration() const
{
    return duration_ms_ * 0.001f;
}

inline void TimerEvent::SetDuration(float i_duration_seconds)
{
    duration_ms_ = i_duration_seconds * 1000.0f;
}

inline int32_t TimerEvent::GetRepeat() const
{
    return repeat_;
}

inline void TimerEvent::SetRepeat(int32_t i_repeat)
{
    repeat_ = i_repeat;
}

} // namespace events
} // namespace engine
