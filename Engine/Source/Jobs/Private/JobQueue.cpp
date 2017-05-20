#include "Jobs\JobQueue.h"

// engine includes
#include "Assert\Assert.h"
#include "Jobs\InterfaceJob.h"
#include "Logger\Logger.h"

namespace engine {
namespace jobs {

JobQueue::JobQueue(const engine::data::PooledString& i_id) : id_(i_id),
    shutdown_requested_(false)
{
    VERBOSE("JobQueue-%s created.", id_.GetString());
}

JobQueue::~JobQueue()
{
#ifdef BUILD_DEBUG
    size_t num_unfinished_jobs = 0;
#endif
    while (!job_queue_.empty())
    {
#ifdef BUILD_DEBUG
        ++num_unfinished_jobs;
#endif
        delete job_queue_.front();
        job_queue_.pop();
    }
#ifdef BUILD_DEBUG
    if (num_unfinished_jobs > 0)
    {
        LOG("JobQueue-%s deleted %zu unfinished jobs.", id_.GetString(), num_unfinished_jobs);
    }
    VERBOSE("JobQueue-%s deleted.", id_.GetString());
#endif
}

bool JobQueue::AddJob(InterfaceJob* i_new_job)
{
    // validate inputs
    ASSERT(i_new_job);

    bool result = false;

    if (shutdown_requested_ == false)
    {
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            job_queue_.push(i_new_job);
#ifdef BUILD_DEBUG
            VERBOSE("\t\tJobQueue-%s added a new job:%s and now has a total of %zu jobs", id_.GetString(), i_new_job->GetName().GetString(), job_queue_.size());
#endif
        }
        result = true;
        start_searching_.notify_one();
    }

    return result;
}

InterfaceJob* JobQueue::GetJob()
{
    if (shutdown_requested_)
    {
        return nullptr;
    }

    InterfaceJob* job = nullptr;

    if (job_queue_.empty())
    {
        std::unique_lock<std::mutex> u_lock(queue_mutex_);
        start_searching_.wait(u_lock);
        u_lock.unlock();
    }
    
    if (shutdown_requested_)
    {
        return nullptr;
    }

    std::lock_guard<std::mutex> lock(queue_mutex_);
    if (!job_queue_.empty())
    {
        job = job_queue_.front();
        job_queue_.pop();
#ifdef BUILD_DEBUG
        VERBOSE("\t\tJobQueue-%s removed job-%s and now has %zu jobs remaining", id_.GetString(), job->GetName().GetString(), job_queue_.size());
#endif
    }

    return job;
}

} // namespace jobs
} // namespace engine