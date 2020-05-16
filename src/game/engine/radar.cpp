/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling radar (minimap when in game).
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "radar.h"
#include "audio.h"
#include "coord.h"
#include "display.h"
#include "drawshape.h"
#include "gamefile.h"
#include "globals.h"
#include "house.h"
#include "infantry.h"
#include "iomap.h"
#include "lists.h"
#include "mixfile.h"
#include "mouse.h"
#include "session.h"
#include "terrain.h"
#include "tileset.h"
#include "voc.h"
#include <algorithm>

#ifndef GAME_DLL
RadarClass::RTacticalClass RadarClass::s_RadarButton;
void *RadarClass::s_RadarAnim = nullptr;
void *RadarClass::s_RadarPulse = nullptr;
void *RadarClass::s_RadarFrame = nullptr;
BOOL RadarClass::s_FullRedraw;
GraphicBufferClass RadarClass::s_TileStage;
#endif

GraphicBufferClass RadarClass::s_IconStage(3, 3);

RadarClass::RTacticalClass::RTacticalClass() :
    GadgetClass(0, 0, 0, 0, MOUSE_LEFT_PRESS | MOUSE_LEFT_HELD | MOUSE_LEFT_RLSE | MOUSE_LEFT_UP | MOUSE_RIGHT_PRESS, true)
{
}

/**
 * Handles input intended for the RadarClass part of the IOMap stack.
 */
BOOL RadarClass::RTacticalClass::Action(unsigned flags, KeyNumType &key)
{
    if (g_Map.Is_Sidebar_Shown()) {
        g_Map.Help_Text(TXT_NULL);
    }

    if (g_Map.Radar_Active()) {
        if (g_Map.Radar_Draw_Names()) {
            GadgetClass::Action(INPUT_NONE, key);

            return true;
        }

        int mouse_x;
        int mouse_y;

        // If we have a button press flagged, get the location from the input handler. Otherwise just query mouse for pos.
        if (flags & (MOUSE_LEFT_PRESS | MOUSE_LEFT_RLSE | MOUSE_RIGHT_PRESS | MOUSE_RIGHT_RLSE)) {
            mouse_x = g_Keyboard->Get_MouseQX();
            mouse_y = g_Keyboard->Get_MouseQY();
        } else {
            mouse_x = g_Mouse->Get_Mouse_X();
            mouse_y = g_Mouse->Get_Mouse_Y();
        }

        if (g_Map.Get_Min_Radar_X() + g_Map.Get_Radar_Button_X() > mouse_x
            || g_Map.Get_Max_Radar_Width() + g_Map.Get_Radar_Button_X() <= mouse_x
            || g_Map.Get_Min_Radar_Y() + g_Map.Get_Radar_Button_Y() > mouse_y
            || g_Map.Get_Max_Radar_Height() + g_Map.Get_Radar_Button_Y() <= mouse_y) {
            g_Map.Override_Mouse_Shape(MOUSE_POINTER);

            return false;
        }

        int in_radar = g_Map.Click_In_Radar(mouse_x, mouse_y);
        cell_t click_cell = g_Map.Click_Cell_Calc(mouse_x, mouse_y);

        if (in_radar != 1 || click_cell == -1 || !g_Map.In_Radar(click_cell)) {
            if (in_radar == -1) {
                g_Map.Override_Mouse_Shape(MOUSE_POINTER, true);
            }

            GadgetClass::Action(INPUT_NONE, key);

            return true;
        }

        ObjectClass *cell_object = nullptr;

        if (g_Map[click_cell].Is_Visible() || g_DebugUnshroud) {
            cell_object = g_Map.Cell_Object(click_cell, 12, 12);
        }

        if (CurrentObjects.Count() == 0) {
            g_Map.Set_Default_Mouse(MOUSE_RADAR);

            if (flags & MOUSE_LEFT_PRESS) {
                click_cell = g_Map.Click_Cell_Calc(mouse_x, mouse_y);

                if (click_cell != -1) {
                    uint8_t x = std::max((Cell_Get_X(click_cell) - Lepton_To_Cell_Coord(g_Map.Get_DisplayWidth()) / 2),
                        g_Map.Get_Map_Cell_X());
                    uint8_t y = std::max((Cell_Get_Y(click_cell) - Lepton_To_Cell_Coord(g_Map.Get_DisplayHeight()) / 2),
                        g_Map.Get_Map_Cell_Y());
                    cell_t new_cell = Cell_From_XY(x, y);
                    g_Map.Set_Tactical_Position(Cell_To_Coord(new_cell));
                    click_cell = Coord_To_Cell(g_Map.New_Pos());
                    g_Map.Set_To_Redraw(true);
                    g_Map.Flag_To_Redraw(true);
                    g_Map.Set_Cursor_Frame(4);
                }
            }

            if (flags & MOUSE_RIGHT_PRESS) {
                g_Map.Zoom_Mode(click_cell);
            }

            if (in_radar == -1) {
                g_Map.Override_Mouse_Shape(MOUSE_POINTER, true);
            }

            GadgetClass::Action(INPUT_NONE, key);

            return true;
        }

        ActionType action;

        if (cell_object != nullptr) {
            action = CurrentObjects.Fetch_Head()->What_Action(cell_object);
        } else {
            action = CurrentObjects.Fetch_Head()->What_Action(click_cell);
        }

        switch (action) {
            case ACTION_MOVE:
            case ACTION_NO_MOVE:
            case ACTION_ENTER:
            case ACTION_SELF:
            case ACTION_ATTACK:
            case ACTION_CAPTURE:
            case ACTION_SABOTAGE:
                break;

            default:
                cell_object = nullptr;
                action = ACTION_NONE;
                break;
        }

        if (action == ACTION_NONE) {
            if (cell_object != nullptr && cell_object->Selected()) {
                cell_object = nullptr;
            } else {
                action = ACTION_NO_MOVE;
            }
        }

        if (flags & MOUSE_RIGHT_PRESS) {
            g_Map.Mouse_Right_Press();
        }

        if (flags & MOUSE_LEFT_UP) {
            g_Map.Mouse_Left_Up(-1, !g_Map[click_cell].Is_Visible() && !g_DebugUnshroud, cell_object, action, true);
        }

        if (flags & MOUSE_LEFT_PRESS) {
            g_Map.Mouse_Left_Release(click_cell, 12, 12, cell_object, action, true);
        }

        if (in_radar == -1) {
            g_Map.Override_Mouse_Shape(MOUSE_POINTER, true);
        }

        GadgetClass::Action(INPUT_NONE, key);

        return true;
    }

    if (g_Map.Is_Sidebar_Shown()) {
        g_Map.Override_Mouse_Shape(MOUSE_POINTER);
    }

    return false;
}

RadarClass::RadarClass() :
    m_RadarToRedraw(false),
    m_RadarCursorRedraw(false),
    m_RadarExists(false),
    m_RadarActive(false),
    m_RadarActivating(false),
    m_RadarDeactivating(false),
    m_RadarJammed(false),
    m_RadarPulseActive(false),
    m_RadarZoomed(true),
    m_RadarDrawNames(false),
    m_RadarDrawSpiedInfo(false),
    m_RadarPulseFrame(0),
    m_RadarCursorFrame(0),
    m_RadarAnimFrame(0),
    m_MiniMapXOffset(0),
    m_MiniMapYOffset(0),
    m_MiniMapCellWidth(0),
    m_MiniMapCellHeight(0),
    m_MiniMapCell(0),
    m_MiniMap(0, 0, 0, 0),
    m_MiniMapScale(0),
    m_SpiedHouse(HOUSES_NONE),
    m_MiniMapCellCount(0)
{
}

/**
 * Performs one time initialisation needed for the class.
 */
void RadarClass::One_Time()
{
    m_RadarButtonWidth = 160;
    m_RadarButtonHeight = 140;
    m_RadarButtonYPos = 14;
    m_field_CA8 = 128;
    m_field_CAC = 128;
    m_MinRadarX = 6;
    m_MinRadarY = 7;
    m_MaxRadarWidth = 146;
    m_MaxRadarHeight = 130;
    m_RadarButtonXPos = g_SeenBuff.Get_Width() - m_RadarButtonWidth;

    DisplayClass::One_Time();

    s_RadarButton.Set_Position(m_RadarButtonXPos, m_RadarButtonYPos);
    s_RadarButton.Set_Size(m_RadarButtonWidth, m_RadarButtonHeight);

    // Moved from Draw_It as its the same no matter what.
    s_RadarPulse = GameFileClass::Retrieve("pulse.shp");
}

/**
 * Clears the state of the class.
 */
void RadarClass::Init_Clear()
{
    DisplayClass::Init_Clear();

    m_RadarToRedraw = true;
    m_RadarCursorRedraw = true;
    m_RadarExists = false;
    m_RadarActive = false;
    m_RadarActivating = false;
    m_RadarDeactivating = false;
    m_RadarDrawNames = false;
    m_MiniMapCellCount = 0;

    if (m_MapCellWidth > 0 || m_MapCellHeight > 0) {
        m_RadarZoomed = false;
        Zoom_Mode(Coord_To_Cell(m_DisplayPos));
    }
}

/**
 * Performs a logic tick based in input from the player.
 */
void RadarClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
    if (m_RadarActive && g_Map.Is_Sidebar_Shown() && m_RadarCursorFrame > 0) {
        --m_RadarCursorFrame;
        m_RadarToRedraw = true;
        m_RadarCursorRedraw = true;
        Flag_To_Redraw();
    }

    if (m_RadarActivating) {
        if (m_RadarExists) {
            --m_RadarAnimFrame;

            if (m_RadarAnimFrame > 22) {
                m_RadarToRedraw = true;
                Flag_To_Redraw();
            } else {
                Radar_Activate(RADAR_3);
            }
        } else {
            ++m_RadarAnimFrame;

            if (m_RadarAnimFrame < 22) {
                m_RadarToRedraw = true;
                Flag_To_Redraw();
            } else {
                m_RadarExists = true;
                Radar_Activate(RADAR_3);
            }
        }
    }

    if (m_RadarDeactivating) {
        ++m_RadarAnimFrame;

        if (m_RadarAnimFrame == 41) {
            m_RadarDeactivating = false;
        } else {
            m_RadarToRedraw = true;
            Flag_To_Redraw();
        }
    }

    if (!m_RadarActivating && !m_RadarDeactivating && m_RadarJammed) {
        ++m_RadarAnimFrame;

        if (m_RadarAnimFrame < 22) {
            m_RadarAnimFrame = 22;
        }

        if (m_RadarAnimFrame > 25) {
            m_RadarAnimFrame = 22;
        }

        m_RadarToRedraw = true;
        Flag_To_Redraw();
    }

    if (m_RadarPulseActive) {
        Flag_To_Redraw(true);
        m_RadarToRedraw = true;

        if (m_RadarPulseFrame >= 8) {
            m_RadarPulseFrame = 0;
            m_RadarPulseActive = false;
        }
    }

    DisplayClass::AI(key, mouse_x, mouse_y);
}

/**
 * Draws the area of the screen controlled by the RadarClass portion of the IOMap object.
 */
void RadarClass::Draw_It(BOOL force_redraw)
{
    static HousesType _house = HOUSES_NONE;

    // in order of the houses.
    static char const *const _radarnames[HOUSES_PLAYABLE_COUNT] = { "natoradr.shp",
        "natoradr.shp",
        "ussrradr.shp",
        "natoradr.shp",
        "ussrradr.shp",
        "natoradr.shp",
        "natoradr.shp",
        "natoradr.shp",
        "natoradr.shp",
        "ussrradr.shp" };

    static char const *const _radarframes[HOUSES_PLAYABLE_COUNT] = { "nradrfrm.shp",
        "nradrfrm.shp",
        "uradrfrm.shp",
        "nradrfrm.shp",
        "uradrfrm.shp",
        "nradrfrm.shp",
        "nradrfrm.shp",
        "nradrfrm.shp",
        "nradrfrm.shp",
        "uradrfrm.shp" };

    DisplayClass::Draw_It(force_redraw);

    if (force_redraw || m_RadarToRedraw || s_FullRedraw) {
        if (g_PlayerPtr->Acts_Like() != _house) {
            HousesType side = g_PlayerPtr->Acts_Like();
            s_RadarAnim = GameMixFile::Retrieve(_radarnames[side]);
            s_RadarFrame = GameMixFile::Retrieve(_radarframes[side]);
            _house = g_PlayerPtr->Acts_Like();
        }

        if (m_RadarDrawNames) {
            Draw_Names();
            m_RadarToRedraw = false;
        } else if (!m_RadarDrawSpiedInfo || (m_RadarToRedraw = false, Draw_House_Info() == false)) {
            if (m_RadarActivating || m_RadarDeactivating || m_RadarJammed) {
                Radar_Anim();
                g_Map.Draw_Sidebar_Buttons();
                m_RadarToRedraw = false;
            } else if (g_Map.Is_Sidebar_Shown()) {
                if (m_RadarActive) {
                    if (force_redraw || !m_RadarToRedraw || s_FullRedraw || m_RadarPulseActive) {
                        GraphicViewPortClass *prevlogic = Set_Logic_Page(g_HidPage);
                        CC_Draw_Shape(s_RadarFrame, 1, m_RadarButtonXPos, m_RadarButtonYPos + 2);

                        if (m_MiniMap.Get_Left() != 0 || m_MiniMap.Get_Top() != 0) {
                            if (!m_RadarZoomed && m_MiniMap.Get_Left() != 0 && m_MiniMap.Get_Top() != 0
                                && (m_MaxRadarWidth - 1) > m_MiniMap.Get_Right()
                                && (m_MaxRadarHeight - 1) > m_MiniMap.Get_Bottom()) {
                                g_LogicPage->Draw_Rect(m_MinRadarX + m_RadarButtonXPos + m_MiniMap.Get_Left() - 1,
                                    m_MinRadarY + m_RadarButtonYPos + m_MiniMap.Get_Top() - 1,
                                    m_MinRadarX + m_RadarButtonXPos + m_MiniMap.Get_Left() + m_MiniMap.Get_Right(),
                                    m_MinRadarY + m_RadarButtonYPos + m_MiniMap.Get_Top() + m_MiniMap.Get_Bottom(),
                                    COLOR_WHITE);
                            }
                        } else {
                            g_LogicPage->Fill_Rect(m_RadarButtonXPos + m_MinRadarX,
                                m_RadarButtonYPos + m_MinRadarY,
                                m_RadarButtonXPos + m_MinRadarX + m_MaxRadarWidth,
                                m_RadarButtonYPos + m_MinRadarY + m_MaxRadarHeight - 1,
                                COLOR_BLACK);
                        }

                        if (g_LogicPage->Lock()) {
                            for (int i = 0; i < MAP_MAX_AREA; ++i) {
                                if (In_Radar(i) && Cell_On_Radar(i)) {
                                    Plot_Radar_Pixel(i);
                                }
                            }

                            if (m_RadarPulseActive) {
                                CC_Draw_Shape(s_RadarPulse,
                                    m_RadarPulseFrame++,
                                    m_MinRadarX + m_RadarButtonXPos,
                                    m_RadarButtonYPos + 2);
                            }

                            g_LogicPage->Unlock();
                        }

                        Radar_Cursor(true);
                        m_RadarToRedraw = false;
                        s_FullRedraw = false;
                        g_Map.Draw_Sidebar_Buttons();

                        if (prevlogic == &g_SeenBuff) {
                            g_Mouse->Hide_Mouse();

                            g_LogicPage->Blit(g_SeenBuff,
                                m_RadarButtonXPos,
                                m_RadarButtonYPos,
                                m_RadarButtonXPos,
                                m_RadarButtonYPos,
                                m_RadarButtonWidth,
                                m_RadarButtonHeight);

                            g_Mouse->Show_Mouse();
                        }

                        Set_Logic_Page(prevlogic);
                    } else {
                        m_RadarToRedraw = false;

                        if (m_MiniMapCellCount > 0) {
                            if (g_LogicPage->Lock()) {
                                for (int i = 0; i < m_MiniMapCellCount; ++i) {
                                    if (Cell_On_Radar(m_MiniMapCells[i])) {
                                        g_Map[m_MiniMapCells[i]].Set_Bit1(false);
                                        Plot_Radar_Pixel(m_MiniMapCells[i]);
                                        m_RadarCursorRedraw = g_Map[m_MiniMapCells[i]].Get_Bit32() || m_RadarCursorRedraw;
                                    }
                                }

                                g_LogicPage->Unlock();
                            }

                            if (m_MiniMapCellCount == MINI_MAP_CELLS) {
                                m_MiniMapCellCount = 0;

                                for (int i = 0; i < m_MapCellHeight && m_MiniMapCellCount < MINI_MAP_CELLS; ++i) {
                                    for (int j = 0; j < m_MapCellWidth && m_MiniMapCellCount < MINI_MAP_CELLS; ++j) {
                                        cell_t cellnum = Cell_From_XY(j + m_MapCellX, i + m_MapCellY);

                                        if (Cell_On_Radar(cellnum) && g_Map[cellnum].Get_Bit1()) {
                                            m_RadarToRedraw = true;
                                            m_MiniMapCells[m_MiniMapCellCount++] = cellnum;
                                        }
                                    }
                                }
                            } else {
                                m_MiniMapCellCount = 0;
                            }
                        }

                        Radar_Cursor(m_RadarCursorRedraw);
                    }

                } else {
                    CC_Draw_Shape(s_RadarAnim, m_RadarExists ? 41 : 0, m_RadarButtonXPos, m_RadarButtonYPos + 2);
                    s_FullRedraw = false;
                    m_RadarToRedraw = false;

                    if (g_Session.Game_To_Play() != GAME_CAMPAIGN) {
                        Fancy_Text_Print(HouseTypeClass::As_Reference(g_PlayerPtr->Acts_Like()).Full_Name(),
                            m_RadarButtonXPos + (m_RadarButtonWidth / 2),
                            m_RadarButtonHeight + m_RadarButtonYPos - 20,
                            &g_ColorRemaps[g_PlayerPtr->Player_Color()],
                            COLOR_TBLACK,
                            TPF_6PT_GRAD | TPF_NOSHADOW | TPF_SHADOW | TPF_CENTER);
                    }

                    g_Map.Draw_Sidebar_Buttons();
                }
            }
        }
    }
}

/**
 * Sets the maps playable dimensions.
 */
void RadarClass::Set_Map_Dimensions(int x, int y, int w, int h)
{
    Set_Radar_Position(Cell_From_XY(x, y));
    MapClass::Set_Map_Dimensions(x, y, w, h);
}

/**
 * Marks a cell as being visited by the given house, provided the house is spied by, allied with or is the player.
 */
BOOL RadarClass::Map_Cell(cell_t cellnum, HouseClass *house)
{
    captainslog_assert(cellnum < MAP_MAX_AREA);

    if (DisplayClass::Map_Cell(cellnum, house)) {
        Radar_Pixel(cellnum);

        return true;
    }

    return false;
}

/**
 * Determines which cell a click on screen corresponds to.
 */
cell_t RadarClass::Click_Cell_Calc(int x, int y) const
{
    switch (Click_In_Radar(x, y, true)) {
        case RADAR_CLICK_M1:
            return -1;

        case RADAR_CLICK_1:
            return Cell_From_XY(x, y);

        case RADAR_CLICK_0:
        default:
            return DisplayClass::Click_Cell_Calc(x, y);
    }
}

/**
 * Marks an area around a cell as needing a redraw.
 */
void RadarClass::Refresh_Cells(cell_t cellnum, const int16_t *list)
{
    captainslog_assert(cellnum < MAP_MAX_AREA);

    if (list != nullptr) {
        if (*list == LIST_START) {
            m_RadarToRedraw = true;
            Flag_To_Redraw();
        }
    }

    DisplayClass::Refresh_Cells(cellnum, list);
}

/**
 * Sets the position of the view port on the map.
 */
void RadarClass::Set_Tactical_Position(coord_t location)
{
    DisplayClass::Set_Tactical_Position(location);
    Set_Radar_Position(Coord_To_Cell(m_DisplayPos));
}

/**
 * Flags a cell.
 */
void RadarClass::Flag_Cell(cell_t cellnum)
{
    captainslog_assert(cellnum < MAP_MAX_AREA);
    DisplayClass::Flag_Cell(cellnum);
}

/**
 * Jam a cell for a given house.
 */
BOOL RadarClass::Jam_Cell(cell_t cellnum, HouseClass *house)
{
    m_Array[cellnum].Jam_House(house->What_Type());

    if (house != g_PlayerPtr) {
        Shroud_Cell(cellnum);
    }

    Radar_Pixel(cellnum);

    return true;
}

/**
 * Unjam a cell for a given house.
 */
BOOL RadarClass::UnJam_Cell(cell_t cellnum, HouseClass *house)
{
    m_Array[cellnum].Redraw_Objects();
    m_Array[cellnum].Unjam_House(house->What_Type());
    Radar_Pixel(cellnum);

    return true;
}

/**
 * Check if the radar is jammed for the player.
 */
BOOL RadarClass::Is_Radar_Jammed()
{
    if (g_PlayerPtr->Is_Map_Clear()) {
        return false;
    }

    return m_RadarJammed;
}

/**
 * Check if the radar is active.
 */
BOOL RadarClass::Is_Radar_Active()
{
    return m_RadarActive || g_PlayerPtr->Is_Map_Clear();
}

/**
 * Check if the radar exists.
 */
BOOL RadarClass::Is_Radar_Existing()
{
    return m_RadarExists || g_PlayerPtr->Is_Map_Clear();
}

/**
 * Activates the radar pulse animation.
 */
void RadarClass::Activate_Pulse()
{
    if (Is_Radar_Active()) {
        m_RadarPulseFrame = 0;
        m_RadarPulseActive = true;
    }
}

/**
 * Marks a cell as on the radar?
 */
void RadarClass::Radar_Pixel(cell_t cellnum)
{
    if (m_RadarActive && g_Map.Is_Sidebar_Shown() && Cell_On_Radar(cellnum)) {
        m_RadarToRedraw = true;
        m_Array[cellnum].Set_Bit1(true);

        if (m_MiniMapCellCount < MINI_MAP_CELLS) {
            m_MiniMapCells[m_MiniMapCellCount++] = cellnum;
        }
    }
}

/**
 * Renders terrain objects to the minimap.
 */
void RadarClass::Render_Terrain(cell_t cellnum, int x, int y, int scale)
{
    captainslog_assert(cellnum < MAP_MAX_AREA);
    ObjectClass *objects[OVERLAPPER_COUNT + 1] = { nullptr };
    int object_count = 0;

    ObjectClass *occupier = m_Array[cellnum].Get_Occupier();

    if (occupier != nullptr && occupier->What_Am_I() == RTTI_TERRAIN) {
        objects[object_count++] = occupier;
    }

    for (int i = 0; i < OVERLAPPER_COUNT; ++i) {
        ObjectClass *overlap = m_Array[cellnum].Get_Overlapper(i);

        if (overlap != nullptr && overlap->What_Am_I() == RTTI_TERRAIN) {
            objects[object_count++] = overlap;
        }
    }

    if (object_count > 0) {
        if (scale == 1) {
            g_LogicPage->Put_Pixel(x, y, 21);
        } else {
            for (int i = 0; i < object_count - 1; ++i) {
                for (int j = i + 1; j < object_count; ++j) {
                    if (ObjectClass::Sort_Y_Greater_Than(objects[i], objects[j])) {
                        std::swap(objects[i], objects[j]);
                    }
                }
            }

            // Render loop for objects
            for (int index = 0; index < object_count; ++index) {
                uint8_t *icondata = reinterpret_cast<TerrainClass *>(objects[index])->Radar_Icon(cellnum);

                if (icondata != nullptr) {
                    s_IconStage.From_Buffer(0, 0, 3, 3, icondata);
                    s_IconStage.Scale(
                        *g_LogicPage, 0, 0, x, y, 3, 3, m_MiniMapScale, m_MiniMapScale, true, s_FadingBrighten);
                }
            }
        }
    }
}

/**
 * Renders techno class objects to the minimap.
 */
void RadarClass::Render_Infantry(cell_t cellnum, int x, int y, int scale)
{
    for (TechnoClass *occupier = reinterpret_cast<TechnoClass *>(m_Array[cellnum].Get_Occupier()); occupier != nullptr;
         occupier = reinterpret_cast<TechnoClass *>(occupier->Get_Next())) {
        if (!occupier->Is_Techno() || !occupier->Is_Visible_On_Radar()) {
            continue;
        }

        uint8_t col = g_ColorRemaps[occupier->Get_Owner_House()->Player_Color()].WindowPalette[7];
        int third = std::max(1, scale / 3);
        int hw = scale + 1; // height/width

        switch (occupier->What_Am_I()) {
            case RTTI_INFANTRY: {
                int cellx = std::clamp((Coord_Sub_Cell_X(occupier->Get_Coord()) / (256 / hw)) - third / 2, 0, scale - third);
                int celly = std::clamp((Coord_Sub_Cell_Y(occupier->Get_Coord()) / (256 / hw)) - third / 2, 0, scale - third);

                if (reinterpret_cast<InfantryClass *>(occupier)->What_Type() == INFANTRY_SPY) {
                    col = g_ColorRemaps[g_PlayerPtr->Player_Color()].WindowPalette[7];
                }

                g_LogicPage->Fill_Rect(cellx + x, celly + y, cellx + x + third - 1, celly + y + third - 1, col);
            } break;

            case RTTI_BUILDING: // fall through
            case RTTI_UNIT: // fall through
            case RTTI_VESSEL: // fall through
            case RTTI_AIRCRAFT:
                g_LogicPage->Fill_Rect(x, y, scale + x - 1, scale + y - 1, col);
                break;

            default:
                break;
        }
    }
}

/**
 * Renders overlay to the minimap.
 */
void RadarClass::Render_Overlay(cell_t cellnum, int x, int y, int scale)
{
    captainslog_assert(cellnum < MAP_MAX_AREA);
    CellClass &cell = m_Array[cellnum];

    if (cell.Get_Overlay() != OVERLAY_NONE) {
        OverlayTypeClass &optr = OverlayTypeClass::As_Reference(cell.Get_Overlay());

        if (optr.Is_Radar_Visible()) {
            uint8_t *icondata = optr.Radar_Icon(cell.Get_Overlay_Frame());

            if (icondata != nullptr) {
                s_IconStage.From_Buffer(0, 0, 3, 3, icondata);

                if (optr.Is_Ore()) {
                    if (scale == 1) {
                        g_LogicPage->Put_Pixel(x, y, COLOR_GREY);
                    } else {
                        s_IconStage.Scale(*g_LogicPage, 0, 0, x, y, 3, 3, scale, scale, true, s_FadingYellow);
                    }
                }
            }
        }
    }
}

/**
 * Sets the zoom mode and moves the displayed part of the map to cellnum.
 */
void RadarClass::Zoom_Mode(cell_t cellnum)
{
    if (Is_Zoomable()) {
        m_RadarZoomed = !m_RadarZoomed;
    } else {
        m_RadarZoomed = true;
    }

    m_MiniMap.m_left = 0;
    m_MiniMap.m_top = 0;

    int width;
    int height;

    if (m_RadarZoomed) {
        m_MiniMapScale = 3;
        width = m_MaxRadarWidth / m_MiniMapScale;
        height = m_MaxRadarHeight / m_MiniMapScale;
    } else {
        int xscale = m_MaxRadarWidth / m_MapCellWidth;
        int yscale = m_MaxRadarHeight / m_MapCellHeight;
        m_MiniMapScale = std::clamp(std::min(xscale, yscale), 1, yscale);
        width = m_MapCellWidth;
        height = m_MapCellHeight;
    }

    width = std::min(std::min(width, m_MaxRadarWidth), m_MapCellWidth);
    height = std::min(std::min(height, m_MaxRadarHeight), m_MapCellHeight);

    int xremain = m_MaxRadarWidth - m_MiniMapScale * width;
    int yremain = m_MaxRadarHeight - m_MiniMapScale * height;

    m_MiniMap.m_left = xremain / 2;
    m_MiniMap.m_top = yremain / 2;
    m_MiniMap.m_right = m_MaxRadarWidth - xremain;
    m_MiniMap.m_bottom = m_MaxRadarHeight - yremain;
    m_MiniMapCellWidth = width;
    m_MiniMapCellHeight = height;

    Set_Radar_Position(cellnum);
    m_RadarToRedraw = true;
    Flag_To_Redraw();
    s_FullRedraw = true;
}

/**
 * Checks if it makes sense to allow radar map zooming given the current map size.
 */
BOOL RadarClass::Is_Zoomable() const
{
    int wratio = m_MaxRadarWidth / m_MapCellWidth;
    int hratio = m_MaxRadarHeight / m_MapCellHeight;

    return std::max(1, std::min(wratio, hratio)) != 3;
}

/**
 * Checks if a click falls within the radar.
 */
int RadarClass::Click_In_Radar(int &x, int &y, BOOL set_coords) const
{
    // TODO: make sidebar check virtual so don't need global object.
    if (g_Map.Is_Sidebar_Shown() && m_RadarActive) {
        unsigned x_diff = x - (m_MinRadarX + m_RadarButtonXPos);
        unsigned y_diff = y - (m_MinRadarY + m_RadarButtonYPos);

        // Abuses unsigned underflow to check if within bounds or negative in fewer compares.
        if (x_diff < (unsigned)m_MaxRadarWidth && y_diff < (unsigned)m_MaxRadarHeight) {
            int xpos = x - (m_MinRadarX + m_RadarButtonXPos) - m_MiniMap.m_left;
            int ypos = y - (m_MinRadarY + m_RadarButtonYPos) - m_MiniMap.m_top;
            int scale = m_MiniMapScale - 1;

            if ((unsigned)xpos >= (unsigned)(scale + m_MiniMap.m_right)
                || (unsigned)ypos >= (unsigned)(scale + m_MiniMap.m_bottom)) {
                return -1;
            } else {
                if (set_coords) {
                    x = xpos / m_MiniMapScale + m_MiniMapXOffset;
                    y = ypos / m_MiniMapScale + m_MiniMapYOffset;
                }

                return 1;
            }
        }
    }

    return 0;
}

/**
 * Checks if a cell is currently shown on the radar.
 */
BOOL RadarClass::Cell_On_Radar(cell_t cellnum)
{
    if (cellnum > MAP_MAX_AREA) {
        return false;
    }

    if (!m_RadarZoomed) {
        return true;
    }

    if (Cell_Get_X(cellnum) - m_MiniMapXOffset > m_MiniMapCellWidth
        || Cell_Get_Y(cellnum) - m_MiniMapYOffset > m_MiniMapCellHeight) {
        return false;
    }

    return true;
}

/**
 * Moves the radar view to a given cell.
 */
void RadarClass::Set_Radar_Position(cell_t cellnum)
{
    int xpos;
    int ypos;

    // Calculate X and Y pos and confine to map, recalc cellnum based on confined pos.
    if (m_MiniMapScale == 1) {
        xpos = 0;
        ypos = 0;
    } else {
        xpos = Cell_Get_X(cellnum) - m_MapCellX;
        ypos = Cell_Get_Y(cellnum) - m_MapCellY;
    }

    Confine_Rect(xpos, ypos, m_MiniMapCellWidth, m_MiniMapCellHeight, m_MapCellWidth, m_MapCellHeight);
    xpos += m_MapCellX;
    ypos += m_MapCellY;
    cellnum = Cell_From_XY(xpos, ypos);

    if (cellnum == m_MiniMapCell) {
        m_RadarToRedraw = m_RadarCursorRedraw = m_RadarActive;
        Flag_To_Redraw();
    } else {
        int diffx = Cell_Get_X(m_MiniMapCell) - xpos;
        int diffy = Cell_Get_Y(m_MiniMapCell) - ypos;
        BOOL neg_move = false;

        m_MiniMapXOffset = xpos;
        m_MiniMapYOffset = ypos;
        m_MiniMapCell = cellnum;

        if (g_Map.Is_Sidebar_Shown() && m_RadarActive) {
            int movex = m_MiniMapCellWidth - std::abs(diffx);
            int movey = m_MiniMapCellHeight - std::abs(diffy);

            if (movex < 1 || movey < 1) {
                neg_move = true;
            }

            // Do a linear blit to move the unchanged part of the map.
            if (!neg_move && (movex != m_MiniMap.Get_Right() || movey != m_MiniMap.Get_Bottom())) {
                int dst_adjustx = diffx >= 0 ? diffx : 0;
                int dst_adjusty = diffy >= 0 ? diffy : 0;
                int src_adjustx = diffx >= 0 ? 0 : -diffx;
                int src_adjusty = diffy >= 0 ? 0 : -diffy;

                if (!g_OverlappedVideoBlits && g_HidPage.In_Video_Memory()) {
                    GraphicBufferClass tmp;
                    tmp.Init(
                        (m_MiniMap.Get_Right() + 16) & ~16, (m_MiniMap.Get_Bottom() + 16) & ~16, nullptr, 0, GBC_VIDEO_MEM);
                    g_HidPage.Blit(tmp,
                        m_MiniMap.Get_Left() + m_MinRadarX + m_RadarButtonXPos + m_MiniMapScale * src_adjustx,
                        m_MiniMap.Get_Top() + m_MinRadarY + m_RadarButtonYPos + m_MiniMapScale * src_adjusty,
                        0,
                        0,
                        m_MiniMap.Get_Right(),
                        m_MiniMap.Get_Bottom());
                    tmp.Blit(g_HidPage,
                        0,
                        0,
                        m_MiniMap.Get_Left() + m_MinRadarX + m_RadarButtonXPos + m_MiniMapScale * dst_adjustx,
                        m_MiniMap.Get_Top() + m_MinRadarY + m_RadarButtonYPos + m_MiniMapScale * dst_adjusty,
                        movex * m_MiniMapScale,
                        movey * m_MiniMapScale);
                } else {
                    g_HidPage.Blit(g_HidPage,
                        m_MiniMap.Get_Left() + m_MinRadarX + m_RadarButtonXPos + m_MiniMapScale * src_adjustx,
                        m_MiniMap.Get_Top() + m_MinRadarY + m_RadarButtonYPos + m_MiniMapScale * src_adjusty,
                        m_MiniMap.Get_Left() + m_MinRadarX + m_RadarButtonXPos + m_MiniMapScale * dst_adjustx,
                        m_MiniMap.Get_Top() + m_MinRadarY + m_RadarButtonYPos + m_MiniMapScale * dst_adjusty,
                        movex * m_MiniMapScale,
                        movey * m_MiniMapScale);
                }

                // Draw the changed rows
                if (diffx != 0) {
                    int sx = movex;
                    int fx = diffx;

                    if (diffx >= 0) {
                        sx = 0;
                    } else {
                        fx = movex + std::abs(diffx);
                    }

                    for (int x = sx; x < fx; ++x) {
                        for (int y = 0; y < m_MiniMapCellHeight; ++y) {
                            Radar_Pixel(Cell_From_XY(x + xpos, y + ypos));
                        }
                    }
                }

                // Draw the changed columns
                if (ypos != 0) {
                    int sy = movey;
                    int fy = diffy;

                    if (diffy >= 0) {
                        sy = 0;
                    } else {
                        fy = movey + std::abs(diffy);
                    }

                    for (int y = sy; y < fy; ++y) {
                        for (int x = 0; x < m_MiniMapCellWidth; ++x) {
                            Radar_Pixel(Cell_From_XY(x + xpos, y + ypos));
                        }
                    }
                }
            }
        }

        m_RadarToRedraw = m_RadarCursorRedraw = m_RadarActive;
        Flag_To_Redraw();

        if (m_MiniMapScale > 4) {
            s_FullRedraw = neg_move;
        }
    }
}

/**
 * Returns the cell that the radar is currently positioned over.
 */
cell_t RadarClass::Radar_Position()
{
    return m_MiniMapCell;
}

/**
 * Converts a cell position into a pixel position on the radar.
 */
void RadarClass::Cell_XY_To_Radar_Pixel(int cell_x, int cell_y, int &x, int &y)
{
    x = m_MiniMapScale * (cell_x - m_MiniMapXOffset);
    y = m_MiniMapScale * (cell_y - m_MiniMapYOffset);
}

/**
 * Draws the current frame for the radar activate/deactivate animation.
 */
void RadarClass::Radar_Anim()
{
    if (!m_RadarDrawNames && g_Map.Is_Sidebar_Shown()) {
        GraphicViewPortClass *old_vp = Set_Logic_Page(&g_HidPage);
        GraphicViewPortClass vp(g_LogicPage->Get_Graphic_Buffer(),
            m_MinRadarX + m_RadarButtonXPos + g_LogicPage->Get_XPos(),
            g_LogicPage->Get_YPos() + m_RadarButtonYPos + m_MinRadarY,
            m_MaxRadarWidth,
            m_MaxRadarHeight);
        Draw_Box(m_MinRadarX + m_RadarButtonXPos - 1,
            m_MinRadarY + m_RadarButtonYPos - 1,
            m_MaxRadarWidth + 2,
            m_MaxRadarHeight + 2,
            BOX_STYLE_1,
            true);
        vp.Clear();
        CC_Draw_Shape(s_RadarAnim, m_RadarAnimFrame, m_RadarButtonXPos, m_RadarButtonYPos + 2);
        Flag_To_Redraw();
        Set_Logic_Page(old_vp);
    }
}

/**
 * Draws the radar reticle cursor indicating currently viewed area.
 */
void RadarClass::Radar_Cursor(BOOL redraw)
{
    static cell_t _last_pos = -1;
    static int _last_frame = -1;

    cell_t cell = Coord_To_Cell(m_DisplayPos);
    int cell_x = Cell_Get_X(cell);
    int cell_y = Cell_Get_Y(cell);
    int cell_w = 0;
    int cell_h = 0;

    if (cell == -1 || cell != _last_pos || m_RadarCursorFrame != _last_frame || redraw) {
        int pixel_left = 0;
        int pixel_top = 0;
        int pixel_right = 0;
        int pixel_btm = 0;

        // Unmarks cells marked in the last frame if it changed.
        if (_last_pos != -1) {
            int last_x = Cell_Get_X(_last_pos);
            int last_y = Cell_Get_Y(_last_pos);

            Cell_XY_To_Radar_Pixel(last_x, last_y, pixel_left, pixel_top);

            // Add 1 to the dimensions if the position lies more than half way into another cell.
            cell_w = last_x + Lepton_To_Cell_Coord(m_DisplayWidth);
            cell_h = last_y + Lepton_To_Cell_Coord(m_DisplayHeight);

            Cell_XY_To_Radar_Pixel(cell_w, cell_h, pixel_right, pixel_btm);

            --pixel_right;
            --pixel_btm;

            pixel_left -= _last_frame;
            pixel_top -= _last_frame;
            pixel_right += _last_frame;
            pixel_btm += _last_frame;

            Mark_Radar(pixel_left, pixel_top, pixel_right, pixel_btm, false, RADAR_CURSOR_SIZE);
        }

        // Get extents of the cursor and mark any cells where their pixel will be covered by the cursor and so shouldn't be
        // redrawn.
        Cell_XY_To_Radar_Pixel(cell_x, cell_y, pixel_left, pixel_top);
        Cell_XY_To_Radar_Pixel(cell_x + Lepton_To_Cell_Coord(m_DisplayWidth),
            cell_y + Lepton_To_Cell_Coord(m_DisplayHeight),
            pixel_right,
            pixel_btm);

        --pixel_right;
        --pixel_btm;

        pixel_left -= m_RadarCursorFrame;
        pixel_top -= m_RadarCursorFrame;
        pixel_right += m_RadarCursorFrame;
        pixel_btm += m_RadarCursorFrame;

        Mark_Radar(pixel_left, pixel_top, pixel_right, pixel_btm, true, RADAR_CURSOR_SIZE);
        GraphicViewPortClass *old = Set_Logic_Page(&g_HidPage);

        GraphicViewPortClass gvp(g_LogicPage->Get_Graphic_Buffer(),
            g_LogicPage->Get_XPos() + m_MinRadarX + m_RadarButtonXPos + m_MiniMap.m_left,
            g_LogicPage->Get_YPos() + m_MinRadarY + m_RadarButtonYPos + m_MiniMap.m_top,
            m_MiniMap.m_right,
            m_MiniMap.m_bottom);

        // Top left
        gvp.Draw_Line(pixel_left, pixel_top, pixel_left + RADAR_CURSOR_SIZE, pixel_top, COLOR_LTGREEN);
        gvp.Draw_Line(pixel_left, pixel_top, pixel_left, pixel_top + RADAR_CURSOR_SIZE, COLOR_LTGREEN);

        // Top right
        gvp.Draw_Line(pixel_right - RADAR_CURSOR_SIZE, pixel_top, pixel_right, pixel_top, COLOR_LTGREEN);
        gvp.Draw_Line(pixel_right, pixel_top, pixel_right, pixel_top + RADAR_CURSOR_SIZE, COLOR_LTGREEN);

        // Bottom left
        gvp.Draw_Line(pixel_left, pixel_btm - RADAR_CURSOR_SIZE, pixel_left, pixel_btm, COLOR_LTGREEN);
        gvp.Draw_Line(pixel_left, pixel_btm, pixel_left + RADAR_CURSOR_SIZE, pixel_btm, COLOR_LTGREEN);

        // Bottom right
        gvp.Draw_Line(pixel_right, pixel_btm - RADAR_CURSOR_SIZE, pixel_right, pixel_btm, COLOR_LTGREEN);
        gvp.Draw_Line(pixel_right - RADAR_CURSOR_SIZE, pixel_btm, pixel_right, pixel_btm, COLOR_LTGREEN);

        Set_Logic_Page(old);

        _last_pos = cell;
        _last_frame = m_RadarCursorFrame;

        m_RadarCursorRedraw = false;
    }
}

/**
 * Sets whether player names should be drawn.
 */
void RadarClass::Player_Names(BOOL draw)
{
    m_RadarDrawNames = draw;
    Flag_To_Redraw(true);
}

/**
 * Draws info on a spied house.
 */
BOOL RadarClass::Draw_House_Info()
{
    if (g_Map.Is_Sidebar_Shown()) {
        int x = m_MinRadarX + m_RadarButtonXPos + 12;
        int y = m_MinRadarY + m_RadarButtonYPos + 4;
        CC_Draw_Shape(s_RadarFrame, 1, m_RadarButtonXPos, m_RadarButtonYPos + 2);
        g_Map.Draw_Sidebar_Buttons();
        Fancy_Text_Print(TXT_SPY_INFO, x, y, &g_ColorRemaps[REMAP_5], COLOR_TBLACK, TPF_6PT_GRAD | TPF_NOSHADOW);
        y += 4;

        HouseClass *house = HouseClass::As_Pointer(m_SpiedHouse);

        if (house != nullptr && house->Spied_My_Radar(house)) {
            char buffer[11];
            RemapControlType *housecolor = &g_ColorRemaps[house->Player_Color()];
            snprintf(buffer, sizeof(buffer), "%s", house->Player_Name());

            if (strlen(buffer) == 0) {
                snprintf(buffer, sizeof(buffer), "%s", "________");
            }

            if (strlen(buffer) > 9) {
                buffer[9] = '.';
                buffer[10] = '\0';
            }

            Fancy_Text_Print(buffer, x, y, housecolor, COLOR_BLACK, TPF_6PT_GRAD | TPF_NOSHADOW | TPF_USE_GRAD_PAL);
            y += 13;

            // Building info
            Fancy_Text_Print(TXT_INFO_BUILDINGS, x, y, &g_ColorRemaps[REMAP_5], COLOR_TBLACK, TPF_6PT_GRAD | TPF_NOSHADOW);
            snprintf(buffer, sizeof(buffer), "%d", house->Currently_Owned_Building_Count());
            y += 13;
            Fancy_Text_Print(buffer, x, y, &g_ColorRemaps[REMAP_5], COLOR_TBLACK, TPF_6PT_GRAD | TPF_NOSHADOW);
            y += 13;

            // Unit info
            Fancy_Text_Print(TXT_INFO_UNITS, x, y, &g_ColorRemaps[REMAP_5], COLOR_TBLACK, TPF_6PT_GRAD | TPF_NOSHADOW);
            snprintf(buffer, sizeof(buffer), "%d", house->Currently_Owned_Unit_Count());
            y += 13;
            Fancy_Text_Print(buffer, x, y, &g_ColorRemaps[REMAP_5], COLOR_TBLACK, TPF_6PT_GRAD | TPF_NOSHADOW);
            y += 13;

            // Infantry info
            Fancy_Text_Print(TXT_INFO_INFANTRY, x, y, &g_ColorRemaps[REMAP_5], COLOR_TBLACK, TPF_6PT_GRAD | TPF_NOSHADOW);
            snprintf(buffer, sizeof(buffer), "%d", house->Currently_Owned_Infantry_Count());
            y += 13;
            Fancy_Text_Print(buffer, x, y, &g_ColorRemaps[REMAP_5], COLOR_TBLACK, TPF_6PT_GRAD | TPF_NOSHADOW);

            return true;
        }
    }

    return false;
}

/**
 * Draws player names and their kill counts.
 */
void RadarClass::Draw_Names()
{
    if (g_Map.Is_Sidebar_Shown()) {
        CC_Draw_Shape(s_RadarFrame, 1, m_RadarButtonXPos, m_RadarButtonYPos + 2);

        Fancy_Text_Print(TXT_NAME_COLON,
            m_RadarButtonXPos + m_MinRadarX,
            m_RadarButtonYPos + m_MinRadarY,
            &g_ColorRemaps[REMAP_5],
            COLOR_TBLACK,
            TPF_6PT_GRAD | TPF_NOSHADOW);

        Fancy_Text_Print(TXT_KILLS_COLON,
            m_RadarButtonXPos + m_MinRadarX + m_MaxRadarWidth - 2,
            m_RadarButtonYPos + m_MinRadarY,
            &g_ColorRemaps[REMAP_5],
            COLOR_TBLACK,
            TPF_6PT_GRAD | TPF_NOSHADOW | TPF_RIGHT);

        g_LogicPage->Draw_Line(m_RadarButtonXPos + m_MinRadarX,
            m_RadarButtonYPos + m_MinRadarY + 13,
            m_RadarButtonXPos + m_MinRadarX + m_MaxRadarWidth - 1,
            m_RadarButtonYPos + m_MinRadarY + 13,
            COLOR_LTGREY);

        int y = m_RadarButtonYPos + m_MinRadarY + 17;

        for (HousesType house = HOUSES_MULTI_1; house < g_Session.MPlayer_Max() + HOUSES_MULTI_1; ++house) {
            HouseClass *hptr = HouseClass::As_Pointer(house);

            if (hptr != nullptr) {
                RemapControlType *remap;
                TextPrintType tpt;

                if (hptr->Is_Defeated()) {
                    remap = &g_GreyScheme;
                    tpt = TPF_6PT_GRAD | TPF_NOSHADOW;
                } else {
                    remap = &g_ColorRemaps[hptr->Player_Color()];
                    tpt = TPF_6PT_GRAD | TPF_NOSHADOW | TPF_USE_GRAD_PAL;
                }

                char const *name;

                if (hptr->Is_Human()) {
                    name = hptr->Player_Name();
                } else {
                    name = Text_String(TXT_COMPUTER);
                }

                char namebuff[11];

                snprintf(namebuff, sizeof(namebuff), "%s", name);

                if (strlen(namebuff) == 0) {
                    strlcpy(namebuff, "________", sizeof(namebuff));
                }

                if (strlen(namebuff) > 9) {
                    namebuff[9] = '.';
                    namebuff[10] = '\0';
                }

                Fancy_Text_Print(namebuff, m_RadarButtonXPos + m_MinRadarX, y, remap, COLOR_TBLACK, tpt);
                int kills = 0;

                for (HousesType i = HOUSES_FIRST; i < HOUSES_COUNT; ++i) {
                    kills += hptr->Get_Units_Destroyed(i);
                    kills += hptr->Get_Buildings_Destroyed(i);
                }

                snprintf(namebuff, sizeof(namebuff), "%2d", kills);

                Fancy_Text_Print(namebuff,
                    m_RadarButtonXPos + m_MinRadarX + m_MaxRadarWidth - 2,
                    y,
                    remap,
                    COLOR_TBLACK,
                    tpt | TPF_RIGHT);

                y += 13;
            }
        }

        g_Map.Draw_Sidebar_Buttons();
    }
}

/**
 * Marks and unmarks all affected cells as being under the radar reticle cursor and so potentially needing redraws.
 */
void RadarClass::Mark_Radar(int left, int top, int right, int bottom, BOOL mark, int size)
{
    int l_adj = (left / m_MiniMapScale) + m_MiniMapXOffset;
    int t_adj = (top / m_MiniMapScale) + m_MiniMapYOffset;
    int r_adj = (right / m_MiniMapScale) + m_MiniMapXOffset;
    int b_adj = (bottom / m_MiniMapScale) + m_MiniMapYOffset;
    int scaling = (size / m_MiniMapScale) + 1;

    for (int index = 0; index < scaling; ++index) {
        Cursor_Cell(Cell_From_XY(index + l_adj, t_adj), mark);
        Cursor_Cell(Cell_From_XY(index + l_adj, b_adj), mark);
        Cursor_Cell(Cell_From_XY(r_adj - index, t_adj), mark);
        Cursor_Cell(Cell_From_XY(r_adj - index, b_adj), mark);
        Cursor_Cell(Cell_From_XY(l_adj, index + t_adj), mark);
        Cursor_Cell(Cell_From_XY(r_adj, index + t_adj), mark);
        Cursor_Cell(Cell_From_XY(l_adj, b_adj - index), mark);
        Cursor_Cell(Cell_From_XY(r_adj, b_adj - index), mark);
    }
}

/**
 * Marks and unmarks a cell as being under the radar reticle cursor and so potentially needing redraws.
 */
void RadarClass::Cursor_Cell(cell_t cellnum, BOOL mark)
{
    if (Cell_On_Radar(cellnum)) {
        CellClass &cell = m_Array[cellnum];

        // If the cell isn't already marked the same way we want, change it and redraw if situation warrants.
        if (cell.Get_Bit32() != mark) {
            cell.Set_Bit32(mark);

            if (!mark) {
                Plot_Radar_Pixel(cellnum);
            }
        }
    }
}

/**
 * Plots a pixel on the radar for the given cell.
 */
void RadarClass::Plot_Radar_Pixel(cell_t cellnum)
{
    static GraphicBufferClass _tile_stage(CELL_PIXELS, CELL_PIXELS);

    if (cellnum == -1) {
        cellnum = 1;
    }

    if (m_RadarActive && unsigned(cellnum) <= MAP_MAX_AREA && In_Radar(cellnum) && Cell_On_Radar(cellnum)) {
        int x = Cell_Get_X(cellnum) - m_MiniMapXOffset;
        int y = Cell_Get_Y(cellnum) - m_MiniMapYOffset;

        if (unsigned(x) < m_MiniMapCellWidth && unsigned(y) < m_MiniMapCellHeight) {
            bool somebool = false;

            if (g_LogicPage->Lock()) {
                x = m_MiniMapScale * x + m_MinRadarX + m_RadarButtonXPos + m_MiniMap.Get_Left();
                y = m_MiniMapScale * y + m_MinRadarY + m_RadarButtonYPos + m_MiniMap.Get_Top();

                uint8_t color = COLOR_TBLACK;
                CellClass &cell = m_Array[cellnum];

                if (!cell.Other_Houses_Jammed(g_PlayerPtr->What_Type()) && (cell.Is_Visible() || g_DebugUnshroud)) {
                    color = cell.Cell_Color(true);

                    if (cell.Is_House_Jammed(g_PlayerPtr->What_Type()) && color == COLOR_TBLACK) {
                        color = COLOR_BLACK;
                        somebool = true;
                    }
                } else {
                    color = COLOR_BLACK;
                }

                if (color != COLOR_TBLACK) {
                    g_LogicPage->Fill_Rect(x, y, m_MiniMapScale + x - 1, m_MiniMapScale + y - 1, color);
                } else if (m_MiniMapScale <= 1) {
                    g_LogicPage->Put_Pixel(x, y, cell.Cell_Color());
                } else {
                    IconControlType *icondata = nullptr;
                    uint8_t icon = 0;

                    if (cell.Get_Template() != TEMPLATE_NONE && cell.Get_Template() != TEMPLATE_ARRO0003) {
                        icondata = static_cast<IconControlType *>(
                            TemplateTypeClass::As_Reference(cell.Get_Template()).Get_Image_Data());
                        icon = cell.Get_Icon();
                    }

                    if (icondata == nullptr) {
                        icondata =
                            static_cast<IconControlType *>(TemplateTypeClass::As_Reference(TEMPLATE_CLEAR).Get_Image_Data());
                        icon = cell.Clear_Icon();
                    }

                    uint8_t *iconstart = icondata->Get_Icon_Data();
                    uint8_t iconidx = icondata->Get_Icon_Map()[icon];
                    _tile_stage.From_Buffer(
                        0, 0, CELL_PIXELS, CELL_PIXELS, &iconstart[(CELL_PIXELS * CELL_PIXELS) * iconidx]);
                    _tile_stage.Scale(
                        *g_LogicPage, 0, 0, x, y, CELL_PIXELS, CELL_PIXELS, m_MiniMapScale, m_MiniMapScale, true);
                }

                if (color != COLOR_BLACK) {
                    Render_Overlay(cellnum, x, y, m_MiniMapScale);
                    Render_Terrain(cellnum, x, y, m_MiniMapScale);
                    Render_Infantry(cellnum, x, y, m_MiniMapScale);
                } else if (somebool) {
                    Render_Infantry(cellnum, x, y, m_MiniMapScale);
                }

                g_LogicPage->Unlock();
            }
        }
    }
}

/**
 * Activates the radar into the given mode.
 */
int RadarClass::Radar_Activate(int mode)
{
    BOOL is_active = m_RadarActive;

    switch (mode) {
        default:
        case RADAR_M1: // Toggle active and inactive
            if (m_RadarActive) {
                Radar_Activate(RADAR_0);
                captainslog_debug("Radar: TacticalMap availability is off");
            } else {
                Radar_Activate(RADAR_1);
                captainslog_debug("Radar: TacticalMap availability is on");
            }

            break;

        case RADAR_0: // Turn radar off
            if (!g_Map.Is_Sidebar_Shown()) {
                Radar_Activate(RADAR_2);

                return is_active;
            }

            if (!m_RadarActive || m_RadarDeactivating) {
                return is_active;
            }

            Sound_Effect(VOC_RADAR_OFF);

            captainslog_debug("Radar: DEACTIVING");

            m_RadarDeactivating = true;
            m_RadarActive = false;

            if (m_RadarActivating) {
                m_RadarActivating = false;
            } else {
                m_RadarAnimFrame = 22;
            }

            return is_active;

        case RADAR_1: // Turn radar on
            if (!g_Map.Is_Sidebar_Shown()) {
                Radar_Activate(RADAR_3);

                return is_active;
            }

            if (m_RadarActivating || m_RadarActive) {
                return is_active;
            }

            Sound_Effect(VOC_RADAR_ON);

            captainslog_debug("Radar: ACTIVATING");

            m_RadarActivating = true;

            if (m_RadarDeactivating) {
                m_RadarDeactivating = false;
            } else {
                m_RadarAnimFrame = m_RadarExists ? 41 : 0;
            }

            return is_active;

        case RADAR_2: // Radar off no side bar.
            if (g_Session.Game_To_Play() != GAME_CAMPAIGN) {
                g_Map.Enable_Zoom_Button();
            } else {
                g_Map.Disable_Zoom_Button();
            }

            m_RadarActivating = false;
            m_RadarDeactivating = false;

            break;

        case RADAR_3: // Radar on no side bar
            if (g_Session.Game_To_Play() == GAME_CAMPAIGN && Is_Zoomable()) {
                g_Map.Enable_Zoom_Button();
            }

            m_RadarActive = true;
            m_RadarActivating = false;
            m_RadarDeactivating = false;

            break;

        case RADAR_4: // Remove radar
            m_RadarExists = false;
            m_RadarActive = false;
            m_RadarActivating = false;
            m_RadarDeactivating = false;
            Flag_To_Redraw();
            m_RadarToRedraw = true;

            break;
    }

    if (is_active != m_RadarActive) {
        m_RadarToRedraw = true;
        Flag_To_Redraw();
    }

    s_FullRedraw = m_RadarActive;

    return is_active;
}

/**
 * Set the Spied House to the next house that the player has spied on.
 */
BOOL RadarClass::Spy_Next_House()
{
    m_RadarDrawNames = false;
    m_RadarToRedraw = true;

    HousesType first;
    HousesType last;

    if (g_Session.Game_To_Play() == GAME_CAMPAIGN) {
        first = HOUSES_FIRST;
        last = HOUSES_SP_LAST;
    } else {
        first = HOUSES_MULTI_FIRST;
        last = HOUSES_MULTI_LAST;
    }

    if (m_RadarDrawSpiedInfo && first <= (m_SpiedHouse + 1)) {
        first = (HousesType)(m_SpiedHouse + 1);
    }

    bool have_spied = false;

    for (HousesType house = first; house < last; ++house) {
        HouseClass *hptr = HouseClass::As_Pointer(house);

        // Check if house is active and if the PlayerHouse is in the list of houses that have spied upon this house.
        if (hptr != nullptr && hptr->Is_Active() && hptr != g_PlayerPtr && hptr->Spied_My_Radar(g_PlayerPtr)) {
            have_spied = true;
            m_SpiedHouse = house;
            break;
        }
    }

    m_RadarDrawSpiedInfo = have_spied;
    Flag_To_Redraw(true);

    return have_spied;
}
