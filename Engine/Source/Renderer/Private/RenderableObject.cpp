#include "Renderer\RenderableObject.h"

// engine includes
#include "GameObject\GameObject.h"
#include "GLib.h"
#include "Renderer\Renderer.h"

namespace engine {
namespace render {

RenderableObject::RenderableObject(GLib::Sprites::Sprite* i_sprite, const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object) : sprite_(i_sprite),
	game_object_(i_game_object)
{
	// validate inputs
	ASSERT(sprite_);
	ASSERT(game_object_);
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
	// get a shared pointer to operate on
	engine::memory::SharedPointer<engine::gameobject::GameObject> game_object(game_object_);

	GLib::Point2D offset = { game_object->GetPosition().x(), game_object->GetPosition().y() };
	GLib::Sprites::RenderSprite(*sprite_, offset, 0.0f);
}

} // namespace render
} // namespace engine
