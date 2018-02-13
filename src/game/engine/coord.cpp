/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Functions for handling the different distance and position representations in the engine.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "coord.h"
#include "abs.h"

const uint32_t AdjacentCoord[FACING_COUNT] = {
    0xFF000000,     // (-256,  0)       // NORTH
    0xFF000100,     // (-256,  +256)    // NORTH EAST
    0x00000100,     // (0   ,  +256)    // EAST
    0x01000100,     // (+256,  +256)    // SOUTH EAST
    0x01000000,     // (+256,  0)       // SOUTH
    0x0100FF00,     // (+256,  -256)    // SOUTH WEST
    0x0000FF00,     // (0   ,  -256)    // WEST
    0xFF00FF00      // (-256,  -256)    // NORTH WEST
};

int Distance(uint32_t coord1, uint32_t coord2)
{
    int ydiff;
    int xdiff;

    ydiff = Abs(Coord_Lepton_Y(coord1) - Coord_Lepton_Y(coord2));
    xdiff = Abs(Coord_Lepton_X(coord1) - Coord_Lepton_X(coord2));

    if (ydiff <= xdiff) {
        ydiff /= 2;
    } else {
        xdiff /= 2;
    }

    return ydiff + xdiff;
}
