#ifndef WORKER_H_
#define WORKER_H_

// library includes
#include <thread>

namespace engine {
namespace jobs {

// forward declaration
class JobQueue;

class Worker
{
public:
    Worker(uint16_t i_id, JobQueue* i_job_queue);
    ~Worker();

    void DoJob(JobQueue* i_job_queue);

private:
    Worker(const Worker&) = delete;
    Worker(Worker&&) = delete;

    Worker& operator=(const Worker&) = delete;
    Worker& operator=(Worker&&) = delete;

    std::thread                 thread_;
    uint32_t                    id_;
    JobQueue*                   job_queue_;

}; // class Worker

} // namespace jobs
} // namespace engine

#endif // WORKER_H_