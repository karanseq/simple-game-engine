#include "GameObject\Actor.h"

// engine includes
#include "Physics\Physics.h"
#include "Renderer\Renderer.h"

namespace engine {
namespace gameobject {

Actor::Actor() : name_(""),
	type_(""),
	game_object_(nullptr),
	physics_object_(nullptr),
	renderable_object_(nullptr)
{}

Actor::Actor(const engine::data::PooledString& i_name, const engine::data::HashedString& i_type) : name_(i_name),
	type_(i_type),
	game_object_(nullptr),
	physics_object_(nullptr),
	renderable_object_(nullptr)
{}

Actor::Actor(const engine::memory::SharedPointer<GameObject>& i_game_object) : name_(),
	type_(),
	game_object_(i_game_object),
	physics_object_(nullptr),
	renderable_object_(nullptr)
{}

Actor::Actor(const engine::memory::SharedPointer<GameObject>& i_game_object, const engine::memory::WeakPointer<engine::physics::PhysicsObject>& i_physics_object) : name_(),
	type_(),
	game_object_(i_game_object),
	physics_object_(i_physics_object),
	renderable_object_(nullptr)
{}

Actor::Actor(const engine::memory::SharedPointer<GameObject>& i_game_object, const engine::memory::WeakPointer<engine::render::RenderableObject>& i_renderable_object) : name_(),
	type_(),
	game_object_(i_game_object),
	physics_object_(nullptr),
	renderable_object_(i_renderable_object)
{}


Actor::Actor(const engine::memory::SharedPointer<GameObject>& i_game_object, const engine::memory::WeakPointer<engine::physics::PhysicsObject>& i_physics_object, const engine::memory::WeakPointer<engine::render::RenderableObject>& i_renderable_object) : name_(),
	type_(),
	game_object_(i_game_object),
	physics_object_(i_physics_object),
	renderable_object_(i_renderable_object)
{}

Actor::~Actor()
{
	if (physics_object_)
	{
		engine::physics::Physics::Get()->RemovePhysicsObject(physics_object_.Lock());
	}

	if (renderable_object_)
	{
		engine::render::Renderer::Get()->RemoveRenderableObject(renderable_object_.Lock());
	}
}

} // namespace gameobject
} // namespace engine
