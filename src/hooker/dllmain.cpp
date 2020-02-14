/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Entry point for the DLL and place where we hook into the original code.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include <captainslog.h>

#include "hooker.h"
#include "setuphooks.h"
#include <cstdio>

using std::printf;

class StaticInitObject
{
public:
    StaticInitObject();
    ~StaticInitObject();
};

StaticInitObject g_initHooks;

StaticInitObject::StaticInitObject()
{
    printf("Initialising DLL.\n");
    StartHooking();
    Setup_Hooks();
}

StaticInitObject::~StaticInitObject()
{
    StopHooking();
}

// Use DLLMain to Set up our hooks when the DLL loads. The launcher should stall
// the main thread at the entry point so hooked code called after that should
// be our code.
// Watcom doesn't seem to want to run this so we use the static class ctors and dtors above to run our code instead.
BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            // StartHooking();
            // Setup_Hooks();
            // printf("Initialising DLL.\n");
            // DEBUG_INIT(captainslog_debug_TO_FILE);
            // captainslog_debug("Attaching DLL.");
            break;

        case DLL_PROCESS_DETACH:
            // DEBUG_STOP();
            // StopHooking();
            break;

        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        default:
            break;
    }

    return TRUE;
}
