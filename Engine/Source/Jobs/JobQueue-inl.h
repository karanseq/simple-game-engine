#include "JobQueue.h"

namespace engine {
namespace jobs {

inline void JobQueue::RequestShutdown()
{
	std::lock_guard<std::mutex> lock(shutdown_mutex_);
	shutdown_requested_ = true;
	start_searching_.notify_all();
}

inline bool JobQueue::HasShutdownBeenRequested() const
{
	return shutdown_requested_;
}

inline const engine::data::PooledString& JobQueue::GetID() const
{
	return id_;
}

inline void JobQueue::SetID(const engine::data::PooledString& i_id)
{
	id_ = i_id;
}

} // namespace jobs
} // namespace engine