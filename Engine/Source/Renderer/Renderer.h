#ifndef RENDERER_H_
#define RENDERER_H_

// library includes
#include <mutex>
#include <vector>

// engine includes
#include "Memory\SharedPointer.h"
#include "RenderableObject.h"

// forward declarations
namespace GLib {
namespace Sprites {
    struct Sprite;
}
}
namespace engine {
namespace data {
    class PooledString;
}
}

namespace engine {
namespace render {

class Renderer
{
private:
    Renderer();
    ~Renderer();
    static Renderer* instance_;

    // disable copy constructor & copy assignment operator
    Renderer(const Renderer& i_copy) = delete;
    Renderer& operator=(const Renderer& i_copy) = delete;

public:
    static Renderer* Create();
    static void Destroy();
    static inline Renderer* Get();

    void Run(float i_dt);

    // create, add & remove renderer objects
    inline engine::memory::SharedPointer<RenderableObject> CreateRenderableObject(const engine::data::PooledString& i_file_name, const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object);
    inline void AddRenderableObject(const engine::memory::SharedPointer<RenderableObject>& i_renderable_object);
    inline void RemoveRenderableObject(const engine::memory::SharedPointer<RenderableObject>& i_renderable_object);

    static GLib::Sprites::Sprite* CreateSprite(const engine::data::PooledString& i_texture_file_name);

private:
    size_t                                                                          num_renderables_;
    std::vector<engine::memory::SharedPointer<RenderableObject>>                    renderables_;
    std::mutex                                                                      renderables_mutex_;

}; // class Renderer

} // namespace render
} // namespace engine

#include "Renderer-inl.h"

#endif // RENDERER_H_