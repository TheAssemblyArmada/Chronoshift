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
#include "aircraftdata.h"
#include "gamefile.h"
#include "aircraft.h"
#include "lists.h"
#include "iomap.h"

#ifndef GAME_DLL
TFixedIHeapClass<AircraftTypeClass> g_AircraftTypes;
void *AircraftTypeClass::g_LeftRotorData = nullptr;
void *AircraftTypeClass::g_RightRotorData = nullptr;
#endif

/**
 * 0x00401210
 */
AircraftTypeClass::AircraftTypeClass(AircraftType type, int ui_name, const char *name, int fire_offset_z, int pri_fire_off_x, int pri_fire_off_y,
    BOOL airplane, BOOL rotors, BOOL transport, BOOL custom_rotors, BOOL radar_invisible, BOOL selectable, BOOL legal_target, BOOL insignificant, BOOL immune,
    BuildingType dock, int landing_dist, int rot_count, MissionType mission) :
    TechnoTypeClass(RTTI_AIRCRAFTTYPE, type, ui_name, name, REMAP_1, fire_offset_z, pri_fire_off_x, pri_fire_off_y, pri_fire_off_x, pri_fire_off_y,
        false, radar_invisible, selectable, legal_target, insignificant, immune, false, false, true, true, rot_count, SPEED_WINGED),
    m_Airplane(airplane),
    m_CustomRotor(custom_rotors),
    m_Rotors(rotors),
    m_Transport(transport),
    m_Type(type),
    m_UnkMission(mission),
    m_DockingBay(dock),
    m_LandingDistance(landing_dist)
{
}

/**
 * 0x00404E80
 */
AircraftTypeClass::AircraftTypeClass(const AircraftTypeClass &that) :
    TechnoTypeClass(that),
    m_Airplane(that.m_Airplane),
    m_CustomRotor(that.m_CustomRotor),
    m_Rotors(that.m_Rotors),
    m_Transport(that.m_Transport),
    m_Type(that.m_Type),
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
    return m_Primary != nullptr ? 5 : Max_Passengers();
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
BOOL AircraftTypeClass::Create_And_Place(cell_t cellnum, HousesType house) const
{
#if 0
    AircraftClass *aptr = new AircraftClass(m_Type, house);
    DEBUG_ASSERT(aptr != nullptr);
    if (aptr != nullptr) {
        return aptr->Unlimbo(Cell_To_Coord(cellnum));
    }

    return false;
#endif
    // TODO: This function returns false, this could be why you can't pre place aircraft on a map correctly...
    return false;
}

/**
 * Creates an AircraftClass for the specified house.
 *
 * 0x00404024
 */
ObjectClass *AircraftTypeClass::Create_One_Of(HouseClass *house) const
{
    //TODO: Requires AircaftClass to be implemented
#ifdef GAME_DLL
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
    static const int16_t _list[] = { -127, -128, -129, -1, 1, 127, 128, 129, LIST_END };

    return _list;
}

/**
 * Fetches a reference to the actual object from a type enum value.
 *
 * @warning Heap allocation order MUST match the enum order in Init_Heap for this to work.
 */
AircraftTypeClass &AircraftTypeClass::As_Reference(AircraftType type)
{
    DEBUG_ASSERT(type != AIRCRAFT_NONE);
    DEBUG_ASSERT(type < AIRCRAFT_COUNT);

    return g_AircraftTypes[type];
}

AircraftTypeClass *AircraftTypeClass::As_Pointer(AircraftType type)
{
    return (type < AIRCRAFT_COUNT) && (type != AIRCRAFT_NONE) ? &g_AircraftTypes[type] : nullptr;
}

/**
 * Fetches the type enum value from a name string.
 */
AircraftType AircraftTypeClass::From_Name(const char *name)
{
    if (name != nullptr) {
        for (AircraftType type = AIRCRAFT_FIRST; type < AIRCRAFT_COUNT; ++type) {
            if (strcasecmp(name, As_Reference(type).m_Name) == 0) {
                return type;
            }
        }
    }

    return AIRCRAFT_NONE;
}

/**
* Initialises data that requires a one time load.
*
* 0x00403F40
*/
void AircraftTypeClass::One_Time()
{
    char buffer[256];

    for (AircraftType i = AIRCRAFT_FIRST; i < AIRCRAFT_COUNT; ++i) {
        AircraftTypeClass &type = As_Reference(i);
        const char *name = type.m_ImageName[0] != '\0' ? type.m_ImageName : type.m_Name;

        snprintf(buffer, sizeof(buffer), "%.4sicon.shp", name);
        type.m_CameoData = GameFileClass::Retrieve(buffer);
        snprintf(buffer, sizeof(buffer), "%s.shp", name);
        type.m_ImageData = GameFileClass::Retrieve(buffer);
    }

    if (g_LeftRotorData == nullptr) {
        g_LeftRotorData = GameFileClass::Retrieve("lrotor.shp");
    }

    if (g_RightRotorData == nullptr) {
        g_RightRotorData = GameFileClass::Retrieve("rrotor.shp");
    }
}

/**
* @brief Initialises the memory heap for AircraftTypeClass objects.
* @warning Order of initialisation is important so enum matches position in the heap.
*
* 0x00401350
*/
void AircraftTypeClass::Init_Heap()
{
    new AircraftTypeClass(AircraftTransportHeli);
    new AircraftTypeClass(AircraftBadgerPlane);
    new AircraftTypeClass(AircraftU2Plane);
    new AircraftTypeClass(AircraftMigPlane);
    new AircraftTypeClass(AircraftYakPlane);
    new AircraftTypeClass(AircraftAttackHeli);
    new AircraftTypeClass(AircraftHindHeli);
}

/**
 * @brief 
 *
 * @address 0x00413C28 (beta)
 */
void AircraftTypeClass::Prep_For_Add()
{
    for (AircraftType i = AIRCRAFT_FIRST; i < AIRCRAFT_COUNT; ++i) {
        AircraftTypeClass *atptr = As_Pointer(i);
        if (atptr != nullptr) {
            if (atptr->m_ImageData != nullptr) {
                Map.Add_To_List(atptr);
            }
        }
    }
}

