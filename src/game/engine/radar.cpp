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
#include "audio.h"
#include "gamefile.h"
#include "coord.h"
#include "display.h"
#include "drawshape.h"
#include "globals.h"
#include "iomap.h"
#include "lists.h"
#include "minmax.h"
#include "mixfile.h"
#include "radar.h"
#include "session.h"
#include "swap.h"
#include "voc.h"

#ifndef CHRONOSHIFT_STANDALONE
RadarClass::RTacticalClass &RadarClass::RadarButton = Make_Global<RadarClass::RTacticalClass>(0x006878E4);
void *&RadarClass::RadarAnim = Make_Global<void *>(0x00687908);
void *&RadarClass::RadarPulse = Make_Global<void *>(0x0068790C);
void *&RadarClass::RadarFrame = Make_Global<void *>(0x00687910);
BOOL &RadarClass::FullRedraw = Make_Global<BOOL>(0x00687914);
GraphicBufferClass &RadarClass::TileStage = Make_Global<GraphicBufferClass>(0x006879BC);
#else
RadarClass::RTacticalClass RadarClass::RadarButton;
void *RadarClass::RadarAnim = nullptr;
void *RadarClass::RadarPulse = nullptr;
void *RadarClass::RadarFrame = nullptr;
BOOL RadarClass::FullRedraw;
GraphicBufferClass RadarClass::TileStage;
#endif

GraphicBufferClass RadarClass::IconStage(3, 3);

RadarClass::RTacticalClass::RTacticalClass() :
    GadgetClass(0, 0, 0, 0, MOUSE_LEFT_PRESS | MOUSE_LEFT_HELD | MOUSE_LEFT_RLSE | MOUSE_LEFT_UP | MOUSE_RIGHT_PRESS, true)
{
}

BOOL RadarClass::RTacticalClass::Action(unsigned flags, KeyNumType &key)
{
// TODO, needs HelpClass, MouseClass, TechnoClass.
#ifndef CHRONOSHIFT_STANDALONE
    BOOL(*func)
    (const RTacticalClass *, unsigned, KeyNumType &) =
        reinterpret_cast<BOOL (*)(const RTacticalClass *, unsigned, KeyNumType &)>(0x00531034);
    return func(this, flags, key);
#else
    return false;
#endif
}

RadarClass::RadarClass() :
    RadarToRedraw(false),
    RadarCursorRedraw(false),
    RadarExists(false),
    RadarActive(false),
    RadarActivating(false),
    RadarDeactivating(false),
    RadarJammed(false),
    RadarPulseActive(false),
    RadarZoomed(false),
    RadarDrawNames(false),
    RadarDrawSpiedInfo(false),
    RadarPulseFrame(0),
    RadarCursorFrame(0),
    RadarAnimFrame(0),
    MiniMapXOffset(0),
    MiniMapYOffset(0),
    MiniMapCellWidth(0),
    MiniMapCellHeight(0),
    MiniMapCell(0),
    MiniMap(0, 0, 0, 0),
    MiniMapScale(0),
    SpiedHouse(HOUSES_NONE),
    MiniMapCellCount(0)
{
}

void RadarClass::One_Time()
{
    RadarButtonWidth = 160;
    RadarButtonHeight = 140;
    RadarButtonYPos = 14;
    field_CA8 = 128;
    field_CAC = 128;
    MinRadarX = 6;
    MinRadarY = 7;
    MaxRadarWidth = 146;
    MaxRadarHeight = 130;
    RadarButtonXPos = g_seenBuff.Get_Width() - RadarButtonWidth;

    DisplayClass::One_Time();

    RadarButton.Set_Position(RadarButtonXPos, RadarButtonYPos);
    RadarButton.Set_Size(RadarButtonWidth, RadarButtonHeight);

    // Moved from Draw_It as its the same no matter what.
    RadarPulse = GameFileClass::Retrieve("pulse.shp");
}

void RadarClass::Init_Clear()
{
    DisplayClass::Init_Clear();

    RadarToRedraw = true;
    RadarCursorRedraw = true;
    RadarExists = false;
    RadarActive = false;
    RadarActivating = false;
    RadarDeactivating = false;
    RadarDrawNames = false;
    MiniMapCellCount = 0;

    if (MapCellWidth > 0 || MapCellHeight > 0) {
        RadarZoomed = false;
        Zoom_Mode(Coord_To_Cell(DisplayPos));
    }
}

void RadarClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
// TODO Needs SidebarClass.
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(const RadarClass *, KeyNumType &, int, int) =
        reinterpret_cast<void (*)(const RadarClass *, KeyNumType &, int, int)>(0x00530E6C);
    func(this, key, mouse_x, mouse_y);
#endif
}

void RadarClass::Draw_It(BOOL force_redraw)
{
// TODO Needs HouseClass.
#ifndef CHRONOSHIFT_STANDALONE
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

void RadarClass::Refresh_Cells(cell_t cellnum, int16_t *overlap_list)
{
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);

    if (overlap_list != nullptr) {
        if (*overlap_list == LIST_START) {
            RadarToRedraw = true;
            Flag_To_Redraw();
        }
    }

    DisplayClass::Refresh_Cells(cellnum, overlap_list);
}

void RadarClass::Set_Tactical_Position(coord_t location)
{
    DisplayClass::Set_Tactical_Position(location);
    Set_Radar_Position(Coord_To_Cell(DisplayPos));
}

void RadarClass::Flag_Cell(cell_t cellnum)
{
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);
    DisplayClass::Flag_Cell(cellnum);
}

BOOL RadarClass::Jam_Cell(cell_t cellnum, HouseClass *house)
{
// TODO Needs HouseClass.
#ifndef CHRONOSHIFT_STANDALONE
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
#ifndef CHRONOSHIFT_STANDALONE
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
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(const RadarClass *) = reinterpret_cast<BOOL (*)(const RadarClass *)>(0x005329C4);
    return func(this);
#else
    /*
    DEBUG_ASSERT(this != nullptr);
    if (PlayerPtr->GPSActive) {
        return false;
    }
    */
    return RadarJammed;
#endif
}

BOOL RadarClass::Is_Radar_Active()
{
// TODO Requires HouseClass
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(const RadarClass *) = reinterpret_cast<BOOL (*)(const RadarClass *)>(0x00532974);
    return func(this);
#else
    /*
    return RadarActive || PlayerPtr->GPSActive;
    */
    return false;
#endif
}

BOOL RadarClass::Is_Radar_Existing()
{
// TODO Requires HouseClass
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(const RadarClass *) = reinterpret_cast<BOOL (*)(const RadarClass *)>(0x0053299C);
    return func(this);
#else
    /*
    return RadarExists || PlayerPtr->GPSActive;
    */
    return 0;
#endif
}

void RadarClass::Activate_Pulse()
{
    if (Is_Radar_Active()) {
        RadarPulseFrame = 0;
        RadarPulseActive = true;
    }
}

void RadarClass::Radar_Pixel(cell_t cellnum)
{
    if (RadarActive && Map.Is_Sidebar_Drawn() && Cell_On_Radar(cellnum)) {
        RadarToRedraw = true;
        Array[cellnum].Set_Bit1(true);

        if (MiniMapCellCount < MINI_MAP_CELLS) {
            MiniMapCells[MiniMapCellCount++] = cellnum;
        }
    }
}

void RadarClass::Render_Terrain(cell_t cellnum, int x, int y, int scale)
{
// TODO Needs TerrainClass
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(const RadarClass *, cell_t, int, int, int) =
        reinterpret_cast<void (*)(const RadarClass *, cell_t, int, int, int)>(0x0052E5BC);
    func(this, cellnum, x, y, scale);
#elif 0
    // static TerrainClass const *terrains[3];    //not sure, but its there...
    ObjectClass *objects[4];
    int objectcount = 0;

    ObjectClass *occupier = Array[cellnum].Get_Occupier();

    if (occupier != nullptr && occupier->What_Am_I() == RTTI_TERRAIN) {
        objects[objectcount++] = occupier;
    }

    for (int index = 0; index < OVERLAPPER_COUNT; ++index) {
        ObjectClass *overlap = Array[cellnum].Get_Overlapper(index);

        if (overlap != nullptr && overlap->What_Am_I() == RTTI_TERRAIN) {
            objects[objectcount++] = overlap;
        }
    }

    if (objectcount > 0) {
        if (scale == 1) {
            g_logicPage->Put_Pixel(x, y, 21);
        } else {
            // TODO, A simple sort algo?
            for (int i = 0; i < objectcount - 1; ++i) {
                for (int j = i + 1; j < objectcount; ++j) {
                    if (ObjectClass::Sort_Y_Greater_Than(objects[i], objects[j])) {
                        Swap(objects[i], objects[j]);
                    }
                }
            }

            // Render loop for objects
            for (int index = 0; index < objectcount; ++index) {
                char *icondata = reinterpret_cast<TerrainClass *>(objects[index])->Radar_Icon(cellnum);

                if (icondata != nullptr) {
                    IconStage.From_Buffer(0, 0, 3, 3, icondata);
                    IconStage.Scale(*g_logicPage, 0, 0, x, y, 3, 3, MiniMapScale, MiniMapScale, true, FadingBrighten);
                }
            }
        }
    }
#endif
}

void RadarClass::Render_Infantry(cell_t cellnum, int x, int y, int scale)
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(const RadarClass *, cell_t, int, int, int) =
        reinterpret_cast<void (*)(const RadarClass *, cell_t, int, int, int)>(0x0052E9E8);
    func(this, cellnum, x, y, scale);
#endif
}

void RadarClass::Render_Overlay(cell_t cellnum, int x, int y, int scale)
{
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);
    CellClass &cell = Array[cellnum];

    if (cell.Get_Overlay() != OVERLAY_NONE) {
        OverlayTypeClass &optr = OverlayTypeClass::As_Reference(cell.Get_Overlay());

        if (optr.Is_Radar_Visible()) {
            uint8_t *icondata = optr.Radar_Icon(cell.Get_Overlay_Frame());

            if (icondata != nullptr) {
                IconStage.From_Buffer(0, 0, 3, 3, icondata);

                if (optr.Is_Ore()) {
                    if (scale == 1) {
                        g_logicPage->Put_Pixel(x, y, COLOR_GREY);
                    } else {
                        IconStage.Scale(*g_logicPage, 0, 0, x, y, 3, 3, scale, scale, true, FadingYellow);
                    }
                }
            }
        }
    }
}

void RadarClass::Zoom_Mode(cell_t cellnum)
{
    if (Is_Zoomable()) {
        RadarZoomed = !RadarZoomed;
    } else {
        RadarZoomed = true;
    }

    MiniMap.m_left = 0;
    MiniMap.m_top = 0;

    int width;
    int height;

    if (RadarZoomed) {
        MiniMapScale = 3;
        width = MaxRadarWidth / MiniMapScale;
        height = MaxRadarHeight / MiniMapScale;
    } else {
        int xscale = MaxRadarWidth / MapCellWidth;
        int yscale = MaxRadarHeight / MapCellHeight;
        MiniMapScale = Clamp(Min(xscale, yscale), 1, yscale);
        width = MapCellWidth;
        height = MapCellHeight;
    }

    width = Min(Min(width, MaxRadarWidth), MapCellWidth);
    height = Min(Min(height, MaxRadarHeight), MapCellHeight);

    int xremain = MaxRadarWidth - MiniMapScale * width;
    int yremain = MaxRadarHeight - MiniMapScale * height;

    MiniMap.m_left = xremain / 2;
    MiniMap.m_top = yremain / 2;
    MiniMap.m_right = MaxRadarWidth - xremain;
    MiniMap.m_bottom = MaxRadarHeight - yremain;
    MiniMapCellWidth = width;
    MiniMapCellHeight = height;

    Set_Radar_Position(cellnum);
    RadarToRedraw = true;
    Flag_To_Redraw();
    FullRedraw = true;
}

BOOL RadarClass::Is_Zoomable() const
{
    int wratio = MaxRadarWidth / MapCellWidth;
    int hratio = MaxRadarHeight / MapCellHeight;

    return Max(1, Min(wratio, hratio)) != 3;
}

int RadarClass::Click_In_Radar(int &x, int &y, BOOL set_coords) const
{
    // TODO, make sidebar check virtual so don't need global object.
    if (Map.Is_Sidebar_Drawn() && RadarActive) {
        unsigned x_diff = x - (MinRadarX + RadarButtonXPos);
        unsigned y_diff = y - (MinRadarY + RadarButtonYPos);

        // Abuses unsigned underflow to check if within bounds or negative in fewer compares.
        if (x_diff < (unsigned)MaxRadarWidth && y_diff < (unsigned)MaxRadarHeight) {
            int xpos = x - (MinRadarX + RadarButtonXPos) - MiniMap.m_left;
            int ypos = y - (MinRadarY + RadarButtonYPos) - MiniMap.m_top;
            int scale = MiniMapScale - 1;

            if ((unsigned)xpos >= (unsigned)(scale + MiniMap.m_right)
                || (unsigned)ypos >= (unsigned)(scale + MiniMap.m_bottom)) {
                return -1;
            } else {
                if (set_coords) {
                    x = xpos / MiniMapScale + MiniMapXOffset;
                    y = ypos / MiniMapScale + MiniMapYOffset;
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

    if (!RadarZoomed) {
        return true;
    }

    if (Cell_Get_X(cellnum) - MiniMapXOffset > MiniMapCellWidth
        || Cell_Get_Y(cellnum) - MiniMapYOffset > MiniMapCellHeight) {
        return false;
    }

    return true;
}

void RadarClass::Set_Radar_Position(cell_t cellnum)
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(const RadarClass *, cell_t) = reinterpret_cast<void (*)(const RadarClass *, cell_t)>(0x005314B4);
    func(this, cellnum);
#endif
}
cell_t RadarClass::Radar_Position()
{
    return MiniMapCell;
}

void RadarClass::Cell_XY_To_Radar_Pixel(int cell_x, int cell_y, int &x, int &y)
{
    x = MiniMapScale * (cell_x - MiniMapXOffset);
    y = MiniMapScale * (cell_y - MiniMapYOffset);
}

void RadarClass::Radar_Anim()
{
    if (!RadarDrawNames && Map.Is_Sidebar_Drawn()) {
        GraphicViewPortClass *old_vp = Set_Logic_Page(&g_hidPage);

        GraphicViewPortClass vp(g_logicPage->Get_Graphic_Buffer(),
            MinRadarX + RadarButtonXPos + g_logicPage->Get_XPos(),
            g_logicPage->Get_YPos() + RadarButtonYPos + MinRadarY,
            MaxRadarWidth,
            MaxRadarHeight);

        Draw_Box(MinRadarX + RadarButtonXPos - 1,
            MinRadarY + RadarButtonYPos - 1,
            MaxRadarWidth + 2,
            MaxRadarHeight + 2,
            BOX_STYLE_1,
            true);

        vp.Clear();

        CC_Draw_Shape(RadarAnim, RadarAnimFrame, RadarButtonXPos, RadarButtonYPos + 2);

        Set_Logic_Page(old_vp);
    }
}

void RadarClass::Radar_Cursor(BOOL redraw)
{
#define RADAR_CURSOR_SIZE 6
    static cell_t _last_pos = -1;
    static int _last_frame = -1;

    cell_t cell = Coord_To_Cell(DisplayPos);
    int cell_x = Cell_Get_X(cell);
    int cell_y = Cell_Get_Y(cell);
    int cell_w = 0;
    int cell_h = 0;

    if (cell == -1 || cell != _last_pos || RadarCursorFrame != _last_frame || redraw) {
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
            cell_w = last_x + Lepton_To_Cell_Coord(DisplayWidth) + (Lepton_Sub_Cell(DisplayWidth) < 128 ? 0 : 1);
            cell_h = last_y + Lepton_To_Cell_Coord(DisplayHeight) + (Lepton_Sub_Cell(DisplayHeight) < 128 ? 0 : 1);

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
        cell_w = cell_x + Lepton_To_Cell_Coord(DisplayWidth) + (Lepton_Sub_Cell(DisplayWidth) < 128 ? 0 : 1);
        cell_h = cell_y + Lepton_To_Cell_Coord(DisplayHeight) + (Lepton_Sub_Cell(DisplayHeight) < 128 ? 0 : 1);

        Cell_XY_To_Radar_Pixel(cell_w, cell_h, pixel_right, pixel_btm);

        --pixel_right;
        --pixel_btm;

        pixel_left -= RadarCursorFrame;
        pixel_top -= RadarCursorFrame;
        pixel_right += RadarCursorFrame;
        pixel_btm += RadarCursorFrame;

        Mark_Radar(pixel_left, pixel_top, pixel_right, pixel_btm, true, RADAR_CURSOR_SIZE);

        GraphicViewPortClass *old = Set_Logic_Page(&g_hidPage);

        GraphicViewPortClass gvp(g_logicPage->Get_Graphic_Buffer(),
            g_logicPage->Get_XPos() + MinRadarX + RadarButtonXPos + MiniMap.m_left,
            g_logicPage->Get_YPos() + MinRadarY + RadarButtonYPos + MiniMap.m_top,
            MiniMap.m_right,
            MiniMap.m_bottom);

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
        _last_frame = RadarCursorFrame;

        RadarCursorRedraw = false;
    }

        // Undefine line length so not misused elsewhere.
#undef RADAR_CURSOR_SIZE
}

void RadarClass::Player_Names(BOOL draw)
{
    RadarDrawNames = draw;
    Flag_To_Redraw(true);
}

BOOL RadarClass::Draw_House_Info()
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(const RadarClass *) = reinterpret_cast<BOOL (*)(const RadarClass *)>(0x00532268);
    return func(this);
#else
    return true;
#endif
}

void RadarClass::Draw_Names()
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(const RadarClass *) = reinterpret_cast<void (*)(const RadarClass *)>(0x005325E4);
    func(this);
#endif
}

void RadarClass::Mark_Radar(int left, int top, int right, int bottom, BOOL mark, int size)
{
    int l_adj = (left / MiniMapScale) + MiniMapXOffset;
    int t_adj = (top / MiniMapScale) + MiniMapYOffset;
    int r_adj = (right / MiniMapScale) + MiniMapXOffset;
    int b_adj = (bottom / MiniMapScale) + MiniMapYOffset;
    int scaling = (size / MiniMapScale) + 1;

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
        CellClass &cell = Array[cellnum];

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
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(const RadarClass *, cell_t) = reinterpret_cast<void (*)(const RadarClass *, cell_t)>(0x0052F4C4);
    func(this, cellnum);
#endif
}

int RadarClass::Radar_Activate(int mode)
{
    bool is_active = RadarActive;

    switch (mode) {
        default:
        case RADAR_M1: // Toggle active and inactive
            if (RadarActive) {
                Radar_Activate(RADAR_0);
                DEBUG_LOG("Radar: TacticalMap availability is off\n");
            } else {
                Radar_Activate(RADAR_1);
                DEBUG_LOG("Radar: TacticalMap availability is on\n");
            }

            break;

        case RADAR_0: // Turn radar off
            if (!Map.Is_Sidebar_Drawn()) {
                Radar_Activate(RADAR_2);

                return is_active;
            }

            if (!RadarActive || RadarDeactivating) {
                return is_active;
            }

            Sound_Effect(VOC_RADAR_OFF);

            DEBUG_LOG("Radar: DEACTIVING\n");

            RadarDeactivating = true;
            RadarActive = false;

            if (RadarActivating) {
                RadarActivating = false;
            } else {
                RadarAnimFrame = 22;
            }

            return is_active;

        case RADAR_1: // Turn radar on
            if (!Map.Is_Sidebar_Drawn()) {
                Radar_Activate(RADAR_3);

                return is_active;
            }

            if (RadarActivating || RadarActive) {
                return is_active;
            }

            Sound_Effect(VOC_RADAR_ON);

            DEBUG_LOG("Radar: ACTIVATING\n");

            RadarActivating = true;

            if (RadarDeactivating) {
                RadarDeactivating = false;
            } else {
                RadarAnimFrame = RadarExists ? 41 : 0;
            }

            return is_active;

        case RADAR_2: // Radar off no side bar.
            if (Session.Game_To_Play() != GAME_CAMPAIGN) {
                Map.Enable_Zoom_Button();
            } else {
                Map.Disable_Zoom_Button();
            }

            RadarActivating = false;
            RadarDeactivating = false;

            break;

        case RADAR_3: // Radar on no side bar
            if (Session.Game_To_Play() == GAME_CAMPAIGN && Is_Zoomable()) {
                Map.Enable_Zoom_Button();
            }

            RadarActive = true;
            RadarActivating = false;
            RadarDeactivating = false;

            break;

        case RADAR_4: // Remove radar
            RadarExists = false;
            RadarActive = false;
            RadarActivating = false;
            RadarDeactivating = false;
            Flag_To_Redraw();
            RadarToRedraw = true;

            break;
    }

    if (is_active != RadarActive) {
        RadarToRedraw = true;
        Flag_To_Redraw();
    }

    FullRedraw = RadarActive;

    return is_active;
}

BOOL RadarClass::Spy_Next_House()
{
// TODO Requires HouseClass
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(const RadarClass *) = reinterpret_cast<BOOL (*)(const RadarClass *)>(0x0053214C);
    return func(this);
#else
    /*
    DrawNames = false;
    RadarToRedraw = true;

    HousesType first;
    HousesType last;

    if (Session.GameToPlay == GAME_CAMPAIGN) {
        first = HOUSES_FIRST;
        last = HOUSES_LAST;
    } else {
        first = HOUSES_MULTI_FIRST;
        last = HOUSES_MULTI_LAST;
    }

    if (DrawSpiedInfo && first <= (SpiedHouse + 1)) {
        first = (HousesType)(SpiedHouse + 1);
    }

    bool have_spied = false;

    for (HousesType house = first; house < last; ++house) {
        HouseClass *hptr = HouseClass::As_Pointer(house);

        // Check if house is active and if the PlayerHouse is in the list of
        // houses that have spied upon this house.
        if (hptr != nullptr && hptr->Is_Active() && hptr != PlayerPtr && hptr->field_2FD[PlayerPtr->What_Type()] == true) {
            have_spied = true;
            SpiedHouse = house;
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
