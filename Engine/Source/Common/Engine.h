#ifndef ENGINE_H_
#define ENGINE_H_

namespace engine {

// TODO: Find a better place for this
// Currently it is publicly accessible
static bool quit_requested_;

bool StartUp();

void Run();

void Shutdown();

// TODO: Find a better place for this
void RequestQuit();

} // namespace engine

#endif // ENGINE_H_
