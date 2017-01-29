#include "Common\Engine.h"

// library includes
#include <stdlib.h>
// TODO: Resolve conflict with namespace time
//#include <time.h>

// engine includes
#include "Memory\AllocatorUtil.h"
#include "Physics\Physics.h"
#include "Time\TimerUtil.h"
#include "Time\Updater.h"

namespace engine {

bool StartUp()
{
	// create allocators
	engine::memory::CreateAllocators();

	// create updater
	engine::time::Updater::Create();

	// create physics
	engine::physics::Physics::Create();

	// give rand a new seed
	// TODO: Resolve conflict with namespace time
	//srand(static_cast<unsigned int>(time(0)));

	quit_requested_ = false;

	return true;
}

void Run()
{
	// calculate the ideal delta to run at 60 FPS
	static const float ideal_dt = 1000.0f / 60.0f;

	// save pointers to the modules that need ticking
	static engine::time::Updater* updater = engine::time::Updater::Get();
	static engine::physics::Physics* physics = engine::physics::Physics::Get();

	while (!quit_requested_)
	{
		// get delta
		float dt = engine::time::TimerUtil::CalculateLastFrameTime_ms();

		// update modules
		physics->Run(dt);
		updater->Run(dt);

		// ensure we have a steady 60 frames per second
		const float diff_dt = ideal_dt - dt;
		engine::time::TimerUtil::CustomSleep(uint32_t(diff_dt > 0.0f ? diff_dt : 0.0f));
	}
}

void Shutdown()
{
	// delete physics
	engine::physics::Physics::Destroy();

	// delete updater
	engine::time::Updater::Destroy();

	// delete allocators
	engine::memory::DestroyAllocators();
}

void RequestQuit()
{
	quit_requested_ = true;
}

} // namespace engine