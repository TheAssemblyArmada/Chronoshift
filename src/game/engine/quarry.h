/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Holds quarry type enum and conversion functions to/from strings.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef QUARRY_H
#define QUARRY_H

#include "always.h"

enum QuarryType
{
    QUARRY_NA,
    QUARRY_ANYTHING,
    QUARRY_ANY_BUILDING,
    QUARRY_HARVESTERS,
    QUARRY_INFANTRY,
    QUARRY_ANY_VEHICLES,
    QUARRY_ANY_VESSEL,
    QUARRY_FACTORIES,
    QUARRY_BASE_DEFENSES,
    QUARRY_BASE_THREATS,
    QUARRY_POWER_FACILITIES,
    QUARRY_FAKE_BUILDINGS,
    QUARRY_COUNT,
    QUARRY_NONE = -1,
    QUARRY_FIRST = 0,
};

DEFINE_ENUMERATION_OPERATORS(QuarryType);

QuarryType Quarry_From_Name(const char *name);
const char *Name_From_Quarry(QuarryType type);

#endif // QUARRY_H
