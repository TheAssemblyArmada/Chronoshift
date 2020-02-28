/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class holding static info on vessel objects.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef VESSELTYPE_H
#define VESSELTYPE_H

#include "always.h"
#include "animtype.h"
#include "facing.h"
#include "heap.h"
#include "technotype.h"

enum VesselType
{
    VESSEL_SUBMARINE,
    VESSEL_DESTROYER,
    VESSEL_CRUISER,
    VESSEL_TRANSPORT,
    VESSEL_PT_BOAT,

    // Aftermath additions.
    VESSEL_MISSILE_SUB,
    VESSEL_CARRIER,

    VESSEL_COUNT,
    VESSEL_NOEXP_COUNT = 5, // Used for arrays that exclude new expansion units.

    VESSEL_NONE = -1,
    VESSEL_FIRST = VESSEL_SUBMARINE,
};

DEFINE_ENUMERATION_OPERATORS(VesselType);

class VesselTypeClass : public TechnoTypeClass
{
public:
    VesselTypeClass(VesselType type, int uiname, const char *name, AnimType death_anim, int def_fire_coord,
        int pri_fire_coord_a, int pri_fire_coord_b, int sec_fire_coord_a, int sec_fire_coord_b, BOOL a10, BOOL nominal,
        BOOL has_turret, int facings, MissionType mission);
    VesselTypeClass(const VesselTypeClass &that);
    VesselTypeClass(const NoInitClass &noinit) : TechnoTypeClass(noinit) {}
    ~VesselTypeClass() {}

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    virtual int Max_Pips() const override;
    virtual void Dimensions(int &w, int &h) const override;
    virtual BOOL Create_And_Place(cell_t cellnum, HousesType house = HOUSES_NONE) const override;
    virtual ObjectClass *Create_One_Of(HouseClass *house) const override;
    virtual const int16_t *Overlap_List() const override;

    void Code_Pointers() {}
    void Decode_Pointers() {}

    VesselType What_Type() const { return m_Type; }

    static void One_Time();
    static void Init_Heap();
    static VesselTypeClass &As_Reference(VesselType type);
    static VesselTypeClass *As_Pointer(VesselType type);
    static VesselType From_Name(const char *name);
    static const char *Name_From(VesselType type) { return As_Reference(type).m_Name; }

    static void Prep_For_Add();

private:
    void Turret_Adjust(DirType dir, int &x, int &y) const;

#ifdef GAME_DLL
public:
    friend void Setup_Hooks();

    VesselTypeClass *Hooked_Ctor(VesselType type, int uiname, const char *name, AnimType death_anim, int def_fire_coord,
        int pri_fire_coord_a, int pri_fire_coord_b, int sec_fire_coord_a, int sec_fire_coord_b, BOOL a10, BOOL nominal,
        BOOL has_turret, int facings, MissionType mission)
    {
        return new (this) VesselTypeClass(type,
            uiname,
            name,
            death_anim,
            def_fire_coord,
            pri_fire_coord_a,
            pri_fire_coord_b,
            sec_fire_coord_a,
            sec_fire_coord_b,
            a10,
            nominal,
            has_turret,
            facings,
            mission);
    }
    int Hooked_Max_Pips() { return VesselTypeClass::Max_Pips(); }
    void Hooked_Dimensions(int &w, int &h) const { VesselTypeClass::Dimensions(w, h); }
    BOOL Hooked_Create_And_Place(cell_t cellnum, HousesType house) const { return VesselTypeClass::Create_And_Place(cellnum, house); }
    ObjectClass *Hooked_Create_One_Of(HouseClass *house) const { return VesselTypeClass::Create_One_Of(house); }
    const int16_t *Hooked_Overlap_List() const { return VesselTypeClass::Hooked_Overlap_List(); }
    void Hooked_Turret_Adjust(DirType dir, int &x, int &y) const { VesselTypeClass::Turret_Adjust(dir, x, y); }
    void Hooked_One_Time() { VesselTypeClass::One_Time(); }
    void Hooked_Init_Heap() { VesselTypeClass::Init_Heap(); }
#endif

private:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_UnkBool : 1; // & 1 Unknown, may relate to TS ini entry "Rotates".
#else
    bool m_UnkBool; // Unknown, may relate to TS ini entry "Rotates".
#endif
    VesselType m_Type;
    MissionType m_UnkMissionA; // Gets set to 14 or 0 by argument in ctor, never used. 14 is MISSION_HUNT.
    MissionType m_UnkMissionB; // Gets set to 5 in ctor, never used
    AnimType m_ExplosionAnim;
    int m_UnkInt; // Gets set to 26 in One_Time(), never used
};

#ifdef GAME_DLL
extern TFixedIHeapClass<VesselTypeClass> &g_VesselTypes;
#else
extern TFixedIHeapClass<VesselTypeClass> g_VesselTypes;
#endif

#endif // VESSELTYPE_H
