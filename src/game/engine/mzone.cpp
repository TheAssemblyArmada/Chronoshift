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
#include "mzone.h"
#include "gamedebug.h"

#ifndef PLATFORM_WINDOWS
#include <strings.h>
#endif

const char *MZoneName[MZONE_COUNT] = { "Normal", "Crusher", "Destroyer", "AmphibiousDestroyer" };

MZoneType MZone_From_Name(const char *name)
{
    DEBUG_ASSERT(name != nullptr);

    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return MZONE_NONE;
    }

    if (name != nullptr) {
        for (MZoneType mzone = MZONE_FIRST; mzone < MZONE_COUNT; ++mzone) {
            if (strcasecmp(name, MZoneName[mzone]) == 0) {
                return mzone;
            }
        }
    }
    return MZONE_NONE;
}

const char *Name_From_MZone(MZoneType mzone)
{
    DEBUG_ASSERT(mzone < MZONE_COUNT);
    DEBUG_ASSERT(mzone != MZONE_NONE);

    if (mzone != MZONE_NONE && mzone < MZONE_COUNT) {
        return MZoneName[mzone];
    }
    return "<none>";
}
