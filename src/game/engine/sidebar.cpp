/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling sidebar.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "sidebar.h"
#include "audio.h"
#include "ccfileclass.h"
#include "coord.h"
#include "drawshape.h"
#include "fading.h"
#include "globals.h"
#include "iomap.h"
#include "lists.h"
#include "mixfile.h"
#include "session.h"
#include "vox.h"

#ifndef RAPP_STANDALONE
void *&SidebarClass::StripClass::LogoShapes = Make_Global<void *>(0x0068A464);
void *&SidebarClass::StripClass::ClockShapes = Make_Global<void *>(0x0068A468);
void **SidebarClass::StripClass::SpecialShapes = Make_Pointer<void *>(0x0068A46C);
SidebarClass::StripClass::SelectClass *SidebarClass::StripClass::SelectButton =
    Make_Pointer<SidebarClass::StripClass::SelectClass>(0x0068A2C4);
ShapeButtonClass *SidebarClass::StripClass::UpButton = Make_Pointer<ShapeButtonClass>(0x0068A1E4);
ShapeButtonClass *SidebarClass::StripClass::DownButton = Make_Pointer<ShapeButtonClass>(0x0068A254);
char *SidebarClass::StripClass::ClockTranslucentTable = Make_Pointer<char>(0x00689F18);
SidebarClass::SBGadgetClass &SidebarClass::Background = Make_Global<SidebarClass::SBGadgetClass>(0x0068A118);
ShapeButtonClass &SidebarClass::RepairButton = Make_Global<ShapeButtonClass>(0x0068A13C);
ShapeButtonClass &SidebarClass::SellButton = Make_Global<ShapeButtonClass>(0x0068A174);
ShapeButtonClass &SidebarClass::ZoomButton = Make_Global<ShapeButtonClass>(0x0068A1AC);
void *&SidebarClass::SidebarShape = Make_Global<void *>(0x00689F0C);
void *&SidebarClass::SidebarMiddleShape = Make_Global<void *>(0x00689F10);
void *&SidebarClass::SidebarBottomShape = Make_Global<void *>(0x00689F14);
#else
void *SidebarClass::StripClass::LogoShapes = nullptr;
void *SidebarClass::StripClass::ClockShapes = nullptr;
void *SidebarClass::StripClass::SpecialShapes[SPECIAL_COUNT];
SidebarClass::StripClass::SelectClass SidebarClass::StripClass::SelectButton[COLUMN_COUNT][ROW_COUNT];
ShapeButtonClass SidebarClass::StripClass::UpButton[COLUMN_COUNT];
ShapeButtonClass SidebarClass::StripClass::DownButton[COLUMN_COUNT];
char SidebarClass::StripClass::ClockTranslucentTable[256][2];
SidebarClass::SBGadgetClass SidebarClass::Background;
ShapeButtonClass SidebarClass::RepairButton;
ShapeButtonClass SidebarClass::SellButton;
ShapeButtonClass SidebarClass::ZoomButton;
void *SidebarClass::SidebarShape = nullptr;
void *SidebarClass::SidebarMiddleShape = nullptr;
void *SidebarClass::SidebarBottomShape = nullptr;
#endif

TextButtonClass SidebarClass::Shore;
TextButtonClass SidebarClass::River;
TextButtonClass SidebarClass::Road;
TextButtonClass SidebarClass::Ridges;
TextButtonClass SidebarClass::Trees;
TextButtonClass SidebarClass::Debris;
ShapeButtonClass SidebarClass::Terrain;
ShapeButtonClass SidebarClass::Ore;
ShapeButtonClass SidebarClass::Gems;
ShapeButtonClass SidebarClass::Water;
ShapeButtonClass SidebarClass::Left;
ShapeButtonClass SidebarClass::Right;
ShapeButtonClass SidebarClass::Maximize;
ShapeButtonClass SidebarClass::StartPosition;
ControlClass SidebarClass::SelectedObject(CONTROL_ED_PREVIEW, 0, 0, 0, 0, 1 | 4, 1);

void *SidebarClass::SidebarAddonShape = nullptr;

SidebarClass::SBGadgetClass::SBGadgetClass() : GadgetClass(496, 154, 143, 244, MOUSE_LEFT_UP, false) {}

BOOL SidebarClass::SBGadgetClass::Action(unsigned flags, KeyNumType &key)
{
    // TODO, needs HelpClass, MouseClass, TechnoClass.
#ifndef RAPP_STANDALONE
    BOOL(*func)
    (const SBGadgetClass *, unsigned, KeyNumType &) =
        reinterpret_cast<BOOL (*)(const SBGadgetClass *, unsigned, KeyNumType &)>(0x0054F380);
    return func(this, flags, key);
#else
    return false;
#endif
}

SidebarClass::StripClass::SelectClass::SelectClass() :
    ControlClass(CONTROL_SB_SELECT_BUTTON, 0, 0, 62, 48, MOUSE_LEFT_PRESS | MOUSE_LEFT_UP | MOUSE_RIGHT_PRESS,
        false), // maybe CAMEO_WIDTH instead of 62? see above
    Owner(nullptr),
    Row(0)
{
}

BOOL SidebarClass::StripClass::SelectClass::Action(unsigned flags, KeyNumType &key)
{
    // TODO, needs HelpClass, MouseClass, TechnoClass, EventClass.
#ifndef RAPP_STANDALONE
    BOOL(*func)
    (const SelectClass *, unsigned, KeyNumType &) =
        reinterpret_cast<BOOL (*)(const SelectClass *, unsigned, KeyNumType &)>(0x0054ECD8);
    return func(this, flags, key);
#else
    return false;
#endif
}

void SidebarClass::StripClass::SelectClass::Set_Owner(StripClass &strip, int row)
{
    Owner = &strip;
    Row = row;
    XPos = strip.XPos;
    YPos = (48 * row) + strip.YPos;
}

SidebarClass::StripClass::StripClass()
{
    // Such empty, this is effectively the "NoInitClass" ctor for this class.
}

SidebarClass::StripClass::StripClass(InitClass const &init) :

    // TODO, one of thse is -1
    XPos(0),
    YPos(0),
    WhichColumn(),
    StripToRedraw(true),
    Strip_Boolean2(false),
    Strip_Boolean4(false),
    Strip_Boolean8(false),
    Strip_Boolean16(false),
    Strip_Boolean32(false),
    field_21(-1),
    CurrentRow(0),
    RowStartIndex(0),
    field_2D(0),
    field_31(0),
    CameoCount(0)
{
}

void SidebarClass::StripClass::One_Time(int column)
{
    // TODO, needs HouseClass, TabClass, HelpClass.
#ifndef RAPP_STANDALONE
    void (*func)(const StripClass *, int) = reinterpret_cast<void (*)(const StripClass *, int)>(0x0054DD54);
    func(this, column);
#else
    char icon_fname[16];
    // ClockShapes = MixFileClass<CCFileClass>::Retrieve("clock.shp");

    // for ( SpecialWeaponType super = SPECIAL_FIRST; super < SPECIAL_COUNT; ++super ) {
    //    snprintf(icon_fname, sizeof(icon_fname), "%.4sicon.shp", SuperWeaponClass::Get_Special_Icon(super));
    //    SpecialShapes[super] = MixFileClass<CCFileClass>::Retrieve(icon_fname);
    //}
#endif
}

void *SidebarClass::StripClass::Get_Special_Cameo(SpecialWeaponType super)
{
    if (super != SPECIAL_NONE && super < SPECIAL_COUNT) {
        return SpecialShapes[super];
    }
    return nullptr;
}

void SidebarClass::StripClass::Init_Clear()
{
    field_21 = -1;
    CurrentRow = 0;
    field_2D = 0;
    CameoCount = 0;
    Strip_Boolean2 = false;
    Strip_Boolean4 = false;
    Strip_Boolean8 = false;

    // Reset all the cameo entries
    for (int i = 0; i < MAX_BUTTONS_PER_COLUMN; ++i) {
        Entries[i].ID = 0;
        Entries[i].Type = RTTI_NONE;
        Entries[i].Factory = 0xFFFFFFFF;
    }
}

void SidebarClass::StripClass::Init_IO(int column)
{
    if (!g_inMapEditor) {
        WhichColumn = column;

        UpButton[WhichColumn].Set_ID(column + GADGET_STRIP_COLUMN_LEFT);
        UpButton[WhichColumn].Set_XPos(XPos + 4);
        UpButton[WhichColumn].Set_YPos(YPos + 194);
        UpButton[WhichColumn].Set_Shape(MixFileClass<CCFileClass>::Retrieve("stripup.shp"));
        UpButton[WhichColumn].Set_Sticky(true);

        DownButton[WhichColumn].Set_ID(column + GADGET_STRIP_COLUMN_RIGHT);
        DownButton[WhichColumn].Set_XPos(XPos + 36);
        DownButton[WhichColumn].Set_YPos(YPos + 194);
        DownButton[WhichColumn].Set_Shape(MixFileClass<CCFileClass>::Retrieve("stripdn.shp"));
        DownButton[WhichColumn].Set_Sticky(true);

        for (int index = 0; index < ROW_COUNT; ++index) {
#ifndef RAPP_STANDALONE
            SelectButton[WhichColumn * 4 + index].Set_Size(64, 48);
            SelectButton[WhichColumn * 4 + index].Set_XPos(XPos);
            SelectButton[WhichColumn * 4 + index].Set_YPos(YPos + index * 48);
            SelectButton[WhichColumn * 4 + index].Set_Owner(*this, index);
#else
            SelectButton[WhichColumn][index].Set_Size(64, 48);
            SelectButton[WhichColumn][index].Set_XPos(XPos);
            SelectButton[WhichColumn][index].Set_YPos(YPos + index * 48);
            SelectButton[WhichColumn][index].Set_Owner(*this, index);
#endif
        }
    }
}

void SidebarClass::StripClass::Init_Theater(TheaterType theater)
{
    static const TLucentType _clock_cols = { 3, 12, 100, 0 };

    PaletteClass palette;

    if (!g_inMapEditor) {
        Reload_LogoShapes();

        if (theater != THEATER_NONE && theater != g_lastTheater) {
            palette = OriginalPalette;

            // Block out 7 (21 / 3 = 7) entries of the temp palette as white (63, 63, 63 is white in 7bit vga pal)
            memset(&palette[32], 63, 21);

            // Build the fading tables for drawing the clock sweep overlay.
#ifndef RAPP_STANDALONE
            Build_Translucent_Table(palette, &_clock_cols, 1, ClockTranslucentTable);
            Conquer_Build_Fading_Table(GamePalette, &ClockTranslucentTable[256], 12, 100);
#else
            Build_Translucent_Table(palette, &_clock_cols, 1, ClockTranslucentTable[0]);
            Conquer_Build_Fading_Table(GamePalette, ClockTranslucentTable[1], 12, 100);
#endif
        }
    }
}

void SidebarClass::StripClass::Reload_LogoShapes()
{
    // TODO Needs HouseClass
#ifndef RAPP_STANDALONE
    void (*func)(const StripClass *) = reinterpret_cast<void (*)(const StripClass *)>(0x0054E094);
    func(this);
#endif
}

void SidebarClass::StripClass::Activate()
{
    UpButton[WhichColumn].Unlink();
    DownButton[WhichColumn].Unlink();
    Map.Add_A_Button(UpButton[WhichColumn]);
    Map.Add_A_Button(DownButton[WhichColumn]);

    for (int index = 0; index < ROW_COUNT; ++index) {
#ifndef RAPP_STANDALONE
        SelectButton[(WhichColumn * ROW_COUNT) + index].Unlink();
        Map.Add_A_Button(SelectButton[(WhichColumn * ROW_COUNT) + index]);
#else
        SelectButton[WhichColumn][index].Unlink();
        Map.Add_A_Button(SelectButton[WhichColumn][index]);
#endif
    }
}

void SidebarClass::StripClass::Deactivate()
{
    Map.Remove_A_Button(UpButton[WhichColumn]);
    Map.Remove_A_Button(DownButton[WhichColumn]);

    for (int index = 0; index < ROW_COUNT; ++index) {
#ifndef RAPP_STANDALONE
        Map.Remove_A_Button(SelectButton[(WhichColumn * ROW_COUNT) + index]);
#else
        Map.Remove_A_Button(SelectButton[WhichColumn][index]);
#endif
    }
}

BOOL SidebarClass::StripClass::Add(RTTIType type, int id)
{
    DEBUG_ASSERT(type >= RTTI_NONE);
    DEBUG_ASSERT(type < RTTI_COUNT);

    // Check our cameo count isn't greater than we have space for. If it is, return false.
    if (CameoCount < MAX_BUTTONS_PER_COLUMN) {
        // If the list already has at least 1 cameo in it, check we aren't trying to add the same one again, if we are,
        // return false.
        if (CameoCount > 0) {
            for (int index = 0; index < MAX_BUTTONS_PER_COLUMN; ++index) {
                if (Entries[index].Type == type && Entries[index].ID == id) {
                    return false;
                }
            }
        }

        if (!ScenarioInit && type != RTTI_SPECIAL) {
            Speak(VOX_NEW_OPTIONS);
        }

        Entries[CameoCount].Type = type;
        Entries[CameoCount].ID = id;
        ++CameoCount;
        StripToRedraw = true;

        return true;
    }

    return false;
}

BOOL SidebarClass::StripClass::Scroll(BOOL reverse)
{
    if (reverse) {
        // We can't go below 0 for the current row, so check and if we are trying to, just return false.
        if (CurrentRow == 0) {
            return false;
        }

        --RowStartIndex;
    } else {
        // We can't go beyond the count of cameos we have, so again return false if we try.
        if (CurrentRow + ROW_COUNT >= CameoCount) {
            return false;
        }

        ++RowStartIndex;
    }

    return true;
}

void SidebarClass::StripClass::Flag_To_Redraw()
{
    StripToRedraw = true;
    Map.Flag_To_Redraw();
}

BOOL SidebarClass::StripClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
    // TODO needs EventClass and FactoryClass
#ifndef RAPP_STANDALONE
    BOOL(*func)
    (const StripClass *, KeyNumType &, int, int) =
        reinterpret_cast<BOOL (*)(const StripClass *, KeyNumType &, int, int)>(0x0054E2E8);
    return func(this, key, mouse_x, mouse_y);
#endif
}

void SidebarClass::StripClass::Draw_It(BOOL force_redraw)
{
    // TODO Needs HouseClass and TechnoClass
#ifndef RAPP_STANDALONE
    void (*func)(const StripClass *, BOOL) = reinterpret_cast<void (*)(const StripClass *, BOOL)>(0x0054E6FC);
    func(this, force_redraw);
#endif
}

BOOL SidebarClass::StripClass::Recalc()
{
    // TODO Needs HouseClass, HouseTypeClass, and TechnoClass
#ifndef RAPP_STANDALONE
    BOOL (*func)(const StripClass *) = reinterpret_cast<BOOL (*)(const StripClass *)>(0x0054EB1C);
    return func(this);
#endif
}

BOOL SidebarClass::StripClass::Factory_Link(int factory_id, RTTIType type, int id)
{
    for (int index = 0; index < CameoCount; ++index) {
        if (Entries[index].Type == type && Entries[index].ID == id) {
            Entries[index].Factory = factory_id;
            Strip_Boolean2 = true;
            Flag_To_Redraw();

            return true;
        }
    }

    return false;
}

int SidebarClass::StripClass::Abandon_Production(int unk1)
{
    // TODO Needs FactoryClass
#ifndef RAPP_STANDALONE
    int (*func)(const StripClass *, int) = reinterpret_cast<int (*)(const StripClass *, int)>(0x0054F434);
    return func(this, unk1);
#endif
}

SidebarClass::SidebarClass() :
    PowerClass(),
    SidebarIsDrawn(false),
    SidebarToRedraw(true),
    SidebarBit4(false),
    SidebarBit8(false),
    SidebarBit16(false)
{
    InitClass init;
    for (int column = 0; column < COLUMN_COUNT; ++column) {
        // Placement new to perform construction of object array with specific constructor.
        new (&Columns[column]) StripClass(init);
    }
}

void SidebarClass::One_Time()
{
    PowerClass::One_Time();

    WindowList[WINDOW_SIDEBAR].X = 496;
    WindowList[WINDOW_SIDEBAR].Y = 180;
    WindowList[WINDOW_SIDEBAR].W = 160;
    WindowList[WINDOW_SIDEBAR].H = 192;

    for (int column = 0; column < COLUMN_COUNT; ++column) {
        Columns[column].One_Time(column);
    }

    if (SidebarShape == nullptr) {
        SidebarShape = MixFileClass<CCFileClass>::Retrieve("sidebar.shp");
    }

    if (SidebarAddonShape == nullptr) {
        SidebarAddonShape = MixFileClass<CCFileClass>::Retrieve("addon.shp");
    }
}

void SidebarClass::Init_Clear()
{
    PowerClass::Init_Clear();
    SidebarToRedraw = true;
    if (!g_inMapEditor) {
        SidebarBit4 = false;
        SidebarBit8 = false;
        SidebarBit16 = false;

        for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
            Columns[column].Init_Clear();
        }

        Activate(0);
    }
}

void SidebarClass::Init_IO()
{
    PowerClass::Init_IO();

    if (!g_inMapEditor) {
        // TODO remove hard coded position constants.
        RepairButton.Set_ID(BUTTON_REPAIR);
        RepairButton.Set_Position(498, 150);
        RepairButton.Set_Sticky(true);
        RepairButton.Set_Toggle_Bool1(false);
        RepairButton.Set_Toggle_Disabled(true);
        RepairButton.Set_Shape_Bool_One(true);
        RepairButton.Set_Shape(MixFileClass<CCFileClass>::Retrieve("repair.shp"));

        SellButton.Set_ID(BUTTON_SELL);
        SellButton.Set_Position(543, 150);
        SellButton.Set_Sticky(true);
        SellButton.Set_Toggle_Bool1(false);
        SellButton.Set_Toggle_Disabled(true);
        SellButton.Set_Shape_Bool_One(true);
        SellButton.Set_Shape(MixFileClass<CCFileClass>::Retrieve("sell.shp"));

        ZoomButton.Set_ID(BUTTON_ZOOM);
        ZoomButton.Set_Position(588, 150);
        ZoomButton.Set_Sticky(true);
        ZoomButton.Set_Toggle_Bool1(false);
        ZoomButton.Set_Shape(MixFileClass<CCFileClass>::Retrieve("map.shp"));

        if ((RadarActive && Is_Zoomable()) || Session.Game_To_Play() != GAME_CAMPAIGN) {
            ZoomButton.Enable();
        } else {
            ZoomButton.Disable();
        }

        for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
            Columns[column].Init_IO(column);
        }

        if (SidebarIsDrawn) {
            SidebarIsDrawn = false;
            Activate(1);
        }
    }
}

void SidebarClass::Init_Theater(TheaterType theater)
{
    PowerClass::Init_Theater(theater);

    if (!g_inMapEditor) {
        Reload_Sidebar();

        for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
            Columns[column].Init_Theater(theater);
        }
    }
}

void SidebarClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
    // Requires HouseClass
#ifndef RAPP_STANDALONE
    void (*func)(const SidebarClass *, KeyNumType &, int, int) =
        reinterpret_cast<void (*)(const SidebarClass *, KeyNumType &, int, int)>(0x0054D8F0);
    func(this, key, mouse_x, mouse_y);
#endif
}

void SidebarClass::Draw_It(BOOL force_redraw)
{
    PowerClass::Draw_It(force_redraw);

    if (!g_inMapEditor) {
        if (SidebarIsDrawn && (SidebarToRedraw || force_redraw) && !g_inMapEditor ) {
            SidebarToRedraw = false;
            if (g_logicPage->Lock()) {
                if (SidebarShape != nullptr) {
                    CC_Draw_Shape(SidebarShape,
                        0,
                        480, // GVPC Width - Sidebar width
                        16, // 16, Height of Tabs
                        WINDOW_0,
                        SHAPE_WIN_REL);
                }

                if (SidebarMiddleShape != nullptr) {
                    CC_Draw_Shape(SidebarMiddleShape,
                        force_redraw ? 0 : 1,
                        480,
                        176, // Height of tab + 160, height of the first sidebar piece
                        WINDOW_0,
                        SHAPE_WIN_REL);
                }

                if (SidebarBottomShape != nullptr) {
                    CC_Draw_Shape(SidebarBottomShape,
                        force_redraw ? 0 : 1,
                        480,
                        276, // Height of tab + 160 + 100,, height of last piece added on
                        WINDOW_0,
                        SHAPE_WIN_REL);
                }

                // Handles drawing extra bits for higher resolutions.
                if (g_logicPage->Get_Height() > 400) {
                    if (SidebarAddonShape != nullptr) {
                        int addonheight = Get_Build_Frame_Height(SidebarAddonShape);

                        // TODO
                        // new shape, draw this as the resolution requires the black underneath to be filled with a image.
                        CC_Draw_Shape(SidebarAddonShape,
                            0,
                            480,
                            400, // Draw after everything else... Sidebar height.
                            WINDOW_0,
                            SHAPE_WIN_REL);
                    }
                }

                RepairButton.Draw_Me(true);
                SellButton.Draw_Me(true);
                ZoomButton.Draw_Me(true);

                // for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
                //    Columns[column].StripToRedraw = true;
                //}

                g_logicPage->Unlock();
            }
        }

        if (SidebarIsDrawn) {
            for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
                Columns[column].Draw_It(force_redraw);
            }

            if (SidebarToRedraw || force_redraw) {
                RepairButton.Draw_Me(true);
                SellButton.Draw_Me(true);
                ZoomButton.Draw_Me(true);
            }
        }

        SidebarToRedraw = false;
    }
}

void SidebarClass::Refresh_Cells(int16_t cellnum, int16_t *overlap_list)
{
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);
    if (overlap_list != nullptr) {
        if (overlap_list[0] == LIST_START) {
            for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
                Columns[column].StripToRedraw = true;
            }

            SidebarToRedraw = true;
            Flag_To_Redraw();
        }
    }

    PowerClass::Refresh_Cells(cellnum, overlap_list);
}

void SidebarClass::Reload_Sidebar()
{
// TODO Needs HouseClass
#ifndef RAPP_STANDALONE
    void (*func)(const SidebarClass *) = reinterpret_cast<void (*)(const SidebarClass *)>(0x0054D340);
    func(this);
#else
    // in order of the houses.
    static char sidebarnames[HOUSES_COUNT][12] = { "SIDE?NA.SHP",
        "SIDE?NA.SHP",
        "SIDE?US.SHP",
        "SIDE?NA.SHP",
        "SIDE?US.SHP",
        "SIDE?NA.SHP",
        "SIDE?NA.SHP",
        "SIDE?NA.SHP",
        "SIDE?NA.SHP",
        "SIDE?US.SHP" };

    int side_index;

    if (PlayerPtr) {
        side_index = PlayerPtr->Side;
    } else {
        side_index = 0;
    }

    // this basicly replaces the '?' in the filenames above with a number.
    sidebarnames[side_index][4] = '1';
    SidebarShape = MixFileClass<CCFileClass>::Retrieve(sidebarnames[side_index]);
    sidebarnames[side_index][4] = '2';
    SidebarMiddleShape = MixFileClass<CCFileClass>::Retrieve(sidebarnames[side_index]);
    sidebarnames[side_index][4] = '3';
    SidebarBottomShape = MixFileClass<CCFileClass>::Retrieve(sidebarnames[side_index]);

    // reload the side specific stip backgrounds.
    Strips[COLUMN_LEFT].Reload_LogoShapes();
    Strips[COLUMN_LEFT].Reload_LogoShapes();
#endif
}

int SidebarClass::Which_Column(RTTIType type)
{
    switch (type) {
        case RTTI_BUILDING:
        case RTTI_BUILDINGTYPE:
            return COLUMN_LEFT;

        case RTTI_AIRCRAFT:
        case RTTI_AIRCRAFTTYPE:
        case RTTI_INFANTRY:
        case RTTI_INFANTRYTYPE:
        case RTTI_UNIT:
        case RTTI_UNITTYPE:
        case RTTI_VESSEL:
        case RTTI_VESSELTYPE:
            return COLUMN_RIGHT;

        default:
            DEBUG_ASSERT_PRINT(false, "Unhandled RTTIType in Which_Column() returning COLUMN_RIGHT, We should never reach here!");
            return COLUMN_RIGHT;
    }
}

BOOL SidebarClass::Factory_Link(int factory_id, RTTIType type, int id)
{
    return Columns[Which_Column(type)].Factory_Link(factory_id, type, id);
}

BOOL SidebarClass::Activate_Repair(int state)
{
    bool to_redraw = SidebarToRedraw;

    if (state == -1) {
        state = (SidebarBit4 == 0);
    }

    if (state == 1) {
        SidebarBit4 = true;
    } else {
        SidebarBit4 = false;
    }

    if (SidebarToRedraw != to_redraw) {
        Flag_To_Redraw();
        SidebarToRedraw = true;

        if (!SidebarBit4) {
            Help_Text(TXT_NONE);
            Set_Default_Mouse(MOUSE_POINTER);
        }
    }

    return to_redraw;
}

BOOL SidebarClass::Activate_Upgrade(int state) // Actually Sell?
{
    bool to_redraw = SidebarToRedraw;

    if (state == -1) {
        state = (SidebarBit8 == 0);
    }

    if (state == 1) {
        SidebarBit8 = true;
    } else {
        SidebarBit8 = false;
    }

    if (SidebarToRedraw != to_redraw) {
        Flag_To_Redraw();
        SidebarToRedraw = true;

        if (!SidebarBit8) {
            Set_Default_Mouse(MOUSE_POINTER);
        }
    }

    return to_redraw;
}

BOOL SidebarClass::Activate_Demolish(int state) // Actually Zoom?
{
    bool to_redraw = SidebarToRedraw;

    if (state == -1) {
        state = (SidebarBit16 == 0);
    }

    if (state == 1) {
        SidebarBit16 = true;
    } else {
        SidebarBit16 = false;
    }

    if (SidebarToRedraw != to_redraw) {
        Flag_To_Redraw();
        SidebarToRedraw = true;

        if (!SidebarBit16) {
            Set_Default_Mouse(MOUSE_POINTER);
        }
    }

    return to_redraw;
}

BOOL SidebarClass::Add(RTTIType item, int id)
{
    if (g_inMapEditor) {
        return false;
    }

    // get Left or Right strip and add the "item", based on return value of Which_Column().
    if (Columns[Which_Column(item)].Add(item, id)) {
        Activate(1);
        SidebarToRedraw = true;
        Flag_To_Redraw();

        return true;
    }

    return false;
}

BOOL SidebarClass::Scroll(BOOL reverse, int column)
{
    BOOL scroll_result;

    if (column == COLUMN_NONE) {
        scroll_result = (Columns[COLUMN_LEFT].Scroll(reverse) || Columns[COLUMN_RIGHT].Scroll(reverse));

        if (!scroll_result) {
            Sound_Effect(VOC_SCOLDY1, fixed::_1_1, 1, 0, HOUSES_NONE);
        }

        if (!scroll_result) {
            return false;
        }

        SidebarToRedraw = true;
        Flag_To_Redraw();

        return true;
    }

    scroll_result = Columns[column].Scroll(reverse);

    if (!scroll_result) {
        return false;
    }

    SidebarToRedraw = true;
    Flag_To_Redraw();

    return true;
}

void SidebarClass::Recalc()
{
    BOOL recalculated = false;

    for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
        recalculated = Columns[column].Recalc();

        if (recalculated) {
            break;
        }
    }

    if (recalculated) {
        SidebarToRedraw = true;
        Flag_To_Redraw();
    }
}

BOOL SidebarClass::Activate(int mode)
{
    bool prevvalue = SidebarIsDrawn;

    if (!g_inMapEditor) {
        if (!Session.Playback_Game()) {
            SidebarIsDrawn = (mode == SIDEBAR_TOGGLE ? !SidebarIsDrawn : mode == SIDEBAR_ACTIVATE);

            bool newvalue = SidebarIsDrawn;

            if (newvalue != prevvalue) {
                if (newvalue) {
                    Set_View_Dimensions(0, 16 /*TabClass::TabButtonHeight*/, 20, -1);
                    SidebarToRedraw = true;
                    Help_Text(TXT_NULL, -1, -1);
                    RepairButton.Unlink();
                    Add_A_Button(RepairButton);
                    SellButton.Unlink();
                    Add_A_Button(SellButton);
                    ZoomButton.Unlink();
                    Add_A_Button(ZoomButton);

                    for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
                        Columns[column].Activate();
                    }

                    Background.Unlink();
                    Add_A_Button(Background);
                    RadarButton.Unlink();
                    Add_A_Button(RadarButton);
                    PowerButton.Unlink();
                    Add_A_Button(PowerButton);
                } else {
                    Set_View_Dimensions(0, 16 /*TabClass::TabButtonHeight*/, -1, -1);
                    Help_Text(TXT_NULL, -1, -1);
                    Remove_A_Button(RepairButton);
                    Remove_A_Button(SellButton);
                    Remove_A_Button(ZoomButton);

                    for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
                        Columns[column].Deactivate();
                    }

                    Remove_A_Button(Background);
                    Remove_A_Button(RadarButton);
                    Remove_A_Button(PowerButton);
                }

                Flag_To_Redraw(true);
            }
        }
    }
    return prevvalue;
}

int SidebarClass::Abandon_Production(RTTIType type, int unk2)
{
    return Columns[Which_Column(type)].Abandon_Production(unk2);
}

void SidebarClass::Zoom_Mode_Control(ModeControl mode)
{
    if (!RadarActive) {
        if (Session.Game_To_Play() != GAME_CAMPAIGN) {
            Player_Names(!RadarDrawNames);
        }
    } else if (RadarZoomed || Session.Game_To_Play() == GAME_CAMPAIGN) {
        if (RadarZoomed || !Spy_Next_House()) {
            Zoom_Mode(Coord_To_Cell(DisplayPos));
        }
    } else if (!RadarDrawNames && !RadarZoomed) {
        Player_Names(true);
    } else {
        Player_Names(false);

        if (!Spy_Next_House()) {
            Zoom_Mode(Coord_To_Cell(DisplayPos));
        }
    }
}
