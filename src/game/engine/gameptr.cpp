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
#include "animtype.h"
#include "buildingtype.h"
#include "bullettype.h"
#include "housetype.h"
#include "infantrytype.h"
#include "overlaytype.h"
#include "smudgetype.h"
#include "templatetype.h"
#include "terraintype.h"
#include "triggertype.h"
#include "unittype.h"
#include "vesseltype.h"
#include "warheadtype.h"
#include "weapontype.h"

// Template specialisation to initialise the heaps required for GamePtr.
template<> FixedIHeapClass *GamePtr<AircraftTypeClass>::Heap = &g_AircraftTypes;
template<> FixedIHeapClass *GamePtr<AnimTypeClass>::Heap = &AnimTypes;
template<> FixedIHeapClass *GamePtr<BuildingTypeClass>::Heap = &g_BuildingTypes;
template<> FixedIHeapClass *GamePtr<BulletTypeClass>::Heap = &BulletTypes;
template<> FixedIHeapClass *GamePtr<HouseTypeClass>::Heap = &HouseTypes;
template<> FixedIHeapClass *GamePtr<InfantryTypeClass>::Heap = &g_InfantryTypes;
template<> FixedIHeapClass *GamePtr<OverlayTypeClass>::Heap = &OverlayTypes;
template<> FixedIHeapClass *GamePtr<SmudgeTypeClass>::Heap = &SmudgeTypes;
template<> FixedIHeapClass *GamePtr<TemplateTypeClass>::Heap = &TemplateTypes;
template<> FixedIHeapClass *GamePtr<TerrainTypeClass>::Heap = &TerrainTypes;
template<> FixedIHeapClass *GamePtr<TriggerTypeClass>::Heap = &TriggerTypes;
template<> FixedIHeapClass *GamePtr<UnitTypeClass>::Heap = &g_UnitTypes;
template<> FixedIHeapClass *GamePtr<VesselTypeClass>::Heap = &g_VesselTypes;
template<> FixedIHeapClass *GamePtr<WarheadTypeClass>::Heap = &WarheadTypes;
template<> FixedIHeapClass *GamePtr<WeaponTypeClass>::Heap = &WeaponTypes;
