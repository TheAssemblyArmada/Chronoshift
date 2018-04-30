/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling tactical display (main action pane when in game).
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */

#include "radar.h"
#include "globals.h"
#include "ccfileclass.h"
#include "mixfile.h"
#include "lists.h"
#include "minmax.h"
#include "coord.h"
#include "display.h"
#include "iomap.h"

RadarClass::RTacticalClass &RadarClass::RadarButton = Make_Global<RadarClass::RTacticalClass>(0x006878E4);
void *&RadarClass::RadarAnim = Make_Global<void *>(0x00687908);
void *&RadarClass::RadarPulse = Make_Global<void *>(0x0068790C);
void *&RadarClass::RadarFrame = Make_Global<void *>(0x00687910);
bool &RadarClass::FullRedraw = Make_Global<bool>(0x00687914);
GraphicBufferClass &RadarClass::IconStage = Make_Global<GraphicBufferClass>(0x00687918);
GraphicBufferClass &RadarClass::TileStage = Make_Global<GraphicBufferClass>(0x006879BC);

RadarClass::RTacticalClass::RTacticalClass() :
    GadgetClass(0, 0, 0, 0, MOUSE_LEFT_PRESS | MOUSE_LEFT_HELD | MOUSE_LEFT_RLSE | MOUSE_LEFT_UP | MOUSE_RIGHT_PRESS, true)
{

}

BOOL RadarClass::RTacticalClass::Action(unsigned flags, KeyNumType &key)
{
    // TODO, needs HelpClass, MouseClass, TechnoClass.
#ifndef RAPP_STANDALONE
    BOOL(*func)(const RTacticalClass *, unsigned, KeyNumType &) = reinterpret_cast<BOOL(*)(const RTacticalClass *, unsigned, KeyNumType &)>(0x00531034);
    return func(this, flags, key);
#else
    return false;
#endif
}

RadarClass::RadarClass() :
    DisplayClass(),
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
    DEBUG_ASSERT(this != nullptr);

    RadarButtonHeight = 160;
    RadarButtonWidth = 140;
    RadarButtonYPos = 14;
    field_CA8 = 128;
    field_CAC = 128;
    MinRadarX = 6;
    MinRadarY = 7;
    MaxRadarWidth = 146;
    MaxRadarHeight = 130;
    RadarButtonXPos = g_seenBuff.Get_Width() - RadarButtonHeight;

    DisplayClass::One_Time();

    RadarButton.Set_Position(RadarButtonXPos, RadarButtonYPos);
    RadarButton.Set_Size(RadarButtonHeight, RadarButtonWidth);

    // Moved from Draw_It as its the same no matter what.
    RadarPulse = MixFileClass<CCFileClass>::Retrieve("pulse.shp");
}

void RadarClass::Init_Clear()
{
    DEBUG_ASSERT(this != nullptr);

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
#ifndef RAPP_STANDALONE
    void (*func)(const RadarClass *, KeyNumType &, int, int) =
        reinterpret_cast<void (*)(const RadarClass *, KeyNumType &, int, int)>(0x00530E6C);
    func(this, key, mouse_x, mouse_y);
#endif
}

void RadarClass::Draw_It(BOOL force_redraw)
{
#ifndef RAPP_STANDALONE
    void (*func)(const RadarClass *, BOOL) = reinterpret_cast<void (*)(const RadarClass *, BOOL)>(0x0052DA14);
    func(this, force_redraw);
#endif
}

void RadarClass::Set_Map_Dimensions(int x, int y, int w, int h)
{
    DEBUG_ASSERT(this != nullptr);

    Set_Radar_Position(Cell_From_XY(x, y));
    MapClass::Set_Map_Dimensions(x, y, w, h);
}

BOOL RadarClass::Map_Cell(int16_t cellnum, HouseClass *house)
{
    DEBUG_ASSERT(this != nullptr);
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);

    if (DisplayClass::Map_Cell(cellnum, house)) {
        Radar_Pixel(cellnum);

        return true;
    }

    return false;
}

int16_t RadarClass::Click_Cell_Calc(int x, int y) const
{

    DEBUG_ASSERT(this != nullptr);

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

void RadarClass::Refresh_Cells(int16_t cellnum, int16_t *overlap_list)
{
    DEBUG_ASSERT(this != nullptr);
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);

    if (overlap_list != nullptr) {
        if (*overlap_list == LIST_START) {
            RadarToRedraw = true;
            Flag_To_Redraw();
        }
    }
    DisplayClass::Refresh_Cells(cellnum, overlap_list);
}

void RadarClass::Set_Tactical_Position(uint32_t location)
{
    DEBUG_ASSERT(this != nullptr);
    DisplayClass::Set_Tactical_Position(location);
    Set_Radar_Position(Coord_To_Cell(DisplayPos));
}

void RadarClass::Flag_Cell(int16_t cellnum)
{
    DEBUG_ASSERT(this != nullptr);
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);
    DisplayClass::Flag_Cell(cellnum);
}

BOOL RadarClass::Jam_Cell(int16_t cellnum, HouseClass *house)
{
#ifndef RAPP_STANDALONE
    BOOL(*func)
    (const RadarClass *, int16_t, HouseClass *) =
        reinterpret_cast<BOOL (*)(const RadarClass *, int16_t, HouseClass *)>(0x005301F0);
    return func(this, cellnum, house);
#endif
    return 0;
}
BOOL RadarClass::UnJam_Cell(int16_t cellnum, HouseClass *house)
{
#ifndef RAPP_STANDALONE
    BOOL(*func)
    (const RadarClass *, int16_t, HouseClass *) =
        reinterpret_cast<BOOL (*)(const RadarClass *, int16_t, HouseClass *)>(0x00530274);
    return func(this, cellnum, house);
#endif
    return 0;
}

//Renamed from Get_Jammed
BOOL RadarClass::Is_Radar_Jammed()
{
    // TODO Requires HouseClass
#ifndef RAPP_STANDALONE
    BOOL (*func)(const RadarClass *) = reinterpret_cast<BOOL (*)(const RadarClass *)>(0x005329C4);
    return func(this);
#endif
    /*
    DEBUG_ASSERT(this != nullptr);
    if (PlayerPtr->GPSActive) {
        return false;
    }
    */
    return RadarJammed;
}

BOOL RadarClass::Is_Radar_Active()
{
    // TODO Requires HouseClass
#ifndef RAPP_STANDALONE
    BOOL (*func)(const RadarClass *) = reinterpret_cast<BOOL (*)(const RadarClass *)>(0x00532974);
    return func(this);
#endif
    /*
    DEBUG_ASSERT(this != nullptr);

    return RadarActive || PlayerPtr->GPSActive;
    */
    return 0;
}

BOOL RadarClass::Is_Radar_Existing()
{
    // TODO Requires HouseClass
#ifndef RAPP_STANDALONE
    BOOL (*func)(const RadarClass *) = reinterpret_cast<BOOL (*)(const RadarClass *)>(0x0053299C);
    return func(this);
#endif
    /*
    DEBUG_ASSERT(this != nullptr);

    return RadarExists || PlayerPtr->GPSActive;
    */
    return 0;
}

void RadarClass::Activate_Pulse()
{
    DEBUG_ASSERT(this != nullptr);

    if (Is_Radar_Active()) {
        RadarPulseFrame = 0;
        RadarPulseActive = true;
    }
}

void RadarClass::Radar_Pixel(int16_t cellnum)
{
    // TODO needs SidebarClass for SidebarIsDrawn
#ifndef RAPP_STANDALONE
    void (*func)(const RadarClass *, int16_t) = reinterpret_cast<void (*)(const RadarClass *, int16_t)>(0x0052FCC0);
    func(this, cellnum);
#endif
    /*
    DEBUG_ASSERT(this != nullptr);

    if (RadarActive && Map.SidebarIsDrawn && Cell_On_Radar(cellnum)) {
        RadarToRedraw = true;
        Map[cellnum].Set_Bit1(true);

        if (MiniMapCellCount < ARRAY_SIZE(MiniMapCells)) {
            MiniMapCells[MiniMapCellCount++] = cellnum;
        }
    }
    */
}

void RadarClass::Render_Terrain(int16_t cellnum, int x, int y, int scale)
{
#ifndef RAPP_STANDALONE
    void (*func)(const RadarClass *, int16_t, int, int, int) =
        reinterpret_cast<void (*)(const RadarClass *, int16_t, int, int, int)>(0x0052E5BC);
    func(this, cellnum, x, y, scale);
#endif
}

void RadarClass::Render_Infantry(int16_t cellnum, int x, int y, int scale)
{
#ifndef RAPP_STANDALONE
    void (*func)(const RadarClass *, int16_t, int, int, int) =
        reinterpret_cast<void (*)(const RadarClass *, int16_t, int, int, int)>(0x0052E9E8);
    func(this, cellnum, x, y, scale);
#endif
}
void RadarClass::Render_Overlay(int16_t cellnum, int x, int y, int scale)
{
#ifndef RAPP_STANDALONE
    void (*func)(const RadarClass *, int16_t, int, int, int) =
        reinterpret_cast<void (*)(const RadarClass *, int16_t, int, int, int)>(0x0052EF8C);
    func(this, cellnum, x, y, scale);
#endif
}
void RadarClass::Zoom_Mode(int16_t cellnum)
{
    // TODO figure out Rect stuff
#ifndef RAPP_STANDALONE
    void (*func)(const RadarClass *, int16_t) = reinterpret_cast<void (*)(const RadarClass *, int16_t)>(0x0052F294);
    func(this, cellnum);
#endif
    /*
    DEBUG_ASSERT(this != nullptr);

    if (Is_Zoomable()) {
        Zoomed = !Zoomed;
    } else {
        Zoomed = true;
    }

    MiniMap.X = 0;
    MiniMap.Y = 0;

    int width;
    int height;

    if (Zoomed) {
        MiniMapScale = 3;
        width = MaxRadarWidth / MiniMapScale;
        height = MaxRadarHeight / MiniMapScale;
    } else {
        int xscale = MaxRadarWidth / MapCellWidth;
        int yscale = MaxRadarHeight / MapCellHeight;
        MiniMapScale = Bound(Min(xscale, yscale), 1, yscale);
        width = MapCellWidth;
        height = MapCellHeight;
    }

    width = Min(Min(width, MaxRadarWidth), MapCellWidth);
    height = Min(Min(height, MaxRadarHeight), MapCellHeight);

    int xremain = MaxRadarWidth - MiniMapScale * width;
    int yremain = MaxRadarHeight - MiniMapScale * height;

    MiniMap.X = xremain / 2;
    MiniMap.Y = yremain / 2;
    MiniMap.Width = MaxRadarWidth - xremain;
    MiniMap.Height = MaxRadarHeight - yremain;
    MiniMapCellWidth = width;
    MiniMapCellHeight = height;

    Set_Radar_Position(cellnum);

    RadarToRedraw = true;

    Flag_To_Redraw();

    FullRedraw = true;
    */
}

BOOL const RadarClass::Is_Zoomable() const
{
    DEBUG_ASSERT(this != nullptr);

    int wratio = MaxRadarWidth / MapCellWidth;
    int hratio = MaxRadarHeight / MapCellHeight;

    return Max(1, Min(wratio, hratio)) != 3;
}

int const RadarClass::Click_In_Radar(int &x, int &y, BOOL set_coords) const
{
#ifndef RAPP_STANDALONE
    void (*func)(const RadarClass *, int &, int &, BOOL) =
        reinterpret_cast<void (*)(const RadarClass *, int &, int &, BOOL)>(0x0052FD3C);
    func(this, x, y, set_coords);
#endif
    /*
    DEBUG_ASSERT(this != nullptr);

    if (Map.SidebarIsDrawn && RadarActive && x - (MinRadarX + RadarButtonXPos) < MaxRadarWidth
        && y - (MinRadarY + RadarButtonYPos) < MaxRadarHeight) {
        int xpos = x - (MinRadarX + RadarButtonXPos) - MiniMap.X;
        int ypos = y - (MinRadarY + RadarButtonYPos) - MiniMap.Y;

        if (xpos >= MiniMapScale - 1 + MiniMap.Width || ypos >= MiniMapScale - 1 + MiniMap.Height) {
            return -1;
        } else {
            if (set_coords) {
                x = xpos / MiniMapScale + MiniMapXOffset;
                y = ypos / MiniMapScale + MiniMapYOffset;
            }

            return 1;
        }
    }
    */
    return 0;
}

BOOL const RadarClass::Cell_On_Radar(int16_t cellnum)
{
    DEBUG_ASSERT(this != nullptr);

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

void RadarClass::Set_Radar_Position(int16_t cellnum)
{
#ifndef RAPP_STANDALONE
    void (*func)(const RadarClass *, int16_t) = reinterpret_cast<void (*)(const RadarClass *, int16_t)>(0x005314B4);
    func(this, cellnum);
#endif
}
int16_t RadarClass::Radar_Position()
{
    return MiniMapCell;
}

void RadarClass::Cell_XY_To_Radar_Pixel(int cell_x, int cell_y, int &x, int &y)
{
    DEBUG_ASSERT(this != nullptr);

    x = MiniMapScale * (cell_x - MiniMapXOffset);
    y = MiniMapScale * (cell_y - MiniMapYOffset);
}

void RadarClass::Radar_Anim()
{
#ifndef RAPP_STANDALONE
    void (*func)(const RadarClass *) = reinterpret_cast<void (*)(const RadarClass *)>(0x00530CF4);
    func(this);
#endif
    /*
    DEBUG_ASSERT(this != nullptr);

    if (!RadarDrawNames && Map.SidebarIsDrawn) {
        Set_Logic_Page(&g_hidPage);

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

        Restore_Logic_Page();
    }
    */
}

void RadarClass::Radar_Cursor(BOOL redraw)
{
#ifndef RAPP_STANDALONE
    void (*func)(const RadarClass *, BOOL) = reinterpret_cast<void (*)(const RadarClass *, BOOL)>(0x00530300);
    func(this, redraw);
#endif
}

void RadarClass::Player_Names(BOOL draw)
{
    DEBUG_ASSERT(this != nullptr);

    RadarDrawNames = draw;
    Flag_To_Redraw(true);
}

BOOL RadarClass::Draw_House_Info()
{
#ifndef RAPP_STANDALONE
    BOOL (*func)(const RadarClass *) = reinterpret_cast<BOOL (*)(const RadarClass *)>(0x00532268);
    return func(this);
#endif
}

void RadarClass::Draw_Names()
{
#ifndef RAPP_STANDALONE
    void (*func)(const RadarClass *) = reinterpret_cast<void (*)(const RadarClass *)>(0x005325E4);
    func(this);
#endif
}

void RadarClass::Mark_Radar(int left, int top, int right, int bottom, BOOL mark, int size)
{
    DEBUG_ASSERT(this != nullptr);

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

void RadarClass::Cursor_Cell(int16_t cellnum, BOOL mark)
{
    DEBUG_ASSERT(this != nullptr);

    if (Cell_On_Radar(cellnum)) {
        CellClass *cell = &Map[cellnum];

        DEBUG_ASSERT(cell != nullptr);

        // If the cell isn't already marked the same way we want, change it and
        // redraw if situation warrants.
        if (cell->Get_Bit32() != mark) {
            cell->Set_Bit32(mark);

            if (!mark) {
                Plot_Radar_Pixel(cellnum);
            }
        }
    }
}

void RadarClass::Plot_Radar_Pixel(int16_t cellnum)
{
#ifndef RAPP_STANDALONE
    void (*func)(const RadarClass *, int16_t) = reinterpret_cast<void (*)(const RadarClass *, int16_t)>(0x0052F4C4);
    func(this, cellnum);
#endif
}
int RadarClass::Radar_Activate(int mode)
{
#ifndef RAPP_STANDALONE
    int (*func)(const RadarClass *, int) = reinterpret_cast<int (*)(const RadarClass *, int)>(0x0052D790);
    return func(this, mode);
#endif
}

BOOL RadarClass::Spy_Next_House()
{
    // TODO Requires HouseClass
#ifndef RAPP_STANDALONE
    BOOL (*func)(const RadarClass *) = reinterpret_cast<BOOL (*)(const RadarClass *)>(0x0053214C);
    return func(this);
#endif
    /*
    DEBUG_ASSERT(this != nullptr);

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
}