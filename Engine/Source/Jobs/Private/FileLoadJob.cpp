#include "Jobs\FileLoadJob.h"

// engine includes
#include "Util\FileUtils.h"

namespace engine {
namespace jobs {

FileLoadJob::FileLoadJob(const engine::data::PooledString& i_file_name, const std::function<void(const engine::util::FileUtils::FileData)>& i_callback, bool i_does_cache_file) : 
    callback_(i_callback),
    file_name_(i_file_name),
    does_cache_file_(i_does_cache_file)
{
    // validate inputs
    ASSERT(file_name_.GetLength() > 0);
    ASSERT(callback_);

    SetName("FileLoadJob");
}

FileLoadJob::~FileLoadJob()
{}

void FileLoadJob::DoWork()
{
    const engine::util::FileUtils::FileData file_data = engine::util::FileUtils::Get()->ReadFile(file_name_, does_cache_file_);
    callback_(file_data);
}

} // namespace jobs
} // namespace engine
