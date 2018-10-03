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
#include "unittype.h"
#include "ccfileclass.h"
#include "ccini.h"
#include "coord.h"
#include "globals.h"
#include "minmax.h"
#include "mixfile.h"
#include "shape.h"
#include <cstdio>

using std::snprintf;

#ifndef CHRONOSHIFT_STANDALONE
TFixedIHeapClass<UnitTypeClass> &g_UnitTypes = Make_Global<TFixedIHeapClass<UnitTypeClass> >(0x0065DEEC);
#else
TFixedIHeapClass<UnitTypeClass> g_UnitTypes;
#endif

UnitTypeClass const UnitV2Launcher(UNIT_V2RL, TXT_V2RL, "V2RL", ANIM_FRAG1, REMAP_1, 0, 0, 0, 0, 0, true, false, true,
    false, false, false, false, false, false, false, true, false, false, false, FACING_COUNT_32, MISSION_SLEEP,
    MISSION_HUNT);
UnitTypeClass const UnitLTank(UNIT_LTANK, TXT_LTANK, "1TNK", ANIM_FRAG1, REMAP_1, 32, 192, 0, 0, 0, true, false, true,
    false, false, false, true, false, false, false, false, false, false, false, FACING_COUNT_32, MISSION_SLEEP,
    MISSION_HUNT);
UnitTypeClass const UnitMTank(UNIT_MTANK, TXT_HTANK, "3TNK", ANIM_FRAG1, REMAP_1, 64, 128, 24, 128, 24, true, false, true,
    false, false, false, true, false, false, false, true, false, false, false, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);
UnitTypeClass const UnitMTank2(UNIT_MTANK2, TXT_MTANK, "2TNK", ANIM_FRAG1, REMAP_1, 48, 192, 0, 192, 0, true, false, true,
    false, false, false, true, false, false, false, true, false, false, false, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);
UnitTypeClass const UnitHTank(UNIT_HTANK, TXT_4TANK, "4TNK", ANIM_ART_EXP1, REMAP_1, 32, 192, 40, 8, 64, true, false,
    true, false, false, false, true, false, false, false, true, false, false, false, FACING_COUNT_32, MISSION_SLEEP,
    MISSION_HUNT);
UnitTypeClass const UnitMRJammer(UNIT_MRJAMMER, TXT_MRJ, "MRJ", ANIM_FRAG1, REMAP_1, 0, 0, 0, 0, 0, false, false, true,
    false, true, false, false, true, false, false, false, false, true, false, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);
UnitTypeClass const UnitMGG(UNIT_MGAPGEN, TXT_MGG, "MGG", ANIM_FRAG1, REMAP_1, 0, 0, 0, 0, 0, false, false, true,
    false, false, false, false, true, false, false, true, false, false, true, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);
UnitTypeClass const UnitArty(UNIT_ARTY, TXT_ARTY, "ARTY", ANIM_ART_EXP1, REMAP_1, 64, 96, 0, 0, 0, true, false, false,
    false, false, false, false, false, false, false, false, false, false, false, FACING_COUNT_32, MISSION_SLEEP,
    MISSION_HUNT);
UnitTypeClass const UnitHarvester(UNIT_HARVESTER, TXT_HARVESTER, "HARV", ANIM_FBALL1, REMAP_2, 0, 0, 0, 0, 0, true, true,
    true, true, false, false, false, false, false, false, true, false, false, false, FACING_COUNT_32, MISSION_SLEEP,
    MISSION_HARVEST);
UnitTypeClass const UnitMCV(UNIT_MCV, TXT_MCV, "MCV", ANIM_FBALL1, REMAP_2, 0, 0, 0, 0, 0, true, false, true, false,
    false, false, false, false, false, false, true, false, false, false, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);
UnitTypeClass const UnitJeep(UNIT_JEEP, TXT_JEEP, "JEEP", ANIM_FRAG1, REMAP_1, 48, 48, 0, 48, 0, true, false, false,
    false, false, false, true, false, false, false, false, false, false, false, FACING_COUNT_32, MISSION_SLEEP,
    MISSION_HUNT);
UnitTypeClass const UnitAPC(UNIT_APC, TXT_APC, "APC", ANIM_FRAG1, REMAP_1, 48, 48, 0, 48, 0, true, false, true, false,
    false, false, false, false, false, false, false, false, false, false, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);
UnitTypeClass const UnitMineLayer(UNIT_MINELAYER, TXT_MNLY, "MNLY", ANIM_FRAG1, REMAP_1, 0, 0, 0, 0, 0, true, false,
    true, false, false, false, false, false, false, false, false, false, false, false, FACING_COUNT_32, MISSION_SLEEP,
    MISSION_HUNT);
UnitTypeClass const UnitConvoyTruck(UNIT_CARGO_TRUCK, TXT_TRUCK, "TRUK", ANIM_FRAG1, REMAP_1, 0, 0, 0, 0, 0, false,
    false, false, false, false, false, false, false, false, false, false, false, false, false, FACING_COUNT_32,
    MISSION_SLEEP, MISSION_GUARD);
UnitTypeClass const UnitAnt1(UNIT_ANT1, TXT_NULL, "ANT1", ANIM_ANTDEATH, REMAP_1, 0, 0, 0, 0, 0, false, true, false,
    false, false, true, false, false, false, false, true, false, false, false, FACING_COUNT_8, MISSION_SLEEP, MISSION_HUNT);
UnitTypeClass const UnitAnt2(UNIT_ANT2, TXT_NULL, "ANT2", ANIM_ANTDEATH, REMAP_1, 0, 0, 0, 0, 0, false, true, false,
    false, false, true, false, false, false, false, true, false, false, false, FACING_COUNT_8, MISSION_SLEEP, MISSION_HUNT);
UnitTypeClass const UnitAnt3(UNIT_ANT3, TXT_NULL, "ANT3", ANIM_ANTDEATH, REMAP_1, 0, 0, 0, 0, 0, false, true, false,
    false, false, true, false, false, false, false, true, false, false, false, FACING_COUNT_8, MISSION_SLEEP, MISSION_HUNT);
UnitTypeClass const UnitChrono(UNIT_CHRONO, TXT_CTNK, "CTNK", ANIM_FRAG1, REMAP_1, 0, 0, 0, 0, 0, false, false, true,
    false, false, false, false, false, false, false, true, false, false, false, FACING_COUNT_32, MISSION_SLEEP,
    MISSION_HUNT);
UnitTypeClass const UnitTesla(UNIT_TESLA, TXT_TTNK, "TTNK", ANIM_FRAG1, REMAP_1, 0, 0, 0, 0, 0, false, false, true,
    false, true, false, false, true, false, false, true, false, true, false, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);
UnitTypeClass const UnitMAD(UNIT_MAD_TANK, TXT_QTNK, "QTNK", ANIM_FRAG1, REMAP_1, 0, 0, 0, 0, 0, false, false, true,
    false, false, false, false, false, false, false, true, false, false, false, FACING_COUNT_32, MISSION_SLEEP,
    MISSION_HUNT);
UnitTypeClass const UnitDemoTruck(UNIT_DEMO_TRUCK, TXT_DTRK, "DTRK", ANIM_FRAG1, REMAP_1, 0, 0, 0, 0, 0, false, false,
    false, false, false, false, false, false, false, false, false, false, false, false, FACING_COUNT_32, MISSION_SLEEP,
    MISSION_GUARD);
UnitTypeClass const UnitPhase(UNIT_PHASE, TXT_STNK, "STNK", ANIM_FRAG1, REMAP_1, 48, 48, 0, 48, 0, false, false, true,
    false, false, false, true, false, false, false, true, false, false, false, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);

/**
 * 0x0056E09C
 */
UnitTypeClass::UnitTypeClass(UnitType type, int uiname, const char *name, AnimType death_anim, RemapType remap,
    int def_fire_coord, int pri_fire_coord_a, int pri_fire_coord_b, int sec_fire_coord_a, int sec_fire_coord_b,
    BOOL crate_goodie, BOOL nominal, BOOL crusher, BOOL harvester, BOOL radar_invisible, BOOL insignificant, BOOL turret, BOOL turret_spins,
    BOOL unk3, BOOL unk4, BOOL largeimage, BOOL is_viceroid, BOOL radarjammer, BOOL mgapgen, int facings, MissionType mission, MissionType alt_mission) :
    TechnoTypeClass(RTTI_UNITTYPE, type, uiname, name, remap, def_fire_coord, pri_fire_coord_a, pri_fire_coord_b,
        sec_fire_coord_a, sec_fire_coord_b, nominal, radar_invisible, true, true, insignificant, false, false, turret, true,
        true, facings, SPEED_TRACK),
    m_CrateGoodie(crate_goodie),
    m_Crusher(crusher),
    m_Harvester(harvester),
    m_TurretSpins(turret_spins),
    m_Bit16(unk3),
    m_Bit32(unk4),
    m_IsLarge(largeimage),
    m_IsViceroid(is_viceroid),
    m_IsRadarJammer(radarjammer),
    m_IsMobileGapGen(mgapgen),
    m_NoMovingFire(false),
    m_Type(type),
    m_UnkMissionA(mission),
    m_UnkMissionB(alt_mission),
    m_ExplosionAnim(death_anim),
    m_UnkInt(0)
{
    Speed = SPEED_WHEEL;
}

/**
 * 0x00578DE0
 */
UnitTypeClass::UnitTypeClass(UnitTypeClass const &that) :
    TechnoTypeClass(that),
    m_CrateGoodie(that.m_CrateGoodie),
    m_Crusher(that.m_Crusher),
    m_Harvester(that.m_Harvester),
    m_TurretSpins(that.m_TurretSpins),
    m_Bit16(that.m_Bit16),
    m_Bit32(that.m_Bit32),
    m_IsLarge(that.m_IsLarge),
    m_IsViceroid(that.m_IsViceroid),
    m_IsRadarJammer(that.m_IsRadarJammer),
    m_IsMobileGapGen(that.m_IsMobileGapGen),
    m_NoMovingFire(that.m_NoMovingFire),
    m_Type(that.m_Type),
    m_UnkMissionA(that.m_UnkMissionA),
    m_UnkMissionB(that.m_UnkMissionB),
    m_ExplosionAnim(that.m_ExplosionAnim),
    m_UnkInt(that.m_UnkInt)
{
}

/**
 * 0x0056E290
 */
void *UnitTypeClass::operator new(size_t size)
{
    DEBUG_ASSERT(size == sizeof(UnitTypeClass) && size == g_UnitTypes.Heap_Size());
    return g_UnitTypes.Allocate();
}

/**
 * 0x0056E2A4
 */
void UnitTypeClass::operator delete(void *ptr)
{
    DEBUG_ASSERT(ptr != nullptr);
    g_UnitTypes.Free(ptr);
}

/**
 * Calculate the maximum number of pips at could be displayed over the object.
 *
 * 0x00578CA8
 */
int UnitTypeClass::Max_Pips() const
{
    // TODO, Maybe set these behaviours with bools in future to remove hard coded unit behaviour?
    switch (m_Type) {
        case UNIT_HARVESTER:
            return 7;

        case UNIT_MINELAYER:
            return Ammo;

        default:
            return Max_Passengers();
    };
}

/**
 * Fetches the width and height of the object.
 *
 * 0x00578C3C
 */
void UnitTypeClass::Dimensions(int &w, int &h) const
{
    w = Min(m_UnkInt - (m_UnkInt / 4), 48);
    h = Min(m_UnkInt - (m_UnkInt / 4), 48);
}

/**
 * Creates a UnitClass instance for the specified house and places it at the specified cell.
 *
 * 0x00578B50
 */
BOOL UnitTypeClass::Create_And_Place(int16_t cellnum, HousesType house) const
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)
    (const UnitTypeClass *, int16_t, HousesType) =
        reinterpret_cast<BOOL (*)(const UnitTypeClass *, int16_t, HousesType)>(0x00578B50);
    return func(this, cellnum, house);
#else
    /*UnitClass *vptr = new UnitClass(m_Type, HOUSES_NONE);

    if (vptr != nullptr) {
        DirType dir = (DirType)Scen.Get_Random_Value(DIR_FIRST, DIR_LAST);
        return vptr->Unlimbo(Cell_To_Coord(cellnum), dir);
    }*/

    return false;
#endif
}

/**
 * Creates a UnitClass for the specified house.
 *
 * 0x00578BD0
 */
ObjectClass *UnitTypeClass::Create_One_Of(HouseClass *house) const
{
#ifndef CHRONOSHIFT_STANDALONE
    ObjectClass *(*func)(const UnitTypeClass *, HouseClass *) =
        reinterpret_cast<ObjectClass *(*)(const UnitTypeClass *, HouseClass *)>(0x00578BD0);
    return func(this, house);
#else
    /*DEBUG_ASSERT(house != nullptr);

    return new UnitClass(m_Type, house->What_Type());*/
    return nullptr;
#endif
}

/**
 * Reads an object of this type from an ini file.
 *
 * 0x00578D34
 */
BOOL UnitTypeClass::Read_INI(CCINIClass &ini)
{
    if (TechnoTypeClass::Read_INI(ini)) {
        // TODO Allow setting additional unit parameters, will need some conditional code for units to get correct
        // default with original rules.ini.

        // m_CrateGoodie = ini.Get_Bool(Get_Name(), "CrateGoodie", m_CrateGoodie);
        // m_Crusher = ini.Get_Bool(Get_Name(), "Crusher", m_Crusher);
        // m_Harvester = ini.Get_Bool(Get_Name(), "Harvester", m_Harvester);
        // m_TurretSpins = ini.Get_Bool(Get_Name(), "TurretSpins", m_TurretSpins);
        // m_IsRadarJammer = ini.Get_Bool(Get_Name(), "IsRadarJammer", m_IsRadarJammer);
        // m_IsMobileGapGen = ini.Get_Bool(Get_Name(), "IsMobileGapGen", m_IsMobileGapGen);
        m_NoMovingFire = ini.Get_Bool(Get_Name(), "NoMovingFire", m_NoMovingFire);
        Speed = ini.Get_Bool(Get_Name(), "Tracked", Speed == SPEED_TRACK) ? SPEED_TRACK : SPEED_FOOT;

        if (MovementZone == MZONE_NORMAL && m_Crusher) {
            MovementZone = MZONE_CRUSHER;
        }

        return true;
    }

    return false;
}

/**
 * Fetches a reference to the actual object from a type enum value.
 *
 * 0x00578C24
 * @warning Heap allocation order MUST match the enum order in Init_Heap for this to work.
 */
UnitTypeClass &UnitTypeClass::As_Reference(UnitType unit)
{
    DEBUG_ASSERT(unit != UNIT_NONE);
    DEBUG_ASSERT(unit < UNIT_COUNT);

    return g_UnitTypes[unit];
}

/**
 * Fetches the type enum value from a name string.
 *
 * 0x0057895C
 */
UnitType UnitTypeClass::From_Name(const char *name)
{
    if (name != nullptr) {
        for (UnitType unit = UNIT_FIRST; unit < UNIT_COUNT; ++unit) {
            if (strcasecmp(name, As_Reference(unit).Name) == 0) {
                return unit;
            }
        }
    }

    return UNIT_NONE;
}

/**
 * Initialises data that requires a one time load.
 *
 * 0x005789AC
 */
void UnitTypeClass::One_Time()
{
    char buffer[256];

    for (UnitType i = UNIT_FIRST; i < UNIT_COUNT; ++i) {
        UnitTypeClass &unit = As_Reference(i);
        const char *name = unit.ImageName[0] != '\0' ? unit.ImageName : unit.Get_Name();

        snprintf(buffer, sizeof(buffer), "%.4sicon.shp", name);
        unit.CameoData = MixFileClass<CCFileClass>::Retrieve(buffer);
        snprintf(buffer, sizeof(buffer), "%s.shp", name);
        unit.ImageData = MixFileClass<CCFileClass>::Retrieve(buffer);

        int big_dimension = 0;

        if (unit.ImageData != nullptr) {
            big_dimension = Max(Max(0, (int)Get_Build_Frame_Width(unit.ImageData)), (int)Get_Build_Frame_Height(unit.ImageData));
        }

        unit.m_UnkInt = Max(big_dimension, 8);
    }

    // TODO original initialises these here, move them somewhere more appropriate when possible.
    if (g_WakeShapes == nullptr) {
        g_WakeShapes = MixFileClass<CCFileClass>::Retrieve("wake.shp");
    }

    if (g_TurretShapes == nullptr) {
        g_TurretShapes = MixFileClass<CCFileClass>::Retrieve("turr.shp");
    }

    if (g_SamShapes == nullptr) {
        g_SamShapes = MixFileClass<CCFileClass>::Retrieve("ssam.shp");
    }

    if (g_MGunShapes == nullptr) {
        g_MGunShapes = MixFileClass<CCFileClass>::Retrieve("mgun.shp");
    }
}
