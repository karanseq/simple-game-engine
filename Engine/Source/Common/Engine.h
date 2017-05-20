#ifndef ENGINE_H_
#define ENGINE_H_

// library includes
#include <Windows.h>

namespace engine {

bool StartUp(HINSTANCE i_h_instance, int i_n_cmd_show, const char* i_window_name, unsigned int i_window_width, unsigned int i_window_height);
void Run();

void Pause();
void Resume();
bool IsPaused();

void InitiateShutdown();
void Shutdown();

} // namespace engine

#endif // ENGINE_H_
