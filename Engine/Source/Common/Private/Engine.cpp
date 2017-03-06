#include "Common\Engine.h"

// library includes
#include <stdlib.h>
// TODO: Resolve conflict with namespace time
//#include <time.h>

// engine includes
#include "Data\StringPool.h"
#include "Input\Input.h"
#include "Jobs\JobSystem.h"
#include "Memory\AllocatorUtil.h"
#include "Physics\Physics.h"
#include "Renderer\Renderer.h"
#include "Time\TimerUtil.h"
#include "Time\Updater.h"
#include "Util\FileUtils.h"

namespace engine {

bool StartUp()
{
	// create allocators
	engine::memory::CreateAllocators();

	// create string pools
	engine::data::StringPool::Create();

	// create job system
	engine::jobs::JobSystem* job_system = engine::jobs::JobSystem::Create();
	job_system->CreateTeam(engine::data::PooledString("EngineJobs"), 5);

	// create file util
	engine::util::FileUtils::Create();

	// initialize input
	engine::input::StartUp();

	// create updater
	engine::time::Updater::Create();

	// create physics
	engine::physics::Physics::Create();

	// create renderer
	engine::render::Renderer::Create();

	// give rand a new seed
	// TODO: Resolve conflict with namespace time
	//srand(static_cast<unsigned int>(time(0)));

	shutdown_requested_ = false;

	return true;
}

void Run()
{
	// calculate the ideal delta to run at 60 FPS
	static const float ideal_dt = 1000.0f / 60.0f;

	// save pointers to the modules that need ticking
	static engine::time::Updater* updater = engine::time::Updater::Get();
	static engine::physics::Physics* physics = engine::physics::Physics::Get();
	static engine::render::Renderer* renderer = engine::render::Renderer::Get();

	while (!shutdown_requested_)
	{
		// get delta
		float dt = engine::time::TimerUtil::CalculateLastFrameTime_ms();

		// update modules
		physics->Run(dt);
		updater->Run(dt);
		renderer->Run(dt);

		// ensure we have a steady 60 frames per second
		const float diff_dt = ideal_dt - dt;
		engine::time::TimerUtil::CustomSleep(uint32_t(diff_dt > 0.0f ? diff_dt : 0.0f));
	}
}

void Shutdown()
{
	// delete renderer
	engine::render::Renderer::Destroy();

	// delete physics
	engine::physics::Physics::Destroy();

	// delete updater
	engine::time::Updater::Destroy();

	// shutdown the input
	engine::input::Shutdown();

	// delete file util
	engine::util::FileUtils::Destroy();

	// delete job system
	engine::jobs::JobSystem::Destroy();

	// delete string pools
	engine::data::StringPool::Destroy();

	// delete allocators
	engine::memory::DestroyAllocators();
}

void RequestShutdown()
{
	if (shutdown_requested_)
	{
		return;
	}
	shutdown_requested_ = true;
}

} // namespace engine