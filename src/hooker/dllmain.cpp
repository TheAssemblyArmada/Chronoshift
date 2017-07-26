////////////////////////////////////////////////////////////////////////////////
//                            --  REDALERT++ --                               //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: Redalert++
//
//          File:: DLLMAIN.CPP
//
//        Author:: OmniBlade
//
//  Contributors:: CCHyper
//
//   Description:: Defines the entry point for the DLL application.
//
//       License:: Redalert++ is free software: you can redistribute it and/or 
//                 modify it under the terms of the GNU General Public License 
//                 as published by the Free Software Foundation, either version 
//                 2 of the License, or (at your option) any later version.
//
//                 A full copy of the GNU General Public License can be found in
//                 LICENSE
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////
#include "alloc.h"
#include "hooker.h"
#include "gamedebug.h"
#include <windows.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

class StaticInitObject
{
public:
    StaticInitObject();
    ~StaticInitObject();
};

StaticInitObject g_initHooks;

void Setup_Hooks()
{
    //
    // Hook WinMain
    //
    //Hook_StdCall_Function((Make_StdCall_Ptr<int, HINSTANCE, HINSTANCE, LPSTR, int>(0x00401700)), Main_Func);

    // Hooking memory allocation functions.
    Hook_Function((void*)0x005C5965, (void*)&malloc);
    Hook_Function((void*)0x005C3945, (void*)&free);
    Hook_Function((void*)0x005DE4DE, (void*)&realloc);
    Hook_Function((void*)0x005D5FC0, (void*)&Alloc);
    Hook_Function((void*)0x005D6010, (void*)&Free);
    Hook_Function((void*)0x005D6020, (void*)&Resize_Alloc);
}

StaticInitObject::StaticInitObject()
{
    printf("Initialising DLL.\n");
    DEBUG_INIT(DEBUG_LOG_TO_FILE);
    DEBUG_LOG("Attaching DLL.\n");
    StartHooking();
    Setup_Hooks();
}

StaticInitObject::~StaticInitObject()
{
    DEBUG_STOP();
    StopHooking();
}

// Use DLLMain to Set up our hooks when the DLL loads. The launcher should stall
// the main thread at the entry point so hooked code called after that should
// be our code.
// Watcom doesn't seem to want to run this so we use the static class ctors and dtors above to run our code instead.
BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch ( ul_reason_for_call ) {

        case DLL_PROCESS_ATTACH:
			//StartHooking();
            //Setup_Hooks();
            //printf("Initialising DLL.\n");
            //DEBUG_INIT(DEBUG_LOG_TO_FILE);
            //DEBUG_LOG("Attaching DLL.\n");
            break;

        case DLL_PROCESS_DETACH:
            //DEBUG_STOP();
            //StopHooking();
            break;
            
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        default:
            break;

    }

    return TRUE;
}
