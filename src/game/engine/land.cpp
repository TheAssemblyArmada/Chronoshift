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
#include "gamedebug.h"
#include "minmax.h"

#ifndef RAPP_STANDALONE
GroundClass *Ground = Make_Pointer<GroundClass>(0x00655DF0);
#else
GroundClass Ground[LAND_COUNT];
#endif

char const *LandName[LAND_COUNT] = { "Clear", "Road", "Water", "Rock", "Wall", "Ore", "Beach", "Rough", "River" };
const char *GroundClass::s_speedNames[SPEED_COUNT] = { "Foot", "Track", "Wheel", "Winged", "Float" };

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

BOOL GroundClass::Read_INI(CCINIClass &ini, LandType const land)
{
    DEBUG_ASSERT(land != LAND_NONE);
    DEBUG_ASSERT(land < LAND_COUNT);

    char const *landname = Name_From_Land(land);

    if (ini.Find_Section(landname) != nullptr) {
        for (SpeedType speed = SPEED_FIRST; speed < SPEED_COUNT; ++speed) {
            m_speeds[speed] = Min(fixed::_1_1, ini.Get_Fixed(landname, s_speedNames[speed], m_speeds[SPEED_FOOT]));
        }

        m_buildable = ini.Get_Bool(landname, "Buildable", false);

        return true;
    }

    return false;
}

BOOL GroundClass::Write_INI(CCINIClass &ini, LandType const land) const
{
    // TODO
    return 0;
}
