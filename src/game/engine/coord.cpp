/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Functions for handling the different distance and position representations in the engine.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "coord.h"
#include "target.h"
#include "lists.h"
#include "scenario.h"
#include <algorithm>

const coord_t AdjacentCoord[FACING_COUNT] = {
    0xFF000000,     // (-256,  0)       // NORTH
    0xFF000100,     // (-256,  +256)    // NORTH EAST
    0x00000100,     // (0   ,  +256)    // EAST
    0x01000100,     // (+256,  +256)    // SOUTH EAST
    0x01000000,     // (+256,  0)       // SOUTH
    0x0100FF00,     // (+256,  -256)    // SOUTH WEST
    0x0000FF00,     // (0   ,  -256)    // WEST
    0xFF00FF00      // (-256,  -256)    // NORTH WEST
};

const cell_t AdjacentCell[FACING_COUNT] = {
    -MAP_MAX_WIDTH,     // North
    -MAP_MAX_WIDTH + 1, // North East
    1,                  // East
    MAP_MAX_WIDTH + 1,  // South East
    MAP_MAX_WIDTH,      // South
    MAP_MAX_WIDTH - 1,  // South West
    -1,                 // West
    -MAP_MAX_WIDTH - 1  // North West
};

/**
 * Calculate the distance (in leptons) between two coords.
 *
 * 0x004AC41C
 */
int Coord_Distance(coord_t coord1, coord_t coord2)
{
    int ydiff = std::abs(Coord_Lepton_Y(coord1) - Coord_Lepton_Y(coord2));
    int xdiff = std::abs(Coord_Lepton_X(coord1) - Coord_Lepton_X(coord2));

    if (ydiff <= xdiff) {
        ydiff /= 2;
    } else {
        xdiff /= 2;
    }

    return ydiff + xdiff;
}

/**
 * Calculate the distance (in cells within the map bounds) between two cells.
 */
int Cell_Distance(cell_t cell1, cell_t cell2)
{
    int ydiff = std::abs(Cell_Get_Y(cell1) - Cell_Get_Y(cell2));
    int xdiff = std::abs(Cell_Get_X(cell1) - Cell_Get_X(cell2));

    if (ydiff <= xdiff) {
        ydiff /= 2;
    } else {
        xdiff /= 2;
    }

    return ydiff + xdiff;
}

/**
 * Moves the x and y values using fixed point trigonometry lookup tables. 
 *
 * 0x004AC814
 */
void Move_Point(int16_t &x, int16_t &y, DirType dir, uint16_t distance)
{
    // These tables are 7bit fixed point lookup tables representing the values of sine and cosine radian values.
    static const int8_t _cosinetable[256] = {
        0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45,
        48, 51, 54, 57, 59, 62, 65, 67, 70, 73, 75, 78, 80, 82, 85, 87,
        89, 91, 94, 96, 98, 100, 101, 103, 105, 107, 108, 110, 111, 113, 114, 116,
        117, 118, 119, 120, 121, 122, 123, 123, 124, 125, 125, 126, 126, 126, 126, 126,
        127, 126, 126, 126, 126, 126, 125, 125, 124, 123, 123, 122, 121, 120, 119, 118,
        117, 116, 114, 113, 112, 110, 108, 107, 105, 103, 102, 100, 98, 96, 94, 91,
        89, 87, 85, 82, 80, 78, 75, 73, 70, 67, 65, 62, 59, 57, 54, 51,
        48, 45, 42, 39, 36, 33, 30, 27, 24, 21, 18, 15, 12, 9, 6, 3,
        0, -3, -6, -9, -12, -15, -18, -21, -24, -27, -30, -33, -36, -39, -42, -45,
        -48, -51, -54, -57, -59, -62, -65, -67, -70, -73, -75, -78, -80, -82, -85, -87,
        -89, -91, -94, -96, -98, -100, -102, -103, -105, -107, -108, -110, -111, -113, -114, -116,
        -117, -118, -119, -120, -121, -122, -123, -123, -124, -125, -125, -126, -126, -126, -126, -126,
        -126, -126, -126, -126, -126, -126, -125, -125, -124, -123, -123, -122, -121, -120, -119, -118,
        -117, -116, -114, -113, -112, -110, -108, -107, -105, -103, -102, -100, -98, -96, -94, -91,
        -89, -87, -85, -82, -80, -78, -75, -73, -70, -67, -65, -62, -59, -57, -54, -51,
        -48, -45, -42, -39, -36, -33, -30, -27, -24, -21, -18, -15, -12, -9, -6, -3
    };

    static const int8_t _sintable[256] = {
        127, 126, 126, 126, 126, 126, 125, 125, 124, 123, 123, 122, 121, 120, 119, 118,
        117, 116, 114, 113, 112, 110, 108, 107, 105, 103, 102, 100, 98, 96, 94, 91,
        89, 87, 85, 82, 80, 78, 75, 73, 70, 67, 65, 62, 59, 57, 54, 51,
        48, 45, 42, 39, 36, 33, 30, 27, 24, 21, 18, 15, 12, 9, 6, 3,
        0, -3, -6, -9, -12, -15, -18, -21, -24, -27, -30, -33, -36, -39, -42, -45,
        -48, -51, -54, -57, -59, -62, -65, -67, -70, -73, -75, -78, -80, -82, -85, -87,
        -89, -91, -94, -96, -98, -100, -102, -103, -105, -107, -108, -110, -111, -113, -114, -116,
        -117, -118, -119, -120, -121, -122, -123, -123, -124, -125, -125, -126, -126, -126, -126, -126,
        -126, -126, -126, -126, -126, -126, -125, -125, -124, -123, -123, -122, -121, -120, -119, -118,
        -117, -116, -114, -113, -112, -110, -108, -107, -105, -103, -102, -100, -98, -96, -94, -91,
        -89, -87, -85, -82, -80, -78, -75, -73, -70, -67, -65, -62, -59, -57, -54, -51,
        -48, -45, -42, -39, -36, -33, -30, -27, -24, -21, -18, -15, -12, -9, -6, -3,
        0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45,
        48, 51, 54, 57, 59, 62, 65, 67, 70, 73, 75, 78, 80, 82, 85, 87,
        89, 91, 94, 96, 98, 100, 101, 103, 105, 107, 108, 110, 111, 113, 114, 116,
        117, 118, 119, 120, 121, 122, 123, 123, 124, 125, 125, 126, 126, 126, 126, 126
    };

    // The division by 128 is for a shift of 7 bits for a fixed point chop to int.
    // Note that this function uses 7 bit accuracy, not 8 bit like the fixed class.
    x += (_cosinetable[dir] * distance) / 128;
    y -= (_sintable[dir] * distance) / 128;
}

/**
 * Calculates a moved Coord relative to the source coord. 
 *
 * 0x004AC798
 */
coord_t Coord_Move(coord_t coord, DirType dir, uint16_t distance)
{
    int16_t x = Coord_Lepton_X(coord);
    int16_t y = Coord_Lepton_Y(coord);
    Move_Point(x, y, dir, distance);

    return Coord_From_Lepton_XY(x, y);
}

/**
 * Returns a coord that is randomly displaced from the provided coord.
 *
 * 0x004AC7C0
 */
coord_t Coord_Scatter(coord_t coord, uint16_t distance, BOOL center)
{
    DirType dir = (DirType)g_Scen.Get_Random_Value(DIR_FIRST, DIR_LAST);
    coord_t new_coord = Coord_Move(coord, dir, distance);

    if (Coord_Is_Negative(new_coord)) {
        new_coord = coord;
    }

    if (center) {
        new_coord = Coord_Centered(new_coord);
    }

    return new_coord;
}

/**
 * Moves the x and y values to lie within the confining dimensions passed. 
 *
 * 0x005CC697
 */
BOOL __cdecl Confine_Rect(int &x_pos, int &y_pos, int x, int y, int w, int h)
{
    BOOL confined = false;

    if ((-x_pos & (x_pos + x - w - 1)) >= 0) {
        if (x_pos > 0) {
            x_pos -= x_pos + x - w;
        } else {
            x_pos = 0;
        }

        confined = true;
    }

    if ((-y_pos & (y_pos + y - h - 1)) >= 0) {
        if (y_pos > 0) {
            y_pos -= y_pos + y - h;
        } else {
            y_pos = 0;
        }

        confined = true;
    }

    return confined;
}

/**
 * Returns an overlap list to cover all possible cells that might need updating given an objects size and position.
 */
const int16_t *Coord_Spillage_List(coord_t coord, int size)
{
    // clang-format off
    static const int16_t _gigundo[] = { 
        -254, -255, -256, -257, -258, -126, -127, -128, -129, -130,
           2,    1,    0,   -1,   -2,  126,  127,  128,  129,  130,
         254,  255,  256,  257,  258, LIST_END };

    static const uint8_t _pix_to_lepton[] = {
          0u,  11u,  21u,  32u,  43u,  53u,  64u,  75u,  85u,  96u, 107u,
        117u, 128u, 139u, 149u, 160u, 171u, 181u, 192u, 203u, 213u, 224u
    };

    static const int16_t _move_spillage[][5] = {
        { 0, -128, LIST_END, 0, 0 },
        { 0, -128, 1, -127, LIST_END },
        { 0, 1, LIST_END, 0, 0 },
        { 0, 1, 128, 129, LIST_END },
        { 0, 128, LIST_END, 0, 0 },
        { 0, -1, 128, 127, LIST_END },
        { 0, -1, LIST_END, 0, 0 },
        { 0, -1, -128, -129, LIST_END },
        { 0, LIST_END, 0, 0, 0 }
    };

    const int8_t _spill_table[16] = {
        8, 6, 2, -1, 0, 7, 1, -1, 4, 5, 3, -1, -1, -1, -1, -1
    };

    static int16_t _manual[10];
    // clang-format on

    // Its big, just use the big refresh list.
    if (size > CELL_PIXELS * 2) {
        return _gigundo;
    }

    if (size > CELL_PIXELS) {
        // Calculate the rect we need to consider.
        int adjust = std::min(size, CELL_PIXELS * 2) / 2;
        unsigned index = 0;

        int x = CELL_PIXELS * Coord_Sub_Cell_X(coord) / CELL_LEPTONS;
        int y = CELL_PIXELS * Coord_Sub_Cell_Y(coord) / CELL_LEPTONS;
        int x_lo = x - adjust;
        int x_hi = x + adjust;
        int y_lo = y - adjust;
        int y_hi = y + adjust;

        // Construct a refresh list based on where rect lies relative to cell boundaries.
        _manual[index++] = 0;

        // These are either side of the cell.
        if (x_lo < 0) {
            _manual[index++] = -1;
        }

        if (x_hi >= CELL_PIXELS) {
            _manual[index++] = 1;
        }

        // These are above and below.
        if (y_lo < 0) {
            _manual[index++] = -128;
        }

        if (y_hi >= CELL_PIXELS) {
            _manual[index++] = 128;
        }

        // These are the diagonal extents.
        if (x_lo < 0 && y_lo < 0) {
            _manual[index++] = -129;
        }

        if (x_hi >= CELL_PIXELS && y_hi >= CELL_PIXELS) {
            _manual[index++] = 129;
        }

        if (x_lo < 0 && y_hi >= CELL_PIXELS) {
            _manual[index++] = 127;
        }

        if (x_hi >= CELL_PIXELS && y_lo < 0) {
            _manual[index++] = -127;
        }

        _manual[index] = LIST_END;

        return _manual;
    }

    int ref = _pix_to_lepton[(CELL_PIXELS - size) / 2];
    int x = Coord_Sub_Cell_X(coord) - 128;
    int y = Coord_Sub_Cell_Y(coord) - 128;
    uint8_t index = 0;

    // Set flags for which border of a cell we cross to perform look up of correct list.
    if (y > ref) {
        index = 8;
    }

    if (y < -ref) {
        index |= 4;
    }

    if (x > ref) {
        index |= 2;
    }

    if (x < -ref) {
        index |= 1;
    }

    // This condition will result in an out of bounds read and probably shouldn't happen.
    captainslog_assert(_spill_table[index] != -1);

    return _move_spillage[_spill_table[index]]; 
}

/**
 * Returns an overlap list to cover all possible cells that might need updating given a position and enclosing rect.
 */
const int16_t *Coord_Spillage_List(coord_t coord, const TRect<int> &rect, BOOL skip_match)
{
    static const int16_t _list[2] = { LIST_END, 0 };
    static int16_t _spillage_list[128];

    if (!rect.Is_Valid()) {
        return _list;
    }

    cell_t cellnum = Coord_To_Cell(coord);
    lepton_t x = Pixel_To_Lepton(rect.m_left) + Coord_Lepton_X(coord);
    lepton_t y = Pixel_To_Lepton(rect.m_top) + Coord_Lepton_Y(coord);
    int cell_w = Lepton_To_Cell_Coord_Chop(Pixel_To_Lepton(rect.m_right - 1) + x);
    int cell_h = Lepton_To_Cell_Coord_Chop(Pixel_To_Lepton(rect.m_bottom - 1) + y);
    unsigned index = 0;

    for (int cell_y = Lepton_To_Cell_Coord_Chop(y); cell_y <= cell_h; ++cell_y) {
        for (int cell_x = Lepton_To_Cell_Coord_Chop(x); cell_x <= cell_w; ++cell_x) {
            cell_t cell_pos = Cell_From_XY(cell_x, cell_y);

            if (!skip_match || cell_pos != cellnum) {
                _spillage_list[index++] = cell_pos - cellnum;
                
                // Break leaving enough space for list end marker.
                if (index >= ARRAY_SIZE(_spillage_list) - 2) {
                    break;
                }
            }
        }

        // Break leaving enough space for list end marker.
        if (index >= ARRAY_SIZE(_spillage_list) - 2) {
            break;
        }
    }

    _spillage_list[index] = LIST_END;

    return _spillage_list;
}

DirType Coord_Direction(coord_t coord1, coord_t coord2)
{
    return Desired_Facing256(Coord_Lepton_X(coord1), Coord_Lepton_Y(coord1), Coord_Lepton_X(coord2), Coord_Lepton_Y(coord2));
}

DirType Target_Direction(target_t target1, target_t target2)
{
    return Coord_Direction(As_Coord(target1), As_Coord(target1));
}

DirType Cell_Direction(cell_t cell1, cell_t cell2)
{
    return Desired_Facing8(Cell_Get_X(cell1), Cell_Get_Y(cell1), Cell_Get_X(cell2), Cell_Get_Y(cell2));
}
