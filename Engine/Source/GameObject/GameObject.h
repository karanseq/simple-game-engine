#ifndef ENGINE_GAME_OBJECT_H_
#define ENGINE_GAME_OBJECT_H_

// engine includes
#include "Math\Transform.h"

namespace engine {
namespace gameobject {

/*
	GameObject
	- A simple class that uses a transform to represent an object in space.
*/
class GameObject
{
public:
	GameObject() : transform_()
	{}
	GameObject(const engine::math::Transform& transform) : transform_(transform)
	{}

	virtual ~GameObject()
	{}

	// copy constructor
	GameObject(const GameObject& copy) : transform_(copy.transform_)
	{}

	// copy assignment operator
	inline GameObject& operator=(const GameObject& game_object);

	// accessors and mutators
	inline const engine::math::Transform& GetTransform() const;
	inline void SetTransform(const engine::math::Transform& transform);

	inline const engine::math::Vec3D& GetPosition() const;
	inline void SetPosition(const engine::math::Vec3D& position);

	inline const engine::math::Vec3D& GetRotation() const;
	inline void SetRotation(const engine::math::Vec3D& rotation);

	inline const engine::math::Vec3D& GetScale() const;
	inline void SetScale(const engine::math::Vec3D& scale);

private:
	engine::math::Transform transform_;
}; // class GameObject

} // namespace gameobject
} // namespace engine

#include "GameObject-inl.h"

#endif // ENGINE_GAME_OBJECT_H_
