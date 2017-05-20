#include "Util\LuaHelper.h"

// library includes
#include <stdlib.h>
#include <string.h>

// external includes
#include "lua.hpp"

// engine includes 
#include "Assert\Assert.h"
#include "Logger\Logger.h"

namespace engine {
namespace util {

bool LuaHelper::CreateCString(lua_State* i_lua_state, const char* i_key_name, size_t i_buffer_size, char* o_buffer)
{
    // validate inputs
    ASSERT(i_lua_state);
    ASSERT(i_key_name);
    ASSERT(i_buffer_size > 0);
    ASSERT(o_buffer);

    int type = LUA_TNIL;

    // 1. Push the key
    lua_pushstring(i_lua_state, i_key_name);

    // 2. Get the associated value
    type = lua_gettable(i_lua_state, -2);
    ASSERT(type == LUA_TSTRING);

    const char* const_string = lua_tostring(i_lua_state, -1);
    ASSERT(const_string != nullptr);

    // copy the string into the output
    bool success = false;
    if (const_string)
    {
        success = true;
        sprintf_s(o_buffer, i_buffer_size, "%s", const_string);
    }
#ifdef BUILD_DEBUG
    else
    {
        LOG_ERROR("%s could not find a string for key %s", __FUNCTION__, i_key_name);
    }
#endif

    // 3. Remove the value from the stack now that we're done with it
    lua_pop(i_lua_state, 1);

    return success;
}

engine::data::PooledString LuaHelper::CreatePooledString(lua_State* i_lua_state, const char* i_key_name)
{
    // validate inputs
    ASSERT(i_lua_state);
    ASSERT(i_key_name);

    int type = LUA_TNIL;

    // 1. Push the key string
    lua_pushstring(i_lua_state, i_key_name);

    // 2. Get the associated value
    type = lua_gettable(i_lua_state, -2);
    ASSERT(type == LUA_TSTRING);
    const char* const_string = lua_tostring(i_lua_state, -1);
    ASSERT(const_string != nullptr);

    // duplicate the string for us to use
    engine::data::PooledString string(const_string ? const_string : "");
    if (!const_string)
    {
        LOG_ERROR("%s could not find a string for key %s", __FUNCTION__, i_key_name);
    }

    // 3. remove the vlaue from the stack now that we're done with it
    lua_pop(i_lua_state, 1);

    return string;
}

size_t LuaHelper::CreateFloatArray(lua_State* i_lua_state, int i_index, float* o_float_array, size_t i_num_floats)
{
    // validate inputs
    ASSERT(i_lua_state);
    ASSERT(lua_gettop(i_lua_state) >= -i_index);
    ASSERT(lua_type(i_lua_state, i_index) == LUA_TTABLE);

    size_t index = 0;

    lua_pushnil(i_lua_state);

    while (lua_next(i_lua_state, i_index - 1) != 0)
    {
        if (lua_type(i_lua_state, -1) == LUA_TNUMBER)
        {
            o_float_array[index] = float(lua_tonumber(i_lua_state, -1));
        }
        else
        {
            LOG_ERROR("%s expected a number at index %d", __FUNCTION__, index);
        }

        // pop the value now that we've retrieved it
        lua_pop(i_lua_state, 1);
        if (++index == i_num_floats)
        {
            lua_pop(i_lua_state, 1);
            break;
        }
    }
    return index;
}

float LuaHelper::CreateFloat(lua_State* i_lua_state, const char* i_key_name)
{
    // validate inputs
    ASSERT(i_lua_state);
    ASSERT(i_key_name);

    int type = LUA_TNIL;

    lua_pushstring(i_lua_state, i_key_name);
    type = lua_gettable(i_lua_state, -2);
    ASSERT(type == LUA_TNIL || type == LUA_TNUMBER);

    float return_float = float(lua_tonumber(i_lua_state, -1));

    lua_pop(i_lua_state, 1);

    return return_float;
}

int LuaHelper::CreateInt(lua_State* i_lua_state, const char* i_key_name)
{
    // validate inputs
    ASSERT(i_lua_state);
    ASSERT(i_key_name);

    int type = LUA_TNIL;

    lua_pushstring(i_lua_state, i_key_name);
    type = lua_gettable(i_lua_state, -2);
    ASSERT(type == LUA_TNIL || type == LUA_TNUMBER);

    int return_int = int(lua_tonumber(i_lua_state, -1));

    lua_pop(i_lua_state, 1);

    return return_int;
}

bool LuaHelper::CreateBool(lua_State* i_lua_state, const char* i_key_name)
{
    // validate inputs
    ASSERT(i_lua_state);
    ASSERT(i_key_name);

    int type = LUA_TNIL;

    lua_pushstring(i_lua_state, i_key_name);
    type = lua_gettable(i_lua_state, -2);
    ASSERT(type == LUA_TNIL || type == LUA_TBOOLEAN);

    bool return_bool = lua_toboolean(i_lua_state, -1) == 1;

    lua_pop(i_lua_state, 1);

    return return_bool;
}

engine::math::Vec3D LuaHelper::CreateVec3D(lua_State* i_lua_state, const char* i_key_name)
{
    // validate inputs
    ASSERT(i_lua_state);
    ASSERT(i_key_name);

    int type = LUA_TNIL;

    // 1. Push the key
    lua_pushstring(i_lua_state, i_key_name);

    // 2. Get the associated value
    type = lua_gettable(i_lua_state, -2);
    ASSERT(type == LUA_TNIL || type == LUA_TTABLE);

    engine::math::Vec3D return_vec;
    if (type == LUA_TTABLE)
    {
        float positions[3] = { 0.0f, 0.0f, 0.0f };
        CreateFloatArray(i_lua_state, -1, positions, 3);
        return_vec.set(positions[0], positions[1], positions[2]);
    }

    // 3. Remove the value from the stack now that we're done with it
    lua_pop(i_lua_state, 1);

    return return_vec;
}

engine::math::AABB LuaHelper::CreateAABB(lua_State* i_lua_state, const char* i_key_name)
{
    // validate inputs
    ASSERT(i_lua_state);
    ASSERT(i_key_name);

    int type = LUA_TNIL;

    // 1. Push the key
    lua_pushstring(i_lua_state, i_key_name);

    // 2. Get the associated value
    type = lua_gettable(i_lua_state, -2);
    ASSERT(type == LUA_TTABLE);

    float floats[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    CreateFloatArray(i_lua_state, -1, floats, 4);

    // 3. Remove the value from the stack now that we're done with it
    lua_pop(i_lua_state, 1);

    return engine::math::AABB{ engine::math::Vec3D(floats[0], floats[1]), engine::math::Vec3D(floats[2], floats[3]) };
}

engine::math::Rect LuaHelper::CreateRect(lua_State* i_lua_state, const char* i_key_name)
{
    // validate inputs
    ASSERT(i_lua_state);
    ASSERT(i_key_name);

    int type = LUA_TNIL;

    // 1. Push the key
    lua_pushstring(i_lua_state, i_key_name);

    // 2. Get the associated value
    type = lua_gettable(i_lua_state, -2);
    ASSERT(type == LUA_TTABLE);

    float positions[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    CreateFloatArray(i_lua_state, -1, positions, 4);

    // 3. Remove the value from the stack now that we're done with it
    lua_pop(i_lua_state, 1);

    return engine::math::Rect(positions[0], positions[1], positions[2], positions[3]);
}

engine::math::Transform LuaHelper::CreateTransform(lua_State* i_lua_state, const char* i_key_name)
{
    // validate inputs
    ASSERT(i_lua_state);
    ASSERT(i_key_name);

    int type = LUA_TNIL;

    // 1. Push the key
    lua_pushstring(i_lua_state, i_key_name);

    // 2. Get the associated value
    type = lua_gettable(i_lua_state, -2);
    ASSERT(type == LUA_TTABLE);

    const engine::math::Vec3D position = CreateVec3D(i_lua_state, "position");

    engine::math::Vec3D rotation = CreateVec3D(i_lua_state, "rotation");

    rotation.x(engine::math::DegreesToRadians(rotation.x()));
    rotation.y(engine::math::DegreesToRadians(rotation.y()));
    rotation.z(engine::math::DegreesToRadians(rotation.z()));

    const engine::math::Vec3D scale = CreateVec3D(i_lua_state, "scale");

    // 3. Remove the value from the stack now that we're done with it
    lua_pop(i_lua_state, 1);

    return engine::math::Transform(position, rotation, scale);
}

} // namespace util
} // namespace engine

