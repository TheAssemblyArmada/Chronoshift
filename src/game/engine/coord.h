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
#pragma once

#ifndef COORD_H
#define COORD_H

#include "always.h"
#include "facing.h"

#define CELL_PIXELS 24
#define CELL_LEPTONS 256
#define CELL_MIN 0
#define CELL_MAX 128
#define CELL_MAX_X 128
#define COORD_MIN 0
#define COORD_MAX (CELL_LEPTONS * CELL_MAX)

inline int16_t Coord_Lepton_X(uint32_t coord)
{
    return coord & 0x0000FFFF;
}

inline int16_t Coord_Lepton_Y(uint32_t coord)
{
    return (coord & 0xFFFF0000) >> 16;
}

inline uint32_t Coord_From_Lepton_XY(int16_t x, int16_t y)
{
    return x | (y << 16);
}

inline uint32_t Coord_Add(uint32_t coord1, uint32_t coord2)
{
    int16_t lx = Coord_Lepton_X(coord1) + Coord_Lepton_X(coord2);
    int16_t ly = Coord_Lepton_Y(coord1) + Coord_Lepton_Y(coord2);

    return Coord_From_Lepton_XY(lx, ly);
}

inline uint32_t Coord_Centered(uint32_t coord)
{
    return coord & 0xFF80FF80;
}

inline uint32_t Coord_Top_Left(uint32_t coord)
{
    return coord & 0xFF00FF00;
}

inline uint32_t Coord_Get_Adjacent(uint32_t coord, FacingType facing)
{
    static const uint32_t AdjacentCoord[FACING_COUNT] = {
        0xFF000000,     // (-256,  0)       // NORTH
        0xFF000100,     // (-256,  +256)    // NORTH EAST
        0x00000100,     // (0   ,  +256)    // EAST
        0x01000100,     // (+256,  +256)    // SOUTH EAST
        0x01000000,     // (+256,  0)       // SOUTH
        0x0100FF00,     // (+256,  -256)    // SOUTH WEST
        0x0000FF00,     // (0   ,  -256)    // WEST
        0xFF00FF00      // (-256,  -256)    // NORTH WEST
    };

    return Coord_Centered(Coord_Add(coord, AdjacentCoord[(unsigned)facing % FACING_COUNT]));
}

inline uint8_t Coord_Cell_X(uint32_t coord)
{
    return (coord & 0x00007F00) >> 8;
}

inline uint8_t Coord_Cell_Y(uint32_t coord)
{
    return (coord & 0x7F000000) >> 24;
}

inline int16_t Cell_From_XY(uint8_t x, uint8_t y)
{
    return ((y * CELL_MAX_X) + x);
}

inline int16_t Coord_To_Cell(uint32_t coord)
{
    return Cell_From_XY(Coord_Cell_X(coord), Coord_Cell_Y(coord));
}

int Distance(uint32_t coord1, uint32_t coord2);

#endif // COORD_H
