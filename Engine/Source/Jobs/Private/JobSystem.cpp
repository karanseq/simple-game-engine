#include "Jobs\JobSystem.h"

// engine includes
#include "Assert\Assert.h"
#include "Jobs\InterfaceJob.h"
#include "Jobs\JobQueue.h"
#include "Jobs\Worker.h"
#include "Logger\Logger.h"

namespace engine {
namespace jobs {

// static member initialization
JobSystem* JobSystem::instance_ = nullptr;

JobSystem::JobSystem() : shutdown_requested_(false)
{}

JobSystem::~JobSystem()
{
	Shutdown();
	LOG("JobSystem destroyed");
}

JobSystem* JobSystem::Create()
{
	if (JobSystem::instance_ == nullptr)
	{
		JobSystem::instance_ = new JobSystem();
	}
	return JobSystem::instance_;
}

void JobSystem::Destroy()
{
	if (JobSystem::instance_ != nullptr)
	{
		delete JobSystem::instance_;
		JobSystem::instance_ = nullptr;
	}
}

bool JobSystem::CreateTeam(const engine::data::PooledString& i_team_name, const size_t num_workers)
{
	// validate inputs
	ASSERT(teams_.find(i_team_name) == teams_.end());
	ASSERT(num_workers > 0);

	// create a new team
	Team* team = new Team();
	ASSERT(team);

	// name this team
	team->name_ = i_team_name;

	// create a job queue for this team
	team->job_queue_ = new JobQueue(i_team_name);
	ASSERT(team->job_queue_);

	// create the number of workers requested
	for (uint16_t i = 0; i < num_workers; ++i)
	{
		team->workers_.push_back(new Worker(i, team->job_queue_));
	}

	// save the team
	bool result = teams_.insert(std::pair<engine::data::HashedString, Team*>(i_team_name, team)).second;

	return result;
}

bool JobSystem::AddJob(InterfaceJob* i_job, const engine::data::PooledString& i_team_name)
{
	// validate inputs
	ASSERT(i_job);
	std::map<engine::data::HashedString, Team*>::iterator team_it = teams_.find(i_team_name);
	ASSERT(team_it != teams_.end());

	return team_it->second->job_queue_->AddJob(i_job);
}

void JobSystem::Shutdown()
{
	if (shutdown_requested_)
	{
		return;
	}
	shutdown_requested_ = true;

	for (std::map<engine::data::HashedString, Team*>::iterator it = teams_.begin(); it != teams_.end(); ++it)
	{
		// ask this team's queue to shutdown
		it->second->job_queue_->RequestShutdown();

		const size_t num_workers = it->second->workers_.size();
		for (size_t i = 0; i < num_workers; ++i)
		{
			// delete each worker in this team
			// each worker joins its thread in its destructor
			delete it->second->workers_[i];
		}
		it->second->workers_.clear();

		// delete this team's queue
		delete it->second->job_queue_;

		// delete this team
		delete it->second;
	}

	teams_.clear();
}

} // namespace jobs
} // namespace engine