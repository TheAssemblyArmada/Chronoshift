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
#include "gameini.h"
#include "crc.h"
#include "shapipe.h"
#include <algorithm>

#ifndef GAME_DLL
extern GameINIClass g_RuleINI;
extern GameINIClass g_AftermathINI;
#endif

GameINIClass::GameINIClass() :
    m_DigestValid(false)
{
}

GameINIClass::~GameINIClass()
{
}

const lepton_t GameINIClass::Get_Lepton(const char *section, const char *entry, const lepton_t defvalue) const
{
    return Get_Fixed(section, entry, fixed_t(defvalue, 256)) * 256;
}

BOOL GameINIClass::Put_Lepton(const char *section, const char *entry, const lepton_t value)
{
#ifdef GAME_DLL
    BOOL (*func)
    (GameINIClass *, const char *, const char *, const lepton_t) =
        reinterpret_cast<BOOL (*)(GameINIClass *, const char *, const char *, const lepton_t)>(0x004630FC);
    return func(this, section, entry, value);
#else
    // TODO: OmniBlade, please implement.
    return false;
#endif
}

const MPHType GameINIClass::Get_MPHType(const char *section, const char *entry, const MPHType defvalue) const
{
    int value = std::clamp(Get_Int(section, entry, (100 * defvalue) / 256), 0, 100);

    return (MPHType)std::min((value * 256) / 100, 255);
}

BOOL GameINIClass::Put_MPHType(const char *section, const char *entry, const MPHType value)
{
#ifdef GAME_DLL
    BOOL (*func)
    (GameINIClass *, const char *, const char *, const MPHType) =
        reinterpret_cast<BOOL (*)(GameINIClass *, const char *, const char *, const MPHType)>(0x004631A4);
    return func(this, section, entry, value);
#else
    // TODO: OmniBlade, please implement.
    return false;
#endif
}

HousesType GameINIClass::Get_HousesType(const char *section, const char *entry, const HousesType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, HouseTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return HouseTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_HousesType(const char *section, const char *entry, const HousesType value)
{
    return Put_String(section, entry, HouseTypeClass::Name_From(value));
}

const MovieType GameINIClass::Get_MovieType(const char *section, const char *entry, const MovieType defvalue)
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Movie(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Movie_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_MovieType(const char *section, const char *entry, const MovieType value)
{
    return Put_String(section, entry, Name_From_Movie(value));
}

const TheaterType GameINIClass::Get_TheaterType(const char *section, const char *entry, const TheaterType defvalue)
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Theater(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Theater_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_TheaterType(const char *section, const char *entry, const TheaterType value)
{
    return Put_String(section, entry, Name_From_Theater(value));
}

const TriggerTypeClass *GameINIClass::Get_TriggerType(const char *section, const char *entry) const
{
#ifdef GAME_DLL
    TriggerTypeClass *(*func)(GameINIClass *, const char *, const char *) =
        reinterpret_cast<TriggerTypeClass *(*)(GameINIClass *, const char *, const char *)>(0x004638BC);
    return func((GameINIClass *)this, section, entry);
#elif 0
    // TODO: Requires TriggerTypeClass interface to be complete
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, "<none>", valuebuf, sizeof(valuebuf)) > 0) {
        return TriggerTypeClass::From_Name(valuebuf);
    }

    return defvalue;
#else
    return nullptr;
#endif
}

BOOL GameINIClass::Put_TriggerType(const char *section, const char *entry, const TriggerTypeClass *trigger)
{
#ifdef GAME_DLL
    BOOL (*func)
    (GameINIClass *, const char *, const char *, const TriggerTypeClass *) =
        reinterpret_cast<BOOL (*)(GameINIClass *, const char *, const char *, const TriggerTypeClass *)>(0x004638F4);
    return func(this, section, entry, trigger);
#else
    return Put_String(section, entry, trigger != nullptr ? trigger->Get_Name() : "<none>");
#endif
}

const ThemeType GameINIClass::Get_ThemeType(const char *section, const char *entry, const ThemeType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, g_Theme.Base_Name(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return g_Theme.From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_ThemeType(const char *section, const char *entry, const ThemeType value)
{
    return Put_String(section, entry, g_Theme.Base_Name(value));
}

const SourceType GameINIClass::Get_SourceType(const char *section, const char *entry, const SourceType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Source(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Source_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_SourceType(const char *section, const char *entry, const SourceType value)
{
    return Put_String(section, entry, Name_From_Source(value));
}

const CrateType GameINIClass::Get_CrateType(const char *section, const char *entry, const CrateType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, CrateClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return CrateClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_CrateType(const char *section, const char *entry, const CrateType value)
{
    return Put_String(section, entry, CrateClass::Name_From(value));
}

const MissionType GameINIClass::Get_MissionType(const char *section, const char *entry, const MissionType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, MissionClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return MissionClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_MissionType(const char *section, const char *entry, const MissionType value)
{
    return Put_String(section, entry, MissionClass::Name_From(value));
}

const ArmorType GameINIClass::Get_ArmorType(const char *section, const char *entry, const ArmorType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Armor(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Armor_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_ArmorType(const char *section, const char *entry, const ArmorType value)
{
    return Put_String(section, entry, Name_From_Armor(value));
}

const VocType GameINIClass::Get_VocType(const char *section, const char *entry, const VocType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Voc(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Voc_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_VocType(const char *section, const char *entry, const VocType value)
{
    return Put_String(section, entry, Name_From_Voc(value));
}

const VoxType GameINIClass::Get_VoxType(const char *section, const char *entry, const VoxType defvalue) const
{
#ifdef GAME_DLL
    // Inlined in RA
    return VOX_NONE;
#else
    char valuebuf[MAX_LINE_LENGTH];

    // TODO: Requires Vox interface to be complete
    if (Get_String(section, entry, Name_From_Vox(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Vox_From_Name(valuebuf);
    }

    return defvalue;
#endif
}

BOOL GameINIClass::Put_VoxType(const char *section, const char *entry, const VoxType value)
{
#ifdef GAME_DLL
    // Inlined in RA
    return false;
#else
    // TODO: Requires Vox interface to be complete
    return Put_String(section, entry, Name_From_Vox(value));
#endif
}

const AnimType GameINIClass::Get_AnimType(const char *section, const char *entry, const AnimType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, AnimTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return AnimTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_AnimType(const char *section, const char *entry, const AnimType value)
{
    return Put_String(section, entry, AnimTypeClass::Name_From(value));
}

const UnitType GameINIClass::Get_UnitType(const char *section, const char *entry, const UnitType defvalue) const
{
#ifdef GAME_DLL
    UnitType (*func)(GameINIClass *, const char *, const char *, const UnitType) =
        reinterpret_cast<UnitType (*)(GameINIClass *, const char *, const char *, const UnitType)>(0x004631A4);
    return func((GameINIClass *)this, section, entry, defvalue);
#else
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, UnitTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return UnitTypeClass::From_Name(valuebuf);
    }

    return defvalue;
#endif
}

BOOL GameINIClass::Put_UnitType(const char *section, const char *entry, const UnitType value)
{
#ifdef GAME_DLL
    BOOL (*func)
    (GameINIClass *, const char *, const char *, const UnitType) =
        reinterpret_cast<BOOL (*)(GameINIClass *, const char *, const char *, const UnitType)>(0x004631A4);
    return func(this, section, entry, value);
#else
    return Put_String(section, entry, UnitTypeClass::Name_From(value));
#endif
}

const InfantryType GameINIClass::Get_InfantryType(const char *section, const char *entry, const InfantryType defvalue) const
{
#ifdef GAME_DLL
    // Inlined in RA
    return INFANTRY_NONE;
#else
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, InfantryTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return InfantryTypeClass::From_Name(valuebuf);
    }

    return defvalue;
#endif
}

BOOL GameINIClass::Put_InfantryType(const char *section, const char *entry, const InfantryType value)
{
#ifdef GAME_DLL
    // Inlined in RA
    return false;
#else
    // TODO: Requires InfantryTypeClass to be complete
    return Put_String(section, entry, InfantryTypeClass::Name_From(value));
#endif
}

const AircraftType GameINIClass::Get_AircraftType(const char *section, const char *entry, const AircraftType defvalue) const
{
#ifdef GAME_DLL
    // Inlined in RA
    return AIRCRAFT_NONE;
#else
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, AircraftTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return AircraftTypeClass::From_Name(valuebuf);
    }

    return defvalue;
#endif
}

BOOL GameINIClass::Put_AircraftType(const char *section, const char *entry, const AircraftType value)
{
#ifdef GAME_DLL
    // Inlined in RA
    return false;
#else
    // TODO: Requires AircraftTypeClass to be complete
    return Put_String(section, entry, AircraftTypeClass::Name_From(value));
#endif
}

const VesselType GameINIClass::Get_VesselType(const char *section, const char *entry, const VesselType defvalue) const
{
#ifdef GAME_DLL
    // Inlined in RA
    return VESSEL_NONE;
#else
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, VesselTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return VesselTypeClass::From_Name(valuebuf);
    }

    return defvalue;
#endif
}

BOOL GameINIClass::Put_VesselType(const char *section, const char *entry, const VesselType value)
{
#ifdef GAME_DLL
    // Inlined in RA
    return false;
#else
    // TODO: Requires VesselTypeClass to be complete
    return Put_String(section, entry, VesselTypeClass::Name_From(value));
#endif
}

const BuildingType GameINIClass::Get_BuildingType(const char *section, const char *entry, const BuildingType defvalue) const
{
#ifdef GAME_DLL
    // Inlined in RA
    return BUILDING_NONE;
#else
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, BuildingTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return BuildingTypeClass::From_Name(valuebuf);
    }

    return defvalue;
#endif
}

BOOL GameINIClass::Put_BuildingType(const char *section, const char *entry, const BuildingType value)
{
#ifdef GAME_DLL
    // Inlined in RA
    return false;
#else
    // TODO: Requires BuildingTypeClass to be complete
    return Put_String(section, entry, BuildingTypeClass::Name_From(value));
#endif
}

const WeaponType GameINIClass::Get_WeaponType(const char *section, const char *entry, const WeaponType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, WeaponTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return WeaponTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_WeaponType(const char *section, const char *entry, const WeaponType value)
{
    return Put_String(section, entry, WeaponTypeClass::Name_From(value));
}

const WarheadType GameINIClass::Get_WarheadType(const char *section, const char *entry, const WarheadType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, WarheadTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return WarheadTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_WarheadType(const char *section, const char *entry, const WarheadType value)
{
    return Put_String(section, entry, WarheadTypeClass::Name_From(value));
}

const OverlayType GameINIClass::Get_OverlayType(const char *section, const char *entry, const OverlayType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, OverlayTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return OverlayTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_OverlayType(const char *section, const char *entry, const OverlayType value)
{
    return Put_String(section, entry, OverlayTypeClass::Name_From(value));
}

const SmudgeType GameINIClass::Get_SmudgeType(const char *section, const char *entry, const SmudgeType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, SmudgeTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return SmudgeTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_SmudgeType(const char *section, const char *entry, const SmudgeType value)
{
    return Put_String(section, entry, SmudgeTypeClass::Name_From(value));
}

const BulletType GameINIClass::Get_BulletType(const char *section, const char *entry, const BulletType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, BulletTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return BulletTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_BulletType(const char *section, const char *entry, const BulletType value)
{
    return Put_String(section, entry, BulletTypeClass::Name_From(value));
}

const LandType GameINIClass::Get_LandType(const char *section, const char *entry, const LandType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Land(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Land_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_LandType(const char *section, const char *entry, const LandType value)
{
    return Put_String(section, entry, Name_From_Land(value));
}

const MZoneType GameINIClass::Get_MZoneType(const char *section, const char *entry, const MZoneType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_MZone(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return MZone_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_MZoneType(const char *section, const char *entry, const MZoneType value)
{
    return Put_String(section, entry, Name_From_MZone(value));
}

const GroundType GameINIClass::Get_GroundType(const char *section, const char *entry, const GroundType defvalue) const
{
    return GroundType();
}

BOOL GameINIClass::Put_GroundType(const char *section, const char *entry, const GroundType value)
{
    return 0;
}

const TerrainType GameINIClass::Get_TerrainType(const char *section, const char *entry, const TerrainType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, TerrainTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return TerrainTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_TerrainType(const char *section, const char *entry, const TerrainType value)
{
    return Put_String(section, entry, TerrainTypeClass::Name_From(value));
}

const RTTIType GameINIClass::Get_RTTIType(const char *section, const char *entry, const RTTIType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_RTTI(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return RTTI_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_RTTIType(const char *section, const char *entry, const RTTIType value)
{
    return Put_String(section, entry, Name_From_RTTI(value));
}

const ActionType GameINIClass::Get_ActionType(const char *section, const char *entry, const ActionType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Action(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Action_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_ActionType(const char *section, const char *entry, const ActionType value)
{
    return Put_String(section, entry, Name_From_Action(value));
}

const int GameINIClass::Get_Owners(const char *section, const char *entry, const int defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0) {
        int owners = 0;
        for (char *token = strtok(valuebuf, ","); token != nullptr; token = strtok(nullptr, ",")) {
            owners |= HouseTypeClass::Owner_From_Name(token);
        }
        return owners;
    }
    return defvalue;
}

BOOL GameINIClass::Put_Owners(const char *section, const char *entry, const int value)
{
    char buffer[MAX_LINE_LENGTH];

    int owner = value;

    if ((owner & SIDE_SOVIET) == SIDE_SOVIET) {
        strcat(buffer, "soviet");
        owner &= ~(SIDE_SOVIET);
    }

    if ((owner & SIDE_ALLIES) == SIDE_ALLIES) {
        if (buffer[0] != '\0') {
            strcat(&buffer[strlen(buffer)], ",");
        }
        strcat(buffer, "allies");
        owner &= ~(SIDE_ALLIES);
    }

    for (int type = 0; type < HOUSES_COUNT; ++type) {
        if ((1 << type) & owner) {
            if (buffer[0] != '\0') {
                strcat(&buffer[strlen(buffer)], ",");
            }
            strcat(&buffer[strlen(buffer)], HouseTypeClass::As_Reference((HousesType)type).Get_Name());
        }
    }

    return buffer[0] != '\0' ? Put_String(section, entry, buffer) : true;
}

const int GameINIClass::Get_Units(const char *section, const char *entry, const int defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0) {
        int value = 0;
        for (char *token = strtok(valuebuf, ","); token != nullptr; token = strtok(nullptr, ",")) {
            // TODO: Requires UnitTypeClass to be complete
            UnitType type = UNIT_NONE; // UnitTypeClass::From_Name(token);
            if (type != UNIT_NONE && type < UNIT_COUNT) {
                value |= (1 << type);
            }
        }
        return value;
    }
    return defvalue;
}

BOOL GameINIClass::Put_Units(const char *section, const char *entry, const int value)
{
    char buffer[MAX_LINE_LENGTH];

    for (UnitType type = UNIT_FIRST; type < UNIT_COUNT; ++type) {
        if ((1 << type) & value) {
            if (buffer[0] != '\0') {
                strcat(&buffer[strlen(buffer)], ",");
            }
            // TODO: Requires UnitTypeClass to be complete
            strcat(&buffer[strlen(buffer)], UnitTypeClass::Name_From(type));
        }
    }
    return Put_String(section, entry, buffer);
}

const int GameINIClass::Get_Infantry(const char *section, const char *entry, const int defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0) {
        int value = 0;
        for (char *token = strtok(valuebuf, ","); token != nullptr; token = strtok(nullptr, ",")) {
            // TODO: Requires InfantryTypeClass to be complete
            InfantryType type = InfantryTypeClass::From_Name(token);
            if (type != INFANTRY_NONE && type < INFANTRY_COUNT) {
                value |= (1 << type);
            }
        }
        return value;
    }
    return defvalue;
}

BOOL GameINIClass::Put_Infantry(const char *section, const char *entry, const int value)
{
    char buffer[MAX_LINE_LENGTH];

    for (InfantryType type = INFANTRY_FIRST; type < INFANTRY_COUNT; ++type) {
        if ((1 << type) & value) {
            if (buffer[0] != '\0') {
                strcat(&buffer[strlen(buffer)], ",");
            }

            strcat(&buffer[strlen(buffer)], InfantryTypeClass::Name_From(type));
        }
    }
    return Put_String(section, entry, buffer);
}

const int GameINIClass::Get_Aircrafts(const char *section, const char *entry, const int defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0) {
        int value = 0;
        for (char *token = strtok(valuebuf, ","); token != nullptr; token = strtok(nullptr, ",")) {
            AircraftType type = AircraftTypeClass::From_Name(token);
            if (type != AIRCRAFT_NONE && type < AIRCRAFT_COUNT) {
                value |= (1 << type);
            }
        }
        return value;
    }
    return defvalue;
}

BOOL GameINIClass::Put_Aircrafts(const char *section, const char *entry, const int value)
{
    char buffer[MAX_LINE_LENGTH];

    for (AircraftType type = AIRCRAFT_FIRST; type < AIRCRAFT_COUNT; ++type) {
        if ((1 << type) & value) {
            if (buffer[0] != '\0') {
                strcat(&buffer[strlen(buffer)], ",");
            }

            strcat(&buffer[strlen(buffer)], AircraftTypeClass::Name_From(type));
        }
    }
    return Put_String(section, entry, buffer);
}

const int GameINIClass::Get_Vessels(const char *section, const char *entry, const int defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0) {
        int value = 0;
        for (char *token = strtok(valuebuf, ","); token != nullptr; token = strtok(nullptr, ",")) {
            VesselType type = VesselTypeClass::From_Name(token);
            if (type != VESSEL_NONE && type < VESSEL_COUNT) {
                value |= (1 << type);
            }
        }
        return value;
    }
    return defvalue;
}

BOOL GameINIClass::Put_Vessels(const char *section, const char *entry, const int value)
{
    char buffer[MAX_LINE_LENGTH];

    for (VesselType type = VESSEL_FIRST; type < VESSEL_COUNT; ++type) {
        if ((1 << type) & value) {
            if (buffer[0] != '\0') {
                strcat(&buffer[strlen(buffer)], ",");
            }

            strcat(&buffer[strlen(buffer)], VesselTypeClass::Name_From(type));
        }
    }
    return Put_String(section, entry, buffer);
}

const int GameINIClass::Get_Buildings(const char *section, const char *entry, const int defvalue) const
{
#ifdef GAME_DLL
    int (*func)(GameINIClass *, const char *, const char *, const int) =
        reinterpret_cast<int (*)(GameINIClass *, const char *, const char *, const int)>(0x00463A88);
    return func((GameINIClass *)this, section, entry, defvalue);
#else
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0) {
        int value = 0;
        for (char *token = strtok(valuebuf, ","); token != nullptr; token = strtok(nullptr, ",")) {
            BuildingType type = BuildingTypeClass::From_Name(token);
            if (type != BUILDING_NONE && type < BUILDING_COUNT) {
                value |= (1 << type);
            }
        }
        return value;
    }
    return defvalue;
#endif
}

BOOL GameINIClass::Put_Buildings(const char *section, const char *entry, const int value)
{
#ifdef GAME_DLL
    BOOL (*func)
    (GameINIClass *, const char *, const char *, const int) =
        reinterpret_cast<BOOL (*)(GameINIClass *, const char *, const char *, const int)>(0x00463AFC);
    return func(this, section, entry, value);
#else
    char buffer[MAX_LINE_LENGTH];

    for (BuildingType type = BUILDING_FIRST; type < BUILDING_VALID_COUNT; ++type) {
        if ((1 << type) & value) {
            if (buffer[0] != '\0') {
                strcat(&buffer[strlen(buffer)], ",");
            }

            strcat(&buffer[strlen(buffer)], BuildingTypeClass::Name_From(type));
        }
    }
    return Put_String(section, entry, buffer);
#endif
}

const KeyNumType GameINIClass::Get_KeyNumType(const char *section, const char *entry, const KeyNumType defvalue) const
{
    return (KeyNumType)(Get_Int(section, entry, defvalue) & (~KEY_VK_BIT));
}

BOOL GameINIClass::Put_KeyNumType(const char *section, const char *entry, const KeyNumType value)
{
    return (KeyNumType)(Put_Int(section, entry, value));
}

const PKey GameINIClass::Get_PKey(BOOL fast) const
{
    PKey key;
    uint8_t buffer[512];

    if (fast) {
        captainslog_debug("GameINIClass::Get_PKey() - Preparing PublicKey...");
        Int<MAX_UNIT_PRECISION> exp(0x10001);
        MPMath::DER_Encode(exp, buffer, MAX_UNIT_PRECISION);
    } else {
        captainslog_debug("GameINIClass::Get_PKey() - Loading PrivateKey");
        Get_UUBlock("PrivateKey", buffer, sizeof(buffer));
    }

    captainslog_debug("GameINIClass::Get_PKey() - Decoding Exponent");
    key.Decode_Exponent(buffer);

    captainslog_debug("GameINIClass::Get_PKey() - Loading PublicKey");

    Get_UUBlock("PublicKey", buffer, sizeof(buffer));

    captainslog_debug("GameINIClass::Get_PKey() - Decoding Modulus");
    key.Decode_Modulus(buffer);

    return key;
}

BOOL GameINIClass::Put_PKey(PKey &key)
{
    char buffer[sizeof(PKey)];

    // Put public key
    Put_UUBlock("PublicKey", buffer, key.Encode_Modulus(buffer));

    // Put private key
    Put_UUBlock("PrivateKey", buffer, key.Encode_Exponent(buffer));

    return true;
}

void GameINIClass::Calculate_Message_Digest()
{
    if (!m_DigestValid) {
        SHAPipe pipe;
        INIClass::Save(pipe);
        pipe.Result(&s_Digest);
        m_DigestValid = true;
    }
}

void GameINIClass::Invalidate_Message_Digest()
{
    memset(&s_Digest, 0, sizeof(s_Digest));
    m_DigestValid = false;
}

int32_t const GameINIClass::Get_Unique_ID()
{
    if (!m_DigestValid) {
        Calculate_Message_Digest();
    }
    return Calculate_CRC(&s_Digest, sizeof(s_Digest));
}
