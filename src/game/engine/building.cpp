/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "building.h"
#include <algorithm>

#ifndef CHRONOSHIFT_STANDALONE
TFixedIHeapClass<BuildingClass> &g_Buildings = Make_Global<TFixedIHeapClass<BuildingClass> >(0x0065D8B0);
#else
TFixedIHeapClass<BuildingClass> g_Buildings;
#endif

BuildingClass::BuildingClass(RTTIType type, int id, HousesType house) : TechnoClass(type, id, house) {}

BuildingClass::BuildingClass(const BuildingClass &that) : TechnoClass(that) {}

BuildingClass::BuildingClass(const NoInitClass &noinit) : TechnoClass(noinit) {}

BuildingClass::~BuildingClass()
{
    
}

void BuildingClass::Death_Announcement(TechnoClass *killer) const
{

}

FireErrorType BuildingClass::Can_Fire(target_t target, WeaponSlotType weapon) const
{
    return FIRE_NONE;
}

target_t BuildingClass::Greatest_Threat(ThreatType threat)
{
    return 0;
}

void BuildingClass::Assign_Target(target_t target) {}

BOOL BuildingClass::Captured(HouseClass *house)
{
    return 0;
}

void BuildingClass::Enter_Idle_Mode(BOOL a1)
{

}

void BuildingClass::Grand_Opening(int a1)
{

}

void BuildingClass::Update_Buildables()
{

}

void *BuildingClass::Remap_Table()
{
    return nullptr;
}

int BuildingClass::Toggle_Primary()
{
    return 0;
}
