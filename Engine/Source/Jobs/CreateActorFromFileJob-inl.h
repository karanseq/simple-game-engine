#include "CreateActorFromFileJob.h"

// engine includes
#include "Assert\Assert.h"

namespace engine {
namespace jobs {

inline const engine::util::FileUtils::FileData& CreateActorFromFileJob::GetFileData() const
{
    return file_data_;
}

inline void CreateActorFromFileJob::SetFileData(const engine::util::FileUtils::FileData& i_file_data)
{
    ASSERT(i_file_data.file_contents);
    ASSERT(i_file_data.file_size > 0);

    file_data_ = i_file_data;
}

inline const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& CreateActorFromFileJob::GetCallback() const
{
    return callback_;
}

inline void CreateActorFromFileJob::SetCallback(const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& i_callback)
{
    ASSERT(i_callback);
    callback_ = i_callback;
}

} // namespace jobs
} // namespace engine
