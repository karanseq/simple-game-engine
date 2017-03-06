#include "RenderableObject.h"

// engine includes
#include "Assert\Assert.h"
#include "GLib.h"

namespace engine {
namespace render {

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

inline engine::memory::WeakPointer<engine::gameobject::GameObject> RenderableObject::GetGameObject() const
{
	return game_object_;
}

inline void RenderableObject::SetGameObject(const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object)
{
	ASSERT(i_game_object);
	game_object_ = i_game_object;
}

} // namespace render
} // namespace engine
