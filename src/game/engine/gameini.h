/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief INI file parsing for game specific objects/types.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef GAMEINI_H
#define GAMEINI_H

#include "always.h"
#include "gametypes.h"
#include "ini.h"
#include "sha.h"
#include "animtype.h"
#include "bullettype.h"
#include "warheadtype.h"
#include "smudgetype.h"
#include "overlaytype.h"
#include "weapontype.h"
#include "terraintype.h"
#include "triggertype.h"
#include "aircrafttype.h"
#include "infantrytype.h"
#include "buildingtype.h"
#include "vesseltype.h"
#include "unittype.h"
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
#include "pk.h"

class GameINIClass : public INIClass
{
public:
    GameINIClass();
    ~GameINIClass() {}

    int Save(FileClass &file, BOOL save_digest = false);
    int Save(Pipe &pipe, BOOL save_digest = false);
    int Load(FileClass &file, BOOL load_digest = false);
    int Load(Straw &straw, BOOL load_digest = false);

    lepton_t Get_Lepton(const char *section, const char *entry, lepton_t defvalue = 0) const;
    BOOL Put_Lepton(const char *section, const char *entry, lepton_t value);

    MPHType Get_MPHType(const char *section, const char *entry, MPHType defvalue = MPH_MIN) const;
    BOOL Put_MPHType(const char *section, const char *entry, MPHType value);

    MissionType Get_MissionType(const char *section, const char *entry, MissionType defvalue = MISSION_NONE) const;
    BOOL Put_MissionType(const char *section, const char *entry, MissionType value);

    ArmorType Get_ArmorType(const char *section, const char *entry, ArmorType defvalue = ARMOR_NONE) const;
    BOOL Put_ArmorType(const char *section, const char *entry, ArmorType value);

    VocType Get_VocType(const char *section, const char *entry, VocType defvalue = VOC_NONE) const;
    BOOL Put_VocType(const char *section, const char *entry, VocType value);

    VoxType Get_VoxType(const char *section, const char *entry, VoxType defvalue = VOX_NONE) const;
    BOOL Put_VoxType(const char *section, const char *entry, VoxType value);

    AnimType Get_AnimType(const char *section, const char *entry, AnimType defvalue = ANIM_NONE) const;
    BOOL Put_AnimType(const char *section, const char *entry, AnimType value);

    UnitType Get_UnitType(const char *section, const char *entry, UnitType defvalue = UNIT_NONE) const;
    BOOL Put_UnitType(const char *section, const char *entry, UnitType value);

    InfantryType Get_InfantryType(const char *section, const char *entry, InfantryType defvalue = INFANTRY_NONE) const;
    BOOL Put_InfantryType(const char *section, const char *entry, InfantryType value);

    AircraftType Get_AircraftType(const char *section, const char *entry, AircraftType defvalue = AIRCRAFT_NONE) const;
    BOOL Put_AircraftType(const char *section, const char *entry, AircraftType value);

    VesselType Get_VesselType(const char *section, const char *entry, VesselType defvalue = VESSEL_NONE) const;
    BOOL Put_VesselType(const char *section, const char *entry, VesselType value);

    BuildingType Get_BuildingType(const char *section, const char *entry, BuildingType defvalue = BUILDING_NONE) const;
    BOOL Put_BuildingType(const char *section, const char *entry, BuildingType value);

    WeaponType Get_WeaponType(const char *section, const char *entry, WeaponType defvalue = WEAPON_NONE) const;
    BOOL Put_WeaponType(const char *section, const char *entry, WeaponType value);

    WarheadType Get_WarheadType(const char *section, const char *entry, WarheadType defvalue = WARHEAD_NONE) const;
    BOOL Put_WarheadType(const char *section, const char *entry, WarheadType value);

    OverlayType Get_OverlayType(const char *section, const char *entry, OverlayType defvalue = OVERLAY_NONE) const;
    BOOL Put_OverlayType(const char *section, const char *entry, OverlayType value);

    SmudgeType Get_SmudgeType(const char *section, const char *entry, SmudgeType defvalue = SMUDGE_NONE) const;
    BOOL Put_SmudgeType(const char *section, const char *entry, SmudgeType value);

    BulletType Get_BulletType(const char *section, const char *entry, BulletType defvalue = BULLET_NONE) const;
    BOOL Put_BulletType(const char *section, const char *entry, BulletType value);

    HousesType Get_HousesType(const char *section, const char *entry, HousesType defvalue = HOUSES_NONE) const;
    BOOL Put_HousesType(const char *section, const char *entry, HousesType value);

    MovieType Get_MovieType(const char *section, const char *entry, MovieType defvalue = MOVIE_NONE);
    BOOL Put_MovieType(const char *section, const char *entry, MovieType value);

    TheaterType Get_TheaterType(const char *section, const char *entry, TheaterType defvalue = THEATER_NONE);
    BOOL Put_TheaterType(const char *section, const char *entry, TheaterType value);

    TriggerTypeClass *Get_TriggerType(const char *section, const char *entry) const;
    BOOL Put_TriggerType(const char *section, const char *entry, TriggerTypeClass *value);

    ThemeType Get_ThemeType(const char *section, const char *entry, ThemeType defvalue = THEME_NONE) const;
    BOOL Put_ThemeType(const char *section, const char *entry, ThemeType value);

    SourceType Get_SourceType(const char *section, const char *entry, SourceType defvalue = SOURCE_NONE) const;
    BOOL Put_SourceType(const char *section, const char *entry, SourceType value);

    CrateType Get_CrateType(const char *section, const char *entry, CrateType defvalue = CRATE_NONE) const;
    BOOL Put_CrateType(const char *section, const char *entry, CrateType value);

    LandType Get_LandType(const char *section, const char *entry, LandType defvalue = LAND_NONE) const;
    BOOL Put_LandType(const char *section, const char *entry, LandType value);

    MZoneType Get_MZoneType(const char *section, const char *entry, MZoneType defvalue = MZONE_NONE) const;
    BOOL Put_MZoneType(const char *section, const char *entry, MZoneType value);

    GroundType Get_GroundType(const char *section, const char *entry, GroundType defvalue = GROUND_NONE) const;
    BOOL Put_GroundType(const char *section, const char *entry, GroundType value);

    TerrainType Get_TerrainType(const char *section, const char *entry, TerrainType defvalue = TERRAIN_NONE) const;
    BOOL Put_TerrainType(const char *section, const char *entry, TerrainType value);

    RTTIType Get_RTTIType(const char *section, const char *entry, RTTIType defvalue = RTTI_NONE) const;
    BOOL Put_RTTIType(const char *section, const char *entry, RTTIType value);

    ActionType Get_ActionType(const char *section, const char *entry, ActionType defvalue = ACTION_NONE) const;
    BOOL Put_ActionType(const char *section, const char *entry, ActionType value);

    uint32_t Get_Owners(const char *section, const char *entry, uint32_t defvalue = OWNER_NONE) const;
    BOOL Put_Owners(const char *section, const char *entry, uint32_t value);

    uint32_t Get_Units(const char *section, const char *entry, uint32_t defvalue = 0) const;
    BOOL Put_Units(const char *section, const char *entry, uint32_t value);

    uint32_t Get_Infantry(const char *section, const char *entry, uint32_t defvalue = 0) const;
    BOOL Put_Infantry(const char *section, const char *entry, uint32_t value);

    uint32_t Get_Aircrafts(const char *section, const char *entry, uint32_t defvalue = 0) const;
    BOOL Put_Aircrafts(const char *section, const char *entry, uint32_t value);

    uint32_t Get_Vessels(const char *section, const char *entry, uint32_t defvalue = 0) const;
    BOOL Put_Vessels(const char *section, const char *entry, uint32_t value);

    uint32_t Get_Buildings(const char *section, const char *entry, uint32_t defvalue = 0) const;
    BOOL Put_Buildings(const char *section, const char *entry, uint32_t value);

    KeyNumType Get_KeyNumType(const char *section, const char *entry, KeyNumType defvalue = KN_NONE) const;
    BOOL Put_KeyNumType(const char *section, const char *entry, KeyNumType value);

    const PKey Get_PKey(BOOL fast) const;
    BOOL Put_PKey(PKey &key);

    void Calculate_Message_Digest();
    void Invalidate_Message_Digest();

    int32_t Get_Unique_ID();

private:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_DigestValid : 1; // & 1
#else
    bool m_DigestValid;
#endif
    SHAEngine::SHADigest m_Digest;
};

#endif // GAMEINI_H
