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
TFixedIHeapClass<AircraftTypeClass> &g_AircraftTypes = *reinterpret_cast<TFixedIHeapClass<AircraftTypeClass> *>(0x0065DDBC);
#else
TFixedIHeapClass<AircraftTypeClass> g_AircraftTypes;
#endif

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

void *AircraftTypeClass::operator new(size_t size)
{
    DEBUG_ASSERT(size == sizeof(AircraftTypeClass) && size == g_AircraftTypes.Heap_Size());
    return g_AircraftTypes.Allocate();
}

void AircraftTypeClass::operator delete(void *ptr)
{
    DEBUG_ASSERT(ptr != nullptr);
    g_AircraftTypes.Free(ptr);
}

int AircraftTypeClass::Max_Pips() const
{
    return Primary != nullptr ? 5 : Max_Passengers();
}

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

BOOL AircraftTypeClass::Create_And_Place(int16_t cellnum, HousesType house) const
{
    // TODO, could this be why you can't pre place aircraft on a map correctly.
    return false;
}

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

const int16_t *AircraftTypeClass::Occupy_List(BOOL recalc) const
{
    static const int16_t _list[] = { 0, LIST_END };

    return _list;
}

const int16_t *AircraftTypeClass::Overlap_List() const
{
    static const int16_t _list[] = {
        -127, -1, -128, -1, -129, -1, -1, -1, -255, 0, -129, 0, -128, 0, -127, 0, -1, -129, 0, 0, LIST_END
    };

    return _list;
}
