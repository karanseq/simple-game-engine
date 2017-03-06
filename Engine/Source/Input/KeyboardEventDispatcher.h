#ifndef KEYBOARD_EVENT_DISPATCHER_H_
#define KEYBOARD_EVENT_DISPATCHER_H_

// library includes
#include <vector>

// engine includes
#include "KeyboardEvent.h"
#include "Memory\SharedPointer.h"

namespace engine {
namespace input {

class KeyboardEventDispatcher
{
private:
	KeyboardEventDispatcher();
	~KeyboardEventDispatcher();
	static KeyboardEventDispatcher* instance_;

	// disable copy & move constructors & assignment operators
	KeyboardEventDispatcher(const KeyboardEventDispatcher& i_copy) = delete;
	KeyboardEventDispatcher& operator=(const KeyboardEventDispatcher& i_copy) = delete;
	KeyboardEventDispatcher(KeyboardEventDispatcher&& i_copy) = delete;
	KeyboardEventDispatcher& operator=(KeyboardEventDispatcher&& i_copy) = delete;

public:
	static KeyboardEventDispatcher* Create();
	static void Destroy();
	static inline KeyboardEventDispatcher* Get();

	inline void AddListener(const engine::memory::SharedPointer<KeyboardEvent>& i_listener);
	inline void RemoveListener(const engine::memory::SharedPointer<KeyboardEvent>& i_listener);

	inline void DispatchEvent(unsigned int i_key_id, bool i_went_down) const;

private:
	std::vector<engine::memory::SharedPointer<KeyboardEvent>>					listeners_;
	size_t																		num_listeners_;

}; // class KeyboardEventDispatcher

} // namespace input
} // namespace engine

#include "KeyboardEventDispatcher-inl.h"

#endif // KEYBOARD_EVENT_DISPATCHER_H_
