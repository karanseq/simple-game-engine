#include "KeyboardEvent.h"

namespace engine {
namespace input {

inline engine::memory::SharedPointer<KeyboardEvent> KeyboardEvent::Create()
{
	return engine::memory::SharedPointer<KeyboardEvent>(new KeyboardEvent());
}

inline engine::memory::SharedPointer<KeyboardEvent> KeyboardEvent::Create(const std::function<void(unsigned int)>& i_on_key_pressed, const std::function<void(unsigned int)>& i_on_key_released)
{
	return engine::memory::SharedPointer<KeyboardEvent>(new KeyboardEvent(i_on_key_pressed, i_on_key_released));
}

inline KeyboardEvent& KeyboardEvent::operator=(const KeyboardEvent& i_copy)
{
	// check for self assignment
	if (this != &i_copy)
	{
		on_key_pressed_ = i_copy.on_key_pressed_;
		on_key_released_ = i_copy.on_key_released_;
	}
	return *this;
}

inline void KeyboardEvent::SetOnKeyPressed(const std::function<void(unsigned int)>& i_on_key_pressed)
{
	on_key_pressed_ = i_on_key_pressed;
}

inline void KeyboardEvent::SetOnKeyReleased(const std::function<void(unsigned int)>& i_on_key_released)
{
	on_key_released_ = i_on_key_released;
}

} // namespace input
} // namespace engine