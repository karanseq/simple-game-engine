#ifndef EVENT_DISPATCHER_H_
#define EVENT_DISPATCHER_H_

// library includes
#include <mutex>
#include <vector>

// engine includes
#include "KeyboardEvent.h"
#include "Memory\SharedPointer.h"

namespace engine {
namespace events {

class EventDispatcher
{
private:
    EventDispatcher();
    ~EventDispatcher();
    static EventDispatcher* instance_;

    // disable copy & move constructors & assignment operators
    EventDispatcher(const EventDispatcher& i_copy) = delete;
    EventDispatcher& operator=(const EventDispatcher& i_other) = delete;
    EventDispatcher(EventDispatcher&& i_copy) = delete;
    EventDispatcher& operator=(EventDispatcher&& i_other) = delete;

public:
    static EventDispatcher* Create();
    static void Destroy();
    static inline EventDispatcher* Get() { return instance_; }

    void AddKeyboardEventListener(const engine::memory::SharedPointer<KeyboardEvent>& i_listener);
    void RemoveKeyboardEventListener(const engine::memory::SharedPointer<KeyboardEvent>& i_listener);
    void DispatchKeyboardEvent(unsigned int i_key_id, bool i_went_down) const;

private:
    std::vector<engine::memory::SharedPointer<engine::events::KeyboardEvent>>               keyboard_event_listeners_;
    mutable std::mutex                                                                      keyboard_event_listeners_mutex_;

}; // class EventDispatcher

} // namespace events
} // namespace engine

#endif // EVENT_DISPATCHER_H_
