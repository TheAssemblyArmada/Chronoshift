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
#include "ccini.h"
#include "minmax.h"
#include "shapipe.h"
#include "crc.h"

CCINIClass::CCINIClass() : DigestValid(false) {}

CCINIClass::~CCINIClass() {}

const lepton_t CCINIClass::Get_Lepton(const char *section, const char *entry, const lepton_t defvalue) const
{
    return Get_Fixed(section, entry, fixed(defvalue, 256)) * 256;
}

BOOL CCINIClass::Put_Lepton(const char *section, const char *entry, const lepton_t value)
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(CCINIClass *, const char *, const char *, const lepton_t) =
        reinterpret_cast<BOOL (*)(CCINIClass *, const char *, const char *, const lepton_t)>(0x004630FC);
    return func(this, section, entry, value);
#else
    //TODO: OmniBlade, please implement.
    return false;
#endif
}

const MPHType CCINIClass::Get_MPHType(const char *section, const char *entry, const MPHType defvalue) const
{
    int value = Clamp(Get_Int(section, entry, (100 * defvalue) / 256), 0, 100);

    return (MPHType)Min((value * 256) / 100, 255);
}

BOOL CCINIClass::Put_MPHType(const char *section, const char *entry, const MPHType value)
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(CCINIClass *, const char *, const char *, const MPHType) =
        reinterpret_cast<BOOL (*)(CCINIClass *, const char *, const char *, const MPHType)>(0x004631A4);
    return func(this, section, entry, value);
#else
    //TODO: OmniBlade, please implement.
    return false;
#endif
}

HousesType CCINIClass::Get_HousesType(const char *section, const char *entry, const HousesType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, HouseTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return HouseTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL CCINIClass::Put_HousesType(const char *section, const char *entry, const HousesType value)
{
    return Put_String(section, entry, HouseTypeClass::Name_From(value));
}

const MovieType CCINIClass::Get_MovieType(const char *section, const char *entry, const MovieType defvalue)
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Movie(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Movie_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL CCINIClass::Put_MovieType(const char *section, const char *entry, const MovieType value)
{
    return Put_String(section, entry, Name_From_Movie(value));
}

const TheaterType CCINIClass::Get_TheaterType(const char *section, const char *entry, const TheaterType defvalue)
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Theater(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Theater_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL CCINIClass::Put_TheaterType(const char *section, const char *entry, const TheaterType value)
{
    return Put_String(section, entry, Name_From_Theater(value));
}

const TriggerTypeClass *CCINIClass::Get_TriggerType(const char *section, const char *entry) const
{
#ifndef CHRONOSHIFT_STANDALONE
    TriggerTypeClass *(*func)(CCINIClass *, const char *, const char *) =
        reinterpret_cast<TriggerTypeClass *(*)(CCINIClass *, const char *, const char *)>(0x004638BC);
    return func((CCINIClass *)this, section, entry);
#else
    //TODO: Requires TriggerTypeClass interface to be complete
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, "<none>", valuebuf, sizeof(valuebuf)) > 0) {
        return TriggerTypeClass::From_Name(valuebuf);
    }

    return defvalue;
#endif
}

BOOL CCINIClass::Put_TriggerType(const char *section, const char *entry, const TriggerTypeClass *trigger)
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL(*func)(CCINIClass *, const char *, const char *, const TriggerTypeClass *) =
        reinterpret_cast<BOOL(*)(CCINIClass *, const char *, const char *, const TriggerTypeClass *)>(0x004638F4);
    return func(this, section, entry, trigger);
#else
    return Put_String(section, entry, trigger != nullptr ? trigger->Get_Name() : "<none>");
#endif
}

const ThemeType CCINIClass::Get_ThemeType(const char *section, const char *entry, const ThemeType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Theme.Base_Name(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Theme.From_Name(valuebuf);
    }

    return defvalue;
}

BOOL CCINIClass::Put_ThemeType(const char *section, const char *entry, const ThemeType value)
{
    return Put_String(section, entry, Theme.Base_Name(value));
}

const SourceType CCINIClass::Get_SourceType(const char *section, const char *entry, const SourceType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Source(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Source_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL CCINIClass::Put_SourceType(const char *section, const char *entry, const SourceType value)
{
    return Put_String(section, entry, Name_From_Source(value));
}

const CrateType CCINIClass::Get_CrateType(const char *section, const char *entry, const CrateType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, CrateClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return CrateClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL CCINIClass::Put_CrateType(const char *section, const char *entry, const CrateType value)
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

BOOL CCINIClass::Put_MissionType(const char *section, const char *entry, const MissionType value)
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

BOOL CCINIClass::Put_ArmorType(const char *section, const char *entry, const ArmorType value)
{
    return Put_String(section, entry, Name_From_Armor(value));
}

const VocType CCINIClass::Get_VocType(const char *section, const char *entry, const VocType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, Name_From_Voc(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Voc_From_Name(valuebuf);
    }

    return defvalue;
}

BOOL CCINIClass::Put_VocType(const char *section, const char *entry, const VocType value)
{
    return Put_String(section, entry, Name_From_Voc(value));
}

const VoxType CCINIClass::Get_VoxType(const char *section, const char *entry, const VoxType defvalue) const
{
#ifndef CHRONOSHIFT_STANDALONE
    // Inlined in RA
    return VOX_NONE;
#else
    char valuebuf[MAX_LINE_LENGTH];

    //TODO: Requires Vox interface to be complete
    if (Get_String(section, entry, Name_From_Vox(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return Vox_From_Name(valuebuf);
    }

    return defvalue;
#endif
}

BOOL CCINIClass::Put_VoxType(const char *section, const char *entry, const VoxType value)
{
#ifndef CHRONOSHIFT_STANDALONE
    // Inlined in RA
    return false;
#else
    //TODO: Requires Vox interface to be complete
    return Put_String(section, entry, Name_From_Vox(value));
#endif
}

const AnimType CCINIClass::Get_AnimType(const char *section, const char *entry, const AnimType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, AnimTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return AnimTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL CCINIClass::Put_AnimType(const char *section, const char *entry, const AnimType value)
{
    return Put_String(section, entry, AnimTypeClass::Name_From(value));
}

const UnitType CCINIClass::Get_UnitType(const char *section, const char *entry, const UnitType defvalue) const
{
#ifndef CHRONOSHIFT_STANDALONE
    UnitType (*func)(CCINIClass *, const char *, const char *, const UnitType) =
        reinterpret_cast<UnitType (*)(CCINIClass *, const char *, const char *, const UnitType)>(0x004631A4);
    return func((CCINIClass *)this, section, entry, defvalue);
#else
    char valuebuf[MAX_LINE_LENGTH];

    //TODO: Requires UnitTypeClass to be complete
    if (Get_String(section, entry, UnitTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return UnitTypeClass::From_Name(valuebuf);
    }

    return defvalue;
#endif
}

BOOL CCINIClass::Put_UnitType(const char *section, const char *entry, const UnitType value)
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(CCINIClass *, const char *, const char *, const UnitType) =
        reinterpret_cast<BOOL (*)(CCINIClass *, const char *, const char *, const UnitType)>(0x004631A4);
    return func(this, section, entry, value);
#else
    //TODO: Requires UnitTypeClass to be complete
    return Put_String(section, entry, UnitTypeClass::Name_From(value));
#endif
}

const InfantryType CCINIClass::Get_InfantryType(const char *section, const char *entry, const InfantryType defvalue) const
{
#ifndef CHRONOSHIFT_STANDALONE
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

BOOL CCINIClass::Put_InfantryType(const char *section, const char *entry, const InfantryType value)
{
#ifndef CHRONOSHIFT_STANDALONE
    // Inlined in RA
    return false;
#else
    //TODO: Requires InfantryTypeClass to be complete
    return Put_String(section, entry, InfantryTypeClass::Name_From(value));
#endif
}

const AircraftType CCINIClass::Get_AircraftType(const char *section, const char *entry, const AircraftType defvalue) const
{
#ifndef CHRONOSHIFT_STANDALONE
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

BOOL CCINIClass::Put_AircraftType(const char *section, const char *entry, const AircraftType value)
{
#ifndef CHRONOSHIFT_STANDALONE
    // Inlined in RA
    return false;
#else
    //TODO: Requires AircraftTypeClass to be complete
    return Put_String(section, entry, AircraftTypeClass::Name_From(value));
#endif
}

const VesselType CCINIClass::Get_VesselType(const char *section, const char *entry, const VesselType defvalue) const
{
#ifndef CHRONOSHIFT_STANDALONE
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

BOOL CCINIClass::Put_VesselType(const char *section, const char *entry, const VesselType value)
{
#ifndef CHRONOSHIFT_STANDALONE
    // Inlined in RA
    return false;
#else
    //TODO: Requires VesselTypeClass to be complete
    return Put_String(section, entry, VesselTypeClass::Name_From(value));
#endif
}

const BuildingType CCINIClass::Get_BuildingType(const char *section, const char *entry, const BuildingType defvalue) const
{
#ifndef CHRONOSHIFT_STANDALONE
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

BOOL CCINIClass::Put_BuildingType(const char *section, const char *entry, const BuildingType value)
{
#ifndef CHRONOSHIFT_STANDALONE
    // Inlined in RA
    return false;
#else
    //TODO: Requires BuildingTypeClass to be complete
    return Put_String(section, entry, BuildingTypeClass::Name_From(value));
#endif
}

const WeaponType CCINIClass::Get_WeaponType(const char *section, const char *entry, const WeaponType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, WeaponTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return WeaponTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL CCINIClass::Put_WeaponType(const char *section, const char *entry, const WeaponType value)
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

BOOL CCINIClass::Put_WarheadType(const char *section, const char *entry, const WarheadType value)
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

BOOL CCINIClass::Put_OverlayType(const char *section, const char *entry, const OverlayType value)
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

BOOL CCINIClass::Put_SmudgeType(const char *section, const char *entry, const SmudgeType value)
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

BOOL CCINIClass::Put_BulletType(const char *section, const char *entry, const BulletType value)
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

BOOL CCINIClass::Put_LandType(const char *section, const char *entry, const LandType value)
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

BOOL CCINIClass::Put_MZoneType(const char *section, const char *entry, const MZoneType value)
{
    return Put_String(section, entry, Name_From_MZone(value));
}

const GroundType CCINIClass::Get_GroundType(const char *section, const char *entry, const GroundType defvalue) const
{
    return GroundType();
}

BOOL CCINIClass::Put_GroundType(const char *section, const char *entry, const GroundType value)
{
    return 0;
}

const TerrainType CCINIClass::Get_TerrainType(const char *section, const char *entry, const TerrainType defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if (Get_String(section, entry, TerrainTypeClass::Name_From(defvalue), valuebuf, sizeof(valuebuf)) > 0) {
        return TerrainTypeClass::From_Name(valuebuf);
    }

    return defvalue;
}

BOOL CCINIClass::Put_TerrainType(const char *section, const char *entry, const TerrainType value)
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

BOOL CCINIClass::Put_RTTIType(const char *section, const char *entry, const RTTIType value)
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

BOOL CCINIClass::Put_ActionType(const char *section, const char *entry, const ActionType value)
{
    return Put_String(section, entry, Name_From_Action(value));
}

const int CCINIClass::Get_Owners(const char *section, const char *entry, const int defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if ( Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0 ) {
        int owners = 0;
        for ( char *token = strtok(valuebuf, ","); token != nullptr; token = strtok(nullptr, ",") ) {
            owners |= HouseTypeClass::Owner_From_Name(token);
        }
        return owners;
    }
    return defvalue;
}

BOOL CCINIClass::Put_Owners(const char *section, const char *entry, const int value)
{
    char buffer[MAX_LINE_LENGTH];

    int owner = value;

    if ( (owner & SIDE_SOVIET) == SIDE_SOVIET ) {
        strcat(buffer, "soviet");
        owner &= ~(SIDE_SOVIET);
    }

    if ( (owner & SIDE_ALLIES) == SIDE_ALLIES ) {
        if ( buffer[0] != '\0' ) {
            strcat(&buffer[strlen(buffer)], ",");
        }
        strcat(buffer, "allies");
        owner &= ~(SIDE_ALLIES);
    }

    for ( int type = 0; type < HOUSES_COUNT; ++type ) {
        if ( (1 << type) & owner ) {
            if ( buffer[0] != '\0' ) {
                strcat(&buffer[strlen(buffer)], ",");
            }
            strcat(&buffer[strlen(buffer)], HouseTypeClass::As_Reference((HousesType)type).Get_Name());
        }
    }

    return buffer[0] != '\0' ? Put_String(section, entry, buffer) : true;
}

const int CCINIClass::Get_Units(const char *section, const char *entry, const int defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if ( Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0 ) {
        int value = 0;
        for ( char *token = strtok(valuebuf, ","); token != nullptr; token = strtok(nullptr, ",") ) {
            //TODO: Requires UnitTypeClass to be complete
            UnitType type = UNIT_NONE; //UnitTypeClass::From_Name(token);
            if (type != UNIT_NONE && type < UNIT_COUNT) {
                value |= (1 << type);
            }
        }
        return value;
    }
    return defvalue;
}

BOOL CCINIClass::Put_Units(const char *section, const char *entry, const int value)
{
    char buffer[MAX_LINE_LENGTH];

    for (UnitType type = UNIT_FIRST; type < UNIT_COUNT; ++type ) {
        if ( (1 << type) & value ) {
            if ( buffer[0] != '\0' ) {
                strcat(&buffer[strlen(buffer)], ",");
            }
            //TODO: Requires UnitTypeClass to be complete
            strcat(&buffer[strlen(buffer)], nullptr /*UnitTypeClass::As_Reference(type).Get_Name()*/);
        }
    }
    return Put_String(section, entry, buffer);
}

const int CCINIClass::Get_Infantry(const char *section, const char *entry, const int defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if ( Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0 ) {
        int value = 0;
        for ( char *token = strtok(valuebuf, ","); token != nullptr; token = strtok(nullptr, ",") ) {
            //TODO: Requires InfantryTypeClass to be complete
            InfantryType type = INFANTRY_NONE; //InfantryTypeClass::From_Name(token);
            if (type != INFANTRY_NONE && type < INFANTRY_COUNT) {
                value |= (1 << type);
            }
        }
        return value;
    }
    return defvalue;
}

BOOL CCINIClass::Put_Infantry(const char *section, const char *entry, const int value)
{
    char buffer[MAX_LINE_LENGTH];

    for (InfantryType type = INFANTRY_FIRST; type < INFANTRY_COUNT; ++type ) {
        if ( (1 << type) & value ) {
            if ( buffer[0] != '\0' ) {
                strcat(&buffer[strlen(buffer)], ",");
            }
            //TODO: Requires InfantryTypeClass to be complete
            strcat(&buffer[strlen(buffer)], nullptr /*InfantryTypeClass::As_Reference(type).Get_Name()*/);
        }
    }
    return Put_String(section, entry, buffer);
}

const int CCINIClass::Get_Aircrafts(const char *section, const char *entry, const int defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if ( Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0 ) {
        int value = 0;
        for ( char *token = strtok(valuebuf, ","); token != nullptr; token = strtok(nullptr, ",") ) {
            //TODO: Requires AircraftTypeClass to be complete
            AircraftType type = AIRCRAFT_NONE; //AircraftTypeClass::From_Name(token);
            if (type != AIRCRAFT_NONE && type < AIRCRAFT_COUNT) {
                value |= (1 << type);
            }
        }
        return value;
    }
    return defvalue;
}

BOOL CCINIClass::Put_Aircrafts(const char *section, const char *entry, const int value)
{
    char buffer[MAX_LINE_LENGTH];

    for (AircraftType type = AIRCRAFT_FIRST; type < AIRCRAFT_COUNT; ++type ) {
        if ( (1 << type) & value ) {
            if ( buffer[0] != '\0' ) {
                strcat(&buffer[strlen(buffer)], ",");
            }
            //TODO: Requires AircraftTypeClass to be complete
            strcat(&buffer[strlen(buffer)], nullptr /*AircraftTypeClass::As_Reference(type).Get_Name()*/);
        }
    }
    return Put_String(section, entry, buffer);
}

const int CCINIClass::Get_Vessels(const char *section, const char *entry, const int defvalue) const
{
    char valuebuf[MAX_LINE_LENGTH];

    if ( Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0 ) {
        int value = 0;
        for ( char *token = strtok(valuebuf, ","); token != nullptr; token = strtok(nullptr, ",") ) {
            //TODO: Requires VesselTypeClass to be complete
            VesselType type = VESSEL_NONE; //VesselTypeClass::From_Name(token);
            if (type != VESSEL_NONE && type < VESSEL_COUNT) {
                value |= (1 << type);
            }
        }
        return value;
    }
    return defvalue;
}

BOOL CCINIClass::Put_Vessels(const char *section, const char *entry, const int value)
{
    char buffer[MAX_LINE_LENGTH];

    for (VesselType type = VESSEL_FIRST; type < VESSEL_COUNT; ++type ) {
        if ( (1 << type) & value ) {
            if ( buffer[0] != '\0' ) {
                strcat(&buffer[strlen(buffer)], ",");
            }
            //TODO: Requires VesselTypeClass to be complete
            strcat(&buffer[strlen(buffer)], nullptr /*VesselTypeClass::As_Reference(type).Get_Name()*/);
        }
    }
    return Put_String(section, entry, buffer);
}

const int CCINIClass::Get_Buildings(const char *section, const char *entry, const int defvalue) const
{
#ifndef CHRONOSHIFT_STANDALONE
    int (*func)(CCINIClass *, const char *, const char *, const int) =
        reinterpret_cast<int (*)(CCINIClass *, const char *, const char *, const int)>(0x00463A88);
    return func((CCINIClass *)this, section, entry, defvalue);
#else
    char valuebuf[MAX_LINE_LENGTH];

    if ( Get_String(section, entry, "", valuebuf, sizeof(valuebuf)) > 0 ) {
        int value = 0;
        for ( char *token = strtok(valuebuf, ","); token != nullptr; token = strtok(nullptr, ",") ) {
            //TODO: Requires BuildingTypeClass to be complete
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

BOOL CCINIClass::Put_Buildings(const char *section, const char *entry, const int value)
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(CCINIClass *, const char *, const char *, const int) =
        reinterpret_cast<BOOL (*)(CCINIClass *, const char *, const char *, const int)>(0x00463AFC);
    return func(this, section, entry, value);
#else
    char buffer[MAX_LINE_LENGTH];

    for ( BuildingType type = BUILDING_FIRST; type < BUILDING_VALID_COUNT; ++type ) {
        if ( (1 << type) & value ) {
            if ( buffer[0] != '\0' ) {
                strcat(&buffer[strlen(buffer)], ",");
            }
            //TODO: Requires BuildingTypeClass to be complete
            strcat(&buffer[strlen(buffer)], BuildingTypeClass::As_Reference(type).Get_Name());
        }
    }
    return Put_String(section, entry, buffer);
#endif
}

const KeyNumType CCINIClass::Get_KeyNumType(const char *section, const char *entry, const KeyNumType defvalue) const
{
    return (KeyNumType)(Get_Int(section, entry, defvalue) & 0xEFFF);
}

BOOL CCINIClass::Put_KeyNumType(const char *section, const char *entry, const KeyNumType value)
{
    return (KeyNumType)(Put_Int(section, entry, value));
}

const PKey CCINIClass::Get_PKey(BOOL fast) const
{
    PKey key;
    uint8_t buffer[512];

    if (fast) {
        DEBUG_LOG("CCINIClass::Get_PKey() - Preparing PublicKey...\n");
        Int<MAX_UNIT_PRECISION> exp(0x10001);
        MPMath::DER_Encode(exp, buffer, MAX_UNIT_PRECISION);
    } else {
        DEBUG_LOG("CCINIClass::Get_PKey() - Loading PrivateKey\n");
        Get_UUBlock("PrivateKey", buffer, sizeof(buffer));
    }

    DEBUG_LOG("CCINIClass::Get_PKey() - Decoding Exponent\n");
    key.Decode_Exponent(buffer);

    DEBUG_LOG("CCINIClass::Get_PKey() - Loading PublicKey\n");

    Get_UUBlock("PublicKey", buffer, sizeof(buffer));

    DEBUG_LOG("CCINIClass::Get_PKey() - Decoding Modulus\n");
    key.Decode_Modulus(buffer);

    return key;
}

BOOL CCINIClass::Put_PKey(PKey &key)
{
    char buffer[sizeof(PKey)];

    // Put public key
    Put_UUBlock("PublicKey", &buffer, key.Encode_Modulus(&buffer));

    // Put private key
    Put_UUBlock("PrivateKey", &buffer, key.Encode_Exponent(&buffer));

    return true;
}

void CCINIClass::Calculate_Message_Digest()
{
    if ( !DigestValid ) {
        SHAPipe pipe;
        INIClass::Save(pipe);
        pipe.Result(&Digest);
        DigestValid = true;
    }
}

void CCINIClass::Invalidate_Message_Digest()
{
    memset(&Digest, 0, sizeof(Digest));
    DigestValid = false;
}

int32_t const CCINIClass::Get_Unique_ID()
{
    if ( !DigestValid ) {
        Calculate_Message_Digest();
    }
    return Calculate_CRC(&Digest, sizeof(Digest));
}
