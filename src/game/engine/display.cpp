/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling tactical display (main action pane when in game).
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "display.h"
#include "aircraft.h"
#include "bench.h"
#include "building.h"
#include "buildingtype.h"
#include "buffpipe.h"
#include "coord.h"
#include "drawshape.h"
#include "fading.h"
#include "gamefile.h"
#include "gameoptions.h"
#include "gbuffer.h"
#include "globals.h"
#include "house.h"
#include "iomap.h"
#include "keyboard.h"
#include "lists.h"
#include "mixfile.h"
#include "mouse.h"
#include "palette.h"
#include "pk.h"
#include "rules.h"
#include "scenario.h"
#include "session.h"
#include "super.h"
#include "target.h"
#include "techno.h"
#include "technotype.h"
#include "textprint.h"
#include "theater.h"
#include "vortex.h"
#include <algorithm>
#include <cstdio>

using std::snprintf;

#ifndef GAME_DLL
LayerClass DisplayClass::s_Layers[LAYER_COUNT];
DisplayClass::TacticalClass DisplayClass::s_TacticalButton;
uint8_t DisplayClass::s_FadingBrighten[256];
uint8_t DisplayClass::s_FadingShade[256];
uint8_t DisplayClass::s_FadingWayDark[256];
uint8_t DisplayClass::s_FadingLight[256];
uint8_t DisplayClass::s_FadingGreen[256];
uint8_t DisplayClass::s_FadingYellow[256];
uint8_t DisplayClass::s_FadingRed[256];
uint8_t DisplayClass::s_TranslucentTable[14][256];
uint8_t DisplayClass::s_WhiteTranslucentTable[2][256];
uint8_t DisplayClass::s_MouseTranslucentTable[5][256];
uint8_t DisplayClass::s_ShadowTrans[5][256];
uint8_t DisplayClass::s_UnitShadow[2][256];
uint8_t DisplayClass::s_UnitShadowAir[2][256];
uint8_t DisplayClass::s_SpecialGhost[2][256];
void *DisplayClass::s_TransIconset = nullptr;
void *DisplayClass::s_ShadowShapes = nullptr;
BufferClass *DisplayClass::s_TheaterBuffer = nullptr;
BooleanVectorClass DisplayClass::s_CellRedraw;
#endif

char DisplayClass::s_FadingWhite[256];

#define SHADOW_CLEAR -1
#define SHADOW_FULL -2

DisplayClass::TacticalClass::TacticalClass() :
    GadgetClass(0, 0, 0, 0, MOUSE_LEFT_PRESS | MOUSE_LEFT_HELD | MOUSE_LEFT_RLSE | MOUSE_LEFT_UP | MOUSE_RIGHT_PRESS, true)
{
}

/**
 * Gadget action for handling input to the main tactical view.
 *
 * RA 0x004B3108
 */
BOOL DisplayClass::TacticalClass::Action(unsigned flags, KeyNumType &key)
{
    int x;
    int y;

    if (flags & (MOUSE_LEFT_RLSE | MOUSE_LEFT_PRESS | MOUSE_RIGHT_RLSE | MOUSE_RIGHT_PRESS)) {
        x = g_Keyboard->Get_MouseQX();
        y = g_Keyboard->Get_MouseQY();
    } else {
        x = g_Mouse->Get_Mouse_X();
        y = g_Mouse->Get_Mouse_Y();
    }

    bool at_edge = x == 0 || y == 0 || x == g_SeenBuff.Get_Width() - 1 || y == g_SeenBuff.Get_Height() - 1;
    coord_t coord = g_Map.Pixel_To_Coord(x, y);
    cell_t cell = Coord_To_Cell(coord);

    if (coord == 0) {
        return GadgetClass::Action(flags, key);
    }

    bool shrouded = !g_Map[cell].Is_Visible() && !g_Debug_Unshroud;
    int mouse_vpx = x - g_Map.Tac_Offset_X();
    int mouse_vpy = y - g_Map.Tac_Offset_Y();

    if (cell != g_Map.Get_Cursor_Start()) {
        g_Map.Set_Cursor_Pos(cell);
    }

    // Work out what object if any is near the cursor.
    TechnoClass *close_tech = nullptr;

    if (!shrouded) {
        ObjectClass *obj = g_Map.Close_Object(coord);

        if (obj != nullptr && obj->Is_Techno()) {
            close_tech = reinterpret_cast<TechnoClass *>(obj);

            if (!close_tech->Is_Player_Owned()
                && (close_tech->Cloak_State() == CLOAK_CLOAKED
                    || reinterpret_cast<const TechnoTypeClass &>(close_tech->Class_Of()).Is_Invisible())) {
                close_tech = nullptr;
            }
        }
    }

    ActionType action = ACTION_NONE;

    if (CurrentObjects.Count() > 0) {
        if (close_tech != nullptr) {
            action = CurrentObjects.Fetch_Head()->What_Action(close_tech);
        } else {
            action = CurrentObjects.Fetch_Head()->What_Action(cell);
        }
    } else {
        if (close_tech != nullptr && close_tech->Class_Of().Is_Selectable()) {
            action = ACTION_SELECT;
        }

        if (g_Map.Repair_Mode()) {
            if (close_tech != nullptr && close_tech->Owner() == g_PlayerPtr->What_Type() && close_tech->Can_Repair()) {
                action = ACTION_REPAIR;
            } else {
                action = ACTION_NO_REPAIR;
            }
        }

        if (g_Map.Sell_Mode()) {
            if (close_tech != nullptr && close_tech->Owner() == g_PlayerPtr->What_Type() && close_tech->Can_Demolish()) {
                if (close_tech->What_Am_I() == RTTI_BUILDING) {
                    action = ACTION_SELL;
                } else {
                    action = ACTION_SELL_UNIT;
                }
            } else {
                if (g_Map[cell].Get_Overlay() != OVERLAY_NONE
                    && OverlayTypeClass::As_Reference(g_Map[cell].Get_Overlay()).Is_Wall()
                    && g_Map[cell].Owner() == g_PlayerPtr->What_Type()) {
                    action = ACTION_SELL;
                } else {
                    action = ACTION_NO_SELL;
                }
            }
        }

        switch (g_Map.m_PendingSuper) {
            case SPECIAL_ATOM_BOMB:
                action = ACTION_NUKE;
                break;

            case SPECIAL_WARP_SPHERE:
                action = ACTION_CHRONOSPHERE;
                break;

            case SPECIAL_PARA_BOMB:
                action = ACTION_PARA_BOMB;
                break;

            case SPECIAL_PARA_INFANTRY:
                action = ACTION_PARA_SABOTEUR;
                break;

            case SPECIAL_SPY_PLANE:
                action = ACTION_SPY_MISSION;
                break;

            case SPECIAL_IRON_CURTAIN:
                action = ACTION_IRON_CURTAIN;
                break;

            case SPECIAL_WARP_TELEPORT: {
                action = ACTION_CHRONOWARP;

                if (shrouded) {
                    action = ACTION_NO_MOVE;
                }

                TechnoClass *shifter = reinterpret_cast<TechnoClass *>(As_Object(g_PlayerPtr->Chrono_Object()));

                if (shifter != nullptr && shifter->Is_Techno()) {
                    if (!shifter->Can_Teleport_Here(cell)) {
                        action = ACTION_NO_MOVE;
                    }
                } else {
                    action = ACTION_NO_MOVE;
                    g_Map.Set_Pending_Super(SPECIAL_NONE);
                }
            }

            break;

            case SPECIAL_SONAR_PULSE: // These don't need to set anything so fall through to default.
            case SPECIAL_GPS:
            default:
                break;
        }

        if (g_Map.Pending_ObjectType() != nullptr) {
            action = ACTION_NONE;
        }
    }

    if (cell != g_Map.Get_Cursor_Start()) {
        g_Map.Set_Cursor_Pos(cell);
    }

    if (flags & MOUSE_RIGHT_PRESS) {
        g_Map.Mouse_Right_Press();
    }

    if ((flags & MOUSE_LEFT_UP) && g_Map.Bit_8()) {
        flags |= MOUSE_LEFT_RLSE;
    }

    if (!at_edge && (flags & MOUSE_LEFT_UP)) {
        g_Map.Mouse_Left_Up(cell, shrouded, close_tech, action);
    }

    if (flags & MOUSE_LEFT_RLSE) {
        g_Map.Mouse_Left_Release(cell, mouse_vpx, mouse_vpy, close_tech, action);
    }

    if (!at_edge && (flags & MOUSE_LEFT_PRESS)) {
        g_Map.Mouse_Left_Up(cell, shrouded, close_tech, action);
        g_Map.Mouse_Left_Press(mouse_vpx, mouse_vpy);
    }

    if ((flags & MOUSE_LEFT_HELD) != 0) {
        g_Map.Mouse_Left_Held(mouse_vpx, mouse_vpy);
    }

    return GadgetClass::Action(flags, key);
}

DisplayClass::DisplayClass() :
    m_DisplayPos(0),
    m_DisplayWidth(0),
    m_DisplayHeight(0),
    m_DisplayCursorStart(0),
    m_DisplayCursorEnd(0),
    m_DisplayCursorOccupy(nullptr),
    m_PassedProximityCheck(false),
    m_PendingObjectPtr(nullptr),
    m_PendingObjectTypePtr(nullptr),
    m_PendingObjectOwner(HOUSES_NONE),
    m_TacOffsetX(0),
    m_TacOffsetY(0),
    m_DisplayNewPos(0),
    m_DisplayToRedraw(true),
    m_DisplayRepairMode(false),
    m_DisplaySellMode(false),
    m_DisplayBit8(false),
    m_DisplayBit16(false),
    m_RedrawShadow(false),
    m_PendingSuper(SPECIAL_NONE),
    m_BandBox(0, 0, 0, 0)
{
    s_ShadowShapes = nullptr;
    s_TransIconset = nullptr;
    Set_View_Dimensions(0, 8, 13, 8);
}

/**
 * @brief Clears initialised data from this layer.
 *
 * 0x004AEF7C
 */
void DisplayClass::Init_Clear()
{
    MapClass::Init_Clear();
    m_PendingObjectPtr = 0;
    m_PendingObjectTypePtr = 0;
    m_PendingObjectOwner = HOUSES_NONE;
    m_DisplayCursorOccupy = 0;
    m_PendingSuper = SPECIAL_NONE;
    m_DisplayToRedraw = true;
    m_DisplayRepairMode = false;
    m_DisplaySellMode = false;
    m_DisplayBit8 = false;
    m_DisplayBit16 = false;
    m_RedrawShadow = true;

    for (int i = 0; i < LAYER_COUNT; ++i) {
        s_Layers[i].Init();
    }
}

/**
 * @brief Initialise data that doesn't change once loaded during runtime.
 *
 * 0x004AEEF4
 */
void DisplayClass::One_Time()
{
    MapClass::One_Time();

    // Resize the cell redraw vector to cover the map.
    s_CellRedraw.Resize(MAP_MAX_AREA);

    for (LayerType layer = LAYER_FIRST; layer < LAYER_COUNT; ++layer) {
        s_Layers[layer].One_Time();
    }

    s_TransIconset = GameFileClass::Retrieve("trans.icn");
    DEBUG_ASSERT(s_TransIconset != nullptr);

    s_ShadowShapes = GameFileClass::Retrieve("shadow.shp");
    DEBUG_ASSERT(s_ShadowShapes != nullptr);

    Set_View_Dimensions(0, 16);
}

/**
 * @brief Initialises the UI gadgets to receive input for this layer.
 *
 * 0x004AEFF4
 */
void DisplayClass::Init_IO()
{
    MapClass::Init_IO();

    if (!g_InMapEditor) {
        s_TacticalButton.Zap();
        Add_A_Button(s_TacticalButton);
    }
}

/**
 * @brief Initialises for given theater. Changes mix file if needed.
 *
 * 0x004AF02C
 */
void DisplayClass::Init_Theater(TheaterType theater)
{
    static GameMixFile *_theater_data = nullptr;

    static const TLucentType _mouse_cols[] = {
        { 12, 12, 110, 0 }, { 15, 15, 110, 0 }, { 14, 14, 110, 0 }, { 13, 13, 110, 0 }
    };

    static const TLucentType _magic_cols[] = { { 32, 32, 110, 0 },
        { 33, 33, 110, 0 },
        { 34, 34, 110, 0 },
        { 35, 35, 110, 0 },
        { 36, 36, 110, 0 },
        { 37, 37, 110, 0 },
        { 38, 38, 110, 0 },
        { 39, 39, 110, 0 },
        { 12, 12, 200, 0 },
        { 15, 12, 40, 0 },
        { 14, 12, 80, 0 },
        { 13, 12, 140, 0 },
        { 4, 12, 130, 0 } };

    static const TLucentType _white_cols[] = { { 1, 15, 80, 0 } };

    static const TLucentType _shadow_cols[] = {
        { 16, 12, 130, 0 }, { 15, 12, 170, 0 }, { 14, 12, 250, 0 }, { 13, 12, 250, 0 }
    };

    static const TLucentType _ushadow_cols[] = { { 4, 12, 130, 0 } };

    static const TLucentType _ushadow_cols_air[] = { { 4, 15, 0, 0 } };

    static const TLucentType _ushadow_cols_snow[] = { { 4, 12, 75, 0 } };

    MapClass::Init_Theater(theater);
    g_Scen.Set_Theater(theater);

    char mix_name[32];
    snprintf(mix_name, sizeof(mix_name), "%s.mix", g_Theaters[theater].data);

    // Does the theater we want to initialise match the last one loaded? If so we don't need to do anything.
    if (g_Scen.Get_Theater() != g_LastTheater) {
        if (_theater_data != nullptr) {
            delete _theater_data;
        }

        _theater_data = new GameMixFile(mix_name, &g_PublicKey);
        _theater_data->Cache(s_TheaterBuffer);
    }

    char pal_name[32];
    snprintf(pal_name, sizeof(pal_name), "%s.pal", g_Theaters[theater].data);

    g_GamePalette = *static_cast<PaletteClass *>(GameFileClass::Retrieve(pal_name));
    g_OriginalPalette = g_GamePalette;

    Build_Fading_Table(g_GamePalette, s_FadingGreen, 3, 110);
    Build_Fading_Table(g_GamePalette, s_FadingYellow, 5, 140);
    Build_Fading_Table(g_GamePalette, s_FadingRed, 8, 140);
    Build_Fading_Table(g_GamePalette, s_FadingWhite, 15, 120);

    Build_Translucent_Table(g_GamePalette, _mouse_cols, ARRAY_SIZE(_mouse_cols), s_MouseTranslucentTable);
    Build_Translucent_Table(g_GamePalette, _magic_cols, ARRAY_SIZE(_magic_cols), s_TranslucentTable);
    Build_Translucent_Table(g_GamePalette, _white_cols, ARRAY_SIZE(_white_cols), s_WhiteTranslucentTable);
    Build_Translucent_Table(g_GamePalette, _shadow_cols, ARRAY_SIZE(_shadow_cols), s_ShadowTrans);

    Conquer_Build_Translucent_Table(g_GamePalette, _ushadow_cols_air, ARRAY_SIZE(_ushadow_cols_air), s_UnitShadowAir);

    if (theater == THEATER_SNOW) {
        Conquer_Build_Translucent_Table(g_GamePalette, _ushadow_cols_snow, ARRAY_SIZE(_ushadow_cols_snow), s_UnitShadow);
        Conquer_Build_Fading_Table(g_GamePalette, s_FadingShade, 12, 75);
    } else {
        Conquer_Build_Translucent_Table(g_GamePalette, _ushadow_cols, ARRAY_SIZE(_ushadow_cols), s_UnitShadow);
        Conquer_Build_Fading_Table(g_GamePalette, s_FadingShade, 12, 130);
    }

    Conquer_Build_Fading_Table(g_GamePalette, s_FadingLight, 15, 85);
    Conquer_Build_Fading_Table(g_GamePalette, s_SpecialGhost[1], 12, 100);
    memset(s_SpecialGhost, 0, 256);
    Make_Fading_Table(g_GamePalette, s_FadingBrighten, 15, 25);
    Make_Fading_Table(g_GamePalette, s_FadingWayDark, 13, 192);

    g_Options.Fixup_Palette();
}

/**
 * @brief Performs per tick logic for this layer.
 *
 * 0x004B0140
 */
void DisplayClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
    if (m_DisplayBit8) {
        if (g_Mouse->Get_Mouse_X() < m_TacOffsetX || g_Mouse->Get_Mouse_Y() < m_TacOffsetY
            || g_Mouse->Get_Mouse_X() >= CELL_PIXELS * m_DisplayHeight + 128 + m_TacOffsetX
            || g_Mouse->Get_Mouse_Y() >= CELL_PIXELS * m_DisplayWidth + 128 + m_TacOffsetY) {
            Mouse_Left_Release(-1, g_Mouse->Get_Mouse_X(), g_Mouse->Get_Mouse_Y());
        }
    }

    MapClass::AI(key, mouse_x, mouse_y);
}

void DisplayClass::Draw_It(BOOL force_redraw)
{
    if (m_DisplayToRedraw || force_redraw) {

        BENCHMARK_START(BENCH_TACTICAL);

        m_DisplayToRedraw = false;
        Refresh_Band();
        g_ChronalVortex.Set_Redraw();
        int msg_count = g_Session.Get_Messages().Num_Messages();

        // If we have messages to draw, redraw the relevant rows of cells
        if (msg_count > 0) {
            cell_t cell = Coord_To_Cell(m_DisplayPos);
            cell_t ecell = cell + Lepton_To_Cell_Coord(m_DisplayWidth) + 1;

            for (; cell < ecell; ++cell) {
                m_Array[cell].Redraw_Objects();
            }

            cell = Coord_To_Cell(m_DisplayPos) + MAP_MAX_WIDTH;
            ecell = cell + Lepton_To_Cell_Coord(m_DisplayWidth) + 1 + MAP_MAX_WIDTH;

            for (; cell < ecell; ++cell) {
                m_Array[cell].Redraw_Objects();
            }

            if (msg_count > 1) {
                cell = Coord_To_Cell(m_DisplayPos) + (MAP_MAX_WIDTH * 2);
                ecell = cell + Lepton_To_Cell_Coord(m_DisplayWidth) + 1 + (MAP_MAX_WIDTH * 2);

                for (; cell < ecell; ++cell) {
                    m_Array[cell].Redraw_Objects();
                }
            }

            if (msg_count > 2) {
                cell = Coord_To_Cell(m_DisplayPos) + (MAP_MAX_WIDTH * 3);
                ecell = cell + Lepton_To_Cell_Coord(m_DisplayWidth) + 1 + (MAP_MAX_WIDTH * 3);

                for (; cell < ecell; ++cell) {
                    m_Array[cell].Redraw_Objects();
                }
            }

            if (msg_count > 3) {
                cell = Coord_To_Cell(m_DisplayPos) + (MAP_MAX_WIDTH * 4);
                ecell = cell + Lepton_To_Cell_Coord(m_DisplayWidth) + 1 + (MAP_MAX_WIDTH * 4);

                for (; cell < ecell; ++cell) {
                    m_Array[cell].Redraw_Objects();
                }
            }
        }

        if (Lepton_To_Pixel(Coord_Lepton_X(m_DisplayNewPos)) == Lepton_To_Pixel(Coord_Lepton_X(m_DisplayPos))
            && Lepton_To_Pixel(Coord_Lepton_Y(m_DisplayNewPos)) == Lepton_To_Pixel(Coord_Lepton_Y(m_DisplayPos))) {
            ++g_ScenarioInit;

            if (m_DisplayNewPos != m_DisplayPos) {
                Set_Tactical_Position(m_DisplayNewPos);
            }

            --g_ScenarioInit;

        } else {
            int x_move_dist = Lepton_To_Pixel(Coord_Lepton_X(m_DisplayPos)) - Lepton_To_Pixel(Coord_Lepton_X(m_DisplayNewPos));
            int y_move_dist = Lepton_To_Pixel(Coord_Lepton_Y(m_DisplayPos)) - Lepton_To_Pixel(Coord_Lepton_Y(m_DisplayNewPos));
            int width_remain = Lepton_To_Pixel(m_DisplayWidth) - std::abs(x_move_dist);
            int height_remain = Lepton_To_Pixel(m_DisplayHeight) - std::abs(y_move_dist);

            // Check if we'v moved more than 1 screen away and so need to redraw
            // everything.
            if (width_remain < 1 || height_remain < 1) {
                force_redraw = true;
            }

            bool move_right = x_move_dist < 0;
            bool move_left = x_move_dist > 0;
            bool move_down = y_move_dist < 0;
            bool move_up = y_move_dist > 0;

            // If we haven't moved at all redraw?
            if (force_redraw
                || (Lepton_To_Pixel(m_DisplayWidth) == width_remain && Lepton_To_Pixel(m_DisplayHeight) == height_remain)) {
                force_redraw = true;
            } else {
                // Do a block blit of anything still in view.
                Set_Cursor_Pos(-1);

                if (g_HidPage.In_Video_Memory()) {
                    g_Mouse->Hide_Mouse();

                    int src_x = m_TacOffsetX + -std::min(0, x_move_dist);
                    int src_y = -std::min(0, y_move_dist) + m_TacOffsetY;
                    int dst_x = std::max(0, x_move_dist) + m_TacOffsetX;
                    int dst_y = m_TacOffsetY + std::max(0, y_move_dist);

                    g_SeenBuff.Blit(g_HidPage, src_x, src_y, dst_x, dst_y, width_remain, height_remain);
                    g_Mouse->Show_Mouse();
                } else {
                    int src_x = m_TacOffsetX + -std::min(0, x_move_dist);
                    int src_y = -std::min(0, y_move_dist) + m_TacOffsetY;
                    int dst_x = std::max(0, x_move_dist) + m_TacOffsetX;
                    int dst_y = m_TacOffsetY + std::max(0, y_move_dist);

                    g_HidPage.Blit(g_HidPage, src_x, src_y, dst_x, dst_y, width_remain, height_remain);
                }
            }

            x_move_dist = std::max(0, x_move_dist);
            y_move_dist = std::max(0, y_move_dist);

            ++g_ScenarioInit;
            Set_Tactical_Position(m_DisplayNewPos);
            --g_ScenarioInit;

            // If we aren't forcing a complete redraw, then redraw around the edges of the moved section.
            if (!force_redraw) {
                width_remain -= 24;
                height_remain -= 24;

                if (std::abs(x_move_dist) >= 37 || std::abs(y_move_dist) >= 37) {
                    int x_pixel = -Lepton_To_Pixel(Coord_Sub_Cell_X(m_DisplayPos));
                    int y_pixel = -Lepton_To_Pixel(Coord_Sub_Cell_Y(m_DisplayPos));
                    width_remain -= CELL_PIXELS;
                    height_remain -= CELL_PIXELS;

                    for (int j = y_pixel; Lepton_To_Pixel(m_DisplayHeight) + CELL_PIXELS * 2 >= j; j += CELL_PIXELS) {
                        for (int k = x_pixel; Lepton_To_Pixel(m_DisplayWidth) + CELL_PIXELS * 2 >= k; k += CELL_PIXELS) {
                            if (k <= x_move_dist || width_remain + x_move_dist <= k || j <= y_move_dist
                                || height_remain + y_move_dist <= j) {
                                int y_check = std::clamp(j, 0, Lepton_To_Pixel(m_DisplayHeight) - 1);
                                int x_check = std::clamp(k, 0, Lepton_To_Pixel(m_DisplayWidth) - 1);
                                cell_t click_cell = Click_Cell_Calc(x_check + m_TacOffsetX, y_check + m_TacOffsetY);

                                if (click_cell > 0) {
                                    m_Array[click_cell].Redraw_Objects(true);
                                }
                            }
                        }
                    }

                } else {
                    int x_move_fudge = std::abs(x_move_dist) < 16 ? 1 : 2;
                    int y_move_fudge = std::abs(y_move_dist) < 16 ? 1 : 2;

                    int x_pixel2 = -Lepton_To_Pixel(Coord_Sub_Cell_X(m_DisplayPos));
                    int y_pixel2 = -Lepton_To_Pixel(Coord_Sub_Cell_Y(m_DisplayPos));

                    if (move_up) {
                        for (int j = y_pixel2; y_pixel2 + CELL_PIXELS * y_move_fudge >= j; j += CELL_PIXELS) {
                            for (int k = x_pixel2; Lepton_To_Pixel(m_DisplayWidth) + CELL_PIXELS * 2 >= k; k += CELL_PIXELS) {
                                int y_check = std::clamp(j, 0, Lepton_To_Pixel(m_DisplayHeight) - 1);
                                int x_check = std::clamp(k, 0, Lepton_To_Pixel(m_DisplayWidth) - 1);
                                cell_t click_cell = Click_Cell_Calc(x_check + m_TacOffsetX, y_check + m_TacOffsetY);

                                if (click_cell > 0) {
                                    m_Array[click_cell].Redraw_Objects(true);
                                }
                            }
                        }
                    }

                    if (move_down) {
                        for (int j = Lepton_To_Pixel(m_DisplayHeight) - CELL_PIXELS * y_move_fudge;
                             Lepton_To_Pixel(m_DisplayHeight) + 72 >= j;
                             j += CELL_PIXELS) {
                            for (int k = x_pixel2; Lepton_To_Pixel(m_DisplayWidth) + CELL_PIXELS * 2 >= k; k += CELL_PIXELS) {
                                int y_check = std::clamp(j, 0, Lepton_To_Pixel(m_DisplayHeight) - 1);
                                int x_check = std::clamp(k, 0, Lepton_To_Pixel(m_DisplayWidth) - 1);
                                cell_t click_cell = Click_Cell_Calc(x_check + m_TacOffsetX, y_check + m_TacOffsetY);

                                if (click_cell > 0) {
                                    m_Array[click_cell].Redraw_Objects(true);
                                }
                            }
                        }
                    }

                    if (move_left) {
                        for (int k = x_pixel2; x_pixel2 + CELL_PIXELS * x_move_fudge >= k; k += CELL_PIXELS) {
                            for (int j = y_pixel2; Lepton_To_Pixel(m_DisplayHeight) + CELL_PIXELS * 2 >= j; j += CELL_PIXELS) {
                                int y_check = std::clamp(j, 0, Lepton_To_Pixel(m_DisplayHeight) - 1);
                                int x_check = std::clamp(k, 0, Lepton_To_Pixel(m_DisplayWidth) - 1);
                                cell_t click_cell = Click_Cell_Calc(x_check + m_TacOffsetX, y_check + m_TacOffsetY);

                                if (click_cell > 0) {
                                    m_Array[click_cell].Redraw_Objects(true);
                                }
                            }
                        }
                    }

                    if (move_right) {
                        for (int k = Lepton_To_Pixel(m_DisplayWidth) - x_move_fudge * CELL_PIXELS;
                             Lepton_To_Pixel(m_DisplayWidth) + 72 >= k;
                             k += CELL_PIXELS) {
                            for (int j = y_pixel2; Lepton_To_Pixel(m_DisplayHeight) + CELL_PIXELS * 2 >= j; j += CELL_PIXELS) {
                                int y_check = std::clamp(j, 0, Lepton_To_Pixel(m_DisplayHeight) - 1);
                                int x_check = std::clamp(k, 0, Lepton_To_Pixel(m_DisplayWidth) - 1);
                                cell_t click_cell = Click_Cell_Calc(x_check + m_TacOffsetX, y_check + m_TacOffsetY);

                                if (click_cell > 0) {
                                    m_Array[click_cell].Redraw_Objects(true);
                                }
                            }
                        }
                    }
                }
            }
        }

        if (force_redraw) {
            s_CellRedraw.Set();
        }

        if (g_HidPage.Lock()) {
            Redraw_Icons();
            g_HidPage.Unlock();
        }

        if (g_HidPage.Lock()) {
            g_ChronalVortex.Render();

            BENCHMARK_START(BENCH_OBJECTS);
            for (LayerType layer = LAYER_FIRST; layer < LAYER_COUNT; ++layer) {
                s_Layers[layer].Render_All(force_redraw);
            }

            // SS does some drawing of flagfly.shp here after rendering surface
            // objects.
            BENCHMARK_END(BENCH_OBJECTS);

            BENCHMARK_START(BENCH_SHROUD);
            Redraw_Shadow();
            BENCHMARK_END(BENCH_SHROUD);

            g_HidPage.Unlock();
        }

        if (m_DisplayBit8) {
            g_LogicPage->Draw_Rect(m_TacOffsetX + m_BandBox.Get_Left(),
                m_TacOffsetY + m_BandBox.Get_Top(),
                m_TacOffsetX + m_BandBox.Get_Right(),
                m_TacOffsetY + m_BandBox.Get_Bottom(),
                15);
        }

        s_CellRedraw.Reset();

        BENCHMARK_END(BENCH_TACTICAL);
    }
}

void DisplayClass::Code_Pointers()
{
    if (m_PendingObjectPtr != nullptr) {
        m_PendingObjectPtr = reinterpret_cast<ObjectClass *>((uintptr_t)m_PendingObjectPtr->As_Target());
    }

    MapClass::Code_Pointers();
}

void DisplayClass::Decode_Pointers()
{
    if ((uintptr_t)m_PendingObjectPtr != 0) {
        m_PendingObjectPtr = As_Object((uintptr_t)m_PendingObjectPtr);
    }

    MapClass::Decode_Pointers();
}

void DisplayClass::Read_INI(GameINIClass &ini)
{
    // Needs all type classes, celltriggerclass
#ifdef GAME_DLL
    void (*func)(const DisplayClass *, GameINIClass &) =
        reinterpret_cast<void (*)(const DisplayClass *, GameINIClass &)>(0x004B50F8);
    func(this, ini);
#endif
}

/*void DisplayClass::Write_INI(GameINIClass &ini)
{
    // Needs celltriggerclass
#ifdef GAME_DLL
    void (*func)(const DisplayClass *, GameINIClass &) =
        reinterpret_cast<void (*)(const DisplayClass *, GameINIClass &)>(0x004B545C);
    func(this, ini);
#endif
}*/

/**
 * Marks a cell as being visited by the given house, provided the house is spied by, allied with or is the player.
 */
BOOL DisplayClass::Map_Cell(cell_t cellnum, HouseClass *house)
{
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);

    // Spied radar reveals enemy movement logic
    if (house != nullptr && !house->Is_Player()) {
        if (house->Spied_My_Radar(g_PlayerPtr) || (g_Session.Game_To_Play() == GAME_CAMPAIGN && house->Is_Ally(g_PlayerPtr))) {
            house = g_PlayerPtr;
        }
    }

    if (!house->Is_Player() || !In_Radar(cellnum)) {
        return false;
    }

    CellClass &cell = g_Map[cellnum];

    if (cell.Is_Visible()) {
        if (!cell.Is_Revealed()) {
            cell.Redraw_Objects();
        }

        return false;
    }

    cell.Set_Visible(true);
    cell.Redraw_Objects();

    if (Cell_Shadow(cellnum) == SHADOW_CLEAR) {
        cell.Set_Revealed(true);
    }

    for (FacingType facing = FACING_FIRST; facing < FACING_COUNT; ++facing) {
        cell_t adjacent = Cell_Get_Adjacent(cellnum, facing);
        CellClass &adj_cell = g_Map[adjacent];

        adj_cell.Redraw_Objects();

        if (adjacent != cellnum && !adj_cell.Is_Revealed()) {
            int shadow = Cell_Shadow(adjacent);

            if (shadow == SHADOW_CLEAR) {
                if (adj_cell.Is_Visible()) {
                    adj_cell.Set_Revealed(true);
                } else {
                    Map_Cell(adjacent, house);
                }

            } else if (shadow != SHADOW_FULL && !adj_cell.Is_Visible()) {
                Map_Cell(adjacent, house);
            }
        }
    }

    TechnoClass *tptr = cell.Cell_Techno();

    if (tptr != nullptr) {
        tptr->Revealed(house);
    }

    return true;
}

cell_t DisplayClass::Click_Cell_Calc(int x, int y) const
{
    int xpos = (lepton_t)Pixel_To_Lepton(x - m_TacOffsetX);
    int ypos = (lepton_t)Pixel_To_Lepton(y - m_TacOffsetY);

    if (xpos < m_DisplayWidth && ypos < m_DisplayHeight) {
        return Coord_To_Cell(Coord_From_Lepton_XY(xpos + Lepton_Round_To_Pixel(Coord_Lepton_X(m_DisplayPos)),
            ypos + Lepton_Round_To_Pixel(Coord_Lepton_Y(m_DisplayPos))));
    }

    return -1;
}

/**
 * @brief Scrolls the map position in a given direction for a given distance in leptons.
 *
 * 0x004B03B4
 */
BOOL DisplayClass::Scroll_Map(DirType dir, int &distance, BOOL redraw)
{
    if (distance > 0) {
        // Check if our facing is toward an edge and compare current pos to see if we are at that edge. Modify dir
        // accordingly.
        FacingType face = Direction_To_Facing(dir);

        if (Coord_Cell_To_Lepton(m_MapCellX) == Coord_Lepton_X(m_DisplayPos) && face != FACING_WEST) {
            if (face == FACING_SOUTH_WEST) {
                dir = DIR_SOUTH;
            }

            if (face == FACING_NORTH_WEST) {
                dir = DIR_NORTH;
            }
        }

        if (Coord_Cell_To_Lepton(m_MapCellY) == Coord_Lepton_Y(m_DisplayPos) && face != FACING_NORTH) {
            if (face == FACING_NORTH_WEST) {
                dir = DIR_WEST;
            }

            if (face == FACING_NORTH_EAST) {
                dir = DIR_EAST;
            }
        }

        int16_t view_edge = m_DisplayWidth + Coord_Lepton_X(m_DisplayPos);

        if (view_edge == Coord_Cell_To_Lepton(m_MapCellWidth + m_MapCellX) && face != FACING_EAST) {
            if (face == FACING_NORTH_EAST) {
                dir = DIR_NORTH;
            }

            if (face == FACING_SOUTH_EAST) {
                dir = DIR_SOUTH;
            }
        }

        view_edge = m_DisplayHeight + Coord_Lepton_Y(m_DisplayPos);

        if (Coord_Cell_To_Lepton(m_MapCellHeight + m_MapCellY) == view_edge && face != FACING_SOUTH) {
            if (face == FACING_SOUTH_EAST) {
                dir = DIR_EAST;
            }

            if (face == FACING_SOUTH_WEST) {
                dir = DIR_WEST;
            }
        }

        coord_t new_pos = Coord_Move(m_DisplayPos, dir, distance);
        int x_pos = Coord_Lepton_X(new_pos) - Coord_Cell_To_Lepton(m_MapCellX);
        int y_pos = Coord_Lepton_Y(new_pos) - Coord_Cell_To_Lepton(m_MapCellY);

        bool confined = Confine_Rect(x_pos,
            y_pos,
            m_DisplayWidth,
            m_DisplayHeight,
            Coord_Cell_To_Lepton(m_MapCellWidth),
            Coord_Cell_To_Lepton(m_MapCellHeight));

        if (x_pos < 0) {
            x_pos = 0;
            confined = true;
        }

        if (y_pos < 0) {
            y_pos = 0;
            confined = true;
        }

        coord_t pos_coord =
            Coord_From_Lepton_XY(x_pos + Coord_Cell_To_Lepton(m_MapCellX), y_pos + Coord_Cell_To_Lepton(m_MapCellY));

        // Update distance if the move was constrained.
        if (confined) {
            distance = Distance(m_DisplayPos, pos_coord);
        }

        // If we actually moved, redraw.
        if (distance != 0 && pos_coord != m_DisplayPos) {
            if (redraw) {
                Set_Tactical_Position(pos_coord);
                m_DisplayToRedraw = true;
                Flag_To_Redraw();
                s_Layers[LAYER_TOP].Mark_All(MARK_REDRAW);
                s_Layers[LAYER_AIR].Mark_All(MARK_REDRAW);
            }

            return true;
        }
    }

    return false;
}

/**
 * @brief Refreshes cells around a target cell using an overlap list.
 *
 * 0x004B0628
 */
void DisplayClass::Refresh_Cells(cell_t cellnum, const int16_t *list)
{
    int16_t tmp_list[60];

    if (list != nullptr) {
        if (*list == LIST_START) {
            ++list;
        }

        List_Copy(tmp_list, list, ARRAY_SIZE(tmp_list));

        for (int i = 0; i < ARRAY_SIZE(tmp_list); ++i) {
            if (tmp_list[i] == LIST_END) {
                break;
            }

            cell_t refresh_cell = cellnum + tmp_list[i];

            if (In_Radar(refresh_cell)) {
                m_Array[refresh_cell].Redraw_Objects();
            }
        }
    }
}

/**
 * @brief Sets the dimensions of the tactical view in pixels.
 *
 * 0x004AF4E0
 */
void DisplayClass::Set_View_Dimensions(int x, int y, int w, int h)
{
    if (w == -1) {
        m_DisplayWidth = Pixel_To_Lepton(g_SeenBuff.Get_Width() - x);
    } else {
        m_DisplayWidth = w * 256;
    }

    if (h == -1) {
        m_DisplayHeight = Pixel_To_Lepton(g_SeenBuff.Get_Height() - y);
    } else {
        m_DisplayHeight = h * 256;
    }

    int x_pos = Coord_Lepton_X(m_DisplayPos) - (m_MapCellX * 256);
    int y_pos = Coord_Lepton_Y(m_DisplayPos) - (m_MapCellY * 256);

    Confine_Rect(x_pos, y_pos, m_DisplayWidth, m_DisplayHeight, m_MapCellWidth * 256, m_MapCellHeight * 256);

    coord_t tacpos_coord = Coord_From_Lepton_XY(x_pos + (m_MapCellX * 256), y_pos + (m_MapCellY * 256));

    Set_Tactical_Position(tacpos_coord);
    m_TacOffsetX = x;
    m_TacOffsetY = y;

    g_WindowList[WINDOW_TACTICAL].W = Lepton_To_Pixel(m_DisplayWidth);
    g_WindowList[WINDOW_TACTICAL].H = Lepton_To_Pixel(m_DisplayHeight);
    g_WindowList[WINDOW_TACTICAL].X = x;
    g_WindowList[WINDOW_TACTICAL].Y = y;

    if (g_Window == WINDOW_TACTICAL) {
        Change_Window(WINDOW_0);
        Change_Window(WINDOW_TACTICAL);
    }

    m_DisplayToRedraw = true;
    Flag_To_Redraw();

    s_TacticalButton.Set_XPos(m_TacOffsetX);
    s_TacticalButton.Set_YPos(m_TacOffsetY);
    s_TacticalButton.Set_Width(Lepton_To_Pixel(m_DisplayWidth));
    s_TacticalButton.Set_Height(Lepton_To_Pixel(m_DisplayHeight));
}

/**
 * @brief Sets the position the tactical view top corner to a given coord.
 *
 * 0x004B4860
 */
void DisplayClass::Set_Tactical_Position(coord_t location)
{
    // TODO This function will probably need adjustment to handle tactical view bigger than map for small maps.
    int x_pos = Coord_Lepton_X(location) - Coord_Cell_To_Lepton(m_MapCellX);
    int y_pos = Coord_Lepton_Y(location) - Coord_Cell_To_Lepton(m_MapCellY);

    // Ensures our x and y positions are within the map
    Confine_Rect(
        x_pos, y_pos, m_DisplayWidth, m_DisplayHeight, Coord_Cell_To_Lepton(m_MapCellWidth), Coord_Cell_To_Lepton(m_MapCellHeight));

    int16_t y_coord = Coord_Cell_To_Lepton(m_MapCellY) + y_pos;
    int16_t x_coord = Coord_Cell_To_Lepton(m_MapCellX) + x_pos;

    m_DisplayNewPos = Coord_From_Lepton_XY(x_coord, y_coord);

    // Update the display position if needed
    if (g_ScenarioInit) {
        m_DisplayPos = Coord_From_Lepton_XY(x_coord, y_coord);
    }

    m_DisplayToRedraw = true;
    Flag_To_Redraw();
}

/**
 * @brief Flags a specific cell to redraw.
 *
 * 0x004B5908
 */
void DisplayClass::Flag_Cell(cell_t cellnum)
{
    s_CellRedraw[cellnum] = true;
    m_DisplayToRedraw = true;
    Flag_To_Redraw();
}

void DisplayClass::Mouse_Right_Press()
{
    if (m_PendingObjectPtr != nullptr && m_PendingObjectPtr->Is_Techno()) {
        m_PendingObjectPtr = nullptr;
        m_PendingObjectTypePtr = nullptr;
        m_PendingObjectOwner = HOUSES_NONE;
        Set_Cursor_Shape();
    } else if (m_DisplayRepairMode) {
        m_DisplayRepairMode = false;
    } else if (m_DisplaySellMode) {
        m_DisplaySellMode = false;
    } else if (m_PendingSuper == SPECIAL_NONE) {
        Unselect_All();
    } else {
        m_PendingSuper = SPECIAL_NONE;
    }

    Set_Default_Mouse(MOUSE_POINTER, g_Map.Mouse_In_Radar());
}

void DisplayClass::Mouse_Left_Press(int mouse_x, int mouse_y)
{
    if (!m_DisplayRepairMode && !m_DisplaySellMode && m_PendingSuper == SPECIAL_NONE && m_PendingObjectTypePtr == nullptr) {
        m_BandBox.m_left = mouse_x;
        m_BandBox.m_top = mouse_y;
        m_BandBox.m_right = mouse_x;
        m_BandBox.m_bottom = mouse_y;
        m_DisplayBit16 = true;
    }
}

/**
 * Handles mouse when left button is unpressed.
 *
 * RA 0x004B3780
 */
void DisplayClass::Mouse_Left_Up(
    cell_t cellnum, BOOL cell_shrouded, ObjectClass *object, ActionType action, BOOL mouse_in_radar)
{
    target_t target = 0;

    m_DisplayBit16 = false;

    if (object != nullptr) {
        target = As_Target(object);
    } else if (cellnum != -1) {
        target = As_Target(cellnum);
    }

    if (cell_shrouded) {
        switch (action) {
            case ACTION_NONE:
                Set_Default_Mouse(MOUSE_POINTER, mouse_in_radar);
                break;

            case ACTION_MOVE:
            case ACTION_ENTER:
            case ACTION_SELF:
            case ACTION_ATTACK:
            case ACTION_HARVEST:
            case ACTION_SELECT:
            case ACTION_TOGGLE_SELECT:
            case ACTION_CAPTURE:
            case ACTION_SABOTAGE:
                Set_Default_Mouse(MOUSE_MOVE, mouse_in_radar);
                break;

            case ACTION_ION_CANNON:
                Set_Default_Mouse(MOUSE_MOVE, mouse_in_radar);
                break;

            case ACTION_NO_MOVE:
                if (CurrentObjects.Count() == 0 || CurrentObjects.Fetch_Head()->What_Am_I() != RTTI_AIRCRAFT) {
                    Set_Default_Mouse(MOUSE_MOVE, mouse_in_radar);
                } else {
                    Set_Default_Mouse(MOUSE_CANT_MOVE, mouse_in_radar); 
                }
                break;

            case ACTION_REPAIR:
                Set_Default_Mouse(MOUSE_CANT_REPAIR, mouse_in_radar);
                break;

            case ACTION_NO_REPAIR:
                Set_Default_Mouse(MOUSE_CANT_REPAIR, mouse_in_radar);
                break;

            case ACTION_SELL:
            case ACTION_SELL_UNIT:
            case ACTION_NO_SELL:
                Set_Default_Mouse(MOUSE_CANT_SELL, mouse_in_radar);
                break;

            case ACTION_PARA_BOMB:
            case ACTION_PARA_SABOTEUR:
            case ACTION_AIR_STRIKE:
            case ACTION_IRON_CURTAIN:
            case ACTION_SPY_MISSION:
                Set_Default_Mouse(MOUSE_AIR_STRIKE, mouse_in_radar);
                break;

            case ACTION_NUKE:
                Set_Default_Mouse(MOUSE_NUKE, mouse_in_radar);
                break;

            case ACTION_CHRONOSPHERE:
                Set_Default_Mouse(MOUSE_CHRONOSPHERE, mouse_in_radar);
                break;

            case ACTION_CHRONOWARP:
                Set_Default_Mouse(MOUSE_CHRONOWARP, mouse_in_radar);
                break;

            case ACTION_GUARD_AREA:
                Set_Default_Mouse(MOUSE_GUARD_AREA, mouse_in_radar);
                break;

            case ACTION_HEAL:
                Set_Default_Mouse(MOUSE_HEAL, mouse_in_radar);
                break;

            case ACTION_DAMAGE:
                Set_Default_Mouse(MOUSE_POINTER, mouse_in_radar);
                break;

            case ACTION_GREPAIR:
                Set_Default_Mouse(MOUSE_POINTER, mouse_in_radar);
                break;

            case ACTION_NO_DEPLOY:
                Set_Default_Mouse(MOUSE_CANT_DEPLOY, mouse_in_radar);
                break;

            case ACTION_NO_ENTER:
                Set_Default_Mouse(MOUSE_CANT_ENTER, mouse_in_radar);
                break;

            case ACTION_NO_GREPAIR:
                Set_Default_Mouse(MOUSE_CANT_GREPAIR, mouse_in_radar);
                break;

            default:
                Set_Default_Mouse(MOUSE_MOVE, mouse_in_radar);
                break;
        }

    } else {
        switch (action) {
            default:
            case ACTION_NONE:
                Set_Default_Mouse(MOUSE_POINTER, mouse_in_radar);
                break;

            case ACTION_NO_DEPLOY:
                Set_Default_Mouse(MOUSE_CANT_DEPLOY, mouse_in_radar);
                break;

            case ACTION_NO_ENTER:
                Set_Default_Mouse(MOUSE_CANT_ENTER, mouse_in_radar);
                break;

            case ACTION_NO_GREPAIR:
                Set_Default_Mouse(MOUSE_CANT_GREPAIR, mouse_in_radar);
                break;

            case ACTION_NO_REPAIR:
                Set_Default_Mouse(MOUSE_CANT_REPAIR, mouse_in_radar);
                break;

            case ACTION_NO_SELL:
                Set_Default_Mouse(MOUSE_CANT_SELL, mouse_in_radar);
                break;

            case ACTION_PARA_BOMB:
            case ACTION_PARA_SABOTEUR:
            case ACTION_AIR_STRIKE:
            case ACTION_IRON_CURTAIN:
            case ACTION_SPY_MISSION:
                Set_Default_Mouse(MOUSE_AIR_STRIKE, mouse_in_radar);
                break;

            case ACTION_MOVE:
                Set_Default_Mouse(MOUSE_MOVE, mouse_in_radar);
                break;

            case ACTION_DAMAGE:
                Set_Default_Mouse(MOUSE_DAMAGE, mouse_in_radar);
                break;

            case ACTION_GREPAIR:
                Set_Default_Mouse(MOUSE_GREPAIR, mouse_in_radar);
                break;

            case ACTION_SELECT:
            case ACTION_TOGGLE_SELECT:
                Set_Default_Mouse(MOUSE_SELECT, mouse_in_radar);
                break;

            case ACTION_GUARD_AREA:
                Set_Default_Mouse(MOUSE_GUARD_AREA, mouse_in_radar);
                break;

            case ACTION_ATTACK:
                if (Target_Legal(target)) {
                    if (CurrentObjects.Count() == 1) {
                        if (CurrentObjects.Fetch_Head()->Is_Techno() && reinterpret_cast<TechnoClass *>(CurrentObjects.Fetch_Head())->In_Range(target)) {
                            Set_Default_Mouse(MOUSE_ATTACK2, mouse_in_radar);
                            break;
                        }
                    }
                }
                Set_Default_Mouse(MOUSE_ATTACK, mouse_in_radar);
                break;

            case ACTION_HARVEST:
                Set_Default_Mouse(MOUSE_ATTACK, mouse_in_radar);
                break;

            case ACTION_SABOTAGE:
                Set_Default_Mouse(MOUSE_SABOTAGE, mouse_in_radar);
                break;

            case ACTION_ENTER:
            case ACTION_CAPTURE:
                Set_Default_Mouse(MOUSE_ENTER, mouse_in_radar);
                break;

            case ACTION_NO_MOVE:
                Set_Default_Mouse(MOUSE_CANT_MOVE, mouse_in_radar);
                break;

            case ACTION_SELF:
                Set_Default_Mouse(MOUSE_DEPLOY, mouse_in_radar);
                break;

            case ACTION_REPAIR:
                Set_Default_Mouse(MOUSE_REPAIR, mouse_in_radar);
                break;

            case ACTION_SELL_UNIT:
                Set_Default_Mouse(MOUSE_SELL_UNIT, mouse_in_radar);
                break;

            case ACTION_SELL:
                Set_Default_Mouse(MOUSE_SELL, mouse_in_radar);
                break;

            case ACTION_NUKE:
                Set_Default_Mouse(MOUSE_NUKE, mouse_in_radar);
                break;

            case ACTION_CHRONOSPHERE:
                Set_Default_Mouse(MOUSE_CHRONOSPHERE, mouse_in_radar);
                break;

            case ACTION_CHRONOWARP:
                Set_Default_Mouse(MOUSE_CHRONOWARP, mouse_in_radar);
                break;

            case ACTION_HEAL:
                Set_Default_Mouse(MOUSE_HEAL, mouse_in_radar);
                break;
        };
    }

    if (!mouse_in_radar) {
        int text = TXT_NULL;
        uint8_t color = COLOR_TBLACK;

        if (cell_shrouded) {
            color = COLOR_YELLOW;
            text = TXT_SHADOW;
        } else if (object != nullptr) {
            if (g_PlayerPtr->Is_Ally(object)) {
                color = COLOR_GREEN;
            } else if (object->Owner() != HOUSES_NONE && object->Owner() != HOUSES_NEUTRAL) {
                color = COLOR_PINK;
            } else {
                color = COLOR_LTGREY;
            }

            text = object->Full_Name();

            if (object->Is_Techno()) {
                TechnoClass *tptr = reinterpret_cast<TechnoClass *>(object);

                if (!reinterpret_cast<const TechnoTypeClass &>(tptr->Class_Of()).Is_Nominal()) {
                    if (!tptr->Get_Owner_House()->Is_Ally(g_PlayerPtr)) {
                        switch (tptr->What_Am_I()) {
                            case RTTI_BUILDING:
                                text = TXT_ENEMY_STRUCTURE;
                                break;

                            case RTTI_INFANTRY:
                                text = TXT_ENEMY_SOLDIER;
                                break;

                            // BUGFIX, seems RA doesnt handle aircraft or vessels? They display full name?
                            case RTTI_UNIT:
                            case RTTI_VESSEL:
                            case RTTI_AIRCRAFT:
                                text = TXT_ENEMY_VEHICLE;
                                break;

                            default:
                                break;
                        };
                    }
                }
            }

            if (object->What_Am_I() == RTTI_TERRAIN) {
                color = COLOR_LTGREY;
            }
        } else {
            CellClass &cell = m_Array[cellnum];

            if (cell.Get_Land() == LAND_ORE) {
                color = COLOR_YELLOW;
                text = TXT_VALUABLE_MINERALS;
            }
        }

        Help_Text(text, -1, -1, color);
    }
}

void DisplayClass::Mouse_Left_Held(int mouse_x, int mouse_y)
{
    if (m_DisplayBit8) {
        if (mouse_x != m_BandBox.m_right || mouse_y != m_BandBox.m_bottom) {
            Refresh_Band();
            // Need confirming, check between sole and ra!
            m_BandBox.m_right = std::clamp(mouse_x, 0, Lepton_To_Pixel(m_DisplayWidth) - 1);
            m_BandBox.m_bottom = std::clamp(mouse_y, 0, Lepton_To_Pixel(m_DisplayHeight) - 1);
            m_DisplayToRedraw = true;
            Flag_To_Redraw();
        }
    } else if (m_DisplayBit16) {
        // Ensure we actually drew a bandbox of usable size?
        if (std::abs(m_BandBox.m_left - mouse_x) > 4 || std::abs(m_BandBox.m_top - mouse_y) > 4) {
            m_DisplayBit8 = true;
            m_BandBox.m_right = std::clamp(mouse_x, 0, Lepton_To_Pixel(m_DisplayWidth) - 1);
            m_BandBox.m_bottom = std::clamp(mouse_y, 0, Lepton_To_Pixel(m_DisplayHeight) - 1);
            m_DisplayToRedraw = true;
            Flag_To_Redraw();

            s_Layers[LAYER_TOP].Mark_All(MARK_REDRAW);
            s_Layers[LAYER_AIR].Mark_All(MARK_REDRAW);
        }
    }
}

void DisplayClass::Mouse_Left_Release(
    cell_t cellnum, int mouse_x, int mouse_y, ObjectClass *object, ActionType action, BOOL mouse_in_radar)
{
#ifdef GAME_DLL
    void (*func)(const DisplayClass *, cell_t, int, int, ObjectClass *, ActionType, BOOL) =
        reinterpret_cast<void (*)(const DisplayClass *, cell_t, int, int, ObjectClass *, ActionType, BOOL)>(0x004B3CA8);
    func(this, cellnum, mouse_x, mouse_y, object, action, mouse_in_radar);
#endif
}

void DisplayClass::Write_INI(GameINIClass &ini)
{
    // Needs celltriggerclass
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(const DisplayClass *, GameINIClass &) =
        reinterpret_cast<void (*)(const DisplayClass *, GameINIClass &)>(0x004B545C);
    func(this, ini);
#else
    static const char *_map_section_name = "Map";
    static const char *_waypoint_section_name = "Waypoints";
    static const char *_mappack_section_name = "MapPack";

    ini.Clear(_map_section_name);

    ini.Put_TheaterType(_map_section_name, "Theater", g_Scen.Get_Theater());
    ini.Put_Int(_map_section_name, "X", m_MapCellX);
    ini.Put_Int(_map_section_name, "Y", m_MapCellY);
    ini.Put_Int(_map_section_name, "Width", m_MapCellWidth);
    ini.Put_Int(_map_section_name, "Height", m_MapCellHeight);

    char buffer[16];
    for ( int i = 0; i < WAYPOINT_COUNT; ++i ) {
        cell_t wpcell = g_Scen.Get_Waypoint(i);
        if ( wpcell != -1 ) {
            std::snprintf(buffer, sizeof(buffer), "%d", i);
            ini.Put_Int(_waypoint_section_name, buffer, wpcell);
        }
    }

    ini.Clear(_mappack_section_name);

    BufferPipe buffpipe(_staging_buffer, sizeof(_staging_buffer));
    if ( Write_Binary(buffpipe) ) {
        ini.Put_UUBlock(_mappack_section_name, _staging_buffer, sizeof(_staging_buffer));
    }
#endif
}

/**
 * @brief Converts a pixel on screen to the logical location in the engine.
 *
 * 0x004B2694
 */
coord_t DisplayClass::Pixel_To_Coord(int x, int y)
{
    int x_pos = Pixel_To_Lepton(x - m_TacOffsetX);
    int y_pos = Pixel_To_Lepton(y - m_TacOffsetY);

    if (x_pos < m_DisplayWidth && y_pos < m_DisplayHeight) {
        return Coord_From_Lepton_XY(x_pos + Coord_Lepton_X(m_DisplayPos), y_pos + Coord_Lepton_Y(m_DisplayPos));
    }

    return 0;
}

/**
 * @brief Sets the occupy list for the cells the band box currently covers.
 * @note Parsing a value of NULL will clear the cursor shape.
 *
 * 0x004AF700
 */
void DisplayClass::Set_Cursor_Shape(int16_t *list)
{
    static int16_t tmp_list[60] = { 0 };

    if (m_DisplayCursorOccupy != nullptr) {
        Cursor_Mark(m_DisplayCursorStart + m_DisplayCursorEnd, false);
    }

    m_DisplayCursorEnd = 0;

    if (list != nullptr) {
        int x;
        int y;

        List_Copy(tmp_list, list, ARRAY_SIZE(tmp_list));
        m_DisplayCursorOccupy = tmp_list;
        Get_Occupy_Dimensions(x, y, tmp_list);
        m_DisplayCursorEnd = -Cell_From_XY(x / 2, y / 2);
        Cursor_Mark(m_DisplayCursorStart + m_DisplayCursorEnd, true);
    } else {
        m_DisplayCursorOccupy = nullptr;
    }
}

/**
 * @brief Draw the selection band box.
 *
 * 0x004B2E84
 */
void DisplayClass::Refresh_Band()
{
    if (m_DisplayBit8) {
        int box_x = m_BandBox.m_left + m_TacOffsetX;
        int box_y = m_BandBox.m_top + m_TacOffsetY;
        int box_w = m_BandBox.m_right + m_TacOffsetX;
        int box_h = m_BandBox.m_bottom + m_TacOffsetY;

        if (box_x > box_w) {
            std::swap(box_x, box_w);
        }

        if (box_y > box_h) {
            std::swap(box_y, box_h);
        }

        // This loop should redraw the left and right side of the selection band.
        for (int i = box_y; i <= (box_h + CELL_PIXELS); i += CELL_PIXELS) {
            int b_pxl = Lepton_To_Pixel(m_DisplayHeight) + m_TacOffsetY;
            cell_t click_cell = Click_Cell_Calc(box_x, std::clamp(i, 0, b_pxl));

            if (click_cell != -1) {
                m_Array[click_cell].Redraw_Objects();
            }

            click_cell = Click_Cell_Calc(box_w, std::clamp(i, 0, b_pxl));

            if (click_cell != -1) {
                m_Array[click_cell].Redraw_Objects();
            }
        }

        // This loop should redraw the top and bottom side of the selection band.
        for (int i = box_x; i < (box_w + CELL_PIXELS); i += CELL_PIXELS) {
            int b_pxl = Lepton_To_Pixel(m_DisplayWidth) + m_TacOffsetX;
            cell_t click_cell = Click_Cell_Calc(std::clamp(i, 0, b_pxl), box_y);

            if (click_cell != -1) {
                m_Array[click_cell].Redraw_Objects();
            }

            click_cell = Click_Cell_Calc(std::clamp(i, 0, b_pxl), box_h);

            if (click_cell != -1) {
                m_Array[click_cell].Redraw_Objects();
            }
        }

        s_Layers[LAYER_TOP].Mark_All(MARK_REDRAW);
        s_Layers[LAYER_AIR].Mark_All(MARK_REDRAW);
    }
}

void DisplayClass::Cursor_Mark(cell_t cellnum, BOOL flag)
{
    // Check we have a valid cellnum
    if (cellnum == -1) {
        return;
    }

    for (int16_t *offsets = m_DisplayCursorOccupy; *offsets != LIST_END; ++offsets) {
        cell_t offset_cell = cellnum + *offsets;

        if (MapClass::In_Radar(offset_cell)) {
            CellClass &cell_ref = g_Map[offset_cell];
            cell_ref.Redraw_Objects();
            cell_ref.Set_Placement_Check(flag);
        }
    }

    if (m_PendingObjectPtr != nullptr) {
        for (const int16_t *overlap = m_PendingObjectPtr->Overlap_List(); *overlap != LIST_END; ++overlap) {
            int16_t offset_cell = cellnum + *overlap;

            if (MapClass::In_Radar(offset_cell)) {
                g_Map[offset_cell].Redraw_Objects();
            }
        }
    }
}

void DisplayClass::Get_Occupy_Dimensions(int &w, int &h, int16_t *list) const
{
    int left = -128;
    int top = -128;
    int right = 128;
    int bottom = 128;

    w = 0;
    h = 0;

    // Original has code to calculate dimensions from an occupy list, but the code checks if the pointer is null before doing
    // it, so it always just sets w and h to 0 instead. After debugging RA and changing this to a "cell != nullptr" check, I
    // noticed no real difference, i suppose we need to find out what is done with the returned dimensions.

#if 1 // This is original logic. Bad juju as it will dereference a null pointer if ever passed null.
    if (list == nullptr) {
#else // This is what it probably should have been.
    if (list != nullptr) {
#endif
        while (*list != LIST_END) {
            int cell_x = Cell_Get_X(*list);
            int cell_y = Cell_Get_Y(*list);

            left = std::max(left, cell_x);
            right = std::min(right, cell_x);
            top = std::max(top, cell_y);
            bottom = std::min(bottom, cell_y);
            ++list;
        }

        w = std::max(1, left - right + 1);
        h = std::min(1, top - bottom + 1);
    }
}

/**
 * @brief Creates an overlap list of the cells that will be obscured by text at a given position.
 *
 * 0x004AF2D8
 */
const int16_t *DisplayClass::Text_Overlap_List(const char *string, int x, int y) const
{
    static int16_t _list[60];
    int16_t *w_list = _list;

    if (string != nullptr) {
        int count = 60;
        int str_width = String_Pixel_Width(string) + CELL_PIXELS;
        int width = m_TacOffsetX + Lepton_To_Pixel(m_DisplayWidth);

        if (str_width + x >= width) {
            str_width = width - x;
            *w_list = LIST_START;
            --count;
            ++w_list;
        }

        if (width >= x) {
            cell_t click_cell = Click_Cell_Calc(x, y - 1);
            int height = std::clamp(y + 24, m_TacOffsetY, m_TacOffsetY + (Lepton_To_Pixel(m_DisplayHeight) - 1));
            cell_t offset_click = Click_Cell_Calc(str_width + x - 1, height);

            if (click_cell == -1) {
                click_cell = Click_Cell_Calc(x, y);
            }

            if (click_cell != -1 && offset_click != -1) {
                // If we have two valid cells locations, iterate through the cells between them.
                for (int i = Cell_Get_Y(click_cell); i <= Cell_Get_Y(offset_click); ++i) {
                    for (int j = Cell_Get_X(click_cell); j <= Cell_Get_X(offset_click); ++j) {
                        cell_t current_cell = Coord_To_Cell(m_DisplayPos);
                        *w_list = Cell_From_XY(j, i) - current_cell;
                        --count;
                        ++w_list;

                        // Break if there is only one slot left
                        if (count < 2) {
                            break;
                        }
                    }

                    if (count < 2) {
                        break;
                    }
                }
            }
        }

        *w_list = LIST_END;
    }

    return _list;
}

/**
 * Finds the previous object in the ground layer.
 */
ObjectClass *DisplayClass::Prev_Object(ObjectClass *object) const
{
    ObjectClass *retval = nullptr;
    bool found = false;

    // Flag to return the first object encountered when iterating the list.
    if (object == nullptr) {
        found = true;
    }

    // Scan backwards through the list of units until we find the one that
    // matches the passed pointer, then return the object before it in the list
    // in the next loop.
    for (int index = s_Layers[LAYER_GROUND].Count() - 1; index >= 0; --index) {
        ObjectClass *prev = s_Layers[LAYER_GROUND][index];

        if (prev != nullptr && prev->Is_Players_Army()) {
            if (retval == nullptr) {
                retval = prev;
            }

            if (found) {
                return prev;
            }

            if (object == prev) {
                found = true;
            }
        }
    }

    return retval;
}

/**
 * Finds the next object in the ground layer.
 */
ObjectClass *DisplayClass::Next_Object(ObjectClass *object) const
{
    ObjectClass *retval = nullptr;
    bool found = false;

    // Flag to return the first object encountered when iterating the list.
    if (object == nullptr) {
        found = true;
    }

    LayerClass *layerptr = &s_Layers[LAYER_GROUND];

    for (int index = 0; index < s_Layers[LAYER_GROUND].Count(); ++index) {
        ObjectClass *next = s_Layers[LAYER_GROUND][index];

        if (next != nullptr && next->Is_Players_Army()) {
            if (retval == nullptr) {
                retval = next;
            }

            if (found) {
                return next;
            }

            if (object == next) {
                found = true;
            }
        }
    }

    return retval;
}

void DisplayClass::Submit(ObjectClass *object, LayerType layer)
{
    if (object != nullptr) {
        s_Layers[layer].Submit(object, layer == LAYER_GROUND);
    }
}

void DisplayClass::Remove(ObjectClass *object, LayerType layer)
{
    if (object != nullptr) {
        s_Layers[layer].Remove(object);
    }
}

ObjectClass *DisplayClass::Cell_Object(cell_t cellnum, int x, int y) const
{
    return m_Array[cellnum].Cell_Object(x, y);
}

/**
 * Selects all objects in the rectangle formed between the two coords.
 *
 * RA 0x004B2C50
 */
void DisplayClass::Select_These(coord_t start, coord_t finish)
{
    // Calc X and Y start and end points for select rectangle
    // based on input coords.
    lepton_t x1 = Coord_Lepton_X(start) + Coord_Lepton_X(m_DisplayPos);
    lepton_t y1 = Coord_Lepton_Y(start) + Coord_Lepton_Y(m_DisplayPos);
    lepton_t x2 = Coord_Lepton_X(finish) + Coord_Lepton_X(m_DisplayPos);
    lepton_t y2 = Coord_Lepton_Y(finish) + Coord_Lepton_Y(m_DisplayPos);

    if (x1 > x2) {
        std::swap(x1, x2);
    }

    if (y1 > y2) {
        std::swap(y1, y2);
    }

    // TODO, add keyboard shortcut to "add to" selection by skipping this?
    Unselect_All();

    g_AllowVoice = true;

    // Loops through all ground units and select them if they fall
    // within the rectangle.
    for (int index = 0; index < s_Layers[LAYER_GROUND].Count(); ++index) {
        TechnoClass *objptr = reinterpret_cast<TechnoClass *>(s_Layers[LAYER_GROUND][index]);

        if (objptr != nullptr) {
            coord_t obj_loc = objptr->Center_Coord();
            lepton_t obj_x = Coord_Lepton_X(obj_loc);
            lepton_t obj_y = Coord_Lepton_Y(obj_loc);

            if (obj_x > x1 && obj_x < x2 && obj_y > y1 && obj_y < y2) {
                HouseClass *owner = HouseClass::As_Pointer(objptr->Owner());

                if (owner != nullptr && owner->Player_Has_Control()) {
                    switch (objptr->What_Am_I()) {
                        case RTTI_UNIT: // fallthrough
                        case RTTI_INFANTRY: // fallthrough
                        case RTTI_VESSEL: // fallthrough
                        case RTTI_AIRCRAFT:
                            if (objptr->Class_Of().Is_Selectable()) {
                                if (objptr->Select()) {
                                    g_AllowVoice = false;
                                }
                            }

                            break;
                        case RTTI_BUILDING: // fallthrough
                        default:
                            break;
                    };
                }
            }
        }
    }

    // This loops all Aircraft and selects those in rect that
    // weren't caught in the ground layer select irrespective
    // of their current layer.
    for (int index = 0; index < g_Aircraft.Count(); ++index) {
        AircraftClass &aptr = g_Aircraft[index];
        coord_t obj_loc = aptr.Center_Coord();
        lepton_t obj_x = Coord_Lepton_X(obj_loc);
        lepton_t obj_y = Coord_Lepton_Y(obj_loc);

        if (obj_x > x1 && obj_x < x2 && obj_y > y1 && obj_y < y2) {
            if (aptr.Get_Owner_House() != nullptr) {
                if (aptr.Class_Of().Is_Selectable()) {
                    if (aptr.Select()) {
                        g_AllowVoice = false;
                    }
                }
            }
        }
    }

    g_AllowVoice = true;
}

/**
 * Toggles the sell mode state.
 *
 * RA 0x004B4B68
 */
void DisplayClass::Sell_Mode_Control(int mode)
{
    bool flag = m_DisplaySellMode;

    switch (mode) {
        case MODE_CONTROL_TOGGLE:
            flag = !m_DisplaySellMode;
            break;

        case MODE_CONTROL_OFF:
            flag = false;
            break;

        case MODE_CONTROL_ON:
            flag = true;
            break;

        default:
            break;
    }

    if (flag != m_DisplaySellMode && m_PendingObjectTypePtr == nullptr) {
        m_DisplayRepairMode = false;

        if (flag && g_PlayerPtr->Has_Buildings()) {
            m_DisplaySellMode = true;
            Unselect_All();
        } else {
            m_DisplaySellMode = false;
            Revert_Mouse_Shape();
        }
    }
}

/**
 * Toggles the repair mode state.
 *
 * RA 0x004B4C10
 */
void DisplayClass::Repair_Mode_Control(int mode)
{
    bool flag = m_DisplayRepairMode;

    switch (mode) {
        case MODE_CONTROL_TOGGLE:
            flag = !m_DisplayRepairMode;
            break;

        case MODE_CONTROL_OFF:
            flag = false;
            break;

        case MODE_CONTROL_ON:
            flag = true;
            break;

        default:
            break;
    }

    if (flag != m_DisplayRepairMode && m_PendingObjectTypePtr == nullptr) {
        m_DisplaySellMode = false;

        if (flag && g_PlayerPtr->Has_Buildings()) {
            m_DisplayRepairMode = true;
            Unselect_All();
        } else {
            m_DisplayRepairMode = false;
            Revert_Mouse_Shape();
        }
    }
}

/**
 * @brief Centers display to given coord.
 *
 * 0x004B4E20
 */
void DisplayClass::Center_Map(coord_t coord)
{
    bool coord_set = false;
    int x = 0;
    int y = 0;

    // Calculate a position based on units selected, used if no coord is passed.
    if (CurrentObjects.Count() > 0) {
        for (int i = 0; i < CurrentObjects.Count(); ++i) {
            coord_t center = CurrentObjects[i]->Center_Coord();
            x += Coord_Lepton_X(center);
            y += Coord_Lepton_Y(center);
        }

        x /= CurrentObjects.Count();
        y /= CurrentObjects.Count();
        coord_set = true;
    }

    // SS version doesn't take a coord, just calculates based on units selected.
    if (coord != 0) {
        x = Coord_Lepton_X(coord);
        y = Coord_Lepton_Y(coord);
        coord_set = true;
    }

    // Only bother setting the position if we have a coord from somewhere. SS doesn't do the min X/Y checks
    if (coord_set) {
        int16_t x_lep = std::max((int16_t)(x - m_DisplayWidth / 2), Coord_Cell_To_Lepton(m_MapCellX));
        int16_t y_lep = std::max((int16_t)(y - m_DisplayHeight / 2), Coord_Cell_To_Lepton(m_MapCellY));
        Set_Tactical_Position(Coord_From_Lepton_XY(x_lep, y_lep));
    }
}

/**
 * @brief Registers the cell in the band box cursor list, calculates from mouse pos if cell is -1.
 *
 * 0x004AFD40
 */
cell_t DisplayClass::Set_Cursor_Pos(cell_t cell)
{
    cell_t retval;

    if (cell == -1) {
        cell = Click_Cell_Calc(g_Mouse->Get_Mouse_X(), g_Mouse->Get_Mouse_Y());
    }

    if (m_DisplayCursorOccupy != nullptr) {
        int occupy_x;
        int occupy_y;

        Get_Occupy_Dimensions(occupy_x, occupy_y, m_DisplayCursorOccupy);

        int cell_x = std::max(Cell_Get_X(m_DisplayCursorEnd + cell), Coord_Cell_X(m_DisplayPos));
        int cell_y = std::max(Cell_Get_Y(m_DisplayCursorEnd + cell), Coord_Cell_Y(m_DisplayPos));

        if (cell_x + occupy_x >= Coord_Lepton_X(m_DisplayPos) + Lepton_To_Cell_Coord(m_DisplayWidth)) {
            cell_x = Coord_Lepton_X(m_DisplayPos) + cell_x - occupy_x;
        }

        if (cell_y + occupy_y >= Coord_Lepton_Y(m_DisplayPos) + Lepton_To_Cell_Coord(m_DisplayHeight)) {
            cell_y = Coord_Lepton_Y(m_DisplayPos) + cell_y - occupy_y;
        }

        retval = Cell_From_XY(cell_x, cell_y) - m_DisplayCursorEnd;

        if (retval != m_DisplayCursorStart) {
            if (m_DisplayCursorOccupy != nullptr) {
                if (retval != m_DisplayCursorStart && m_DisplayCursorStart != -1) {
                    Cursor_Mark(m_DisplayCursorStart + m_DisplayCursorEnd, false);
                }

                if (retval != -1) {
                    Cursor_Mark(retval + m_DisplayCursorEnd, true);
                }
            }

            m_PassedProximityCheck = Passes_Proximity_Check(
                m_PendingObjectTypePtr, m_PendingObjectOwner, m_DisplayCursorOccupy, m_DisplayCursorEnd + retval);

            std::swap(m_DisplayCursorStart, retval);
        }
    } else {
        retval = m_DisplayCursorStart;
        m_DisplayCursorStart = cell;
    }

    return retval;
}

/**
 * Helper function handling the proximity check part.
 */
void DisplayClass::Check_Proximity(ObjectTypeClass *object, HousesType house, cell_t cell, int &passes, bool &outside_radar) const
{
    BuildingTypeClass *bld = reinterpret_cast<BuildingTypeClass *>(object);

    if (!In_Radar(cell)) {
        outside_radar = true;
        passes = 0;
        return;
    }

    // check adjacent cells in all directions until we meet the condition to stop
    for (FacingType i = FACING_NORTH; i < FACING_COUNT; ++i) {
        cell_t adj_cell = Cell_Get_Adjacent(cell, i);
        if (In_Radar(adj_cell)) {
            // is the adjacent cell a Wall or Bib owned by this house?
            if (bld->Is_Wall() || m_Array[adj_cell].Has_Bib()) {
                if (m_Array[adj_cell].Owner() == house) {
                    passes = 1;
                    return;
                }
            }

            // is the adjacent cell a building owned by this house and is BaseNormal?
            BuildingClass *tptr = (BuildingClass *)m_Array[adj_cell].Cell_Techno();
            if (tptr != nullptr && tptr->What_Am_I() == RTTI_BUILDING && tptr->Get_Owner_House()->What_Type() == house) {
                if (tptr->Class_Of().Base_Normal()) {
                    passes = 1;
                    return;
                }
            }

            // proximity hasn't passed yet so check all neighboring cells in all directions until we meet the condition to
            // stop
            for (FacingType j = FACING_NORTH; j < FACING_COUNT; ++j) {
                cell_t next_cell = Cell_Get_Adjacent(adj_cell, j);
                // is the cell next to the adjacent cell a Wall or Bib owned by this house?
                if (bld->Is_Wall() || m_Array[next_cell].Has_Bib()) {
                    if (m_Array[next_cell].Owner() == house) {
                        passes = 1;
                        return;
                    }
                }
                // is the cell next to the adjacent cell a building owned by this house and is BaseNormal?
                BuildingClass *tptr2 = (BuildingClass *)m_Array[next_cell].Cell_Techno();
                if (tptr2 != nullptr && tptr2->What_Am_I() == RTTI_BUILDING
                    && tptr2->Get_Owner_House()->What_Type() == house) {
                    if (tptr2->Class_Of().Base_Normal()) {
                        passes = 1;
                        return;
                    }
                }
            }
        }
    }
}

/**
 * Check if an object is clear to be placed at a given location, mainly for checking buildings can be placed.
 *
 * RA 0x004AF7DC
 */
BOOL DisplayClass::Passes_Proximity_Check(ObjectTypeClass *object, HousesType house, int16_t *list, cell_t cell) const
{
    int passes = -1;
    bool outside_radar = false;
    static bool _passed_proximity; // Appears to have been global originally, but only used in this function.

    if (g_PlayerPtr->What_Type() == house) {
        _passed_proximity = false;
    }

    if (g_InMapEditor) {
        return true;
    }

    if (list == nullptr || cell == 0) {
        return true;
    }

    if (object == nullptr || object->What_Am_I() != RTTI_BUILDINGTYPE) {
        return true;
    }

    cell_t cell_num = cell;
    if (reinterpret_cast<BuildingTypeClass *>(object)->Adjacency() == 1) {
        for (; *list != LIST_END && passes == -1; ++list) {
            cell_num = *list + cell;
            Check_Proximity(object, house, cell_num, passes, outside_radar);
        }
    }
    if (passes == -1) {
        passes = 0;
    }

    if (g_PlayerPtr->What_Type() == house) {
        _passed_proximity = (passes != 0);
    }

    if (!passes && !outside_radar && object->What_Am_I() == RTTI_BUILDINGTYPE
        && reinterpret_cast<BuildingTypeClass *>(object)->Adjacency() > 1) {
        for (int i = 0; i < g_Buildings.Count(); ++i) {
            BuildingClass *bptr = &g_Buildings[i];
            if (bptr != nullptr && !bptr->In_Limbo() && bptr->Get_Owner_House()->What_Type() == house
                && bptr->Class_Of().Base_Normal()) {
                coord_t cell_coord = Cell_To_Coord(cell_num);
                coord_t center_coord = bptr->Center_Coord();
                int width = bptr->Class_Of().Width();
                int height = bptr->Class_Of().Height();

                if ((Distance(center_coord, cell_coord) / 256) - ((width + height) / 2)
                    <= reinterpret_cast<BuildingTypeClass *>(object)->Adjacency()) {
                    return true;
                }
            }
        }
    }

    return passes;
}

/**
 * @brief Redraw cell tiles.
 *
 * 0x004B1FD0
 */
void DisplayClass::Redraw_Icons()
{
    m_RedrawShadow = false;

    // Iterate over all cells currently in view and check if it needs to be redrawn.
    for (int16_t y = -Coord_Sub_Cell_Y(m_DisplayPos); y <= m_DisplayHeight; y += 256) {
        for (int16_t x = -Coord_Sub_Cell_X(m_DisplayPos); x <= m_DisplayWidth; x += 256) {
            cell_t cellnum = Coord_To_Cell(Coord_Add(m_DisplayPos, Coord_From_Lepton_XY(x, y)));
            coord_t coord = Cell_To_Coord(cellnum) & 0xFF00FF00;
            int draw_x = 0;
            int draw_y = 0;

            // Check conditions for current position warrant attempting a redraw.
            if (In_View(cellnum) && Is_Cell_Flagged(cellnum) && Coord_To_Pixel(coord, draw_x, draw_y)) {
                CellClass &cell = m_Array[Coord_To_Cell(coord)];

                if (cell.Is_Visible() || g_DebugUnshroud) {
                    cell.Draw_It(draw_x, draw_y);
                }

                if (!cell.Is_Revealed() && !g_DebugUnshroud) {
                    m_RedrawShadow = true;
                }
            }
        }
    }
}

/**
 * @brief Redraw shroud.
 *
 * 0x004B2178
 */
void DisplayClass::Redraw_Shadow()
{
    if (m_RedrawShadow) {
        // Iterate over every position within the tactical view and evaluate for a shadow redraw.
        for (int16_t y = -Coord_Sub_Cell_Y(m_DisplayPos); y <= m_DisplayHeight; y += 256) {
            for (int16_t x = -Coord_Sub_Cell_X(m_DisplayPos); x <= m_DisplayWidth; x += 256) {
                cell_t cellnum = Coord_To_Cell(Coord_Add(m_DisplayPos, Coord_From_Lepton_XY(x, y)));
                coord_t coord = Cell_To_Coord_Top_Left(cellnum);
                int draw_x = 0;
                int draw_y = 0;

                if (In_View(cellnum) && Is_Cell_Flagged(cellnum) && Coord_To_Pixel(coord, draw_x, draw_y)) {
                    CellClass &cell = m_Array[Coord_To_Cell(coord)];

                    if (!cell.Is_Revealed()) {
                        int frame = SHADOW_FULL;

                        if (cell.Is_Visible()) {
                            frame = Cell_Shadow(cellnum);
                        }

                        if (frame >= 0) { // If we have a valid frame, draw the shroud frame.
                            CC_Draw_Shape(
                                s_ShadowShapes, frame, draw_x, draw_y, WINDOW_TACTICAL, SHAPE_GHOST, nullptr, s_ShadowTrans);
                        } else if (frame != SHADOW_CLEAR) { // Otherwise, if we don't have -1 fill black
                            int w = CELL_PIXELS;
                            int h = CELL_PIXELS;

                            int x_clip = Lepton_To_Pixel(m_DisplayWidth);
                            int y_clip = Lepton_To_Pixel(m_DisplayHeight);

                            if (Clip_Rect(draw_x, draw_y, w, h, x_clip, y_clip) >= 0) {
                                g_LogicPage->Fill_Rect(draw_x + m_TacOffsetX,
                                    draw_y + m_TacOffsetY,
                                    w + draw_x + m_TacOffsetX - 1,
                                    h + draw_y + m_TacOffsetY - 1,
                                    12);
                            }
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief Check if a cell is within the tactical view at all.
 *
 * 0x004B4CB8
 */
BOOL DisplayClass::In_View(cell_t cellnum) const
{
    // Check high bits aren't set, ensures not negative and is within range of 128 * 128 map.
    if ((cellnum & 0xC000) != 0) {
        return false;
    }

    uint32_t cell = Cell_To_Coord_Top_Left(cellnum);
    uint32_t loc1 = Coord_Top_Left(m_DisplayPos);

    // Makes use of unsigned underflow to detect if we are greater than m_DisplayPos but within display dimensions in two
    // compares.
    if ((uint16_t)Coord_Lepton_X(cell) - (uint32_t)Coord_Lepton_X(loc1) <= (uint32_t)m_DisplayWidth + 255) {
        if ((uint16_t)Coord_Lepton_Y(cell) - (uint32_t)Coord_Lepton_Y(loc1) <= (uint32_t)m_DisplayHeight + 255) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Convert a coord to the pixel x y position on screen.
 *
 * 0x004B0968
 */
BOOL DisplayClass::Coord_To_Pixel(coord_t coord, int &x, int &y) const
{
    if (coord != 0) {
        int16_t view_x = Lepton_Round_To_Pixel(Coord_Lepton_X(m_DisplayPos));
        int16_t view_y = Lepton_Round_To_Pixel(Coord_Lepton_Y(m_DisplayPos));
        int16_t lep_x = Lepton_Round_To_Pixel(Coord_Lepton_X(coord)) + 512 - view_x;
        int16_t lep_y = Lepton_Round_To_Pixel(Coord_Lepton_Y(coord)) + 512 - view_y;

        if (lep_x <= (m_DisplayWidth + 1024) && lep_y <= (m_DisplayHeight + 1024)) {
            x = Lepton_To_Pixel(lep_x) - CELL_PIXELS * 2;
            y = Lepton_To_Pixel(lep_y) - CELL_PIXELS * 2;

            return true;
        }
    }

    return false;
}

/**
 * @brief Convert a pixel x y position on screen to a coord.
 *
 * 0x004B2694
 */
coord_t DisplayClass::Pixel_To_Coord(int x, int y) const
{
    int x_pos = Pixel_To_Lepton(x - m_TacOffsetX);
    int y_pos = Pixel_To_Lepton(y - m_TacOffsetY);

    if (x_pos < m_DisplayWidth && y_pos < m_DisplayHeight) {
        return Coord_From_Lepton_XY(x_pos + Coord_Lepton_X(m_DisplayPos), y_pos + Coord_Lepton_Y(m_DisplayPos));
    }

    return 0;
}

/**
 * @brief Calculate how to draw shroud (if at all) for a given cell.
 *
 * 0x004B0698
 */
int DisplayClass::Cell_Shadow(cell_t cellnum) const
{
    // clang-format off
    static const int8_t _shadow[] = {
        -1, 33, 2, 2, 34, 37, 2, 2,
        4, 26, 6, 6, 4, 26, 6, 6,
        35, 45, 17, 17, 38, 41, 17, 17,
        4, 26, 6, 6, 4, 26, 6, 6,
        8, 21, 10, 10, 27, 31, 10, 10,
        12, 23, 14, 14, 12, 23, 14, 14,
        8, 21, 10, 10, 27, 31, 10, 10,
        12, 23, 14, 14, 12, 23, 14, 14,
        32, 36, 25, 25, 44, 40, 25, 25,
        19, 30, 20, 20, 19, 30, 20, 20,
        39, 43, 29, 29, 42, 46, 29, 29,
        19, 30, 20, 20, 19, 30, 20, 20,
        8, 21, 10, 10, 27, 31, 10, 10,
        12, 23, 14, 14, 12, 23, 14, 14,
        8, 21, 10, 10, 27, 31, 10, 10,
        12, 23, 14, 14, 12, 23, 14, 14,
        1, 1, 3, 3, 16, 16, 3, 3,
        5, 5, 7, 7, 5, 5, 7, 7,
        24, 24, 18, 18, 28, 28, 18, 18,
        5, 5, 7, 7, 5, 5, 7, 7,
        9, 9, 11, 11, 22, 22, 11, 11,
        13, 13, -2, -2, 13, 13, -2, -2,
        9, 9, 11, 11, 22, 22, 11, 11,
        13, 13, -2, -2, 13, 13, -2, -2,
        1, 1, 3, 3, 16, 16, 3, 3,
        5, 5, 7, 7, 5, 5, 7, 7,
        24, 24, 18, 18, 28, 28, 18, 18,
        5, 5, 7, 7, 5, 5, 7, 7,
        9, 9, 11, 11, 22, 22, 11, 11,
        13, 13, -2, -2, 13, 13, -2, -2,
        9, 9, 11, 11, 22, 22, 11, 11,
        13, 13, -2, -2, 13, 13, -2, -2,
    };
    // clang-format on

    int16_t cell_x = Cell_Get_X(cellnum);
    int16_t cell_y = Cell_Get_Y(cellnum);

    // Check we are in bounds, SS checks all 4 map bounds, RA only checks right edge bound with if ( cell_x - 1 < 127 ).
    if (cell_x >= 1 && cell_x < 127 && cell_y >= 1 && cell_y < 127) { // SS extended check.
        // Use pointer here rather than reference as we are going to do some pointer arithmetic on it.
        CellClass const *cell = &m_Array[cellnum];

        // If we aren't at least partly revealed, then no need to do anything else
        if (!cell->Is_Revealed() && !cell->Is_Visible()) {
            return SHADOW_FULL;
        }

        // If we are redrawing the icon, we need to know which frame we will need for the shroud
        // SS and C&C check in a different pattern and use two smaller tables to do the lookup, but result should be the
        // same.
        if (cell->Is_Visible()) {
            int index = 0;

            // NW adjacent
            if (!cell[-129].Is_Visible()) {
                index |= 0x40;
            }

            // N adjacent
            if (!cell[-128].Is_Visible()) {
                index |= 0x80;
            }

            // NE adjacent
            if (!cell[-127].Is_Visible()) {
                index |= 0x01;
            }

            // W adjacent
            if (!cell[-1].Is_Visible()) {
                index |= 0x20;
            }

            // E adjacent
            if (!cell[1].Is_Visible()) {
                index |= 0x02;
            }

            // SW adjacent
            if (!cell[127].Is_Visible()) {
                index |= 0x10;
            }

            // S adjacent
            if (!cell[128].Is_Visible()) {
                index |= 0x08;
            }

            // SE adjacent
            if (!cell[129].Is_Visible()) {
                index |= 0x04;
            }

            return _shadow[index];
        }
    }

    // -1 indicates no shroud, -2 indicates draw black if cell is not partly visible.Return is the frame of the shadow shape
    // file otherwise.
    return SHADOW_CLEAR;
}

/**
 * @brief Not sure exactly what this one does.
 *
 * 0x004B0B10
 */
BOOL DisplayClass::Push_Onto_TacMap(coord_t &coord1, coord_t &coord2)
{
    // Check trivial case that a coord is 0
    if (coord1 == 0 || coord2 == 0) {
        return false;
    }

    int coord1_x = Coord_Lepton_X(coord1);
    int coord1_y = Coord_Lepton_Y(coord1);
    int coord2_x = Coord_Lepton_X(coord2);
    int coord2_y = Coord_Lepton_Y(coord2);
    int start_x = Coord_Lepton_X(m_DisplayPos);
    int start_y = Coord_Lepton_Y(m_DisplayPos);
    int end_x = start_x + m_DisplayWidth;
    int end_y = start_y + m_DisplayHeight;

    // Check trivial case where a coord is not within view
    if ((coord1_x < start_x && coord2_x < start_x) || (coord1_x > end_x && coord2_x > end_x)
        || (coord1_y < start_y && coord2_y < start_y) || (coord1_y > end_y && coord2_y > end_y)) {
        return false;
    }

    // Set bounded coords.
    coord1 = Coord_From_Lepton_XY(std::clamp(coord1_x, start_x, end_x), std::clamp(coord1_y, start_y, end_y));
    coord2 = Coord_From_Lepton_XY(std::clamp(coord2_x, start_x, end_x), std::clamp(coord2_y, start_y, end_y));

    return true;
}

/**
 * 0x004B274C
 */
cell_t DisplayClass::Calculated_Cell(
    SourceType source, int waypoint, cell_t cellnum, SpeedType speed, BOOL use_zone, MZoneType mzone) const
{
    cell_t cell_num = -1;
    int zone = -1;

    if (waypoint != -1) {
        cell_num = g_Scen.Get_Waypoint(waypoint);
    }

    if (cell_num == -1) {
        cell_num = cellnum;
    }

    if (use_zone && cell_num != -1) {
        zone = m_Array[cell_num].Get_Zone(mzone);
    }

    bool y_clipped = false;
    bool x_clipped = false;

    int x = 0;
    int y = 0;

    // If we have a cell number from either the parameters or a waypoint, calculate an x and y position and ensure its within
    // the visible map. We also work out which edges we should clip to, x (east and west) or y (north and south).
    if (cell_num != -1) {
        int x_pos = std::min(Cell_Get_X(cell_num) - m_MapCellX, m_MapCellWidth + m_MapCellX - Cell_Get_X(cell_num));

        int y_pos = std::min(Cell_Get_Y(cell_num) - m_MapCellY, m_MapCellHeight + m_MapCellY - Cell_Get_Y(cell_num));

        if (x_pos > y_pos) {
            y_clipped = true;
            x_clipped = false;

            if (m_MapCellHeight / 2 <= Cell_Get_Y(cell_num) - m_MapCellY) {
                y = m_MapCellHeight;
            } else {
                y = -1;
            }

            x = Cell_Get_X(cell_num) - m_MapCellX;
        } else {
            y_clipped = false;
            x_clipped = true;

            if (m_MapCellWidth / 2 <= Cell_Get_X(cell_num) - m_MapCellX) {
                x = m_MapCellWidth;
            } else {
                x = -1;
            }

            y = Cell_Get_Y(cell_num) - m_MapCellY;
        }
    }

    // These won't be true if we didn't get a valid cell from the function parameters or a waypoint. In that case, we
    // randomly generate them based on the edge we are using as the source.
    if (!x_clipped && !y_clipped) {
        switch (source) {
            case SOURCE_EAST:
                x = m_MapCellWidth;
                y = g_Scen.Get_Random_Value(0, m_MapCellHeight - 1);
                break;

            case SOURCE_SOUTH:
                y_clipped = true;
                x = g_Scen.Get_Random_Value(0, m_MapCellWidth - 1);
                y = m_MapCellWidth;
                break;

            case SOURCE_WEST:
                x = -1;
                y = g_Scen.Get_Random_Value(0, m_MapCellHeight - 1);
                break;

            default:
                break;
        };
    }

    // Test cells along the left and right edges first.
    if (x_clipped && m_MapCellHeight > 0) {
        // Depending on edge, offset cell is always 1 cell toward the center of the map, applies to Y dimension as well
        // below.
        int offset = x <= m_MapCellX ? 1 : -1;
        int ny = y;

        for (int i = 0; i < m_MapCellHeight; ++i) {
            cell_t test_cell = Cell_From_XY(m_MapCellX + x, m_MapCellY + (ny % m_MapCellHeight));

            if (Good_Reinforcement_Cell(test_cell, test_cell + offset, speed, zone, mzone)) {
                return test_cell;
            }

            ++ny;
        }

        if (y_clipped && m_MapCellWidth > 0) {
            offset = y <= m_MapCellY ? 128 : -128;
            int nx = x;

            for (int i = 0; i < m_MapCellWidth; ++i) {
                cell_t test_cell = Cell_From_XY(m_MapCellX + (nx % m_MapCellWidth), m_MapCellY + y);

                if (Good_Reinforcement_Cell(test_cell, test_cell + offset, speed, zone, mzone)) {
                    return test_cell;
                }

                ++nx;
            }
        }
    } else if (y_clipped && m_MapCellWidth > 0) {
        int offset = y <= m_MapCellY ? 128 : -128;
        int nx = x;

        for (int i = 0; i < m_MapCellWidth; ++i) {
            cell_t test_cell = Cell_From_XY(m_MapCellX + (nx % m_MapCellWidth), m_MapCellY + y);

            if (Good_Reinforcement_Cell(test_cell, test_cell + offset, speed, zone, mzone)) {
                return test_cell;
            }

            ++nx;
        }
    }

    return Cell_From_XY(m_MapCellX + x, m_MapCellY + y);
}

/**
 * @brief Determines if a cell is a good reinforcement starting cell for moving into a cell offset from it.
 *
 * 0x004B2B90
 */
BOOL DisplayClass::Good_Reinforcement_Cell(cell_t cell1, cell_t cell2, SpeedType speed, int zone, MZoneType mzone) const
{
    if (m_Array[cell1].Is_Clear_To_Move(speed, false, false, zone, mzone)
        && m_Array[cell2].Is_Clear_To_Move(speed, false, false, zone, mzone)) {
        if (m_Array[cell1].Cell_Techno() != nullptr) {
            return false;
        } else {
            return m_Array[cell2].Cell_Techno() == nullptr;
        }
    }

    return false;
}

/**
 * @brief Determines closest free spot in the cell at the given coord.
 *
 * 0x004B4D80
 */
coord_t DisplayClass::Closest_Free_Spot(coord_t coord, BOOL skip_occupied) const
{
    if (!Coord_Is_Negative(coord)) {
        return 0x800080; // Middle of cell 0, the top left cell off the edge of visible map.
    }

    cell_t cellnum = Coord_To_Cell(coord);

    return m_Array[cellnum].Closest_Free_Spot(coord, skip_occupied);
}

/**
 * @brief Sets all objects to "look"?.
 *
 * 0x004B5680
 */
void DisplayClass::All_To_Look(BOOL skip_buildings)
{
    for (int i = 0; i < DisplayClass::s_Layers[LAYER_GROUND].Count(); ++i) {
        TechnoClass *objptr = reinterpret_cast<TechnoClass *>(DisplayClass::s_Layers[LAYER_GROUND][i]);

        if (objptr != nullptr && objptr->Is_Techno()) {
            if (objptr->What_Am_I() != RTTI_BUILDING || !skip_buildings) {
                if (objptr->Get_Owner_House()->Player_Has_Control()) {
                    if (objptr->Is_Player_Aware()) {
                        objptr->Look();
                    }
                } else if (objptr->What_Am_I() == RTTI_BUILDING) {
                    if (g_Rule.Ally_Reveal()) {
                        if (g_PlayerPtr->Is_Ally(objptr->Get_Owner_House())) {
                            objptr->Look();
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief
 *
 * 0x004B5788
 */
void DisplayClass::Constrained_Look(coord_t coord, int constraint)
{
    for (int index = 0; index < DisplayClass::s_Layers[LAYER_GROUND].Count(); ++index) {
        ObjectClass *objptr = DisplayClass::s_Layers[LAYER_GROUND][index];

        if (objptr != nullptr && objptr->Is_Techno()) {
            TechnoClass *tcptr = reinterpret_cast<TechnoClass *>(objptr);

            if (tcptr->Get_Owner_House()->Player_Has_Control()) {
                if (tcptr->Is_Player_Aware()) {
                    int lepton_sight =
                        constraint * (reinterpret_cast<TechnoTypeClass const &>(tcptr->Class_Of()).Get_Sight() * 256);

                    if (Distance(tcptr->Center_Coord(), coord) <= lepton_sight) {
                        tcptr->Look();
                    }
                }
            } else if (tcptr->What_Am_I() == RTTI_BUILDING) {
                if (g_Rule.Ally_Reveal()) {
                    if (g_PlayerPtr->Is_Ally(tcptr->Get_Owner_House()->What_Type())) {
                        int lepton_sight =
                            constraint * (reinterpret_cast<TechnoTypeClass const &>(tcptr->Class_Of()).Get_Sight() * 256);

                        if (Distance(tcptr->Center_Coord(), coord) <= lepton_sight) {
                            tcptr->Look();
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief Moves shroud in one step.
 *
 * 0x004B4F44
 */
void DisplayClass::Encroach_Shadow()
{
    if (!g_DebugUnshroud) {
        // Check every cell and mark unrevealed cells
        for (int cellnum = 0; cellnum < MAP_MAX_AREA; ++cellnum) {
            if (In_Radar(cellnum)) {
                CellClass &cell = m_Array[cellnum];

                if (!cell.Is_Revealed()) {
                    if (cell.Is_Visible()) {
                        cell.Set_Bit128(true);
                    }
                }
            }
        }

        for (int cellnum = 0; cellnum < MAP_MAX_AREA; ++cellnum) {
            if (In_Radar(cellnum)) {
                CellClass &cell = m_Array[cellnum];

                if (cell.Get_Bit128()) {
                    cell.Set_Bit128(false);
                    Shroud_Cell(cellnum);
                }
            }
        }

        All_To_Look(false);
    }

    Flag_To_Redraw(true);
}

/**
 * @brief Shrouds a given cell.
 *
 * 0x004B4FF4
 */
void DisplayClass::Shroud_Cell(cell_t cellnum)
{
    // If player has GPS or has units in the cell, then don't do anything.
    if (!g_PlayerPtr->Is_Map_Clear() || (m_Array[cellnum].Get_Field_A() & (1 << g_PlayerPtr->What_Type()))) {
        if (In_Radar(cellnum)) {
            CellClass &cell = m_Array[cellnum];

            if (cell.Is_Visible()) {
                cell.Set_Visible(false);
                cell.Set_Revealed(false);
                cell.Redraw_Objects();

                for (FacingType facing = FACING_FIRST; facing < FACING_COUNT; ++facing) {
                    cell_t adjcell = Cell_Get_Adjacent(cellnum, facing);

                    if (adjcell != cellnum) {
                        m_Array[adjcell].Set_Revealed(false);
                    }

                    m_Array[adjcell].Redraw_Objects();
                }
            }
        }
    }
}

/**
 * @brief Clip a rectangle. Written in asm in original.
 *
 * 0x005CC5C8
 */
int __cdecl DisplayClass::Clip_Rect(int &x, int &y, int &w, int &h, int clip_w, int clip_h)
{
    int xstart = x;
    int ystart = y;
    int yend = y + h - 1;
    int xend = x + w - 1;
    int result = 0;

    // If we aren't drawing within bounds, return -1
    if (xstart >= clip_w || ystart >= clip_h || xend < 0 || yend < 0) {
        return -1;
    }

    if (xstart < 0) {
        xstart = 0;
        result = 1;
    }

    if (ystart < 0) {
        ystart = 0;
        result = 1;
    }

    if (xend >= clip_w - 1) {
        xend = clip_w - 1;
        result = 1;
    }

    if (yend >= clip_h - 1) {
        yend = clip_h - 1;
        result = 1;
    }

    x = xstart;
    y = ystart;
    w = xend - xstart + 1;
    h = yend - ystart + 1;

    return result;
}

void DisplayClass::All_Layers_To_Redraw()
{
    for (LayerType layer = LAYER_FIRST; layer < LAYER_COUNT; ++layer) {
        s_Layers[layer].Render_All(true);
    }
}

void DisplayClass::Flag_All_Cells_To_Redraw()
{
    for (cell_t cellnum = 0; cellnum < MAP_MAX_AREA; ++cellnum) {
        if (!Is_Cell_Flagged(cellnum)) {
            s_CellRedraw[cellnum] = true;
        }
    }
}

void DisplayClass::Reset_Pending_Object()
{
    m_PendingObjectPtr = nullptr;
    m_PendingObjectTypePtr = nullptr;
    m_PendingObjectOwner = HOUSES_NONE;
    m_PendingSuper = SPECIAL_NONE;
}

/**
 *
 *
 */
BOOL DisplayClass::Is_Spot_Free(coord_t coord) const
{
    if (coord & 0x80008000) {
        return false;
    }
    return g_Map[Coord_To_Cell(coord)].Is_Spot_Free(CellClass::Spot_Index(coord));
}
