#include "Jobs\CreateActorFromFileAtPositionJob.h"

namespace engine {
namespace jobs {

CreateActorFromFileAtPositionJob::CreateActorFromFileAtPositionJob(const engine::data::PooledString& i_file_name, 
    const engine::math::Vec3D& i_position, 
    const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& i_callback) : 
        callback_(i_callback),
        file_data_(engine::util::FileUtils::Get()->GetFileFromCache(i_file_name)),
        position_(i_position)
{
    // validate inputs
    ASSERT(file_data_.file_contents);
    ASSERT(file_data_.file_size > 0);
    ASSERT(callback_);

    SetName("CreateActorFromFileAtPositionJob");
}

CreateActorFromFileAtPositionJob::CreateActorFromFileAtPositionJob(const engine::util::FileUtils::FileData& i_file_data, 
    const engine::math::Vec3D& i_position, 
    const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& i_callback) : 
        callback_(i_callback),
        file_data_(i_file_data),
        position_(i_position)
{
    // validate inputs
    ASSERT(file_data_.file_contents);
    ASSERT(file_data_.file_size > 0);
    ASSERT(callback_);

    SetName("CreateActorFromFileAtPositionJob");
}

CreateActorFromFileAtPositionJob::~CreateActorFromFileAtPositionJob()
{}

void CreateActorFromFileAtPositionJob::DoWork()
{
    engine::memory::SharedPointer<engine::gameobject::Actor> actor;
    engine::gameobject::ActorCreator::CreateActorFromFileData(file_data_, actor);
    ASSERT(actor);
    actor->GetGameObject()->SetPosition(position_);
    callback_(actor);
}

} // namespace jobs
} // namespace engine

