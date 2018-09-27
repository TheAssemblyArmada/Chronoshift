/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class holding static info on aircraft objects.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "aircrafttype.h"
#include "lists.h"

#ifndef CHRONOSHIFT_STANDALONE
TFixedIHeapClass<AircraftTypeClass> &g_AircraftTypes = Make_Global<TFixedIHeapClass<AircraftTypeClass> >(0x0065DDBC);
void *&AircraftTypeClass::LeftRotorData = Make_Global<void *>(0x00623010);
void *&AircraftTypeClass::RightRotorData = Make_Global<void *>(0x00623014);
#else
TFixedIHeapClass<AircraftTypeClass> g_AircraftTypes;
void *AircraftTypeClass::LeftRotorData = nullptr;
void *AircraftTypeClass::RightRotorData = nullptr;
#endif

AircraftTypeClass const AircraftBadgerPlane(AIRCRAFT_BADGER, // AircraftType
    TXT_BADGER,
    "BADR", // BaseName
    0, // def_fire_coord
    0, // fire_coord_a
    0, // fire_coord_b
    true, //
    false, // HasRotors
    false, //
    false, //
    true, // RadarInvisible
    false, // Selectable
    true, // LegalTarget
    false, // Insignificant
    false, // Immune
    BUILDING_NONE, // Dock
    255, //
    FACING_COUNT_16, // Facings
    MISSION_UNLOAD //
);

AircraftTypeClass const AircraftU2Plane(AIRCRAFT_U2, // AircraftType
    TXT_U2,
    "U2", // BaseName
    0, //
    0, //
    0, //
    true, //
    false, // HasRotors
    false, //
    false, //
    true, // RadarInvisible
    false, // Selectable
    true, // LegalTarget
    false, // Insignificant
    false, // Immune
    BUILDING_NONE, // Dock
    255, //
    FACING_COUNT_16, // Facings
    MISSION_UNLOAD //
);

AircraftTypeClass const AircraftMigPlane(AIRCRAFT_MIG, // AircraftType
    TXT_MIG,
    "MIG", // BaseName
    0, //
    32, //
    32, //
    true, //
    false, // HasRotors
    false, //
    false, //
    true, // RadarInvisible
    true, // Selectable
    true, // LegalTarget
    false, // Insignificant
    false, // Immune
    BUILDING_AIRFIELD, // Dock
    192, //
    FACING_COUNT_16, // Facings
    MISSION_UNLOAD //
);

AircraftTypeClass const AircraftYakPlane(AIRCRAFT_YAK, // AircraftType
    TXT_YAK,
    "YAK", // BaseName
    0, //
    32, //
    32, //
    true, //
    false, // HasRotors
    false, //
    false, //
    true, // RadarInvisible
    true, // Selectable
    true, // LegalTarget
    false, // Insignificant
    false, // Immune
    BUILDING_AIRFIELD, // Dock
    255, //
    FACING_COUNT_16, // Facings
    MISSION_UNLOAD //
);

AircraftTypeClass const AircraftTransportHeli(AIRCRAFT_TRANSPORT, // AircraftType
    TXT_TRANS,
    "TRAN", // BaseName
    0, //
    0, //
    0, //
    false, // bool4
    true, // bool3
    true, // bool2
    true, // bool1
    true, // RadarInvisible
    true, // Selectable
    true, // LegalTarget
    false, // Insignificant
    false, // Immune
    BUILDING_NONE, // Dock
    255, //
    FACING_COUNT_32, // Facings
    MISSION_UNLOAD //
);

AircraftTypeClass const AircraftAttackHeli(AIRCRAFT_HELI, // AircraftType
    TXT_HELI,
    "HELI", // BaseName
    0, //
    64, //
    0, //
    false, //
    true, //
    false, //
    false, //
    true, // RadarInvisible
    true, // Selectable
    true, // LegalTarget
    false, // Insignificant
    false, // Immune
    BUILDING_HELIPAD, // Dock
    255, //
    FACING_COUNT_32, // Facings
    MISSION_UNLOAD //
);

AircraftTypeClass const AircraftHindHeli(AIRCRAFT_HIND, // AircraftType
    TXT_HIND,
    "HIND", // BaseName
    0, //
    64, //
    0, //
    false, //
    true, //
    false, //
    false, //
    true, // RadarInvisible
    true, // Selectable
    true, // LegalTarget
    false, // Insignificant
    false, // Immune
    BUILDING_HELIPAD, // Dock
    255, //
    FACING_COUNT_32, // Facings
    MISSION_UNLOAD //
);

/**
 * 0x00401210
 */
AircraftTypeClass::AircraftTypeClass(AircraftType type, int uiname, const char *name, int def_fire_coord, int a5, int a6,
    BOOL airplane, BOOL rotors, BOOL transport, BOOL custom_rotors, BOOL a11, BOOL a12, BOOL a13, BOOL a14, BOOL a15,
    BuildingType dock, int landing_dist, int a18, MissionType mission) :
    TechnoTypeClass(RTTI_AIRCRAFTTYPE, type, uiname, name, REMAP_1, def_fire_coord, a5, a6, a5, a6, false, a11, a12, a13, a14, a15,
        false, false, true, true, a18, SPEED_WINGED),
    m_Type(type),
    m_Airplane(airplane),
    m_CustomRotor(custom_rotors),
    m_Rotors(rotors),
    m_Transport(transport),
    m_UnkMission(mission),
    m_DockingBay(dock),
    m_LandingDistance(landing_dist)
{

}

/**
 * 0x00404E80
 */
AircraftTypeClass::AircraftTypeClass(AircraftTypeClass const &that) :
    TechnoTypeClass(that),
    m_Type(that.m_Type),
    m_Airplane(that.m_Airplane),
    m_CustomRotor(that.m_CustomRotor),
    m_Rotors(that.m_Rotors),
    m_Transport(that.m_Transport),
    m_UnkMission(that.m_UnkMission),
    m_DockingBay(that.m_DockingBay),
    m_LandingDistance(that.m_LandingDistance)
{
}

/**
 * 0x00401324
 */
void *AircraftTypeClass::operator new(size_t size)
{
    DEBUG_ASSERT(size == sizeof(AircraftTypeClass) && size == g_AircraftTypes.Heap_Size());
    return g_AircraftTypes.Allocate();
}

/**
 * 0x00401338
 */
void AircraftTypeClass::operator delete(void *ptr)
{
    DEBUG_ASSERT(ptr != nullptr);
    g_AircraftTypes.Free(ptr);
}

/**
 * Maximum number of pips that can be drawn on this object.
 *
 * 0x00404090
 */
int AircraftTypeClass::Max_Pips() const
{
    return Primary != nullptr ? 5 : Max_Passengers();
}

/**
 * Fetches the width and height of the object.
 *
 * 0x004040BC
 */
void AircraftTypeClass::Dimensions(int &w, int &h) const
{
    switch (m_Type) {
        case AIRCRAFT_BADGER:
            w = 56;
            h = 56;
            break;

        case AIRCRAFT_TRANSPORT:
        case AIRCRAFT_U2:
        case AIRCRAFT_MIG:
        case AIRCRAFT_YAK:
        case AIRCRAFT_HELI:
        case AIRCRAFT_HIND:
        default:
            w = 21;
            h = 20;
            break;
    };
}

/**
 * Creates an AircraftClass instance for the specified house and places it at the specified cell.
 *
 * 0x004040B8
 */
BOOL AircraftTypeClass::Create_And_Place(int16_t cellnum, HousesType house) const
{
    // TODO, could this be why you can't pre place aircraft on a map correctly.
    return false;
}

/**
 * Creates an AircraftClass for the specified house.
 *
 * 0x00404024
 */
ObjectClass *AircraftTypeClass::Create_One_Of(HouseClass *house) const
{
#ifndef CHRONOSHIFT_STANDALONE
    ObjectClass *(*func)(const AircraftTypeClass *, HouseClass *) =
        reinterpret_cast<ObjectClass *(*)(const AircraftTypeClass *, HouseClass *)>(0x00404024);
    return func(this, house);
#else
    /*DEBUG_ASSERT(house != nullptr);
    return new AircraftClass(Type, house->What_Type());*/

    return nullptr;
#endif
}

/**
 * Fetches the occupy list for this type.
 *
 * 0x00404078
 */
const int16_t *AircraftTypeClass::Occupy_List(BOOL recalc) const
{
    static const int16_t _list[] = { 0, LIST_END };

    return _list;
}

/**
 * Fetches the overlap list for this type.
 *
 * 0x00404084
 */
const int16_t *AircraftTypeClass::Overlap_List() const
{
    static const int16_t _list[] = {
        -127, -1, -128, -1, -129, -1, -1, -1, -255, 0, -129, 0, -128, 0, -127, 0, -1, -129, 0, 0, LIST_END
    };

    return _list;
}
