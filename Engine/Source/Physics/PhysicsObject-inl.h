#include "PhysicsObject.h"

// engine includes
#include "GameObject\GameObject.h"

namespace engine {
namespace physics {

inline engine::memory::SharedPointer<PhysicsObject> PhysicsObject::Create(const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object,
    float i_mass,
    float i_drag,
    PhysicsObjectType i_type,
    uint16_t i_collision_filter,
    bool i_is_collidable)
{
    return engine::memory::SharedPointer<PhysicsObject>(new PhysicsObject(i_game_object, i_mass, i_drag, i_type, i_collision_filter, i_is_collidable));
}

inline PhysicsObject& PhysicsObject::operator=(const PhysicsObject& i_copy)
{
    // check for self assignment
    if (this != &i_copy)
    {
        curr_velocity_ = i_copy.curr_velocity_;
        collision_data_ = i_copy.collision_data_;
        game_object_ = i_copy.game_object_;
        mass_ = i_copy.mass_;
        inverse_mass_ = i_copy.inverse_mass_;
        coeff_drag_ = i_copy.coeff_drag_;
        type_ = i_copy.type_;
        is_awake_ = i_copy.is_awake_;
        is_active_ = i_copy.is_active_;
#ifdef ENABLE_DEBUG_DRAW
        debug_draw_data_ = nullptr;
#endif
    }

    return *this;
}

inline const engine::math::Vec3D& PhysicsObject::GetVelocity() const
{
    return curr_velocity_;
}

inline void PhysicsObject::SetVelocity(const engine::math::Vec3D& i_velocity)
{
    curr_velocity_ = i_velocity;
}

inline uint16_t PhysicsObject::GetCollisionFilter() const
{
    return collision_data_.collision_filter_;
}

inline void PhysicsObject::SetCollisionFilter(uint16_t i_collision_filter)
{
    collision_data_.collision_filter_ = i_collision_filter;
}

inline bool PhysicsObject::GetIsCollidable() const
{
    return collision_data_.is_collidable_;
}

inline void PhysicsObject::SetIsCollidable(bool i_is_collidable)
{
    collision_data_.is_collidable_ = i_is_collidable;
}

inline bool PhysicsObject::GetDefaultCollisionResponseEnabled() const
{
    return collision_data_.default_collision_response_enabled_;
}

inline void PhysicsObject::SetDefaultCollisionResponseEnabled(bool i_default_collision_response_enabled)
{
    collision_data_.default_collision_response_enabled_ = i_default_collision_response_enabled;
}

inline engine::memory::WeakPointer<engine::gameobject::GameObject> PhysicsObject::GetGameObject() const
{
    return game_object_;
}

inline void PhysicsObject::SetGameObject(const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object)
{
    ASSERT(i_game_object);
    game_object_ = i_game_object;
}

inline float PhysicsObject::GetMass() const
{
    return mass_;
}

inline void PhysicsObject::SetMass(float i_mass)
{
    // physics objects must have positive mass
    ASSERT(!engine::math::IsNaN(i_mass) && !engine::math::FuzzyEquals(i_mass, 0.0f));
    mass_ = i_mass;
    inverse_mass_ = 1.0f / mass_;
}

inline float PhysicsObject::GetDrag() const
{
    return coeff_drag_;
}

inline void PhysicsObject::SetDrag(float i_drag)
{
    ASSERT(!engine::math::IsNaN(i_drag) && i_drag < MAX_COEFF_DRAG);
    coeff_drag_ = i_drag;
}

inline PhysicsObjectType PhysicsObject::GetType() const
{
    return type_;
}

inline void PhysicsObject::SetType(PhysicsObjectType i_type)
{
    type_ = i_type;
}

inline bool PhysicsObject::GetIsAwake() const
{
    return is_awake_;
}

inline void PhysicsObject::SetIsAwake(bool i_is_awake)
{
    is_awake_ = i_is_awake;
}

inline bool PhysicsObject::GetIsActive() const
{
    return is_active_;
}

inline void PhysicsObject::SetIsActive(bool i_is_active)
{
    is_active_ = i_is_active;
    curr_velocity_ = engine::math::Vec3D::ZERO;
}

} // namespace physics
} // namespace engine