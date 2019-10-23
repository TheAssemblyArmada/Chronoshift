/**
 * @file
 *
 * @author tomsons26
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

#include "carryover.h"
#include "building.h"
#include "infantry.h"
#include "techno.h"
#include "unit.h"
#include "vessel.h"

#ifndef GAME_DLL
CarryoverClass *g_Carryover = nullptr;
#endif

/**
 * @brief
 *
 * @address 0x004624A0
 */
CarryoverClass::CarryoverClass(TechnoClass *techno) :
    m_RTTI(RTTI_NONE),
    m_Cell(0),
    m_Health(0),
    m_Owner(HOUSES_NONE)
{
    if (techno != nullptr) {
        m_RTTI = techno->What_Am_I();
        switch (m_RTTI) {
            case RTTI_INFANTRY:
                m_Type = reinterpret_cast<InfantryClass *>(techno)->What_Type();
                break;
            case RTTI_UNIT:
                m_Type = reinterpret_cast<UnitClass *>(techno)->What_Type();
                break;
            case RTTI_VESSEL:
                m_Type = reinterpret_cast<VesselClass *>(techno)->What_Type();
                break;
            case RTTI_BUILDING:
                m_Type = reinterpret_cast<BuildingClass *>(techno)->What_Type();
                break;
            default:
                break;
        }
        m_Owner = techno->Owner();
        m_Health = techno->Get_Health();
        m_Cell = techno->Get_Cell();
    }
}

/**
 * @brief
 *
 * @address 0x004625E8
 */
BOOL CarryoverClass::Create()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004625E8, BOOL, CarryoverClass *);
    return func(this);
#else
// TODO uncomment when all ctors are callable
#if 0
    TechnoClass *ptr = nullptr;

    switch (m_RTTI) {
        case RTTI_INFANTRY:
            ptr = new InfantryClass(InfantryType(m_Type), m_Owner);
            break;
        case RTTI_UNIT:
            ptr = new UnitClass(UnitType(m_Type), m_Owner);
            break;
        case RTTI_VESSEL:
            ptr = new VesselClass(VesselType(m_Type), m_Owner);
            break;
        case RTTI_BUILDING:
            ptr = new BuildingClass(BuildingType(m_Type), m_Owner);
            break;
        default:
            break;
    }

    if (ptr == nullptr) {
        return false;
    }

    ptr->Set_Health(m_Health);

    int init = ScenarioInit;
    if (m_RTTI == RTTI_INFANTRY) {
        ScenarioInit = 0;
    }

    ptr->Unlimbo(Cell_To_Coord(m_Cell), DIR_NORTH);

    if (m_RTTI == RTTI_INFANTRY) {
        ScenarioInit = init;
    }
    return true;
#endif
    return true;
#endif
}
