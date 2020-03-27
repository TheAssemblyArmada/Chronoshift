/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 * @author tomsons26
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

    operator target_t() const { return m_Target; }

    RTTIType Get_RTTI() const { return RTTIType(m_RTTI); }

    int Get_ID() const { return m_ID; }

    CellClass *As_Cell() const;

private:
    union
    {
        struct
        {
            unsigned int m_ID : 24;
            unsigned int m_RTTI : 8;
        };
        target_t m_Target;
    };
};

inline target_t Make_Target(RTTIType type, int id)
{
    return ((type & 0xFF) << 24) | (id & 0xFFFFFF);
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

inline BOOL Target_Is_TeamType(target_t target)
{
    return Target_Get_RTTI(target) == RTTI_TEAMTYPE;
}

inline BOOL Target_Is_TriggerType(target_t target)
{
    return Target_Get_RTTI(target) == RTTI_TRIGGERTYPE;
}

inline BOOL Target_Is_Anim(target_t target)
{
    return Target_Get_RTTI(target) == RTTI_ANIM;
}

inline BOOL Target_Is_Bullet(target_t target)
{
    return Target_Get_RTTI(target) == RTTI_BULLET;
}

inline BOOL Target_Is_Team(target_t target)
{
    return Target_Get_RTTI(target) == RTTI_TEAM;
}

inline BOOL Target_Is_Trigger(target_t target)
{
    return Target_Get_RTTI(target) == RTTI_TRIGGER;
}

inline BOOL Target_Is_Targetable(target_t target)
{
    RTTIType rtti = Target_Get_RTTI(target);
    return rtti == RTTI_TERRAIN || rtti == RTTI_UNIT || rtti == RTTI_VESSEL || rtti == RTTI_INFANTRY || rtti == RTTI_BUILDING
        || rtti == RTTI_AIRCRAFT;
}

BOOL Target_Legal(target_t target);
target_t As_Target(cell_t cellnum);
target_t As_Target(coord_t coord);

TriggerClass *As_Trigger(target_t target);
TriggerTypeClass *As_TriggerType(target_t target);
TeamClass *As_Team(target_t target);
TeamTypeClass *As_TeamType(target_t target);
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

// This class is for use with EventClass, as unions can't have constructors.
class xTargetClass
{
public:
    CellClass *As_Cell() const;
    AbstractClass *As_Abstract() const;
    AbstractTypeClass *As_TypeClass() const;
    TechnoClass *As_Techno() const;
    ObjectClass *As_Object() const;
    AircraftClass *As_Aircraft() const;
    UnitClass *As_Unit() const;
    VesselClass *As_Vessel() const;
    InfantryClass *As_Infantry() const;
    BuildingClass *As_Building() const;

    void operator=(const TargetClass &target) { m_Target = target; }

    bool Valid_RTTI() const { return Target_Get_RTTI(m_Target) != RTTI_NONE; }

    RTTIType Get_RTTI() const { return Target_Get_RTTI(m_Target); }

    int Get_ID() const { return Target_Get_ID(m_Target); }

    operator target_t() const { return m_Target; }

private:
    target_t m_Target;
};

#endif // TARGET_H
