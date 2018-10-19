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
#include "heap.h"
#include "technotype.h"

enum VesselType
{
    VESSEL_NONE = -1,
    VESSEL_FIRST = 0,
    VESSEL_SUBMARINE = 0,
    VESSEL_DESTROYER = 1,
    VESSEL_CRUISER = 2,
    VESSEL_TRANSPORT = 3,
    VESSEL_PT_BOAT = 4,
    VESSEL_MISSILE_SUB = 5,
    VESSEL_CARRIER = 6,
    VESSEL_COUNT
};

DEFINE_ENUMERATION_OPERATORS(VesselType);

class VesselTypeClass : public TechnoTypeClass
{
public:
    VesselTypeClass(VesselType type, int uiname, const char *name, AnimType death_anim, int def_fire_coord,
        int pri_fire_coord_a, int pri_fire_coord_b, int sec_fire_coord_a, int sec_fire_coord_b, BOOL a10, BOOL nominal,
        BOOL has_turret, BOOL twin_turrets, int facings, MissionType mission);
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
    virtual BOOL Create_And_Place(int16_t cellnum, HousesType house = HOUSES_NONE) const override;
    virtual ObjectClass *Create_One_Of(HouseClass *house) const override;
    virtual const int16_t *Overlap_List() const override;

    void Code_Pointers() {}
    void Decode_Pointers() {}

    static VesselTypeClass &As_Reference(VesselType vessel);
    static VesselType From_Name(const char *name);
    static void One_Time();

private:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool m_UnkBool : 1; // & 1 Unknown, may relate to TS ini entry "Rotates".
        };
        int Bitfield;
    };
#else
    bool m_UnkBool; // Unknown, may relate to TS ini entry "Rotates".
#endif
    VesselType m_Type;
    MissionType m_UnkMissionA; // Gets set to 14 or 0 by argument in ctor, never used. 14 is MISSION_HUNT.
    MissionType m_UnkMissionB; // Gets set to 5 in ctor, never used
    AnimType m_ExplosionAnim;
    int m_UnkInt; // Gets set to 26 in One_Time(), never used
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
extern TFixedIHeapClass<VesselTypeClass> &g_VesselTypes;
#else
extern TFixedIHeapClass<VesselTypeClass> g_VesselTypes;
#endif

#endif // VESSELTYPE_H
