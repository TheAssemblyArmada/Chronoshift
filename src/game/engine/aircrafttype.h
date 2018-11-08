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
#pragma once

#ifndef AIRCRAFTTYPE_H
#define AIRCRAFTTYPE_H

#include "always.h"
#include "buildingtype.h"
#include "heap.h"
#include "technotype.h"

enum AircraftType
{
    AIRCRAFT_NONE = -1,
    AIRCRAFT_FIRST = 0,
    AIRCRAFT_TRANSPORT = 0,
    AIRCRAFT_BADGER = 1,
    AIRCRAFT_U2 = 2,
    AIRCRAFT_MIG = 3,
    AIRCRAFT_YAK = 4,
    AIRCRAFT_HELI = 5,
    AIRCRAFT_HIND = 6,
    AIRCRAFT_COUNT = 7
};

DEFINE_ENUMERATION_OPERATORS(AircraftType);

class AircraftTypeClass : public TechnoTypeClass
{
public:
    AircraftTypeClass(AircraftType type, int uiname, const char *name, int a4, int a5, int a6, BOOL airplane, BOOL rotors,
        BOOL transport, BOOL custom_rotors, BOOL a11, BOOL a12, BOOL a13, BOOL a14, BOOL a15, BuildingType dock,
        int landing_dist, int a18, MissionType mission);
    AircraftTypeClass(const AircraftTypeClass &that);
    AircraftTypeClass(const NoInitClass &noinit) : TechnoTypeClass(noinit) {}
    ~AircraftTypeClass() {}

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    // ObjectTypeClass virtuals
    virtual int Max_Pips() const override;
    virtual void Dimensions(int &w, int &h) const override;
    virtual BOOL Create_And_Place(int16_t cellnum, HousesType house = HOUSES_NONE) const override;
    virtual ObjectClass *Create_One_Of(HouseClass *house) const override;
    virtual const int16_t *Occupy_List(BOOL recalc = false) const override;
    virtual const int16_t *Overlap_List() const override;

    void Code_Pointers() {}
    void Decode_Pointers() {}
    
    static AircraftTypeClass &As_Reference(AircraftType type);
    static AircraftType From_Name(const char *name);
    static const char *Name_From(AircraftType type) { return As_Reference(type).Name; }
    
private:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool m_Airplane : 1;
            bool m_CustomRotor : 1;
            bool m_Rotors : 1;
            bool m_Transport : 1;
        };
        int Bitfield;
    };
#else
    bool m_Airplane;
    bool m_CustomRotor;
    bool m_Rotors;
    bool m_Transport;
#endif
    AircraftType m_Type;
    MissionType m_UnkMission; // Possibly unused?
    BuildingType m_DockingBay; // Buiding type this docks at, planes explode if player doesn't have one.
    int m_LandingDistance;

public:
#ifndef CHRONOSHIFT_STANDALONE
    static void *&LeftRotorData;
    static void *&RightRotorData;
#else
    static void *LeftRotorData;
    static void *RightRotorData;
#endif
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
extern TFixedIHeapClass<AircraftTypeClass> &g_AircraftTypes;
#else
extern TFixedIHeapClass<AircraftTypeClass> g_AircraftTypes;
#endif

#endif // AIRCRAFTTYPE_H
