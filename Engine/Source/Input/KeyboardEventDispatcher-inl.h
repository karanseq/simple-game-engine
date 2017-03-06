#include "KeyboardEventDispatcher.h"

// library includes
#include <algorithm>

// engine includes
#include "Assert\Assert.h"
#include "Logger\Logger.h"

namespace engine {
namespace input {

inline KeyboardEventDispatcher* KeyboardEventDispatcher::Get()
{
	return KeyboardEventDispatcher::instance_;
}

inline void KeyboardEventDispatcher::AddListener(const engine::memory::SharedPointer<KeyboardEvent>& i_listener)
{
	// validate input
	ASSERT(i_listener);

	// check if this listener is already registered
	if (std::find(listeners_.begin(), listeners_.end(), i_listener) != listeners_.end())
	{
		LOG_ERROR("KeyboardEventDispatcher is already tracking this listener!");
		return;
	}

	listeners_.push_back(i_listener);
	++num_listeners_;
}

inline void KeyboardEventDispatcher::RemoveListener(const engine::memory::SharedPointer<KeyboardEvent>& i_listener)
{
	// validate input
	ASSERT(i_listener);
	// can't remove an object if there are none
	ASSERT(num_listeners_ > 0);

	// check if this object exists
	auto it = std::find(listeners_.begin(), listeners_.end(), i_listener);
	if (it == listeners_.end())
	{
		LOG_ERROR("KeyboardEventDispatcher could not find this listener!");
		return;
	}

	listeners_.erase(it);
	--num_listeners_;
}

inline void KeyboardEventDispatcher::DispatchEvent(unsigned int i_key_id, bool i_went_down) const
{
	for (auto i : listeners_)
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

} // namespace input
} // namespace engine
