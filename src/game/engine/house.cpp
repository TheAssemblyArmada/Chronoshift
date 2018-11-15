/**
 * @file
 *
 * @author CCHyper
 *
 * @brief Class holding information on houses.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "house.h"
#include "display.h"
#include "globals.h"
#include "heap.h"
#include "remap.h"
#include "rules.h"
#include "scenario.h"
#include "session.h"
#include "logic.h"

#ifndef CHRONOSHIFT_STANDALONE
TFixedIHeapClass<HouseClass> &g_Houses = Make_Global<TFixedIHeapClass<HouseClass> >(0x0065D994);
HouseClass *&g_PlayerPtr = Make_Global<HouseClass *>(0x00669958);
#else
TFixedIHeapClass<HouseClass> g_Houses;
HouseClass *g_PlayerPtr = nullptr;
#endif

HouseClass::HouseClass(HousesType type) :
    m_RTTI(RTTI_HOUSE),
    m_HeapID(g_Houses.ID(this)),
    m_Type(g_HouseTypes.Ptr(type)),
    m_AIDifficulty(Scen.Get_AI_Difficulty()),
    m_FirepowerMult("1.0"),
    m_GroundspeedMult("1.0"),
    m_AirspeedMult("1.0"),
    m_ArmorMult("1.0"),
    m_ROFMult("1.0"),
    m_CostMult("1.0"),
    m_BuildTimerMult("1.0"),
    m_RepairDelay("0.02"),
    m_BuildDelay("0.03"),
    m_Static(),
    m_ActsLike(m_Type->Get_Type()),
    m_IsHuman(false),
    m_PlayerControl(false),
    m_Production(false),
    m_Autocreate(false),
    m_AutoBaseAI(false),
    m_Discovered(false),
    m_MaxCapacity(false),
    m_Defeated(false),
    m_ToDie(false),
    m_ToWin(false),
    m_ToLose(false),
    m_CivEvac(false),
    m_RecalcNeeded(true),
    m_Visionary(false),
    m_OreShort(false),
    m_Bit3_1(false),
    m_Bit3_2(false),
    m_Repairing(false),
    m_MapIsClear(false),
    m_BuiltSomething(false),
    m_Resigned(false),
    m_GaveUp(false),
    m_Paranoid(false),
    m_AllToLook(true),
    m_CurrentIQ(m_Static.Get_IQ()),
    m_Smarties(URGENCY_NOTHING),
    m_JustBuilding(BUILDING_NONE),
    m_JustInfantry(INFANTRY_NONE),
    m_JustUnit(UNIT_NONE),
    m_JustAircraft(AIRCRAFT_NONE),
    m_JustVessel(VESSEL_NONE),
    m_WinBlocks(0),
    m_RepairTime(0),
    m_AlertTime(0),
    m_BorrowedTime(0),
    m_field_137(0),
    m_field_13B(0),
    m_field_13F(0),
    m_field_143(0),
    m_field_147(0),
    m_field_14B(0),
    m_field_14F(0),
    m_field_153(0),
    m_field_157(0),
    m_field_15B(0),
    m_field_15F(0),
    m_field_163(0),
    m_field_167(0),
    m_field_16B(0),
    m_field_16F(0),
    m_Spent(0),
    m_Harvested(0),
    m_Stolen(0),
    m_CurrentUnitCount(0),
    m_CurrentBuildingCount(0),
    m_CurrentInfantryCount(0),
    m_CurrentVesselCount(0),
    m_CurrentAircraftCount(0),
    m_Ore(0),
    m_Credits(0),
    m_Capacity(0),
    m_AircraftBought(nullptr),
    m_InfantryBought(nullptr),
    m_UnitsBought(nullptr),
    m_BuildingsBought(nullptr),
    m_VesselsBought(nullptr),
    m_AircraftKilled(nullptr),
    m_InfantryKilled(nullptr),
    m_UnitsKilled(nullptr),
    m_BuildingsKilled(nullptr),
    m_VesselsKilled(nullptr),
    m_BuildingsCaptured(nullptr),
    m_CratesFound(nullptr),
    m_AircraftFactoryCount(0),
    m_InfantryFactoryCount(0),
    m_UnitFactoryCount(0),
    m_VesselFactoryCount(0),
    m_BuildingFactoryCount(0),
    m_Power(0),
    m_Drain(0),
    m_AircraftFactory(nullptr),
    m_InfantryFactory(nullptr),
    m_UnitFactory(nullptr),
    m_VesselFactory(nullptr),
    m_BuildingFactory(nullptr),
    m_FlagHolder(0),
    m_FlagLocation(0),
    m_UnitsLost(0),
    m_BuildingsLost(0),
    m_field_2AD(HOUSES_NONE),
    m_BaseCenter(0),
    m_Radius(0),
    m_LastAttackFrame(0),
    m_LastAttackType(RTTI_NONE),
    m_LastAttackZone(ZONE_NONE),
    m_LastAttackHouse(HOUSES_NONE),
    m_field_2F9(0),
    m_field_2FD(0),
    m_Score(0),
    m_PreferredTarget(QUARRY_ANYTHING),
    m_AttackTime(Rule.Attack_Delay() * Scen.Get_Random_Value(450, 1800)),
    m_field_527(HOUSES_NONE),
    m_AITime(0),
    m_ChronoObject(0),
    m_ChosenBuilding(BUILDING_NONE),
    m_ChosenUnit(UNIT_NONE),
    m_ChosenInfantry(INFANTRY_NONE),
    m_ChosenAircraft(AIRCRAFT_NONE),
    m_ChosenVessel(VESSEL_NONE),
    m_MissileTargetCell(0),
    m_Allies(0),
    m_DamageTime(Rule.Get_Damage_Delay() * 900),
    m_TeamTime(Rule.Team_Delay() * 900),
    m_TriggerTime(0),
    m_AttackedSpeechTime(1),
    m_LowPowerSpeechTime(1),
    m_field_177D(1),
    m_LowCreditsSpeechTime(1),
    m_Color(m_Type->Get_Color())
{
    memset(m_Zones, 0, sizeof(m_Zones));

    memset(m_UnitsDestroyed, 0, sizeof(m_UnitsDestroyed));
    memset(m_BuildingsDestroyed, 0, sizeof(m_BuildingsDestroyed));
    memset(m_BuildingQuantity, 0, sizeof(m_BuildingQuantity));
    memset(m_UnitQuantity, 0, sizeof(m_UnitQuantity));
    memset(m_InfantryQuantity, 0, sizeof(m_InfantryQuantity));
    memset(m_AircraftQuantity, 0, sizeof(m_AircraftQuantity));
    memset(m_VesselQuantity, 0, sizeof(m_VesselQuantity));

    memset(m_ThreatRegions, 0, sizeof(m_ThreatRegions));

    new (&m_Specials[SPECIAL_SONAR_PULSE]) SuperClass(Rule.Get_Recharge_Sonar() * 900, false, VOX_NONE, VOX_SONAR_PULSE_AVAILABLE, VOX_NONE, VOX_NONE);
    new (&m_Specials[SPECIAL_ATOM_BOMB]) SuperClass(Rule.Get_Recharge_Nuke() * 900, true, VOX_ABOMB_PREPING, VOX_ABOMB_READY, VOX_NONE, VOX_NO_POWER);
    new (&m_Specials[SPECIAL_WARP_SPHERE]) SuperClass(Rule.Get_Recharge_Chrono() * 900, true, VOX_CHROCHR1, VOX_CHRORDY1, VOX_NONE, VOX_NO_POWER);
    new (&m_Specials[SPECIAL_PARA_BOMB]) SuperClass(Rule.Get_Recharge_ParaBomb() * 900, false, VOX_NONE, VOX_NONE, VOX_NONE, VOX_NONE);
    new (&m_Specials[SPECIAL_PARA_INFANTRY]) SuperClass(Rule.Get_Recharge_Paratrooper() * 900, false, VOX_NONE, VOX_NONE, VOX_NONE, VOX_NONE);
    new (&m_Specials[SPECIAL_SPY_PLANE]) SuperClass(Rule.Get_Recharge_SpyPlane() * 900, false, VOX_NONE, VOX_SPY_PLANE, VOX_NONE, VOX_NONE);
    new (&m_Specials[SPECIAL_IRON_CURTAIN]) SuperClass(Rule.Get_Recharge_IronCurtain() * 900, true, VOX_IRON_CHARGING, VOX_IRON_READY, VOX_NONE, VOX_NO_POWER);
    new (&m_Specials[SPECIAL_GPS]) SuperClass(Rule.Get_Recharge_GPS() * 900, true, VOX_NONE, VOX_NONE, VOX_NONE, VOX_NO_POWER);

    strncpy(m_field_1790, Fetch_String(TXT_COMPUTER), sizeof(m_field_1790));
    memset(m_PlayerHandle, 0, sizeof(m_PlayerHandle));

    if (Session.Game_To_Play() == GAME_INTERNET) {
        m_AircraftBought = new UnitTrackerClass(AIRCRAFT_COUNT);
        m_InfantryBought = new UnitTrackerClass(INFANTRY_COUNT);
        m_UnitsBought = new UnitTrackerClass(UNIT_COUNT);
        m_BuildingsBought = new UnitTrackerClass(BUILDING_COUNT);
        m_VesselsBought = new UnitTrackerClass(VESSEL_COUNT);
        m_AircraftKilled = new UnitTrackerClass(AIRCRAFT_COUNT);
        m_InfantryKilled = new UnitTrackerClass(INFANTRY_COUNT);
        m_UnitsKilled = new UnitTrackerClass(UNIT_COUNT);
        m_BuildingsKilled = new UnitTrackerClass(BUILDING_COUNT);
        m_VesselsKilled = new UnitTrackerClass(VESSEL_COUNT);
        m_BuildingsCaptured = new UnitTrackerClass(BUILDING_COUNT);
        m_CratesFound = new UnitTrackerClass(CRATE_COUNT);
    }

    g_HouseTriggers[type].Clear();

    Make_Ally(type);
    Assign_Handicap(Scen.Get_AI_Difficulty());
}

HouseClass::HouseClass(const HouseClass &that) :
    m_RTTI(that.m_RTTI),
    m_HeapID(that.m_HeapID),
    m_Type(that.m_Type),
    m_AIDifficulty(that.m_AIDifficulty),
    m_FirepowerMult(that.m_FirepowerMult),
    m_GroundspeedMult(that.m_GroundspeedMult),
    m_AirspeedMult(that.m_AirspeedMult),
    m_ArmorMult(that.m_ArmorMult),
    m_ROFMult(that.m_ROFMult),
    m_CostMult(that.m_CostMult),
    m_BuildTimerMult(that.m_BuildTimerMult),
    m_RepairDelay(that.m_RepairDelay),
    m_BuildDelay(that.m_BuildDelay),
    m_Static(that.m_Static),
    m_ActsLike(that.m_ActsLike),
    m_IsActive(that.m_IsActive),
    m_IsHuman(that.m_IsHuman),
    m_PlayerControl(that.m_PlayerControl),
    m_Production(that.m_Production),
    m_Autocreate(that.m_Autocreate),
    m_AutoBaseAI(that.m_AutoBaseAI),
    m_Discovered(that.m_Discovered),
    m_MaxCapacity(that.m_MaxCapacity),
    m_Defeated(that.m_Defeated),
    m_ToDie(that.m_ToDie),
    m_ToWin(that.m_ToWin),
    m_ToLose(that.m_ToLose),
    m_CivEvac(that.m_CivEvac),
    m_RecalcNeeded(that.m_RecalcNeeded),
    m_Visionary(that.m_Visionary),
    m_OreShort(that.m_OreShort),
    m_Bit3_1(that.m_Bit3_1),
    m_Bit3_2(that.m_Bit3_2),
    m_Repairing(that.m_Repairing),
    m_MapIsClear(that.m_MapIsClear),
    m_BuiltSomething(that.m_BuiltSomething),
    m_Resigned(that.m_Resigned),
    m_GaveUp(that.m_GaveUp),
    m_Paranoid(that.m_Paranoid),
    m_AllToLook(that.m_AllToLook),
    m_CurrentIQ(that.m_CurrentIQ),
    m_Smarties(that.m_Smarties),
    m_JustBuilding(that.m_JustBuilding),
    m_JustInfantry(that.m_JustInfantry),
    m_JustUnit(that.m_JustUnit),
    m_JustAircraft(that.m_JustAircraft),
    m_JustVessel(that.m_JustVessel),
    m_WinBlocks(that.m_WinBlocks),
    m_RepairTime(that.m_RepairTime),
    m_AlertTime(that.m_AlertTime),
    m_BorrowedTime(that.m_BorrowedTime),
    m_field_137(that.m_field_137),
    m_field_13B(that.m_field_13B),
    m_field_13F(that.m_field_13F),
    m_field_143(that.m_field_143),
    m_field_147(that.m_field_147),
    m_field_14B(that.m_field_14B),
    m_field_14F(that.m_field_14F),
    m_field_153(that.m_field_153),
    m_field_157(that.m_field_157),
    m_field_15B(that.m_field_15B),
    m_field_15F(that.m_field_15F),
    m_field_163(that.m_field_163),
    m_field_167(that.m_field_167),
    m_field_16B(that.m_field_16B),
    m_field_16F(that.m_field_16F),
    m_Spent(that.m_Spent),
    m_Harvested(that.m_Harvested),
    m_Stolen(that.m_Stolen),
    m_CurrentUnitCount(that.m_CurrentUnitCount),
    m_CurrentBuildingCount(that.m_CurrentBuildingCount),
    m_CurrentInfantryCount(that.m_CurrentInfantryCount),
    m_CurrentVesselCount(that.m_CurrentVesselCount),
    m_CurrentAircraftCount(that.m_CurrentAircraftCount),
    m_Ore(that.m_Ore),
    m_Credits(that.m_Credits),
    m_Capacity(that.m_Capacity),
    m_AircraftBought(that.m_AircraftBought),
    m_InfantryBought(that.m_InfantryBought),
    m_UnitsBought(that.m_UnitsBought),
    m_BuildingsBought(that.m_BuildingsBought),
    m_VesselsBought(that.m_VesselsBought),
    m_AircraftKilled(that.m_AircraftKilled),
    m_InfantryKilled(that.m_InfantryKilled),
    m_UnitsKilled(that.m_UnitsKilled),
    m_BuildingsKilled(that.m_BuildingsKilled),
    m_VesselsKilled(that.m_VesselsKilled),
    m_BuildingsCaptured(that.m_BuildingsCaptured),
    m_CratesFound(that.m_CratesFound),
    m_AircraftFactoryCount(that.m_AircraftFactoryCount),
    m_InfantryFactoryCount(that.m_InfantryFactoryCount),
    m_UnitFactoryCount(that.m_UnitFactoryCount),
    m_VesselFactoryCount(that.m_VesselFactoryCount),
    m_BuildingFactoryCount(that.m_BuildingFactoryCount),
    m_Power(that.m_Power),
    m_Drain(that.m_Drain),
    m_AircraftFactory(that.m_AircraftFactory),
    m_InfantryFactory(that.m_InfantryFactory),
    m_UnitFactory(that.m_UnitFactory),
    m_VesselFactory(that.m_VesselFactory),
    m_BuildingFactory(that.m_BuildingFactory),
    m_FlagHolder(that.m_FlagHolder),
    m_FlagLocation(that.m_FlagLocation),
    m_UnitsLost(that.m_UnitsLost),
    m_BuildingsLost(that.m_BuildingsLost),
    m_field_2AD(that.m_field_2AD),
    m_BaseCenter(that.m_BaseCenter),
    m_Radius(that.m_Radius),
    m_LastAttackFrame(that.m_LastAttackFrame),
    m_LastAttackType(that.m_LastAttackType),
    m_LastAttackZone(that.m_LastAttackZone),
    m_LastAttackHouse(that.m_LastAttackHouse),
    m_field_2F9(that.m_field_2F9),
    m_field_2FD(that.m_field_2FD),
    m_Score(that.m_Score),
    m_PreferredTarget(that.m_PreferredTarget),
    m_AttackTime(that.m_AttackTime),
    m_field_527(that.m_field_527),
    m_AITime(that.m_AITime),
    m_ChronoObject(that.m_ChronoObject),
    m_ChosenBuilding(that.m_ChosenBuilding),
    m_ChosenUnit(that.m_ChosenUnit),
    m_ChosenInfantry(that.m_ChosenInfantry),
    m_ChosenAircraft(that.m_ChosenAircraft),
    m_ChosenVessel(that.m_ChosenVessel),
    m_MissileTargetCell(that.m_MissileTargetCell),
    m_Allies(that.m_Allies),
    m_DamageTime(that.m_DamageTime),
    m_TeamTime(that.m_TeamTime),
    m_TriggerTime(that.m_TriggerTime),
    m_AttackedSpeechTime(that.m_AttackedSpeechTime),
    m_LowPowerSpeechTime(that.m_LowPowerSpeechTime),
    m_field_177D(that.m_field_177D),
    m_LowCreditsSpeechTime(that.m_LowCreditsSpeechTime),
    m_Color(that.m_Color)
{
    memcpy(m_Specials, that.m_Specials, sizeof(m_Specials));
    memcpy(m_UnitsDestroyed, that.m_UnitsDestroyed, sizeof(m_UnitsDestroyed));
    memcpy(m_BuildingsDestroyed, that.m_BuildingsDestroyed, sizeof(m_UnitsDestroyed));
    memcpy(m_Zones, that.m_Zones, sizeof(m_Zones));
    memcpy(m_BuildingQuantity, that.m_BuildingQuantity, sizeof(m_BuildingQuantity));
    memcpy(m_UnitQuantity, that.m_UnitQuantity, sizeof(m_UnitQuantity));
    memcpy(m_InfantryQuantity, that.m_InfantryQuantity, sizeof(m_InfantryQuantity));
    memcpy(m_AircraftQuantity, that.m_AircraftQuantity, sizeof(m_AircraftQuantity));
    memcpy(m_VesselQuantity, that.m_VesselQuantity, sizeof(m_VesselQuantity));
    memcpy(m_ThreatRegions, that.m_ThreatRegions, sizeof(m_ThreatRegions));
    memcpy(m_field_1790, that.m_field_1790, sizeof(m_field_1790));
    memcpy(m_PlayerHandle, that.m_PlayerHandle, sizeof(m_PlayerHandle));
}

HouseClass::~HouseClass()
{
    if (Session.Game_To_Play() == GAME_INTERNET) {
        delete m_AircraftBought;
        delete m_InfantryBought;
        delete m_UnitsBought;
        delete m_BuildingsBought;
        delete m_VesselsBought;
        delete m_AircraftKilled;
        delete m_InfantryKilled;
        delete m_UnitsKilled;
        delete m_BuildingsKilled;
        delete m_VesselsKilled;
        delete m_BuildingsCaptured;
        delete m_CratesFound;
    }
}

void *HouseClass::operator new(size_t size)
{
    HouseClass *this_ptr = g_Houses.Alloc();
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = true;
    }
    return this_ptr;
}

void HouseClass::operator delete(void *ptr)
{
    HouseClass *this_ptr = static_cast<HouseClass *>(ptr);
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = false;
    }
    g_Houses.Free(this_ptr);
}

void HouseClass::Spend_Money(unsigned int amount)
{
    if (amount <= m_Ore) {
        m_Ore -= amount;
    } else {
        amount -= m_Ore;
        m_Ore = 0;
        m_Credits -= amount;
    }
    Silo_Redraw_Check(amount, m_Capacity);
    m_Spent += amount;
}

void HouseClass::Silo_Redraw_Check(unsigned int a1, unsigned int a2)
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(HouseClass *, unsigned int, unsigned int) =
        reinterpret_cast<void (*)(HouseClass *, unsigned int, unsigned int)>(0x004D5EF4);
    func(this, a1, a2);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
#endif
}

DiffType HouseClass::Assign_Handicap(DiffType diff)
{
#ifndef CHRONOSHIFT_STANDALONE
    DiffType (*func)(HouseClass *, DiffType) =
        reinterpret_cast<DiffType (*)(HouseClass *, DiffType)>(0x004D2D48);
    return func(this, Scen.Get_AI_Difficulty());
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return DIFF_NONE;
#endif
}

void HouseClass::Make_Ally(HousesType type)
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(HouseClass *, HousesType) =
        reinterpret_cast<void (*)(HouseClass *, HousesType)>(0x004D6060);
    func(this, type);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
#endif
}

void HouseClass::Make_Enemy(HousesType type)
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(HouseClass *, HousesType) =
        reinterpret_cast<void (*)(HouseClass *, HousesType)>(0x004D6370);
    func(this, type);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
#endif
}

BOOL HouseClass::Flag_To_Die()
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(HouseClass *) = reinterpret_cast<BOOL (*)(HouseClass *)>(0x004D8B40);
    return func(this);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return false;
#endif
}

BOOL HouseClass::Flag_To_Win()
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(HouseClass *) = reinterpret_cast<BOOL (*)(HouseClass *)>(0x004D8BB8);
    return func(this);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return false;
#endif
}

BOOL HouseClass::Flag_To_Lose()
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(HouseClass *) = reinterpret_cast<BOOL (*)(HouseClass *)>(0x004D8C30);
    return func(this);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return false;
#endif
}

const uint8_t *HouseClass::Remap_Table(BOOL unk1, RemapType type)
{
    if (unk1) {
        // TODO Make fading table types consistent across project.
        return (const uint8_t *)DisplayClass::FadingLight;
    }

    if (type != REMAP_0) {
        return ColorRemaps[m_Color].RemapPalette;
    }

    return nullptr;
}
