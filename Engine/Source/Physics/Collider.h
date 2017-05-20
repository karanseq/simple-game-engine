#ifndef COLLIDER_H_
#define COLLIDER_H_

// library includes
#include <mutex>
#include <vector>

// engine includes
#include "Math\Vec3D.h"
#include "Memory\WeakPointer.h"

// forward declarations
namespace engine {
namespace math {
    struct AABB;
    class Mat44;
    class Transform;
}
namespace physics {
    class PhysicsObject;
}
}

namespace engine {
namespace physics {

struct CollisionPair
{
    float                                                       time;
    engine::math::Vec3D                                         normal;
    engine::memory::WeakPointer<PhysicsObject>                  object_a;
    engine::memory::WeakPointer<PhysicsObject>                  object_b;
};

class InterfaceCollisionListener
{
public:
    virtual void OnCollision(const CollisionPair& i_collision_pair) = 0;
};

class Collider
{
private:
    Collider();
    ~Collider();
    static Collider* instance_;

    // disable copy constructor & copy assignment operator
    Collider(const Collider& i_copy) = delete;
    Collider& operator=(const Collider& i_other) = delete;

public:
    static Collider* Create();
    static void Destroy();
    static inline Collider* Get();

    void Run(float i_dt);
    void DetectCollisions(float i_dt);
    void RespondToCollisions(float i_dt);

    bool CheckSeparationForAxis(const float i_relative_vel_WtoA, const float i_a_aabb_center, const float i_a_aabb_extents, const float i_B_center_in_A, const float i_B_extents_in_a, const float i_dt, float &o_t_close, float &o_t_open);

    // add and remove physics objects
    void AddPhysicsObject(const engine::memory::WeakPointer<engine::physics::PhysicsObject>& i_physics_object);
    void RemovePhysicsObject(const engine::memory::WeakPointer<engine::physics::PhysicsObject>& i_physics_object);

    inline void SetCollisionListener(InterfaceCollisionListener* i_collision_listener);

#ifdef BUILD_DEBUG
    void PrintDebugInformation(const engine::math::Mat44& i_mat_WtoA,
        const engine::math::Mat44& i_mat_WtoB,
        const engine::math::Mat44& i_mat_AtoB, 
        const engine::math::Mat44& i_mat_BtoA, 
        const engine::math::AABB& i_a_aabb, 
        const engine::math::AABB& i_b_aabb, 
        const engine::math::Vec3D& i_a_vel, 
        const engine::math::Vec3D& i_b_vel,
        float i_dt) const;
#endif

private:
    std::vector<engine::memory::WeakPointer<PhysicsObject>>                         dynamic_objects_;
    std::vector<engine::memory::WeakPointer<PhysicsObject>>                         static_kynematic_objects_;
    std::vector<CollisionPair>                                                      collided_objects_;

    size_t                                                                          num_dynamic_objects_;
    size_t                                                                          num_static_kynematic_objects_;
    std::mutex                                                                      collider_mutex_;

    InterfaceCollisionListener*                                                     collision_listener_;

}; // class Collider

} // namespace physics
} // namespace engine

#include "Collider-inl.h"

#endif // COLLIDER_H_
