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
    AIRCRAFT_TRANSPORT,
    AIRCRAFT_BADGER,
    AIRCRAFT_U2,
    AIRCRAFT_MIG,
    AIRCRAFT_YAK,
    AIRCRAFT_HELI,
    AIRCRAFT_HIND,

    AIRCRAFT_COUNT,

    AIRCRAFT_NONE = -1,
    AIRCRAFT_FIRST = AIRCRAFT_TRANSPORT,
};

DEFINE_ENUMERATION_OPERATORS(AircraftType);

class AircraftTypeClass : public TechnoTypeClass
{
    friend void Setup_Hooks();
public:
    AircraftTypeClass(AircraftType type, int ui_name, const char *name, int fire_offset_z, int pri_fire_off_x,
        int pri_fire_off_y, BOOL airplane, BOOL rotors, BOOL transport, BOOL custom_rotors, BOOL radar_invisible,
        BOOL selectable, BOOL legal_target, BOOL insignificant, BOOL immune, BuildingType dock, int landing_dist,
        int rot_count, MissionType mission);
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
    virtual BOOL Create_And_Place(cell_t cellnum, HousesType house = HOUSES_NONE) const override;
    virtual ObjectClass *Create_One_Of(HouseClass *house) const override;
    virtual const int16_t *Occupy_List(BOOL recalc = false) const override;
    virtual const int16_t *Overlap_List() const override;

    void Code_Pointers() {}
    void Decode_Pointers() {}

    AircraftType What_Type() const { return m_Type; }

    static AircraftTypeClass &As_Reference(AircraftType type);
    static AircraftType From_Name(const char *name);
    static const char *Name_From(AircraftType type) { return As_Reference(type).m_Name; }

    static void One_Time();
    static void Init_Heap();

private:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_Airplane : 1;
    BOOL m_CustomRotor : 1;
    BOOL m_Rotors : 1;
    BOOL m_Transport : 1;
#else
    bool m_Airplane;
    bool m_CustomRotor; // Does it have custom rotor shapes according to facing?
    bool m_Rotors; // Does this aicraft have an attached rotor animation?
    bool m_Transport;
#endif
    AircraftType m_Type;
    MissionType m_UnkMission; // Possibly unused and leftover from Dune II? Could be initial or ai mission.
    BuildingType m_DockingBay; // The buiding type this aircraft docks at. Planes will explode if house doesn't have one.
    int m_LandingDistance;

public:
#ifdef GAME_DLL
    static void *&g_LeftRotorData;
    static void *&g_RightRotorData;
#else
    static void *g_LeftRotorData;
    static void *g_RightRotorData;
#endif

#ifdef GAME_DLL
private:
    AircraftTypeClass *Hook_Ctor(AircraftType type, int ui_name, const char *name, int def_fire_coord, int pri_fire_coord_a,
        int pri_fire_coord_b, BOOL airplane, BOOL rotors, BOOL transport, BOOL custom_rotors, BOOL radar_invisible,
        BOOL selectable, BOOL legal_target, BOOL insignificant, BOOL immune, BuildingType dock, int landing_dist,
        int rot_count, MissionType mission)
    {
        return new (this) AircraftTypeClass(type,
            ui_name,
            name,
            def_fire_coord,
            pri_fire_coord_a,
            pri_fire_coord_b,
            airplane,
            rotors,
            transport,
            custom_rotors,
            radar_invisible,
            selectable,
            legal_target,
            insignificant,
            immune,
            dock,
            landing_dist,
            rot_count,
            mission);
    }
    AircraftTypeClass *Hook_Ctor_NoInit(const NoInitClass &noinit) { return new (this) AircraftTypeClass(noinit); }
    AircraftTypeClass *Hook_Ctor_Copy(const AircraftTypeClass &that) { return new (this) AircraftTypeClass(that); }
    void Hook_Dtor() { AircraftTypeClass::~AircraftTypeClass(); }
    int Hook_Max_Pips() { return AircraftTypeClass::Max_Pips(); }
    void Hook_Dimensions(int &w, int &h) { return AircraftTypeClass::Dimensions(w, h); }
    BOOL Hook_Create_And_Place(cell_t cellnum, HousesType house)
    {
        return AircraftTypeClass::Create_And_Place(cellnum, house);
    }
    ObjectClass *Hook_Create_One_Of(HouseClass *house) { return AircraftTypeClass::Create_One_Of(house); }
    const int16_t *Hook_Occupy_List(BOOL recalc) { return AircraftTypeClass::Occupy_List(recalc); }
    const int16_t *Hook_Overlap_List() { return AircraftTypeClass::Overlap_List(); }
#endif
};

#ifdef GAME_DLL
extern TFixedIHeapClass<AircraftTypeClass> &g_AircraftTypes;
#else
extern TFixedIHeapClass<AircraftTypeClass> g_AircraftTypes;
#endif

#endif // AIRCRAFTTYPE_H
