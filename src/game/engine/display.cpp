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
#include "display.h"
#include "abs.h"
#include "ccfileclass.h"
#include "coord.h"
#include "fading.h"
#include "gameoptions.h"
#include "gbuffer.h"
#include "globals.h"
#include "iomap.h"
#include "lists.h"
#include "mixfile.h"
#include "palette.h"
#include "pk.h"
#include "scenario.h"
#include "swap.h"
#include "target.h"
#include "textprint.h"
#include "theater.h"
#include <cstdio>

using std::snprintf;

#ifndef RAPP_STANDALONE
LayerClass *DisplayClass::Layers = Make_Pointer<LayerClass>(0x00656080);
DisplayClass::TacticalClass &DisplayClass::TacticalButton = Make_Global<DisplayClass::TacticalClass>(0x00658804);
char *DisplayClass::FadingBrighten = Make_Pointer<char>(0x006560E0);
char *DisplayClass::FadingShade = Make_Pointer<char>(0x006561E0);
char *DisplayClass::FadingWayDark = Make_Pointer<char>(0x006562E0);
char *DisplayClass::FadingLight = Make_Pointer<char>(0x006563E0);
char *DisplayClass::FadingGreen = Make_Pointer<char>(0x006564E0);
char *DisplayClass::FadingYellow = Make_Pointer<char>(0x006565E0);
char *DisplayClass::FadingRed = Make_Pointer<char>(0x006566E0);
char *DisplayClass::TranslucentTable = Make_Pointer<char>(0x006567E0);
char *DisplayClass::WhiteTranslucentTable = Make_Pointer<char>(0x006575E0);
char *DisplayClass::MouseTranslucentTable = Make_Pointer<char>(0x006577E0);
char *DisplayClass::ShadowTrans = Make_Pointer<char>(0x006582E8);
char *DisplayClass::UnitShadow = Make_Pointer<char>(0x00657CE4);
char *DisplayClass::UnitShadowAir = Make_Pointer<char>(0x00657EE4);
char *DisplayClass::SpecialGhost = Make_Pointer<char>(0x006580E4);
void *&DisplayClass::TransIconset = Make_Global<void *>(0x00657CE0);
void *&DisplayClass::ShadowShapes = Make_Global<void *>(0x006582E4);
BufferClass *&DisplayClass::TheaterBuffer = Make_Global<BufferClass *>(0x006680E4);
BooleanVectorClass &DisplayClass::CellRedraw = Make_Global<BooleanVectorClass>(0x006587E8);
#else
LayerClass DisplayClass::Layers[LAYER_COUNT];
TacticalClass DisplayClass::TacticalButton;
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

DisplayClass::TacticalClass::TacticalClass() :
    GadgetClass(0, 0, 0, 0, MOUSE_LEFT_PRESS | MOUSE_LEFT_HELD | MOUSE_LEFT_RLSE | MOUSE_LEFT_UP | MOUSE_RIGHT_PRESS, true)
{
}

BOOL DisplayClass::TacticalClass::Action(unsigned flags, KeyNumType &key)
{
    return false;
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
    PendingObjectHouse(HOUSES_NONE),
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
    PendingObjectHouse = HOUSES_NONE;
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
    static MixFileClass<CCFileClass> *_theater_data = nullptr;

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
        
        _theater_data = new MixFileClass<CCFileClass>(mix_name, &g_publicKey);
        _theater_data->Cache(TheaterBuffer);
    }

    char pal_name[32];
    snprintf(pal_name, sizeof(pal_name), "%s.pal", g_theaters[theater].data);
    
    GamePalette = *static_cast<PaletteClass *>(MixFileClass<CCFileClass>::Retrieve(pal_name));
    OriginalPalette = GamePalette;

    Build_Fading_Table(GamePalette, FadingGreen, 3, 110);
    Build_Fading_Table(GamePalette, FadingYellow, 5, 140);
    Build_Fading_Table(GamePalette, FadingRed, 8, 140);
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

    // Special case where pointer and array of arrays don't behave the same for our purpose.
#ifndef RAPP_STANDALONE
    Conquer_Build_Fading_Table(GamePalette, &SpecialGhost[256], 12, 100);
#else
    Conquer_Build_Fading_Table(GamePalette, SpecialGhost[1], 12, 100);
#endif
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
    // TODO Requires ChronalVortexClass
#ifndef RAPP_STANDALONE
    void (*func)(const DisplayClass *, BOOL) = reinterpret_cast<void (*)(const DisplayClass *, BOOL)>(0x004B0CA8);
    func(this, force_redraw);
#endif
}

void DisplayClass::Code_Pointers()
{
    // Requires target functions.
#ifndef RAPP_STANDALONE
    void (*func)(const DisplayClass *) = reinterpret_cast<void (*)(const DisplayClass *)>(0x004F91DC);
    func(this);
#elif 0
    if (PendingObjectPtr != nullptr) {
        PendingObjectPtr = (ObjectClass *)As_Target(PendingObjectPtr);
        DEBUG_ASSERT(PendingObjectPtr != nullptr);
    }

    MapClass::Code_Pointers();
#endif
}

void DisplayClass::Decode_Pointers()
{
    // Requires target functions.
#ifndef RAPP_STANDALONE
    void (*func)(const DisplayClass *) = reinterpret_cast<void (*)(const DisplayClass *)>(0x004F91DC);
    func(this);
#elif 0
    if ((uintptr_t)PendingObjectPtr != 0) {
        PendingObjectPtr = As_Object((uintptr)PendingObjectPtr);
    }

    MapClass::Decode_Pointers();
#endif
}

void DisplayClass::Read_INI(CCINIClass &ini)
{
    // Needs all type classes, celltriggerclass
#ifndef RAPP_STANDALONE
    void (*func)(const DisplayClass *, CCINIClass &) =
        reinterpret_cast<void (*)(const DisplayClass *, CCINIClass &)>(0x004B50F8);
    func(this, ini);
#endif
}

void DisplayClass::Write_INI(CCINIClass &ini)
{
    // Needs celltriggerclass
#ifndef RAPP_STANDALONE
    void (*func)(const DisplayClass *, CCINIClass &) =
        reinterpret_cast<void (*)(const DisplayClass *, CCINIClass &)>(0x004B545C);
    func(this, ini);
#endif
}

BOOL DisplayClass::Map_Cell(int16_t cellnum, HouseClass *house)
{
    // TODO requires HouseClass
#ifndef RAPP_STANDALONE
    BOOL(*func)
    (const DisplayClass *, int16_t, HouseClass *) =
        reinterpret_cast<BOOL (*)(const DisplayClass *, int16_t, HouseClass *)>(0x004B0788);
    return func(this, cellnum, house);
#else
    return false;
#endif
}

int16_t DisplayClass::Click_Cell_Calc(int x, int y) const
{
    int xpos = Pixel_To_Lepton(x - TacOffsetX);
    int ypos = Pixel_To_Lepton(y - TacOffsetY);

    if (xpos < DisplayWidth && ypos < DisplayHeight) {
        uint32_t coord1 =
            Coord_From_Pixel_XY(Lepton_To_Pixel(Coord_Lepton_X(DisplayPos)), Lepton_To_Pixel(Coord_Lepton_Y(DisplayPos)));
        uint32_t coord2 = Coord_From_Lepton_XY(xpos, ypos);

        return Coord_To_Cell(Coord_Add(coord1, coord2));
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

        uint32_t new_pos = Coord_Move(DisplayPos, dir, distance);
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

        uint32_t pos_coord =
            Coord_From_Lepton_XY(x_pos + Coord_Cell_To_Lepton(MapCellX), y_pos + Coord_Cell_To_Lepton(MapCellY));

        // Update distance if the move was constrained.
        if (confined) {
            distance = Distance(DisplayPos, pos_coord);
        }

        // If we actually moved, redraw.
        if (distance > 0 && pos_coord != DisplayPos) {
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
void DisplayClass::Refresh_Cells(int16_t cellnum, int16_t *overlap_list)
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

            int16_t refresh_cell = cellnum + tmp_list[i];

            if (In_Radar(refresh_cell)) {
                (*this)[refresh_cell].Redraw_Objects();
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

    uint32_t tacpos_coord = Coord_From_Lepton_XY(x_pos + (MapCellX * 256), y_pos + (MapCellY * 256));

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
void DisplayClass::Set_Tactical_Position(uint32_t location)
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
void DisplayClass::Flag_Cell(int16_t cellnum)
{
    CellRedraw[cellnum] = true;
    DisplayToRedraw = true;
    Flag_To_Redraw();
}

void DisplayClass::Mouse_Right_Press(int mouse_x, int mouse_y)
{
    if (PendingObjectPtr != nullptr && PendingObjectPtr->Is_Techno()) {
        PendingObjectPtr = nullptr;
        PendingObjectTypePtr = nullptr;
        PendingObjectHouse = HOUSES_NONE;
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

    Set_Default_Mouse(MOUSE_POINTER);
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
    int16_t cellnum, BOOL cell_shrouded, ObjectClass *object, ActionType action, BOOL mouse_in_radar)
{
#ifndef RAPP_STANDALONE
    void (*func)(const DisplayClass *, int16_t, BOOL, ObjectClass *, ActionType, BOOL) =
        reinterpret_cast<void (*)(const DisplayClass *, int16_t, BOOL, ObjectClass *, ActionType, BOOL)>(0x004B3780);
    func(this, cellnum, cell_shrouded, object, action, mouse_in_radar);
#endif
}

void DisplayClass::Mouse_Left_Held(int mouse_x, int mouse_y)
{
    if (DisplayBit8) {
        if (mouse_x != BandBox.m_right || mouse_y != BandBox.m_bottom) {
            Refresh_Band();
            // Need confirming, check between sole and ra!
            BandBox.m_right = Clamp(mouse_x, 0, Lepton_To_Pixel(DisplayWidth) - 1);
            BandBox.m_bottom = Clamp(mouse_y, 0, Lepton_To_Pixel(DisplayHeight) - 1);
            DisplayToRedraw = true;
            Flag_To_Redraw();
        }
    } else if (DisplayBit16) {
        // Ensure we actually drew a bandbox of usable size?
        if (Abs(BandBox.m_left - mouse_x) > 4 || Abs(BandBox.m_top - mouse_y) > 4) {
            DisplayBit8 = true;
            BandBox.m_right = Clamp(mouse_x, 0, Lepton_To_Pixel(DisplayWidth) - 1);
            BandBox.m_bottom = Clamp(mouse_y, 0, Lepton_To_Pixel(DisplayHeight) - 1);
            DisplayToRedraw = true;
            Flag_To_Redraw();

            Layers[LAYER_TOP].Mark_All(MARK_REDRAW);
            Layers[LAYER_AIR].Mark_All(MARK_REDRAW);
        }
    }
}

void DisplayClass::Mouse_Left_Release(
    int16_t cellnum, int mouse_x, int mouse_y, ObjectClass *object, ActionType action, BOOL mouse_in_radar)
{
#ifndef RAPP_STANDALONE
    void (*func)(const DisplayClass *, int16_t, int, int, ObjectClass *, ActionType, BOOL) =
        reinterpret_cast<void (*)(const DisplayClass *, int16_t, int, int, ObjectClass *, ActionType, BOOL)>(0x004B3CA8);
    func(this, cellnum, mouse_x, mouse_y, object, action, mouse_in_radar);
#endif
}

/**
 * @brief Initialise data that doesn't change once loaded during runtime.
 *
 * 0x004AEEF4
 */
void DisplayClass::One_Time() {
    MapClass::One_Time();

    // Resize the cell redraw vector to cover the map.
    CellRedraw.Resize(MAP_MAX_AREA);

    for (LayerType layer = LAYER_FIRST; layer < LAYER_COUNT; ++layer) {
        Layers[layer].One_Time();
    }

    TransIconset = MixFileClass<CCFileClass>::Retrieve("trans.icn");
    DEBUG_ASSERT(TransIconset != nullptr);

    ShadowShapes = MixFileClass<CCFileClass>::Retrieve("shadow.shp");
    DEBUG_ASSERT(ShadowShapes != nullptr);

    Set_View_Dimensions(0, 16);
}

/**
 * @brief Converts a pixel on screen to the logical location in the engine.
 *
 * 0x004B2694
 */
uint32_t DisplayClass::Pixel_To_Coord(int x, int y)
{
    int x_pos = Pixel_To_Lepton(x - TacOffsetX);
    int y_pos = Pixel_To_Lepton(y - TacOffsetY);

    if (x_pos < DisplayWidth && y_pos < DisplayHeight) {
        return Coord_From_Lepton_XY(x_pos + Coord_Lepton_X(DisplayPos), y_pos + Coord_Lepton_Y(DisplayPos));
    }

    return 0;
}

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
            Swap(box_x, box_w);
        }

        if (box_y > box_h) {
            Swap(box_y, box_h);
        }

        // This loop should redraw the left and right side of the selection band.
        for (int i = box_y; i < (box_h + 24); i += 24) {
            int b_pxl = Lepton_To_Pixel(DisplayHeight) + TacOffsetY;
            int16_t click_cell = Click_Cell_Calc(box_x, Clamp(i, 0, b_pxl));

            if (click_cell != -1) {
                Map[click_cell].Redraw_Objects();
            }

            click_cell = Click_Cell_Calc(box_w, Clamp(i, 0, b_pxl));

            if (click_cell != -1) {
                Map[click_cell].Redraw_Objects();
            }
        }

        // This loop should redraw the top and bottom side of the selection band.
        for (int i = box_x; i < (box_w + 24); i += 24) {
            int b_pxl = Lepton_To_Pixel(DisplayWidth) + TacOffsetX;
            int16_t click_cell = Click_Cell_Calc(Clamp(i, 0, b_pxl), box_y);

            if (click_cell != -1) {
                Map[click_cell].Redraw_Objects();
            }

            click_cell = Click_Cell_Calc(Clamp(i, 0, b_pxl), box_h);

            if (click_cell != -1) {
                Map[click_cell].Redraw_Objects();
            }
        }

        Layers[LAYER_TOP].Mark_All(MARK_REDRAW);
        Layers[LAYER_AIR].Mark_All(MARK_REDRAW);
    }
}

void DisplayClass::Cursor_Mark(int16_t cellnum, BOOL flag)
{
    // Check we have a valid cellnum
    if (cellnum == -1) {
        return;
    }

    for (int16_t *offsets = DisplayCursorOccupy; *offsets != LIST_END; ++offsets) {
        int16_t offset_cell = cellnum + *offsets;

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

            left = Max(left, cell_x);
            right = Min(right, cell_x);
            top = Max(top, cell_y);
            bottom = Min(bottom, cell_y);
            ++list;
        }

        w = Max(1, left - right + 1);
        h = Max(1, top - bottom + 1);
    }
}

/**
 * @brief Creates an overlap list of the cells that will be obscured by text at a given position.
 *
 * 0x004AF2D8
 */
const int16_t *DisplayClass::Text_Overlap_List(char const *string, int x, int y) const
{
    static int16_t _list[60];
    int16_t *w_list = _list;

    if (string != nullptr) {
        int count = 60;
        int str_width = String_Pixel_Width(string) + 24;
        int width = TacOffsetX + Lepton_To_Pixel(DisplayWidth);

        if ((str_width + x) > width) {
            str_width = width - x;
            w_list[0] = LIST_START;
            --count;
            ++w_list;
        }

        if (x <= width) {
            int16_t click_cell = Click_Cell_Calc(x, y - 1);
            int height = Clamp(y + 24, TacOffsetY, TacOffsetY + (Lepton_To_Pixel(DisplayHeight) - 1));
            int16_t offset_click = Click_Cell_Calc(width + x - 1, height);

            if (click_cell == -1) {
                click_cell = Click_Cell_Calc(x, y);
            }

            if (click_cell != -1 && offset_click != -1) {
                // If we have two valid cells locations, iterate through the cells between them.
                for (int i = Cell_Get_Y(click_cell); i < Cell_Get_Y(offset_click); ++i) {
                    for (int j = Cell_Get_X(click_cell); j <= Cell_Get_X(offset_click); ++j) {
                        int16_t current_cell = Coord_To_Cell(DisplayPos);
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