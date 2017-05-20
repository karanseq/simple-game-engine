#include "GameObject.h"

namespace engine {
namespace gameobject {

inline engine::memory::SharedPointer<GameObject> GameObject::Create(const engine::math::AABB& i_aabb, const engine::math::Transform& i_transform, const engine::memory::WeakPointer<Actor>& i_owner)
{
    return engine::memory::SharedPointer<GameObject>(new GameObject(i_aabb, i_transform, i_owner));
}

inline GameObject& GameObject::operator=(const GameObject& i_game_object)
{
    if (this != &i_game_object)
    {
        transform_ = i_game_object.transform_;
        aabb_ = i_game_object.aabb_;
        owner_ = i_game_object.owner_;
    }
    return *this;
}

inline const engine::math::Transform& GameObject::GetTransform() const
{ 
    return transform_;
}

inline void GameObject::SetTransform(const engine::math::Transform& i_transform)
{
    transform_ = i_transform;
}

inline const engine::math::Vec3D& GameObject::GetPosition() const
{
    return transform_.GetPosition();
}

inline void GameObject::SetPosition(const engine::math::Vec3D& i_position)
{
    transform_.SetPosition(i_position);
}

inline const engine::math::Vec3D& GameObject::GetRotation() const
{
    return transform_.GetRotation();
}

inline void GameObject::SetRotation(const engine::math::Vec3D& i_rotation)
{
    transform_.SetRotation(i_rotation);
}

inline const engine::math::Vec3D& GameObject::GetScale() const
{
    return transform_.GetScale();
}

inline void GameObject::SetScale(const engine::math::Vec3D& i_scale)
{
    transform_.SetScale(i_scale);
}

inline const engine::math::AABB& GameObject::GetAABB() const
{
    return aabb_;
}

inline void GameObject::SetAABB(const engine::math::AABB& i_aabb)
{
    aabb_ = i_aabb;
}

inline const engine::memory::WeakPointer<Actor>& GameObject::GetOwner() const
{
    return owner_;
}

inline void GameObject::SetOwner(const engine::memory::WeakPointer<Actor>& i_owner)
{
    owner_ = i_owner;
}

} // namespace gameobject
} // namespace engine