#include "Events\TimerEvent.h"

namespace engine {
namespace events {

#ifdef BUILD_DEBUG
uint32_t TimerEvent::id_counter_ = 0;
#endif

TimerEvent::TimerEvent() : on_timer_elapsed_(nullptr),
    duration_ms_(0.0f),
    duration_elapsed_(0.0f),
    repeat_(0),
    repeated_(0),
#ifdef BUILD_DEBUG
    id_(TimerEvent::id_counter_++),
#endif
    complete_(false)
{}

TimerEvent::TimerEvent(const std::function<void(void)>& i_on_timer_elapsed, float i_duration_seconds, int32_t i_repeat) : on_timer_elapsed_(i_on_timer_elapsed),
    duration_ms_(i_duration_seconds * 1000.0f),
    duration_elapsed_(0.0f),
    repeat_(i_repeat),
    repeated_(0),
#ifdef BUILD_DEBUG
    id_(TimerEvent::id_counter_++),
#endif
    complete_(false)
{}

TimerEvent::TimerEvent(const TimerEvent& i_copy) : on_timer_elapsed_(i_copy.on_timer_elapsed_),
    duration_ms_(i_copy.duration_ms_),
    duration_elapsed_(i_copy.duration_elapsed_),
    repeat_(i_copy.repeat_),
    repeated_(i_copy.repeated_),
#ifdef BUILD_DEBUG
    id_(i_copy.id_),
#endif
    complete_(i_copy.complete_)
{}

void TimerEvent::Tick(float i_dt)
{
    if (complete_)
    {
        return;
    }

    duration_elapsed_ += i_dt;
    if (duration_elapsed_ >= duration_ms_)
    {
        on_timer_elapsed_();

        if (repeat_ < 0 || ++repeated_ < repeat_)
        {
            duration_elapsed_ = 0.0f;
        }
        else
        {
            complete_ = true;
        }
    }
}

} // namespace events
} // namespace engine
