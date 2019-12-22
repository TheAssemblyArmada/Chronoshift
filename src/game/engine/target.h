/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Functions for converting to and from target values.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef TARGET_H
#define TARGET_H

#include "always.h"
#include "gametypes.h"
#include "rtti.h"

class CellClass;
class TriggerClass;
class TriggerTypeClass;
class TeamClass;
class TeamTypeClass;
class AnimClass;
class BulletClass;
class TerrainClass;
class AircraftClass;
class TechnoClass;
class TechnoTypeClass;
class AbstractClass;
class AbstractTypeClass;
class ObjectClass;
class UnitClass;
class VesselClass;
class InfantryClass;
class BuildingClass;

class TargetClass
{
public:
    TargetClass(target_t target) : m_Target(target) {}
    TargetClass(AbstractClass *abstract);
    TargetClass(AbstractTypeClass *abstractype);
    TargetClass(CellClass *cell);
    TargetClass(const TargetClass &that) : m_Target(that.m_Target) {}
    ~TargetClass() {}

    operator target_t() { return m_Target; }

    CellClass *As_Cell() const;
    AbstractClass *As_Abstract() const;
    AbstractTypeClass *As_TypeClass() const;
    TechnoClass *As_Techno() const;
    ObjectClass *As_Object() const;

private:
    union {
        struct {
            unsigned int m_ID : 24;
            unsigned int m_RTTI : 8;
        }; target_t m_Target;
    };
};

inline target_t Make_Target(RTTIType type, int id)
{
    return (id & 0xFFFFFF) | ((type << 24) & 0xFF000000);
}

inline RTTIType Target_Get_RTTI(target_t target)
{
    return RTTIType((target & 0xFF000000) >> 24);
}

inline int Target_Get_ID(target_t target)
{
    return (target & 0xFFFFFF);
}

inline BOOL Target_Is_Building(target_t target)
{
    return Target_Get_RTTI(target) == RTTI_BUILDING;
}

inline BOOL Target_Is_Aircraft(target_t target)
{
    return Target_Get_RTTI(target) == RTTI_AIRCRAFT;
}

inline BOOL Target_Is_Unit(target_t target)
{
    return Target_Get_RTTI(target) == RTTI_UNIT;
}

inline BOOL Target_Is_Vessel(target_t target)
{
    return Target_Get_RTTI(target) == RTTI_VESSEL;
}

inline BOOL Target_Is_Infantry(target_t target)
{
    return Target_Get_RTTI(target) == RTTI_INFANTRY;
}

inline BOOL Target_Is_Terrain(target_t target)
{
    return Target_Get_RTTI(target) == RTTI_TERRAIN;
}

inline BOOL Target_Is_Cell(target_t target)
{
    return Target_Get_RTTI(target) == RTTI_CELL;
}

BOOL Target_Is_Techno(target_t target);

BOOL Target_Legal(target_t target);

target_t As_Target(cell_t cellnum);
target_t As_Target(coord_t coord);
target_t As_Target(CellClass *cell);
target_t As_Target(AbstractClass *object);

TriggerClass *As_Trigger(target_t target);
TriggerTypeClass *As_TriggerType(target_t target);
TeamClass *As_Team(target_t target);
TeamTypeClass *As_TeamType(target_t target);
AbstractClass *As_Abstract(target_t target);
ObjectClass *As_Object(target_t target);
AnimClass *As_Animation(target_t target);
BulletClass *As_Bullet(target_t target);
TerrainClass *As_Terrain(target_t target);
TechnoClass *As_Techno(target_t target);
TechnoTypeClass *As_TechnoType(target_t target);
AircraftClass *As_Aircraft(target_t target);
UnitClass *As_Unit(target_t target);
VesselClass *As_Vessel(target_t target);
InfantryClass *As_Infantry(target_t target);
BuildingClass *As_Building(target_t target);

cell_t As_Cell(target_t target);
coord_t As_Coord(target_t target);
coord_t As_Movement_Coord(target_t target);

#endif // TARGET_H
