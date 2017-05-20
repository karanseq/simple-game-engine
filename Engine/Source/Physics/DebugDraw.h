#ifdef ENABLE_DEBUG_DRAW

#ifndef DEBUG_DRAW_H_
#define DEBUG_DRAW_H_

// engine includes
#include "Memory\SharedPointer.h"
#include "Renderer\RenderableObject.h"

// forward declarations
namespace engine {
namespace math {
    struct AABB;
    class Mat44;
}
}

namespace engine {
namespace physics {

class DebugDrawData
{
public:
    DebugDrawData(uint8_t i_r, uint8_t i_g, uint8_t i_b, uint8_t i_a);
    ~DebugDrawData();

    void Update(float i_dt, const engine::math::AABB& i_aabb, const engine::math::Mat44& i_transformation);

    // constants
    static const char* POINT_SPRITE_FILE_NAME;

private:
    // 0 -> +X  |  1 -> +Y  |  2 -> -X  |  3 -> -Y
    engine::memory::SharedPointer<engine::render::RenderableObject> axes[4];
    // 0 -> +X  |  1 -> +Y  |  2 -> -X  |  3 -> -Y
    engine::memory::SharedPointer<engine::render::RenderableObject> extents[4];
};

} // namespace physics
} // namespace engine

#endif // DEBUG_DRAW_H_

#endif // ENABLE_DEBUG_DRAW
