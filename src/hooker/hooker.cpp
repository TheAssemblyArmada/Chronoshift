/**
 * @file
 *
 * @author CCHyper
 * @author jonwil
 * @author LRFLEW
 * @author OmniBlade
 * @author SaberHawk
 *
 * @brief Code for injecting code into the original and calling code from the original.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 *
 * Originally based on work by the Tiberian Technologies team to hook Renegade, rewritten based on work by LRFLEW for
 * OpenMC2. Provides methods for accessing data and functions in an existing binary and replacing functions with new
 * implementations from an injected DLL.
 */
#include "hooker.h"

#ifndef __WATCOMC__
#include <winuser.h>
#endif

using std::sprintf;

static const int nBinarySize = 0x00244000; // Size of game binary

// HANDLE hProcess = GetCurrentProcess();
DWORD OldProtect;

void StartHooking()
{
    VirtualProtectEx(GetCurrentProcess(), (LPVOID)0x00401000, nBinarySize, PAGE_EXECUTE_READWRITE, &OldProtect);
}

void StopHooking()
{
    DWORD OldProtect2;
    VirtualProtectEx(GetCurrentProcess(), (LPVOID)0x00401000, nBinarySize, OldProtect, &OldProtect2);
}

bool Check_Pointer(uintptr_t address, char *data)
{
    static const char *patterns[] = {
        "\x55\x89\xE5", //watcall and stdcall
        "\x53\x83", //cdecl
        "\x53\x51", //cdecl
        "\x8B\x44", //cdecl
        nullptr
    };

    bool found = false;

    for (int i = 0; i < ARRAY_SIZE(patterns); ++i) {
        if (patterns[i] != nullptr && !memcmp((char *)data, patterns[i], strlen(patterns[i]) - 1)) {
            found = true;
            break;
        }
    }

    if (!found) {
        static char buff[128];
        sprintf(buff, "Can't hook 0x%08X, function not correct\n"
        "First 4 Bytes %X %X %X %X", address, data[0], data[1], data[2], data[3]);
        MessageBoxA(NULL, buff, "Hooking Failed", MB_ICONERROR);
        return false;
    }

    return true;
}
