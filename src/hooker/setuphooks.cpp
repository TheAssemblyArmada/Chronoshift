/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Function to hook all required functions.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "setuphooks.h"
#include "abstract.h"
#include "aircraft.h"
#include "aircrafttype.h"
#include "alloc.h"
#include "anim.h"
#include "animtype.h"
#include "bfiofile.h"
#include "blitters.h"
#include "building.h"
#include "buildingtype.h"
#include "bullettype.h"
#include "cd.h"
#include "cdfile.h"
#include "cell.h"
#include "controlc.h"
#include "coord.h"
#include "cpudetect.h"
#include "dialog.h"
#include "difficulty.h"
#include "display.h"
#include "drawshape.h"
#include "drive.h"
#include "droplist.h"
#include "facing.h"
#include "factory.h"
#include "fading.h"
#include "flasher.h"
#include "foot.h"
#include "gadget.h"
#include "gameevent.h"
#include "gamefile.h"
#include "gameini.h"
#include "gamekeyboard.h"
#include "gameloop.h"
#include "gbuffer.h"
#include "gmouse.h"
#include "ground.h"
#include "heap.h"
#include "help.h"
#include "hooker.h"
#include "house.h"
#include "infantry.h"
#include "infantrytype.h"
#include "ini.h"
#include "init.h"
#include "initvideo.h"
#include "interpolate.h"
#include "iomap.h"
#include "keyboard.h"
#include "lcw.h"
#include "list.h"
#include "lzopipe.h"
#include "lzostraw.h"
#include "main.h"
#include "missioncontrol.h"
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
#include "rules.h"
#include "scenario.h"
#include "shape.h"
#include "sidebar.h"
#include "slider.h"
#include "surfacemonitor.h"
#include "team.h"
#include "teamtype.h"
#include "techno.h"
#include "textbtn.h"
#include "textprint.h"
#include "toggle.h"
#include "unit.h"
#include "unittype.h"
#include "version.h"
#include "vessel.h"
#include "vesseltype.h"
#include "vortex.h"
#include "wsa.h"
#include "xordelta.h"
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void Setup_Hooks()
{
#ifdef __WATCOMC__
    // Disable the DirectDraw based terrain caching system.
    Make_Global<BOOL>(0x0060BA6C) = false; // IconCacheAllowed

    //
    // Hook WinMain
    //
    Hook_Function(0x00551A70, Main_Func);

    // Hooking memory allocation functions.
    Hook_Function(0x005C5965, &malloc);
    Hook_Function(0x005C3945, &free);
    Hook_Function(0x005DE4DE, &realloc);
    Hook_Function(0x005D5FC0, &Alloc);
    Hook_Function(0x005D6010, &Free);
    Hook_Function(0x005D6020, &Resize_Alloc);

    // rawfile.h
    Hook_Function(0x005C006C, *RawFileClass::Set_Name);
    Hook_Function(0x005C05F8, *RawFileClass::Create);
    Hook_Function(0x005C063C, *RawFileClass::Delete);
    Hook_Function(0x005C0100, *RawFileClass::Hook_Open);
    Hook_Function(0x005C0210, *RawFileClass::Is_Available);
    Hook_Function(0x005C0314, *RawFileClass::Read);
    Hook_Function(0x005C04E8, *RawFileClass::Seek);
    Hook_Function(0x005C056C, *RawFileClass::Size);
    Hook_Function(0x005C0430, *RawFileClass::Write);
    Hook_Function(0x005C02C8, *RawFileClass::Close);
    Hook_Function(0x005C07CC, *RawFileClass::Raw_Seek);

    // bfiofile.h
    Hook_Function(0x005BD734, *BufferIOFileClass::Is_Available);
    Hook_Function(0x005BD754, *BufferIOFileClass::Hook_Is_Open);
    Hook_Function(0x005BD780, *BufferIOFileClass::Hook_Open_Name);
    Hook_Function(0x005BD79C, *BufferIOFileClass::Hook_Open);
    Hook_Function(0x005BDAD4, *BufferIOFileClass::Read);
    Hook_Function(0x005BDCF4, *BufferIOFileClass::Seek);
    Hook_Function(0x005BDDF0, *BufferIOFileClass::Size);
    Hook_Function(0x005BD870, *BufferIOFileClass::Write);
    Hook_Function(0x005BDE14, *BufferIOFileClass::Close);

    // gamefile.h
    Hook_Function(0x00462A30, *GameFileClass::Is_Available);
    Hook_Function(0x00462A80, *GameFileClass::Hook_Is_Open);
    Hook_Function(0x00426400, *GameFileClass::Hook_Open_Name);
    Hook_Function(0x00462AD4, *GameFileClass::Hook_Open);
    Hook_Function(0x004628B0, *GameFileClass::Read);
    Hook_Function(0x00462958, *GameFileClass::Seek);
    Hook_Function(0x004629CC, *GameFileClass::Size);
    Hook_Function(0x00462860, *GameFileClass::Write);
    Hook_Function(0x00462AA8, *GameFileClass::Close);
    Hook_Function(0x00462BD8, *GameFileClass::Get_Date_Time);
    Hook_Function(0x00462C50, *GameFileClass::Set_Date_Time);
    Hook_Function(0x00462840, *GameFileClass::Error);

    // cdfile.h
    Hook_Function(0x005BFC60, &CDFileClass::Set_Search_Drives);

    ControlClass::Hook_Me();
    
    // gbuffer.h
    Hook_Function(0x005C0AF4, *GraphicBufferClass::DD_Init);
    Hook_Function(0x005C0C2D, *GraphicBufferClass::Init);
    Hook_Function(0x005C0D0F, *GraphicBufferClass::Un_Init);
    Hook_Function(0x005C101A, *GraphicBufferClass::Lock);
    Hook_Function(0x005C1191, *GraphicBufferClass::Unlock);

    // keyboard.h
    Hook_Function(0x005B82FC, *KeyboardClass::Message_Handler);
    // Hook_Function(0x005B7408, *KeyboardClass::Fill);
    Hook_Function(0x005B7F30, *KeyboardClass::Check);
    Hook_Function(0x005B7F5C, *KeyboardClass::Get);
    Hook_Function(0x005B82CC, *KeyboardClass::Clear);
    
    // ostimer.h
    Hook_Function(0x005BBEB0, &PlatformTimerClass::Timer_Callback);

    // shastraw.h
    Hook_Function(0x005D5B04, *SHAStraw::Result);
    Hook_Function(0x005D5AD0, *SHAStraw::Get);

    // blitters.h
    Hook_Function(0x005C53E4, Linear_Blit_To_Linear);
    Hook_Function(0x005C23F0, Buffer_Fill_Rect);
    Hook_Function(0x005C13E4, Buffer_Draw_Line);
    Hook_Function(0x005C4BC4, Buffer_Remap);
    Hook_Function(0x005CEE10, Buffer_Get_Pixel);
    Hook_Function(0x005CC2B4, Buffer_Put_Pixel);
    Hook_Function(0x005D0F60, Buffer_To_Buffer);
    Hook_Function(0x005D10B8, Buffer_To_Page);
    Hook_Function(0x005C4DE0, Buffer_Clear);
    Hook_Function(0x005D4338, Linear_Scale_To_Linear);

    //fading.h
    Hook_Function(0x004FB914, Make_Fading_Table);
    Hook_Function(0x005CC4C0, Build_Fading_Table);
    Hook_Function(0x004FB994, Conquer_Build_Fading_Table);
    Hook_Function(0x004FB7C4, Build_Translucent_Table);
    Hook_Function(0x004FB870, Conquer_Build_Translucent_Table);

    GadgetClass::Hook_Me();
    
    // interpolate.h
    Hook_Function(0x005B2DD0, Create_Palette_Interpolation_Table);
    Hook_Function(0x005B2CE0, Read_Interpolation_Palette);
    Hook_Function(0x005B2D5C, Write_Interpolation_Palette);
    Hook_Function(0x004A8704, Load_Interpolated_Palettes);
    Hook_Function(0x004A8874, Free_Interpolated_Palettes);
    Hook_Function(0x005B2FCC, Interpolate_2X_Scale);
    
    // lcw.h
    Hook_Function(0x005D6880, LCW_Uncomp);
    Hook_Function(0x005DD28C, LCW_Comp);

    // mouseshape.h
    Hook_Function(0x005D8F5C, Mouse_Shadow_Buffer);
    Hook_Function(0x005D90A1, Mouse_Draw);
    Hook_Function(0x005D91CF, Mouse_Set_Cursor);

    // shape.h
    Hook_Function(0x005B48EC, Build_Frame);
    Hook_Function(0x005AB354, Buffer_Frame_To_Page);
    Hook_Function(0x004A9AB8, Shape_Dimensions);

    TextButtonClass::Hook_Me();
    ToggleClass::Hook_Me();
    
    // wsa.h
    Hook_Function(0x005D03C0, Open_Animation);
    Hook_Function(0x005D0820, Close_Animation);
    Hook_Function(0x005D0860, Animate_Frame);
    Hook_Function(0x005D0CF0, Apply_Delta);
    Hook_Function(0x005D0C00, Get_Animation_Size);
    Hook_Function(0x005D0C30, Get_Resident_Frame_Offset);
    Hook_Function(0x005D0C90, Get_File_Frame_Offset);
    Hook_Function(0x005D0AE0, Get_Animation_Frame_Count);
    Hook_Function(0x005D0B10, Get_Animation_X);
    Hook_Function(0x005D0B40, Get_Animation_Y);
    Hook_Function(0x005D0B70, Get_Animation_Width);
    Hook_Function(0x005D0BA0, Get_Animation_Height);
    Hook_Function(0x005D0BD0, Get_Animation_Palette);

    // mouse.h
    Hook_Function(0x005C1B10, *MouseClass::Low_Hide_Mouse);
    Hook_Function(0x005C1BF0, *MouseClass::Low_Show_Mouse);
    
    // xordelta.h
    Hook_Function(0x005D6A50, Apply_XOR_Delta_To_Page_Or_Viewport);
    Hook_Function(0x005D69E0, Apply_XOR_Delta);

    // surfacemonitor.h
#ifdef BUILD_WITH_DDRAW
    Hook_Function(0x005CA1D0, *SurfaceMonitorClass::Add_Surface);
    Hook_Function(0x005CA230, *SurfaceMonitorClass::Remove_Surface);
    Hook_Function(0x005CA280, *SurfaceMonitorClass::Got_Surface_Already);
#endif
    Hook_Function(0x005CA2D0, *SurfaceMonitorClass::Restore_Surfaces);
    Hook_Function(0x005CA370, *SurfaceMonitorClass::Set_Surface_Focus);
    Hook_Function(0x005CA390, *SurfaceMonitorClass::Release);

    SliderClass::Hook_Me();
    ListClass::Hook_Me();
    FixedHeapClass::Hook_Me();
    FixedIHeapClass::Hook_Me();
    
    // lzopipe.h
    Hook_Function(0x005D5508, *LZOPipe::Put);
    Hook_Function(0x005D57AC, *LZOPipe::Flush);
    
    // lzostraw.h
    Hook_Function(0x005D5BF4, *LZOStraw::Get);

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

    AbstractClass::Hook_Me();
    AbstractTypeClass::Hook_Me();
    AircraftTypeClass::Hook_Me();

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
    Hook_Function(0x005CA150, Wait_Blit);
    Hook_Function(0x005C9D60, Set_Video_Mode);
    Hook_Function(0x005C9E60, Reset_Video_Mode);
    Hook_Function(0x00552368, Init_Video);
    Hook_Function(0x005CA070, Set_Video_Palette);
    ChronalVortexClass::Hook_Me();
#endif
}

#if defined __WATCOMC__ && defined GAME_DLL
// Watcom has no static_assert so we are forced to use this instead
// Watcom throws "dimension cannot be negative" on missmatches
#define ASSERT_SIZEOF(structname, expectedsize) typedef char __ASSERT_SIZEOF__[(sizeof(structname) == expectedsize) ? 1 : -1]

#pragma message("Checking Type Sizes!")
//
ASSERT_SIZEOF(fixed_t, 0x2);
ASSERT_SIZEOF(AbstractClass, 0x15);
ASSERT_SIZEOF(AircraftClass, 0x15F);
ASSERT_SIZEOF(AircraftTypeClass, 0x19D);
ASSERT_SIZEOF(AnimClass, 0x4C);
ASSERT_SIZEOF(AnimTypeClass, 0x162);
ASSERT_SIZEOF(BufferIOFileClass, 0x4C);
ASSERT_SIZEOF(BuildingClass, 0xFB);
ASSERT_SIZEOF(BuildingTypeClass, 0x207);
ASSERT_SIZEOF(BulletTypeClass, 0x146);
ASSERT_SIZEOF(CDFileClass, 0x50);
ASSERT_SIZEOF(CellClass, 0x3A);
ASSERT_SIZEOF(DifficultyClass, 0x16);
ASSERT_SIZEOF(DisplayClass, 0xC88);
ASSERT_SIZEOF(DoorClass, 0x17);
ASSERT_SIZEOF(DriveClass, 0x15C);
ASSERT_SIZEOF(DropListClass, 0x1A5);
ASSERT_SIZEOF(FactoryClass, 0x2E);
ASSERT_SIZEOF(FlasherClass, 0x4);
ASSERT_SIZEOF(FlyClass, 0x5);
ASSERT_SIZEOF(FootClass, 0x141);
ASSERT_SIZEOF(GadgetClass, 0x24);
ASSERT_SIZEOF(GameEventClass, 0x14);
ASSERT_SIZEOF(GameINIClass, 0x3C);
ASSERT_SIZEOF(GaugeClass, 0x3C);
ASSERT_SIZEOF(GameMouseClass, 0x166B);
ASSERT_SIZEOF(GroundClass, 0xE);
ASSERT_SIZEOF(HelpClass, 0x1659);
ASSERT_SIZEOF(HouseClass, 0x17A8);
ASSERT_SIZEOF(InfantryClass, 0x154);
ASSERT_SIZEOF(InfantryTypeClass, 0x1A2);
ASSERT_SIZEOF(ListClass, 0x122);
ASSERT_SIZEOF(MissionControlClass, 0x9);
ASSERT_SIZEOF(MixFileClass<GameFileClass>, 0x24);
ASSERT_SIZEOF(MessageListClass, 0x9AB);
ASSERT_SIZEOF(ObjectClass, 0x27);
ASSERT_SIZEOF(ObjectTypeClass, 0x138);
ASSERT_SIZEOF(OptionsClass, 0x7C);
ASSERT_SIZEOF(OverlayTypeClass, 0x146);
ASSERT_SIZEOF(PowerClass, 0x103E);
ASSERT_SIZEOF(RadarClass, 0x100D);
ASSERT_SIZEOF(RulesClass, 0x200);
ASSERT_SIZEOF(ScenarioClass, 0x7D7);
ASSERT_SIZEOF(ScrollClass, 0x1661);
ASSERT_SIZEOF(ShapeButtonClass, 0x38);
ASSERT_SIZEOF(SidebarClass, 0x15FA);
ASSERT_SIZEOF(SliderClass, 0x54);
ASSERT_SIZEOF(SmudgeTypeClass, 0x145);
ASSERT_SIZEOF(SuperClass, 0x19);
ASSERT_SIZEOF(TabClass, 0x1620);
ASSERT_SIZEOF(TeamClass, 0x5D);
ASSERT_SIZEOF(TeamTypeClass, 0xD1);
ASSERT_SIZEOF(TechnoClass, 0xCD);
ASSERT_SIZEOF(TechnoTypeClass, 0x192);
ASSERT_SIZEOF(TerrainTypeClass, 0x14D);
ASSERT_SIZEOF(TextButtonClass, 0x3A);
ASSERT_SIZEOF(ToggleClass, 0x30);
ASSERT_SIZEOF(TDelayEventClass, 0xD);
ASSERT_SIZEOF(TriggerClass, 0x2D);
ASSERT_SIZEOF(TriggerTypeClass, 0x59);
ASSERT_SIZEOF(UnitClass, 0x17A);
ASSERT_SIZEOF(UnitTypeClass, 0x19E);
ASSERT_SIZEOF(VectorClass<int>, 0x10);
ASSERT_SIZEOF(VersionClass, 0x46);
ASSERT_SIZEOF(VesselClass, 0x178);
ASSERT_SIZEOF(VesselTypeClass, 0x19E);
ASSERT_SIZEOF(WarheadTypeClass, 0x22);
ASSERT_SIZEOF(WeaponTypeClass, 0x26);
//
#pragma message("Done Checking Type Sizes!")
#endif
