#ifndef KEYBOARD_EVENT_H_
#define KEYBOARD_EVENT_H_

// library includes
#include <functional>

// engine includes
#include "Memory\SharedPointer.h"

namespace engine {
namespace input {

class KeyboardEvent
{
public:
	static inline engine::memory::SharedPointer<KeyboardEvent> Create();
	static inline engine::memory::SharedPointer<KeyboardEvent> Create(const std::function<void(unsigned int)>& i_on_key_pressed, const std::function<void(unsigned int)>& i_on_key_released);
	~KeyboardEvent();

	KeyboardEvent(const KeyboardEvent& i_copy);
	inline KeyboardEvent& operator=(const KeyboardEvent& i_copy);

	inline void SetOnKeyPressed(const std::function<void(unsigned int)>& i_on_key_pressed);
	inline void SetOnKeyReleased(const std::function<void(unsigned int)>& i_on_key_released);

private:
	KeyboardEvent();
	KeyboardEvent(const std::function<void(unsigned int)>& i_on_key_pressed, const std::function<void(unsigned int)>& i_on_key_released);

private:
	std::function<void(unsigned int)> on_key_pressed_;
	std::function<void(unsigned int)> on_key_released_;

	friend class KeyboardEventDispatcher;

}; // class KeyboardEvent

} // namespace input
} // namespace engine

#include "KeyboardEvent-inl.h"

#endif // KEYBOARD_EVENT_H_
