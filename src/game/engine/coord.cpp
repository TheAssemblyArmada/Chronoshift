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
#include "scenario.h"

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
 * Calculate the distance between two Coords. 
 *
 * 0x004AC41C
 */
int Distance(coord_t coord1, coord_t coord2)
{
    int ydiff;
    int xdiff;

    ydiff = std::abs(Coord_Lepton_Y(coord1) - Coord_Lepton_Y(coord2));
    xdiff = std::abs(Coord_Lepton_X(coord1) - Coord_Lepton_X(coord2));

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
    DirType dir = (DirType)Scen.Get_Random_Value(DIR_FIRST, DIR_LAST);
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
