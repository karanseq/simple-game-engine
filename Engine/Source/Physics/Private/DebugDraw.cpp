#ifdef ENABLE_DEBUG_DRAW

#include "Physics\DebugDraw.h"

// engine includes
#include "Math\AABB.h"
#include "Math\Mat44.h"
#include "Renderer\Renderer.h"

namespace engine {
namespace physics {

// static member initialization
const char* DebugDrawData::POINT_SPRITE_FILE_NAME = "Data\\4x4_red.dds";

DebugDrawData::DebugDrawData(uint8_t i_r, uint8_t i_g, uint8_t i_b, uint8_t i_a)
{
    /*axes[0] = engine::render::RenderableObject::Create(engine::render::Renderer::CreateSprite(DebugDrawData::POINT_SPRITE_FILE_NAME, i_r, i_g, i_b, i_a));
    engine::render::Renderer::Get()->AddRenderableObject(axes[0]);
    axes[1] = engine::render::RenderableObject::Create(engine::render::Renderer::CreateSprite(DebugDrawData::POINT_SPRITE_FILE_NAME, i_r, i_g, i_b, i_a));
    engine::render::Renderer::Get()->AddRenderableObject(axes[1]);
    axes[2] = engine::render::RenderableObject::Create(engine::render::Renderer::CreateSprite(DebugDrawData::POINT_SPRITE_FILE_NAME, i_r, i_g, i_b, i_a));
    engine::render::Renderer::Get()->AddRenderableObject(axes[2]);
    axes[3] = engine::render::RenderableObject::Create(engine::render::Renderer::CreateSprite(DebugDrawData::POINT_SPRITE_FILE_NAME, i_r, i_g, i_b, i_a));
    engine::render::Renderer::Get()->AddRenderableObject(axes[3]);*/

    extents[0] = engine::render::RenderableObject::Create(engine::render::Renderer::CreateSprite(DebugDrawData::POINT_SPRITE_FILE_NAME, i_r, i_g, i_b, i_a));
    engine::render::Renderer::Get()->AddRenderableObject(extents[0]);
    extents[1] = engine::render::RenderableObject::Create(engine::render::Renderer::CreateSprite(DebugDrawData::POINT_SPRITE_FILE_NAME, i_r, i_g, i_b, i_a));
    engine::render::Renderer::Get()->AddRenderableObject(extents[1]);
    extents[2] = engine::render::RenderableObject::Create(engine::render::Renderer::CreateSprite(DebugDrawData::POINT_SPRITE_FILE_NAME, i_r, i_g, i_b, i_a));
    engine::render::Renderer::Get()->AddRenderableObject(extents[2]);
    extents[3] = engine::render::RenderableObject::Create(engine::render::Renderer::CreateSprite(DebugDrawData::POINT_SPRITE_FILE_NAME, i_r, i_g, i_b, i_a));
    engine::render::Renderer::Get()->AddRenderableObject(extents[3]);
}

DebugDrawData::~DebugDrawData()
{
    /*engine::render::Renderer::Get()->RemoveRenderableObject(axes[0]);
    engine::render::Renderer::Get()->RemoveRenderableObject(axes[1]);
    engine::render::Renderer::Get()->RemoveRenderableObject(axes[2]);
    engine::render::Renderer::Get()->RemoveRenderableObject(axes[3]);*/

    engine::render::Renderer::Get()->RemoveRenderableObject(extents[0]);
    engine::render::Renderer::Get()->RemoveRenderableObject(extents[1]);
    engine::render::Renderer::Get()->RemoveRenderableObject(extents[2]);
    engine::render::Renderer::Get()->RemoveRenderableObject(extents[3]);
}

void DebugDrawData::Update(float i_dt, const engine::math::AABB& i_aabb, const engine::math::Mat44& i_transformation)
{
    // transform the extents to world
    const engine::math::Vec4D extent_positive_x = i_transformation * engine::math::Vec4D(i_aabb.extents.x(), 0.0f, 0.0f, 1.0f);
    const engine::math::Vec4D extent_negative_x = i_transformation * engine::math::Vec4D(-i_aabb.extents.x(), 0.0f, 0.0f, 1.0f);
    const engine::math::Vec4D extent_positive_y = i_transformation * engine::math::Vec4D(0.0f, i_aabb.extents.y(), 0.0f, 1.0f);
    const engine::math::Vec4D extent_negative_y = i_transformation * engine::math::Vec4D(0.0f, -i_aabb.extents.y(), 0.0f, 1.0f);

    // update the positions of the extents renderables
    extents[0]->SetPosition(engine::math::Vec2D(extent_positive_x.x(), extent_positive_x.y()));
    extents[1]->SetPosition(engine::math::Vec2D(extent_positive_y.x(), extent_positive_y.y()));
    extents[2]->SetPosition(engine::math::Vec2D(extent_negative_x.x(), extent_negative_x.y()));
    extents[3]->SetPosition(engine::math::Vec2D(extent_negative_y.x(), extent_negative_y.y()));
}

} // namespace physics
} // namespace engine


#endif
