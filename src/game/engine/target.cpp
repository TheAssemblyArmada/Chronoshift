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
#include "target.h"
#include "aircraft.h"
#include "anim.h"
#include "building.h"
#include "bullet.h"
#include "cell.h"
#include "coord.h"
#include "infantry.h"
#include "iomap.h"
#include "object.h"
#include "overlay.h"
#include "smudge.h"
#include "techno.h"
#include "technotype.h"
#include "terrain.h"
#include "triggertype.h"
#include "unit.h"
#include "vessel.h"

TargetClass::TargetClass(AbstractClass *abstract)
{
    if (abstract != nullptr) {
        m_Target = (abstract->What_Am_I() << 24) | (abstract->Get_Heap_ID() & 0xFFFFFF);
    } else {
        m_RTTI = RTTI_NONE;
    }
}

TargetClass::TargetClass(AbstractTypeClass *abstractype)
{
    if (abstractype != nullptr) {
        m_Target = (abstractype->What_Am_I() << 24) | (abstractype->Get_Heap_ID() & 0xFFFFFF);
    } else {
        m_RTTI = RTTI_NONE;
    }
}

TargetClass::TargetClass(CellClass *cell)
{
    if (cell != nullptr) {
        m_Target = (RTTI_CELL << 24) | (cell->Cell_Number() & 0xFFFFFF);
    } else {
        m_RTTI = RTTI_NONE;
    }
}

CellClass *TargetClass::As_Cell() const
{
    if (Get_RTTI() == RTTI_CELL) {
        return &g_Map[Get_ID()];
    }

    return nullptr;
}

BOOL Target_Legal(target_t target)
{
    // NOTE: Possible future improvement that could be a bug fix.
    // return (Target_Get_RTTI(target) != RTTI_NONE && Target_Get_RTTI(target) < RTTI_COUNT && Target_Get_ID(target) >= 0) &&
    // As_Object(target)->Is_Active();
    return (Target_Get_RTTI(target) != RTTI_NONE && Target_Get_RTTI(target) < RTTI_COUNT && Target_Get_ID(target) >= 0);
}

target_t As_Target(cell_t cellnum)
{
    return Make_Target(RTTI_CELL, (16 * Cell_Get_X(cellnum) + 8)) | ((16 * Cell_Get_Y(cellnum) + 8) << 12);
}

target_t As_Target(coord_t coord)
{
    return Make_Target(RTTI_CELL, (Coord_Lepton_X(coord) / 16) | ((Coord_Lepton_Y(coord) / 16) << 12));
}

TriggerClass *As_Trigger(target_t target)
{
    return Target_Is_Trigger(target) ? &g_Triggers[Target_Get_ID(target)] : nullptr;
}

TriggerTypeClass *As_TriggerType(target_t target)
{
    return Target_Is_TriggerType(target) ? &g_TriggerTypes[Target_Get_ID(target)] : nullptr;
}

TeamClass *As_Team(target_t target)
{
    return Target_Is_Team(target) ? &g_Teams[Target_Get_ID(target)] : nullptr;
}

TeamTypeClass *As_TeamType(target_t target)
{
    return Target_Is_TeamType(target) ? &g_TeamTypes[Target_Get_ID(target)] : nullptr;
}

ObjectClass *As_Object(target_t target)
{
    int id = Target_Get_ID(target);
    ObjectClass *objptr = nullptr;

    // NOTE: Possible future improvement that could be a bug fix.
    // This returns only targetable Objects, in contrast
    // xTargetClass::As_Object also returns Smudge and Overlay
    switch (Target_Get_RTTI(target)) {
        case RTTI_AIRCRAFT:
            objptr = &g_Aircraft[id];
            break;

        case RTTI_ANIM:
            objptr = &g_Anims[id];
            break;

        case RTTI_BUILDING:
            objptr = &g_Buildings[id];
            break;

        case RTTI_BULLET:
            objptr = &g_Bullets[id];
            break;

        case RTTI_INFANTRY:
            objptr = &g_Infantry[id];
            break;

        case RTTI_TERRAIN:
            objptr = &g_Terrains[id];
            break;

        case RTTI_UNIT:
            objptr = &g_Units[id];
            break;

        case RTTI_VESSEL:
            objptr = &g_Vessels[id];
            break;

        default:
            break;
    }

    if (objptr != nullptr && objptr->Is_Active()) {
        return objptr;
    }

    return nullptr;
}

AnimClass *As_Animation(target_t target)
{
    return Target_Is_Anim(target) ? &g_Anims[Target_Get_ID(target)] : nullptr;
}

BulletClass *As_Bullet(target_t target)
{
    return Target_Is_Bullet(target) ? &g_Bullets[Target_Get_ID(target)] : nullptr;
}

TerrainClass *As_Terrain(target_t target)
{
    return Target_Is_Terrain(target) ? &g_Terrains[Target_Get_ID(target)] : nullptr;
}

TechnoClass *As_Techno(target_t target)
{
    ObjectClass *optr = As_Object(target);

    if (optr != nullptr && optr->Is_Techno()) {
        return reinterpret_cast<TechnoClass *>(optr);
    }

    return nullptr;
}

TechnoTypeClass *As_TechnoType(target_t target)
{
    int id = Target_Get_ID(target);

    switch (Target_Get_RTTI(target)) {
        case RTTI_INFANTRYTYPE:
            return InfantryTypeClass::As_Pointer(InfantryType(id));
        case RTTI_UNITTYPE:
            return UnitTypeClass::As_Pointer(UnitType(id));
        case RTTI_VESSELTYPE:
            return VesselTypeClass::As_Pointer(VesselType(id));
        case RTTI_AIRCRAFTTYPE:
            return AircraftTypeClass::As_Pointer(AircraftType(id));
        case RTTI_BUILDINGTYPE:
            return BuildingTypeClass::As_Pointer(BuildingType(id));
        default:
            break;
    }
    return nullptr;
}

AircraftClass *As_Aircraft(target_t target)
{
    return Target_Is_Aircraft(target) ? &g_Aircraft[Target_Get_ID(target)] : nullptr;
}

UnitClass *As_Unit(target_t target)
{
    return Target_Is_Unit(target) ? &g_Units[Target_Get_ID(target)] : nullptr;
}

VesselClass *As_Vessel(target_t target)
{
    return Target_Is_Vessel(target) ? &g_Vessels[Target_Get_ID(target)] : nullptr;
}

InfantryClass *As_Infantry(target_t target)
{
    return Target_Is_Infantry(target) ? &g_Infantry[Target_Get_ID(target)] : nullptr;
}

BuildingClass *As_Building(target_t target)
{
    return Target_Is_Building(target) ? &g_Buildings[Target_Get_ID(target)] : nullptr;
}

cell_t As_Cell(target_t target)
{
    return Coord_To_Cell(As_Coord(target));
}

coord_t As_Coord(target_t target)
{
    if (!Target_Legal(target)) {
        return 0;
    }

    if (Target_Is_Cell(target)) {
        int id = Target_Get_ID(target);
        lepton_t lx = 16 * (id & 0xFFF) + 8;
        lepton_t ly = 16 * ((id >> 12) & 0xFFF) + 8;
        return Coord_From_Lepton_XY(lx, ly);
    }

    ObjectClass *objptr = As_Object(target);

    if (objptr != nullptr) {
        captainslog_assert(objptr->Is_Active());
        return objptr->Target_Coord();
    }
    return 0;
}

coord_t As_Movement_Coord(target_t target)
{
    if (!Target_Legal(target)) {
        return 0;
    }

    if (Target_Is_Cell(target)) {
        int id = Target_Get_ID(target);
        return Cell_To_Coord(id);
    }

    ObjectClass *objptr = As_Object(target);

    if (objptr != nullptr) {
        captainslog_assert(objptr->Is_Active());
        return objptr->Docking_Coord();
    }
    return 0;
}

CellClass *xTargetClass::As_Cell() const
{
    if (Get_RTTI() == RTTI_CELL) {
        return &g_Map[Get_ID()];
    }

    return nullptr;
}

AbstractClass *xTargetClass::As_Abstract() const
{
    const int id = Get_ID();

    switch (Get_RTTI()) {
        case RTTI_TEAM:
            return &g_Teams[id];
        case RTTI_BULLET:
            return &g_Bullets[id];
        case RTTI_OVERLAY:
            return &g_Overlays[id];
        case RTTI_SMUDGE:
            return &g_Smudges[id];
        case RTTI_UNIT:
            return &g_Units[id];
        case RTTI_VESSEL:
            return &g_Vessels[id];
        case RTTI_BUILDING:
            return &g_Buildings[id];
        case RTTI_INFANTRY:
            return &g_Infantry[id];
        case RTTI_AIRCRAFT:
            return &g_Aircraft[id];
        case RTTI_TERRAIN:
            return &g_Terrains[id];
        case RTTI_ANIM:
            return &g_Anims[id];
        default:
            break;
    }
    return nullptr;
}

AbstractTypeClass *xTargetClass::As_TypeClass() const
{
    const int id = Get_ID();

    switch (Get_RTTI()) {
        // TODO: look into this, i don't think the RTTI checks not checking TYPE are correct...
        case RTTI_TEAMTYPE:
            return TeamTypeClass::As_Pointer(TeamType(id));
        case RTTI_TRIGGERTYPE:
            return TriggerTypeClass::As_Pointer(TriggerType(id));
        case RTTI_BULLETTYPE:
            return BulletTypeClass::As_Pointer(BulletType(id));
        case RTTI_OVERLAY:
            return OverlayTypeClass::As_Pointer(OverlayType(id));
        case RTTI_SMUDGE:
            return SmudgeTypeClass::As_Pointer(SmudgeType(id));
        case RTTI_UNIT:
            return UnitTypeClass::As_Pointer(UnitType(id));
        case RTTI_VESSEL:
            return VesselTypeClass::As_Pointer(VesselType(id));
        case RTTI_BUILDING:
            return BuildingTypeClass::As_Pointer(BuildingType(id));
        case RTTI_INFANTRY:
            return InfantryTypeClass::As_Pointer(InfantryType(id));
        case RTTI_AIRCRAFT:
            return AircraftTypeClass::As_Pointer(AircraftType(id));
        case RTTI_TERRAIN:
            return TerrainTypeClass::As_Pointer(TerrainType(id));
        case RTTI_ANIM:
            return AnimTypeClass::As_Pointer(AnimType(id));
        default:
            break;
    }
    return nullptr;
}

TechnoClass *xTargetClass::As_Techno() const
{
    const int id = Get_ID();

    switch (Get_RTTI()) {
        case RTTI_UNIT:
            return &g_Units[id];
        case RTTI_VESSEL:
            return &g_Vessels[id];
        case RTTI_BUILDING:
            return &g_Buildings[id];
        case RTTI_INFANTRY:
            return &g_Infantry[id];
        case RTTI_AIRCRAFT:
            return &g_Aircraft[id];
        default:
            break;
    }
    return nullptr;
}

ObjectClass *xTargetClass::As_Object() const
{
    const int id = Get_ID();

    switch (Get_RTTI()) {
        case RTTI_TERRAIN:
            return &g_Terrains[id];
        case RTTI_SMUDGE:
            return &g_Smudges[id];
        case RTTI_OVERLAY:
            return &g_Overlays[id];
        case RTTI_BULLET:
            return &g_Bullets[id];
        case RTTI_ANIM:
            return &g_Anims[id];
        case RTTI_UNIT:
            return &g_Units[id];
        case RTTI_VESSEL:
            return &g_Vessels[id];
        case RTTI_BUILDING:
            return &g_Buildings[id];
        case RTTI_INFANTRY:
            return &g_Infantry[id];
        case RTTI_AIRCRAFT:
            return &g_Aircraft[id];
        default:
            break;
    }
    return nullptr;
}

AircraftClass *xTargetClass::As_Aircraft() const
{
    if (Get_RTTI() == RTTI_AIRCRAFT) {
        return reinterpret_cast<AircraftClass *>(As_Techno());
    }

    return nullptr;
}

UnitClass *xTargetClass::As_Unit() const
{
    if (Get_RTTI() == RTTI_UNIT) {
        return reinterpret_cast<UnitClass *>(As_Techno());
    }

    return nullptr;
}

VesselClass *xTargetClass::As_Vessel() const
{
    if (Get_RTTI() == RTTI_VESSEL) {
        return reinterpret_cast<VesselClass *>(As_Techno());
    }

    return nullptr;
}

InfantryClass *xTargetClass::As_Infantry() const
{
    if (Get_RTTI() == RTTI_UNIT) {
        return reinterpret_cast<InfantryClass *>(As_Techno());
    }

    return nullptr;
}

BuildingClass *xTargetClass::As_Building() const
{
    if (Get_RTTI() == RTTI_BUILDING) {
        return reinterpret_cast<BuildingClass *>(As_Techno());
    }

    return nullptr;
}
