#ifndef ACTOR_CREATOR_JOB_H_
#define ACTOR_CREATOR_JOB_H_

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

class CreateActorDeleteFileDataJob : public InterfaceJob
{
public:
	CreateActorDeleteFileDataJob(const engine::util::FileUtils::FileData& i_file_data, const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& i_callback);
	~CreateActorDeleteFileDataJob();

	// implement InterfaceJob
	void DoWork() override;

	// accessors and mutators
	inline const engine::util::FileUtils::FileData& GetFileData() const;
	inline void SetFileData(const engine::util::FileUtils::FileData& i_file_data);

	inline const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& GetCallback() const;
	inline void SetCallback(const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& i_callback);

private:
	CreateActorDeleteFileDataJob(const CreateActorDeleteFileDataJob&) = delete;
	CreateActorDeleteFileDataJob(CreateActorDeleteFileDataJob&&) = delete;

	CreateActorDeleteFileDataJob& operator=(const CreateActorDeleteFileDataJob&) = delete;
	CreateActorDeleteFileDataJob& operator=(CreateActorDeleteFileDataJob&&) = delete;

	engine::util::FileUtils::FileData																file_data_;
	std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>					callback_;

}; // class ActorCreatorJob

} // namespace jobs
} // namespace engine

#include "CreateActorDeleteFileDataJob-inl.h"

#endif // ACTOR_CREATOR_JOB_H_
