#include "Jobs\CreateActorFromFileJob.h"

namespace engine {
namespace jobs {

CreateActorFromFileJob::CreateActorFromFileJob(const engine::data::PooledString& i_file_name, const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& i_callback) : 
    callback_(i_callback),
    file_data_(engine::util::FileUtils::Get()->GetFileFromCache(i_file_name))
{
    // validate inputs
    ASSERT(file_data_.file_contents);
    ASSERT(file_data_.file_size > 0);
    ASSERT(callback_);

    SetName("CreateActorFromFileJob");
}

CreateActorFromFileJob::CreateActorFromFileJob(const engine::util::FileUtils::FileData& i_file_data, const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& i_callback) : 
    callback_(i_callback),
    file_data_(i_file_data)
{
    // validate inputs
    ASSERT(file_data_.file_contents);
    ASSERT(file_data_.file_size > 0);
    ASSERT(callback_);

    SetName("CreateActorFromFileJob");
}

CreateActorFromFileJob::~CreateActorFromFileJob()
{}

void CreateActorFromFileJob::DoWork()
{
    engine::memory::SharedPointer<engine::gameobject::Actor> actor;
    engine::gameobject::ActorCreator::CreateActorFromFileData(file_data_, actor);
    callback_(actor);
}

} // namespace jobs
} // namespace engine

