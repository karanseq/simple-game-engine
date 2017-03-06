#include "Physics.h"

// library includes
#include <algorithm>

// engine includes
#include "Assert\Assert.h"
#include "Logger\Logger.h"

namespace engine {
namespace physics {

inline Physics* Physics::Get()
{
	return Physics::instance_;
}

inline engine::memory::SharedPointer<PhysicsObject> Physics::CreatePhysicsObject(const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object, float i_mass, float i_drag)
{
	// validate input
	ASSERT(i_game_object);

	// create a new physics object
	engine::memory::SharedPointer<PhysicsObject> physics_object = PhysicsObject::Create(i_game_object, i_mass, i_drag);

	// add it to the list
	physics_objects_.push_back(physics_object);
	++num_physics_objects_;

	return physics_object;
}

inline void Physics::AddPhysicsObject(const engine::memory::SharedPointer<PhysicsObject>& i_physics_object)
{
	// validate input
	ASSERT(i_physics_object);

	std::lock_guard<std::mutex> lock(physics_mutex_);

	// check if this object already exists
	if (std::find(physics_objects_.begin(), physics_objects_.end(), i_physics_object) != physics_objects_.end())
	{
		LOG_ERROR("Physics is already tracking this physics object!");
		return;
	}

	physics_objects_.push_back(i_physics_object);
	++num_physics_objects_;
}

inline void Physics::RemovePhysicsObject(const engine::memory::SharedPointer<PhysicsObject>& i_physics_object)
{
	// validate input
	ASSERT(i_physics_object);
	// can't remove an object if there are none
	ASSERT(num_physics_objects_ > 0);

	std::lock_guard<std::mutex> lock(physics_mutex_);

	// check if this object exists
	auto it = std::find(physics_objects_.begin(), physics_objects_.end(), i_physics_object);
	if (it == physics_objects_.end())
	{
		LOG_ERROR("Physics could not find this physics object!");
		return;
	}

	physics_objects_.erase(it);
	--num_physics_objects_;
}

} // namespace physics
} // namespace engine