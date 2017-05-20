#ifndef CREATE_ACTOR_FROM_FILE_AT_LOCATION_JOB_H_
#define CREATE_ACTOR_FROM_FILE_AT_LOCATION_JOB_H_

// library includes
#include <functional>

// engine includes
#include "GameObject\Actor.h"
#include "GameObject\ActorCreator.h"
#include "Jobs\InterfaceJob.h"
#include "Math\Vec3D.h"
#include "Memory\SharedPointer.h"
#include "Util\FileUtils.h"

namespace engine {
namespace jobs {

class CreateActorFromFileAtPositionJob : public InterfaceJob
{
public:
    CreateActorFromFileAtPositionJob(const engine::data::PooledString& i_file_name, const engine::math::Vec3D& i_position, const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& i_callback);
    CreateActorFromFileAtPositionJob(const engine::util::FileUtils::FileData& i_file_data, const engine::math::Vec3D& i_position, const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& i_callback);
    ~CreateActorFromFileAtPositionJob();

    // implement InterfaceJob
    void DoWork() override;

    // accessors and mutators
    inline const engine::util::FileUtils::FileData& GetFileData() const;
    inline void SetFileData(const engine::util::FileUtils::FileData& i_file_data);

    inline const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& GetCallback() const;
    inline void SetCallback(const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& i_callback);

    inline const engine::math::Vec3D GetPosition() const;
    inline void SetPosition(const engine::math::Vec3D& i_position);

private:
    CreateActorFromFileAtPositionJob(const CreateActorFromFileAtPositionJob&) = delete;
    CreateActorFromFileAtPositionJob(CreateActorFromFileAtPositionJob&&) = delete;

    CreateActorFromFileAtPositionJob& operator=(const CreateActorFromFileAtPositionJob&) = delete;
    CreateActorFromFileAtPositionJob& operator=(CreateActorFromFileAtPositionJob&&) = delete;

    std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>                   callback_;
    engine::util::FileUtils::FileData                                                               file_data_;
    engine::math::Vec3D                                                                             position_;

}; // class ActorCreatorJob

} // namespace jobs
} // namespace engine

#include "CreateActorFromFileAtPositionJob-inl.h"

#endif // CREATE_ACTOR_FROM_FILE_AT_LOCATION_JOB_H_

