/**
 * @file
 *
 * @author CCHyper
 *
 * @brief Class for handling ground types.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "ground.h"
#include "ccini.h"
#include "minmax.h"

#ifndef CHRONOSHIFT_STANDALONE
GroundClass *Ground = Make_Pointer<GroundClass>(0x00655DF0);
#else
GroundClass Ground[LAND_COUNT];
#endif

const char *GroundTypes[GROUND_COUNT] = { "Foot", "Track", "Wheel", "Winged", "Float" };

GroundType GroundClass::From_Name(const char *name)
{
    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return GROUND_NONE;
    }

    if (name != nullptr) {
        for (GroundType ground = GROUND_FIRST; ground < GROUND_COUNT; ++ground) {
            if (strcasecmp(name, Name_From(ground)) == 0) {
                return ground;
            }
        }
    }

    return GROUND_NONE;
}

const char *GroundClass::Name_From(GroundType ground)
{
    return ground != GROUND_NONE && ground < GROUND_COUNT ? GroundTypes[ground] : "<none>";
}

BOOL GroundClass::Read_INI(CCINIClass &ini, LandType const land)
{
    DEBUG_ASSERT(land != LAND_NONE);
    DEBUG_ASSERT(land < LAND_COUNT);

    const char *landname = Name_From_Land(land);

    if (ini.Find_Section(landname) != nullptr) {
        for (SpeedType speed = SPEED_FIRST; speed < SPEED_COUNT; ++speed) {
            Speeds[speed] = Min(fixed::_1_1, ini.Get_Fixed(landname, GroundTypes[speed], Speeds[SPEED_FOOT]));
        }

        Buildable = ini.Get_Bool(landname, "Buildable", false);

        return true;
    }

    return false;
}

BOOL GroundClass::Write_INI(CCINIClass &ini, LandType const land) const
{
    // TODO
    return 0;
}
