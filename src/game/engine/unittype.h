/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class holding static info on unit objects.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef UNITTYPE_H
#define UNITTYPE_H

#include "always.h"
#include "animtype.h"
#include "heap.h"
#include "technotype.h"

enum UnitType
{
    UNIT_NONE = -1,
    UNIT_FIRST = 0,
    UNIT_HTANK = 0,
    UNIT_MTANK = 1,
    UNIT_MTANK2 = 2,
    UNIT_LTANK = 3,
    UNIT_APC = 4,
    UNIT_MINELAYER = 5,
    UNIT_JEEP = 6,
    UNIT_HARVESTER = 7,
    UNIT_ARTY = 8,
    UNIT_MRJAMMER = 9,
    UNIT_MGAPGEN = 10,
    UNIT_MCV = 11,
    UNIT_V2RL = 12,
    UNIT_CARGO_TRUCK = 13,
    UNIT_ANT1 = 14,
    UNIT_ANT2 = 15,
    UNIT_ANT3 = 16,
    UNIT_CHRONO = 17,
    UNIT_TESLA = 18,
    UNIT_MAD_TANK = 19,
    UNIT_DEMO_TRUCK = 20,
    UNIT_PHASE = 21,
    UNIT_COUNT,
    UNIT_NOEXP_COUNT = 14,
    UNIT_NOAM_COUNT = 17,
};

DEFINE_ENUMERATION_OPERATORS(UnitType);

class UnitTypeClass : public TechnoTypeClass
{
public:
    UnitTypeClass(UnitType type, int uiname, const char *name, AnimType death_anim, RemapType remap, int def_fire_coord,
        int pri_fire_coord_a, int pri_fire_coord_b, int sec_fire_coord_a, int sec_fire_coord_b, BOOL crate_goodie,
        BOOL nominal, BOOL crusher, BOOL harvester, BOOL radar_invisible, BOOL insignificant, BOOL turret, BOOL turret_spins,
        BOOL unk3, BOOL unk4, BOOL largeimage, BOOL is_viceroid, BOOL radarjammer, BOOL mgapgen, int facings, MissionType mission,
        MissionType alt_mission);
    UnitTypeClass(const UnitTypeClass &that);
    UnitTypeClass(const NoInitClass &noinit) : TechnoTypeClass(noinit) {}
    ~UnitTypeClass() {}

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
    virtual BOOL Read_INI(GameINIClass &ini) override;

    void Code_Pointers() {}
    void Decode_Pointers() {}

    UnitType What_Type() const { return m_Type; }

    static UnitTypeClass &As_Reference(UnitType type);
    static UnitType From_Name(const char *name);
    static void One_Time();
    static const char *Name_From(UnitType type) { return As_Reference(type).m_Name; }

private:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_CrateGoodie : 1;
    BOOL m_Crusher : 1;
    BOOL m_Harvester : 1;
    BOOL m_TurretSpins : 1;
    BOOL m_Bit16 : 1;
    BOOL m_Bit32 : 1;
    BOOL m_IsLarge : 1;
    BOOL m_IsViceroid : 1;
    BOOL m_IsRadarJammer : 1;
    BOOL m_IsMobileGapGen : 1;
    BOOL m_NoMovingFire : 1;
#else
    bool m_CrateGoodie;
    bool m_Crusher; // Is this vehicle able to crush infantry (def = false)?
    bool m_Harvester; // Does the special Ore harvesting rules apply (def = false)?
    bool m_TurretSpins; // Does the turret just sit and spin [only if turret equipped] (def = false)?
    bool m_Bit16; // Causes the unit to start firing on itself if it has to drive to the location?
    bool m_Bit32;
    /*
    00:27 < tomscncnet> started the game
    00:29 < tomscncnet> aha
    00:29 < tomscncnet> yea, so when it aquires a target on its own it turns the entire body to face it
    00:29 < tomscncnet> with U on
    00:30 < tomscncnet> but only when it does it itself
    00:30 < tomscncnet> if i force fire it still turns the turret
    00:30 < tomscncnet> heh its like a opposite of OmniFire
    */
    bool m_IsLarge; // /IsLarge? Is large unit for refresh purposes (refresh 48x48)
    bool m_IsViceroid; // Cycle through graphics viceroid style?
    bool m_IsRadarJammer;
    bool m_IsMobileGapGen;
    bool m_NoMovingFire; // The vehicle must stop before it can fire (def = false)?
#endif
    UnitType m_Type;
    MissionType m_UnkMissionA; // Gets set to 14 or 0 by argument in ctor, never used. 14 is MISSION_HUNT.
    MissionType m_UnkMissionB;
    AnimType m_ExplosionAnim;
    int m_UnkInt; // Gets set to 8 or above in One_Time(), used for calulating dimensions.
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
extern TFixedIHeapClass<UnitTypeClass> &g_UnitTypes;
#else
extern TFixedIHeapClass<UnitTypeClass> g_UnitTypes;
#endif

#endif
