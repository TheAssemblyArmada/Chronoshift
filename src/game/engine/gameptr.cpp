/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Wrapper around heap allocation to make heap IDs behave like pointers.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "gameptr.h"

// Classes required for initialisation below.
#include "aircrafttype.h"
#include "anim.h"
#include "animtype.h"
#include "building.h"
#include "buildingtype.h"
#include "bullet.h"
#include "bullettype.h"
#include "factory.h"
#include "house.h"
#include "housetype.h"
#include "infantry.h"
#include "infantrytype.h"
#include "overlay.h"
#include "overlaytype.h"
#include "smudge.h"
#include "smudgetype.h"
#include "team.h"
#include "teamtype.h"
#include "templatetype.h"
#include "terraintype.h"
#include "triggertype.h"
#include "unit.h"
#include "unittype.h"
#include "vessel.h"
#include "vesseltype.h"
#include "warheadtype.h"
#include "weapontype.h"

#ifndef GAME_DLL
// Template specialisation to initialise the heaps required for GamePtr.
template<> const FixedIHeapClass *GamePtr<AircraftTypeClass>::g_Heap = &g_AircraftTypes;
template<> const FixedIHeapClass *GamePtr<AnimClass>::g_Heap = &g_Anims;
template<> const FixedIHeapClass *GamePtr<AnimTypeClass>::g_Heap = &g_AnimTypes;
template<> const FixedIHeapClass *GamePtr<BuildingClass>::g_Heap = &g_Buildings;
template<> const FixedIHeapClass *GamePtr<BuildingTypeClass>::g_Heap = &g_BuildingTypes;
template<> const FixedIHeapClass *GamePtr<BulletClass>::g_Heap = &g_Bullets;
template<> const FixedIHeapClass *GamePtr<BulletTypeClass>::g_Heap = &g_BulletTypes;
template<> const FixedIHeapClass *GamePtr<FactoryClass>::g_Heap = &g_Factories;
template<> const FixedIHeapClass *GamePtr<HouseClass>::g_Heap = &g_Houses;
template<> const FixedIHeapClass *GamePtr<HouseTypeClass>::g_Heap = &g_HouseTypes;
template<> const FixedIHeapClass *GamePtr<InfantryClass>::g_Heap = &g_Infantry;
template<> const FixedIHeapClass *GamePtr<InfantryTypeClass>::g_Heap = &g_InfantryTypes;
template<> const FixedIHeapClass *GamePtr<OverlayClass>::g_Heap = &g_Overlays;
template<> const FixedIHeapClass *GamePtr<OverlayTypeClass>::g_Heap = &g_OverlayTypes;
template<> const FixedIHeapClass *GamePtr<SmudgeClass>::g_Heap = &g_Smudges;
template<> const FixedIHeapClass *GamePtr<SmudgeTypeClass>::g_Heap = &g_SmudgeTypes;
template<> const FixedIHeapClass *GamePtr<TeamClass>::g_Heap = &g_Teams;
template<> const FixedIHeapClass *GamePtr<TeamTypeClass>::g_Heap = &g_TeamTypes;
template<> const FixedIHeapClass *GamePtr<TemplateTypeClass>::g_Heap = &g_TemplateTypes;
template<> const FixedIHeapClass *GamePtr<TerrainTypeClass>::g_Heap = &g_TerrainTypes;
template<> const FixedIHeapClass *GamePtr<TriggerClass>::g_Heap = &g_Triggers;
template<> const FixedIHeapClass *GamePtr<TriggerTypeClass>::g_Heap = &g_TriggerTypes;
template<> const FixedIHeapClass *GamePtr<UnitClass>::g_Heap = &g_Units;
template<> const FixedIHeapClass *GamePtr<UnitTypeClass>::g_Heap = &g_UnitTypes;
template<> const FixedIHeapClass *GamePtr<VesselClass>::g_Heap = &g_Vessels;
template<> const FixedIHeapClass *GamePtr<VesselTypeClass>::g_Heap = &g_VesselTypes;
template<> const FixedIHeapClass *GamePtr<WarheadTypeClass>::g_Heap = &g_WarheadTypes;
template<> const FixedIHeapClass *GamePtr<WeaponTypeClass>::g_Heap = &g_WeaponTypes;
#endif
