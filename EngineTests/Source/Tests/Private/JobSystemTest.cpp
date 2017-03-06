
// engine includes
#include "Data\HashedString.h"
#include "Data\PooledString.h"
#include "Jobs\InterfaceJob.h"
#include "Jobs\JobSystem.h"
#include "Jobs\JobQueue.h"
#include "Logger\Logger.h"

class SimpleSleepJob : public engine::jobs::InterfaceJob
{
public:
	SimpleSleepJob(const engine::data::PooledString& i_name)
	{
		static const size_t duration_weight = 100;
		name_ = i_name;
		job_duration_ = duration_weight + rand() % duration_weight;
	}
	~SimpleSleepJob()
	{}

	void DoWork()
	{
		LOG("\t\t\t\tJob-%s starting work.", name_.GetString());
		std::this_thread::sleep_for(std::chrono::milliseconds(job_duration_));

		// spawn a job inside this job 30% of the time
		if (rand() % 10 > 6)
		{
			LOG("--------------------------------------------");
			LOG("DISPATCHING A JOB WITHIN A JOB");
			engine::jobs::InterfaceJob* new_job = new SimpleSleepJob("SimpleSleepJob-000");
			if (engine::jobs::JobSystem::Get()->AddJob(new_job, engine::data::PooledString("HashBrowns")) == false)
			{
				delete new_job;
			}
			LOG("--------------------------------------------");
		}
		LOG("\t\t\t\tJob-%s finishing work. Took %d milliseconds.", name_.GetString(), job_duration_);
	}
private:
	uint16_t job_duration_;
};

void TestJobSystem()
{
	LOG("--------------------------------------------");
	LOG("RUNNING JOB SYSTEM TEST");
	LOG("--------------------------------------------");
	engine::jobs::JobSystem* job_system = engine::jobs::JobSystem::Create();
	LOG("--------------------------------------------");
	LOG("JOB SYSTEM CREATED");
	LOG("--------------------------------------------");

	const engine::data::PooledString team01_name = engine::data::PooledString("EggsBenedict");
	job_system->CreateTeam(team01_name, 7);
	const engine::data::PooledString team02_name = engine::data::PooledString("HashBrowns");
	job_system->CreateTeam(team02_name, 11);

	LOG("--------------------------------------------");
	LOG("TEAM-EggsBenedict CREATED");
	LOG("TEAM-HashBrowns CREATED");
	LOG("--------------------------------------------");

	size_t num_iterations = 25;
	const size_t max_dispatch_jobs = 10;
	size_t total_jobs = 0;
	int input = 0;
	do
	{
		const size_t num_jobs = max_dispatch_jobs + rand() % max_dispatch_jobs;

		LOG("--------------------------------------------");
		LOG("ITERATION-%zu DISPATCHING %zu JOBS", num_iterations, num_jobs);
		LOG("--------------------------------------------");

		for (uint16_t i = 0; i < num_jobs; ++i)
		{
			char buf[256] = { 0 };
			sprintf_s(buf, "SimpleSleepJob-%zu", ++total_jobs);

			engine::jobs::InterfaceJob* new_job = new SimpleSleepJob(engine::data::PooledString(buf));
			if (job_system->AddJob(new_job, (rand() % 10 < 4) ? team01_name : team02_name) == false)
			{
				delete new_job;
			}
		}

		LOG("--------------------------------------------");
		LOG("DISPATCHED %zu JOBS", num_jobs);
		LOG("--------------------------------------------");

	} while (--num_iterations > 0);

	LOG("--------------------------------------------");
	LOG("SHUTDOWN REQUESTED");
	LOG("--------------------------------------------");

	job_system->Shutdown();
	engine::jobs::JobSystem::Destroy();

	LOG("--------------------------------------------");
	LOG("JOB SYSTEM SHUTDOWN COMPLETE\nTOTAL %zu JOBS CREATED", total_jobs);
	LOG("--------------------------------------------");
}