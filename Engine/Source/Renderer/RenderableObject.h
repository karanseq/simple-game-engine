#ifndef RENDERABLE_OBJECT_H_
#define RENDERABLE_OBJECT_H_

// engine includes
#include "Memory\SharedPointer.h"
#include "Memory\WeakPointer.h"

// forward declarations
namespace GLib {
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
	inline engine::memory::WeakPointer<engine::gameobject::GameObject> GetGameObject() const;
	inline void SetGameObject(const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object);

private:
	RenderableObject(GLib::Sprites::Sprite* i_sprite, const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object);

private:
	GLib::Sprites::Sprite*																sprite_;
	engine::memory::WeakPointer<engine::gameobject::GameObject>							game_object_;

}; // class RenderableObject

} // namespace render
} // namespace engine

#include "RenderableObject-inl.h"

#endif // RENDERABLE_OBJECT_H_
