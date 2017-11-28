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

#define CELL_PIXELS 24
#define CELL_LEPTONS 256
#define CELL_MIN 0
#define CELL_MAX 128
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

int Distance(uint32_t coord1, uint32_t coord2);

#endif // COORD_H
