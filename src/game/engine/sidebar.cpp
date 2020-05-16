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
#include "fetchtechtype.h"
#include "gamefile.h"
#include "globals.h"
#include "house.h"
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
void *SidebarClass::StripClass::s_LogoShapes = nullptr;
void *SidebarClass::StripClass::s_ClockShapes = nullptr;
void *SidebarClass::StripClass::s_SpecialShapes[SPECIAL_COUNT];
SidebarClass::StripClass::SelectClass SidebarClass::StripClass::s_SelectButton[COLUMN_COUNT][ROW_COUNT];
ShapeButtonClass SidebarClass::StripClass::s_UpButton[COLUMN_COUNT];
ShapeButtonClass SidebarClass::StripClass::s_DownButton[COLUMN_COUNT];
char SidebarClass::StripClass::s_ClockTranslucentTable[256][2];
SidebarClass::SBGadgetClass SidebarClass::s_Background;
ShapeButtonClass SidebarClass::s_RepairButton;
ShapeButtonClass SidebarClass::s_SellButton;
ShapeButtonClass SidebarClass::s_ZoomButton;
void *SidebarClass::s_SidebarShape = nullptr;
void *SidebarClass::s_SidebarMiddleShape = nullptr;
void *SidebarClass::s_SidebarBottomShape = nullptr;
#endif

void *SidebarClass::s_SidebarAddonShape = nullptr;

SidebarClass::SBGadgetClass::SBGadgetClass() : GadgetClass(496, 154, 143, 244, MOUSE_LEFT_UP, false) {}

BOOL SidebarClass::SBGadgetClass::Action(unsigned flags, KeyNumType &key)
{
    g_Map.Help_Text(TXT_NULL);
    g_Map.Override_Mouse_Shape(MOUSE_POINTER);
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

SidebarClass::StripClass::StripClass() : m_ProgressTimer() {}

SidebarClass::StripClass::StripClass(InitClass const &init) :
    m_XPos(0),
    m_YPos(0),
    m_WhichColumn(),
    m_StripToRedraw(true),
    m_Busy(false),
    m_ScrollingDown(false),
    m_Scrolling(false),
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
    static char *_special_icon_names[SPECIAL_COUNT] = { "sonr", "atom", "warp", "pbmb", "pinf", "cam", "infx", "gpss" };

    char icon_fname[16];
    s_ClockShapes = GameFileClass::Retrieve("clock.shp");

    for (SpecialWeaponType super = SPECIAL_FIRST; super < SPECIAL_COUNT; ++super) {
        snprintf(icon_fname, sizeof(icon_fname), "%.4sicon.shp", _special_icon_names[super]);
        s_SpecialShapes[super] = GameFileClass::Retrieve(icon_fname);
    }
}

void *SidebarClass::StripClass::Get_Special_Cameo(SpecialWeaponType super)
{
    if (super != SPECIAL_NONE && super < SPECIAL_COUNT) {
        return s_SpecialShapes[super];
    }
    return nullptr;
}

void SidebarClass::StripClass::Init_Clear()
{
    m_field_21 = -1;
    m_CurrentRow = 0;
    m_field_2D = 0;
    m_CameoCount = 0;
    m_Busy = false;
    m_ScrollingDown = false;
    m_Scrolling = false;

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

        s_UpButton[m_WhichColumn].Set_ID(column + GADGET_STRIP_COLUMN_LEFT);
        s_UpButton[m_WhichColumn].Set_XPos(m_XPos + 4);
        s_UpButton[m_WhichColumn].Set_YPos(m_YPos + 194);
        s_UpButton[m_WhichColumn].Set_Shape(GameFileClass::Retrieve("stripup.shp"));
        s_UpButton[m_WhichColumn].Set_Sticky(true);

        s_DownButton[m_WhichColumn].Set_ID(column + GADGET_STRIP_COLUMN_RIGHT);
        s_DownButton[m_WhichColumn].Set_XPos(m_XPos + 36);
        s_DownButton[m_WhichColumn].Set_YPos(m_YPos + 194);
        s_DownButton[m_WhichColumn].Set_Shape(GameFileClass::Retrieve("stripdn.shp"));
        s_DownButton[m_WhichColumn].Set_Sticky(true);

        for (int index = 0; index < ROW_COUNT; ++index) {
            s_SelectButton[m_WhichColumn][index].Set_ID(220);
            s_SelectButton[m_WhichColumn][index].Set_Size(64, 48);
            s_SelectButton[m_WhichColumn][index].Set_XPos(m_XPos);
            s_SelectButton[m_WhichColumn][index].Set_YPos(m_YPos + index * 48);
            s_SelectButton[m_WhichColumn][index].Set_Owner(*this, index);
        }
    }
}

void SidebarClass::StripClass::Init_Theater(TheaterType theater)
{
    static const TLucentType _clock_cols = { 3, 12, 100, 0 };

    PaletteClass palette;

    if (!g_InMapEditor) {
        Reload_LogoShapes();

        if (theater != THEATER_NONE && theater != g_LastTheater) {
            palette = g_OriginalPalette;

            // Block out 7 (21 / 3 = 7) entries of the temp palette as white (63, 63, 63 is white in 7bit vga pal)
            memset(&palette[32], 63, 21);

            // Build the fading tables for drawing the clock sweep overlay.
            Build_Translucent_Table(palette, &_clock_cols, 1, s_ClockTranslucentTable[0]);
            Conquer_Build_Fading_Table(g_GamePalette, s_ClockTranslucentTable[1], 12, 100);
        }
    }
}

void SidebarClass::StripClass::Reload_LogoShapes()
{
    static char *_strip_names[HOUSES_PLAYABLE_COUNT] = {
        "stripna.shp",
        "stripna.shp",
        "stripus.shp",
        "stripna.shp",
        "stripus.shp",
        "stripna.shp",
        "stripna.shp",
        "stripna.shp",
        "stripna.shp",
        "stripus.shp",
    };

    int house = HOUSES_SPAIN;
    if (g_PlayerPtr != nullptr) {
        house = g_PlayerPtr->Acts_Like();
    }
    s_LogoShapes = GameFileClass::Retrieve(_strip_names[house]);
}

void SidebarClass::StripClass::Activate()
{
    s_UpButton[m_WhichColumn].Unlink();
    s_DownButton[m_WhichColumn].Unlink();
    g_Map.Add_A_Button(s_UpButton[m_WhichColumn]);
    g_Map.Add_A_Button(s_DownButton[m_WhichColumn]);

    for (int index = 0; index < ROW_COUNT; ++index) {
        s_SelectButton[m_WhichColumn][index].Unlink();
        g_Map.Add_A_Button(s_SelectButton[m_WhichColumn][index]);
    }
}

void SidebarClass::StripClass::Deactivate()
{
    g_Map.Remove_A_Button(s_UpButton[m_WhichColumn]);
    g_Map.Remove_A_Button(s_DownButton[m_WhichColumn]);

    for (int index = 0; index < ROW_COUNT; ++index) {
        g_Map.Remove_A_Button(s_SelectButton[m_WhichColumn][index]);
    }
}

BOOL SidebarClass::StripClass::Add(RTTIType type, int id)
{
    captainslog_assert(type >= RTTI_NONE);
    captainslog_assert(type < RTTI_COUNT);

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

        if (!g_ScenarioInit && type != RTTI_SPECIAL) {
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
    g_Map.Flag_To_Redraw();
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
    if (g_InMapEditor || !m_CameoCount) {
        return false;
    }

    bool bool1 = false;
    bool redraw = false;

    for (int i = 0; i < m_CameoCount; ++i) {
        TechnoTypeClass *ttptr = Fetch_Techno_Type(m_Entries[i].Type, m_Entries[i].ID);

        if (ttptr != nullptr) {
            bool1 = ttptr->Who_Can_Build_Me(true, false, g_PlayerPtr->What_Type()) != nullptr;
        } else if (m_Entries[i].ID < SPECIAL_COUNT) {
            bool1 = g_PlayerPtr->Special_Weapons((SpecialWeaponType)m_Entries[i].ID).Is_Enabled();
        }

        if (!bool1) {
            if (m_CameoCount > 1 && i < m_CameoCount - 1) {
                memcpy(&m_Entries[i], &m_Entries[i + 1], sizeof(SelectButtonType) * (m_CameoCount - i - 1));
            }

            m_CurrentRow = 0;
            m_StripToRedraw = true;
            redraw = true;
            --m_CameoCount;
            --i;
        }
    }
    return redraw;
}

BOOL SidebarClass::StripClass::Factory_Link(int factory_id, RTTIType type, int id)
{
    for (int index = 0; index < m_CameoCount; ++index) {
        if (m_Entries[index].Type == type && m_Entries[index].ID == id) {
            m_Entries[index].Factory = factory_id;
            m_Busy = true;
            Flag_To_Redraw();

            return true;
        }
    }

    return false;
}

int SidebarClass::StripClass::Abandon_Production(int factory)
{
    bool abandon = false;
    bool bool1 = true;

    for (int i = 0; i < m_CameoCount; ++i) {
        if (m_Entries[i].Factory == factory) {
            g_Factories[factory].Abandon();

            m_Entries[i].Factory = -1;
            abandon = true;

        } else if (m_Entries[i].Factory != -1) {
            bool1 = false;
        }
    }

    if (abandon) {
        Flag_To_Redraw();
    }

    if (bool1) {
        m_Busy = false;
    }

    return abandon;
}

SidebarClass::SidebarClass() :
    PowerClass(),
    m_SidebarIsShown(false),
    m_SidebarToRedraw(true),
    m_RepairActive(false),
    m_UpgradeActive(false),
    m_DemolishActive(false)
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

    g_WindowList[WINDOW_SIDEBAR].X = 496;
    g_WindowList[WINDOW_SIDEBAR].Y = 180;
    g_WindowList[WINDOW_SIDEBAR].W = 160;
    g_WindowList[WINDOW_SIDEBAR].H = 192;

    for (int column = 0; column < COLUMN_COUNT; ++column) {
        m_Columns[column].One_Time(column);
    }

    if (s_SidebarShape == nullptr) {
        s_SidebarShape = GameFileClass::Retrieve("sidebar.shp");
    }

    // NOTE: Chronoshift hi-res sidebar addition.
    if (s_SidebarAddonShape == nullptr) {
        s_SidebarAddonShape = GameFileClass::Retrieve("addon.shp");
    }
}

void SidebarClass::Init_Clear()
{
    PowerClass::Init_Clear();
    m_SidebarToRedraw = true;
    if (!g_InMapEditor) {
        m_RepairActive = false;
        m_UpgradeActive = false;
        m_DemolishActive = false;

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
        s_RepairButton.Set_ID(BUTTON_REPAIR);
        s_RepairButton.Set_Position(498, 150);
        s_RepairButton.Set_Sticky(true);
        s_RepairButton.Set_Toggle_Bool1(false);
        s_RepairButton.Set_Toggle_Disabled(true);
        s_RepairButton.Set_Shape_Bool_One(true);
        s_RepairButton.Set_Shape(GameFileClass::Retrieve("repair.shp"));

        s_SellButton.Set_ID(BUTTON_SELL);
        s_SellButton.Set_Position(543, 150);
        s_SellButton.Set_Sticky(true);
        s_SellButton.Set_Toggle_Bool1(false);
        s_SellButton.Set_Toggle_Disabled(true);
        s_SellButton.Set_Shape_Bool_One(true);
        s_SellButton.Set_Shape(GameFileClass::Retrieve("sell.shp"));

        s_ZoomButton.Set_ID(BUTTON_ZOOM);
        s_ZoomButton.Set_Position(588, 150);
        s_ZoomButton.Set_Sticky(true);
        s_ZoomButton.Set_Toggle_Bool1(false);
        s_ZoomButton.Set_Shape(GameFileClass::Retrieve("map.shp"));

        if ((m_RadarActive && Is_Zoomable()) || g_Session.Game_To_Play() != GAME_CAMPAIGN) {
            s_ZoomButton.Enable();
        } else {
            s_ZoomButton.Disable();
        }

        for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
            m_Columns[column].Init_IO(column);
        }

        if (m_SidebarIsShown) {
            m_SidebarIsShown = false;
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
    if (!g_InMapEditor) {
        SidebarClass::Activate(1);
    }

    if (!g_InMapEditor) {
        m_Columns[COLUMN_LEFT].AI(key, mouse_x, mouse_y);
        m_Columns[COLUMN_RIGHT].AI(key, mouse_x, mouse_y);
    }

    if (m_SidebarIsShown) {
        SidebarClass::Activate_Repair(g_PlayerPtr->Get_BScan_Built() != 0);

        if (key == GADGET_BUTTON(BUTTON_REPAIR)) {
            Repair_Mode_Control(-1);
        }

        if (key == GADGET_BUTTON(BUTTON_SELL)) {
            Sell_Mode_Control(-1);
        }

        if (key == GADGET_BUTTON(BUTTON_ZOOM)) {
            Zoom_Mode_Control();
        }
    }

    if (!Repair_Mode() && s_RepairButton.Get_Toggle_State()) {
        s_RepairButton.Turn_Off();
    }

    if (!Sell_Mode() && s_SellButton.Get_Toggle_State()) {
        s_SellButton.Turn_Off();
    }

    PowerClass::AI(key, mouse_x, mouse_y);
}

void SidebarClass::Draw_It(BOOL force_redraw)
{
    PowerClass::Draw_It(force_redraw);

    if (!g_InMapEditor) {
        BENCHMARK_START(BENCH_SIDEBAR);

        if (m_SidebarIsShown && (m_SidebarToRedraw || force_redraw) && !g_InMapEditor) {
            m_SidebarToRedraw = false;
            if (g_LogicPage->Lock()) {
                if (s_SidebarShape != nullptr) {
                    CC_Draw_Shape(s_SidebarShape, 0, 480, 16, WINDOW_0, SHAPE_WIN_REL);
                }

                if (s_SidebarMiddleShape != nullptr) {
                    CC_Draw_Shape(s_SidebarMiddleShape,
                        force_redraw ? 0 : 1,
                        480,
                        176, // Height of tab + 160, height of the first sidebar piece.
                        WINDOW_0,
                        SHAPE_WIN_REL);
                }

                if (s_SidebarBottomShape != nullptr) {
                    CC_Draw_Shape(s_SidebarBottomShape,
                        force_redraw ? 0 : 1,
                        480,
                        276, // Height of tab + 160 + 100,, height of last piece added on.
                        WINDOW_0,
                        SHAPE_WIN_REL);
                }

                // Handles drawing extra bits for higher resolutions.
                if (g_LogicPage->Get_Height() > 400) {
                    if (s_SidebarAddonShape != nullptr) {
                        int addonheight = Get_Build_Frame_Height(s_SidebarAddonShape);
                        CC_Draw_Shape(s_SidebarAddonShape, 0, 480, 400, WINDOW_0, SHAPE_WIN_REL);
                    }
                }

                s_RepairButton.Draw_Me(true);
                s_SellButton.Draw_Me(true);
                s_ZoomButton.Draw_Me(true);

                /*for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
                    m_Columns[column].m_StripToRedraw = true;
                }*/

                g_LogicPage->Unlock();
            }
        }

        if (m_SidebarIsShown) {
            for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
                m_Columns[column].Draw_It(force_redraw);
            }

            if (m_SidebarToRedraw || force_redraw) {
                s_RepairButton.Draw_Me(true);
                s_SellButton.Draw_Me(true);
                s_ZoomButton.Draw_Me(true);
            }
        }

        m_SidebarToRedraw = false;

        BENCHMARK_END(BENCH_SIDEBAR);
    }
}

void SidebarClass::Refresh_Cells(cell_t cellnum, const int16_t *list)
{
    captainslog_assert(cellnum < MAP_MAX_AREA);
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
    // in order of the houses.
    static char _sidebar_names[HOUSES_COUNT][12] = { "side?na.shp",
        "side?na.shp",
        "side?us.shp",
        "side?na.shp",
        "side?us.shp",
        "side?na.shp",
        "side?na.shp",
        "side?na.shp",
        "side?na.shp",
        "side?us.shp" };

    int side_index = g_PlayerPtr != nullptr ? g_PlayerPtr->Acts_Like() : 0;

    char name[16];
    strcpy(name, _sidebar_names[side_index]);
    // this basicly replaces the '?' in the filenames above with a number.
    name[4] = '1';
    s_SidebarShape = GameFileClass::Retrieve(name);
    name[4] = '2';
    s_SidebarMiddleShape = GameFileClass::Retrieve(name);
    name[4] = '3';
    s_SidebarBottomShape = GameFileClass::Retrieve(name);

    // reload the side specific stip backgrounds.
    m_Columns[COLUMN_LEFT].Reload_LogoShapes();
    m_Columns[COLUMN_RIGHT].Reload_LogoShapes();
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
            captainslog_debug("Unhandled RTTIType in Which_Column() returning COLUMN_RIGHT, We should never reach here!");
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
        state = (m_RepairActive == 0);
    }

    if (state == 1) {
        m_RepairActive = true;
    } else {
        m_RepairActive = false;
    }

    if (m_SidebarToRedraw != to_redraw) {
        Flag_To_Redraw();
        m_SidebarToRedraw = true;

        if (!m_RepairActive) {
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
        state = (m_UpgradeActive == 0);
    }

    if (state == 1) {
        m_UpgradeActive = true;
    } else {
        m_UpgradeActive = false;
    }

    if (m_SidebarToRedraw != to_redraw) {
        Flag_To_Redraw();
        m_SidebarToRedraw = true;

        if (!m_UpgradeActive) {
            Set_Default_Mouse(MOUSE_POINTER);
        }
    }

    return to_redraw;
}

BOOL SidebarClass::Activate_Demolish(int state) // Actually Zoom?
{
    bool to_redraw = m_SidebarToRedraw;

    if (state == -1) {
        state = (m_DemolishActive == 0);
    }

    if (state == 1) {
        m_DemolishActive = true;
    } else {
        m_DemolishActive = false;
    }

    if (m_SidebarToRedraw != to_redraw) {
        Flag_To_Redraw();
        m_SidebarToRedraw = true;

        if (!m_DemolishActive) {
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
    bool prevvalue = m_SidebarIsShown;

    if (!g_InMapEditor) {
        if (!g_Session.Playback_Game()) {
            m_SidebarIsShown = (mode == SIDEBAR_TOGGLE ? !m_SidebarIsShown : mode == SIDEBAR_ACTIVATE);

            bool newvalue = m_SidebarIsShown;

            if (newvalue != prevvalue) {
                if (newvalue) {
                    Set_View_Dimensions(0, 16, 20, -1);
                    m_SidebarToRedraw = true;
                    Help_Text(TXT_NULL, -1, -1);
                    s_RepairButton.Unlink();
                    Add_A_Button(s_RepairButton);
                    s_SellButton.Unlink();
                    Add_A_Button(s_SellButton);
                    s_ZoomButton.Unlink();
                    Add_A_Button(s_ZoomButton);

                    for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
                        m_Columns[column].Activate();
                    }

                    s_Background.Unlink();
                    Add_A_Button(s_Background);
                    s_RadarButton.Unlink();
                    Add_A_Button(s_RadarButton);
                    s_PowerButton.Unlink();
                    Add_A_Button(s_PowerButton);
                } else {
                    Set_View_Dimensions(0, 16, -1, -1);
                    Help_Text(TXT_NULL, -1, -1);
                    Remove_A_Button(s_RepairButton);
                    Remove_A_Button(s_SellButton);
                    Remove_A_Button(s_ZoomButton);

                    for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
                        m_Columns[column].Deactivate();
                    }

                    Remove_A_Button(s_Background);
                    Remove_A_Button(s_RadarButton);
                    Remove_A_Button(s_PowerButton);
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
        if (g_Session.Game_To_Play() != GAME_CAMPAIGN) {
            Player_Names(!m_RadarDrawNames);
        }
    } else if (m_RadarZoomed || g_Session.Game_To_Play() == GAME_CAMPAIGN) {
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
