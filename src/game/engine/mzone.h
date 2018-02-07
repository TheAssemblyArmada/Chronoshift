/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Translates between string and enum for movement zones.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
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

// Based on TS/RA2.
// https://github.com/Ares-Developers/YRpp/blob/d387fbb9d36e4645d93caf87be502d6d87d042c3/GeneralDefinitions.h#L778
enum MZoneType
{
    MZONE_NONE = -1,
    MZONE_FIRST = 0,
    MZONE_NORMAL = 0, //
    MZONE_CRUSHER,
    MZONE_DESTROYER,
    MZONE_AMPHIBIOUS_DESTROYER, // used by vessels
    MZONE_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(MZoneType);

MZoneType MZone_From_Name(const char *name);
const char *Name_From_MZone(MZoneType land);

#endif // MZONE_H
