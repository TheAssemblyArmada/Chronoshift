/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Entry point for the DLL and place where we hook into the original code.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "alloc.h"
#include "blitters.h"
#include "ccfileclass.h"
#include "controlc.h"
#include "cpudetect.h"
#include "fading.h"
#include "gadget.h"
#include "gamedebug.h"
#include "gbuffer.h"
#include "hooker.h"
#include "ini.h"
#include "interpolate.h"
#include "keyboard.h"
#include "lcw.h"
#include "mixfile.h"
#include "mouse.h"
#include "mouseshape.h"
#include "ostimer.h"
#include "rawfileclass.h"
#include "rgb.h"
#include "shape.h"
#include "textprint.h"
#include "surfacemonitor.h"
#include "wsa.h"
#include "xordelta.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

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
    // Hook_StdCall_Function((Make_StdCall_Ptr<int, HINSTANCE, HINSTANCE, LPSTR, int>(0x00401700)), Main_Func);

    // Hooking memory allocation functions.
    Memory_Hook_Me();
    RawFileClass::Hook_Me();
    BufferIOFileClass::Hook_Me();
    CCFileClass::Hook_Me();
    ControlClass::Hook_Me();
    GraphicViewPortClass::Hook_Me();
    INIClass::Hook_Me();
    KeyboardClass::Hook_Me();
    PlatformTimerClass::Hook_Me();
    Blitters::Hook_Me();
    Fading::Hook_Me();
    GadgetClass::Hook_Me();
    Interpolate::Hook_Me();
    Lcw::Hook_Me();
    MouseShape::Hook_Me();
    Shape::Hook_Me();
    WSAFile::Hook_Me();
    WWMouseClass::Hook_Me();
    XorDelta::Hook_Me();
    SurfaceMonitorClass::Hook_Me();
    Hook_Function(0x005B42F4, Buffer_Print);
    Hook_Function(0x005B96F0, &MixFileClass<CCFileClass>::Offset);
    Hook_Function(0x005B9330, &MixFileClass<CCFileClass>::Retrieve);
    //Hook_Function(0x005E5200, (void *)0x005E53CD); // This one forces better interpolation algo.
}

StaticInitObject::StaticInitObject()
{
    printf("Initialising DLL.\n");
    DEBUG_INIT(DEBUG_LOG_TO_FILE);
    StartHooking();
    Setup_Hooks();

    if (CPUDetectClass::Has_CPUID_Instruction()) {
        DEBUG_LOG("%s", CPUDetectClass::Get_Processor_Log());
    } else {
        DEBUG_LOG("Doesn't look like CPUDetect is initialised yet.\n");
    }
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
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            // StartHooking();
            // Setup_Hooks();
            // printf("Initialising DLL.\n");
            // DEBUG_INIT(DEBUG_LOG_TO_FILE);
            // DEBUG_LOG("Attaching DLL.\n");
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
