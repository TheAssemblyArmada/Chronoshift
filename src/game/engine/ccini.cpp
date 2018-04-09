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
#include "ccini.h"
#include "minmax.h"
#include "shapipe.h"
#include "crc.h"
 //#include "triggertype.h"

CCINIClass::CCINIClass() : DigestValid(false) {}

CCINIClass::~CCINIClass() {}

const lepton_t CCINIClass::Get_Lepton(const char *section, const char *entry, const lepton_t defvalue) const
{
    return Get_Fixed(section, entry, fixed(defvalue, 256)) * 256;
}

bool CCINIClass::Put_Lepton(const char *section, const char *entry, const lepton_t value)
{
    //TODO: OmniBlade, please implement.
    return false;
}

const MPHType CCINIClass::Get_MPHType(const char *section, const char *entry, const MPHType defvalue) const
{
    int value = Clamp(Get_Int(section, entry, (100 * defvalue) / 256), 0, 100);

    return (MPHType)Min((value * 256) / 100, 255);
}

bool CCINIClass::Put_MPHType(const char *section, const char *entry, const MPHType value)
{
    //TODO: OmniBlade, please implement.
    return false;
}

HousesType CCINIClass::Get_HousesType(const char *section, const char *entry, const HousesType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, HouseTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return HouseTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

bool CCINIClass::Put_HousesType(const char *section, const char *entry, const HousesType value)
{
    return Put_String(section, entry, HouseTypeClass::Name_From(value));
}

const MovieType CCINIClass::Get_MovieType(const char *section, const char *entry, const MovieType defvalue)
{
    char valuebuf[MAX_LINE_LENGTH];

    //TODO: Requires Movie interface to be complete
    /*if (Get_String(section, entry, Name_From_Movie(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Movie_From_Name(valuebuf);
    }*/

    return defvalue;
}

bool CCINIClass::Put_MovieType(const char *section, const char *entry, const MovieType value)
{
    //TODO: Requires Movie interface to be complete
    //return Put_String(section, entry, Name_From_Movie(value));
    return false;
}

const TheaterType CCINIClass::Get_TheaterType(const char *section, const char *entry, const TheaterType defvalue)
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Theater(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Theater_From_Name(valuebuf);
    }

    return defvalue;
}

bool CCINIClass::Put_TheaterType(const char *section, const char *entry, const TheaterType value)
{
    return Put_String(section, entry, Name_From_Theater(value));
}

const ThemeType CCINIClass::Get_ThemeType(const char *section, const char *entry, const ThemeType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Theme.Base_Name(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Theme.From_Name(valuebuf);
    }

    return defvalue;
}

bool CCINIClass::Put_ThemeType(const char *section, const char *entry, const ThemeType value)
{
    return Put_String(section, entry, Theme.Base_Name(value));
}

const CrateType CCINIClass::Get_CrateType(const char *section, const char *entry, const CrateType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, CrateClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return CrateClass::From_Name(valuebuf);
    }

    return defvalue;
}

bool CCINIClass::Put_CrateType(const char *section, const char *entry, const CrateType value)
{
    return Put_String(section, entry, CrateClass::Name_From(value));
}

const MissionType CCINIClass::Get_MissionType(const char *section, const char *entry, const MissionType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, MissionClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return MissionClass::From_Name(valuebuf);
    }

    return defvalue;
}

bool CCINIClass::Put_MissionType(const char *section, const char *entry, const MissionType value)
{
    return Put_String(section, entry, MissionClass::Name_From(value));
}

const ArmorType CCINIClass::Get_ArmorType(const char *section, const char *entry, const ArmorType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Armor(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Armor_From_Name(valuebuf);
    }

    return defvalue;
}

bool CCINIClass::Put_ArmorType(const char *section, const char *entry, const ArmorType value)
{
    return Put_String(section, entry, Name_From_Armor(value));
}

const VocType CCINIClass::Get_VocType(const char *section, const char *entry, const VocType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Voc_Name(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Voc_From_Name(valuebuf);
    }

    return defvalue;
}

bool CCINIClass::Put_VocType(const char *section, const char *entry, const VocType value)
{
    return Put_String(section, entry, Voc_Name(value));
}

const VoxType CCINIClass::Get_VoxType(const char *section, const char *entry, const VoxType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    //TODO: Requires Vox interface to be complete
    /*if (Get_String(section, entry, Vox_Name(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Vox_From_Name(valuebuf);
    }*/

    return defvalue;
}

bool CCINIClass::Put_VoxType(const char *section, const char *entry, const VoxType value)
{
    //TODO: Requires Vox interface to be complete
    //return Put_String(section, entry, Vox_Name(value));
    return false;
}

const AnimType CCINIClass::Get_AnimType(const char *section, const char *entry, const AnimType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, AnimTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return AnimTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

bool CCINIClass::Put_AnimType(const char *section, const char *entry, const AnimType value)
{
    return Put_String(section, entry, AnimTypeClass::Name_From(value));
}

const UnitType CCINIClass::Get_UnitType(const char *section, const char *entry, const UnitType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    //TODO: Requires UnitTypeClass to be complete
    /*if (Get_String(section, entry, UnitTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return UnitTypeClass::From_Name(valuebuf);
    }*/

    return defvalue;
}

bool CCINIClass::Put_UnitType(const char *section, const char *entry, const UnitType value)
{
    //TODO: Requires UnitTypeClass to be complete
    //return Put_String(section, entry, UnitTypeClass::Name_From(value));
    return false;
}

const WeaponType CCINIClass::Get_WeaponType(const char *section, const char *entry, const WeaponType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, WeaponTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return WeaponTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

bool CCINIClass::Put_WeaponType(const char *section, const char *entry, const WeaponType value)
{
    return Put_String(section, entry, WeaponTypeClass::Name_From(value));
}

const WarheadType CCINIClass::Get_WarheadType(const char *section, const char *entry, const WarheadType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, WarheadTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return WarheadTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

bool CCINIClass::Put_WarheadType(const char *section, const char *entry, const WarheadType value)
{
    return Put_String(section, entry, WarheadTypeClass::Name_From(value));
}

const OverlayType CCINIClass::Get_OverlayType(const char *section, const char *entry, const OverlayType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, OverlayTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return OverlayTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

bool CCINIClass::Put_OverlayType(const char *section, const char *entry, const OverlayType value)
{
    return Put_String(section, entry, OverlayTypeClass::Name_From(value));
}

const SmudgeType CCINIClass::Get_SmudgeType(const char *section, const char *entry, const SmudgeType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, SmudgeTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return SmudgeTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

bool CCINIClass::Put_SmudgeType(const char *section, const char *entry, const SmudgeType value)
{
    return Put_String(section, entry, SmudgeTypeClass::Name_From(value));
}

const BulletType CCINIClass::Get_BulletType(const char *section, const char *entry, const BulletType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, BulletTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return BulletTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

bool CCINIClass::Put_BulletType(const char *section, const char *entry, const BulletType value)
{
    return Put_String(section, entry, BulletTypeClass::Name_From(value));
}

const LandType CCINIClass::Get_LandType(const char *section, const char *entry, const LandType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Land(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Land_From_Name(valuebuf);
    }

    return defvalue;
}

bool CCINIClass::Put_LandType(const char *section, const char *entry, const LandType value)
{
    return Put_String(section, entry, Name_From_Land(value));
}

const MZoneType CCINIClass::Get_MZoneType(const char *section, const char *entry, const MZoneType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_MZone(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return MZone_From_Name(valuebuf);
    }

    return defvalue;
}

bool CCINIClass::Put_MZoneType(const char *section, const char *entry, const MZoneType value)
{
    return Put_String(section, entry, Name_From_MZone(value));
}

const TerrainType CCINIClass::Get_TerrainType(const char *section, const char *entry, const TerrainType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, TerrainTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return TerrainTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

bool CCINIClass::Put_TerrainType(const char *section, const char *entry, const TerrainType value)
{
    return Put_String(section, entry, TerrainTypeClass::Name_From(value));
}

const RTTIType CCINIClass::Get_RTTIType(const char *section, const char *entry, const RTTIType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_RTTI(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return RTTI_From_Name(valuebuf);
    }

    return defvalue;
}

bool CCINIClass::Put_RTTIType(const char *section, const char *entry, const RTTIType value)
{
    return Put_String(section, entry, Name_From_RTTI(value));
}

const ActionType CCINIClass::Get_ActionType(const char *section, const char *entry, const ActionType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Action(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Action_From_Name(valuebuf);
    }

    return defvalue;
}

bool CCINIClass::Put_ActionType(const char *section, const char *entry, const ActionType value)
{
    return Put_String(section, entry, Name_From_Action(value));
}

const int CCINIClass::Get_Owners(const char *section, const char *entry, int const defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    int owners = defvalue;

    if ( Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0 ) {
        for ( char *t = strtok(valuebuf, ","); t != nullptr; t = strtok(nullptr, ",") ) {
            owners |= HouseTypeClass::Owner_From_Name(valuebuf);
        }
    }
    return owners;
}

bool CCINIClass::Put_Owners(const char *section, const char *entry, const int value)
{
    //TODO: OmniBlade, please implement.
    return false;
}

const int CCINIClass::Get_Units(const char *section, const char *entry, int const defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    int value = defvalue;

    if ( Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0 ) {
        for ( char *t = strtok(valuebuf, ","); t != nullptr; t = strtok(nullptr, ",") ) {
            //TODO: Requires UnitTypeClass to be complete
            //value |= UnitTypeClass::From_Name(valuebuf);
        }
    }
    return value;
}

bool CCINIClass::Put_Units(const char *section, const char *entry, const int value)
{
    //TODO: OmniBlade, please implement (to be based on Put_Buildings).
    return false;
}

const int CCINIClass::Get_Infantry(const char *section, const char *entry, int const defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    int value = defvalue;

    if ( Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0 ) {
        for ( char *t = strtok(valuebuf, ","); t != nullptr; t = strtok(nullptr, ",") ) {
            //TODO: Requires InfantryTypeClass to be complete
            //value |= InfantryTypeClass::From_Name(valuebuf);
        }
    }
    return value;
}

bool CCINIClass::Put_Infantry(const char *section, const char *entry, const int value)
{
    //TODO: OmniBlade, please implement (to be based on Put_Buildings).
    return false;
}

const int CCINIClass::Get_Aircrafts(const char *section, const char *entry, int const defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    int value = defvalue;

    if ( Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0 ) {
        for ( char *t = strtok(valuebuf, ","); t != nullptr; t = strtok(nullptr, ",") ) {
            //TODO: Requires AircraftTypeClass to be complete
            //value |= AircraftTypeClass::From_Name(valuebuf);
        }
    }
    return value;
}

bool CCINIClass::Put_Aircrafts(const char *section, const char *entry, const int value)
{
    //TODO: OmniBlade, please implement (to be based on Put_Buildings).
    return false;
}

const int CCINIClass::Get_Vessels(char const *section, char const *entry, int const defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    int value = defvalue;

    if ( Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0 ) {
        for ( char *t = strtok(valuebuf, ","); t != nullptr; t = strtok(nullptr, ",") ) {
            //TODO: Requires VesselTypeClass to be complete
            //value |= VesselTypeClass::From_Name(valuebuf);
        }
    }
    return value;
}

bool CCINIClass::Put_Vessels(char const *section, char const *entry, const int value)
{
    //TODO: OmniBlade, please implement (to be based on Put_Buildings).
    return false;
}

const int CCINIClass::Get_Buildings(char const *section, char const *entry, int const defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    int value = defvalue;

    if ( Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0 ) {
        for ( char *t = strtok(valuebuf, ","); t != nullptr; t = strtok(nullptr, ",") ) {
            //TODO: Requires BuildingTypeClass to be complete
            //value |= BuildingTypeClass::From_Name(valuebuf);
        }
    }
    return value;
}

bool CCINIClass::Put_Buildings(char const *section, char const *entry, const int value)
{
    //TODO: OmniBlade, please implement.
    return false;
}

void CCINIClass::Calculate_Message_Digest(void)
{
    if ( !DigestValid ) {
        SHAPipe pipe;
        INIClass::Save(pipe);
        pipe.Result(&Digest);
        DigestValid = true;
    }
}

void CCINIClass::Invalidate_Message_Digest(void)
{
    memset(&Digest, 0, sizeof(Digest));
    DigestValid = false;
}

int32_t const CCINIClass::Get_Unique_ID(void)
{
    if ( !DigestValid ) {
        SHAPipe pipe;
        INIClass::Save(pipe);
        pipe.Result(&Digest);
        DigestValid = true;
    }
    return Calculate_CRC(&Digest, sizeof(Digest));
}