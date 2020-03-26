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
#include "basec.h"
#include "bfiofile.h"
#include "bigcheck.h"
#include "blitters.h"
#include "building.h"
#include "buildingtype.h"
#include "bullet.h"
#include "bullettype.h"
#include "carryover.h"
#include "cd.h"
#include "cdfile.h"
#include "cell.h"
#include "checkbox.h"
#include "checklist.h"
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
#include "loaddialog.h"
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
#include "overlay.h"
#include "power.h"
#include "radar.h"
#include "rawfile.h"
#include "rgb.h"
#include "rules.h"
#include "scenario.h"
#include "session.h"
#include "shape.h"
#include "shapebtn.h"
#include "sidebar.h"
#include "slider.h"
#include "smudge.h"
#include "sndctrl.h"
#include "super.h"
#include "surfacemonitor.h"
#include "team.h"
#include "teamtype.h"
#include "techno.h"
#include "template.h"
#include "terrain.h"
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

    // controlc.h
    Hook_Function(0x004AC2C0, *ControlClass::Action);
    Hook_Function(0x004AC338, *ControlClass::Draw_Me);

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

    // fading.h
    Hook_Function(0x004FB914, Make_Fading_Table);
    Hook_Function(0x005CC4C0, Build_Fading_Table);
    Hook_Function(0x004FB994, Conquer_Build_Fading_Table);
    Hook_Function(0x004FB7C4, Build_Translucent_Table);
    Hook_Function(0x004FB870, Conquer_Build_Translucent_Table);

    // gadget.h
    Hook_Function(0x004C3E40, *GadgetClass::Remove);
    Hook_Function(0x004C3F38, *GadgetClass::Input);
    Hook_Function(0x004C3F08, *GadgetClass::Draw_All);
    Hook_Function(0x004C3E74, *GadgetClass::Delete_List);
    Hook_Function(0x004C4160, *GadgetClass::Extract_Gadget);
    Hook_Function(0x004A2810, *GadgetClass::Flag_List_To_Redraw);
    Hook_Function(0x004C3E2C, *GadgetClass::Disable);
    Hook_Function(0x004C3E08, *GadgetClass::Enable);
    // Hook_Function(0x004B5950, *GadgetClass::Get_ID);
    Hook_Function(0x004C4198, *GadgetClass::Flag_To_Redraw);
    Hook_Function(0x004AC370, *GadgetClass::Peer_To_Peer);
    Hook_Function(0x004C41D4, *GadgetClass::Set_Focus);
    Hook_Function(0x004C4210, *GadgetClass::Clear_Focus);
    Hook_Function(0x004C423C, *GadgetClass::Has_Focus);
    Hook_Function(0x004C4254, *GadgetClass::Is_List_To_Redraw);
    Hook_Function(0x004A2830, *GadgetClass::Is_To_Redraw);
    Hook_Function(0x004C4280, *GadgetClass::Set_Position);
    Hook_Function(0x004C3EE8, *GadgetClass::Draw_Me);
    Hook_Function(0x004C41A4, *GadgetClass::Sticky_Process); // issue
    Hook_Function(0x004C3EC8, *GadgetClass::Action);
    Hook_Function(0x004C3DB4, *GadgetClass::Clicked_On); // issue

    //link.h
    Hook_Function(0x004FBC7C, *LinkClass::Zap);
    Hook_Function(0x004FBCCC, *LinkClass::Head_Of_List);
    Hook_Function(0x004FBCEC, *LinkClass::Tail_Of_List);
    Hook_Function(0x004FBD08, *LinkClass::Add);
    Hook_Function(0x004FBD28, *LinkClass::Add_Head);
    Hook_Function(0x004FBD4C, *LinkClass::Add_Tail);
    Hook_Function(0x004FBD74, *LinkClass::Remove);

    //textbtn.h
    Hook_Function(0x0056BA70, *TextButtonClass::Hook_Ctor1);
    Hook_Function(0x0056BB64, *TextButtonClass::Hook_Ctor2);
    Hook_Function(0x0056BC2C, *TextButtonClass::Draw_Me);
    Hook_Function(0x0056BD74, *TextButtonClass::Draw_Background);
    Hook_Function(0x0056BDE4, *TextButtonClass::Draw_Text);
    Hook_Function(0x0056BE50, *TextButtonClass::Set_Style);

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

    // bigcheck.h
    Hook_Function(0x005AAC70, *BigCheckBoxClass::Draw_Me);
    Hook_Function(0x005AAD4C, *BigCheckBoxClass::Action);

    // checkbox.h
    Hook_Function(0x004A1F00, *CheckBoxClass::Draw_Me);
    Hook_Function(0x004A22E4, *CheckBoxClass::Action);

    // checklist.h
    Hook_Function(0x004A2510, *CheckListClass::Action);
    Hook_Function(0x004A2568, *CheckListClass::Draw_Entry);
    Hook_Function(0x004A24D4, *CheckListClass::Check_Item);

    // gauge.h
    Hook_Function(0x004C4E70, *GaugeClass::Draw_Me);
    Hook_Function(0x004C52D0, *GaugeClass::Action);
    Hook_Function(0x004C4D70, *GaugeClass::Set_Maximum);
    Hook_Function(0x004C4D90, *GaugeClass::Set_Value);
    // Hook_Function(0x004C6FB0, *GaugeClass::Get_Value);
    Hook_Function(0x004C6FC0, *GaugeClass::Use_Thumb);
    Hook_Function(0x004C6FF0, *GaugeClass::Thumb_Pixels);
    Hook_Function(0x004C5440, *GaugeClass::Draw_Thumb);
    Hook_Function(0x004C4DC8, *GaugeClass::Pixel_To_Value);
    Hook_Function(0x004C4E28, *GaugeClass::Value_To_Pixel);

    // shapebtn.h
    Hook_Function(0x0054CD7C, *ShapeButtonClass::Set_Shape);
    Hook_Function(0x0054CDB0, *ShapeButtonClass::Draw_Me);

    // textbtn.h
    Hook_Function(0x0056BC2C, *TextButtonClass::Draw_Me);
    Hook_Function(0x0056BD74, *TextButtonClass::Draw_Background);
    Hook_Function(0x0056BDE4, *TextButtonClass::Draw_Text);

    // toggle.h
    Hook_Function(0x0056C4C8, *ToggleClass::Action);

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

    // slider.h
    Hook_Function(0x004FCD70, *SliderClass::Thumb_Pixels);
    Hook_Function(0x0054F8D0, *SliderClass::Set_Maximum);
    Hook_Function(0x0054F8F4, *SliderClass::Set_Thumb_Size);
    Hook_Function(0x0054F92C, *SliderClass::Set_Value);
    Hook_Function(0x0054F95C, *SliderClass::Recalc_Thumb);
    Hook_Function(0x0054F9E8, *SliderClass::Action);
    Hook_Function(0x0054FA9C, *SliderClass::Bump);
    Hook_Function(0x0054FACC, *SliderClass::Step);
    Hook_Function(0x0054FAF0, *SliderClass::Draw_Thumb);
    Hook_Function(0x0054FB3C, *SliderClass::Draw_Me);
    Hook_Function(0x0054FBCC, *SliderClass::Peer_To_Peer);

    // list.h
    Hook_Function(0x004FCB38, *ListClass::Add);
    Hook_Function(0x004FCBC0, *ListClass::Add_Tail);
    Hook_Function(0x004FCB7C, *ListClass::Add_Head);
    Hook_Function(0x004FCC08, *ListClass::Remove);
    Hook_Function(0x004FCCD8, *ListClass::Flag_To_Redraw);
    Hook_Function(0x004FC710, *ListClass::Peer_To_Peer);
    Hook_Function(0x004FC1B0, *ListClass::Set_Position);
    Hook_Function(0x004FC558, *ListClass::Draw_Me);
    Hook_Function(0x004FC474, *ListClass::Action);
    // Hook_Function(0x004FC2C0, static_cast<int(*)(const char *)>(*ListClass::Add_Item));//char
    // Hook_Function(0x004FC320, static_cast<int(*)(int)>(*ListClass::Add_Item));//int
    Hook_Function(0x004FC7C8, *ListClass::Add_Scroll_Bar);
    Hook_Function(0x004FC640, *ListClass::Bump);
    // Hook_Function(0x004A2840, *ListClass::Count);
    // Hook_Function(0x004FC6D8, *ListClass::Current_Index);
    // Hook_Function(0x004FC700, *ListClass::Current_Item);
    // Hook_Function(0x004FC6B0, *ListClass::Get_Item);
    Hook_Function(0x004FCCB8, *ListClass::Step_Selected_Index);
    // Hook_Function(0x004FC394, *ListClass::Remove_Item);//int
    // Hook_Function(0x004FC444, *ListClass::Remove_Item);//char*
    Hook_Function(0x004FC890, *ListClass::Remove_Scroll_Bar);
    // Hook_Function(0x004FCC40, *ListClass::Set_Selected_Index);//int
    // Hook_Function(0x004FCD10, static_cast<void(*)(int)>(&ListClass::Set_Selected_Index));//char*
    Hook_Function(0x004FC8E8, *ListClass::Set_Tabs);
    Hook_Function(0x004FC768, *ListClass::Set_View_Index);
    Hook_Function(0x004FC678, *ListClass::Step);
    Hook_Function(0x004FC8F4, *ListClass::Draw_Entry);

    // heap.h
    Hook_Function(0x004CD60C, *FixedHeapClass::Allocate);
    Hook_Function(0x004CD744, *FixedHeapClass::Clear);
    Hook_Function(0x004CD6A0, *FixedHeapClass::Free);
    Hook_Function(0x004CD78C, *FixedHeapClass::Free_All);
    Hook_Function(0x004CD594, *FixedHeapClass::Set_Heap);
    // Hook_Function(0x004CD718, *FixedHeapClass::ID);
    Hook_Function(0x004CD818, *FixedIHeapClass::Allocate);
    Hook_Function(0x004CD7C4, *FixedIHeapClass::Clear);
    Hook_Function(0x004CD850, *FixedIHeapClass::Free);
    Hook_Function(0x004CD7A8, *FixedIHeapClass::Free_All);
    Hook_Function(0x004CD7E4, *FixedIHeapClass::Set_Heap);

    // lzopipe.h
    Hook_Function(0x005D5508, *LZOPipe::Put);
    Hook_Function(0x005D57AC, *LZOPipe::Flush);

    // lzostraw.h
    Hook_Function(0x005D5BF4, *LZOStraw::Get);

    // ini.h
    Hook_Function(0x004F3B34, *INISection::Find_Section);
    Hook_Function(0x004F2F08, static_cast<int (*)(FileClass &)>(&INIClass::Save));
    Hook_Function(0x004F2F44, static_cast<int (*)(Pipe &)>(&INIClass::Save));
    Hook_Function(0x004F28C4, static_cast<int (*)(FileClass &)>(&INIClass::Load));
    Hook_Function(0x004F2900, static_cast<int (*)(Straw &)>(&INIClass::Load));
    Hook_Function(0x004F2670, *INIClass::Clear);
    Hook_Function(0x004F3150, *INIClass::Find_Section);
    Hook_Function(0x004F31AC, *INIClass::Section_Count);
    Hook_Function(0x004F31D4, *INIClass::Find_Entry);
    Hook_Function(0x004F31BC, *INIClass::Entry_Count);
    Hook_Function(0x004F31EC, *INIClass::Get_Entry);
    Hook_Function(0x004F3248, *INIClass::Put_UUBlock);
    Hook_Function(0x004F3338, *INIClass::Get_UUBlock);
    Hook_Function(0x004F343C, *INIClass::Put_TextBlock);
    Hook_Function(0x004F3528, *INIClass::Get_TextBlock);
    Hook_Function(0x004F35E8, *INIClass::Put_Int);
    Hook_Function(0x004F3660, *INIClass::Get_Int);
    Hook_Function(0x004F3AB0, *INIClass::Put_Bool);
    Hook_Function(0x004F3ACC, *INIClass::Get_Bool);
    Hook_Function(0x004F36F0, *INIClass::Put_Hex);
    Hook_Function(0x004F3724, *INIClass::Get_Hex);
    Hook_Function(0x004F3760, *INIClass::Put_String);
    Hook_Function(0x004F3A34, *INIClass::Get_String);
    Hook_Function(0x004F3D80, *INIClass::Put_Fixed);
    Hook_Function(0x004F3D08, *INIClass::Get_Fixed);

    // gameini.h
    Hook_Function(0x0046300C, static_cast<int (*)(FileClass &, BOOL)>(&GameINIClass::Save));
    Hook_Function(0x00463048, static_cast<int (*)(Pipe &, BOOL)>(&GameINIClass::Save));
    Hook_Function(0x00462F50, static_cast<int (*)(FileClass &, BOOL)>(&GameINIClass::Load));
    Hook_Function(0x00462F8C, static_cast<int (*)(Straw &, BOOL)>(&GameINIClass::Load));
    Hook_Function(0x004630A8, *GameINIClass::Get_Lepton);
    Hook_Function(0x004630FC, *GameINIClass::Put_Lepton);
    Hook_Function(0x00463134, *GameINIClass::Get_MPHType);
    Hook_Function(0x004631A4, *GameINIClass::Put_MPHType);
    Hook_Function(0x004631E4, *GameINIClass::Get_Owners);
    Hook_Function(0x00463244, *GameINIClass::Put_Owners);
    Hook_Function(0x004633D4, *GameINIClass::Get_ArmorType);
    Hook_Function(0x00463410, *GameINIClass::Put_ArmorType);
    Hook_Function(0x00463428, *GameINIClass::Get_VocType);
    Hook_Function(0x00463464, *GameINIClass::Put_VocType);
    Hook_Function(0x00463490, *GameINIClass::Get_AnimType);
    Hook_Function(0x004634D0, *GameINIClass::Put_AnimType);
    Hook_Function(0x004634FC, *GameINIClass::Get_UnitType);
    Hook_Function(0x00463548, *GameINIClass::Put_UnitType);
    Hook_Function(0x00463574, *GameINIClass::Get_WeaponType);
    Hook_Function(0x004635B4, *GameINIClass::Put_WeaponType);
    Hook_Function(0x004635E0, *GameINIClass::Get_WarheadType);
    Hook_Function(0x00463648, *GameINIClass::Put_WarheadType);
    Hook_Function(0x00463674, *GameINIClass::Get_OverlayType);
    Hook_Function(0x004636B4, *GameINIClass::Put_OverlayType);
    Hook_Function(0x004636D4, *GameINIClass::Get_BulletType);
    Hook_Function(0x0046373C, *GameINIClass::Put_BulletType);
    Hook_Function(0x00463768, *GameINIClass::Get_HousesType);
    Hook_Function(0x004637A8, *GameINIClass::Put_HousesType);
    Hook_Function(0x004637C8, *GameINIClass::Get_MovieType);
    Hook_Function(0x00463830, *GameINIClass::Put_MovieType);
    Hook_Function(0x00463854, *GameINIClass::Get_TheaterType);
    Hook_Function(0x00463894, *GameINIClass::Put_TheaterType);
    Hook_Function(0x004638BC, *GameINIClass::Get_TriggerType);
    Hook_Function(0x004638F4, *GameINIClass::Put_TriggerType);
    Hook_Function(0x00463904, *GameINIClass::Get_ThemeType);
    Hook_Function(0x00463948, *GameINIClass::Put_ThemeType);
    Hook_Function(0x00463970, *GameINIClass::Get_SourceType);
    Hook_Function(0x004639B0, *GameINIClass::Put_SourceType);
    Hook_Function(0x004639C8, *GameINIClass::Get_CrateType);
    Hook_Function(0x00463A08, *GameINIClass::Put_CrateType);
    Hook_Function(0x00463A20, *GameINIClass::Get_TerrainType);
    Hook_Function(0x00463A68, *GameINIClass::Put_TerrainType);
    Hook_Function(0x00463A88, *GameINIClass::Get_Buildings);
    Hook_Function(0x00463AFC, *GameINIClass::Put_Buildings);
    Hook_Function(0x00463BD4, *GameINIClass::Get_Unique_ID);
    Hook_Function(0x00463C10, *GameINIClass::Calculate_Message_Digest);
    Hook_Function(0x00463CB4, *GameINIClass::Invalidate_Message_Digest);
    Hook_Function(0x004F3B90, *GameINIClass::Put_PKey);
    Hook_Function(0x004F3BF4, *GameINIClass::Get_PKey);

    // version.h
    Hook_Function(0x00589960, ::Version_Name);

    // facing.h
    Hook_Function(0x004BEB44, *FacingClass::Rotation_Adjust);
    Hook_Function(0x004BEB2C, *FacingClass::Set_Current);
    Hook_Function(0x004BEB10, *FacingClass::Set_Desired);
    Hook_Function(0x004BEA7C, Desired_Facing256);
    Hook_Function(0x004BEA20, Desired_Facing8);

    // flasher.h
    Hook_Function(0x004C0690, *FlasherClass::Process);

    // options.h
    // Hook_Function(0x00525884, *OptionsClass::Adjust_Palette);

    // !!! Required to be hooked because main now loads 'chronoshift.ini' !!!
    Hook_Function(0x00525A24, *OptionsClass::Load_Settings);
    Hook_Function(0x005263A8, *OptionsClass::Save_Settings);
    Hook_Function(0x0054AB08, *SessionClass::Read_MultiPlayer_Settings);
    Hook_Function(0x0054B510, *SessionClass::Write_MultiPlayer_Settings);

    // display.h
    Hook_Function(0x004AEF7C, *DisplayClass::Init_Clear);
    Hook_Function(0x004AEFF4, *DisplayClass::Init_IO);
    Hook_Function(0x004AF02C, *DisplayClass::Init_Theater);
    Hook_Function(0x004B0140, *DisplayClass::AI);
    Hook_Function(0x004AEEF4, *DisplayClass::One_Time);
    Hook_Function(0x004AF700, *DisplayClass::Set_Cursor_Shape);
    Hook_Function(0x004B0278, *DisplayClass::Hook_Click_Cell_Calc);
    Hook_Function(0x004B03B4, *DisplayClass::Scroll_Map);
    Hook_Function(0x004B0628, *DisplayClass::Refresh_Cells);
    Hook_Function(0x004AF4E0, *DisplayClass::Set_View_Dimensions);
    Hook_Function(0x004B4860, *DisplayClass::Set_Tactical_Position);
    Hook_Function(0x004B5908, *DisplayClass::Flag_Cell);
    Hook_Function(0x004B35C4, *DisplayClass::Mouse_Right_Press);
    Hook_Function(0x004B4608, *DisplayClass::Mouse_Left_Press);
    Hook_Function(0x004B465C, *DisplayClass::Mouse_Left_Held);
    Hook_Function(0x004B3780, *DisplayClass::Mouse_Left_Up);
    Hook_Function(0x004B2E84, *DisplayClass::Refresh_Band);
    Hook_Function(0x004B006C, *DisplayClass::Cursor_Mark);
    Hook_Function(0x004AFFA8, *DisplayClass::Hook_Get_Occupy_Dimensions);
    Hook_Function(0x004AF2D8, *DisplayClass::Hook_Text_Overlap_List);
    Hook_Function(0x004B25A4, *DisplayClass::Hook_Next_Object);
    Hook_Function(0x004B0214, *DisplayClass::Submit);
    Hook_Function(0x004B0248, *DisplayClass::Remove);
    Hook_Function(0x004B0C78, *DisplayClass::Hook_Cell_Object);
    Hook_Function(0x004B4E20, *DisplayClass::Center_Map);
    Hook_Function(0x004AFD40, *DisplayClass::Set_Cursor_Pos);
    Hook_Function(0x004B1FD0, *DisplayClass::Redraw_Icons);
    Hook_Function(0x004B2178, *DisplayClass::Redraw_Shadow);
    Hook_Function(0x004B4CB8, *DisplayClass::Hook_In_View);
    Hook_Function(0x004B0968, *DisplayClass::Hook_Coord_To_Pixel);
    Hook_Function(0x004B0698, *DisplayClass::Hook_Cell_Shadow);
    Hook_Function(0x004B0B10, *DisplayClass::Push_Onto_TacMap);
    Hook_Function(0x004B274C, *DisplayClass::Hook_Calculated_Cell);
    Hook_Function(0x004B2B90, *DisplayClass::Hook_Good_Reinforcement_Cell);
    Hook_Function(0x004B4D80, *DisplayClass::Hook_Closest_Free_Spot);
    Hook_Function(0x004B4F44, *DisplayClass::Encroach_Shadow);
    Hook_Function(0x004B4B68, *DisplayClass::Sell_Mode_Control);
    Hook_Function(0x004B4C10, *DisplayClass::Repair_Mode_Control);
    Hook_Function(0x004B5680, *DisplayClass::All_To_Look);
    Hook_Function(0x004B5788, *DisplayClass::Constrained_Look);
    Hook_Function(0x004B4FF4, *DisplayClass::Shroud_Cell);
    Hook_Function(0x004B0CA8, *DisplayClass::Draw_It);
    Hook_Function(0x004F91DC, *DisplayClass::Code_Pointers);
    Hook_Function(0x004F9220, *DisplayClass::Decode_Pointers);
    Hook_Function(0x004B0788, *DisplayClass::Map_Cell);
    Hook_Function(0x004B3108, *DisplayClass::TacticalClass::Action);

    // radar.h
    Hook_Function(0x0052D668, *RadarClass::One_Time); // seems to work
    Hook_Function(0x0052D718, *RadarClass::Init_Clear); // seems to work
    Hook_Function(0x00532010, *RadarClass::Set_Map_Dimensions); // seems to work
    Hook_Function(0x0052FEAC, *RadarClass::Map_Cell); // seems to work
    Hook_Function(0x0052FE28, *RadarClass::Hook_Click_Cell_Calc); // seems to work
    Hook_Function(0x00531474, *RadarClass::Refresh_Cells); // seems to work
    Hook_Function(0x00532070, *RadarClass::Set_Tactical_Position); // seems to work
    Hook_Function(0x005329EC, *RadarClass::Flag_Cell); // seems to work
    Hook_Function(0x00532938, *RadarClass::Activate_Pulse); // seems to work
    Hook_Function(0x0052F460, *RadarClass::Hook_Is_Zoomable); // seems to work
    Hook_Function(0x00532098, *RadarClass::Hook_Cell_On_Radar); // seems to work
    Hook_Function(0x00532000, *RadarClass::Radar_Position); // seems to work
    Hook_Function(0x005301B4, *RadarClass::Cell_XY_To_Radar_Pixel); // seems to work
    Hook_Function(0x0053210C, *RadarClass::Player_Names); // seems to work
    Hook_Function(0x0052FF40, *RadarClass::Mark_Radar); // seems to work
    Hook_Function(0x0052FED8, *RadarClass::Cursor_Cell); // seems to work
    Hook_Function(0x0052EF8C, *RadarClass::Render_Overlay); // works
    Hook_Function(0x00530300, *RadarClass::Radar_Cursor); // works
    Hook_Function(0x0052F294, *RadarClass::Zoom_Mode); //
    Hook_Function(0x0052FCC0, *RadarClass::Radar_Pixel); //
    Hook_Function(0x0052FD3C, *RadarClass::Hook_Click_In_Radar); //
    Hook_Function(0x0052D790, *RadarClass::Radar_Activate); //

    // power.h
    Hook_Function(0x005275CC, *PowerClass::One_Time); // seems to work
    Hook_Function(0x00527534, *PowerClass::Init_Clear); // seems to work
    Hook_Function(0x00527EA4, *PowerClass::Refresh_Cells); // seems to work
    Hook_Function(0x00527EE4, *PowerClass::Power_Height); // seems to work
    Hook_Function(0x00528010, *PowerClass::Flash_Power); // seems to work
    Hook_Function(0x00527F54, *PowerClass::PowerButtonClass::Action);

    // sidebar.h
    Hook_Function(0x0054ECA8, *SidebarClass::StripClass::SelectClass::Set_Owner); // seems to work
    Hook_Function(0x0054DDE0, *SidebarClass::StripClass::Get_Special_Cameo); // seems to work
    Hook_Function(0x0054DE00, *SidebarClass::StripClass::Init_Clear); // seems to work
    Hook_Function(0x0054DE54, *SidebarClass::StripClass::Init_IO); // works
    Hook_Function(0x0054E008, *SidebarClass::StripClass::Init_Theater); // works
    Hook_Function(0x0054E0C4, *SidebarClass::StripClass::Activate); // seems to work
    Hook_Function(0x0054E184, *SidebarClass::StripClass::Deactivate); // seems to work
    Hook_Function(0x0054E1F8, *SidebarClass::StripClass::Add); // seems to work
    Hook_Function(0x0054E290, *SidebarClass::StripClass::Scroll); // seems to work
    Hook_Function(0x0054E2CC, *SidebarClass::StripClass::Flag_To_Redraw); // seems to work
    Hook_Function(0x0054F3B4, *SidebarClass::StripClass::Factory_Link); // works
    Hook_Function(0x0054D07C, *SidebarClass::One_Time); // seems to work
    Hook_Function(0x0054D0F8, *SidebarClass::Init_Clear); // works
    Hook_Function(0x0054D144, *SidebarClass::Init_IO); // works
    Hook_Function(0x0054D304, *SidebarClass::Init_Theater); // works
    Hook_Function(0x0054D724, *SidebarClass::Draw_It); // Powerbar needed shows even when power structure not in play.
    Hook_Function(0x0054D404, *SidebarClass::Refresh_Cells); // works
    Hook_Function(0x0054D3B0, *SidebarClass::Which_Column); // works
    Hook_Function(0x0054D61C, *SidebarClass::Add); // seems to work
    Hook_Function(0x0054D684, *SidebarClass::Scroll); // breaks scrolling both strips at same time
    Hook_Function(0x0054DA2C, *SidebarClass::Recalc); // seems to work
    Hook_Function(0x0054DA70, *SidebarClass::Activate); // works
    Hook_Function(0x0054F408, *SidebarClass::Abandon_Production); // seems to work
    Hook_Function(0x0054F4B8, *SidebarClass::Zoom_Mode_Control); // works

    // tab.h
    Hook_Function(0x005533A0, *TabClass::Draw_It);
    Hook_Function(0x005539D8, *TabClass::One_Time);
    Hook_Function(0x005538D0, *TabClass::AI);
    Hook_Function(0x00553744, *TabClass::Draw_Credits_Tab);

    // help.h
    Hook_Function(0x004D2338, *HelpClass::Init_Clear);
    Hook_Function(0x004D23C8, *HelpClass::AI);
    Hook_Function(0x004D26B0, *HelpClass::Draw_It);
    Hook_Function(0x004D2574, *HelpClass::Help_Text);
    Hook_Function(0x004D2B6C, *HelpClass::Scroll_Map);
    Hook_Function(0x004D2BC0, *HelpClass::Set_Tactical_Position);
    Hook_Function(0x004D2354, *HelpClass::Hook_Overlap_List);
    Hook_Function(0x004D293C, *HelpClass::Set_Text);

    // scroll.h
    Hook_Function(0x004F92E0, *ScrollClass::Init_IO);
    Hook_Function(0x00547088, *ScrollClass::AI);
    Hook_Function(0x00547464, *ScrollClass::Set_Autoscroll);

    // gmouse.h
    Hook_Function(0x005033E0, *GameMouseClass::One_Time);
    Hook_Function(0x005033FC, *GameMouseClass::Init_Clear);
    Hook_Function(0x0050329C, *GameMouseClass::AI);
    Hook_Function(0x0050305C, *GameMouseClass::Set_Default_Mouse);
    Hook_Function(0x0050316C, *GameMouseClass::Override_Mouse_Shape);
    Hook_Function(0x004F90F8, *GameMouseClass::Hook_Save);
    Hook_Function(0x00503078, *GameMouseClass::Revert_Mouse_Shape);
    Hook_Function(0x00503098, *GameMouseClass::Mouse_Small);
    Hook_Function(0x004F8F70, *GameMouseClass::Load);

    // gameevent.h
    Hook_Call(0x0052BC5E, *GameEventClass::Execute);

    // cell.h
    Hook_Function(0x0049EE70, *CellClass::Hook_Ctor);
    Hook_Function(0x0049FF98, *CellClass::Spot_Index);
    Hook_Function(0x004F8E64, *CellClass::Load);
    Hook_Function(0x0049F5F8, *CellClass::Hook_Draw_It);
    Hook_Function(0x0049EFBC, *CellClass::Hook_Cell_Techno);
    Hook_Function(0x0049F084, *CellClass::Hook_Cell_Find_Object);
    Hook_Function(0x0049F0D8, *CellClass::Hook_Cell_Object);
    Hook_Function(0x0049F314, *CellClass::Recalc_Attributes);
    Hook_Function(0x004A1B0C, *CellClass::Hook_Is_Clear_To_Move);
    // Hook_Function(0x004A1E40, *CellClass::Can_Ore_Germinate);
    Hook_Function(0x0049F10C, *CellClass::Redraw_Objects);
    Hook_Function(0x0049F590, *CellClass::Overlap_Up);
    // Hook_Function(0x0049FC58, *CellClass::Wall_Update);//crashes
    Hook_Function(0x004A0230, *CellClass::Adjust_Threat);
    Hook_Function(0x004A1AE4, *CellClass::Shimmer);
    Hook_Function(0x004A013C, *CellClass::Incoming);
    Hook_Function(0x004A1A5C, *CellClass::Flag_Place);
    Hook_Function(0x004A1AB4, *CellClass::Flag_Remove);

    // door.h
    Hook_Function(0x004B5C40, *DoorClass::AI);
    Hook_Function(0x004B5D08, *DoorClass::Open_Door);
    Hook_Function(0x004B5D60, *DoorClass::Close_Door);
    Hook_Function(0x004B5DC8, *DoorClass::Hook_Door_Stage);

    // layer.h
    Hook_Function(0x004B59B0, *LayerClass::Init);
    Hook_Function(0x004FBA80, *LayerClass::Submit);
    Hook_Function(0x004FBAA4, *LayerClass::Sort);
    Hook_Function(0x004FBB20, *LayerClass::Sorted_Add);
    Hook_Function(0x004F9580, *LayerClass::Load);
    Hook_Function(0x004F9600, *LayerClass::Hook_Save);

    // map.h
    Hook_Function(0x004FE310, *MapClass::One_Time);
    Hook_Function(0x004FE344, *MapClass::Init_Clear);
    Hook_Function(0x004FE3B0, *MapClass::Alloc_Cells);
    Hook_Function(0x004FE3DC, *MapClass::Free_Cells);
    Hook_Function(0x004FE3EC, *MapClass::Init_Cells);
    Hook_Function(0x004FEE94, *MapClass::Logic_AI);
    Hook_Function(0x004FE420, *MapClass::Set_Map_Dimensions);
    Hook_Function(0x004FF218, *MapClass::Place_Random_Crate);
    Hook_Function(0x004FF2A0, *MapClass::Remove_Crate);
    Hook_Function(0x005008A4, *MapClass::Hook_Pick_Random_Location);
    Hook_Function(0x004FE8AC, *MapClass::Hook_In_Radar);
    Hook_Function(0x004FE918, *MapClass::Place_Down);
    Hook_Function(0x004FEA28, *MapClass::Pick_Up);
    Hook_Function(0x004FEB38, *MapClass::Overlap_Down);
    Hook_Function(0x004FEBD8, *MapClass::Overlap_Up);
    Hook_Function(0x004FEC78, *MapClass::Overpass);
    Hook_Function(0x004FF168, *MapClass::Cell_Region);
    Hook_Function(0x004FF690, *MapClass::Zone_Reset);
    Hook_Function(0x004FF7D8, *MapClass::Zone_Span);
    Hook_Function(0x004FFAC4, *MapClass::Hook_Nearby_Location);
    Hook_Function(0x0050083C, *MapClass::Hook_Intact_Bridge_Count);
    Hook_Function(0x004FED90, *MapClass::Read_Binary);
    Hook_Function(0x004FF1BC, *MapClass::Cell_Threat);
    Hook_Function(0x004FFEAC, *MapClass::Hook_Base_Region);
    Hook_Function(0x00500908, *MapClass::Shroud_The_Map);
    Hook_Function(0x004FF554, *MapClass::Hook_Close_Object);
    Hook_Function(0x004FE7C8, *MapClass::UnJam_From);
    Hook_Function(0x004FE68C, *MapClass::Jam_From);
    Hook_Function(0x004FE588, *MapClass::Shroud_From);
    Hook_Function(0x004FE438, *MapClass::Sight_From);

    // scenario.h
    Hook_Function(0x0053CFB0, *ScenarioClass::Hook_Set_Scenario_Name1);
    Hook_Function(0x0053CD80, *ScenarioClass::Hook_Set_Scenario_Name2);
    Hook_Function(0x00539BF8, *ScenarioClass::Do_BW_Fade);
    Hook_Function(0x00539C40, *ScenarioClass::Do_Fade_AI);

    // mission.h
    Hook_Function(0x00502C70, *MissionClass::Assign_Mission);
    Hook_Function(0x00502C2C, *MissionClass::Commence);
    Hook_Function(0x00502798, *MissionClass::Set_Mission);
    Hook_Function(0x00502CFC, *MissionClass::Override_Mission);
    Hook_Function(0x00502D20, *MissionClass::Restore_Mission);
    Hook_Function(0x00502824, *MissionClass::AI);
    Hook_Function(0x00502D54, *MissionClass::Is_Recruitable_Mission);

    // msgbox.h
    Hook_Function(0x005043D0, *MessageBoxClass::Hook_Process);

    // msglist.h
    Hook_Function(0x00505244, *MessageListClass::Init);
    Hook_Function(0x005053AC, *MessageListClass::Reset);
    Hook_Function(0x0050542C, *MessageListClass::Add_Message);
    Hook_Function(0x005057B0, *MessageListClass::Get_Message);
    Hook_Function(0x005057DC, *MessageListClass::Get_Label);
    Hook_Function(0x00505804, *MessageListClass::Concat_Message);
    Hook_Function(0x00505AE4, *MessageListClass::Set_Edit_Focus);
    Hook_Function(0x00505B00, *MessageListClass::Has_Edit_Focus);
    Hook_Function(0x00505B20, *MessageListClass::Add_Edit);
    Hook_Function(0x00505C9C, *MessageListClass::Remove_Edit);
    Hook_Function(0x00505CDC, *MessageListClass::Get_Edit_Buf);
    Hook_Function(0x00505CF4, *MessageListClass::Set_Edit_Color);
    Hook_Function(0x00505D18, *MessageListClass::Manage);
    Hook_Function(0x00505DEC, *MessageListClass::Input);
    Hook_Function(0x005060E4, *MessageListClass::Draw);
    Hook_Function(0x005061DC, *MessageListClass::Num_Messages);
    Hook_Function(0x00506214, *MessageListClass::Set_Width);
    Hook_Function(0x00506248, *MessageListClass::Trim_Message);
    Hook_Function(0x00506340, *MessageListClass::Compute_Y);

    // radio.h
    Hook_Function(0x00532BA8, *RadioClass::Limbo);

    // techno.h
    Hook_Function(0x0056706C, *TechnoClass::Hook_Techno_Draw_Object);
    Hook_Function(0x005618E4, *TechnoClass::Hook_Draw_It);
    Hook_Function(0x00566154, *TechnoClass::Hook_Can_Repair);
    Hook_Function(0x005660C0, *TechnoClass::Hook_Can_Player_Fire);
    Hook_Function(0x00561444, *TechnoClass::Hidden);
    Hook_Function(0x0056950C, *TechnoClass::Look);
    Hook_Function(0x00561480, *TechnoClass::Mark);
    Hook_Function(0x005643B4, *TechnoClass::Clicked_As_Target);
    Hook_Function(0x00564C34, *TechnoClass::Select);
    Hook_Function(0x005661A0, *TechnoClass::Hook_Weapon_Range);
    Hook_Function(0x00561870, *TechnoClass::Per_Cell_Process);
    Hook_Function(0x005661E8, *TechnoClass::Override_Mission);
    Hook_Function(0x0056621C, *TechnoClass::Restore_Mission);
    Hook_Function(0x00560C9C, *TechnoClass::Hook_How_Many_Survivors);
    Hook_Function(0x00569220, *TechnoClass::Hook_Find_Exit_Cell);
    Hook_Function(0x00566134, *TechnoClass::Hook_Is_Weapon_Equipped);
    Hook_Function(0x00564F3C, *TechnoClass::Stun);
    Hook_Function(0x0056624C, *TechnoClass::Renovate);
    Hook_Function(0x0056733C, *TechnoClass::Hook_Remap_Table);
    Hook_Function(0x005686C0, *TechnoClass::Hook_Is_Ready_To_Random_Animate);
    Hook_Function(0x00566EFC, *TechnoClass::Hook_Visual_Character);
    Hook_Function(0x005673AC, *TechnoClass::Detach);
    Hook_Function(0x005609B0, *TechnoClass::Hook_Is_Players_Army);
    Hook_Function(0x00562878, *TechnoClass::Hook_In_Range_Coord);
    Hook_Function(0x0056756C, *TechnoClass::Hook_Value);
    Hook_Function(0x00569234, *TechnoClass::Hook_Refund_Amount);
    Hook_Function(0x00567648, *TechnoClass::Hook_Threat_Range);
    Hook_Function(0x00566290, *TechnoClass::Captured);
    Hook_Function(0x0056364C, *TechnoClass::Hook_Evaluate_Just_Cell);

    // house.h
    Hook_Function(0x004D5EF4, *HouseClass::Silo_Redraw_Check);
    Hook_Function(0x004D2D48, *HouseClass::Assign_Handicap);
    Hook_Function(0x004D6060, *HouseClass::Make_Ally);
    Hook_Function(0x004D6370, *HouseClass::Make_Enemy);
    Hook_Function(0x004D8B40, *HouseClass::Flag_To_Die);
    Hook_Function(0x004D8BB8, *HouseClass::Flag_To_Win);
    Hook_Function(0x004D8C30, *HouseClass::Flag_To_Lose);
    Hook_Function(0x004DCB14, *HouseClass::Tracking_Remove);
    Hook_Function(0x004DCC30, *HouseClass::Tracking_Add);
    Hook_Function(0x004DCEB4, *HouseClass::Factory_Counter);
    Hook_Function(0x004DCF6C, *HouseClass::Active_Add);
    Hook_Function(0x004DCF1C, *HouseClass::Active_Remove);
    Hook_Function(0x004DE640, *HouseClass::Computer_Paranoid);
    Hook_Function(0x004D4014, *HouseClass::Can_Build);
    Hook_Function(0x004D2D28, *HouseClass::One_Time);
    Hook_Function(0x004D8CA8, *HouseClass::Init_Data);
    Hook_Function(0x004D5D9C, *HouseClass::Harvested);
    Hook_Function(0x004D5E80, *HouseClass::Adjust_Capacity);
    Hook_Function_Const(0x004D8CC4, &HouseClass::Power_Fraction);
    Hook_Function(0x004D658C, *HouseClass::Adjust_Threat);
    Hook_Function(0x004DDBD0, *HouseClass::Hook_Fetch_Factory);
    Hook_Function(0x004DDC58, *HouseClass::Set_Factory);
    Hook_Function(0x004D40F4, *HouseClass::Init);
    Hook_Function(0x004D8D1C, *HouseClass::Sell_Wall);
    Hook_Function(0x004D8F34, *HouseClass::Hook_Find_Building);
    Hook_Function(0x004D9020, *HouseClass::Hook_Find_Build_Location);
    Hook_Function(0x004DE81C, *HouseClass::Hook_Find_Cell_In_Zone);
    Hook_Function(0x004DE9C0, *HouseClass::Hook_Random_Cell_In_Zone);
    Hook_Function(0x004DD07C, *HouseClass::Hook_Which_Zone_Obj);
    Hook_Function(0x004DD0A4, *HouseClass::Hook_Which_Zone_Cell);
    Hook_Function(0x004DCFBC, *HouseClass::Hook_Which_Zone_Coord);
    Hook_Function(0x004DDCD0, *HouseClass::Factory_Count);
    Hook_Function(0x004DD920, *HouseClass::Hook_Zone_Cell);
    Hook_Function(0x004DD9FC, *HouseClass::Hook_Where_To_Go);
    Hook_Function(0x004DDA80, *HouseClass::Hook_Find_Juicy_Target);
    Hook_Function(0x004DE094, *HouseClass::Hook_Is_No_Yak_Mig);
    Hook_Function(0x004DE154, *HouseClass::Hook_Is_Hack_Prevented);
    Hook_Function(0x004DDCFC, *HouseClass::Read_INI);
    Hook_Function(0x004D92B0, *HouseClass::Recalc_Center);
    Hook_Function(0x004DA184, *HouseClass::AI_Lower_Power);
    Hook_Function(0x004DA148, *HouseClass::AI_Fire_Sale);
    Hook_Function(0x004D9D54, *HouseClass::Check_Raise_Power);
    Hook_Function(0x004DE178, *HouseClass::Fire_Sale);
    Hook_Function(0x004D7DC4, *HouseClass::Detach);
    //Hook_Function_Const(0x004D7E80, &HouseClass::Suggest_New_Object);//can't hook cause of defarg
    Hook_Function(0x004D6600, *HouseClass::Begin_Production);
    Hook_Function(0x004D66D0, *HouseClass::Suspend_Production);
    Hook_Function(0x004D671C, *HouseClass::Abandon_Production);
    Hook_Function(0x004DC93C, *HouseClass::Production_Begun);
    Hook_Function(0x004D67C4, *HouseClass::Special_Weapon_AI);
    Hook_Function(0x004D7678, *HouseClass::Place_Object);
    Hook_Function(0x004D7A18, *HouseClass::Manual_Place);

    // foot.h
    Hook_Function(0x004C3328, *FootClass::Hook_Can_Demolish);
    Hook_Function(0x004C154C, *FootClass::Hook_Sort_Y);
    // Hook_Function(0x004C1B6C, *FootClass::Unlimbo); Needs TechnoClass stuff.
    Hook_Function(0x004BF77C, *FootClass::Find_Path);
    Hook_Function(0x004BF49C, *FootClass::Unravel_Loop);
    Hook_Function(0x004BF5E0, *FootClass::Register_Cell);
    Hook_Function(0x004BFDE4, *FootClass::Follow_Edge);
    Hook_Function(0x004C0130, *FootClass::Optimize_Moves);
    // Hook_Function(0x004C037C, *FootClass::Safety_Point); Need to work out test case
    Hook_Function(0x004C0570, *FootClass::Passable_Cell);
    Hook_Function(0x004BF470, *FootClass::Point_Relative_To_Line);
    Hook_Function(0x004C09E4, *FootClass::Basic_Path);
    Hook_Function(0x004C08D0, *FootClass::Mark);
    Hook_Function(0x004C33C4, *FootClass::Sell_Back);
    Hook_Function(0x004F97B8, *FootClass::Mission_Attack);
    Hook_Function(0x004C2B98, *FootClass::Override_Mission);
    Hook_Function(0x004C2BCC, *FootClass::Restore_Mission);
    Hook_Function(0x004C160C, *FootClass::Stun);
    Hook_Function(0x004C2BFC, *FootClass::Receive_Message);
    Hook_Function(0x004C14C8, *FootClass::Start_Driver);

    // abstract.h
    Hook_Function(0x004CD8E0, *AbstractClass::Hook_Ctor_NoInit);
    Hook_Function(0x00423CB0, *AbstractClass::Hook_Dtor);
    Hook_Function(0x004CD8F0, *AbstractClass::Hook_Name);
    Hook_Function(0x00423CE0, *AbstractClass::Hook_Owner);
    Hook_Function(0x004CD900, *AbstractClass::Hook_Center_Coord);
    Hook_Function(0x004CD910, *AbstractClass::Hook_Target_Coord);
    Hook_Function(0x00423CF0, *AbstractClass::Hook_Can_Enter_Cell);
    Hook_Function(0x004CD920, *AbstractClass::AI);
    Hook_Function(0x00405540, *AbstractClass::Hook_Distance_To_Target);

    // abstracttype.h
    Hook_Function(0x004055DC, *AbstractTypeClass::Hook_Ctor);
    Hook_Function(0x004CD990, *AbstractTypeClass::Hook_Ctor_NoInit);
    Hook_Function(0x00405480, *AbstractTypeClass::Hook_Ctor_Copy);
    Hook_Function(0x00405610, *AbstractTypeClass::Hook_Coord_Fixup);
    Hook_Function(0x0040561C, *AbstractTypeClass::Hook_Full_Name);
    Hook_Function(0x0040566C, *AbstractTypeClass::Hook_Get_Ownable);

    // aircrafttype.h
    Hook_Function(0x00401210, *AircraftTypeClass::Hook_Ctor);
    Hook_Function(0x00404E80, *AircraftTypeClass::Hook_Ctor_Copy);
    Hook_Function(0x00405470, *AircraftTypeClass::Hook_Dtor);
    Hook_Function(0x00404090, *AircraftTypeClass::Hook_Max_Pips);
    Hook_Function(0x004040BC, *AircraftTypeClass::Hook_Dimensions);
    Hook_Function(0x004040B0, *AircraftTypeClass::Hook_Create_And_Place);
    // Hook_Function(0x00404024, *AircraftTypeClass::Hook_Create_One_Of); // TODO: Requires implementation.
    Hook_Function(0x00404078, *AircraftTypeClass::Hook_Occupy_List);
    Hook_Function(0x00404084, *AircraftTypeClass::Hook_Overlap_List);
    Hook_Function(0x004040F0, *AircraftTypeClass::As_Reference);
    Hook_Function(0x00403EF0, *AircraftTypeClass::From_Name);
    Hook_Function(0x00403F40, *AircraftTypeClass::One_Time);
    Hook_Function(0x00401350, *AircraftTypeClass::Init_Heap);

    Hook_Function(0x005B42F4, Buffer_Print);
    Hook_Function(0x005B96F0, &MixFileClass<GameFileClass>::Offset);
    Hook_Function(0x005B9330, &MixFileClass<GameFileClass>::Retrieve);
    Hook_Function(0x004A96E8, CC_Draw_Shape);

    // factory.h
    Hook_Function(0x004BEBE0, *FactoryClass::Hook_Ctor);
    Hook_Function(0x004BEE10, *FactoryClass::Has_Changed);
    Hook_Function(0x004BEF80, *FactoryClass::Suspend);
    Hook_Function(0x004BF330, *FactoryClass::Hooked_Has_Completed);
    Hook_Function(0x004BF380, *FactoryClass::Hooked_Cost_Per_Tick);
    Hook_Function(0x004BF3B4, *FactoryClass::Completed);
    Hook_Function(0x004BF228, *FactoryClass::Abandon);
    //Hook_Function(0x004BEE30, *FactoryClass::Hooked_Set1);
    Hook_Function(0x004BEF04, *FactoryClass::Hooked_Set2);
    Hook_Function(0x004BECE8, *FactoryClass::AI);
    // Hook_Function(0x004F9550, *FactoryClass::Decode_Pointers);
    // Hook_Function(0x004F94E8, *FactoryClass::Code_Pointers);

    // dialog.h
    Hook_Function(0x004AE3FC, &Simple_Text_Print);
    Hook_Function(0x004AE7FC,
        static_cast<void (*)(int, unsigned, unsigned, RemapControlType *, uint8_t, TextPrintType, ...)>(
            &Fancy_Text_Print)); // int
    Hook_Function(0x004AE8C0,
        static_cast<void (*)(const char *, unsigned, unsigned, RemapControlType *, uint8_t, TextPrintType, ...)>(
            &Fancy_Text_Print)); // char
    Hook_Function(0x004AE930, &Conquer_Clip_Text_Print);
    Hook_Function(0x004AEA74,
        static_cast<void (*)(int, unsigned, unsigned, uint8_t, uint8_t, TextPrintType, ...)>(&Plain_Text_Print)); // int
    Hook_Function(0x004AEAF8,
        static_cast<void (*)(const char *, unsigned, unsigned, uint8_t, uint8_t, TextPrintType, ...)>(
            &Plain_Text_Print)); // char
    Hook_Function(0x004AE284, &Format_Window_String);
    Hook_Function(0x00591F3C, &Format_Window_String_New);
    Hook_Function(0x004AEBB4, static_cast<void (*)(int, int, int, int)>(&Draw_Caption)); // int
    Hook_Function(0x004AEC14, static_cast<void (*)(const char *, int, int, int)>(&Draw_Caption)); // char
    Hook_Function(0x004AD670, &Dialog_Box);
    Hook_Function(0x004ADB5C, &Draw_Box);
    Hook_Function(0x004AE350, &Window_Box);

    Hook_Function(0x004AC814, Move_Point);

    // init.h
    Hook_Function(0x004F5EC4, Init_Random);
    Hook_Function(0x004F6240, Init_Color_Remaps);
    Hook_Function(0x004F781C, Init_Expansion_Files);
    Hook_Function(0x004F7948, Init_Fonts);
    Hook_Function(0x004F7DF0, Init_Bootstrap_Mixfiles);
    Hook_Function(0x004F8018, Init_Secondary_Mixfiles);
    Hook_Function(0x004F8664, Init_Keys);
    Hook_Function(0x004F81CC, Bootstrap);
    Hook_Function(0x004F8390, Init_Mouse);
    Hook_Function(0x004F7A08, Init_CDROM_Access);
    Hook_Function(0x005B3CD8, Load_Title_Screen);
    Hook_Function(0x004F3E98, Load_Prolog_Page);
    Hook_Function(0x004F78D8, Init_One_Time_Systems);
    Hook_Function(0x004F850C, Init_Bulk_Data);

    // super.h
    Hook_Function(0x00552FE8, *SuperClass::Suspend);
    Hook_Function(0x00553034, *SuperClass::Enable);
    Hook_Function(0x0055308C, *SuperClass::Remove);
    Hook_Function(0x005530B4, *SuperClass::Recharge);
    Hook_Function(0x00553110, *SuperClass::Discharged);
    Hook_Function(0x0055315C, *SuperClass::AI);
    Hook_Function(0x00553224, *SuperClass::Hook_Anim_Stage);
    Hook_Function(0x005532BC, *SuperClass::Hook_Impatient_Click);
    Hook_Function(0x005532E8, *SuperClass::Forced_Charge);

    // loaddialog.h
    // Hook_Function(0x004FCEB4, *LoadOptionsClass::Process); // Not implemented.
    Hook_Function(0x004FDA04, *LoadOptionsClass::Clear_List);
    Hook_Function(0x004FDA80, *LoadOptionsClass::Fill_List);

    Hook_Function(0x004AAC58, &Force_CD_Available);
    Hook_Function(0x004A7C74, Main_Loop);
    Hook_Function(0x004A56D8, Keyboard_Process);
    Hook_Function(0x005CA150, Wait_Blit);
    Hook_Function(0x005C9D60, Set_Video_Mode);
    Hook_Function(0x005C9E60, Reset_Video_Mode);
    Hook_Function(0x00552368, Init_Video);
    Hook_Function(0x005CA070, Set_Video_Palette);

    // vortex.h
    Hook_Function(0x0058E0F4, *ChronalVortexClass::Appear);
    Hook_Function(0x0058E240, *ChronalVortexClass::Disappear);
    Hook_Function(0x0058E270, *ChronalVortexClass::Hide);
    Hook_Function(0x0058E290, *ChronalVortexClass::Show);
    Hook_Function(0x0058E304, *ChronalVortexClass::Stop);
    Hook_Function(0x0058E3A8, *ChronalVortexClass::AI);
    Hook_Function(0x0058E510, *ChronalVortexClass::Movement);
    Hook_Function(0x0058E780, *ChronalVortexClass::Set_Target);
    Hook_Function(0x0058E324, *ChronalVortexClass::Load);
    Hook_Function(0x0058E364, *ChronalVortexClass::Save);
    Hook_Function(0x0058E800, *ChronalVortexClass::Attack);
    // Hook_Function(0x0058E9E0, *ChronalVortexClass::Zap_Target); // Not implemented.
    Hook_Function(0x0058EDD8, *ChronalVortexClass::Render);
    Hook_Function(0x0058EBC4, *ChronalVortexClass::Coordinate_Remap);
    Hook_Function(0x0058F67C, *ChronalVortexClass::Set_Redraw);
    Hook_Function(0x0058F858, *ChronalVortexClass::Build_Fading_Table);
    Hook_Function(0x0058F724, *ChronalVortexClass::Setup_Remap_Tables);
    Hook_Function(0x0058F960, *ChronalVortexClass::Detach);

    // special.h
    Hook_Function(0x00550DE0, *SpecialClass::Init);

    // template.h
    Hook_Function(0x00550DE0, *TemplateClass::Mark);

    // coord.h
    Hook_Function(0x004AC46C, static_cast<const int16_t *(*)(coord_t, int)>(&Coord_Spillage_List));
    Hook_Function(0x004AC630, static_cast<const int16_t *(*)(coord_t, const TRect<int> &, BOOL)>(&Coord_Spillage_List));

    // terrain.h
    Hook_Function(0x0056AB98, *TerrainClass::Hook_Center_Coord);
    Hook_Function(0x0056AC98, *TerrainClass::Hook_Target_Coord);
    Hook_Function(0x0056A668, *TerrainClass::Hook_Can_Enter_Cell);
    Hook_Function(0x0056A91C, *TerrainClass::AI);
    Hook_Function(0x004CDCA0, *TerrainClass::Hook_Sort_Y);
    Hook_Function(0x0056AB58, *TerrainClass::Limbo);
    Hook_Function(0x0056AAB4, *TerrainClass::Unlimbo);
    Hook_Function(0x0056A5AC, *TerrainClass::Hook_Draw_It);
    Hook_Function(0x0056A510, *TerrainClass::Mark);
    Hook_Function(0x0056A2FC, *TerrainClass::Take_Damage);
    Hook_Function(0x0056A734, *TerrainClass::Catch_Fire);
    Hook_Function(0x0056A854, *TerrainClass::Fire_Out);
    Hook_Function(0x0056AB08, *TerrainClass::Start_To_Crumble);
    Hook_Function(0x0056ABF0, *TerrainClass::Hook_Radar_Icon);

    Hook_Function(0x00550290, *SoundControlsClass::Process);

    Hook_Function_Const(0x004265F8, &BaseClass::Get_Building);
    //Hook_Function(0x0042674C, static_cast<BaseNodeClass *(*)(BuildingClass *)>(*BaseClass::Get_Node));
    //Hook_Function(0x004267C4, static_cast<BaseNodeClass *(*)(cell_t)>(*BaseClass::Get_Node));
    Hook_Function(0x004267F8, *BaseClass::Next_Buildable);
    Hook_Function(0x00426470, *BaseClass::Load);
    Hook_Function(0x00426540, *BaseClass::Save);
    Hook_Function(0x00426858, *BaseClass::Read_INI);
    Hook_Function(0x00426944, *BaseClass::Write_INI);

    Hook_Function(0x004624A0, *CarryoverClass::Hook_Ctor);
    //Hook_Function(0x004625E8, *CarryoverClass::Create);

    //anim.cpp
    Hook_Function(0x004240C0, *AnimClass::Wrap_Center_Coord);
    Hook_Function(0x00425610, *AnimClass::Wrap_In_Which_Layer);
    Hook_Function(0x00425A98, *AnimClass::Detach);
    Hook_Function(0x0042410C, *AnimClass::Render);
    Hook_Function(0x004245E8, *AnimClass::Wrap_Occupy_List);
    Hook_Function(0x0042415C, *AnimClass::Wrap_Draw_It);
    Hook_Function(0x004242D0, *AnimClass::Mark);
    Hook_Function(0x00423F64, *AnimClass::Wrap_Sort_Y);
    Hook_Function(0x004256B8, *AnimClass::Start);
    Hook_Function(0x00425748, *AnimClass::Middle);
    Hook_Function(0x0042554C, *AnimClass::Attach_To);

    // aircraft.cpp
    Hook_Function(0x00421918, *AircraftClass::Hook_Can_Enter_Cell);
    Hook_Function(0x0041E0E8, *AircraftClass::AI);
    Hook_Function(0x0041EA98, *AircraftClass::Hook_Sort_Y);
    //Hook_Function(0x0041D25C, *AircraftClass::Hook_Draw_It);
    Hook_Function(0x00423C38, *AircraftClass::Look);
    Hook_Function(0x00421464, *AircraftClass::Hook_Desired_Load_Dir);
    Hook_Function(0x00421D04, *AircraftClass::Hook_Pip_Count);
    Hook_Function(0x00423154, *AircraftClass::Response_Select);
    Hook_Function(0x004230FC, *AircraftClass::Response_Move);
    Hook_Function(0x004230A4, *AircraftClass::Response_Attack);
    Hook_Function(0x00423B60, *AircraftClass::Assign_Destination);
    //Hook_Function(0x0042263C, *AircraftClass::Set_Speed);
    Hook_Function(0x00423958, *AircraftClass::Movement_AI);
    Hook_Function(0x0042381C, *AircraftClass::Edge_Of_World_AI);
    Hook_Function(0x00420208, *AircraftClass::Hook_Active_Click_With);
    Hook_Function(0x004202A8, *AircraftClass::Hook_Player_Assign_Mission);
    Hook_Function(0x0041E9F4, *AircraftClass::Is_LZ_Clear);
    Hook_Function_Const(0x0042063C, &AircraftClass::Pose_Dir);
    
    // object.cpp
    Hook_Function(0x0051DDE8, *ObjectClass::Limbo);
    Hook_Function(0x0051DF74, *ObjectClass::Detach);
    Hook_Function(0x0051DFDC, *ObjectClass::Detach_All);
    Hook_Function(0x0051E5C0, *ObjectClass::Paradrop);
    Hook_Function(0x0051E690, *ObjectClass::Attach_Trigger);

    // cargo.cpp
    Hook_Function(0x004623D0, *CargoClass::Attach);
    Hook_Function(0x00462448, *CargoClass::Detach_Object);

    // crate.cpp
    Hook_Function(0x004AC8E0, *CrateClass::Remove_It);
    Hook_Function(0x004AC91C, *CrateClass::Create_It);
    Hook_Function(0x004AC9B4, *CrateClass::Put_Crate);
    Hook_Function(0x004ACB1C, *CrateClass::Get_Crate);

    // building.cpp
    Hook_Function(0x0045DE68, *BuildingClass::Hook_Death_Announcement);
    Hook_Function(0x0045E3E4, *BuildingClass::Detach);
    Hook_Function(0x0046072C, *BuildingClass::Hook_Value);
    Hook_Function(0x004606D0, *BuildingClass::Hook_Get_Image_Data);
    Hook_Function(0x00458760, *BuildingClass::Hook_Active_Click_With_Obj);
    Hook_Function(0x00458850, *BuildingClass::Hook_Active_Click_With_Cell);
    Hook_Function(0x0045523C, *BuildingClass::Hook_Draw_It);
    Hook_Function(0x0045E320, *BuildingClass::Power_Output);
    Hook_Function(0x00459880, *BuildingClass::Limbo);
    Hook_Function(0x0045A6D8, *BuildingClass::Hook_What_Action_Cell);
    Hook_Function(0x0045B79C, *BuildingClass::Hook_Sort_Y);
    Hook_Function(0x0045A138, *BuildingClass::Repair);
    Hook_Function(0x0045A288, *BuildingClass::Sell_Back);

    // unit.cpp
    Hook_Function(0x00581730, *UnitClass::Limbo);
    Hook_Function(0x0057B40C, *UnitClass::Hook_Active_Click_With_Obj);
    Hook_Function(0x0057B4B0, *UnitClass::Hook_Active_Click_With_Cell);
    Hook_Function(0x0057FD4C, *UnitClass::Mission_Move);
    Hook_Function(0x00580290, *UnitClass::Hook_Crew_Type);
    Hook_Function(0x0057F398, *UnitClass::Start_Driver);
    Hook_Function(0x005808D8, *UnitClass::Offload_Ore_Bail);
    Hook_Function(0x00579EEC, *UnitClass::Edge_Of_World_AI);
    Hook_Function(0x0057FF98, *UnitClass::Flag_Attach);
    Hook_Function(0x0057FFD0, *UnitClass::Flag_Remove);
    Hook_Function(0x00580218, *UnitClass::APC_Close_Door);
    Hook_Function(0x00580238, *UnitClass::APC_Open_Door);
    Hook_Function(0x0058143C, *UnitClass::Credit_Load);
    Hook_Function(0x0057CFA0, *UnitClass::Goto_Ore);
    Hook_Function(0x005804E8, *UnitClass::Ok_To_Move);
    Hook_Function(0x00581644, *UnitClass::Scatter);
    Hook_Function(0x0057FB98, *UnitClass::Mission_Guard);
    Hook_Function(0x005819E8, *UnitClass::Mission_Guard_Area);
    Hook_Function(0x00580554, *UnitClass::Hook_Can_Fire);
    Hook_Function(0x00580F14, *UnitClass::Greatest_Threat);
    Hook_Function(0x005808FC, *UnitClass::Approach_Target);
    Hook_Function(0x005809EC, *UnitClass::Overrun_Cell);
    Hook_Function(0x005803C4, *UnitClass::Hook_Fire_Direction);

    // vessel.cpp
    Hook_Function(0x0058A634, *VesselClass::AI);
    Hook_Function(0x0058B5C4, *VesselClass::Hook_Fire_Coord);
    Hook_Function(0x0058AD00, *VesselClass::Hook_Active_Click_With_Obj);
    Hook_Function(0x0058AD3C, *VesselClass::Hook_Active_Click_With_Cell);
    Hook_Function(0x004CDE80, *VesselClass::Hook_Turret_Facing);
    Hook_Function(0x0058C91C, *VesselClass::Hook_Is_Allowed_To_Recloak);
    Hook_Function(0x0058C654, *VesselClass::Assign_Destination);
    Hook_Function(0x0058D498, *VesselClass::Fire_At);
    Hook_Function_Const(0x0058A040, &VesselClass::Shape_Number);
    Hook_Function(0x0058D26C, *VesselClass::Edge_Of_World_AI);
    Hook_Function(0x0058C23C, *VesselClass::Transport_Open_Door);
    Hook_Function(0x0058C26C, *VesselClass::Transport_Close_Door);
    Hook_Function(0x0058CEE4, *VesselClass::Rotation_AI);
    Hook_Function(0x0058D300, *VesselClass::Repair_AI);

    // overlay.cpp
    Hook_Function(0x00526D64, *OverlayClass::Mark);

    // smudge.cpp
    Hook_Function(0x0054FCF0, *SmudgeClass::Mark);
    Hook_Function(0x0054FFAC, *SmudgeClass::Disown);
    Hook_Function(0x005500B0, *SmudgeClass::Read_INI);

    // teamtype.cpp
    Hook_Function(0x0055FF10, *TeamTypeClass::Hook_Ctor);
    Hook_Function(0x004F9340, *TeamTypeClass::Code_Pointers);
    Hook_Function(0x004F939C, *TeamTypeClass::Decode_Pointers);
    Hook_Function(0x00560310, *TeamTypeClass::Detach);
    Hook_Function(0x0055FFC0, TeamTypeClass::Init);
    Hook_Function(0x0055FFD4, TeamTypeClass::As_Pointer);
    Hook_Function(0x00560020, TeamTypeClass::Mission_From_Name);
    Hook_Function(0x00560070, TeamTypeClass::Name_From_Mission);
    Hook_Function(0x00560368, TeamTypeClass::Read_INI);
    Hook_Function(0x0056076C, TeamTypeClass::Write_INI);
    Hook_Function(0x00560248, TeamTypeClass::From_Name);
    Hook_Function(0x0056016C, TeamTypeClass::Suggested_New_Team);
    Hook_Function_Const(0x005600C4, &TeamTypeClass::Create_One_Of);
    Hook_Function_Const(0x00560114, &TeamTypeClass::Destroy_All_Of);

    // infantry.cpp
    Hook_Function(0x004F24B0, *InfantryClass::Hook_Get_Image_Data);
    Hook_Function(0x004F2564, *InfantryClass::Paradrop);
    Hook_Function(0x004EC3AC, *InfantryClass::Hook_Draw_It);
    Hook_Function(0x004F03D4, *InfantryClass::Hook_Active_Click_With);//obj
    Hook_Function(0x004F049C, *InfantryClass::Hook_Full_Name);
    Hook_Function(0x004ED1FC, *InfantryClass::Detach);
    Hook_Function(0x004F0548, *InfantryClass::Mission_Attack);
    Hook_Function(0x004EDF98, *InfantryClass::Hook_Can_Fire);
    Hook_Function(0x004ED478, *InfantryClass::Assign_Target);
    Hook_Function(0x004EEE00, *InfantryClass::Fire_At);
    Hook_Function(0x004F2504, *InfantryClass::Hook_Is_Ready_To_Random_Animate);
    Hook_Function(0x004F0DB4, *InfantryClass::Edge_Of_World_AI);
    Hook_Function(0x004ED260, *InfantryClass::Assign_Destination);

    // bullet.cpp
    Hook_Function(0x00460B90, *BulletClass::Hook_Ctor);
    Hook_Function(0x00460C38, *BulletClass::Hook_Dtor);
    Hook_Function(0x00460EEC, *BulletClass::Hook_Occupy_List);
    Hook_Function(0x00460FF8, *BulletClass::Mark);
    Hook_Function(0x00461068, *BulletClass::AI);
    Hook_Function_Const(0x004614EC, &BulletClass::Shape_Number);
    Hook_Function(0x004615A0, *BulletClass::Hook_Draw_It);
    Hook_Function(0x004617CC, *BulletClass::Detach);
    Hook_Function(0x00461838, *BulletClass::Unlimbo);
    Hook_Function(0x00461D40, *BulletClass::Hook_Target_Coord);
    Hook_Function(0x00461D8C, *BulletClass::Hook_Sort_Y);
    Hook_Function(0x00461DA8, *BulletClass::Hook_In_Which_Layer);
    Hook_Function_Const(0x00461DEC, &BulletClass::Is_Forced_To_Explode);
    Hook_Function(0x00461FBC, *BulletClass::Bullet_Explodes);
    Hook_Function(0x00462380, *BulletClass::Assign_Target);
    Hook_Function(0x004623B0, *BulletClass::Hook_Overlap_List);
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
ASSERT_SIZEOF(BulletClass, 0x4B);
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
ASSERT_SIZEOF(INIClass, 0x24);
ASSERT_SIZEOF(ListClass, 0x122);
ASSERT_SIZEOF(MissionControlClass, 0x9);
ASSERT_SIZEOF(MixFileClass<GameFileClass>, 0x24);
ASSERT_SIZEOF(MessageListClass, 0x9AB);
ASSERT_SIZEOF(ObjectClass, 0x27);
ASSERT_SIZEOF(ObjectTypeClass, 0x138);
ASSERT_SIZEOF(OptionsClass, 0x7C);
ASSERT_SIZEOF(OverlayClass, 0x2B);
ASSERT_SIZEOF(OverlayTypeClass, 0x146);
ASSERT_SIZEOF(PowerClass, 0x103E);
ASSERT_SIZEOF(RadarClass, 0x100D);
ASSERT_SIZEOF(RulesClass, 0x200);
ASSERT_SIZEOF(ScenarioClass, 0x7D7);
ASSERT_SIZEOF(ScrollClass, 0x1661);
ASSERT_SIZEOF(ShapeButtonClass, 0x38);
ASSERT_SIZEOF(SidebarClass, 0x15FA);
ASSERT_SIZEOF(SliderClass, 0x54);
ASSERT_SIZEOF(SmudgeClass, 0x2B);
ASSERT_SIZEOF(SmudgeTypeClass, 0x145);
ASSERT_SIZEOF(SuperClass, 0x19);
ASSERT_SIZEOF(TabClass, 0x1620);
ASSERT_SIZEOF(TeamClass, 0x5D);
ASSERT_SIZEOF(TeamTypeClass, 0xD1);
ASSERT_SIZEOF(TechnoClass, 0xCD);
ASSERT_SIZEOF(TechnoTypeClass, 0x192);
ASSERT_SIZEOF(TemplateClass, 0x2B);
ASSERT_SIZEOF(TerrainClass, 0x40);
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
