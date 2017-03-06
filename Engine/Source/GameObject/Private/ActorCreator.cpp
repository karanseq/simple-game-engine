#include "GameObject\ActorCreator.h"

// external includes
#include "lua.hpp"

// engine includes
#include "Assert\Assert.h"
#include "Data\PooledString.h"
#include "Jobs\FileLoadJob.h"
#include "Jobs\JobSystem.h"
#include "Util\FileUtils.h"
#include "Util\LuaHelper.h"

namespace engine {
namespace gameobject {

bool ActorCreator::CreateActorFromFile(const engine::data::PooledString& i_file_name, engine::memory::SharedPointer<Actor>& o_actor)
{
	// validate input
	ASSERT(i_file_name.GetLength() > 0);

	// read the lua file
	engine::util::FileUtils::FileData file_data = engine::util::FileUtils::Get()->ReadFile(i_file_name, false);
	ASSERT(file_data.file_contents && file_data.file_size > 0);

	CreateActorFromFileData(file_data, o_actor);

	delete[] file_data.file_contents;

	return true;
}

bool ActorCreator::CreateActorsFromFile(const engine::data::PooledString& i_file_name, std::vector<engine::memory::SharedPointer<Actor>>& o_actors)
{
	// validate input
	ASSERT(i_file_name.GetLength() > 0);

	// read the lua file
	engine::util::FileUtils::FileData file_data = engine::util::FileUtils::Get()->ReadFile(i_file_name, false);
	ASSERT(file_data.file_contents && file_data.file_size > 0);

	CreateActorsFromFileData(file_data, o_actors);

	delete[] file_data.file_contents;

	return true;
}

bool ActorCreator::CreateActorFromFileData(const engine::util::FileUtils::FileData& i_file_data, engine::memory::SharedPointer<Actor>& o_actor)
{
	ASSERT(i_file_data.file_contents && i_file_data.file_size > 0);

	// initialize lua state
	lua_State* lua_state = luaL_newstate();
	ASSERT(lua_state);

	luaL_openlibs(lua_state);

	// do initial buffer parsing
	int result = 0;
	result = luaL_loadbuffer(lua_state, reinterpret_cast<char*>(i_file_data.file_contents), i_file_data.file_size, nullptr);
	ASSERT(result == 0);
	lua_pcall(lua_state, 0, 0, 0);
	ASSERT(result == 0);

	int type = LUA_TNIL;

	// find the global variable named "Actor" & push it onto stack
	// if not found, it pushes nil value instead
	result = lua_getglobal(lua_state, "Actor");
	ASSERT(result == LUA_TTABLE);

	bool success = CreateActor(lua_state, o_actor);

	// pop "Actor" table
	lua_pop(lua_state, 1);

	// validate that the stack is clean
	int stack_items = lua_gettop(lua_state);
	ASSERT(stack_items == 0);

	lua_close(lua_state);

	return success;
}

bool ActorCreator::CreateActorsFromFileData(const engine::util::FileUtils::FileData& i_file_data, std::vector<engine::memory::SharedPointer<Actor>>& o_actors)
{
	ASSERT(i_file_data.file_contents && i_file_data.file_size > 0);

	// initialize lua state
	lua_State* lua_state = luaL_newstate();
	ASSERT(lua_state);

	luaL_openlibs(lua_state);

	// do initial buffer parsing
	int result = 0;
	result = luaL_loadbuffer(lua_state, reinterpret_cast<char*>(i_file_data.file_contents), i_file_data.file_size, nullptr);
	ASSERT(result == 0);
	lua_pcall(lua_state, 0, 0, 0);
	ASSERT(result == 0);

	int type = LUA_TNIL;

	// find the global variable named "ActorList" & push it onto stack
	// if not found, it pushes nil value instead
	result = lua_getglobal(lua_state, "ActorList");
	ASSERT(result == LUA_TTABLE);

	size_t index = 0;

	lua_pushnil(lua_state);

	while (lua_next(lua_state, -2) != 0)
	{
		if (lua_type(lua_state, -1) == LUA_TTABLE)
		{
			engine::memory::SharedPointer<Actor> actor;
			if (CreateActor(lua_state, actor))
			{
				o_actors.push_back(actor);
			}
			else
			{
				LOG_ERROR("%s could not create an Actor at index %d", __FUNCTION__, index);
			}
		}
		else
		{
			LOG_ERROR("%s expected a table at index %d", __FUNCTION__, index);
		}

		// remove value
		lua_pop(lua_state, 1);
		++index;
	}

	// pop "ActorList" table
	lua_pop(lua_state, 1);

	// validate that the stack is clean
	int stack_items = lua_gettop(lua_state);
	ASSERT(stack_items == 0);

	lua_close(lua_state);

	return true;
}

bool ActorCreator::CreateActor(lua_State* i_lua_state, engine::memory::SharedPointer<Actor>& o_actor)
{
	// validate input
	ASSERT(i_lua_state);

	// get the actor's name
	const auto name = engine::util::LuaHelper::CreatePooledString(i_lua_state, "name");

	// get the actor's type
	const auto type = engine::data::HashedString(engine::util::LuaHelper::CreatePooledString(i_lua_state, "type"));

	// create the actor
	o_actor = engine::gameobject::Actor::Create(name, type);

	// get the actor's bounding box
	const auto aabb = engine::util::LuaHelper::CreateRect(i_lua_state, "bounding_box");

	// get the actor's transform
	const auto transform = engine::util::LuaHelper::CreateTransform(i_lua_state, "transform");

	// create the game object
	const auto game_object = engine::gameobject::GameObject::Create(aabb, transform);
	o_actor->SetGameObject(game_object);

	// create a physics object
	engine::memory::WeakPointer<engine::physics::PhysicsObject> physics_object;
	CreatePhysicsObject(i_lua_state, game_object, physics_object);
	o_actor->SetPhysicsObject(physics_object);

	// create a renderable object
	engine::memory::WeakPointer<engine::render::RenderableObject> renderable_object;
	CreateRenderableObject(i_lua_state, game_object, renderable_object);
	o_actor->SetRenderableObject(renderable_object);

	return true;
}

bool ActorCreator::CreatePhysicsObject(lua_State* i_lua_state, const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object, engine::memory::WeakPointer<engine::physics::PhysicsObject>& o_physics_object)
{
	// validate inputs
	ASSERT(i_lua_state);
	ASSERT(i_game_object);

	int type = LUA_TNIL;

	// check if this actor has physics settings
	lua_pushstring(i_lua_state, "physics_settings");
	type = lua_gettable(i_lua_state, -2);
	ASSERT(type == LUA_TNIL || type == LUA_TTABLE);

	bool has_physics = false;
	float physics_mass = 0.0f, physics_drag = 0.0f;

	if (type == LUA_TTABLE)
	{
		has_physics = true;

		float mass = engine::util::LuaHelper::CreateFloat(i_lua_state, "mass");
		float drag = engine::util::LuaHelper::CreateFloat(i_lua_state, "drag");

		o_physics_object = engine::physics::Physics::Get()->CreatePhysicsObject(i_game_object, mass, drag);
	}

	// pop the physics settings value
	lua_pop(i_lua_state, 1);

	return has_physics;
}

bool ActorCreator::CreateRenderableObject(lua_State* i_lua_state, const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object, engine::memory::WeakPointer<engine::render::RenderableObject>& o_renderable_object)
{
	// validate inputs
	ASSERT(i_lua_state);
	ASSERT(i_game_object);

	int type = LUA_TNIL;

	// check if this actor has render settings
	lua_pushstring(i_lua_state, "render_settings");
	type = lua_gettable(i_lua_state, -2);
	ASSERT(type == LUA_TNIL || type == LUA_TTABLE);

	bool is_renderable = false;

	if (type == LUA_TTABLE)
	{
		is_renderable = true;

		const size_t file_name_length = 512;
		char file_name_buffer[file_name_length];
		engine::util::LuaHelper::CreateCString(i_lua_state, "sprite_name", file_name_length, file_name_buffer);

		o_renderable_object = engine::render::Renderer::Get()->CreateRenderableObject(file_name_buffer, i_game_object);
	}

	// pop the render settings value
	lua_pop(i_lua_state, 1);

	return is_renderable;
}

} // namespace gameobject
} // namespace engine
