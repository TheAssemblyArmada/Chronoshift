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
LayerClass DisplayClass::Layers[LAYER_COUNT];
DisplayClass::TacticalClass DisplayClass::TacticalButton;
char DisplayClass::FadingBrighten[256];
char DisplayClass::FadingShade[256];
char DisplayClass::FadingWayDark[256];
char DisplayClass::FadingLight[256];
char DisplayClass::FadingGreen[256];
char DisplayClass::FadingYellow[256];
char DisplayClass::FadingRed[256];
char DisplayClass::TranslucentTable[14][256];
char DisplayClass::WhiteTranslucentTable[2][256];
char DisplayClass::MouseTranslucentTable[5][256];
char DisplayClass::ShadowTrans[5][256];
char DisplayClass::UnitShadow[2][256];
char DisplayClass::UnitShadowAir[2][256];
char DisplayClass::SpecialGhost[2][256];
void *DisplayClass::TransIconset = nullptr;
void *DisplayClass::ShadowShapes = nullptr;
BufferClass *DisplayClass::TheaterBuffer = nullptr;
BooleanVectorClass DisplayClass::CellRedraw;
#endif

char DisplayClass::FadingWhite[256];

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
        x = g_keyboard->Get_MouseQX();
        y = g_keyboard->Get_MouseQY();
    } else {
        x = g_mouse->Get_Mouse_X();
        y = g_mouse->Get_Mouse_Y();
    }

    bool at_edge = x == 0 || y == 0 || x == g_seenBuff.Get_Width() - 1 || y == g_seenBuff.Get_Height() - 1;
    coord_t coord = Map.Pixel_To_Coord(x, y);
    cell_t cell = Coord_To_Cell(coord);

    if (coord == 0) {
        return GadgetClass::Action(flags, key);
    }

    bool shrouded = !Map[cell].Is_Visible() && !g_Debug_Unshroud;
    int mouse_vpx = x - Map.Tac_Offset_X();
    int mouse_vpy = y - Map.Tac_Offset_Y();

    if (cell != Map.Get_Cursor_Start()) {
        Map.Set_Cursor_Pos(cell);
    }

    // Work out what object if any is near the cursor.
    TechnoClass *close_tech = nullptr;

    if (!shrouded) {
        ObjectClass *obj = Map.Close_Object(coord);

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

        if (Map.Repair_Mode()) {
            if (close_tech != nullptr && close_tech->Owner() == g_PlayerPtr->What_Type() && close_tech->Can_Repair()) {
                action = ACTION_REPAIR;
            } else {
                action = ACTION_NO_REPAIR;
            }
        }

        if (Map.Sell_Mode()) {
            if (close_tech != nullptr && close_tech->Owner() == g_PlayerPtr->What_Type() && close_tech->Can_Demolish()) {
                if (close_tech->What_Am_I() == RTTI_BUILDING) {
                    action = ACTION_SELL;
                } else {
                    action = ACTION_SELL_UNIT;
                }
            } else {
                if (Map[cell].Get_Overlay() != OVERLAY_NONE
                    && OverlayTypeClass::As_Reference(Map[cell].Get_Overlay()).Is_Wall()
                    && Map[cell].Owner() == g_PlayerPtr->What_Type()) {
                    action = ACTION_SELL;
                } else {
                    action = ACTION_NO_SELL;
                }
            }
        }

        switch (Map.PendingSuper) {
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
                    Map.Set_Pending_Super(SPECIAL_NONE);
                }
            }

            break;

            case SPECIAL_SONAR_PULSE: // These don't need to set anything so fall through to default.
            case SPECIAL_GPS:
            default:
                break;
        }

        if (Map.Pending_ObjectType() != nullptr) {
            action = ACTION_NONE;
        }
    }

    if (cell != Map.Get_Cursor_Start()) {
        Map.Set_Cursor_Pos(cell);
    }

    if (flags & MOUSE_RIGHT_PRESS) {
        Map.Mouse_Right_Press();
    }

    if ((flags & MOUSE_LEFT_UP) && Map.Bit_8()) {
        flags |= MOUSE_LEFT_RLSE;
    }

    if (!at_edge && (flags & MOUSE_LEFT_UP)) {
        Map.Mouse_Left_Up(cell, shrouded, close_tech, action);
    }

    if (flags & MOUSE_LEFT_RLSE) {
        Map.Mouse_Left_Release(cell, mouse_vpx, mouse_vpy, close_tech, action);
    }

    if (!at_edge && (flags & MOUSE_LEFT_PRESS)) {
        Map.Mouse_Left_Up(cell, shrouded, close_tech, action);
        Map.Mouse_Left_Press(mouse_vpx, mouse_vpy);
    }

    if ((flags & MOUSE_LEFT_HELD) != 0) {
        Map.Mouse_Left_Held(mouse_vpx, mouse_vpy);
    }

    return GadgetClass::Action(flags, key);
}

DisplayClass::DisplayClass() :
    DisplayPos(0),
    DisplayWidth(0),
    DisplayHeight(0),
    DisplayCursorStart(0),
    DisplayCursorEnd(0),
    DisplayCursorOccupy(nullptr),
    PassedProximityCheck(false),
    PendingObjectPtr(nullptr),
    PendingObjectTypePtr(nullptr),
    PendingObjectOwner(HOUSES_NONE),
    TacOffsetX(0),
    TacOffsetY(0),
    DisplayNewPos(0),
    DisplayToRedraw(true),
    DisplayRepairMode(false),
    DisplaySellMode(false),
    DisplayBit8(false),
    DisplayBit16(false),
    RedrawShadow(false),
    PendingSuper(SPECIAL_NONE),
    BandBox(0, 0, 0, 0)
{
    ShadowShapes = nullptr;
    TransIconset = nullptr;
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
    PendingObjectPtr = 0;
    PendingObjectTypePtr = 0;
    PendingObjectOwner = HOUSES_NONE;
    DisplayCursorOccupy = 0;
    PendingSuper = SPECIAL_NONE;
    DisplayToRedraw = true;
    DisplayRepairMode = false;
    DisplaySellMode = false;
    DisplayBit8 = false;
    DisplayBit16 = false;
    RedrawShadow = true;

    for (int i = 0; i < LAYER_COUNT; ++i) {
        Layers[i].Init();
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
    CellRedraw.Resize(MAP_MAX_AREA);

    for (LayerType layer = LAYER_FIRST; layer < LAYER_COUNT; ++layer) {
        Layers[layer].One_Time();
    }

    TransIconset = GameFileClass::Retrieve("trans.icn");
    DEBUG_ASSERT(TransIconset != nullptr);

    ShadowShapes = GameFileClass::Retrieve("shadow.shp");
    DEBUG_ASSERT(ShadowShapes != nullptr);

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

    if (!g_inMapEditor) {
        TacticalButton.Zap();
        Add_A_Button(TacticalButton);
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
    Scen.Set_Theater(theater);

    char mix_name[32];
    snprintf(mix_name, sizeof(mix_name), "%s.mix", g_theaters[theater].data);

    // Does the theater we want to initialise match the last one loaded? If so we don't need to do anything.
    if (Scen.Get_Theater() != g_lastTheater) {
        if (_theater_data != nullptr) {
            delete _theater_data;
        }

        _theater_data = new GameMixFile(mix_name, &g_publicKey);
        _theater_data->Cache(TheaterBuffer);
    }

    char pal_name[32];
    snprintf(pal_name, sizeof(pal_name), "%s.pal", g_theaters[theater].data);

    GamePalette = *static_cast<PaletteClass *>(GameFileClass::Retrieve(pal_name));
    OriginalPalette = GamePalette;

    Build_Fading_Table(GamePalette, FadingGreen, 3, 110);
    Build_Fading_Table(GamePalette, FadingYellow, 5, 140);
    Build_Fading_Table(GamePalette, FadingRed, 8, 140);
    Build_Fading_Table(GamePalette, FadingWhite, 15, 120);

    Build_Translucent_Table(GamePalette, _mouse_cols, ARRAY_SIZE(_mouse_cols), MouseTranslucentTable);
    Build_Translucent_Table(GamePalette, _magic_cols, ARRAY_SIZE(_magic_cols), TranslucentTable);
    Build_Translucent_Table(GamePalette, _white_cols, ARRAY_SIZE(_white_cols), WhiteTranslucentTable);
    Build_Translucent_Table(GamePalette, _shadow_cols, ARRAY_SIZE(_shadow_cols), ShadowTrans);

    Conquer_Build_Translucent_Table(GamePalette, _ushadow_cols_air, ARRAY_SIZE(_ushadow_cols_air), UnitShadowAir);

    if (theater == THEATER_SNOW) {
        Conquer_Build_Translucent_Table(GamePalette, _ushadow_cols_snow, ARRAY_SIZE(_ushadow_cols_snow), UnitShadow);
        Conquer_Build_Fading_Table(GamePalette, FadingShade, 12, 75);
    } else {
        Conquer_Build_Translucent_Table(GamePalette, _ushadow_cols, ARRAY_SIZE(_ushadow_cols), UnitShadow);
        Conquer_Build_Fading_Table(GamePalette, FadingShade, 12, 130);
    }

    Conquer_Build_Fading_Table(GamePalette, FadingLight, 15, 85);
    Conquer_Build_Fading_Table(GamePalette, SpecialGhost[1], 12, 100);
    memset(SpecialGhost, 0, 256);
    Make_Fading_Table(GamePalette, FadingBrighten, 15, 25);
    Make_Fading_Table(GamePalette, FadingWayDark, 13, 192);

    Options.Fixup_Palette();
}

/**
 * @brief Performs per tick logic for this layer.
 *
 * 0x004B0140
 */
void DisplayClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
    if (DisplayBit8) {
        if (g_mouse->Get_Mouse_X() < TacOffsetX || g_mouse->Get_Mouse_Y() < TacOffsetY
            || g_mouse->Get_Mouse_X() >= CELL_PIXELS * DisplayHeight + 128 + TacOffsetX
            || g_mouse->Get_Mouse_Y() >= CELL_PIXELS * DisplayWidth + 128 + TacOffsetY) {
            Mouse_Left_Release(-1, g_mouse->Get_Mouse_X(), g_mouse->Get_Mouse_Y());
        }
    }

    MapClass::AI(key, mouse_x, mouse_y);
}

void DisplayClass::Draw_It(BOOL force_redraw)
{
    if (DisplayToRedraw || force_redraw) {
        DisplayToRedraw = false;
        Refresh_Band();
        g_ChronalVortex.Set_Redraw();
        int msg_count = Session.Get_Messages().Num_Messages();

        // If we have messages to draw, redraw the relevant rows of cells
        if (msg_count > 0) {
            cell_t cell = Coord_To_Cell(DisplayPos);
            cell_t ecell = cell + Lepton_To_Cell_Coord(DisplayWidth) + 1;

            for (; cell < ecell; ++cell) {
                Map[cell].Redraw_Objects();
            }

            cell = Coord_To_Cell(DisplayPos) + MAP_MAX_WIDTH;
            ecell = cell + Lepton_To_Cell_Coord(DisplayWidth) + 1 + MAP_MAX_WIDTH;

            for (; cell < ecell; ++cell) {
                Map[cell].Redraw_Objects();
            }

            if (msg_count > 1) {
                cell = Coord_To_Cell(DisplayPos) + (MAP_MAX_WIDTH * 2);
                ecell = cell + Lepton_To_Cell_Coord(DisplayWidth) + 1 + (MAP_MAX_WIDTH * 2);

                for (; cell < ecell; ++cell) {
                    Map[cell].Redraw_Objects();
                }
            }

            if (msg_count > 2) {
                cell = Coord_To_Cell(DisplayPos) + (MAP_MAX_WIDTH * 3);
                ecell = cell + Lepton_To_Cell_Coord(DisplayWidth) + 1 + (MAP_MAX_WIDTH * 3);

                for (; cell < ecell; ++cell) {
                    Map[cell].Redraw_Objects();
                }
            }

            if (msg_count > 3) {
                cell = Coord_To_Cell(DisplayPos) + (MAP_MAX_WIDTH * 4);
                ecell = cell + Lepton_To_Cell_Coord(DisplayWidth) + 1 + (MAP_MAX_WIDTH * 4);

                for (; cell < ecell; ++cell) {
                    Map[cell].Redraw_Objects();
                }
            }
        }
        
        if (Lepton_To_Pixel(Coord_Lepton_X(DisplayNewPos)) == Lepton_To_Pixel(Coord_Lepton_X(DisplayPos))
            && Lepton_To_Pixel(Coord_Lepton_Y(DisplayNewPos)) == Lepton_To_Pixel(Coord_Lepton_Y(DisplayPos))) {
            ++ScenarioInit;

            if (DisplayNewPos != DisplayPos) {
                Set_Tactical_Position(DisplayNewPos);
            }

            --ScenarioInit;

        } else {
            int x_move_dist = Lepton_To_Pixel(Coord_Lepton_X(DisplayPos)) - Lepton_To_Pixel(Coord_Lepton_X(DisplayNewPos));
            int y_move_dist = Lepton_To_Pixel(Coord_Lepton_Y(DisplayPos)) - Lepton_To_Pixel(Coord_Lepton_Y(DisplayNewPos));
            int width_remain = Lepton_To_Pixel(DisplayWidth) - std::abs(x_move_dist);
            int height_remain = Lepton_To_Pixel(DisplayHeight) - std::abs(y_move_dist);

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
                || (Lepton_To_Pixel(DisplayWidth) == width_remain && Lepton_To_Pixel(DisplayHeight) == height_remain)) {
                force_redraw = true;
            } else {
                // Do a block blit of anything still in view.
                Set_Cursor_Pos(-1);

                if (g_hidPage.In_Video_Memory()) {
                    g_mouse->Hide_Mouse();

                    int src_x = TacOffsetX + -std::min(0, x_move_dist);
                    int src_y = -std::min(0, y_move_dist) + TacOffsetY;
                    int dst_x = std::max(0, x_move_dist) + TacOffsetX;
                    int dst_y = TacOffsetY + std::max(0, y_move_dist);

                    g_seenBuff.Blit(g_hidPage, src_x, src_y, dst_x, dst_y, width_remain, height_remain);
                    g_mouse->Show_Mouse();
                } else {
                    int src_x = TacOffsetX + -std::min(0, x_move_dist);
                    int src_y = -std::min(0, y_move_dist) + TacOffsetY;
                    int dst_x = std::max(0, x_move_dist) + TacOffsetX;
                    int dst_y = TacOffsetY + std::max(0, y_move_dist);

                    g_hidPage.Blit(g_hidPage, src_x, src_y, dst_x, dst_y, width_remain, height_remain);
                }
            }

            x_move_dist = std::max(0, x_move_dist);
            y_move_dist = std::max(0, y_move_dist);

            ++ScenarioInit;
            Set_Tactical_Position(DisplayNewPos);
            --ScenarioInit;

            // If we aren't forcing a complete redraw, then redraw around the edges of the moved section.
            if (!force_redraw) {
                width_remain -= 24;
                height_remain -= 24;

                if (std::abs(x_move_dist) >= 37 || std::abs(y_move_dist) >= 37) {
                    int x_pixel = -Lepton_To_Pixel(Coord_Sub_Cell_X(DisplayPos));
                    int y_pixel = -Lepton_To_Pixel(Coord_Sub_Cell_Y(DisplayPos));
                    width_remain -= CELL_PIXELS;
                    height_remain -= CELL_PIXELS;

                    for (int j = y_pixel; Lepton_To_Pixel(DisplayHeight) + CELL_PIXELS * 2 >= j; j += CELL_PIXELS) { // 24 == ICON_HEIGHT?
                        for (int k = x_pixel; Lepton_To_Pixel(DisplayWidth) + CELL_PIXELS * 2 >= k; k += CELL_PIXELS) { // 24 == ICON_WIDTH?
                            if (k <= x_move_dist || width_remain + x_move_dist <= k || j <= y_move_dist
                                || height_remain + y_move_dist <= j) {
                                int hi = Lepton_To_Pixel(DisplayHeight) - 1;
                                int lo = 0;

                                if (hi <= 0) {
                                    std::swap(hi, lo);
                                }

                                int y_check = std::clamp(j, lo, hi);

                                hi = Lepton_To_Pixel(DisplayWidth) - 1;
                                lo = 0;

                                if (hi <= 0) {
                                    std::swap(hi, lo);
                                }

                                int x_check = std::clamp(k, lo, hi);

                                cell_t click_cell = Click_Cell_Calc(x_check + TacOffsetX, y_check + TacOffsetY);

                                if (click_cell > 0) {
                                    (*this)[click_cell].Redraw_Objects(true);
                                }
                            }
                        }
                    }

                } else {
                    int x_move_fudge = std::abs(x_move_dist) < 16 ? 1 : 2;
                    int y_move_fudge = std::abs(y_move_dist) < 16 ? 1 : 2;

                    int x_pixel2 = -Lepton_To_Pixel(Coord_Sub_Cell_X(DisplayPos));
                    int y_pixel2 = -Lepton_To_Pixel(Coord_Sub_Cell_Y(DisplayPos));

                    if (move_up) {
                        for (int j = y_pixel2; y_pixel2 + CELL_PIXELS * y_move_fudge >= j; j += CELL_PIXELS) {
                            for (int k = x_pixel2; Lepton_To_Pixel(DisplayWidth) + CELL_PIXELS * 2 >= k; k += CELL_PIXELS) {
                                int hi = Lepton_To_Pixel(DisplayHeight) - 1;
                                int lo = 0;

                                if (hi <= 0) {
                                    std::swap(hi, lo);
                                }

                                int y_check = std::clamp(j, lo, hi);

                                hi = Lepton_To_Pixel(DisplayWidth) - 1;
                                lo = 0;

                                if (hi <= 0) {
                                    std::swap(hi, lo);
                                }

                                int x_check = std::clamp(k, lo, hi);

                                cell_t click_cell = Click_Cell_Calc(x_check + TacOffsetX, y_check + TacOffsetY);

                                if (click_cell > 0) {
                                    (*this)[click_cell].Redraw_Objects(true);
                                }
                            }
                        }
                    }

                    if (move_down) {
                        for (int j = Lepton_To_Pixel(DisplayHeight) - CELL_PIXELS * y_move_fudge;
                             Lepton_To_Pixel(DisplayHeight) + 72 >= j;
                             j += CELL_PIXELS) {
                            for (int k = x_pixel2; Lepton_To_Pixel(DisplayWidth) + CELL_PIXELS * 2 >= k; k += CELL_PIXELS) {
                                int hi = Lepton_To_Pixel(DisplayHeight) - 1;
                                int lo = 0;

                                if (hi <= 0) {
                                    std::swap(hi, lo);
                                }

                                int y_check = std::clamp(j, lo, hi);

                                hi = Lepton_To_Pixel(DisplayWidth) - 1;
                                lo = 0;

                                if (hi <= 0) {
                                    std::swap(hi, lo);
                                }

                                int x_check = std::clamp(k, lo, hi);

                                cell_t click_cell = Click_Cell_Calc(x_check + TacOffsetX, y_check + TacOffsetY);

                                if (click_cell > 0) {
                                    (*this)[click_cell].Redraw_Objects(true);
                                }
                            }
                        }
                    }

                    if (move_left) {
                        for (int k = x_pixel2; x_pixel2 + CELL_PIXELS * x_move_fudge >= k; k += CELL_PIXELS) {
                            for (int j = y_pixel2; Lepton_To_Pixel(DisplayHeight) + CELL_PIXELS * 2 >= j; j += CELL_PIXELS) {
                                int hi = Lepton_To_Pixel(DisplayHeight) - 1;
                                int lo = 0;

                                if (hi <= 0) {
                                    std::swap(hi, lo);
                                }

                                int y_check = std::clamp(j, lo, hi);

                                hi = Lepton_To_Pixel(DisplayWidth) - 1;
                                lo = 0;

                                if (hi <= 0) {
                                    std::swap(hi, lo);
                                }

                                int x_check = std::clamp(k, lo, hi);

                                cell_t click_cell = Click_Cell_Calc(x_check + TacOffsetX, y_check + TacOffsetY);

                                if (click_cell > 0) {
                                    (*this)[click_cell].Redraw_Objects(true);
                                }
                            }
                        }
                    }

                    if (move_right) {
                        for (int k = Lepton_To_Pixel(DisplayWidth) - x_move_fudge * CELL_PIXELS;
                             Lepton_To_Pixel(DisplayWidth) + 72 >= k;
                             k += CELL_PIXELS) {
                            for (int j = y_pixel2; Lepton_To_Pixel(DisplayHeight) + CELL_PIXELS * 2 >= j; j += CELL_PIXELS) {
                                int hi = Lepton_To_Pixel(DisplayHeight) - 1;
                                int lo = 0;

                                if (hi <= 0) {
                                    std::swap(hi, lo);
                                }

                                int y_check = std::clamp(j, lo, hi);

                                hi = Lepton_To_Pixel(DisplayWidth) - 1;
                                lo = 0;

                                if (hi <= 0) {
                                    std::swap(hi, lo);
                                }

                                int x_check = std::clamp(k, lo, hi);

                                cell_t click_cell = Click_Cell_Calc(x_check + TacOffsetX, y_check + TacOffsetY);

                                if (click_cell > 0) {
                                    (*this)[click_cell].Redraw_Objects(true);
                                }
                            }
                        }
                    }
                }
            }
        }

        if (force_redraw) {
            CellRedraw.Set();
        }

        if (g_hidPage.Lock()) {
            Redraw_Icons();
            g_hidPage.Unlock();
        }

        if (g_hidPage.Lock()) {
            g_ChronalVortex.Render();

            for (LayerType layer = LAYER_FIRST; layer < LAYER_COUNT; ++layer) {
                Layers[layer].Render_All();
            }

            // SS does some drawing of FLAGFLY.SHP here after rendering surface
            // objects.

            Redraw_Shadow();
            g_hidPage.Unlock();
        }

        if (DisplayBit8) {
            g_logicPage->Draw_Rect(TacOffsetX + BandBox.Get_Left(),
                TacOffsetY + BandBox.Get_Top(),
                TacOffsetX + BandBox.Get_Right(),
                TacOffsetY + BandBox.Get_Bottom(),
                15);
        }

        CellRedraw.Reset();
    }
}

void DisplayClass::Code_Pointers()
{
    if (PendingObjectPtr != nullptr) {
        PendingObjectPtr = reinterpret_cast<ObjectClass *>((uintptr_t)PendingObjectPtr->As_Target());
    }

    MapClass::Code_Pointers();
}

void DisplayClass::Decode_Pointers()
{
    if ((uintptr_t)PendingObjectPtr != 0) {
        PendingObjectPtr = As_Object((uintptr_t)PendingObjectPtr);
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

void DisplayClass::Write_INI(GameINIClass &ini)
{
    // Needs celltriggerclass
#ifdef GAME_DLL
    void (*func)(const DisplayClass *, GameINIClass &) =
        reinterpret_cast<void (*)(const DisplayClass *, GameINIClass &)>(0x004B545C);
    func(this, ini);
#endif
}

/**
 * Marks a cell as being visited by the givent house, provided the house is spied by, allied with or is the player.
 */
BOOL DisplayClass::Map_Cell(cell_t cellnum, HouseClass *house)
{
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);

    // Spied radar reveals enemy movement logic
    if (house != nullptr && !house->Is_Player()) {
        if (house->Spied_My_Radar(g_PlayerPtr) || (Session.Game_To_Play() == GAME_CAMPAIGN && house->Is_Ally(g_PlayerPtr))) {
            house = g_PlayerPtr;
        }
    }

    if (!house->Is_Player() || !In_Radar(cellnum)) {
        return false;
    }

    CellClass &cell = Map[cellnum];

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
        CellClass &adj_cell = Map[adjacent];

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
    int xpos = (lepton_t)Pixel_To_Lepton(x - TacOffsetX);
    int ypos = (lepton_t)Pixel_To_Lepton(y - TacOffsetY);

    if (xpos < DisplayWidth && ypos < DisplayHeight) {
        return Coord_To_Cell(Coord_From_Lepton_XY(xpos + Lepton_Round_To_Pixel(Coord_Lepton_X(DisplayPos)),
            ypos + Lepton_Round_To_Pixel(Coord_Lepton_Y(DisplayPos))));
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

        if (Coord_Cell_To_Lepton(MapCellX) == Coord_Lepton_X(DisplayPos) && face != FACING_WEST) {
            if (face == FACING_SOUTH_WEST) {
                dir = DIR_SOUTH;
            }

            if (face == FACING_NORTH_WEST) {
                dir = DIR_NORTH;
            }
        }

        if (Coord_Cell_To_Lepton(MapCellY) == Coord_Lepton_Y(DisplayPos) && face != FACING_NORTH) {
            if (face == FACING_NORTH_WEST) {
                dir = DIR_WEST;
            }

            if (face == FACING_NORTH_EAST) {
                dir = DIR_EAST;
            }
        }

        int16_t view_edge = DisplayWidth + Coord_Lepton_X(DisplayPos);

        if (view_edge == Coord_Cell_To_Lepton(MapCellWidth + MapCellX) && face != FACING_EAST) {
            if (face == FACING_NORTH_EAST) {
                dir = DIR_NORTH;
            }

            if (face == FACING_SOUTH_EAST) {
                dir = DIR_SOUTH;
            }
        }

        view_edge = DisplayHeight + Coord_Lepton_Y(DisplayPos);

        if (Coord_Cell_To_Lepton(MapCellHeight + MapCellY) == view_edge && face != FACING_SOUTH) {
            if (face == FACING_SOUTH_EAST) {
                dir = DIR_EAST;
            }

            if (face == FACING_SOUTH_WEST) {
                dir = DIR_WEST;
            }
        }

        coord_t new_pos = Coord_Move(DisplayPos, dir, distance);
        int x_pos = Coord_Lepton_X(new_pos) - Coord_Cell_To_Lepton(MapCellX);
        int y_pos = Coord_Lepton_Y(new_pos) - Coord_Cell_To_Lepton(MapCellY);

        bool confined = Confine_Rect(x_pos,
            y_pos,
            DisplayWidth,
            DisplayHeight,
            Coord_Cell_To_Lepton(MapCellWidth),
            Coord_Cell_To_Lepton(MapCellHeight));

        if (x_pos < 0) {
            x_pos = 0;
            confined = true;
        }

        if (y_pos < 0) {
            y_pos = 0;
            confined = true;
        }

        coord_t pos_coord =
            Coord_From_Lepton_XY(x_pos + Coord_Cell_To_Lepton(MapCellX), y_pos + Coord_Cell_To_Lepton(MapCellY));

        // Update distance if the move was constrained.
        if (confined) {
            distance = Distance(DisplayPos, pos_coord);
        }

        // If we actually moved, redraw.
        if (distance != 0 && pos_coord != DisplayPos) {
            if (redraw) {
                Set_Tactical_Position(pos_coord);
                DisplayToRedraw = true;
                Flag_To_Redraw();
                Layers[LAYER_TOP].Mark_All(MARK_REDRAW);
                Layers[LAYER_AIR].Mark_All(MARK_REDRAW);
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
void DisplayClass::Refresh_Cells(cell_t cellnum, int16_t *overlap_list)
{
    int16_t tmp_list[60];

    if (overlap_list != nullptr) {
        if (*overlap_list == LIST_START) {
            ++overlap_list;
        }

        List_Copy(tmp_list, overlap_list, ARRAY_SIZE(tmp_list));

        for (int i = 0; i < ARRAY_SIZE(tmp_list); ++i) {
            if (tmp_list[i] == LIST_END) {
                break;
            }

            cell_t refresh_cell = cellnum + tmp_list[i];

            if (In_Radar(refresh_cell)) {
                Array[refresh_cell].Redraw_Objects();
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
        DisplayWidth = Pixel_To_Lepton(g_seenBuff.Get_Width() - x);
    } else {
        DisplayWidth = w * 256;
    }

    if (h == -1) {
        DisplayHeight = Pixel_To_Lepton(g_seenBuff.Get_Height() - y);
    } else {
        DisplayHeight = h * 256;
    }

    int x_pos = Coord_Lepton_X(DisplayPos) - (MapCellX * 256);
    int y_pos = Coord_Lepton_Y(DisplayPos) - (MapCellY * 256);

    Confine_Rect(x_pos, y_pos, DisplayWidth, DisplayHeight, MapCellWidth * 256, MapCellHeight * 256);

    coord_t tacpos_coord = Coord_From_Lepton_XY(x_pos + (MapCellX * 256), y_pos + (MapCellY * 256));

    Set_Tactical_Position(tacpos_coord);
    TacOffsetX = x;
    TacOffsetY = y;

    WindowList[WINDOW_TACTICAL].W = Lepton_To_Pixel(DisplayWidth);
    WindowList[WINDOW_TACTICAL].H = Lepton_To_Pixel(DisplayHeight);
    WindowList[WINDOW_TACTICAL].X = x;
    WindowList[WINDOW_TACTICAL].Y = y;

    if (Window == WINDOW_TACTICAL) {
        Change_Window(WINDOW_0);
        Change_Window(WINDOW_TACTICAL);
    }

    DisplayToRedraw = true;
    Flag_To_Redraw();

    TacticalButton.Set_XPos(TacOffsetX);
    TacticalButton.Set_YPos(TacOffsetY);
    TacticalButton.Set_Width(Lepton_To_Pixel(DisplayWidth));
    TacticalButton.Set_Height(Lepton_To_Pixel(DisplayHeight));
}

/**
 * @brief Sets the position the tactical view top corner to a given coord.
 *
 * 0x004B4860
 */
void DisplayClass::Set_Tactical_Position(coord_t location)
{
    // TODO This function will probably need adjustment to handle tactical view bigger than map for small maps.
    int x_pos = Coord_Lepton_X(location) - Coord_Cell_To_Lepton(MapCellX);
    int y_pos = Coord_Lepton_Y(location) - Coord_Cell_To_Lepton(MapCellY);

    // Ensures our x and y positions are within the map
    Confine_Rect(
        x_pos, y_pos, DisplayWidth, DisplayHeight, Coord_Cell_To_Lepton(MapCellWidth), Coord_Cell_To_Lepton(MapCellHeight));

    int16_t y_coord = Coord_Cell_To_Lepton(MapCellY) + y_pos;
    int16_t x_coord = Coord_Cell_To_Lepton(MapCellX) + x_pos;

    DisplayNewPos = Coord_From_Lepton_XY(x_coord, y_coord);

    // Update the display position if needed
    if (ScenarioInit) {
        DisplayPos = Coord_From_Lepton_XY(x_coord, y_coord);
    }

    DisplayToRedraw = true;
    Flag_To_Redraw();
}

/**
 * @brief Flags a specific cell to redraw.
 *
 * 0x004B5908
 */
void DisplayClass::Flag_Cell(cell_t cellnum)
{
    CellRedraw[cellnum] = true;
    DisplayToRedraw = true;
    Flag_To_Redraw();
}

void DisplayClass::Mouse_Right_Press()
{
    if (PendingObjectPtr != nullptr && PendingObjectPtr->Is_Techno()) {
        PendingObjectPtr = nullptr;
        PendingObjectTypePtr = nullptr;
        PendingObjectOwner = HOUSES_NONE;
        Set_Cursor_Shape();
    } else if (DisplayRepairMode) {
        DisplayRepairMode = false;
    } else if (DisplaySellMode) {
        DisplaySellMode = false;
    } else if (PendingSuper == SPECIAL_NONE) {
        Unselect_All();
    } else {
        PendingSuper = SPECIAL_NONE;
    }

    Set_Default_Mouse(MOUSE_POINTER, Map.Mouse_In_Radar());
}

void DisplayClass::Mouse_Left_Press(int mouse_x, int mouse_y)
{
    if (!DisplayRepairMode && !DisplaySellMode && PendingSuper == SPECIAL_NONE && PendingObjectTypePtr == nullptr) {
        BandBox.m_left = mouse_x;
        BandBox.m_top = mouse_y;
        BandBox.m_right = mouse_x;
        BandBox.m_bottom = mouse_y;
        DisplayBit16 = true;
    }
}

void DisplayClass::Mouse_Left_Up(
    cell_t cellnum, BOOL cell_shrouded, ObjectClass *object, ActionType action, BOOL mouse_in_radar)
{
#ifdef GAME_DLL
    void (*func)(const DisplayClass *, cell_t, BOOL, ObjectClass *, ActionType, BOOL) =
        reinterpret_cast<void (*)(const DisplayClass *, cell_t, BOOL, ObjectClass *, ActionType, BOOL)>(0x004B3780);
    func(this, cellnum, cell_shrouded, object, action, mouse_in_radar);
#endif
}

void DisplayClass::Mouse_Left_Held(int mouse_x, int mouse_y)
{
    if (DisplayBit8) {
        if (mouse_x != BandBox.m_right || mouse_y != BandBox.m_bottom) {
            Refresh_Band();
            // Need confirming, check between sole and ra!
            BandBox.m_right = std::clamp(mouse_x, 0, Lepton_To_Pixel(DisplayWidth) - 1);
            BandBox.m_bottom = std::clamp(mouse_y, 0, Lepton_To_Pixel(DisplayHeight) - 1);
            DisplayToRedraw = true;
            Flag_To_Redraw();
        }
    } else if (DisplayBit16) {
        // Ensure we actually drew a bandbox of usable size?
        if (std::abs(BandBox.m_left - mouse_x) > 4 || std::abs(BandBox.m_top - mouse_y) > 4) {
            DisplayBit8 = true;
            BandBox.m_right = std::clamp(mouse_x, 0, Lepton_To_Pixel(DisplayWidth) - 1);
            BandBox.m_bottom = std::clamp(mouse_y, 0, Lepton_To_Pixel(DisplayHeight) - 1);
            DisplayToRedraw = true;
            Flag_To_Redraw();

            Layers[LAYER_TOP].Mark_All(MARK_REDRAW);
            Layers[LAYER_AIR].Mark_All(MARK_REDRAW);
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

/**
 * @brief Converts a pixel on screen to the logical location in the engine.
 *
 * 0x004B2694
 */
coord_t DisplayClass::Pixel_To_Coord(int x, int y)
{
    int x_pos = Pixel_To_Lepton(x - TacOffsetX);
    int y_pos = Pixel_To_Lepton(y - TacOffsetY);

    if (x_pos < DisplayWidth && y_pos < DisplayHeight) {
        return Coord_From_Lepton_XY(x_pos + Coord_Lepton_X(DisplayPos), y_pos + Coord_Lepton_Y(DisplayPos));
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

    if (DisplayCursorOccupy != nullptr) {
        Cursor_Mark(DisplayCursorStart + DisplayCursorEnd, false);
    }

    DisplayCursorEnd = 0;

    if (list != nullptr) {
        int x;
        int y;

        List_Copy(tmp_list, list, ARRAY_SIZE(tmp_list));
        DisplayCursorOccupy = tmp_list;
        Get_Occupy_Dimensions(x, y, tmp_list);
        DisplayCursorEnd = -Cell_From_XY(x / 2, y / 2);
        Cursor_Mark(DisplayCursorStart + DisplayCursorEnd, true);
    } else {
        DisplayCursorOccupy = nullptr;
    }
}

/**
 * @brief Draw the selection band box.
 *
 * 0x004B2E84
 */
void DisplayClass::Refresh_Band()
{
    if (DisplayBit8) {
        int box_x = BandBox.m_left + TacOffsetX;
        int box_y = BandBox.m_top + TacOffsetY;
        int box_w = BandBox.m_right + TacOffsetX;
        int box_h = BandBox.m_bottom + TacOffsetY;

        if (box_x > box_w) {
            std::swap(box_x, box_w);
        }

        if (box_y > box_h) {
            std::swap(box_y, box_h);
        }

        // This loop should redraw the left and right side of the selection band.
        for (int i = box_y; i <= (box_h + CELL_PIXELS); i += CELL_PIXELS) {
            int b_pxl = Lepton_To_Pixel(DisplayHeight) + TacOffsetY;
            cell_t click_cell = Click_Cell_Calc(box_x, std::clamp(i, 0, b_pxl));

            if (click_cell != -1) {
                Array[click_cell].Redraw_Objects();
            }

            click_cell = Click_Cell_Calc(box_w, std::clamp(i, 0, b_pxl));

            if (click_cell != -1) {
                Array[click_cell].Redraw_Objects();
            }
        }

        // This loop should redraw the top and bottom side of the selection band.
        for (int i = box_x; i < (box_w + CELL_PIXELS); i += CELL_PIXELS) {
            int b_pxl = Lepton_To_Pixel(DisplayWidth) + TacOffsetX;
            cell_t click_cell = Click_Cell_Calc(std::clamp(i, 0, b_pxl), box_y);

            if (click_cell != -1) {
                Array[click_cell].Redraw_Objects();
            }

            click_cell = Click_Cell_Calc(std::clamp(i, 0, b_pxl), box_h);

            if (click_cell != -1) {
                Array[click_cell].Redraw_Objects();
            }
        }

        Layers[LAYER_TOP].Mark_All(MARK_REDRAW);
        Layers[LAYER_AIR].Mark_All(MARK_REDRAW);
    }
}

void DisplayClass::Cursor_Mark(cell_t cellnum, BOOL flag)
{
    // Check we have a valid cellnum
    if (cellnum == -1) {
        return;
    }

    for (int16_t *offsets = DisplayCursorOccupy; *offsets != LIST_END; ++offsets) {
        cell_t offset_cell = cellnum + *offsets;

        if (MapClass::In_Radar(offset_cell)) {
            CellClass &cell_ref = Map[offset_cell];
            cell_ref.Redraw_Objects();
            cell_ref.Set_Placement_Check(flag);
        }
    }

    if (PendingObjectPtr != nullptr) {
        for (const int16_t *overlap = PendingObjectPtr->Overlap_List(); *overlap != LIST_END; ++overlap) {
            int16_t offset_cell = cellnum + *overlap;

            if (MapClass::In_Radar(offset_cell)) {
                Map[offset_cell].Redraw_Objects();
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
        int width = TacOffsetX + Lepton_To_Pixel(DisplayWidth);

        if (str_width + x >= width) {
            str_width = width - x;
            *w_list = LIST_START;
            --count;
            ++w_list;
        }

        if (width >= x) {
            cell_t click_cell = Click_Cell_Calc(x, y - 1);
            int height = std::clamp(y + 24, TacOffsetY, TacOffsetY + (Lepton_To_Pixel(DisplayHeight) - 1));
            cell_t offset_click = Click_Cell_Calc(str_width + x - 1, height);

            if (click_cell == -1) {
                click_cell = Click_Cell_Calc(x, y);
            }

            if (click_cell != -1 && offset_click != -1) {
                // If we have two valid cells locations, iterate through the cells between them.
                for (int i = Cell_Get_Y(click_cell); i <= Cell_Get_Y(offset_click); ++i) {
                    for (int j = Cell_Get_X(click_cell); j <= Cell_Get_X(offset_click); ++j) {
                        cell_t current_cell = Coord_To_Cell(DisplayPos);
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

ObjectClass *DisplayClass::Prev_Object(ObjectClass *object) const
{
#ifdef GAME_DLL
    ObjectClass *(*func)(const DisplayClass *, ObjectClass *) =
        reinterpret_cast<ObjectClass *(*)(const DisplayClass *, ObjectClass *)>(0x004B2618);
    return func(this, object);
#else
    return 0;
#endif
}

ObjectClass *DisplayClass::Next_Object(ObjectClass *object) const
{
    ObjectClass *retval = nullptr;
    bool found = false;

    // Flag to return the first object encountered when iterating the list.
    if (object == nullptr) {
        found = true;
    }

    LayerClass *layerptr = &Layers[LAYER_GROUND];

    for (int index = 0; index < Layers[LAYER_GROUND].Count(); ++index) {
        ObjectClass *next = Layers[LAYER_GROUND][index];

        if (next != nullptr && next->Is_Player_Army()) {
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
        Layers[layer].Submit(object, layer == LAYER_GROUND);
    }
}

void DisplayClass::Remove(ObjectClass *object, LayerType layer)
{
    if (object != nullptr) {
        Layers[layer].Remove(object);
    }
}

ObjectClass *DisplayClass::Cell_Object(cell_t cellnum, int x, int y) const
{
    return Array[cellnum].Cell_Object(x, y);
}

void DisplayClass::Select_These(coord_t start, coord_t finish)
{
    // Needs HouseClass.
#ifdef GAME_DLL
    void (*func)(const DisplayClass *, coord_t, coord_t) =
        reinterpret_cast<void (*)(const DisplayClass *, coord_t, coord_t)>(0x004B2C50);
    func(this, start, finish);
#endif
}

/**
 * Toggles the sell mode state.
 *
 * RA 0x004B4B68
 */
void DisplayClass::Sell_Mode_Control(int mode)
{
    bool flag = DisplaySellMode;

    switch (mode) {
        case MODE_CONTROL_TOGGLE:
            flag = !DisplaySellMode;
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

    if (flag != DisplaySellMode && PendingObjectTypePtr == nullptr) {
        DisplayRepairMode = false;

        if (flag && g_PlayerPtr->Has_Buildings()) {
            DisplaySellMode = true;
            Unselect_All();
        } else {
            DisplaySellMode = false;
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
    bool flag = DisplayRepairMode;

    switch (mode) {
        case MODE_CONTROL_TOGGLE:
            flag = !DisplayRepairMode;
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

    if (flag != DisplayRepairMode && PendingObjectTypePtr == nullptr) {
        DisplaySellMode = false;

        if (flag && g_PlayerPtr->Has_Buildings()) {
            DisplayRepairMode = true;
            Unselect_All();
        } else {
            DisplayRepairMode = false;
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
        int16_t x_lep = std::max((int16_t)(x - DisplayWidth / 2), Coord_Cell_To_Lepton(MapCellX));
        int16_t y_lep = std::max((int16_t)(y - DisplayHeight / 2), Coord_Cell_To_Lepton(MapCellY));
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
        cell = Click_Cell_Calc(g_mouse->Get_Mouse_X(), g_mouse->Get_Mouse_Y());
    }

    if (DisplayCursorOccupy != nullptr) {
        int occupy_x;
        int occupy_y;

        Get_Occupy_Dimensions(occupy_x, occupy_y, DisplayCursorOccupy);

        int cell_x = std::max(Cell_Get_X(DisplayCursorEnd + cell), Coord_Cell_X(DisplayPos));
        int cell_y = std::max(Cell_Get_Y(DisplayCursorEnd + cell), Coord_Cell_Y(DisplayPos));

        if (cell_x + occupy_x >= Coord_Lepton_X(DisplayPos) + Lepton_To_Cell_Coord(DisplayWidth)) {
            cell_x = Coord_Lepton_X(DisplayPos) + cell_x - occupy_x;
        }

        if (cell_y + occupy_y >= Coord_Lepton_Y(DisplayPos) + Lepton_To_Cell_Coord(DisplayHeight)) {
            cell_y = Coord_Lepton_Y(DisplayPos) + cell_y - occupy_y;
        }

        retval = Cell_From_XY(cell_x, cell_y) - DisplayCursorEnd;

        if (retval != DisplayCursorStart) {
            if (DisplayCursorOccupy != nullptr) {
                if (retval != DisplayCursorStart && DisplayCursorStart != -1) {
                    Cursor_Mark(DisplayCursorStart + DisplayCursorEnd, false);
                }

                if (retval != -1) {
                    Cursor_Mark(retval + DisplayCursorEnd, true);
                }
            }

            PassedProximityCheck = Passes_Proximity_Check(
                PendingObjectTypePtr, PendingObjectOwner, DisplayCursorOccupy, DisplayCursorEnd + retval);

            std::swap(DisplayCursorStart, retval);
        }
    } else {
        retval = DisplayCursorStart;
        DisplayCursorStart = cell;
    }

    return retval;
}

BOOL DisplayClass::Passes_Proximity_Check(ObjectTypeClass *object, HousesType house, int16_t *list, cell_t cell) const
{
    // Needs HouseClass, BuildingTypeClass.
#ifdef GAME_DLL
    BOOL(*func)
    (const DisplayClass *, ObjectTypeClass *, HousesType, int16_t *, cell_t) =
        reinterpret_cast<BOOL (*)(const DisplayClass *, ObjectTypeClass *, HousesType, int16_t *, cell_t)>(0x004AF7DC);
    return func(this, object, house, list, cell);
#else
    return false;
#endif
}

/**
 * @brief Redraw cell tiles.
 *
 * 0x004B1FD0
 */
void DisplayClass::Redraw_Icons()
{
    RedrawShadow = false;

    // Iterate over all cells currently in view and check if it needs to be redrawn.
    for (int16_t y = -Coord_Sub_Cell_Y(DisplayPos); y <= DisplayHeight; y += 256) {
        for (int16_t x = -Coord_Sub_Cell_X(DisplayPos); x <= DisplayWidth; x += 256) {
            cell_t cellnum = Coord_To_Cell(Coord_Add(DisplayPos, Coord_From_Lepton_XY(x, y)));
            coord_t coord = Cell_To_Coord(cellnum) & 0xFF00FF00;
            int draw_x = 0;
            int draw_y = 0;

            // Check conditions for current position warrant attempting a redraw.
            if (In_View(cellnum) && Is_Cell_Flagged(cellnum) && Coord_To_Pixel(coord, draw_x, draw_y)) {
                CellClass &cell = Array[Coord_To_Cell(coord)];

                if (cell.Is_Visible() || DebugUnshroud) {
                    cell.Draw_It(draw_x, draw_y);
                }

                if (!cell.Is_Revealed() && !DebugUnshroud) {
                    RedrawShadow = true;
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
    if (RedrawShadow) {
        // Iterate over every position within the tactical view and evaluate for a shadow redraw.
        for (int16_t y = -Coord_Sub_Cell_Y(DisplayPos); y <= DisplayHeight; y += 256) {
            for (int16_t x = -Coord_Sub_Cell_X(DisplayPos); x <= DisplayWidth; x += 256) {
                cell_t cellnum = Coord_To_Cell(Coord_Add(DisplayPos, Coord_From_Lepton_XY(x, y)));
                coord_t coord = Coord_Top_Left(Cell_To_Coord(cellnum));
                int draw_x = 0;
                int draw_y = 0;

                if (In_View(cellnum) && Is_Cell_Flagged(cellnum) && Coord_To_Pixel(coord, draw_x, draw_y)) {
                    CellClass &cell = Array[Coord_To_Cell(coord)];

                    if (!cell.Is_Revealed()) {
                        int frame = SHADOW_FULL;

                        if (cell.Is_Visible()) {
                            frame = Cell_Shadow(cellnum);
                        }

                        if (frame >= 0) { // If we have a valid frame, draw the shroud frame.
                            CC_Draw_Shape(
                                ShadowShapes, frame, draw_x, draw_y, WINDOW_TACTICAL, SHAPE_GHOST, nullptr, ShadowTrans);
                        } else if (frame != SHADOW_CLEAR) { // Otherwise, if we don't have -1 fill black
                            int w = CELL_PIXELS;
                            int h = CELL_PIXELS;

                            int x_clip = Lepton_To_Pixel(DisplayWidth);
                            int y_clip = Lepton_To_Pixel(DisplayHeight);

                            if (Clip_Rect(draw_x, draw_y, w, h, x_clip, y_clip) >= 0) {
                                g_logicPage->Fill_Rect(draw_x + TacOffsetX,
                                    draw_y + TacOffsetY,
                                    w + draw_x + TacOffsetX - 1,
                                    h + draw_y + TacOffsetY - 1,
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

    uint32_t cell = Coord_Top_Left(Cell_To_Coord(cellnum));
    uint32_t loc1 = Coord_Top_Left(DisplayPos);

    // Makes use of unsigned underflow to detect if we are greater than DisplayPos but within display dimensions in two
    // compares.
    if ((uint16_t)Coord_Lepton_X(cell) - (uint32_t)Coord_Lepton_X(loc1) <= (uint32_t)DisplayWidth + 255) {
        if ((uint16_t)Coord_Lepton_Y(cell) - (uint32_t)Coord_Lepton_Y(loc1) <= (uint32_t)DisplayHeight + 255) {
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
        int16_t view_x = Lepton_Round_To_Pixel(Coord_Lepton_X(DisplayPos));
        int16_t view_y = Lepton_Round_To_Pixel(Coord_Lepton_Y(DisplayPos));
        int16_t lep_x = Lepton_Round_To_Pixel(Coord_Lepton_X(coord)) + 512 - view_x;
        int16_t lep_y = Lepton_Round_To_Pixel(Coord_Lepton_Y(coord)) + 512 - view_y;

        if (lep_x <= (DisplayWidth + 1024) && lep_y <= (DisplayHeight + 1024)) {
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
    int x_pos = Pixel_To_Lepton(x - TacOffsetX);
    int y_pos = Pixel_To_Lepton(y - TacOffsetY);

    if (x_pos < DisplayWidth && y_pos < DisplayHeight) {
        return Coord_From_Lepton_XY(x_pos + Coord_Lepton_X(DisplayPos), y_pos + Coord_Lepton_Y(DisplayPos));
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
        CellClass const *cell = &Array[cellnum];

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
    int start_x = Coord_Lepton_X(DisplayPos);
    int start_y = Coord_Lepton_Y(DisplayPos);
    int end_x = start_x + DisplayWidth;
    int end_y = start_y + DisplayHeight;

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
        cell_num = Scen.Get_Waypoint(waypoint);
    }

    if (cell_num == -1) {
        cell_num = cellnum;
    }

    if (use_zone && cell_num != -1) {
        zone = Array[cell_num].Get_Zone(mzone);
    }

    bool y_clipped = false;
    bool x_clipped = false;

    int x = 0;
    int y = 0;

    // If we have a cell number from either the parameters or a waypoint, calculate an x and y position and ensure its within
    // the visible map. We also work out which edges we should clip to, x (east and west) or y (north and south).
    if (cell_num != -1) {
        int x_pos = std::min(Cell_Get_X(cell_num) - MapCellX, MapCellWidth + MapCellX - Cell_Get_X(cell_num));

        int y_pos = std::min(Cell_Get_Y(cell_num) - MapCellY, MapCellHeight + MapCellY - Cell_Get_Y(cell_num));

        if (x_pos > y_pos) {
            y_clipped = true;
            x_clipped = false;

            if (MapCellHeight / 2 <= Cell_Get_Y(cell_num) - MapCellY) {
                y = MapCellHeight;
            } else {
                y = -1;
            }

            x = Cell_Get_X(cell_num) - MapCellX;
        } else {
            y_clipped = false;
            x_clipped = true;

            if (MapCellWidth / 2 <= Cell_Get_X(cell_num) - MapCellX) {
                x = MapCellWidth;
            } else {
                x = -1;
            }

            y = Cell_Get_Y(cell_num) - MapCellY;
        }
    }

    // These won't be true if we didn't get a valid cell from the function parameters or a waypoint. In that case, we
    // randomly generate them based on the edge we are using as the source.
    if (!x_clipped && !y_clipped) {
        switch (source) {
            case SOURCE_EAST:
                x = MapCellWidth;
                y = Scen.Get_Random_Value(0, MapCellHeight - 1);
                break;

            case SOURCE_SOUTH:
                y_clipped = true;
                x = Scen.Get_Random_Value(0, MapCellWidth - 1);
                y = MapCellWidth;
                break;

            case SOURCE_WEST:
                x = -1;
                y = Scen.Get_Random_Value(0, MapCellHeight - 1);
                break;

            default:
                break;
        };
    }

    // Test cells along the left and right edges first.
    if (x_clipped && MapCellHeight > 0) {
        // Depending on edge, offset cell is always 1 cell toward the center of the map, applies to Y dimension as well
        // below.
        int offset = x <= MapCellX ? 1 : -1;
        int ny = y;

        for (int i = 0; i < MapCellHeight; ++i) {
            cell_t test_cell = Cell_From_XY(MapCellX + x, MapCellY + (ny % MapCellHeight));

            if (Good_Reinforcement_Cell(test_cell, test_cell + offset, speed, zone, mzone)) {
                return test_cell;
            }

            ++ny;
        }

        if (y_clipped && MapCellWidth > 0) {
            offset = y <= MapCellY ? 128 : -128;
            int nx = x;

            for (int i = 0; i < MapCellWidth; ++i) {
                cell_t test_cell = Cell_From_XY(MapCellX + (nx % MapCellWidth), MapCellY + y);

                if (Good_Reinforcement_Cell(test_cell, test_cell + offset, speed, zone, mzone)) {
                    return test_cell;
                }

                ++nx;
            }
        }
    } else if (y_clipped && MapCellWidth > 0) {
        int offset = y <= MapCellY ? 128 : -128;
        int nx = x;

        for (int i = 0; i < MapCellWidth; ++i) {
            cell_t test_cell = Cell_From_XY(MapCellX + (nx % MapCellWidth), MapCellY + y);

            if (Good_Reinforcement_Cell(test_cell, test_cell + offset, speed, zone, mzone)) {
                return test_cell;
            }

            ++nx;
        }
    }

    return Cell_From_XY(MapCellX + x, MapCellY + y);
}

/**
 * @brief Determines if a cell is a good reinforcement starting cell for moving into a cell offset from it.
 *
 * 0x004B2B90
 */
BOOL DisplayClass::Good_Reinforcement_Cell(cell_t cell1, cell_t cell2, SpeedType speed, int zone, MZoneType mzone) const
{
    if (Array[cell1].Is_Clear_To_Move(speed, false, false, zone, mzone)
        && Array[cell2].Is_Clear_To_Move(speed, false, false, zone, mzone)) {
        if (Array[cell1].Cell_Techno(0, 0) != nullptr) {
            return false;
        } else {
            return Array[cell2].Cell_Techno(0, 0) == nullptr;
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

    return Array[cellnum].Closest_Free_Spot(coord, skip_occupied);
}

/**
 * @brief Sets all objects to "look"?.
 *
 * 0x004B5680
 */
void DisplayClass::All_To_Look(BOOL skip_buildings)
{
    for (int i = 0; i < DisplayClass::Layers[LAYER_GROUND].Count(); ++i) {
        TechnoClass *objptr = reinterpret_cast<TechnoClass *>(DisplayClass::Layers[LAYER_GROUND][i]);

        if (objptr != nullptr && objptr->Is_Techno()) {
            if (objptr->What_Am_I() != RTTI_BUILDING || !skip_buildings) {
                if (objptr->Get_Owner_House()->Player_Has_Control()) {
                    if (objptr->Is_Player_Aware()) {
                        objptr->Look();
                    }
                } else if (objptr->What_Am_I() == RTTI_BUILDING) {
                    if (Rule.Ally_Reveal()) {
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
    for (int index = 0; index < DisplayClass::Layers[LAYER_GROUND].Count(); ++index) {
        ObjectClass *objptr = DisplayClass::Layers[LAYER_GROUND][index];

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
                if (Rule.Ally_Reveal()) {
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
    if (!DebugUnshroud) {
        // Check every cell and mark unrevealed cells
        for (int cellnum = 0; cellnum < MAP_MAX_AREA; ++cellnum) {
            if (In_Radar(cellnum)) {
                CellClass &cell = Array[cellnum];

                if (!cell.Is_Revealed()) {
                    if (cell.Is_Visible()) {
                        cell.Set_Bit128(true);
                    }
                }
            }
        }

        for (int cellnum = 0; cellnum < MAP_MAX_AREA; ++cellnum) {
            if (In_Radar(cellnum)) {
                CellClass &cell = Array[cellnum];

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
    if (!g_PlayerPtr->Is_Map_Clear() || (Array[cellnum].Get_Field_A() & (1 << g_PlayerPtr->What_Type()))) {
        if (In_Radar(cellnum)) {
            CellClass &cell = Array[cellnum];

            if (cell.Is_Visible()) {
                cell.Set_Visible(false);
                cell.Set_Revealed(false);
                cell.Redraw_Objects();

                for (FacingType facing = FACING_FIRST; facing < FACING_COUNT; ++facing) {
                    cell_t adjcell = Cell_Get_Adjacent(cellnum, facing);

                    if (adjcell != cellnum) {
                        Array[adjcell].Set_Revealed(false);
                    }

                    Array[adjcell].Redraw_Objects();
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
        Layers[layer].Render_All(true);
    }
}

void DisplayClass::Flag_All_Cells_To_Redraw()
{
    for (cell_t cellnum = 0; cellnum < MAP_MAX_AREA; ++cellnum) {
        if (!Is_Cell_Flagged(cellnum)) {
            CellRedraw[cellnum] = true;
        }
    }
}

void DisplayClass::Reset_Pending_Object()
{
    PendingObjectPtr = nullptr;
    PendingObjectTypePtr = nullptr;
    PendingObjectOwner = HOUSES_NONE;
    PendingSuper = SPECIAL_NONE;
}
