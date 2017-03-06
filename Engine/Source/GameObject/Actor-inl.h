#include "Actor.h"

namespace engine {
namespace gameobject {

inline engine::memory::SharedPointer<Actor> Actor::Create()
{
	return engine::memory::SharedPointer<Actor>(new Actor());
}

inline engine::memory::SharedPointer<Actor> Actor::Create(const engine::data::PooledString& i_name, const engine::data::HashedString& i_type)
{
	return engine::memory::SharedPointer<Actor>(new Actor(i_name, i_type));
}

inline engine::memory::SharedPointer<Actor> Actor::Create(const engine::memory::SharedPointer<GameObject>& i_game_object)
{
	return engine::memory::SharedPointer<Actor>(new Actor(i_game_object));
}

inline engine::memory::SharedPointer<Actor> Actor::Create(const engine::memory::SharedPointer<GameObject>& i_game_object, const engine::memory::WeakPointer<engine::physics::PhysicsObject>& i_physics_object)
{
	return engine::memory::SharedPointer<Actor>(new Actor(i_game_object, i_physics_object));
}

inline engine::memory::SharedPointer<Actor> Actor::Create(const engine::memory::SharedPointer<GameObject>& i_game_object, const engine::memory::WeakPointer<engine::render::RenderableObject>& i_renderable_object)
{
	return engine::memory::SharedPointer<Actor>(new Actor(i_game_object, i_renderable_object));
}

inline engine::memory::SharedPointer<Actor> Actor::Create(const engine::memory::SharedPointer<GameObject>& i_game_object, const engine::memory::WeakPointer<engine::physics::PhysicsObject>& i_physics_object, const engine::memory::WeakPointer<engine::render::RenderableObject>& i_renderable_object)
{
	return engine::memory::SharedPointer<Actor>(new Actor(i_game_object, i_physics_object, i_renderable_object));
}

inline void Actor::SetID(uint16_t i_id)
{
	id_ = i_id;
}

inline uint32_t Actor::GetID() const
{
	return id_;
}

inline void Actor::SetName(const engine::data::PooledString& i_name)
{
	name_ = i_name;
}

inline const engine::data::PooledString& Actor::GetName() const
{
	return name_;
}

inline void Actor::SetType(const engine::data::HashedString& i_type)
{
	type_ = i_type;
}

inline const engine::data::HashedString& Actor::GetType() const
{
	return type_;
}

inline void Actor::SetGameObject(const engine::memory::SharedPointer<GameObject>& i_game_object)
{
	game_object_ = i_game_object;
}

inline const engine::memory::SharedPointer<GameObject>& Actor::GetGameObject() const
{
	return game_object_;
}

inline void Actor::SetPhysicsObject(const engine::memory::WeakPointer<engine::physics::PhysicsObject>& i_physics_object)
{
	physics_object_ = i_physics_object;
}

inline const engine::memory::WeakPointer<engine::physics::PhysicsObject>& Actor::GetPhysicsObject() const
{
	return physics_object_;
}

inline void Actor::SetRenderableObject(const engine::memory::WeakPointer<engine::render::RenderableObject>& i_renderable_object)
{
	renderable_object_ = i_renderable_object;
}

inline const engine::memory::WeakPointer<engine::render::RenderableObject>& Actor::GetRenderableObject() const
{
	return renderable_object_;
}

} // namespace gameobject
} // namespace engine
