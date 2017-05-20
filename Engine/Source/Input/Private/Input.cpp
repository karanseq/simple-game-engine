#include "Input\Input.h"

// engine includes
#include "GLib.h"
#include "Events\EventDispatcher.h"

namespace engine {
namespace input {

bool StartUp()
{
    // register the key callback
    GLib::SetKeyStateChangeCallback(engine::input::KeyCallback);

    return true;
}

void Shutdown()
{}

void KeyCallback(unsigned int i_key_id, bool i_went_down)
{
    static engine::events::EventDispatcher* dispatcher = engine::events::EventDispatcher::Get();
    dispatcher->DispatchKeyboardEvent(i_key_id, i_went_down);
}

} // namespace input
} // namespace engine
