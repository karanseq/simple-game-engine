#include "RenderableObject.h"

// external includes
#include "BasicTypes.h"
#include "GLib.h"

// engine includes
#include "Assert\Assert.h"
#include "Math\MathUtil.h"

namespace engine {
namespace render {

inline engine::memory::SharedPointer<RenderableObject> RenderableObject::Create(GLib::Sprites::Sprite* i_sprite)
{
    return engine::memory::SharedPointer<RenderableObject>(new RenderableObject(i_sprite));
}

inline engine::memory::SharedPointer<RenderableObject> RenderableObject::Create(GLib::Sprites::Sprite* i_sprite, const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object)
{
    return engine::memory::SharedPointer<RenderableObject>(new RenderableObject(i_sprite, i_game_object));
}

inline GLib::Sprites::Sprite* RenderableObject::GetSprite() const
{
    return sprite_;
}

inline void RenderableObject::SetSprite(GLib::Sprites::Sprite* i_sprite)
{
    ASSERT(i_sprite);
    sprite_ = i_sprite;
}

inline const float RenderableObject::GetAngle() const
{
    return angle_;
}

inline void RenderableObject::SetAngle(const float i_angle)
{
    ASSERT(!engine::math::IsNaN(i_angle));
    angle_ = i_angle;
}

inline const engine::math::Vec2D RenderableObject::GetPositionAsVec2D() const
{
    return engine::math::Vec2D(position_.x, position_.y);
}

inline const GLib::Point2D& RenderableObject::GetPositionAsPoint2D() const
{
    return position_;
}

inline void RenderableObject::SetPosition(const engine::math::Vec2D& i_position)
{
    position_.x = i_position.x();
    position_.y = i_position.y();
}

inline void RenderableObject::SetPosition(const GLib::Point2D& i_position)
{
    position_ = i_position;
}

inline engine::memory::WeakPointer<engine::gameobject::GameObject> RenderableObject::GetGameObject() const
{
    return game_object_;
}

inline void RenderableObject::SetGameObject(const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object)
{
    ASSERT(i_game_object);
    game_object_ = i_game_object;
}

inline bool RenderableObject::GetIsVisible() const
{
    return is_visible_;
}

inline void RenderableObject::SetIsVisible(bool i_is_visible)
{
    is_visible_ = i_is_visible;
}

} // namespace render
} // namespace engine
