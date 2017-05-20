#ifndef CREATE_ACTOR_FROM_FILE_JOB_H_
#define CREATE_ACTOR_FROM_FILE_JOB_H_

// library includes
#include <functional>

// engine includes
#include "GameObject\Actor.h"
#include "GameObject\ActorCreator.h"
#include "Jobs\InterfaceJob.h"
#include "Memory\SharedPointer.h"
#include "Util\FileUtils.h"

namespace engine {
namespace jobs {

class CreateActorFromFileJob : public InterfaceJob
{
public:
    CreateActorFromFileJob(const engine::data::PooledString& i_file_name, const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& i_callback);
    CreateActorFromFileJob(const engine::util::FileUtils::FileData& i_file_data, const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& i_callback);
    ~CreateActorFromFileJob();

    // implement InterfaceJob
    void DoWork() override;

    // accessors and mutators
    inline const engine::util::FileUtils::FileData& GetFileData() const;
    inline void SetFileData(const engine::util::FileUtils::FileData& i_file_data);

    inline const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& GetCallback() const;
    inline void SetCallback(const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& i_callback);

private:
    CreateActorFromFileJob(const CreateActorFromFileJob&) = delete;
    CreateActorFromFileJob(CreateActorFromFileJob&&) = delete;

    CreateActorFromFileJob& operator=(const CreateActorFromFileJob&) = delete;
    CreateActorFromFileJob& operator=(CreateActorFromFileJob&&) = delete;

    std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>                   callback_;
    engine::util::FileUtils::FileData                                                               file_data_;

}; // class ActorCreatorJob

} // namespace jobs
} // namespace engine

#include "CreateActorFromFileJob-inl.h"

#endif // CREATE_ACTOR_FROM_FILE_JOB_H_
