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
MissionControlClass MissionControlClass::s_MissionControl[MISSION_COUNT];
#endif

/**
 * 0x00502D7C
 */
MissionControlClass::MissionControlClass() :
    m_Mission(MISSION_NONE),
    m_NoThreat(false),
    m_Zombie(false),
    m_Recruitable(true),
    m_Paralyzed(false),
    m_Retaliate(true),
    m_Scatter(true),
    m_Rate("0.016"),
    m_AARate("0.016")
{
}

/**
 * @brief Gets the name for this mission.
 *
 * 0x00502DB8
 */
const char *MissionControlClass::Name() const
{
    if (m_Mission != MISSION_NONE && m_Mission < MISSION_COUNT) {
        return MissionClass::s_Missions[m_Mission];
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
        m_NoThreat = ini.Get_Bool(Name(), "NoThreat", m_NoThreat);
        m_Zombie = ini.Get_Bool(Name(), "Zombie", m_Zombie);
        m_Recruitable = ini.Get_Bool(Name(), "Recruitable", m_Recruitable);
        m_Paralyzed = ini.Get_Bool(Name(), "Paralyzed", m_Paralyzed);
        m_Retaliate = ini.Get_Bool(Name(), "Retaliate", m_Retaliate);
        m_Scatter = ini.Get_Bool(Name(), "Scatter", m_Scatter);
        m_Rate = ini.Get_Fixed(Name(), "Rate", m_Rate);
        m_AARate = ini.Get_Fixed(Name(), "AARate", fixed_t(0, 0));

        // Falls back to Rate for AARate if AARate is zero.
        if (m_AARate == fixed_t(0, 1)) {
            m_AARate = m_Rate;
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
    ini.Put_Bool(Name(), "NoThreat", m_NoThreat);
    ini.Put_Bool(Name(), "Zombie", m_Zombie);
    ini.Put_Bool(Name(), "Recruitable", m_Recruitable);
    ini.Put_Bool(Name(), "Paralyzed", m_Paralyzed);
    ini.Put_Bool(Name(), "Retaliate", m_Retaliate);
    ini.Put_Bool(Name(), "Scatter", m_Scatter);
    ini.Put_Fixed(Name(), "Rate", m_Rate);
    ini.Put_Fixed(Name(), "AARate", m_AARate);
}
