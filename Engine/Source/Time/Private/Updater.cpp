#include "Time\Updater.h"

// engine includes
#include "Common\HelperMacros.h"
#include "Time\InterfaceTickable.h"

#include "Logger\Logger.h"

namespace engine {
namespace time {

// static member initialization
Updater* Updater::instance_ = nullptr;

Updater::Updater() : num_tickables_(0),
    num_timer_events_(0)
{}

Updater::~Updater()
{}

Updater* Updater::Create()
{
    if (!Updater::instance_)
    {
        Updater::instance_ = new Updater();
    }
    return Updater::instance_;
}

void Updater::Destroy()
{
    SAFE_DELETE(Updater::instance_);
}

void Updater::Run(float dt)
{
    // tick all tickables
    for (size_t i = 0; i < num_tickables_; ++i)
    {
        tickables_[i]->Tick(dt);
    }

    // tick all timer events
    {
        for (size_t i = 0; i < num_timer_events_; ++i)
        {
            timer_events_[i]->Tick(dt);
        }

        if (num_timer_events_ > 0)
        {
            std::lock_guard<std::mutex> lock(timer_events_mutex_);

            // remove all timer events that completed this tick
            auto logical_end = std::remove_if(timer_events_.begin(), timer_events_.end(), [](const engine::memory::SharedPointer<engine::events::TimerEvent>& i_timer_event) { return i_timer_event->complete_; });
            auto it = timer_events_.erase(logical_end, timer_events_.end());
            num_timer_events_ = it - timer_events_.begin();
        }
    }
}

} // namespace time
} // namespace engin