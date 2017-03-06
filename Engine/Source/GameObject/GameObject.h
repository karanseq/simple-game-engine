#ifndef ENGINE_GAME_OBJECT_H_
#define ENGINE_GAME_OBJECT_H_

// engine includes
#include "Math\Rect.h"
#include "Math\Transform.h"
#include "Memory\SharedPointer.h"

namespace engine {
namespace gameobject {

/*
	GameObject
	- A simple class that uses a transform to represent an object in space.
*/
class GameObject
{
public:
	inline static engine::memory::SharedPointer<GameObject> Create(const engine::math::Rect& i_aabb = engine::math::Rect::ZERO, 
		const engine::math::Transform& i_transform = engine::math::Transform::ZERO);

	virtual ~GameObject()
	{}

	// copy constructor
	GameObject(const GameObject& i_copy) : aabb_(i_copy.aabb_),
		transform_(i_copy.transform_)
	{}
	// copy assignment operator
	inline GameObject& operator=(const GameObject& i_game_object);

	// accessors and mutators
	inline const engine::math::Rect& GetAABB() const;
	inline void SetAABB(const engine::math::Rect& i_aabb);

	inline const engine::math::Transform& GetTransform() const;
	inline void SetTransform(const engine::math::Transform& i_transform);

	inline const engine::math::Vec3D& GetPosition() const;
	inline void SetPosition(const engine::math::Vec3D& i_position);

	inline const engine::math::Vec3D& GetRotation() const;
	inline void SetRotation(const engine::math::Vec3D& i_rotation);

	inline const engine::math::Vec3D& GetScale() const;
	inline void SetScale(const engine::math::Vec3D& i_scale);

private:
	explicit GameObject(const engine::math::Rect& i_aabb = engine::math::Rect::ZERO, 
		const engine::math::Transform& i_transform = engine::math::Transform::ZERO) : aabb_(i_aabb),
		transform_(i_transform)
	{}

private:
	engine::math::Rect						aabb_;
	engine::math::Transform					transform_;
}; // class GameObject

} // namespace gameobject
} // namespace engine

#include "GameObject-inl.h"

#endif // ENGINE_GAME_OBJECT_H_
