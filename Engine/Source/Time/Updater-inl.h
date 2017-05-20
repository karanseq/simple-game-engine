#include "Updater.h"

// library includes
#include <algorithm>

// engine includes
#include "Assert\Assert.h"
#include "Events\TimerEvent.h"
#include "Logger\Logger.h"

namespace engine {
namespace time {

inline Updater* Updater::Get()
{
    return Updater::instance_;
}

inline void Updater::AddTickable(InterfaceTickable* i_tickable)
{
    // validate input
    ASSERT(i_tickable);

    std::lock_guard<std::mutex> lock(tickables_mutex_);

    // check if this object already exists
    if (std::find(tickables_.begin(), tickables_.end(), i_tickable) != tickables_.end())
    {
        LOG_ERROR("Updater is already tracking this tickable!");
        return;
    }

    tickables_.push_back(i_tickable);
    ++num_tickables_;
}

inline void Updater::RemoveTickable(InterfaceTickable* i_tickable)
{
    // validate input
    ASSERT(i_tickable);
    // can't remove an object if there are none
    ASSERT(num_tickables_ > 0);

    std::lock_guard<std::mutex> lock(tickables_mutex_);

    // check if this object exists
    auto it = std::find(tickables_.begin(), tickables_.end(), i_tickable);
    if (it == tickables_.end())
    {
        LOG_ERROR("Updater could not find this tickable!");
        return;
    }
    tickables_.erase(it);
    --num_tickables_;
}

inline void Updater::AddTimerEvent(const engine::memory::SharedPointer<engine::events::TimerEvent>& i_timer_event)
{
    // validate input
    ASSERT(i_timer_event);

    std::lock_guard<std::mutex> lock(timer_events_mutex_);

    // check if this object already exists
    if (std::find(timer_events_.begin(), timer_events_.end(), i_timer_event) != timer_events_.end())
    {
        LOG_ERROR("Updater is already tracking this timer event!");
        return;
    }

    timer_events_.push_back(i_timer_event);
    ++num_timer_events_;
}

inline void Updater::RemoveTimerEvent(const engine::memory::SharedPointer<engine::events::TimerEvent>& i_timer_event)
{
    // validate input
    ASSERT(i_timer_event);
    // can't remove an object if there are none
    ASSERT(num_timer_events_ > 0);

    std::lock_guard<std::mutex> lock(timer_events_mutex_);

    // check if this object exists
    if (std::find(timer_events_.begin(), timer_events_.end(), i_timer_event) != timer_events_.end())
    {
        // mark it as complete so it gets removed in the subsequent tick
        i_timer_event->complete_ = true;
    }
}

inline void Updater::RemoveAllTimerEvents()
{
    std::lock_guard<std::mutex> lock(timer_events_mutex_);
    timer_events_.clear();
    num_timer_events_ = 0;
}

} // namespace time
} // namespace engine
