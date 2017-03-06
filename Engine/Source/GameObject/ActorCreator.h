#ifndef ACTOR_CREATOR_H_
#define ACTOR_CREATOR_H_

// library includes
#include <vector>

// engine includes
#include "Data\PooledString.h"
#include "GameObject\Actor.h"
#include "Memory\SharedPointer.h"
#include "Memory\WeakPointer.h"
#include "Physics\Physics.h"
#include "Renderer\Renderer.h"
#include "Util\FileUtils.h"

// forward declarations
struct lua_State;

namespace engine {
namespace gameobject {

class ActorCreator
{
private:
	ActorCreator() = delete;
	~ActorCreator() = delete;

	ActorCreator(const ActorCreator& i_copy) = delete;
	ActorCreator& operator=(const ActorCreator& i_copy) = delete;
	ActorCreator(ActorCreator&& i_copy) = delete;
	ActorCreator& operator=(ActorCreator&& i_copy) = delete;

public:
	static bool CreateActorFromFile(const engine::data::PooledString& i_file_name, engine::memory::SharedPointer<Actor>& o_actor);
	static bool CreateActorsFromFile(const engine::data::PooledString& i_file_name, std::vector<engine::memory::SharedPointer<Actor>>& o_actors);

	static bool CreateActorFromFileData(const engine::util::FileUtils::FileData& i_file_data, engine::memory::SharedPointer<Actor>& o_actor);
	static bool CreateActorsFromFileData(const engine::util::FileUtils::FileData& i_file_data, std::vector<engine::memory::SharedPointer<Actor>>& o_actors);

private:
	static bool CreateActor(lua_State* i_lua_state, engine::memory::SharedPointer<Actor>& o_actor);
	static bool CreatePhysicsObject(lua_State* i_lua_state, const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object, engine::memory::WeakPointer<engine::physics::PhysicsObject>& o_physics_object);
	static bool CreateRenderableObject(lua_State* i_lua_state, const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object, engine::memory::WeakPointer<engine::render::RenderableObject>& o_renderable_object);

}; // class ActorCreator

} // namespace gameobject
} // namespace engine

#endif // ACTOR_CREATOR_H_
