/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Land conversion between string and enum.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef LAND_H
#define LAND_H

#include	"always.h"

enum LandType {
    LAND_NONE = -1,

    LAND_FIRST = 0,

    LAND_CLEAR = 0,		// Clear grassy terrain
    LAND_ROAD = 1,		// Roads
    LAND_WATER = 2,		// Open water
    LAND_ROCK = 3,		// Cliffs
    LAND_WALL = 4,		// Walls and other man made obstacles
    LAND_ORE = 5,		// Ore and Gems
    LAND_BEACH = 6,		// Sandy beach
    LAND_ROUGH = 7,		// Rocky terrain
    LAND_RIVER = 8,		// Rivers and streams

    LAND_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(LandType);

LandType Land_From_Name(char const *name);
char const *Name_From_Land(LandType land);

#endif // LAND_H
