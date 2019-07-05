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
#include "alloc.h"
#include "blitters.h"
#include "cd.h"
#include "cdfile.h"
#include "cell.h"
#include "controlc.h"
#include "coord.h"
#include "cpudetect.h"
#include "dialog.h"
#include "display.h"
#include "drawshape.h"
#include "facing.h"
#include "fading.h"
#include "flasher.h"
#include "foot.h"
#include "gadget.h"
#include "gamedebug.h"
#include "gamefile.h"
#include "gamekeyboard.h"
#include "gbuffer.h"
#include "heap.h"
#include "hooker.h"
#include "house.h"
#include "ini.h"
#include "init.h"
#include "interpolate.h"
#include "iomap.h"
#include "keyboard.h"
#include "lcw.h"
#include "list.h"
#include "lzopipe.h"
#include "lzostraw.h"
#include "main.h"
#include "mixfile.h"
#include "mouse.h"
#include "mouseshape.h"
#include "msgbox.h"
#include "msglist.h"
#include "options.h"
#include "ostimer.h"
#include "power.h"
#include "radar.h"
#include "rawfile.h"
#include "rgb.h"
#include "scenario.h"
#include "shape.h"
#include "sidebar.h"
#include "slider.h"
#include "surfacemonitor.h"
#include "techno.h"
#include "textbtn.h"
#include "textprint.h"
#include "toggle.h"
#include "version.h"
#include "wsa.h"
#include "xordelta.h"
#include "gameloop.h"
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
    // Disable the DirectDraw based terrain caching system.
    Make_Global<BOOL>(0x0060BA6C) = false; // IconCacheAllowed

    //
    // Hook WinMain
    //
    Hook_Function(0x00551A70, Main_Func);

    // Hooking memory allocation functions.
    Memory_Hook_Me();
    RawFileClass::Hook_Me();
    BufferIOFileClass::Hook_Me();
    GameFileClass::Hook_Me();
    CDFileClass::Hook_Me();
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
    TextButtonClass::Hook_Me();
    ToggleClass::Hook_Me();
    WSAFile::Hook_Me();
    MouseClass::Hook_Me();
    XorDelta::Hook_Me();
    SurfaceMonitorClass::Hook_Me();
    SliderClass::Hook_Me();
    ListClass::Hook_Me();
    FixedHeapClass::Hook_Me();
    FixedIHeapClass::Hook_Me();
    LZOPipe::Hook_Me();
    LZOStraw::Hook_Me();
    VersionClass::Hook_Me();
    FacingClass::Hook_Me();
    FlasherClass::Hook_Me();
    OptionsClass::Hook_Me();
    DisplayClass::Hook_Me();
    RadarClass::Hook_Me();
    PowerClass::Hook_Me();
    SidebarClass::Hook_Me();
    TabClass::Hook_Me();
    HelpClass::Hook_Me();
    ScrollClass::Hook_Me();
    GameMouseClass::Hook_Me();
    CellClass::Hook_Me();
    DoorClass::Hook_Me();
    LayerClass::Hook_Me();
    MapClass::Hook_Me();
    ScenarioClass::Hook_Me();
    MissionClass::Hook_Me();
    MessageBoxClass::Hook_Me();
    MessageListClass::Hook_Me();
    RadioClass::Hook_Me();
    TechnoClass::Hook_Me();
    HouseClass::Hook_Me();
    FootClass::Hook_Me();
    Hook_Function(0x005B42F4, Buffer_Print);
    Hook_Function(0x005B96F0, &MixFileClass<GameFileClass>::Offset);
    Hook_Function(0x005B9330, &MixFileClass<GameFileClass>::Retrieve);
    Hook_Function(0x004A96E8, CC_Draw_Shape);
    // Hook_Function(0x004AD670, Dialog_Box);
    Dialog::Hook_Me();
    // Hook_Function(0x004AC798, Coord_Move);
    Hook_Function(0x004AC814, Move_Point);
    // Hook_Function(0x005E5200, (void *)0x005E53CD); // This one forces better interpolation algo.
    Init::Hook_Me();
    Hook_Function(0x004AAC58, &Force_CD_Available);
    Hook_Function(0x004A7C74, Main_Loop);
    Hook_Function(0x004A56D8, Keyboard_Process);
}

StaticInitObject::StaticInitObject()
{
    printf("Initialising DLL.\n");
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
