#ifndef PHYSICS_OBJECT_H_
#define PHYSICS_OBJECT_H_

// library includes
#include <stdint.h>

// engine includes
#include "Math\Vec3D.h"
#include "Memory\SharedPointer.h"
#include "Memory\WeakPointer.h"

#ifdef ENABLE_DEBUG_DRAW
#include "Physics\DebugDraw.h"
#endif

// forward declaration
namespace engine {
namespace gameobject {
    class GameObject;
}
}

namespace engine {
namespace physics {

enum class PhysicsObjectType : uint8_t
{
    kPhysicsObjectStatic =              0,
    kPhysicsObjectKinematic =           1,
    kPhysicsObjectDynamic =             2
};

struct CollisionData
{
    uint16_t                                                                collision_filter_;
    bool                                                                    is_collidable_;
    bool                                                                    done_collision_response_;
    bool                                                                    default_collision_response_enabled_;
};

/*
    PhysicsObject
    - A class that can be used to associate physics with a game object
*/

class PhysicsObject
{
public:
    inline static engine::memory::SharedPointer<PhysicsObject> Create(const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object,
        float i_mass = DEFAULT_MASS,
        float i_drag = DEFAULT_COEFF_DRAG,
        PhysicsObjectType i_type = PhysicsObjectType::kPhysicsObjectStatic,
        uint16_t i_collision_filter = 0x0000,
        bool i_is_collidable = false);
    ~PhysicsObject();

    // copy constructor
    PhysicsObject(const PhysicsObject& i_copy);
    // copy assignment operator
    inline PhysicsObject& operator=(const PhysicsObject& i_copy);

    // functions
    void Update(float i_dt);
    void ApplyImpulse(const engine::math::Vec3D& i_impulse);
    void RespondToCollision(const engine::math::Vec3D& collision_normal);

    // debug draw functions
#ifdef ENABLE_DEBUG_DRAW
    void InitDebugDraw(uint8_t i_r, uint8_t i_g, uint8_t i_b, uint8_t i_a);
    void DrawDebugData(float i_dt);
#endif

    // accessors and mutators
    inline const engine::math::Vec3D& GetVelocity() const;
    inline void SetVelocity(const engine::math::Vec3D& i_velocity);

    inline uint16_t GetCollisionFilter() const;
    inline void SetCollisionFilter(uint16_t i_collision_filter);

    inline bool GetIsCollidable() const;
    inline void SetIsCollidable(bool i_is_collidable);

    inline bool GetDefaultCollisionResponseEnabled() const;
    inline void SetDefaultCollisionResponseEnabled(bool i_default_collision_response_enabled);

    inline engine::memory::WeakPointer<engine::gameobject::GameObject> GetGameObject() const;
    inline void SetGameObject(const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object);

    inline float GetMass() const;
    inline void SetMass(float i_mass);

    inline float GetDrag() const;
    inline void SetDrag(float i_drag);

    inline PhysicsObjectType GetType() const;
    inline void SetType(PhysicsObjectType i_type);

    inline bool GetIsAwake() const;
    inline void SetIsAwake(bool i_is_awake);

    inline bool GetIsActive() const;
    inline void SetIsActive(bool i_is_active);

private:
    explicit PhysicsObject(const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object, 
        float i_mass = DEFAULT_MASS, 
        float i_drag = DEFAULT_COEFF_DRAG, 
        PhysicsObjectType i_type = PhysicsObjectType::kPhysicsObjectStatic,
        uint16_t i_collision_filter = 0x0000,
        bool i_is_collidable = false);

public:
    // constants
    static const float                                                      DEFAULT_MASS;
    static const float                                                      DEFAULT_COEFF_DRAG;
    static const float                                                      MAX_COEFF_DRAG;
    static const float                                                      MIN_VELOCITY_LENGTH_SQUARED;
    static const float                                                      MAX_VELOCITY_LENGTH_SQUARED;

private:
    engine::math::Vec3D                                                     curr_velocity_;
    CollisionData                                                           collision_data_;
    engine::memory::WeakPointer<engine::gameobject::GameObject>             game_object_;

    float                                                                   mass_;
    float                                                                   inverse_mass_;
    float                                                                   coeff_drag_;

    PhysicsObjectType                                                       type_;
    bool                                                                    is_awake_;
    bool                                                                    is_active_;

#ifdef ENABLE_DEBUG_DRAW
    DebugDrawData*                                                          debug_draw_data_;
#endif

}; // class PhysicsObject

} // namespace physics
} // namespace engine

#include "PhysicsObject-inl.h"

#endif // PHYSICS_OBJECT_H_
