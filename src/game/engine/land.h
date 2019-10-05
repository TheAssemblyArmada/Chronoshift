/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Land conversion between string and enum.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef LAND_H
#define LAND_H

#include "always.h"
#include "fixed.h"
#include "gametypes.h"

class GameINIClass;

enum LandType
{
    LAND_CLEAR, // Clear grassy terrain
    LAND_ROAD, // Roads
    LAND_WATER, // Open water
    LAND_ROCK, // Cliffs
    LAND_WALL, // Walls and other man made obstacles
    LAND_ORE, // Ore and Gems
    LAND_BEACH, // Sandy beach
    LAND_ROUGH, // Rocky terrain
    LAND_RIVER, // Rivers and streams

    LAND_COUNT,

    LAND_NONE = -1,
    LAND_FIRST = LAND_CLEAR,
};

DEFINE_ENUMERATION_OPERATORS(LandType);

LandType Land_From_Name(const char *name);
const char *Name_From_Land(LandType land);

#endif // LAND_H
