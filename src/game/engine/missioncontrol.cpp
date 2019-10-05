/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class for holding customisable settings for each type of object mission.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "missioncontrol.h"
#include "mission.h"

#ifndef GAME_DLL
MissionControlClass MissionControlClass::MissionControl[MISSION_COUNT];
#endif

/**
 * 0x00502D7C
 */
MissionControlClass::MissionControlClass() :
    Mission(MISSION_NONE),
    NoThreat(false),
    Zombie(false),
    Recruitable(true),
    Paralyzed(false),
    Retaliate(true),
    Scatter(true),
    Rate("0.016"),
    AARate("0.016")
{
}

/**
 * @brief Gets the name for this mission.
 *
 * 0x00502DB8
 */
const char *MissionControlClass::Name() const
{
    if (Mission != MISSION_NONE && Mission < MISSION_COUNT) {
        return MissionClass::Missions[Mission];
    }

    return "<none>";
}

/**
 * @brief Initialises the mission with data from rules.ini.
 *
 * 0x00502DD8
 */
BOOL MissionControlClass::Read_INI(GameINIClass &ini)
{
    if (ini.Find_Section(Name()) != nullptr) {
        NoThreat = ini.Get_Bool(Name(), "NoThreat", NoThreat);
        Zombie = ini.Get_Bool(Name(), "Zombie", Zombie);
        Recruitable = ini.Get_Bool(Name(), "Recruitable", Recruitable);
        Paralyzed = ini.Get_Bool(Name(), "Paralyzed", Paralyzed);
        Retaliate = ini.Get_Bool(Name(), "Retaliate", Retaliate);
        Scatter = ini.Get_Bool(Name(), "Scatter", Scatter);
        Rate = ini.Get_Fixed(Name(), "Rate", Rate);
        AARate = ini.Get_Fixed(Name(), "AARate", fixed_t(0, 0));

        // Falls back to Rate for AARate if AARate is zero.
        if (AARate == fixed_t(0, 1)) {
            AARate = Rate;
        }

        return true;
    }

    return false;
}

/**
 * @brief Writes this missions data out to an ini file.
 */
void MissionControlClass::Write_INI(GameINIClass &ini)
{
    ini.Clear(Name());
    ini.Put_Bool(Name(), "NoThreat", NoThreat);
    ini.Put_Bool(Name(), "Zombie", Zombie);
    ini.Put_Bool(Name(), "Recruitable", Recruitable);
    ini.Put_Bool(Name(), "Paralyzed", Paralyzed);
    ini.Put_Bool(Name(), "Retaliate", Retaliate);
    ini.Put_Bool(Name(), "Scatter", Scatter);
    ini.Put_Fixed(Name(), "Rate", Rate);
    ini.Put_Fixed(Name(), "AARate", AARate);
}
