#ifndef PHYSICS_H_
#define PHYSICS_H_

// library includes
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

	Physics(const Physics& i_copy) = delete;
	Physics& operator=(const Physics& i_copy) = delete;

public:
	static Physics* Create();
	static void Destroy();
	static inline Physics* Get();

	void Run(float dt);

	// add/remove physics objects
	inline void AddPhysicsObject(const engine::memory::SharedPointer<PhysicsObject>& i_physics_object);
	inline void RemovePhysicsObject(const engine::memory::SharedPointer<PhysicsObject>& i_physics_object);

private:
	size_t																			num_physics_objects_;
	std::vector<engine::memory::SharedPointer<PhysicsObject>>						physics_objects_;
};

} // namespace physics
} // namespace engine

#include "Physics-inl.h"

#endif // PHYSICS_H_
