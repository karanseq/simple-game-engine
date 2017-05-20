#ifndef RENDERABLE_OBJECT_H_
#define RENDERABLE_OBJECT_H_

// external includes
#include "BasicTypes.h"

// engine includes
#include "Math\Vec2D.h"
#include "Memory\SharedPointer.h"
#include "Memory\WeakPointer.h"

// forward declarations
namespace GLib {
    struct Point2D;
namespace Sprites {
    struct Sprite;
}
}
namespace engine {
namespace gameobject {
    class GameObject;
}
}

namespace engine {
namespace render {

class RenderableObject
{
public:
    inline static engine::memory::SharedPointer<RenderableObject> Create(GLib::Sprites::Sprite* i_sprite);
    inline static engine::memory::SharedPointer<RenderableObject> Create(GLib::Sprites::Sprite* i_sprite, const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object);
    ~RenderableObject();

    // disable copy constructor & copy assignment operator
    RenderableObject(const RenderableObject& i_copy) = delete;
    RenderableObject& operator=(const RenderableObject& i_copy) = delete;

    // functions
    void Render(float i_dt);

    // accessors and mutators
    inline GLib::Sprites::Sprite* GetSprite() const;
    inline void SetSprite(GLib::Sprites::Sprite* i_sprite);

    inline const float GetAngle() const;
    inline void SetAngle(const float i_angle);

    inline const engine::math::Vec2D GetPositionAsVec2D() const;
    inline const GLib::Point2D& GetPositionAsPoint2D() const;
    inline void SetPosition(const engine::math::Vec2D& i_position);
    inline void SetPosition(const GLib::Point2D& i_position);

    inline engine::memory::WeakPointer<engine::gameobject::GameObject> GetGameObject() const;
    inline void SetGameObject(const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object);

    inline bool GetIsVisible() const;
    inline void SetIsVisible(bool i_is_visible);

private:
    RenderableObject(GLib::Sprites::Sprite* i_sprite);
    RenderableObject(GLib::Sprites::Sprite* i_sprite, const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object);

private:
    GLib::Sprites::Sprite*                                                              sprite_;
    float                                                                               angle_;
    GLib::Point2D                                                                       position_;
    engine::memory::WeakPointer<engine::gameobject::GameObject>                         game_object_;
    bool                                                                                is_visible_;

}; // class RenderableObject

} // namespace render
} // namespace engine

#include "RenderableObject-inl.h"

#endif // RENDERABLE_OBJECT_H_
