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
#include "bullettype.h"
#include "factory.h"
#include "house.h"
#include "housetype.h"
#include "infantry.h"
#include "infantrytype.h"
#include "overlaytype.h"
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
template<> FixedIHeapClass *const GamePtr<AircraftTypeClass>::g_Heap = &g_AircraftTypes;
template<> FixedIHeapClass *const GamePtr<AnimClass>::g_Heap = &g_Anims;
template<> FixedIHeapClass *const GamePtr<AnimTypeClass>::g_Heap = &g_AnimTypes;
template<> FixedIHeapClass *const GamePtr<BuildingClass>::g_Heap = &g_Buildings;
template<> FixedIHeapClass *const GamePtr<BuildingTypeClass>::g_Heap = &g_BuildingTypes;
template<> FixedIHeapClass *const GamePtr<BulletTypeClass>::g_Heap = &g_BulletTypes;
template<> FixedIHeapClass *const GamePtr<FactoryClass>::g_Heap = &g_Factories;
template<> FixedIHeapClass *const GamePtr<HouseClass>::g_Heap = &g_Houses;
template<> FixedIHeapClass *const GamePtr<HouseTypeClass>::g_Heap = &g_HouseTypes;
template<> FixedIHeapClass *const GamePtr<InfantryClass>::g_Heap = &g_Infantry;
template<> FixedIHeapClass *const GamePtr<InfantryTypeClass>::g_Heap = &g_InfantryTypes;
template<> FixedIHeapClass *const GamePtr<OverlayTypeClass>::g_Heap = &g_OverlayTypes;
template<> FixedIHeapClass *const GamePtr<SmudgeTypeClass>::g_Heap = &g_SmudgeTypes;
template<> FixedIHeapClass *const GamePtr<TeamClass>::g_Heap = &g_Teams;
template<> FixedIHeapClass *const GamePtr<TeamTypeClass>::g_Heap = &g_TeamTypes;
template<> FixedIHeapClass *const GamePtr<TemplateTypeClass>::g_Heap = &g_TemplateTypes;
template<> FixedIHeapClass *const GamePtr<TerrainTypeClass>::g_Heap = &g_TerrainTypes;
template<> FixedIHeapClass *const GamePtr<TriggerTypeClass>::g_Heap = &g_TriggerTypes;
template<> FixedIHeapClass *const GamePtr<UnitClass>::g_Heap = &g_Units;
template<> FixedIHeapClass *const GamePtr<UnitTypeClass>::g_Heap = &g_UnitTypes;
template<> FixedIHeapClass *const GamePtr<VesselClass>::g_Heap = &g_Vessels;
template<> FixedIHeapClass *const GamePtr<VesselTypeClass>::g_Heap = &g_VesselTypes;
template<> FixedIHeapClass *const GamePtr<WarheadTypeClass>::g_Heap = &g_WarheadTypes;
template<> FixedIHeapClass *const GamePtr<WeaponTypeClass>::g_Heap = &g_WeaponTypes;
#else
#include "hooker.h"
template<>
FixedIHeapClass *const GamePtr<AircraftTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DDBC);
template<>
FixedIHeapClass *const GamePtr<AnimClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065D864);
template<>
FixedIHeapClass *const GamePtr<AnimTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DEA0);
template<>
FixedIHeapClass *const GamePtr<BuildingClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065D8B0);
template<>
FixedIHeapClass *const GamePtr<BuildingTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DD70);
template<>
FixedIHeapClass *const GamePtr<BulletTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DE54);
template<>
FixedIHeapClass *const GamePtr<FactoryClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065D948);
template<>
FixedIHeapClass *const GamePtr<HouseClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065D994);
template<>
FixedIHeapClass *const GamePtr<HouseTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DD24);
template<>
FixedIHeapClass *const GamePtr<InfantryClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065D9E0);
template<>
FixedIHeapClass *const GamePtr<InfantryTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DE08);
template<>
FixedIHeapClass *const GamePtr<OverlayTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065E01C);
template<>
FixedIHeapClass *const GamePtr<SmudgeTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065E068);
template<>
FixedIHeapClass *const GamePtr<TeamClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DAC4);
template<>
FixedIHeapClass *const GamePtr<TeamTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DB10);
template<>
FixedIHeapClass *const GamePtr<TemplateTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DF84);
template<>
FixedIHeapClass *const GamePtr<TerrainTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DFD0);
template<>
FixedIHeapClass *const GamePtr<TriggerTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DCD8);
template<>
FixedIHeapClass *const GamePtr<UnitClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DC40);
template<>
FixedIHeapClass *const GamePtr<UnitTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DEEC);
template<>
FixedIHeapClass *const GamePtr<VesselClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DC8C);
template<>
FixedIHeapClass *const GamePtr<VesselTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0065DF38);
template<>
FixedIHeapClass *const GamePtr<WarheadTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x00691600);
template<>
FixedIHeapClass *const GamePtr<WeaponTypeClass>::g_Heap = Make_Pointer<FixedIHeapClass>(0x0069164C);
#endif
