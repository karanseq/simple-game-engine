#include "Physics\Physics.h"

// engine includes
#include "Common\HelperMacros.h"

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
    for (size_t i = 0; i < num_physics_objects_; ++i)
    {
        physics_objects_[i]->Update(i_dt);
    }
}

} // namespace physics
} // namespace engine
