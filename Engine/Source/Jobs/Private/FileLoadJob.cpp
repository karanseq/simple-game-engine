#include "Jobs\FileLoadJob.h"

// engine includes
#include "Util\FileUtils.h"

namespace engine {
namespace jobs {

FileLoadJob::FileLoadJob(const engine::data::PooledString& i_file_name, const std::function<void(const engine::util::FileUtils::FileData)>& i_callback) : file_name_(i_file_name),
	callback_(i_callback)
{
	// validate inputs
	ASSERT(file_name_.GetLength() > 0);
	ASSERT(callback_);
}

FileLoadJob::~FileLoadJob()
{}

void FileLoadJob::DoWork()
{
	engine::util::FileUtils::FileData file_data = engine::util::FileUtils::Get()->ReadFile(file_name_, false);
	callback_(file_data);
}

} // namespace jobs
} // namespace engine
