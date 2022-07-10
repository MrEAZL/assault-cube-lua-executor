#pragma once
#include "Logger.h"
#include <ctime>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#pragma comment(lib, "lua54.lib")

int ac_print(lua_State* L)
{
    int args = lua_gettop(L);

    for (int i = 1; i <= args; i++)
    {
        if (lua_isstring(L, i))
        {
            const char* message = lua_tostring(L, i);
            Logger::log(message);
        }
    }

    return 0;
}

int ac_wait(lua_State* L)
{
    if (lua_isnumber(L, 1))
    {
        float time = lua_tonumber(L, 1) * 1000;

        std::clock_t start = std::clock();
        float elapsed = 0.0;

        while (elapsed < time)
            elapsed = 1000 * (std::clock() - start) / CLOCKS_PER_SEC;
    }

    return 0;
}

int ac_setRot(lua_State* L)
{
    if (lua_isnumber(L, 1) && lua_isnumber(L, 2))
    {
        float rotX = lua_tonumber(L, 1);
        float rotZ = lua_tonumber(L, 2);

        AC::localPlayer->rotX = rotX;
        AC::localPlayer->rotZ = rotZ;
    }

    return 0;
}

int ac_getRot(lua_State* L)
{
    lua_pushnumber(L, AC::localPlayer->rotZ);
    lua_pushnumber(L, AC::localPlayer->rotX);
    return 2;
}

int ac_getInfo(lua_State* L)
{
    lua_pushinteger(L, AC::localPlayer->health);
    lua_pushinteger(L, AC::localPlayer->armor);
    return 2;
}

int ac_getPos(lua_State* L)
{
    lua_pushnumber(L, AC::localPlayer->x);
    lua_pushnumber(L, AC::localPlayer->y);
    lua_pushnumber(L, AC::localPlayer->z);
    return 3;
}

int ac_getEyePos(lua_State* L)
{
    lua_pushnumber(L, AC::localPlayer->eyeX);
    lua_pushnumber(L, AC::localPlayer->eyeY);
    lua_pushnumber(L, AC::localPlayer->eyeZ);
    return 3;
}

bool lua_check(lua_State* L, int s)
{
    if (s != LUA_OK)
    {
        std::string errmsg = lua_tostring(L, -1);
        errmsg.insert(0, "\f3");

        Logger::log(errmsg.c_str());
        return false;
    }

    return true;
}

void ac_registerFunctions(lua_State* L)
{
    lua_register(L, "print", ac_print);
    lua_register(L, "wait", ac_wait);

    lua_register(L, "setRot", ac_setRot);

    lua_register(L, "getInfo", ac_getInfo);
    lua_register(L, "getRot", ac_getRot);
    lua_register(L, "getPos", ac_getPos);
    lua_register(L, "getEyePos", ac_getEyePos);
}