/**
 * @file
 *
 * @author CCHyper
 *
 * @brief Class holding information on static house info.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "housestatic.h"
#include "gameini.h"
#include "rules.h"
#include "scenario.h"

HouseStaticClass::HouseStaticClass() :
    m_IQ(0),
    m_TechLevel(1),
    m_Allies(0),
    m_MaxUnit(Rule.Max_Unit() / 6),
    m_MaxBuilding(Rule.Max_Building() / 6),
    m_MaxInfantry(Rule.Max_Infantry() / 6),
    m_MaxVessel(Rule.Max_Vessel() / 6),
    m_MaxAircraft(Rule.Max_Unit() / 6),
    m_Credits(0),
    m_Edge(SOURCE_NORTH)
{
}

HouseStaticClass::HouseStaticClass(const HouseStaticClass &that) :
    m_IQ(that.m_IQ),
    m_TechLevel(that.m_TechLevel),
    m_Allies(that.m_Allies),
    m_MaxUnit(that.m_MaxUnit),
    m_MaxBuilding(that.m_MaxBuilding),
    m_MaxInfantry(that.m_MaxInfantry),
    m_MaxVessel(that.m_MaxVessel),
    m_MaxAircraft(that.m_MaxAircraft),
    m_Credits(that.m_Credits),
    m_Edge(that.m_Edge)
{
}

bool HouseStaticClass::Read_INI(GameINIClass &ini, char const *section)
{
    if (ini.Section_Present(section)) {
        m_MaxBuilding = ini.Get_Int(section, "MaxBuilding", m_MaxBuilding);
        m_MaxUnit = ini.Get_Int(section, "MaxUnit", m_MaxUnit);
        m_MaxInfantry = ini.Get_Int(section, "MaxInfantry", m_MaxInfantry);
        m_MaxAircraft = ini.Get_Int(section, "MaxAircraft", m_MaxAircraft);
        m_MaxVessel = ini.Get_Int(section, "MaxVessel", m_MaxVessel);

        if (m_MaxVessel <= 0) {
            m_MaxVessel = m_MaxUnit;
        }

        m_TechLevel = ini.Get_Int(section, "TechLevel", m_TechLevel);

        m_Credits = (ini.Get_Int(section, "Credits", m_Credits) * 100);

        m_IQ = ini.Get_Int(section, "IQ", m_IQ);
        if (m_IQ > Rule.IQ_Controls().m_MaxIQLevels) {
            m_IQ = 1;
        }

        m_Edge = ini.Get_SourceType(section, "Edge", SOURCE_NORTH);

        return true;
    }

    return false;
}

void HouseStaticClass::Write_INI(GameINIClass &ini, char const *section)
{
    if (ini.Section_Present(section)) {
        // Default instance for comparison purposes.
        HouseStaticClass statichouse;

        if (m_Credits != statichouse.m_Credits) {
            ini.Put_Int(section, "Credits", (m_Credits / 100));
        }

        if (m_Edge != statichouse.m_Edge) {
            ini.Put_SourceType(section, "Edge", m_Edge);
        }

        if (m_MaxBuilding > 0 && m_MaxBuilding != statichouse.m_MaxBuilding) {
            ini.Put_Int(section, "MaxBuilding", m_MaxBuilding);
        }

        if (m_MaxUnit > 0 && m_MaxUnit != statichouse.m_MaxUnit) {
            ini.Put_Int(section, "MaxUnit", m_MaxUnit);
        }

        if (m_MaxInfantry > 0 && m_MaxInfantry != statichouse.m_MaxInfantry) {
            ini.Put_Int(section, "MaxInfantry", m_MaxInfantry);
        }

        if (m_MaxAircraft > 0 && m_MaxAircraft != statichouse.m_MaxAircraft) {
            ini.Put_Int(section, "MaxAircraft", m_MaxAircraft);
        }

        if (m_MaxVessel > 0 && m_MaxVessel != statichouse.m_MaxVessel) {
            ini.Put_Int(section, "MaxAircraft", m_MaxVessel);
        }

        if (m_IQ != statichouse.m_IQ) {
            ini.Put_Int(section, "IQ", m_IQ);
        }

        if (m_Edge != statichouse.m_Edge) {
            ini.Put_Int(section, "Edge", m_Edge);
        }
    }
}
