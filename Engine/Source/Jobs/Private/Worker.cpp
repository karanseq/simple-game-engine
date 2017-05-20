#include "Jobs\Worker.h"

// engine includes
#include "Assert\Assert.h"
#include "Jobs\InterfaceJob.h"
#include "Jobs\JobQueue.h"
#include "Logger\Logger.h"

namespace engine {
namespace jobs {

Worker::Worker(uint16_t i_id, JobQueue* i_job_queue) : id_(i_id),
    job_queue_(i_job_queue)
{
    // validate inputs
    ASSERT(job_queue_);
    thread_ = std::thread(&Worker::DoJob, this, job_queue_);
    VERBOSE("Worker-%d created.", id_);
}

Worker::~Worker()
{
    thread_.join();
    VERBOSE("Worker-%d deleted.", id_);
}

void Worker::DoJob(JobQueue* i_job_queue)
{
    // validate inputs
    ASSERT(i_job_queue);

    bool stop_working = false;

    do
    {
        VERBOSE("\t\t\tWorker-%d starting to look for a job.", id_);

        InterfaceJob* job = i_job_queue->GetJob();
        if (job)
        {
            VERBOSE("\t\t\tWorker-%d found job:%s", id_, job->GetName().GetString());
            
            job->DoWork();
            
            VERBOSE("\t\t\tWorker-%d finished job:%s", id_, job->GetName().GetString());

            delete job;

            stop_working = i_job_queue->HasShutdownBeenRequested();
        }
        else
        {
            stop_working = i_job_queue->HasShutdownBeenRequested();
        }

    } while (stop_working == false);

    VERBOSE("Worker-%d shutting down", id_);
}

} // namespace jobs
} // namespace engine