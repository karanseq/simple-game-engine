#include "Input\KeyboardEvent.h"

namespace engine {
namespace input {

KeyboardEvent::KeyboardEvent() : on_key_pressed_(nullptr),
	on_key_released_(nullptr)
{}

KeyboardEvent::KeyboardEvent(const std::function<void(unsigned int)>& i_on_key_pressed, const std::function<void(unsigned int)>& i_on_key_released) : on_key_pressed_(i_on_key_pressed),
	on_key_released_(i_on_key_released)
{}

KeyboardEvent::~KeyboardEvent()
{}

KeyboardEvent::KeyboardEvent(const KeyboardEvent& i_copy) : on_key_pressed_(i_copy.on_key_pressed_),
	on_key_released_(i_copy.on_key_released_)
{}

} // namespace input
} // namespace engine
