// dllmain.cpp : DLL uygulamasının giriş noktasını tanımlar.
#include "pch.h"
#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include "AC.h"
#include "ACLua.h"
#include "LuaPipe.h"

HANDLE baseHandle;
DWORD baseAddress;

lua_State* L = nullptr;
LuaPipe* pipe = nullptr;

int WINAPI main();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main, NULL, 0, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

int WINAPI main()
{
    baseHandle = GetModuleHandle(NULL);
    baseAddress = (DWORD)baseHandle;

    Logger::setupConsole();

    initializeFunctions(baseAddress);
    Logger::log("\f1Attaching");
    Logger::clog(AC::localPlayer);

    pipe = new LuaPipe(TEXT("\\\\.\\pipe\\LuaExecutor"));
    pipe->init();

    L = luaL_newstate();
    ac_registerFunctions(L);

    while (1)
    {
        ScriptData data = pipe->readScript();
        if (data.failed)
            break;

        if (data.bytes == 0)
            continue;

        std::string script = data.script.data();
        if (lua_check(L, luaL_dostring(L, script.c_str())))
        {
            Logger::clog("Script successfully executed");
        }
    }

    Logger::log("\f3Disconnected");
    return 0;
}