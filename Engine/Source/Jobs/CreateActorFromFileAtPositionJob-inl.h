#include "CreateActorFromFileAtPositionJob.h"

// engine includes
#include "Assert\Assert.h"

namespace engine {
namespace jobs {

inline const engine::util::FileUtils::FileData& CreateActorFromFileAtPositionJob::GetFileData() const
{
    return file_data_;
}

inline void CreateActorFromFileAtPositionJob::SetFileData(const engine::util::FileUtils::FileData& i_file_data)
{
    ASSERT(i_file_data.file_contents);
    ASSERT(i_file_data.file_size > 0);

    file_data_ = i_file_data;
}

inline const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& CreateActorFromFileAtPositionJob::GetCallback() const
{
    return callback_;
}

inline void CreateActorFromFileAtPositionJob::SetCallback(const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& i_callback)
{
    ASSERT(i_callback);
    callback_ = i_callback;
}

inline const engine::math::Vec3D CreateActorFromFileAtPositionJob::GetPosition() const
{
    return position_;
}

inline void CreateActorFromFileAtPositionJob::SetPosition(const engine::math::Vec3D& i_position)
{
    position_ = i_position;
}

} // namespace jobs
} // namespace engine
