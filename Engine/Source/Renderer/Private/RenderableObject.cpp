#include "Renderer\RenderableObject.h"

// external includes
#include "BasicTypes.h"
#include "GLib.h"

// engine includes
#include "GameObject\GameObject.h"
#include "Renderer\Renderer.h"

namespace engine {
namespace render {

RenderableObject::RenderableObject(GLib::Sprites::Sprite* i_sprite) : sprite_(i_sprite),
    game_object_(nullptr),
    is_visible_(true)
{
    // validate inputs
    ASSERT(sprite_);

    position_ = { 0.0f, 0.0f, };
    angle_ = 0.0f;
}

RenderableObject::RenderableObject(GLib::Sprites::Sprite* i_sprite, const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object) : sprite_(i_sprite),
    game_object_(i_game_object),
    is_visible_(true)
{
    // validate inputs
    ASSERT(sprite_);
    ASSERT(game_object_);

    engine::memory::SharedPointer<engine::gameobject::GameObject> game_object(game_object_);
    position_ = { game_object->GetPosition().x(), game_object->GetPosition().y() };
    angle_ = game_object->GetRotation().z();
}

RenderableObject::~RenderableObject()
{
    if (sprite_)
    {
        GLib::Sprites::Release(sprite_);
        sprite_ = nullptr;
    }
}

void RenderableObject::Render(float i_dt)
{
    if (!is_visible_)
    {
        return;
    }

    if (game_object_)
    {
        // get a shared pointer to operate on
        engine::memory::SharedPointer<engine::gameobject::GameObject> game_object(game_object_);
        position_ = { game_object->GetPosition().x(), game_object->GetPosition().y() };
        angle_ = game_object->GetRotation().z();
    }

    GLib::Sprites::RenderSprite(*sprite_, position_, angle_);
}

} // namespace render
} // namespace engine
