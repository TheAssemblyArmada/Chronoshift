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
#include "drawshape.h"
#include "fading.h"
#include "gameoptions.h"
#include "gbuffer.h"
#include "globals.h"
#include "iomap.h"
#include "lists.h"
#include "mixfile.h"
#include "palette.h"
#include "pk.h"
#include "rules.h"
#include "scenario.h"
#include "swap.h"
#include "technotype.h"
#include "target.h"
#include "textprint.h"
#include "theater.h"
#include <cstdio>

using std::snprintf;

#ifndef CHRONOSHIFT_STANDALONE
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
    // TODO, needs HouseClass, TechnoClass.
#ifndef CHRONOSHIFT_STANDALONE
    BOOL(*func)(const TacticalClass *, unsigned, KeyNumType &) = reinterpret_cast<BOOL(*)(const TacticalClass *, unsigned, KeyNumType &)>(0x004B3108);
    return func(this, flags, key);
#else
    return false;
#endif
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
#ifndef CHRONOSHIFT_STANDALONE
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
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(const DisplayClass *, BOOL) = reinterpret_cast<void (*)(const DisplayClass *, BOOL)>(0x004B0CA8);
    func(this, force_redraw);
#endif
}

void DisplayClass::Code_Pointers()
{
    // Requires target functions.
#ifndef CHRONOSHIFT_STANDALONE
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
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(const DisplayClass *) = reinterpret_cast<void (*)(const DisplayClass *)>(0x004F9220);
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
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(const DisplayClass *, CCINIClass &) =
        reinterpret_cast<void (*)(const DisplayClass *, CCINIClass &)>(0x004B50F8);
    func(this, ini);
#endif
}

void DisplayClass::Write_INI(CCINIClass &ini)
{
    // Needs celltriggerclass
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(const DisplayClass *, CCINIClass &) =
        reinterpret_cast<void (*)(const DisplayClass *, CCINIClass &)>(0x004B545C);
    func(this, ini);
#endif
}

BOOL DisplayClass::Map_Cell(int16_t cellnum, HouseClass *house)
{
    // TODO requires HouseClass
#ifndef CHRONOSHIFT_STANDALONE
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
    int xpos = (uint16_t)Pixel_To_Lepton(x - TacOffsetX);
    int ypos = (uint16_t)Pixel_To_Lepton(y - TacOffsetY);

    if (xpos < DisplayWidth && ypos < DisplayHeight) {
        return Coord_To_Cell(Coord_From_Lepton_XY(xpos + Lepton_Round_To_Pixel(Coord_Lepton_X(DisplayPos)), ypos + Lepton_Round_To_Pixel(Coord_Lepton_Y(DisplayPos))));
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

void DisplayClass::Mouse_Right_Press()
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
#ifndef CHRONOSHIFT_STANDALONE
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
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(const DisplayClass *, int16_t, int, int, ObjectClass *, ActionType, BOOL) =
        reinterpret_cast<void (*)(const DisplayClass *, int16_t, int, int, ObjectClass *, ActionType, BOOL)>(0x004B3CA8);
    func(this, cellnum, mouse_x, mouse_y, object, action, mouse_in_radar);
#endif
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

/**
 * @brief Sets the occupy list for the cells the band box currently covers.
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
            Swap(box_x, box_w);
        }

        if (box_y > box_h) {
            Swap(box_y, box_h);
        }

        // This loop should redraw the left and right side of the selection band.
        for (int i = box_y; i <= (box_h + 24); i += 24) {
            int b_pxl = Lepton_To_Pixel(DisplayHeight) + TacOffsetY;
            int16_t click_cell = Click_Cell_Calc(box_x, Clamp(i, 0, b_pxl));
            
            if (click_cell != -1) {
                Array[click_cell].Redraw_Objects();
            }

            click_cell = Click_Cell_Calc(box_w, Clamp(i, 0, b_pxl));

            if (click_cell != -1) {
                Array[click_cell].Redraw_Objects();
            }
        }

        // This loop should redraw the top and bottom side of the selection band.
        for (int i = box_x; i < (box_w + 24); i += 24) {
            int b_pxl = Lepton_To_Pixel(DisplayWidth) + TacOffsetX;
            int16_t click_cell = Click_Cell_Calc(Clamp(i, 0, b_pxl), box_y);
            
            if (click_cell != -1) {
                Array[click_cell].Redraw_Objects();
            }

            click_cell = Click_Cell_Calc(Clamp(i, 0, b_pxl), box_h);
            
            if (click_cell != -1) {
                Array[click_cell].Redraw_Objects();
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

        if (str_width + x >= width) {
            str_width = width - x;
            *w_list = LIST_START;
            --count;
            ++w_list;
        }

        if (width >= x) {
            int16_t click_cell = Click_Cell_Calc(x, y - 1);
            int height = Clamp(y + 24, TacOffsetY, TacOffsetY + (Lepton_To_Pixel(DisplayHeight) - 1));
            int16_t offset_click = Click_Cell_Calc(str_width + x - 1, height);

            if (click_cell == -1) {
                click_cell = Click_Cell_Calc(x, y);
            }

            if (click_cell != -1 && offset_click != -1) {
                // If we have two valid cells locations, iterate through the cells between them.
                for (int i = Cell_Get_Y(click_cell); i <= Cell_Get_Y(offset_click); ++i) {
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

ObjectClass *DisplayClass::Cell_Object(int16_t cellnum, int x, int y) const
{
    return Array[cellnum].Cell_Object(x, y);
}

void DisplayClass::Select_These(uint32_t start, uint32_t finish)
{
    // Needs HouseClass.
#ifndef CHRONOSHIFT_STANDALONE
    void(*func)(const DisplayClass *, uint32_t, uint32_t) = reinterpret_cast<void(*)(const DisplayClass *, uint32_t, uint32_t)>(0x004B2C50);
    func(this, start, finish);
#endif
}

void DisplayClass::Sell_Mode_Control(int mode)
{
    // Needs HouseClass.
#ifndef CHRONOSHIFT_STANDALONE
    void(*func)(const DisplayClass *, int) = reinterpret_cast<void(*)(const DisplayClass *, int)>(0x004B4B68);
    func(this, mode);
#elif 0
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

        if (flag && !PlayerPtr->field_137.All_False()) {
            DisplaySellMode = true;
            Unselect_All();
        } else {
            DisplaySellMode = false;
            Revert_Mouse_Shape();
        }
    }
#endif
}

void DisplayClass::Repair_Mode_Control(int mode)
{
    // Needs HouseClass.
#ifndef CHRONOSHIFT_STANDALONE
    void(*func)(const DisplayClass *, int) = reinterpret_cast<void(*)(const DisplayClass *, int)>(0x004B4C10);
    func(this, mode);
#elif 0
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

        if (flag && !PlayerPtr->field_137.All_False()) {
            DisplayRepairMode = true;
            Unselect_All();
        } else {
            DisplayRepairMode = false;
            Revert_Mouse_Shape();
        }
    }
#endif
}

/**
 * @brief Centers display to given coord.
 *
 * 0x004B4E20
 */
void DisplayClass::Center_Map(uint32_t coord)
{
    bool coord_set = false;
    int x = 0;
    int y = 0;

    // Calculate a position based on units selected, used if no coord is passed.
    if (CurrentObjects.Count() > 0) {
        for (int i = 0; i < CurrentObjects.Count(); ++i) {
            uint32_t center = CurrentObjects[i]->Center_Coord();
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
        int16_t x_lep = Max((int16_t)(x - DisplayWidth / 2), Coord_Cell_To_Lepton(MapCellX));
        int16_t y_lep = Max((int16_t)(y - DisplayHeight / 2), Coord_Cell_To_Lepton(MapCellY));
        Set_Tactical_Position(Coord_From_Lepton_XY(x_lep, y_lep));
    }
}

/**
 * @brief Registers the cell in the band box cursor list, calculates from mouse pos if cell is -1.
 *
 * 0x004AFD40
 */
int16_t DisplayClass::Set_Cursor_Pos(int16_t cell)
{
    int16_t retval;

    if (cell == -1) {
        cell = Click_Cell_Calc(g_mouse->Get_Mouse_X(), g_mouse->Get_Mouse_Y());
    }

    if (DisplayCursorOccupy != nullptr) {
        int occupy_x;
        int occupy_y;

        Get_Occupy_Dimensions(occupy_x, occupy_y, DisplayCursorOccupy);

        int cell_x = Max(Cell_Get_X(DisplayCursorEnd + cell), Coord_Cell_X(DisplayPos));
        int cell_y = Max(Cell_Get_Y(DisplayCursorEnd + cell), Coord_Cell_Y(DisplayPos));

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
                PendingObjectTypePtr, PendingObjectHouse, DisplayCursorOccupy, DisplayCursorEnd + retval);

            Swap(DisplayCursorStart, retval);
        }
    } else {
        retval = DisplayCursorStart;
        DisplayCursorStart = cell;
    }

    return retval;
}

BOOL DisplayClass::Passes_Proximity_Check(ObjectTypeClass *object, HousesType house, int16_t *list, int16_t cell) const
{
    // Needs HouseClass, BuildingTypeClass.
#ifndef CHRONOSHIFT_STANDALONE
    BOOL(*func)(const DisplayClass *, ObjectTypeClass *, HousesType, int16_t *, int16_t) = reinterpret_cast<BOOL(*)(const DisplayClass *, ObjectTypeClass *, HousesType, int16_t *, int16_t)>(0x004AF7DC);
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
            int16_t cellnum = Coord_To_Cell(Coord_Add(DisplayPos, Coord_From_Lepton_XY(x, y)));
            uint32_t coord = Cell_To_Coord(cellnum) & 0xFF00FF00;
            int draw_x = 0;
            int draw_y = 0;

            // Check conditions for current position warrant attempting a redraw.
            if (In_View(cellnum) && Is_Cell_Flagged(cellnum) && Coord_To_Pixel(coord, draw_x, draw_y)) {
                CellClass &cell = Array[Coord_To_Cell(coord)];

                if (cell.Is_Visible() || DebugUnshroud) {
                    cell.Draw_It(draw_x, draw_y, false);
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
                int16_t cellnum = Coord_To_Cell(Coord_Add(DisplayPos, Coord_From_Lepton_XY(x, y)));
                uint32_t coord = Coord_Top_Left(Cell_To_Coord(cellnum));
                int draw_x = 0;
                int draw_y = 0;

                if (In_View(cellnum) && Is_Cell_Flagged(cellnum) && Coord_To_Pixel(coord, draw_x, draw_y)) {
                    CellClass &cell = Array[Coord_To_Cell(coord)];

                    if (!cell.Is_Revealed()) {
                        int frame = -2;
                            
                        if (cell.Is_Visible()) {
                            frame = Cell_Shadow(cellnum);
                        }

                        if (frame >= 0) { // If we have a valid frame, draw the shroud frame.
                            CC_Draw_Shape(
                                ShadowShapes, frame, draw_x, draw_y, WINDOW_TACTICAL, SHAPE_SHADOW, nullptr, ShadowTrans);
                        } else if (frame != -1) { // Otherwise, if we don't have -1 fill black
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
BOOL DisplayClass::In_View(int16_t cellnum) const
{
    // Check high bits aren't set, ensures not negative and is within range of 128 * 128 map.
    if ((cellnum & 0xC000) != 0) {
        return false;
    }

    uint32_t cell = Coord_Top_Left(Cell_To_Coord(cellnum));
    uint32_t loc1 = Coord_Top_Left(DisplayPos);

    // Makes use of unsigned underflow to detect if we are greater than DisplayPos but within display dimensions in two compares.
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
BOOL DisplayClass::Coord_To_Pixel(uint32_t coord, int &x, int &y) const
{
    if (coord != 0) {
        int16_t view_x = Lepton_Round_To_Pixel(Coord_Lepton_X(DisplayPos));
        int16_t view_y = Lepton_Round_To_Pixel(Coord_Lepton_Y(DisplayPos));
        int16_t lep_x = Lepton_Round_To_Pixel(Coord_Lepton_X(coord)) + 512 - view_x;
        int16_t lep_y = Lepton_Round_To_Pixel(Coord_Lepton_Y(coord)) + 512 - view_y;

        if (lep_x <= (DisplayWidth + 1024) && lep_y <= (DisplayHeight + 1024)) {
            x = Lepton_To_Pixel(lep_x) - 48;
            y = Lepton_To_Pixel(lep_y) - 48;

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
uint32_t DisplayClass::Pixel_To_Coord(int x, int y) const
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
int DisplayClass::Cell_Shadow(int16_t cellnum) const
{
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

    int16_t cell_x = Cell_Get_X(cellnum);
    int16_t cell_y = Cell_Get_Y(cellnum);

    // Check we are in bounds, SS checks all 4 map bounds, RA only checks right edge bound with if ( cell_x - 1 < 127 ).
    if (cell_x >= 1 && cell_x < 127 && cell_y >= 1 && cell_y < 127) { // SS extended check.
        // Use pointer here rather than reference as we are going to do some pointer arithmetic on it.
        CellClass const *cell = &Array[cellnum];

        // If we aren't at least partly revealed, then no need to do anything else
        if (!cell->Is_Revealed() && !cell->Is_Visible()) {
            return -2;
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
    return -1;
}

/**
 * @brief Not sure exactly what this one does.
 *
 * 0x004B0B10
 */
BOOL DisplayClass::Push_Onto_TacMap(uint32_t &coord1, uint32_t &coord2)
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
    coord1 = Coord_From_Lepton_XY(Clamp(coord1_x, start_x, end_x), Clamp(coord1_y, start_y, end_y));
    coord2 = Coord_From_Lepton_XY(Clamp(coord2_x, start_x, end_x), Clamp(coord2_y, start_y, end_y));

    return true;
}

/**
 * 0x004B274C
 */
int16_t DisplayClass::Calculated_Cell(SourceType source, int waypoint, int16_t cellnum, SpeedType speed, BOOL use_zone, MZoneType mzone) const
{
    int16_t cell_num = -1;
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
        int x_pos = Min(Cell_Get_X(cell_num) - MapCellX, MapCellWidth + MapCellX - Cell_Get_X(cell_num));

        int y_pos = Min(Cell_Get_Y(cell_num) - MapCellY, MapCellHeight + MapCellY - Cell_Get_Y(cell_num));

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
            int16_t test_cell = Cell_From_XY(MapCellX + x, MapCellY + (ny % MapCellHeight));

            if (Good_Reinforcement_Cell(test_cell, test_cell + offset, speed, zone, mzone)) {
                return test_cell;
            }

            ++ny;
        }

        if (y_clipped && MapCellWidth > 0) {
            offset = y <= MapCellY ? 128 : -128;
            int nx = x;

            for (int i = 0; i < MapCellWidth; ++i) {
                int16_t test_cell = Cell_From_XY(MapCellX + (nx % MapCellWidth), MapCellY + y);

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
            int16_t test_cell = Cell_From_XY(MapCellX + (nx % MapCellWidth), MapCellY + y);

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
BOOL DisplayClass::Good_Reinforcement_Cell(int16_t cell1, int16_t cell2, SpeedType speed, int zone, MZoneType mzone) const
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
uint32_t DisplayClass::Closest_Free_Spot(uint32_t coord, BOOL skip_occupied) const
{
    if (!Coord_Is_Negative(coord)) {
        return 0x800080; // Middle of cell 0, the top left cell off the edge of visible map.
    }

    int16_t cellnum = Coord_To_Cell(coord);

    return Array[cellnum].Closest_Free_Spot(coord, skip_occupied);

}

/**
 * @brief Sets all objects to "look"?.
 *
 * 0x004B5680
 */
void DisplayClass::All_To_Look(BOOL skip_buildings)
{
    // Needs HouseClass, TechnoClass.
#ifndef CHRONOSHIFT_STANDALONE
    void(*func)(const DisplayClass *, BOOL) = reinterpret_cast<void(*)(const DisplayClass *, BOOL)>(0x004B5680);
    func(this, skip_buildings);
#elif 0
    for (int i = 0; i < DisplayClass::Layers[LAYER_GROUND].Count(); ++i) {
        ObjectClass *objptr = DisplayClass::Layers[LAYER_GROUND][i];

        if (objptr != nullptr && objptr->Is_Techno()) {
            if (objptr->What_Am_I() != RTTI_BUILDING || !skip_buildings) {
                TechnoClass *tcptr = reinterpret_cast<TechnoClass *>(objptr);

                if (tcptr->OwnerHouse->PlayerControl) {
                    if (tcptr->PlayerAware) {
                        tcptr->Look();
                    }
                } else if (objptr->What_Am_I() == RTTI_BUILDING) {
                    if (Rule.Ally_Reveal()) {
                        if (PlayerPtr->Is_Ally(objptr->OwnerHouse)) {
                            objptr->Look();
                        }
                    }
                }
            }
        }
    }
#endif
}

/**
 * @brief 
 *
 * 0x004B5788
 */
void DisplayClass::Constrained_Look(uint32_t coord, int constraint)
{
    // Needs HouseClass, TechnoClass.
#ifndef CHRONOSHIFT_STANDALONE
    void(*func)(const DisplayClass *, uint32_t, int) = reinterpret_cast<void(*)(const DisplayClass *, uint32_t, int)>(0x004B5788);
    func(this, coord, constraint);
#elif 0
    for (int index = 0; index < DisplayClass::Layers[LAYER_GROUND].Count(); ++index) {
        ObjectClass *objptr = DisplayClass::Layers[LAYER_GROUND][index];

        if (objptr != nullptr && objptr->Is_Techno()) {
            TechnoClass *tcptr = reinterpret_cast<TechnoClass *>(objptr);

            if (tcptr->OwnerHouse->PlayerControl) {
                if (tcptr->PlayerAware) {
                    int lepton_sight = constraint * (reinterpret_cast<TechnoTypeClass const &>(tcptr->Class_Of()).Sight * 256);

                    if (Distance(tcptr->Center_Coord(), coord) <= lepton_sight) {
                        tcptr->Look();
                    }
                }
            } else if (tcptr->What_Am_I() == RTTI_BUILDING) {
                if (Rule.Ally_Reveal()) {
                    if (PlayerPtr->Is_Ally(tcptr->OwnerHouse->What_Type())) {
                        int lepton_sight = constraint * (reinterpret_cast<TechnoTypeClass const &>(tcptr->Class_Of()).Sight * 256);

                        if (Distance(tcptr->Center_Coord(), coord) <= lepton_sight) {
                            tcptr->Look();
                        }
                    }
                }
            }
        }
    }
#endif
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
void DisplayClass::Shroud_Cell(int16_t cellnum)
{
    // Needs HouseClass, TechnoClass.
#ifndef CHRONOSHIFT_STANDALONE
    void(*func)(const DisplayClass *, int16_t) = reinterpret_cast<void(*)(const DisplayClass *, int16_t)>(0x004B4FF4);
    func(this, cellnum);
#elif 0
    // If player has GPS or has units in the cell, then don't do anything.
    if (!PlayerPtr->GPSActive || Array[cellnum].field_A[PlayerPtr->What_Type()] == false) { //TODO, needs confirming
        if (In_Radar(cellnum)) {
            CellClass &cell = Array[cellnum];

            if (cell.Visible) {
                cell.Visible = false;
                cell.Revealed = false;
                cell.Redraw_Objects();

                for (FacingType facing = FACING_FIRST; facing < FACING_COUNT; ++facing) {
                    int16_t adjcell = cellnum + AdjacentCell[facing];

                    if (adjcell != cellnum) {
                        Array[adjcell].Revealed = false;
                    }

                    Array[adjcell].Redraw_Objects();
                }
            }
        }
    }
#endif
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
