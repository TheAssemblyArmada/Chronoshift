/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Translates between string and enum for movement zones.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef MZONE_H
#define MZONE_H

#include "always.h"

enum MZoneType
{
    MZONE_NORMAL,
    MZONE_CRUSHER,
    MZONE_DESTROYER,
    MZONE_AMPHIBIOUS_DESTROYER, // used by Vessels only.

    MZONE_COUNT,

    MZONE_NONE = -1,
    MZONE_FIRST = MZONE_NORMAL,
};

DEFINE_ENUMERATION_OPERATORS(MZoneType);

MZoneType MZone_From_Name(const char *name);
const char *Name_From_MZone(MZoneType land);

#endif // MZONE_H
