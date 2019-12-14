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
#include "iomap.h"
#include "lists.h"
#include "mixfile.h"
#include "session.h"
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

BOOL RadarClass::RTacticalClass::Action(unsigned flags, KeyNumType &key)
{
// TODO, needs HelpClass, MouseClass, TechnoClass.
#ifdef GAME_DLL
    BOOL(*func)
    (const RTacticalClass *, unsigned, KeyNumType &) =
        reinterpret_cast<BOOL (*)(const RTacticalClass *, unsigned, KeyNumType &)>(0x00531034);
    return func(this, flags, key);
#else
    return false;
#endif
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
    m_RadarZoomed(false),
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

void RadarClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
// TODO Needs SidebarClass.
#ifdef GAME_DLL
    void (*func)(const RadarClass *, KeyNumType &, int, int) =
        reinterpret_cast<void (*)(const RadarClass *, KeyNumType &, int, int)>(0x00530E6C);
    func(this, key, mouse_x, mouse_y);
#endif
}

void RadarClass::Draw_It(BOOL force_redraw)
{
// TODO Needs HouseClass.
#ifdef GAME_DLL
    void (*func)(const RadarClass *, BOOL) = reinterpret_cast<void (*)(const RadarClass *, BOOL)>(0x0052DA14);
    func(this, force_redraw);
#endif
}

void RadarClass::Set_Map_Dimensions(int x, int y, int w, int h)
{
    Set_Radar_Position(Cell_From_XY(x, y));
    MapClass::Set_Map_Dimensions(x, y, w, h);
}

BOOL RadarClass::Map_Cell(cell_t cellnum, HouseClass *house)
{
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);

    if (DisplayClass::Map_Cell(cellnum, house)) {
        Radar_Pixel(cellnum);

        return true;
    }

    return false;
}

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

void RadarClass::Refresh_Cells(cell_t cellnum, const int16_t *list)
{
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);

    if (list != nullptr) {
        if (*list == LIST_START) {
            m_RadarToRedraw = true;
            Flag_To_Redraw();
        }
    }

    DisplayClass::Refresh_Cells(cellnum, list);
}

void RadarClass::Set_Tactical_Position(coord_t location)
{
    DisplayClass::Set_Tactical_Position(location);
    Set_Radar_Position(Coord_To_Cell(m_DisplayPos));
}

void RadarClass::Flag_Cell(cell_t cellnum)
{
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);
    DisplayClass::Flag_Cell(cellnum);
}

BOOL RadarClass::Jam_Cell(cell_t cellnum, HouseClass *house)
{
// TODO Needs HouseClass.
#ifdef GAME_DLL
    BOOL(*func)
    (const RadarClass *, cell_t, HouseClass *) =
        reinterpret_cast<BOOL (*)(const RadarClass *, cell_t, HouseClass *)>(0x005301F0);
    return func(this, cellnum, house);
#else
    return false;
#endif
}

BOOL RadarClass::UnJam_Cell(cell_t cellnum, HouseClass *house)
{
// TODO Needs HouseClass.
#ifdef GAME_DLL
    BOOL(*func)
    (const RadarClass *, cell_t, HouseClass *) =
        reinterpret_cast<BOOL (*)(const RadarClass *, cell_t, HouseClass *)>(0x00530274);
    return func(this, cellnum, house);
#else
    return false;
#endif
}

// Renamed from Get_Jammed
BOOL RadarClass::Is_Radar_Jammed()
{
// TODO Requires HouseClass
#ifdef GAME_DLL
    BOOL (*func)(const RadarClass *) = reinterpret_cast<BOOL (*)(const RadarClass *)>(0x005329C4);
    return func(this);
#else
    /*
    DEBUG_ASSERT(this != nullptr);
    if (PlayerPtr->GPSActive) {
        return false;
    }
    */
    return m_RadarJammed;
#endif
}

BOOL RadarClass::Is_Radar_Active()
{
// TODO Requires HouseClass
#ifdef GAME_DLL
    BOOL (*func)(const RadarClass *) = reinterpret_cast<BOOL (*)(const RadarClass *)>(0x00532974);
    return func(this);
#else
    /*
    return m_RadarActive || PlayerPtr->GPSActive;
    */
    return false;
#endif
}

BOOL RadarClass::Is_Radar_Existing()
{
// TODO Requires HouseClass
#ifdef GAME_DLL
    BOOL (*func)(const RadarClass *) = reinterpret_cast<BOOL (*)(const RadarClass *)>(0x0053299C);
    return func(this);
#else
    /*
    return m_RadarExists || PlayerPtr->GPSActive;
    */
    return 0;
#endif
}

void RadarClass::Activate_Pulse()
{
    if (Is_Radar_Active()) {
        m_RadarPulseFrame = 0;
        m_RadarPulseActive = true;
    }
}

void RadarClass::Radar_Pixel(cell_t cellnum)
{
    if (m_RadarActive && g_Map.Is_Sidebar_Drawn() && Cell_On_Radar(cellnum)) {
        m_RadarToRedraw = true;
        m_Array[cellnum].Set_Bit1(true);

        if (m_MiniMapCellCount < MINI_MAP_CELLS) {
            m_MiniMapCells[m_MiniMapCellCount++] = cellnum;
        }
    }
}

void RadarClass::Render_Terrain(cell_t cellnum, int x, int y, int scale)
{
// TODO Needs TerrainClass
#ifdef GAME_DLL
    void (*func)(const RadarClass *, cell_t, int, int, int) =
        reinterpret_cast<void (*)(const RadarClass *, cell_t, int, int, int)>(0x0052E5BC);
    func(this, cellnum, x, y, scale);
#elif 0
    // static TerrainClass const *terrains[3];    //not sure, but its there...
    ObjectClass *objects[4];
    int objectcount = 0;

    ObjectClass *occupier = m_Array[cellnum].Get_Occupier();

    if (occupier != nullptr && occupier->What_Am_I() == RTTI_TERRAIN) {
        objects[objectcount++] = occupier;
    }

    for (int index = 0; index < OVERLAPPER_COUNT; ++index) {
        ObjectClass *overlap = m_Array[cellnum].Get_Overlapper(index);

        if (overlap != nullptr && overlap->What_Am_I() == RTTI_TERRAIN) {
            objects[objectcount++] = overlap;
        }
    }

    if (objectcount > 0) {
        if (scale == 1) {
            g_LogicPage->Put_Pixel(x, y, 21);
        } else {
            // TODO, A simple sort algo?
            for (int i = 0; i < objectcount - 1; ++i) {
                for (int j = i + 1; j < objectcount; ++j) {
                    if (ObjectClass::Sort_Y_Greater_Than(objects[i], objects[j])) {
                        std::swap(objects[i], objects[j]);
                    }
                }
            }

            // Render loop for objects
            for (int index = 0; index < objectcount; ++index) {
                char *icondata = reinterpret_cast<TerrainClass *>(objects[index])->Radar_Icon(cellnum);

                if (icondata != nullptr) {
                    s_IconStage.From_Buffer(0, 0, 3, 3, icondata);
                    s_IconStage.Scale(*g_LogicPage, 0, 0, x, y, 3, 3, m_MiniMapScale, m_MiniMapScale, true, s_FadingBrighten);
                }
            }
        }
    }
#endif
}

void RadarClass::Render_Infantry(cell_t cellnum, int x, int y, int scale)
{
#ifdef GAME_DLL
    void (*func)(const RadarClass *, cell_t, int, int, int) =
        reinterpret_cast<void (*)(const RadarClass *, cell_t, int, int, int)>(0x0052E9E8);
    func(this, cellnum, x, y, scale);
#endif
}

void RadarClass::Render_Overlay(cell_t cellnum, int x, int y, int scale)
{
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);
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

BOOL RadarClass::Is_Zoomable() const
{
    int wratio = m_MaxRadarWidth / m_MapCellWidth;
    int hratio = m_MaxRadarHeight / m_MapCellHeight;

    return std::max(1, std::min(wratio, hratio)) != 3;
}

int RadarClass::Click_In_Radar(int &x, int &y, BOOL set_coords) const
{
    // TODO, make sidebar check virtual so don't need global object.
    if (g_Map.Is_Sidebar_Drawn() && m_RadarActive) {
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

void RadarClass::Set_Radar_Position(cell_t cellnum)
{
#ifdef GAME_DLL
    void (*func)(const RadarClass *, cell_t) = reinterpret_cast<void (*)(const RadarClass *, cell_t)>(0x005314B4);
    func(this, cellnum);
#endif
}
cell_t RadarClass::Radar_Position()
{
    return m_MiniMapCell;
}

void RadarClass::Cell_XY_To_Radar_Pixel(int cell_x, int cell_y, int &x, int &y)
{
    x = m_MiniMapScale * (cell_x - m_MiniMapXOffset);
    y = m_MiniMapScale * (cell_y - m_MiniMapYOffset);
}

void RadarClass::Radar_Anim()
{
    if (!m_RadarDrawNames && g_Map.Is_Sidebar_Drawn()) {
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

        Set_Logic_Page(old_vp);
    }
}

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
            cell_w = last_x + Lepton_To_Cell_Coord(m_DisplayWidth) + (Lepton_Sub_Cell(m_DisplayWidth) < 128 ? 0 : 1);
            cell_h = last_y + Lepton_To_Cell_Coord(m_DisplayHeight) + (Lepton_Sub_Cell(m_DisplayHeight) < 128 ? 0 : 1);

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

        // Add 1 to the dimensions if the position lies more than half way into another cell.
        cell_w = cell_x + Lepton_To_Cell_Coord(m_DisplayWidth) + (Lepton_Sub_Cell(m_DisplayWidth) < 128 ? 0 : 1);
        cell_h = cell_y + Lepton_To_Cell_Coord(m_DisplayHeight) + (Lepton_Sub_Cell(m_DisplayHeight) < 128 ? 0 : 1);

        Cell_XY_To_Radar_Pixel(cell_w, cell_h, pixel_right, pixel_btm);

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

void RadarClass::Player_Names(BOOL draw)
{
    m_RadarDrawNames = draw;
    Flag_To_Redraw(true);
}

BOOL RadarClass::Draw_House_Info()
{
#ifdef GAME_DLL
    BOOL (*func)(const RadarClass *) = reinterpret_cast<BOOL (*)(const RadarClass *)>(0x00532268);
    return func(this);
#else
    return true;
#endif
}

void RadarClass::Draw_Names()
{
#ifdef GAME_DLL
    void (*func)(const RadarClass *) = reinterpret_cast<void (*)(const RadarClass *)>(0x005325E4);
    func(this);
#endif
}

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

void RadarClass::Plot_Radar_Pixel(cell_t cellnum)
{
#ifdef GAME_DLL
    void (*func)(const RadarClass *, cell_t) = reinterpret_cast<void (*)(const RadarClass *, cell_t)>(0x0052F4C4);
    func(this, cellnum);
#endif
}

int RadarClass::Radar_Activate(int mode)
{
    bool is_active = m_RadarActive;

    switch (mode) {
        default:
        case RADAR_M1: // Toggle active and inactive
            if (m_RadarActive) {
                Radar_Activate(RADAR_0);
                DEBUG_LOG("Radar: TacticalMap availability is off\n");
            } else {
                Radar_Activate(RADAR_1);
                DEBUG_LOG("Radar: TacticalMap availability is on\n");
            }

            break;

        case RADAR_0: // Turn radar off
            if (!g_Map.Is_Sidebar_Drawn()) {
                Radar_Activate(RADAR_2);

                return is_active;
            }

            if (!m_RadarActive || m_RadarDeactivating) {
                return is_active;
            }

            Sound_Effect(VOC_RADAR_OFF);

            DEBUG_LOG("Radar: DEACTIVING\n");

            m_RadarDeactivating = true;
            m_RadarActive = false;

            if (m_RadarActivating) {
                m_RadarActivating = false;
            } else {
                m_RadarAnimFrame = 22;
            }

            return is_active;

        case RADAR_1: // Turn radar on
            if (!g_Map.Is_Sidebar_Drawn()) {
                Radar_Activate(RADAR_3);

                return is_active;
            }

            if (m_RadarActivating || m_RadarActive) {
                return is_active;
            }

            Sound_Effect(VOC_RADAR_ON);

            DEBUG_LOG("Radar: ACTIVATING\n");

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

BOOL RadarClass::Spy_Next_House()
{
// TODO Requires HouseClass
#ifdef GAME_DLL
    BOOL (*func)(const RadarClass *) = reinterpret_cast<BOOL (*)(const RadarClass *)>(0x0053214C);
    return func(this);
#else
    /*
    DrawNames = false;
    m_RadarToRedraw = true;

    HousesType first;
    HousesType last;

    if (g_Session.GameToPlay == GAME_CAMPAIGN) {
        first = HOUSES_FIRST;
        last = HOUSES_LAST;
    } else {
        first = HOUSES_MULTI_FIRST;
        last = HOUSES_MULTI_LAST;
    }

    if (DrawSpiedInfo && first <= (m_SpiedHouse + 1)) {
        first = (HousesType)(m_SpiedHouse + 1);
    }

    bool have_spied = false;

    for (HousesType house = first; house < last; ++house) {
        HouseClass *hptr = HouseClass::As_Pointer(house);

        // Check if house is active and if the PlayerHouse is in the list of
        // houses that have spied upon this house.
        if (hptr != nullptr && hptr->m_IsActive && hptr != PlayerPtr && hptr->field_2FD[PlayerPtr->What_Type()] == true) {
            have_spied = true;
            m_SpiedHouse = house;
            break;
        }
    }

    DrawSpiedInfo = have_spied;

    Flag_To_Redraw(true);

    return have_spied;
    */
    return 0;
#endif
}
