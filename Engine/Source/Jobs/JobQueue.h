#ifndef JOB_QUEUE_H_
#define JOB_QUEUE_H_

// library includes
#include <condition_variable>
#include <mutex>
#include <queue>

// engine includes
#include "Data\PooledString.h"

namespace engine {
namespace jobs {

// forward declaration
class InterfaceJob;

class JobQueue
{
public:
	JobQueue(const engine::data::PooledString& i_id);
	~JobQueue();

	bool AddJob(InterfaceJob* i_new_job);
	InterfaceJob* GetJob();

	inline void RequestShutdown();
	inline bool HasShutdownBeenRequested() const;

	inline const engine::data::PooledString& GetID() const;
	inline void SetID(const engine::data::PooledString& i_id);

private:
	JobQueue(const JobQueue&) = delete;
	JobQueue(JobQueue&&) = delete;

	JobQueue& operator=(const JobQueue&) = delete;
	JobQueue& operator=(JobQueue&&) = delete;

	engine::data::PooledString					id_;
	std::condition_variable						start_searching_;

	std::mutex									shutdown_mutex_;
	bool										shutdown_requested_;

	std::mutex									queue_mutex_;
	std::queue<InterfaceJob*>					job_queue_;

}; // class JobQueue

} // namespace jobs
} // namespace engine

#include "JobQueue-inl.h"

#endif // JOB_QUEUE_H_