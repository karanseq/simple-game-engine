#include "Input\KeyboardEventDispatcher.h"

// engine includes
#include "Common\HelperMacros.h"

namespace engine {
namespace input {

KeyboardEventDispatcher* KeyboardEventDispatcher::instance_ = nullptr;

KeyboardEventDispatcher::KeyboardEventDispatcher() : num_listeners_(0)
{}

KeyboardEventDispatcher::~KeyboardEventDispatcher()
{
	listeners_.clear();
	num_listeners_ = 0;
}

KeyboardEventDispatcher* KeyboardEventDispatcher::Create()
{
	if (KeyboardEventDispatcher::instance_ == nullptr)
	{
		KeyboardEventDispatcher::instance_ = new KeyboardEventDispatcher();
	}
	return KeyboardEventDispatcher::instance_;
}

void KeyboardEventDispatcher::Destroy()
{
	SAFE_DELETE(KeyboardEventDispatcher::instance_);
}

} // namespace input
} // namespace engine