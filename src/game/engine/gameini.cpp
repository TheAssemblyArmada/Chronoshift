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
#include "filepipe.h"
#include "filestraw.h"
#include "shapipe.h"
#include <algorithm>

GameINIClass::GameINIClass() :
    m_DigestValid(false),
    m_Digest()
{
}

int GameINIClass::Save(FileClass &file, BOOL save_digest)
{
    FilePipe fpipe(file);
    return Save(fpipe, save_digest);
}

int GameINIClass::Save(Pipe &pipe, BOOL save_digest)
{
    if (save_digest) {
        Clear("Digest");
        Calculate_Message_Digest();
        Put_UUBlock("Digest", &m_Digest, sizeof(m_Digest));
        int retval = INIClass::Save(pipe);
        Clear("Digest");
        return retval;
    }

    return INIClass::Save(pipe);
}

int GameINIClass::Load(FileClass &file, BOOL load_digest)
{
    FileStraw fstraw(file);
    return Load(fstraw, load_digest);
}

int GameINIClass::Load(Straw &straw, BOOL load_digest)
{
    SHAEngine::SHADigest digest;

    int retval = INIClass::Load(straw);

    Invalidate_Message_Digest();

    if (retval > 0 && load_digest) {
        if (Get_UUBlock("Digest", &digest, sizeof(digest)) > 0) {
            Clear("Digest");
            Calculate_Message_Digest();
            if (memcmp(&digest, &m_Digest, sizeof(m_Digest))) {
                return 2;
            }
        }
    }

    return retval;
}

lepton_t GameINIClass::Get_Lepton(const char *section, const char *entry, lepton_t defvalue) const
{
    fixed_t lepton = Get_Fixed(section, entry, fixed_t(defvalue, CELL_LEPTONS));
    return lepton * CELL_LEPTONS;
}

BOOL GameINIClass::Put_Lepton(const char *section, const char *entry, lepton_t value)
{
    fixed_t lepton(value, CELL_LEPTONS);
    return Put_Fixed(section, entry, lepton);
}

MPHType GameINIClass::Get_MPHType(const char *section, const char *entry, MPHType defvalue) const
{
    int value = Get_Int(section, entry, (100 * defvalue) / 256);
    return std::clamp(MPHType(value), MPH_MIN, MPHType(100));
}

BOOL GameINIClass::Put_MPHType(const char *section, const char *entry, MPHType value)
{
    return Put_Int(section, entry, (100 * value / 256));
}

HousesType GameINIClass::Get_HousesType(const char *section, const char *entry, HousesType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, HouseTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return HouseTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_HousesType(const char *section, const char *entry, HousesType value)
{
    return Put_String(section, entry, HouseTypeClass::Name_From(value));
}

MovieType GameINIClass::Get_MovieType(const char *section, const char *entry, MovieType defvalue)
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Movie(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Movie_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_MovieType(const char *section, const char *entry, MovieType value)
{
    return Put_String(section, entry, Name_From_Movie(value));
}

TheaterType GameINIClass::Get_TheaterType(const char *section, const char *entry, TheaterType defvalue)
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Theater(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Theater_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_TheaterType(const char *section, const char *entry, TheaterType value)
{
    return Put_String(section, entry, Name_From_Theater(value));
}

TriggerTypeClass *GameINIClass::Get_TriggerType(const char *section, const char *entry) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, "<none>", valuebuf, sizeof(valuebuf)) > 0) {
        return TriggerTypeClass::From_Name(valuebuf);
    }

    return nullptr;
}

BOOL GameINIClass::Put_TriggerType(const char *section, const char *entry, TriggerTypeClass *trigger)
{
    return Put_String(section, entry, trigger != nullptr ? trigger->Get_Name() : "<none>");
}

ThemeType GameINIClass::Get_ThemeType(const char *section, const char *entry, ThemeType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, g_Theme.Base_Name(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return g_Theme.From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_ThemeType(const char *section, const char *entry, ThemeType value)
{
    return Put_String(section, entry, g_Theme.Base_Name(value));
}

SourceType GameINIClass::Get_SourceType(const char *section, const char *entry, SourceType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Source(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Source_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_SourceType(const char *section, const char *entry, SourceType value)
{
    return Put_String(section, entry, Name_From_Source(value));
}

CrateType GameINIClass::Get_CrateType(const char *section, const char *entry, CrateType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, CrateClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return CrateClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_CrateType(const char *section, const char *entry, CrateType value)
{
    return Put_String(section, entry, CrateClass::Name_From(value));
}

MissionType GameINIClass::Get_MissionType(const char *section, const char *entry, MissionType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, MissionClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return MissionClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_MissionType(const char *section, const char *entry, MissionType value)
{
    return Put_String(section, entry, MissionClass::Name_From(value));
}

ArmorType GameINIClass::Get_ArmorType(const char *section, const char *entry, ArmorType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Armor(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Armor_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_ArmorType(const char *section, const char *entry, ArmorType value)
{
    return Put_String(section, entry, Name_From_Armor(value));
}

VocType GameINIClass::Get_VocType(const char *section, const char *entry, VocType defvalue) const
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

VoxType GameINIClass::Get_VoxType(const char *section, const char *entry, VoxType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Vox(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Vox_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_VoxType(const char *section, const char *entry, VoxType value)
{
    return Put_String(section, entry, Name_From_Vox(value));
}

AnimType GameINIClass::Get_AnimType(const char *section, const char *entry, AnimType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, AnimTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return AnimTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_AnimType(const char *section, const char *entry, AnimType value)
{
    return Put_String(section, entry, AnimTypeClass::Name_From(value));
}

UnitType GameINIClass::Get_UnitType(const char *section, const char *entry, UnitType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, UnitTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return UnitTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_UnitType(const char *section, const char *entry, UnitType value)
{
    return Put_String(section, entry, UnitTypeClass::Name_From(value));
}

InfantryType GameINIClass::Get_InfantryType(const char *section, const char *entry, InfantryType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, InfantryTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return InfantryTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_InfantryType(const char *section, const char *entry, InfantryType value)
{
    return Put_String(section, entry, InfantryTypeClass::Name_From(value));
}

AircraftType GameINIClass::Get_AircraftType(const char *section, const char *entry, AircraftType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, AircraftTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return AircraftTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_AircraftType(const char *section, const char *entry, AircraftType value)
{
    return Put_String(section, entry, AircraftTypeClass::Name_From(value));
}

VesselType GameINIClass::Get_VesselType(const char *section, const char *entry, VesselType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, VesselTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return VesselTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_VesselType(const char *section, const char *entry, VesselType value)
{
    return Put_String(section, entry, VesselTypeClass::Name_From(value));
}

BuildingType GameINIClass::Get_BuildingType(const char *section, const char *entry, BuildingType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, BuildingTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return BuildingTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_BuildingType(const char *section, const char *entry, BuildingType value)
{
    return Put_String(section, entry, BuildingTypeClass::Name_From(value));
}

WeaponType GameINIClass::Get_WeaponType(const char *section, const char *entry, WeaponType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, WeaponTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return WeaponTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_WeaponType(const char *section, const char *entry, WeaponType value)
{
    return Put_String(section, entry, WeaponTypeClass::Name_From(value));
}

WarheadType GameINIClass::Get_WarheadType(const char *section, const char *entry, WarheadType defvalue) const
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

OverlayType GameINIClass::Get_OverlayType(const char *section, const char *entry, OverlayType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, OverlayTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return OverlayTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_OverlayType(const char *section, const char *entry, OverlayType value)
{
    return Put_String(section, entry, OverlayTypeClass::Name_From(value));
}

SmudgeType GameINIClass::Get_SmudgeType(const char *section, const char *entry, SmudgeType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, SmudgeTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return SmudgeTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_SmudgeType(const char *section, const char *entry, SmudgeType value)
{
    return Put_String(section, entry, SmudgeTypeClass::Name_From(value));
}

BulletType GameINIClass::Get_BulletType(const char *section, const char *entry, BulletType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, BulletTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return BulletTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_BulletType(const char *section, const char *entry, BulletType value)
{
    return Put_String(section, entry, BulletTypeClass::Name_From(value));
}

LandType GameINIClass::Get_LandType(const char *section, const char *entry, LandType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Land(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Land_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_LandType(const char *section, const char *entry, LandType value)
{
    return Put_String(section, entry, Name_From_Land(value));
}

MZoneType GameINIClass::Get_MZoneType(const char *section, const char *entry, MZoneType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_MZone(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return MZone_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_MZoneType(const char *section, const char *entry, MZoneType value)
{
    return Put_String(section, entry, Name_From_MZone(value));
}

GroundType GameINIClass::Get_GroundType(const char *section, const char *entry, GroundType defvalue) const
{
    return GroundType();
}

BOOL GameINIClass::Put_GroundType(const char *section, const char *entry, GroundType value)
{
    return 0;
}

TerrainType GameINIClass::Get_TerrainType(const char *section, const char *entry, TerrainType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, TerrainTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return TerrainTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_TerrainType(const char *section, const char *entry, TerrainType value)
{
    return Put_String(section, entry, TerrainTypeClass::Name_From(value));
}

RTTIType GameINIClass::Get_RTTIType(const char *section, const char *entry, RTTIType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_RTTI(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return RTTI_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_RTTIType(const char *section, const char *entry, RTTIType value)
{
    return Put_String(section, entry, Name_From_RTTI(value));
}

ActionType GameINIClass::Get_ActionType(const char *section, const char *entry, ActionType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Action(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Action_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL GameINIClass::Put_ActionType(const char *section, const char *entry, ActionType value)
{
    return Put_String(section, entry, Name_From_Action(value));
}

uint32_t GameINIClass::Get_Owners(const char *section, const char *entry, uint32_t defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0) {
        uint32_t owners = 0;
        for (const char *token = strtok(valuebuf, ","); token != nullptr; token = strtok(nullptr, ",")) {
            owners |= HouseTypeClass::Owner_From_Name(token);
        }
        return owners;
    }
    return defvalue;
}

BOOL GameINIClass::Put_Owners(const char *section, const char *entry, uint32_t value)
{
    char buffer[MAX_LINE_LENGTH];

    uint32_t owner = value;

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

    for (HousesType type = HOUSES_FIRST; type < HOUSES_COUNT; ++type) {
        if ((1 << type) & owner) {
            if (buffer[0] != '\0') {
                strcat(&buffer[strlen(buffer)], ",");
            }
            strcat(&buffer[strlen(buffer)], HouseTypeClass::As_Reference(type).Get_Name());
        }
    }

    return buffer[0] != '\0' ? Put_String(section, entry, buffer) : true;
}

uint32_t GameINIClass::Get_Units(const char *section, const char *entry, uint32_t defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0) {
        uint32_t value = 0;
        for (const char *token = strtok(valuebuf, ","); token != nullptr; token = strtok(nullptr, ",")) {
            UnitType type = UnitTypeClass::From_Name(token);
            if (type != UNIT_NONE && type < UNIT_COUNT) {
                value |= (1 << type);
            }
        }
        return value;
    }
    return defvalue;
}

BOOL GameINIClass::Put_Units(const char *section, const char *entry, uint32_t value)
{
    char buffer[MAX_LINE_LENGTH];

    for (UnitType type = UNIT_FIRST; type < UNIT_COUNT; ++type) {
        if ((1 << type) & value) {
            if (buffer[0] != '\0') {
                strcat(&buffer[strlen(buffer)], ",");
            }
            strcat(&buffer[strlen(buffer)], UnitTypeClass::Name_From(type));
        }
    }
    return Put_String(section, entry, buffer);
}

uint32_t GameINIClass::Get_Infantry(const char *section, const char *entry, uint32_t defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0) {
        uint32_t value = 0;
        for (const char *token = strtok(valuebuf, ","); token != nullptr; token = strtok(nullptr, ",")) {
            InfantryType type = InfantryTypeClass::From_Name(token);
            if (type != INFANTRY_NONE && type < INFANTRY_COUNT) {
                value |= (1 << type);
            }
        }
        return value;
    }
    return defvalue;
}

BOOL GameINIClass::Put_Infantry(const char *section, const char *entry, uint32_t value)
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

uint32_t GameINIClass::Get_Aircrafts(const char *section, const char *entry, uint32_t defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0) {
        uint32_t value = 0;
        for (const char *token = strtok(valuebuf, ","); token != nullptr; token = strtok(nullptr, ",")) {
            AircraftType type = AircraftTypeClass::From_Name(token);
            if (type != AIRCRAFT_NONE && type < AIRCRAFT_COUNT) {
                value |= (1 << type);
            }
        }
        return value;
    }
    return defvalue;
}

BOOL GameINIClass::Put_Aircrafts(const char *section, const char *entry, uint32_t value)
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

uint32_t GameINIClass::Get_Vessels(const char *section, const char *entry, uint32_t defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0) {
        uint32_t value = 0;
        for (const char *token = strtok(valuebuf, ","); token != nullptr; token = strtok(nullptr, ",")) {
            VesselType type = VesselTypeClass::From_Name(token);
            if (type != VESSEL_NONE && type < VESSEL_COUNT) {
                value |= (1 << type);
            }
        }
        return value;
    }
    return defvalue;
}

BOOL GameINIClass::Put_Vessels(const char *section, const char *entry, uint32_t value)
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

uint32_t GameINIClass::Get_Buildings(const char *section, const char *entry, uint32_t defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0) {
        uint32_t value = 0;
        for (const char *token = strtok(valuebuf, ","); token != nullptr; token = strtok(nullptr, ",")) {
            BuildingType type = BuildingTypeClass::From_Name(token);
            if (type != BUILDING_NONE && type < BUILDING_COUNT) {
                value |= (1 << type);
            }
        }
        return value;
    }
    return defvalue;
}

BOOL GameINIClass::Put_Buildings(const char *section, const char *entry, uint32_t value)
{
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
}

KeyNumType GameINIClass::Get_KeyNumType(const char *section, const char *entry, KeyNumType defvalue) const
{
    return KeyNumType(Get_Int(section, entry, defvalue) & (~KEY_VK_BIT));
}

BOOL GameINIClass::Put_KeyNumType(const char *section, const char *entry, KeyNumType value)
{
    return KeyNumType(Put_Int(section, entry, value));
}

const PKey GameINIClass::Get_PKey(BOOL fast) const
{
    PKey key;
    uint8_t buffer[sizeof(PKey)];

    if (fast) {
        captainslog_debug("GameINIClass::Get_PKey() - Preparing PublicKey...");
        Int<MAX_UNIT_PRECISION> exp(0x10001);
        MPMath::DER_Encode(exp, buffer, MAX_UNIT_PRECISION);
    } else {
        captainslog_debug("GameINIClass::Get_PKey() - Loading PrivateKey.");
        Get_UUBlock("PrivateKey", buffer, sizeof(buffer));
    }

    captainslog_debug("GameINIClass::Get_PKey() - Decoding Exponent.");
    key.Decode_Exponent(buffer);

    captainslog_debug("GameINIClass::Get_PKey() - Loading PublicKey.");

    Get_UUBlock("PublicKey", buffer, sizeof(buffer));

    captainslog_debug("GameINIClass::Get_PKey() - Decoding Modulus.");
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
        pipe.Result(&m_Digest);
        m_DigestValid = true;
    }
}

void GameINIClass::Invalidate_Message_Digest()
{
    memset(&m_Digest, 0, sizeof(m_Digest));
    m_DigestValid = false;
}

int32_t GameINIClass::Get_Unique_ID()
{
    if (!m_DigestValid) {
        Calculate_Message_Digest();
    }
    return Calculate_CRC(&m_Digest, sizeof(m_Digest));
}
