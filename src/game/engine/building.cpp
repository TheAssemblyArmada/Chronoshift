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
#include "globals.h"
#include "iomap.h"
#include <algorithm>

#ifndef GAME_DLL
TFixedIHeapClass<BuildingClass> g_Buildings;
#endif

BuildingClass::BuildingClass(BuildingType type, HousesType house) :
    TechnoClass(RTTI_BUILDING, g_Buildings.ID(this), house)
{
}

BuildingClass::BuildingClass(const BuildingClass &that) :
    TechnoClass(that)
{
}

BuildingClass::BuildingClass(const NoInitClass &noinit) :
    TechnoClass(noinit)
{
}

BuildingClass::~BuildingClass()
{}

/**
 *
 *
 * @address 0x00460AD0
 */
const BuildingTypeClass &BuildingClass::Class_Of() const
{
    return *m_Class;
}

/**
 *
 *
 * @address 0x0045DE68
 */
void BuildingClass::Death_Announcement(TechnoClass *killer) const
{
    if (killer != nullptr && m_OwnerHouse->Player_Has_Control()) {
        Speak(VOX_STRUCTURE_DESTROYED);
    }
}

/**
 *
 *
 * @address 0x0045EAC0
 */
BOOL BuildingClass::Can_Player_Move() const
{
    return What_Type() == BUILDING_FACT;
}

/**
 *
 *
 * @address 0x0045E3E4
 */
void BuildingClass::Detach(target_t target, int a2)
{
    TechnoClass::Detach(target, a2);
    if (m_SabotagedBy == target) {
        m_SabotagedBy = 0;
    }
    if (m_AttachedAnim == target) {
        m_AttachedAnim = 0;
    }
}

/**
 *
 *
 * @address 0x0046072C
 */
int BuildingClass::Value() const
{
    if (Class_Of().Is_Fake()) {
        BuildingTypeClass *btptr = nullptr;
        switch (What_Type()) {
            case BUILDING_WEAF:
                btptr = &BuildingTypeClass::As_Reference(BUILDING_WEAP);
                return btptr->Get_ThreatPosed() + btptr->Get_ThreatPoints();
            case BUILDING_FACF:
                btptr = &BuildingTypeClass::As_Reference(BUILDING_FACT);
                return btptr->Get_ThreatPosed() + btptr->Get_ThreatPoints();
            case BUILDING_SYRF:
                btptr = &BuildingTypeClass::As_Reference(BUILDING_SYRD);
                return btptr->Get_ThreatPosed() + btptr->Get_ThreatPoints();
            case BUILDING_SPEF:
                btptr = &BuildingTypeClass::As_Reference(BUILDING_SPEN);
                return btptr->Get_ThreatPosed() + btptr->Get_ThreatPoints();
            case BUILDING_DOMF:
                btptr = &BuildingTypeClass::As_Reference(BUILDING_DOME);
                return btptr->Get_ThreatPosed() + btptr->Get_ThreatPoints();
            default:
                DEBUG_LOG("BuildingClass::Value - Unhandled fake!\n");
                break;
        }
    }
    return TechnoClass::Value();
}

/**
 *
 *
 * @address 0x004606D0
 */
void *BuildingClass::Get_Image_Data() const
{
    if (m_CurrentState == BSTATE_0) {
        return Class_Of().Get_Buildup_Data();
    }
    return ObjectClass::Get_Image_Data();
}
