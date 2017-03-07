#ifndef JOB_SYSTEM_H_
#define JOB_SYSTEM_H_

// library includes
#include <map>
#include <vector>

// engine includes
#include "Data\HashedString.h"
#include "Data\PooledString.h"

// TODO: Figure out why winspool conflicts and handle this more gracefully
#undef AddJob

namespace engine {
namespace jobs {

// forward declarations
class InterfaceJob;
class JobQueue;
class Worker;

class JobSystem
{
    struct Team
    {
        JobQueue*                           job_queue_;
        engine::data::PooledString          name_;
        std::vector<Worker*>                workers_;
    };

public:
    static JobSystem* Create();
    static void Destroy();
    static inline JobSystem* Get() { return JobSystem::instance_; }

    bool CreateTeam(const engine::data::PooledString& i_team_name, const size_t num_workers);
    bool AddJob(InterfaceJob* i_job, const engine::data::PooledString& i_team_name);
    void Shutdown();

private:
    JobSystem();
    ~JobSystem();
    static JobSystem*                                                       instance_;

    JobSystem(const JobSystem&) = delete;
    JobSystem(JobSystem&&) = delete;

    JobSystem& operator=(const JobSystem&) = delete;
    JobSystem& operator=(JobSystem&&) = delete;

    std::map<engine::data::HashedString, Team*>                             teams_;
    bool                                                                    shutdown_requested_;

}; // class JobSystem

} // namespace jobs
} // namespace engine

#endif // JOB_SYSTEM_H_