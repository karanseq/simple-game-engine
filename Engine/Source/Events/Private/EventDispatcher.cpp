#include "Events\EventDispatcher.h"

// engine includes
#include "Assert\Assert.h"
#include "Common\HelperMacros.h"
#include "Logger\Logger.h"

namespace engine {
namespace events {

EventDispatcher* EventDispatcher::instance_ = nullptr;

EventDispatcher::EventDispatcher()
{}

EventDispatcher::~EventDispatcher()
{
    keyboard_event_listeners_.clear();
}

EventDispatcher* EventDispatcher::Create()
{
    if (EventDispatcher::instance_ == nullptr)
    {
        EventDispatcher::instance_ = new EventDispatcher();
    }
    return EventDispatcher::instance_;
}

void EventDispatcher::Destroy()
{
    SAFE_DELETE(EventDispatcher::instance_);
}

void EventDispatcher::AddKeyboardEventListener(const engine::memory::SharedPointer<KeyboardEvent>& i_listener)
{
    // validate input
    ASSERT(i_listener);

    std::lock_guard<std::mutex> lock(keyboard_event_listeners_mutex_);

    // check if listener is already registered
    if (std::find(keyboard_event_listeners_.begin(), keyboard_event_listeners_.end(), i_listener) != keyboard_event_listeners_.end())
    {
        LOG_ERROR("%s is already tracking this keyboard event listener!", __FUNCTION__);
        return;
    }

    keyboard_event_listeners_.push_back(i_listener);
}

void EventDispatcher::RemoveKeyboardEventListener(const engine::memory::SharedPointer<KeyboardEvent>& i_listener)
{
    // validate input
    ASSERT(i_listener);

    std::lock_guard<std::mutex> lock(keyboard_event_listeners_mutex_);

    // check if this listener is registered
    const auto& it = std::find(keyboard_event_listeners_.begin(), keyboard_event_listeners_.end(), i_listener);
    if (it == keyboard_event_listeners_.end())
    {
        LOG_ERROR("%s could not find this listener!", __FUNCTION__);
        return;
    }

    keyboard_event_listeners_.erase(it);
}

void EventDispatcher::DispatchKeyboardEvent(unsigned int i_key_id, bool i_went_down) const
{
    std::lock_guard<std::mutex> lock(keyboard_event_listeners_mutex_);

    for (const auto& i : keyboard_event_listeners_)
    {
        // get the appropriate function
        auto callback = i_went_down ? i->on_key_pressed_ : i->on_key_released_;
        // listeners may not want both events so only send for valid ones
        if (callback)
        {
            callback(i_key_id);
        }
    }
}

} // namespace events
} // namespace engine
