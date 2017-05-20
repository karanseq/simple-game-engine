#ifndef TIMER_EVENT_H_
#define TIMER_EVENT_H_

// library includes
#include <functional>
#include <stdint.h>

// engine includes
#include "Memory\SharedPointer.h"

namespace engine {

// forward declaration
namespace time {
class Updater;
}

namespace events {

class TimerEvent
{
public:
    static inline engine::memory::SharedPointer<TimerEvent> Create();
    static inline engine::memory::SharedPointer<TimerEvent> Create(const std::function<void(void)>& i_on_timer_elapsed, float i_duration_seconds, int32_t i_repeat);

    TimerEvent(const TimerEvent& i_copy);
    inline TimerEvent& operator=(const TimerEvent& i_other);

    inline void SetOnTimerElapsed(const std::function<void(void)>& i_on_timer_elapsed);
    inline float GetDuration() const;
    inline void SetDuration(float i_duration_seconds);
    inline int32_t GetRepeat() const;
    inline void SetRepeat(int32_t i_repeat);

private:
    TimerEvent();
    TimerEvent(const std::function<void(void)>& i_on_timer_elapsed, float i_duration_seconds, int32_t i_repeat);

    void Tick(float i_dt);

private:
    std::function<void(void)>           on_timer_elapsed_;
    float                               duration_ms_;
    float                               duration_elapsed_;
    int32_t                             repeat_;
    int32_t                             repeated_;
#ifdef BUILD_DEBUG
    uint32_t                            id_;
    static uint32_t                     id_counter_;
#endif
    bool                                complete_;

    friend class engine::time::Updater;

}; // class TimerEvent

} // namespace events
} // namespace engine

#include "TimerEvent-inl.h"

#endif // TIMER_EVENT_H_
