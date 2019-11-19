/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling sidebar.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "sidebar.h"
#include "bench.h"
#include "coord.h"
#include "drawshape.h"
#include "fading.h"
#include "gamefile.h"
#include "globals.h"
#include "iomap.h"
#include "lists.h"
#include "mixfile.h"
#include "palette.h"
#include "session.h"
#include "voc.h"
#include "vox.h"

namespace
{
const NoInitClass noinit;
}

#ifndef GAME_DLL
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

void *SidebarClass::SidebarAddonShape = nullptr;

SidebarClass::SBGadgetClass::SBGadgetClass() :
    GadgetClass(496, 154, 143, 244, MOUSE_LEFT_UP, false)
{
}

BOOL SidebarClass::SBGadgetClass::Action(unsigned flags, KeyNumType &key)
{
    Map.Help_Text(TXT_NULL);
    Map.Override_Mouse_Shape(MOUSE_POINTER);
    return true;
}

SidebarClass::StripClass::SelectClass::SelectClass() :
    ControlClass(CONTROL_SB_SELECT_BUTTON, 0, 0, 62, 48, MOUSE_LEFT_PRESS | MOUSE_LEFT_UP | MOUSE_RIGHT_PRESS, false),
    m_Owner(nullptr),
    m_Row(0)
{
}

BOOL SidebarClass::StripClass::SelectClass::Action(unsigned flags, KeyNumType &key)
{
    // TODO, needs HelpClass, MouseClass, TechnoClass, EventClass.
#ifdef GAME_DLL
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
    m_Owner = &strip;
    m_Row = row;
    m_XPos = strip.m_XPos;
    m_YPos = (48 * row) + strip.m_YPos;
}

SidebarClass::StripClass::StripClass() :
    m_ProgressTimer()
{
}

SidebarClass::StripClass::StripClass(InitClass const &init) :
    m_XPos(0),
    m_YPos(0),
    m_WhichColumn(),
    m_StripToRedraw(true),
    m_Strip_Boolean2(false),
    m_Strip_Boolean4(false),
    m_Strip_Boolean8(false),
    m_Strip_Boolean16(false),
    m_Strip_Boolean32(false),
    m_field_21(-1),
    m_CurrentRow(0),
    m_RowStartIndex(0),
    m_field_2D(0),
    m_field_31(0),
    m_CameoCount(0)
{
    for (int i = 0; i < MAX_BUTTONS_PER_COLUMN; ++i) {
        m_Entries[i].ID = 0;
        m_Entries[i].Type = RTTI_NONE;
        m_Entries[i].Factory = -1;
    }
}

void SidebarClass::StripClass::One_Time(int column)
{
    // TODO, needs HouseClass, TabClass, HelpClass.
#ifdef GAME_DLL
    void (*func)(const StripClass *, int) = reinterpret_cast<void (*)(const StripClass *, int)>(0x0054DD54);
    func(this, column);
#else
    char icon_fname[16];
    // ClockShapes = GameFileClass::Retrieve("clock.shp");

    // for ( SpecialWeaponType super = SPECIAL_FIRST; super < SPECIAL_COUNT; ++super ) {
    //    snprintf(icon_fname, sizeof(icon_fname), "%.4sicon.shp", SuperWeaponClass::Get_Special_Icon(super));
    //    SpecialShapes[super] = GameFileClass::Retrieve(icon_fname);
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
    m_field_21 = -1;
    m_CurrentRow = 0;
    m_field_2D = 0;
    m_CameoCount = 0;
    m_Strip_Boolean2 = false;
    m_Strip_Boolean4 = false;
    m_Strip_Boolean8 = false;

    // Reset all the cameo entries
    for (int i = 0; i < MAX_BUTTONS_PER_COLUMN; ++i) {
        m_Entries[i].ID = 0;
        m_Entries[i].Type = RTTI_NONE;
        m_Entries[i].Factory = -1;
    }
}

void SidebarClass::StripClass::Init_IO(int column)
{
    if (!g_InMapEditor) {
        m_WhichColumn = column;

        UpButton[m_WhichColumn].Set_ID(column + GADGET_STRIP_COLUMN_LEFT);
        UpButton[m_WhichColumn].Set_XPos(m_XPos + 4);
        UpButton[m_WhichColumn].Set_YPos(m_YPos + 194);
        UpButton[m_WhichColumn].Set_Shape(GameFileClass::Retrieve("stripup.shp"));
        UpButton[m_WhichColumn].Set_Sticky(true);

        DownButton[m_WhichColumn].Set_ID(column + GADGET_STRIP_COLUMN_RIGHT);
        DownButton[m_WhichColumn].Set_XPos(m_XPos + 36);
        DownButton[m_WhichColumn].Set_YPos(m_YPos + 194);
        DownButton[m_WhichColumn].Set_Shape(GameFileClass::Retrieve("stripdn.shp"));
        DownButton[m_WhichColumn].Set_Sticky(true);

        for (int index = 0; index < ROW_COUNT; ++index) {
            SelectButton[m_WhichColumn][index].Set_ID(220);
            SelectButton[m_WhichColumn][index].Set_Size(64, 48);
            SelectButton[m_WhichColumn][index].Set_XPos(m_XPos);
            SelectButton[m_WhichColumn][index].Set_YPos(m_YPos + index * 48);
            SelectButton[m_WhichColumn][index].Set_Owner(*this, index);
        }
    }
}

void SidebarClass::StripClass::Init_Theater(TheaterType theater)
{
    static const TLucentType _clock_cols = { 3, 12, 100, 0 };

    PaletteClass palette;

    if (!g_InMapEditor) {
        Reload_LogoShapes();

        if (theater != THEATER_NONE && theater != g_lastTheater) {
            palette = OriginalPalette;

            // Block out 7 (21 / 3 = 7) entries of the temp palette as white (63, 63, 63 is white in 7bit vga pal)
            memset(&palette[32], 63, 21);

            // Build the fading tables for drawing the clock sweep overlay.
            Build_Translucent_Table(palette, &_clock_cols, 1, ClockTranslucentTable[0]);
            Conquer_Build_Fading_Table(GamePalette, ClockTranslucentTable[1], 12, 100);
        }
    }
}

void SidebarClass::StripClass::Reload_LogoShapes()
{
    // TODO Needs HouseClass
#ifdef GAME_DLL
    void (*func)(const StripClass *) = reinterpret_cast<void (*)(const StripClass *)>(0x0054E094);
    func(this);
#endif
}

void SidebarClass::StripClass::Activate()
{
    UpButton[m_WhichColumn].Unlink();
    DownButton[m_WhichColumn].Unlink();
    Map.Add_A_Button(UpButton[m_WhichColumn]);
    Map.Add_A_Button(DownButton[m_WhichColumn]);

    for (int index = 0; index < ROW_COUNT; ++index) {
        SelectButton[m_WhichColumn][index].Unlink();
        Map.Add_A_Button(SelectButton[m_WhichColumn][index]);
    }
}

void SidebarClass::StripClass::Deactivate()
{
    Map.Remove_A_Button(UpButton[m_WhichColumn]);
    Map.Remove_A_Button(DownButton[m_WhichColumn]);

    for (int index = 0; index < ROW_COUNT; ++index) {
        Map.Remove_A_Button(SelectButton[m_WhichColumn][index]);
    }
}

BOOL SidebarClass::StripClass::Add(RTTIType type, int id)
{
    DEBUG_ASSERT(type >= RTTI_NONE);
    DEBUG_ASSERT(type < RTTI_COUNT);

    // Check our cameo count isn't greater than we have space for. If it is, return false.
    if (m_CameoCount < MAX_BUTTONS_PER_COLUMN) {
        // If the list already has at least 1 cameo in it, check we aren't trying to add the same one again, if we are,
        // return false.
        if (m_CameoCount > 0) {
            for (int index = 0; index < MAX_BUTTONS_PER_COLUMN; ++index) {
                if (m_Entries[index].Type == type && m_Entries[index].ID == id) {
                    return false;
                }
            }
        }

        if (!ScenarioInit && type != RTTI_SPECIAL) {
            Speak(VOX_NEW_OPTIONS);
        }

        m_Entries[m_CameoCount].Type = type;
        m_Entries[m_CameoCount].ID = id;
        ++m_CameoCount;
        m_StripToRedraw = true;

        return true;
    }

    return false;
}

BOOL SidebarClass::StripClass::Scroll(BOOL reverse)
{
    if (reverse) {
        // We can't go below 0 for the current row, so check and if we are trying to, just return false.
        if (m_CurrentRow == 0) {
            return false;
        }

        --m_RowStartIndex;
    } else {
        // We can't go beyond the count of cameos we have, so again return false if we try.
        if (m_CurrentRow + ROW_COUNT >= m_CameoCount) {
            return false;
        }

        ++m_RowStartIndex;
    }

    return true;
}

void SidebarClass::StripClass::Flag_To_Redraw()
{
    m_StripToRedraw = true;
    Map.Flag_To_Redraw();
}

BOOL SidebarClass::StripClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
    // TODO needs EventClass and FactoryClass
#ifdef GAME_DLL
    BOOL(*func)
    (const StripClass *, KeyNumType &, int, int) =
        reinterpret_cast<BOOL (*)(const StripClass *, KeyNumType &, int, int)>(0x0054E2E8);
    return func(this, key, mouse_x, mouse_y);
#else
    return true;
#endif
}

void SidebarClass::StripClass::Draw_It(BOOL force_redraw)
{
    // TODO Needs HouseClass and TechnoClass
#ifdef GAME_DLL
    void (*func)(const StripClass *, BOOL) = reinterpret_cast<void (*)(const StripClass *, BOOL)>(0x0054E6FC);
    func(this, force_redraw);
#endif
}

BOOL SidebarClass::StripClass::Recalc()
{
    // TODO Needs HouseClass, HouseTypeClass, and TechnoClass
#ifdef GAME_DLL
    BOOL (*func)(const StripClass *) = reinterpret_cast<BOOL (*)(const StripClass *)>(0x0054EB1C);
    return func(this);
#else
    return true;
#endif
}

BOOL SidebarClass::StripClass::Factory_Link(int factory_id, RTTIType type, int id)
{
    for (int index = 0; index < m_CameoCount; ++index) {
        if (m_Entries[index].Type == type && m_Entries[index].ID == id) {
            m_Entries[index].Factory = factory_id;
            m_Strip_Boolean2 = true;
            Flag_To_Redraw();

            return true;
        }
    }

    return false;
}

int SidebarClass::StripClass::Abandon_Production(int unk1)
{
    // TODO Needs FactoryClass
#ifdef GAME_DLL
    int (*func)(const StripClass *, int) = reinterpret_cast<int (*)(const StripClass *, int)>(0x0054F434);
    return func(this, unk1);
#else
    return 0;
#endif
}

SidebarClass::SidebarClass() :
    PowerClass(),
    m_SidebarIsDrawn(false),
    m_SidebarToRedraw(true),
    m_SidebarBit4(false),
    m_SidebarBit8(false),
    m_SidebarBit16(false)
{
    InitClass init;
    for (int column = 0; column < COLUMN_COUNT; ++column) {
        // Placement new to perform construction of object array with specific constructor.
        new (&m_Columns[column]) StripClass(init);
    }
    m_Columns[0].Set_Position(496, 180);
    m_Columns[1].Set_Position(566, 180);
}

void SidebarClass::One_Time()
{
    PowerClass::One_Time();

    WindowList[WINDOW_SIDEBAR].X = 496;
    WindowList[WINDOW_SIDEBAR].Y = 180;
    WindowList[WINDOW_SIDEBAR].W = 160;
    WindowList[WINDOW_SIDEBAR].H = 192;

    for (int column = 0; column < COLUMN_COUNT; ++column) {
        m_Columns[column].One_Time(column);
    }

    if (SidebarShape == nullptr) {
        SidebarShape = GameFileClass::Retrieve("sidebar.shp");
    }

    // NOTE: Chronoshift hi-res sidebar addition.
    if (SidebarAddonShape == nullptr) {
        SidebarAddonShape = GameFileClass::Retrieve("addon.shp");
    }
}

void SidebarClass::Init_Clear()
{
    PowerClass::Init_Clear();
    m_SidebarToRedraw = true;
    if (!g_InMapEditor) {
        m_SidebarBit4 = false;
        m_SidebarBit8 = false;
        m_SidebarBit16 = false;

        for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
            m_Columns[column].Init_Clear();
        }

        Activate(0);
    }
}

void SidebarClass::Init_IO()
{
    PowerClass::Init_IO();

    if (!g_InMapEditor) {
        RepairButton.Set_ID(BUTTON_REPAIR);
        RepairButton.Set_Position(498, 150);
        RepairButton.Set_Sticky(true);
        RepairButton.Set_Toggle_Bool1(false);
        RepairButton.Set_Toggle_Disabled(true);
        RepairButton.Set_Shape_Bool_One(true);
        RepairButton.Set_Shape(GameFileClass::Retrieve("repair.shp"));

        SellButton.Set_ID(BUTTON_SELL);
        SellButton.Set_Position(543, 150);
        SellButton.Set_Sticky(true);
        SellButton.Set_Toggle_Bool1(false);
        SellButton.Set_Toggle_Disabled(true);
        SellButton.Set_Shape_Bool_One(true);
        SellButton.Set_Shape(GameFileClass::Retrieve("sell.shp"));

        ZoomButton.Set_ID(BUTTON_ZOOM);
        ZoomButton.Set_Position(588, 150);
        ZoomButton.Set_Sticky(true);
        ZoomButton.Set_Toggle_Bool1(false);
        ZoomButton.Set_Shape(GameFileClass::Retrieve("map.shp"));

        if ((m_RadarActive && Is_Zoomable()) || Session.Game_To_Play() != GAME_CAMPAIGN) {
            ZoomButton.Enable();
        } else {
            ZoomButton.Disable();
        }

        for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
            m_Columns[column].Init_IO(column);
        }

        if (m_SidebarIsDrawn) {
            m_SidebarIsDrawn = false;
            Activate(1);
        }
    }
}

void SidebarClass::Init_Theater(TheaterType theater)
{
    PowerClass::Init_Theater(theater);

    if (!g_InMapEditor) {
        Reload_Sidebar();

        for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
            m_Columns[column].Init_Theater(theater);
        }
    }
}

void SidebarClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
    // Requires HouseClass
#ifdef GAME_DLL
    void (*func)(const SidebarClass *, KeyNumType &, int, int) =
        reinterpret_cast<void (*)(const SidebarClass *, KeyNumType &, int, int)>(0x0054D8F0);
    func(this, key, mouse_x, mouse_y);
#endif
}

void SidebarClass::Draw_It(BOOL force_redraw)
{
    PowerClass::Draw_It(force_redraw);

    if (!g_InMapEditor) {

        BENCHMARK_START(BENCH_SIDEBAR);

        if (m_SidebarIsDrawn && (m_SidebarToRedraw || force_redraw) && !g_InMapEditor ) {
            m_SidebarToRedraw = false;
            if (g_logicPage->Lock()) {
                if (SidebarShape != nullptr) {
                    CC_Draw_Shape(SidebarShape,
                        0,
                        480,
                        16,
                        WINDOW_0,
                        SHAPE_WIN_REL);
                }

                if (SidebarMiddleShape != nullptr) {
                    CC_Draw_Shape(SidebarMiddleShape,
                        force_redraw ? 0 : 1,
                        480,
                        176, // Height of tab + 160, height of the first sidebar piece.
                        WINDOW_0,
                        SHAPE_WIN_REL);
                }

                if (SidebarBottomShape != nullptr) {
                    CC_Draw_Shape(SidebarBottomShape,
                        force_redraw ? 0 : 1,
                        480,
                        276, // Height of tab + 160 + 100,, height of last piece added on.
                        WINDOW_0,
                        SHAPE_WIN_REL);
                }

                // Handles drawing extra bits for higher resolutions.
                if (g_logicPage->Get_Height() > 400) {
                    if (SidebarAddonShape != nullptr) {
                        int addonheight = Get_Build_Frame_Height(SidebarAddonShape);
                        CC_Draw_Shape(SidebarAddonShape,
                            0,
                            480,
                            400,
                            WINDOW_0,
                            SHAPE_WIN_REL);
                    }
                }

                RepairButton.Draw_Me(true);
                SellButton.Draw_Me(true);
                ZoomButton.Draw_Me(true);

                /*for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
                    m_Columns[column].m_StripToRedraw = true;
                }*/

                g_logicPage->Unlock();
            }
        }

        if (m_SidebarIsDrawn) {
            for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
                m_Columns[column].Draw_It(force_redraw);
            }

            if (m_SidebarToRedraw || force_redraw) {
                RepairButton.Draw_Me(true);
                SellButton.Draw_Me(true);
                ZoomButton.Draw_Me(true);
            }
        }

        m_SidebarToRedraw = false;

        BENCHMARK_END(BENCH_SIDEBAR);
    }
}

void SidebarClass::Refresh_Cells(cell_t cellnum, const int16_t *list)
{
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);
    if (list != nullptr) {
        if (list[0] == LIST_START) {
            for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
                m_Columns[column].m_StripToRedraw = true;
            }

            m_SidebarToRedraw = true;
            Flag_To_Redraw();
        }
    }

    PowerClass::Refresh_Cells(cellnum, list);
}

void SidebarClass::Reload_Sidebar()
{
// TODO Needs HouseClass
#ifdef GAME_DLL
    void (*func)(const SidebarClass *) = reinterpret_cast<void (*)(const SidebarClass *)>(0x0054D340);
    func(this);
#elif 0
    // in order of the houses.
    static char sidebarnames[HOUSES_COUNT][12] = { "side?na.shp",
        "side?na.shp",
        "side?us.shp",
        "side?na.shp",
        "side?us.shp",
        "side?na.shp",
        "side?na.shp",
        "side?na.shp",
        "side?na.shp",
        "side?us.shp" };

    int side_index;

    if (PlayerPtr) {
        side_index = PlayerPtr->Side;
    } else {
        side_index = 0;
    }

    // this basicly replaces the '?' in the filenames above with a number.
    sidebarnames[side_index][4] = '1';
    SidebarShape = GameFileClass::Retrieve(sidebarnames[side_index]);
    sidebarnames[side_index][4] = '2';
    SidebarMiddleShape = GameFileClass::Retrieve(sidebarnames[side_index]);
    sidebarnames[side_index][4] = '3';
    SidebarBottomShape = GameFileClass::Retrieve(sidebarnames[side_index]);

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
        case RTTI_SPECIAL:
        case RTTI_UNIT:
        case RTTI_UNITTYPE:
        case RTTI_VESSEL:
        case RTTI_VESSELTYPE:
            return COLUMN_RIGHT;

        default:
            DEBUG_LOG("Unhandled RTTIType in Which_Column() returning COLUMN_RIGHT, We should never reach here!");
            return COLUMN_RIGHT;
    }
}

BOOL SidebarClass::Factory_Link(int factory_id, RTTIType type, int id)
{
    return m_Columns[Which_Column(type)].Factory_Link(factory_id, type, id);
}

BOOL SidebarClass::Activate_Repair(int state)
{
    bool to_redraw = m_SidebarToRedraw;

    if (state == -1) {
        state = (m_SidebarBit4 == 0);
    }

    if (state == 1) {
        m_SidebarBit4 = true;
    } else {
        m_SidebarBit4 = false;
    }

    if (m_SidebarToRedraw != to_redraw) {
        Flag_To_Redraw();
        m_SidebarToRedraw = true;

        if (!m_SidebarBit4) {
            Help_Text(TXT_NONE);
            Set_Default_Mouse(MOUSE_POINTER);
        }
    }

    return to_redraw;
}

BOOL SidebarClass::Activate_Upgrade(int state) // Actually Sell?
{
    bool to_redraw = m_SidebarToRedraw;

    if (state == -1) {
        state = (m_SidebarBit8 == 0);
    }

    if (state == 1) {
        m_SidebarBit8 = true;
    } else {
        m_SidebarBit8 = false;
    }

    if (m_SidebarToRedraw != to_redraw) {
        Flag_To_Redraw();
        m_SidebarToRedraw = true;

        if (!m_SidebarBit8) {
            Set_Default_Mouse(MOUSE_POINTER);
        }
    }

    return to_redraw;
}

BOOL SidebarClass::Activate_Demolish(int state) // Actually Zoom?
{
    bool to_redraw = m_SidebarToRedraw;

    if (state == -1) {
        state = (m_SidebarBit16 == 0);
    }

    if (state == 1) {
        m_SidebarBit16 = true;
    } else {
        m_SidebarBit16 = false;
    }

    if (m_SidebarToRedraw != to_redraw) {
        Flag_To_Redraw();
        m_SidebarToRedraw = true;

        if (!m_SidebarBit16) {
            Set_Default_Mouse(MOUSE_POINTER);
        }
    }

    return to_redraw;
}

BOOL SidebarClass::Add(RTTIType item, int id)
{
    if (g_InMapEditor) {
        return false;
    }

    // get Left or Right strip and add the "item", based on return value of Which_Column().
    if (m_Columns[Which_Column(item)].Add(item, id)) {
        Activate(1);
        m_SidebarToRedraw = true;
        Flag_To_Redraw();

        return true;
    }

    return false;
}

BOOL SidebarClass::Scroll(BOOL reverse, int column)
{
    BOOL scroll_result;

    if (column == COLUMN_NONE) {
        scroll_result = (m_Columns[COLUMN_LEFT].Scroll(reverse) || m_Columns[COLUMN_RIGHT].Scroll(reverse));

        if (!scroll_result) {
            Sound_Effect(VOC_SCOLDY1);
        }

        if (!scroll_result) {
            return false;
        }

        m_SidebarToRedraw = true;
        Flag_To_Redraw();

        return true;
    }

    scroll_result = m_Columns[column].Scroll(reverse);

    if (!scroll_result) {
        return false;
    }

    m_SidebarToRedraw = true;
    Flag_To_Redraw();

    return true;
}

void SidebarClass::Recalc()
{
    BOOL recalculated = false;

    for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
        recalculated = m_Columns[column].Recalc();

        if (recalculated) {
            break;
        }
    }

    if (recalculated) {
        m_SidebarToRedraw = true;
        Flag_To_Redraw();
    }
}

BOOL SidebarClass::Activate(int mode)
{
    bool prevvalue = m_SidebarIsDrawn;

    if (!g_InMapEditor) {
        if (!Session.Playback_Game()) {
            m_SidebarIsDrawn = (mode == SIDEBAR_TOGGLE ? !m_SidebarIsDrawn : mode == SIDEBAR_ACTIVATE);

            bool newvalue = m_SidebarIsDrawn;

            if (newvalue != prevvalue) {
                if (newvalue) {
                    Set_View_Dimensions(0, 16, 20, -1);
                    m_SidebarToRedraw = true;
                    Help_Text(TXT_NULL, -1, -1);
                    RepairButton.Unlink();
                    Add_A_Button(RepairButton);
                    SellButton.Unlink();
                    Add_A_Button(SellButton);
                    ZoomButton.Unlink();
                    Add_A_Button(ZoomButton);

                    for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
                        m_Columns[column].Activate();
                    }

                    Background.Unlink();
                    Add_A_Button(Background);
                    RadarButton.Unlink();
                    Add_A_Button(RadarButton);
                    PowerButton.Unlink();
                    Add_A_Button(PowerButton);
                } else {
                    Set_View_Dimensions(0, 16, -1, -1);
                    Help_Text(TXT_NULL, -1, -1);
                    Remove_A_Button(RepairButton);
                    Remove_A_Button(SellButton);
                    Remove_A_Button(ZoomButton);

                    for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
                        m_Columns[column].Deactivate();
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
    return m_Columns[Which_Column(type)].Abandon_Production(unk2);
}

void SidebarClass::Zoom_Mode_Control()
{
    if (!m_RadarActive) {
        if (Session.Game_To_Play() != GAME_CAMPAIGN) {
            Player_Names(!m_RadarDrawNames);
        }
    } else if (m_RadarZoomed || Session.Game_To_Play() == GAME_CAMPAIGN) {
        if (m_RadarZoomed || !Spy_Next_House()) {
            Zoom_Mode(Coord_To_Cell(m_DisplayPos));
        }
    } else if (!m_RadarDrawNames && !m_RadarZoomed) {
        Player_Names(true);
    } else {
        Player_Names(false);

        if (!Spy_Next_House()) {
            Zoom_Mode(Coord_To_Cell(m_DisplayPos));
        }
    }
}
