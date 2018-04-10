/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief INI file parsing for game specific objects/types.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef CCINI_H
#define CCINI_H

#include "always.h"
#include "gametypes.h"
#include "ini.h"
#include "sha.h"
#include "animtype.h"
#include "unittype.h"
#include "bullettype.h"
#include "warheadtype.h"
#include "smudgetype.h"
#include "overlaytype.h"
#include "weapontype.h"
#include "terraintype.h"
#include "triggertype.h"
#include "mission.h"
#include "crate.h"
#include "action.h"
#include "mzone.h"
#include "land.h"
#include "ground.h"
#include "source.h"
#include "theme.h"
#include "movie.h"
#include "voc.h"
#include "vox.h"

class CCINIClass : public INIClass
{
public:
    CCINIClass();
    ~CCINIClass();

    const lepton_t Get_Lepton(const char *section, const char *entry, const lepton_t defvalue = 0) const;
    BOOL Put_Lepton(const char *section, const char *entry, const lepton_t value);

    const MPHType Get_MPHType(const char *section, const char *entry, const MPHType defvalue = MPH_MIN) const;
    BOOL Put_MPHType(const char *section, const char *entry, const MPHType value);

    const MissionType Get_MissionType(const char *section, const char *entry, const MissionType defvalue = MISSION_NONE) const;
    BOOL Put_MissionType(const char *section, const char *entry, const MissionType value);

    const ArmorType Get_ArmorType(const char *section, const char *entry, const ArmorType defvalue = ARMOR_NONE) const;
    BOOL Put_ArmorType(const char *section, const char *entry, const ArmorType value);

    const VocType Get_VocType(const char *section, const char *entry, const VocType defvalue = VOC_NONE) const;
    BOOL Put_VocType(const char *section, const char *entry, const VocType value);

    const VoxType Get_VoxType(const char *section, const char *entry, const VoxType defvalue = VOX_NONE) const;
    BOOL Put_VoxType(const char *section, const char *entry, const VoxType value);

    const AnimType Get_AnimType(const char *section, const char *entry, const AnimType defvalue = ANIM_NONE) const;
    BOOL Put_AnimType(const char *section, const char *entry, const AnimType value);

    const UnitType Get_UnitType(const char *section, const char *entry, const UnitType defvalue = UNIT_NONE) const;
    BOOL Put_UnitType(const char *section, const char *entry, const UnitType value);

    const InfantryType Get_InfantryType(const char *section, const char *entry, const InfantryType defvalue = INFANTRY_NONE) const;
    BOOL Put_InfantryType(const char *section, const char *entry, const InfantryType value);

    const AircraftType Get_AircraftType(const char *section, const char *entry, const AircraftType defvalue = AIRCRAFT_NONE) const;
    BOOL Put_AircraftType(const char *section, const char *entry, const AircraftType value);

    const VesselType Get_VesselType(const char *section, const char *entry, const VesselType defvalue = VESSEL_NONE) const;
    BOOL Put_VesselType(const char *section, const char *entry, const VesselType value);

    const BuildingType Get_BuildingType(const char *section, const char *entry, const BuildingType defvalue = BUILDING_NONE) const;
    BOOL Put_BuildingType(const char *section, const char *entry, const BuildingType value);

    const WeaponType Get_WeaponType(const char *section, const char *entry, const WeaponType defvalue = WEAPON_NONE) const;
    BOOL Put_WeaponType(const char *section, const char *entry, const WeaponType value);

    const WarheadType Get_WarheadType(const char *section, const char *entry, const WarheadType defvalue = WARHEAD_NONE) const;
    BOOL Put_WarheadType(const char *section, const char *entry, const WarheadType value);

    const OverlayType Get_OverlayType(const char *section, const char *entry, const OverlayType defvalue = OVERLAY_NONE) const;
    BOOL Put_OverlayType(const char *section, const char *entry, const OverlayType value);

    const SmudgeType Get_SmudgeType(const char *section, const char *entry, const SmudgeType defvalue = SMUDGE_NONE) const;
    BOOL Put_SmudgeType(const char *section, const char *entry, const SmudgeType value);

    const BulletType Get_BulletType(const char *section, const char *entry, const BulletType defvalue = BULLET_NONE) const;
    BOOL Put_BulletType(const char *section, const char *entry, const BulletType value);

    HousesType Get_HousesType(const char *section, const char *entry, const HousesType defvalue = HOUSES_NONE) const;
    BOOL Put_HousesType(const char *section, const char *entry, const HousesType value);

    const MovieType Get_MovieType(const char *section, const char *entry, const MovieType defvalue = MOVIE_NONE);
    BOOL Put_MovieType(const char *section, const char *entry, const MovieType value);

    const TheaterType Get_TheaterType(const char *section, const char *entry, const TheaterType defvalue = THEATER_NONE);
    BOOL Put_TheaterType(const char *section, const char *entry, const TheaterType value);

    const TriggerTypeClass *Get_TriggerType(const char *section, const char *entry) const;
    BOOL Put_TriggerType(const char *section, const char *entry, const TriggerTypeClass *value);

    const ThemeType Get_ThemeType(const char *section, const char *entry, const ThemeType defvalue = THEME_NONE) const;
    BOOL Put_ThemeType(const char *section, const char *entry, const ThemeType value);

    const SourceType Get_SourceType(const char *section, const char *entry, const SourceType defvalue = SOURCE_NONE) const;
    BOOL Put_SourceType(const char *section, const char *entry, const SourceType value);

    const CrateType Get_CrateType(const char *section, const char *entry, const CrateType defvalue = CRATE_NONE) const;
    BOOL Put_CrateType(const char *section, const char *entry, const CrateType value);

    const LandType Get_LandType(const char *section, const char *entry, const LandType defvalue = LAND_NONE) const;
    BOOL Put_LandType(const char *section, const char *entry, const LandType value);

    const MZoneType Get_MZoneType(const char *section, const char *entry, const MZoneType defvalue = MZONE_NONE) const;
    BOOL Put_MZoneType(const char *section, const char *entry, const MZoneType value);

    const GroundType Get_GroundType(const char *section, const char *entry, const GroundType defvalue = GROUND_NONE) const;
    BOOL Put_GroundType(const char *section, const char *entry, const GroundType value);

    const TerrainType Get_TerrainType(const char *section, const char *entry, const TerrainType defvalue = TERRAIN_NONE) const;
    BOOL Put_TerrainType(const char *section, const char *entry, const TerrainType value);

    const RTTIType Get_RTTIType(const char *section, const char *entry, const RTTIType defvalue = RTTI_NONE) const;
    BOOL Put_RTTIType(const char *section, const char *entry, const RTTIType value);

    const ActionType Get_ActionType(const char *section, const char *entry, const ActionType defvalue = ACTION_NONE) const;
    BOOL Put_ActionType(const char *section, const char *entry, const ActionType value);

    const int Get_Owners(const char *section, const char *entry, const int defvalue = OWNER_NONE) const;
    BOOL Put_Owners(const char *section, const char *entry, const int value);

    const int Get_Units(const char *section, const char *entry, const int defvalue = 0) const;
    BOOL Put_Units(const char *section, const char *entry, const int value);

    const int Get_Infantry(const char *section, const char *entry, const int defvalue = 0) const;
    BOOL Put_Infantry(const char *section, const char *entry, const int value);

    const int Get_Aircrafts(const char *section, const char *entry, const int defvalue = 0) const;
    BOOL Put_Aircrafts(const char *section, const char *entry, const int value);

    const int Get_Vessels(const char *section, const char *entry, const int defvalue = 0) const;
    BOOL Put_Vessels(const char *section, const char *entry, const int value);

    const int Get_Buildings(const char *section, const char *entry, const int defvalue = 0) const;
    BOOL Put_Buildings(const char *section, const char *entry, const int value);

    void Calculate_Message_Digest(void);
    void Invalidate_Message_Digest(void);

    int32_t const Get_Unique_ID(void);

private:
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool DigestValid : 1; // & 1
        };
        int Bitfield;
    };
#else
    bool DigestValid;
#endif
    SHAEngine::SHADigest Digest;
};

#endif // CCINI_H
