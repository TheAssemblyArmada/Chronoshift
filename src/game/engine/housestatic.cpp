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
    IQ(0),
    TechLevel(1),
    Allies(0),
    MaxUnit(Rule.Max_Unit() / 6),
    MaxBuilding(Rule.Max_Building() / 6),
    MaxInfantry(Rule.Max_Infantry() / 6),
    MaxVessel(Rule.Max_Vessel() / 6),
    MaxAircraft(Rule.Max_Unit() / 6),
    Credits(0),
    Edge(SOURCE_NORTH)
{
}

HouseStaticClass::HouseStaticClass(const HouseStaticClass &that) :
    IQ(that.IQ),
    TechLevel(that.TechLevel),
    Allies(that.Allies),
    MaxUnit(that.MaxUnit),
    MaxBuilding(that.MaxBuilding),
    MaxInfantry(that.MaxInfantry),
    MaxVessel(that.MaxVessel),
    MaxAircraft(that.MaxAircraft),
    Credits(that.Credits),
    Edge(that.Edge)
{
}

bool HouseStaticClass::Read_INI(GameINIClass &ini, char const *section)
{
    if (ini.Section_Present(section)) {
        MaxBuilding = ini.Get_Int(section, "MaxBuilding", MaxBuilding);
        MaxUnit = ini.Get_Int(section, "MaxUnit", MaxUnit);
        MaxInfantry = ini.Get_Int(section, "MaxInfantry", MaxInfantry);
        MaxAircraft = ini.Get_Int(section, "MaxAircraft", MaxAircraft);
        MaxVessel = ini.Get_Int(section, "MaxVessel", MaxVessel);

        if (MaxVessel <= 0) {
            MaxVessel = MaxUnit;
        }

        TechLevel = ini.Get_Int(section, "TechLevel", TechLevel);

        Credits = (ini.Get_Int(section, "Credits", Credits) * 100);

        IQ = ini.Get_Int(section, "IQ", IQ);
        if (IQ > Rule.IQ_Controls().MaxIQLevels) {
            IQ = 1;
        }

        Edge = ini.Get_SourceType(section, "Edge", SOURCE_NORTH);

        return true;
    }

    return false;
}

void HouseStaticClass::Write_INI(GameINIClass &ini, char const *section)
{
    if (ini.Section_Present(section)) {
        // Default instance for comparison purposes.
        HouseStaticClass statichouse;

        if (Credits != statichouse.Credits) {
            ini.Put_Int(section, "Credits", (Credits / 100));
        }

        if (Edge != statichouse.Edge) {
            ini.Put_SourceType(section, "Edge", Edge);
        }

        if (MaxBuilding > 0 && MaxBuilding != statichouse.MaxBuilding) {
            ini.Put_Int(section, "MaxBuilding", MaxBuilding);
        }

        if (MaxUnit > 0 && MaxUnit != statichouse.MaxUnit) {
            ini.Put_Int(section, "MaxUnit", MaxUnit);
        }

        if (MaxInfantry > 0 && MaxInfantry != statichouse.MaxInfantry) {
            ini.Put_Int(section, "MaxInfantry", MaxInfantry);
        }

        if (MaxAircraft > 0 && MaxAircraft != statichouse.MaxAircraft) {
            ini.Put_Int(section, "MaxAircraft", MaxAircraft);
        }

        if (MaxVessel > 0 && MaxVessel != statichouse.MaxVessel) {
            ini.Put_Int(section, "MaxAircraft", MaxVessel);
        }

        if (IQ != statichouse.IQ) {
            ini.Put_Int(section, "IQ", IQ);
        }

        if (Edge != statichouse.Edge) {
            ini.Put_Int(section, "Edge", Edge);
        }
    }
}
