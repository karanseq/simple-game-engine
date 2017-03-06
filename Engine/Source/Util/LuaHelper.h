#ifndef LUA_HELPER_H_
#define LUA_HELPER_H_

// engine includes
#include "Data\PooledString.h"
#include "Math\Rect.h"
#include "Math\Transform.h"
#include "Math\Vec3D.h"

// forward declarations
struct lua_State;

namespace engine {
namespace util {

class LuaHelper
{
private:
	LuaHelper() = delete;
	~LuaHelper() = delete;

	LuaHelper(const LuaHelper& i_copy) = delete;
	LuaHelper& operator=(const LuaHelper& i_copy) = delete;
	LuaHelper(LuaHelper&& i_copy) = delete;
	LuaHelper& operator=(LuaHelper&& i_copy) = delete;

public:
	// helpers that extract strings
	static bool CreateCString(lua_State* i_lua_state, const char* i_key_name, size_t i_buffer_size, char* o_buffer);
	static engine::data::PooledString CreatePooledString(lua_State* i_lua_state, const char* i_key_name);

	// helpers that extract numbers
	static size_t CreateFloatArray(lua_State* i_lua_state, int i_index, float* o_float_array, size_t i_num_floats);
	static float CreateFloat(lua_State* i_lua_state, const char* i_key_name);
	static int CreateInt(lua_State* i_lua_state, const char* i_key_name);

	// helpers that extract geometry
	static engine::math::Vec3D CreateVec3D(lua_State* i_lua_state, const char* i_key_name);
	static engine::math::Rect CreateRect(lua_State* i_lua_state, const char* i_key_name);
	static engine::math::Transform CreateTransform(lua_State* i_lua_state, const char* i_key_name);

}; // class LuaHelper

} // namespace util
} // namespace engine

#endif // LUA_HELPER_H_
