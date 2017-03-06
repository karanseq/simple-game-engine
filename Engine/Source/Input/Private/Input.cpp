#include "Input\Input.h"

// engine includes
#include "GLib.h"
#include "Input\KeyboardEventDispatcher.h"

namespace engine {
namespace input {

bool StartUp()
{
	if (!KeyboardEventDispatcher::Create())
	{
		return false;
	}

	// register the key callback
	GLib::SetKeyStateChangeCallback(engine::input::KeyCallback);

	return true;
}

void Shutdown()
{
	KeyboardEventDispatcher::Destroy();
}

void KeyCallback(unsigned int i_key_id, bool i_went_down)
{
	static KeyboardEventDispatcher* dispatcher = KeyboardEventDispatcher::Get();
	dispatcher->DispatchEvent(i_key_id, i_went_down);
}

} // namespace input
} // namespace engine
