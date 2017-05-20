#ifndef ENGINE_GAME_OBJECT_H_
#define ENGINE_GAME_OBJECT_H_

// engine includes
#include "Math\AABB.h"
#include "Math\Transform.h"
#include "Memory\SharedPointer.h"
#include "Memory\WeakPointer.h"

namespace engine {
namespace gameobject {

// forward declaration
class Actor;

/*
    GameObject
    - A simple class that uses a transform to represent an object in space.
*/
class GameObject
{
public:
    inline static engine::memory::SharedPointer<GameObject> Create(const engine::math::AABB& i_aabb = engine::math::AABB::ZERO,
        const engine::math::Transform& i_transform = engine::math::Transform::ZERO,
        const engine::memory::WeakPointer<Actor>& i_owner = nullptr);

    // copy constructor
    GameObject(const GameObject& i_copy) : 
        transform_(i_copy.transform_),
        aabb_(i_copy.aabb_),
        owner_(i_copy.owner_)
    {}
    // copy assignment operator
    inline GameObject& operator=(const GameObject& i_game_object);

    // accessors and mutators
    inline const engine::math::Transform& GetTransform() const;
    inline void SetTransform(const engine::math::Transform& i_transform);

    inline const engine::math::Vec3D& GetPosition() const;
    inline void SetPosition(const engine::math::Vec3D& i_position);

    inline const engine::math::Vec3D& GetRotation() const;
    inline void SetRotation(const engine::math::Vec3D& i_rotation);

    inline const engine::math::Vec3D& GetScale() const;
    inline void SetScale(const engine::math::Vec3D& i_scale);

    inline const engine::math::AABB& GetAABB() const;
    inline void SetAABB(const engine::math::AABB& i_aabb);

    inline const engine::memory::WeakPointer<Actor>& GetOwner() const;
    inline void SetOwner(const engine::memory::WeakPointer<Actor>& i_owner);

private:
    explicit GameObject(const engine::math::AABB& i_aabb = engine::math::AABB::ZERO,
        const engine::math::Transform& i_transform = engine::math::Transform::ZERO,
        const engine::memory::WeakPointer<Actor>& i_owner = nullptr) :
            transform_(i_transform),
            aabb_(i_aabb),
            owner_(i_owner)
    {}

private:
    engine::math::Transform                 transform_;
    engine::math::AABB                      aabb_;
    engine::memory::WeakPointer<Actor>      owner_;
}; // class GameObject

} // namespace gameobject
} // namespace engine

#include "GameObject-inl.h"

#endif // ENGINE_GAME_OBJECT_H_
