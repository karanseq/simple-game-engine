#include "GameObject.h"

namespace engine {
namespace gameobject {

inline GameObject& GameObject::operator=(const GameObject& game_object)
{
	if (this != &game_object)
	{
		transform_ = game_object.transform_;
	}
	return *this;
}

inline const engine::math::Transform& GameObject::GetTransform() const
{ 
	return transform_;
}

inline void GameObject::SetTransform(const engine::math::Transform& transform)
{
	transform_ = transform;
}

inline const engine::math::Vec3D& GameObject::GetPosition() const
{
	return transform_.GetPosition();
}

inline void GameObject::SetPosition(const engine::math::Vec3D& position)
{
	transform_.SetPosition(position);
}

inline const engine::math::Vec3D& GameObject::GetRotation() const
{
	return transform_.GetRotation();
}

inline void GameObject::SetRotation(const engine::math::Vec3D& rotation)
{
	transform_.SetRotation(rotation);
}

inline const engine::math::Vec3D& GameObject::GetScale() const
{
	return transform_.GetScale();
}

inline void GameObject::SetScale(const engine::math::Vec3D& scale)
{
	transform_.SetScale(scale);
}

} // namespace gameobject
} // namespace engine