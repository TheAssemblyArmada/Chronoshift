/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class containing information about unit types.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef UNITTYPE_H
#define UNITTYPE_H

#include "always.h"

enum UnitType
{
    UNIT_NONE = -1,
    UNIT_FIRST = 0,
    UNIT_HTANK = 0,
    UNIT_MTANK = 1,
    UNIT_MTANK2 = 2,
    UNIT_LTANK = 3,
    UNIT_APC = 4,
    UNIT_MINELAYER = 5,
    UNIT_JEEP = 6,
    UNIT_HARVESTER = 7,
    UNIT_ARTY = 8,
    UNIT_MRJAMMER = 9,
    UNIT_MGAPGEN = 10,
    UNIT_MCV = 11,
    UNIT_V2RL = 12,
    UNIT_CARGO_TRUCK = 13,
    UNIT_ANT1 = 14,
    UNIT_ANT2 = 15,
    UNIT_ANT3 = 16,
    UNIT_CHRONO = 17,
    UNIT_TESLA = 18,
    UNIT_MAD_TANK = 19,
    UNIT_DEMO_TRUCK = 20,
    UNIT_PHASE = 21,
    UNIT_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(UnitType);

#endif
