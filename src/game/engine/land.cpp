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
#include "land.h"
#include "ccini.h"
#include "gamedebug.h"
#include "minmax.h"

char const *LandName[LAND_COUNT] = { "Clear", "Road", "Water", "Rock", "Wall", "Ore", "Beach", "Rough", "River" };

LandType Land_From_Name(char const *name)
{
    DEBUG_ASSERT(name != nullptr);

    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return LAND_NONE;
    }

    if (name != nullptr) {
        for (LandType land = LAND_FIRST; land < LAND_COUNT; ++land) {
            if (strcasecmp(name, LandName[land]) == 0) {
                return land;
            }
        }
    }

    return LAND_NONE;
}

char const *Name_From_Land(LandType land)
{
    if (land < LAND_COUNT) {
        return LandName[land];
    }

    return "<none>";
}
