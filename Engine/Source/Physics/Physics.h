#ifndef PHYSICS_H_
#define PHYSICS_H_

// library includes
#include <mutex>
#include <vector>

// engine includes
#include "Memory\SharedPointer.h"
#include "Physics\PhysicsObject.h"

namespace engine {
namespace physics {

/*
    Physics
    - A class that updates physics objects
*/

class Physics
{
private:
    Physics();
    ~Physics();
    static Physics* instance_;

    // disable copy constructor & copy assignment operator
    Physics(const Physics& i_copy) = delete;
    Physics& operator=(const Physics& i_copy) = delete;

public:
    static Physics* Create();
    static void Destroy();
    static inline Physics* Get();

    void Run(float i_dt);

    // create, add & remove physics objects
    engine::memory::SharedPointer<PhysicsObject> CreatePhysicsObject(const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object, 
                                                                    float i_mass = PhysicsObject::DEFAULT_MASS, 
                                                                    float i_drag = PhysicsObject::DEFAULT_COEFF_DRAG, 
                                                                    PhysicsObjectType i_type = PhysicsObjectType::kPhysicsObjectStatic,
                                                                    uint16_t i_collision_filter = 0x0000,
                                                                    bool i_is_collidable = false);

    void AddPhysicsObject(const engine::memory::SharedPointer<PhysicsObject>& i_physics_object);
    void RemovePhysicsObject(const engine::memory::SharedPointer<PhysicsObject>& i_physics_object);

private:
    size_t                                                                          num_physics_objects_;
    std::vector<engine::memory::SharedPointer<PhysicsObject>>                       physics_objects_;
    std::mutex                                                                      physics_mutex_;
};

} // namespace physics
} // namespace engine

#include "Physics-inl.h"

#endif // PHYSICS_H_
