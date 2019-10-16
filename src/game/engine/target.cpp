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
#include "target.h"
#include "cell.h"
#include "coord.h"
#include "object.h"
#include "techno.h"
#include "technotype.h"
//#include "triggertype.h"

CellClass *TargetClass::As_Cell() const
{
    return nullptr;
    //return ::As_Cell(m_Target);
}

AbstractClass *TargetClass::As_Abstract() const
{
    return ::As_Abstract(m_Target);
}

AbstractTypeClass *TargetClass::As_TypeClass() const
{
    return nullptr;
    //return ::As_TypeClass(m_Target);
}

TechnoClass *TargetClass::As_Techno() const
{
    return ::As_Techno(m_Target);
}

ObjectClass *TargetClass::As_Object() const
{
    return ::As_Object(m_Target);
}

TargetClass::TargetClass(AbstractClass *abstract)
{
    if (abstract != nullptr){
        m_Target = (abstract->What_Am_I() << 24) | (abstract->Get_Heap_ID() & 0xFFFFFF);
    } else {
        m_RTTI = RTTI_NONE;
    }
}

TargetClass::TargetClass(AbstractTypeClass *abstractype)
{
    if (abstractype != nullptr){
        m_Target = (abstractype->What_Am_I() << 24) | (abstractype->Get_Heap_ID() & 0xFFFFFF);
    } else {
        m_RTTI = RTTI_NONE;
    }
}

TargetClass::TargetClass(CellClass *cell)
{
    if (cell != nullptr){
        m_Target = (RTTI_CELL << 24) | (cell->Cell_Number() & 0xFFFFFF);
    } else {
        m_RTTI = RTTI_NONE;
    }
}

BOOL Target_Is_Techno(target_t target)
{
    TechnoClass *ptr = As_Techno(target);
    return ptr != nullptr ? ptr->Is_Techno() : false;
}

BOOL Target_Legal(target_t target)
{
    return (Target_Get_RTTI(target) != RTTI_NONE && Target_Get_RTTI(target) < RTTI_COUNT && Target_Get_ID(target) >= 0);
}

target_t As_Target(cell_t cellnum)
{
    return Make_Target(RTTI_CELL, (16 * Cell_Get_X(cellnum) + 8)) | ((16 * Cell_Get_Y(cellnum) + 8) << 12);
}

target_t As_Target(coord_t coord)
{
    return Make_Target(RTTI_CELL, (Coord_Lepton_X(coord) >> 4) | ((Coord_Lepton_Y(coord) >> 4) << 12));
}

target_t As_Target(CellClass *cell)
{
    return cell != nullptr ? As_Target(cell->Cell_Number()) : 0;
}

target_t As_Target(AbstractClass *object)
{
    return object != nullptr ? ((object->Get_Heap_ID() & 0xFFFFFF) | ((object->What_Am_I() << 24) & 0xFF000000)) : 0;
}

TriggerClass *As_Trigger(target_t target)
{
#ifdef GAME_DLL
    TriggerClass *(*func)(target_t) = reinterpret_cast<TriggerClass *(*)(target_t)>(0x00554FFC);
    return func(target);
#elif 0
    // TODO: Requires TriggerClass implementation.
    return Target_Get_RTTI(target) == RTTI_TRIGGER ? &Triggers[Target_Get_ID(target)] : nullptr;
#else
    return nullptr;
#endif
}

TriggerTypeClass *As_TriggerType(target_t target)
{
#ifdef GAME_DLL
    TriggerTypeClass *(*func)(target_t) = reinterpret_cast<TriggerTypeClass *(*)(target_t)>(0x005559CC);
    return func(target);
#elif 0
    // TODO: Requires TriggerTypeClass implementation.
    return Target_Get_RTTI(target) == RTTI_TRIGGERTYPE ? &TriggerTypes[Target_Get_ID(target)] : nullptr;
#else
    return nullptr;
#endif
}

TeamClass *As_Team(target_t target)
{
#ifdef GAME_DLL
    TeamClass *(*func)(target_t) = reinterpret_cast<TeamClass *(*)(target_t)>(0x00555034);
    return func(target);
#elif 0
    // TODO: Requires TeamClass implementation.
    return Target_Get_RTTI(target) == RTTI_TEAM ? &Teams[Target_Get_ID(target)] : nullptr;
#else
    return nullptr;
#endif
}

TeamTypeClass *As_TeamType(target_t target)
{
#ifdef GAME_DLL
    TeamTypeClass *(*func)(target_t) = reinterpret_cast<TeamTypeClass *(*)(target_t)>(0x0055506C);
    return func(target);
#elif 0
    // TODO: Requires TeamTypeClass implementation.
    return Target_Get_RTTI(target) == RTTI_TEAMTYPE ? &TeamTypes[Target_Get_ID(target)] : nullptr;
#else
    return nullptr;
#endif
}

AbstractClass *As_Abstract(target_t target)
{
    return (AbstractClass *)As_Object(target);
}

ObjectClass *As_Object(target_t target)
{
#ifdef GAME_DLL
    ObjectClass *(*func)(target_t) = reinterpret_cast<ObjectClass *(*)(target_t)>(0x00555190);
    return func(target);
#elif 0
    ObjectClass *objptr = nullptr;

    switch (Target_Get_RTTI(target)) {
        case RTTI_AIRCRAFT:
            objptr = As_Aircraft(target);
            break;

        case RTTI_ANIM:
            objptr = As_Animation(target);
            break;

        case RTTI_BUILDING:
            objptr = As_Building(target);
            break;

        case RTTI_BULLET:
            objptr = As_Bullet(target);
            break;

        case RTTI_INFANTRY:
            objptr = As_Infantry(target);
            break;

        case RTTI_TERRAIN:
            objptr = As_Terrain(target);
            break;

        case RTTI_UNIT:
            objptr = As_Unit(target);
            break;

        case RTTI_VESSEL:
            objptr = As_Vessel(target);
            break;

        default:
            break;
    };

    // DEBUG_ASSERT(objptr != nullptr);

    if (objptr != nullptr && objptr->m_IsActive) {
        return objptr;
    }

    return nullptr;
#else
    return nullptr;
#endif
}

AnimClass *As_Animation(target_t target)
{
#ifdef GAME_DLL
    AnimClass *(*func)(target_t) = reinterpret_cast<AnimClass *(*)(target_t)>(0x005550A4);
    return func(target);
#elif 0
    // TODO: Requires AnimClass implementation.
    return Target_Get_RTTI(target) == RTTI_ANIM ? &Anims[Target_Get_ID(target)] : nullptr;
#else
    return nullptr;
#endif
}

BulletClass *As_Bullet(target_t target)
{
#ifdef GAME_DLL
    BulletClass *(*func)(target_t) = reinterpret_cast<BulletClass *(*)(target_t)>(0x005550DC);
    return func(target);
#elif 0
    // TODO: Requires BulletClass implementation.
    return Target_Get_RTTI(target) == RTTI_BULLET ? &Bullets[Target_Get_ID(target)] : nullptr;
#else
    return nullptr;
#endif
}

TerrainClass *As_Terrain(target_t target)
{
#ifdef GAME_DLL
    // TODO: Inlined in RA
    return nullptr;
#elif 0
    // TODO: Requires TerrainClass implementation.
    return Target_Get_RTTI(target) == RTTI_TERRAIN ? &Terrains[Target_Get_ID(target)] : nullptr;
#else
    return nullptr;
#endif
}

TechnoClass *As_Techno(target_t target)
{
#ifdef GAME_DLL
    TechnoClass *(*func)(target_t) = reinterpret_cast<TechnoClass *(*)(target_t)>(0x0055514C);
    return func(target);
#else
    ObjectClass *objptr = As_Object(target);
    DEBUG_ASSERT(objptr != nullptr);
    return objptr != nullptr && objptr->Is_Techno() ? (TechnoClass *)objptr : nullptr;
#endif
}

TechnoTypeClass *As_TechnoType(target_t target)
{
#ifdef GAME_DLL
    TechnoTypeClass *(*func)(target_t) = reinterpret_cast<TechnoTypeClass *(*)(target_t)>(0x00555964);
    return func(target);
#else
    // TODO: Requires all derivant TechnoTypeClass's to be implemented.
    return nullptr;
#endif
}

AircraftClass *As_Aircraft(target_t target)
{
#ifdef GAME_DLL
    AircraftClass *(*func)(target_t) = reinterpret_cast<AircraftClass *(*)(target_t)>(0x00555114);
    return func(target);
#else
    // TODO: Requires AircraftClass implementation.
    return 0;
#endif
}

UnitClass *As_Unit(target_t target)
{
#ifdef GAME_DLL
    UnitClass *(*func)(target_t) = reinterpret_cast<UnitClass *(*)(target_t)>(0x00555290);
    return func(target);
#else
    // TODO: Requires UnitClass implementation.
    return 0;
#endif
}

VesselClass *As_Vessel(target_t target)
{
#ifdef GAME_DLL
    VesselClass *(*func)(target_t) = reinterpret_cast<VesselClass *(*)(target_t)>(0x005552C8);
    return func(target);
#else
    // TODO: Requires VesselClass implementation.
    return nullptr;
#endif
}

InfantryClass *As_Infantry(target_t target)
{
#ifdef GAME_DLL
    InfantryClass *(*func)(target_t) = reinterpret_cast<InfantryClass *(*)(target_t)>(0x00555300);
    return func(target);
#else
    // TODO: Requires InfantryClass implementation.
    return nullptr;
#endif
}

BuildingClass *As_Building(target_t target)
{
#ifdef GAME_DLL
    BuildingClass *(*func)(target_t) = reinterpret_cast<BuildingClass *(*)(target_t)>(0x00555338);
    return func(target);
#else
    // TODO: Requires BuildingClass implementation.
    return nullptr;
#endif
}

cell_t As_Cell(target_t target)
{
    return Coord_To_Cell(As_Coord(target));
}

coord_t As_Coord(target_t target)
{
#ifdef GAME_DLL
    coord_t (*func)(target_t) = reinterpret_cast<coord_t (*)(target_t)>(0x00555384);
    return func(target);
#else
    if (!Target_Legal(target)) {
        return 0;
}
    if (!Target_Is_Cell(target)) {
        ObjectClass *objptr = As_Object(target);
        return (objptr != nullptr ? objptr->Target_Coord() : 0);
    }
    lepton_t lx = 16 * (Target_Get_ID(target) & 0xFFF) + 8;
    lepton_t ly = 16 * ((Target_Get_ID(target) >> 12) & 0xFFF) + 8;
    return Coord_From_Lepton_XY(lx, ly);
#endif
}

coord_t As_Movement_Coord(target_t target)
{
#ifdef GAME_DLL
    coord_t(*func)(target_t) = reinterpret_cast<coord_t(*)(target_t)>(0x005553F8);
    return func(target);
#else
    if (!Target_Legal(target)) {
        return 0;
    }
    if (!Target_Is_Cell(target)) {
        ObjectClass *objptr = As_Object(target);
        return (objptr != nullptr ? objptr->Docking_Coord() : 0);
    }
    return Cell_To_Coord(Target_Get_ID(target));
#endif
}
