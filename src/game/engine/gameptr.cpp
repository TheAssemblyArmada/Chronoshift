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
#include "factory.h"
#include "house.h"
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
template<> FixedIHeapClass *GamePtr<AircraftTypeClass>::g_Heap = &g_AircraftTypes;
template<> FixedIHeapClass *GamePtr<AnimTypeClass>::g_Heap = &AnimTypes;
template<> FixedIHeapClass *GamePtr<BuildingTypeClass>::g_Heap = &g_BuildingTypes;
template<> FixedIHeapClass *GamePtr<BulletTypeClass>::g_Heap = &BulletTypes;
template<> FixedIHeapClass *GamePtr<FactoryClass>::g_Heap = &g_Factories;
template<> FixedIHeapClass *GamePtr<HouseClass>::g_Heap = &g_Houses;
template<> FixedIHeapClass *GamePtr<HouseTypeClass>::g_Heap = &HouseTypes;
template<> FixedIHeapClass *GamePtr<InfantryTypeClass>::g_Heap = &g_InfantryTypes;
template<> FixedIHeapClass *GamePtr<OverlayTypeClass>::g_Heap = &OverlayTypes;
template<> FixedIHeapClass *GamePtr<SmudgeTypeClass>::g_Heap = &SmudgeTypes;
template<> FixedIHeapClass *GamePtr<TemplateTypeClass>::g_Heap = &TemplateTypes;
template<> FixedIHeapClass *GamePtr<TerrainTypeClass>::g_Heap = &TerrainTypes;
template<> FixedIHeapClass *GamePtr<TriggerTypeClass>::g_Heap = &TriggerTypes;
template<> FixedIHeapClass *GamePtr<UnitTypeClass>::g_Heap = &g_UnitTypes;
template<> FixedIHeapClass *GamePtr<VesselTypeClass>::g_Heap = &g_VesselTypes;
template<> FixedIHeapClass *GamePtr<WarheadTypeClass>::g_Heap = &WarheadTypes;
template<> FixedIHeapClass *GamePtr<WeaponTypeClass>::g_Heap = &WeaponTypes;
