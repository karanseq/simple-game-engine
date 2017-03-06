#ifndef INPUT_H_
#define INPUT_H_

namespace engine {
namespace input {

bool StartUp();

void Shutdown();

void KeyCallback(unsigned int i_key_id, bool i_went_down);

} // namespace input
} // namespace engine

#endif // INPUT_H_
