#include "Physics\Physics.h"

// engine includes
#include "Common\HelperMacros.h"
#include "Physics\Collider.h"

namespace engine {
namespace physics {

// static member initialization
Physics* Physics::instance_ = nullptr;

Physics::Physics() : num_physics_objects_(0)
{}

Physics::~Physics()
{
    physics_objects_.clear();
    num_physics_objects_ = 0;
}

Physics* Physics::Create()
{
    if (!Physics::instance_)
    {
        Physics::instance_ = new Physics();
    }
    return Physics::instance_;
}

void Physics::Destroy()
{
    SAFE_DELETE(Physics::instance_);
}

void Physics::Run(float i_dt)
{
    // acquire a lock
    std::lock_guard<std::mutex> lock(physics_mutex_);

    for (size_t i = 0; i < num_physics_objects_; ++i)
    {
        if (physics_objects_[i]->GetType() != PhysicsObjectType::kPhysicsObjectStatic)
        {
            physics_objects_[i]->Update(i_dt);
        }
    }
}

engine::memory::SharedPointer<PhysicsObject> Physics::CreatePhysicsObject(const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object, 
    float i_mass, 
    float i_drag, 
    PhysicsObjectType i_type, 
    uint16_t i_collision_filter,
    bool i_is_collidable)
{
    // validate input
    ASSERT(i_game_object);

    // create a new physics object
    engine::memory::SharedPointer<PhysicsObject> physics_object = PhysicsObject::Create(i_game_object, i_mass, i_drag, i_type, i_collision_filter, i_is_collidable);

    AddPhysicsObject(physics_object);

    return physics_object;
}

void Physics::AddPhysicsObject(const engine::memory::SharedPointer<PhysicsObject>& i_physics_object)
{
    // validate input
    ASSERT(i_physics_object);

    // add to the collider if it is collidable
    if (i_physics_object->GetIsCollidable())
    {
        Collider::Get()->AddPhysicsObject(i_physics_object);
    }

    // acquire a lock
    std::lock_guard<std::mutex> lock(physics_mutex_);

    // check if this object already exists
    if (std::find(physics_objects_.begin(), physics_objects_.end(), i_physics_object) != physics_objects_.end())
    {
        LOG_ERROR("Physics is already tracking this physics object!");
        return;
    }

    // add it to the list
    physics_objects_.push_back(i_physics_object);
    ++num_physics_objects_;
}

void Physics::RemovePhysicsObject(const engine::memory::SharedPointer<PhysicsObject>& i_physics_object)
{
    // validate input
    ASSERT(i_physics_object);
    // can't remove an object if there are none
    ASSERT(num_physics_objects_ > 0);

    // remove from the collider if it is collidable
    if (i_physics_object->GetIsCollidable())
    {
        Collider::Get()->RemovePhysicsObject(i_physics_object);
    }

    // acquire a lock
    std::lock_guard<std::mutex> lock(physics_mutex_);

    // check if this object exists
    auto it = std::find(physics_objects_.begin(), physics_objects_.end(), i_physics_object);
    if (it == physics_objects_.end())
    {
        LOG_ERROR("Physics could not find this physics object!");
        return;
    }

    // erase it from the list
    physics_objects_.erase(it);
    --num_physics_objects_;
}

} // namespace physics
} // namespace engine
