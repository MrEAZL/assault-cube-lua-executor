#pragma once
#include <Windows.h>

struct playerEntity
{
    char padding0[4];
    float eyeX;
    float eyeY;
    float eyeZ;
    char padding1[36];
    float x;
    float y;
    float z;
    float rotZ;
    float rotX;
    char padding2[176];
    int health;
    int armor;
};

namespace Addresses
{
    namespace AC
    {
        DWORD chat = 0x6B060;
        DWORD player = 0x10F4F4;
    }
}

namespace AC
{
    typedef int(__cdecl* chatFunction)(const char* Format, char* ArgList);

    chatFunction chat = nullptr;
    playerEntity* localPlayer = nullptr;
}

void initializeFunctions(DWORD baseAddress)
{
    AC::chat = (AC::chatFunction)(baseAddress + Addresses::AC::chat);
    AC::localPlayer = (playerEntity*)(*(void**)(baseAddress + Addresses::AC::player));
}