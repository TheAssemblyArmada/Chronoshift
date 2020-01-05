/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 * @author tomsons26
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
#include "aircraft.h"
#include "building.h"
#include "coord.h"
#include "display.h"
#include "foot.h"
#include "fetchtechtype.h"
#include "gameini.h"
#include "globals.h"
#include "heap.h"
#include "housetype.h"
#include "infantry.h"
#include "iomap.h"
#include "logic.h"
#include "remap.h"
#include "rules.h"
#include "scenario.h"
#include "session.h"
#include "target.h"
#include "teamtype.h"
#include "unit.h"
#include "utracker.h"
#include "vessel.h"
#include <algorithm>
#include <cstdio>

using std::max;
using std::min;
using std::snprintf;

#ifndef GAME_DLL
TFixedIHeapClass<HouseClass> g_Houses;
HouseClass *g_PlayerPtr = nullptr;
TFixedIHeapClass<HouseClass::BuildChoiceClass> HouseClass::g_BuildChoice;
#endif

/**
 * 0x004D33E4
 */
HouseClass::HouseClass(HousesType type) :
    m_RTTI(RTTI_HOUSE),
    m_HeapID(g_Houses.ID(this)),
    m_Class(g_HouseTypes.Ptr(type)),
    m_AIDifficulty(g_Scen.Get_AI_Difficulty()),
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
    m_ActsLike(m_Class->Get_Type()),
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
    m_Spied(false),
    m_Infiltrated(false),
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
    m_BScan(),
    m_UScan(),
    m_IScan(),
    m_AScan(),
    m_VScan(),
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
    m_UnkHouseType(HOUSES_NONE),
    m_BaseCenter(0),
    m_Radius(0),
    m_LastAttackFrame(0),
    m_LastAttackType(RTTI_NONE),
    m_LastAttackZone(ZONE_NONE),
    m_LastAttackHouse(HOUSES_NONE),
    m_LastCapBldTarget(0),
    m_RadarSpied(0),
    m_Score(0),
    m_PreferredTarget(QUARRY_ANYTHING),
    m_AttackTime(g_Rule.Attack_Delay() * g_Scen.Get_Random_Value(450, 1800)),
    m_Enemy(HOUSES_NONE),
    m_AITime(0),
    m_ChronoObject(0),
    m_ChosenBuilding(BUILDING_NONE),
    m_ChosenUnit(UNIT_NONE),
    m_ChosenInfantry(INFANTRY_NONE),
    m_ChosenAircraft(AIRCRAFT_NONE),
    m_ChosenVessel(VESSEL_NONE),
    m_MissileTargetCell(0),
    m_Allies(0),
    m_DamageTime(g_Rule.Get_Damage_Delay() * 900),
    m_TeamTime(g_Rule.Team_Delay() * 900),
    m_TriggerTime(0),
    m_AttackedSpeechTime(1),
    m_LowPowerSpeechTime(1),
    m_UnkTimer(1),
    m_LowCreditsSpeechTime(1),
    m_Color(m_Class->Get_Color())
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

    new (&m_Specials[SPECIAL_SONAR_PULSE])
        SuperClass(g_Rule.Get_Recharge_Sonar() * 900, false, VOX_NONE, VOX_SONAR_PULSE_AVAILABLE, VOX_NONE, VOX_NONE);
    new (&m_Specials[SPECIAL_ATOM_BOMB])
        SuperClass(g_Rule.Get_Recharge_Nuke() * 900, true, VOX_ABOMB_PREPING, VOX_ABOMB_READY, VOX_NONE, VOX_NO_POWER);
    new (&m_Specials[SPECIAL_WARP_SPHERE])
        SuperClass(g_Rule.Get_Recharge_Chrono() * 900, true, VOX_CHROCHR1, VOX_CHRORDY1, VOX_NONE, VOX_NO_POWER);
    new (&m_Specials[SPECIAL_PARA_BOMB])
        SuperClass(g_Rule.Get_Recharge_ParaBomb() * 900, false, VOX_NONE, VOX_NONE, VOX_NONE, VOX_NONE);
    new (&m_Specials[SPECIAL_PARA_INFANTRY])
        SuperClass(g_Rule.Get_Recharge_Paratrooper() * 900, false, VOX_NONE, VOX_NONE, VOX_NONE, VOX_NONE);
    new (&m_Specials[SPECIAL_SPY_PLANE])
        SuperClass(g_Rule.Get_Recharge_SpyPlane() * 900, false, VOX_NONE, VOX_SPY_PLANE, VOX_NONE, VOX_NONE);
    new (&m_Specials[SPECIAL_IRON_CURTAIN])
        SuperClass(g_Rule.Get_Recharge_IronCurtain() * 900, true, VOX_IRON_CHARGING, VOX_IRON_READY, VOX_NONE, VOX_NO_POWER);
    new (&m_Specials[SPECIAL_GPS])
        SuperClass(g_Rule.Get_Recharge_GPS() * 900, true, VOX_NONE, VOX_NONE, VOX_NONE, VOX_NO_POWER);

    strlcpy(m_PlayerName, Text_String(TXT_COMPUTER), sizeof(m_PlayerName));
    memset(m_PlayerHandle, 0, sizeof(m_PlayerHandle));

    if (g_Session.Game_To_Play() == GAME_INTERNET) {
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
    Assign_Handicap(g_Scen.Get_AI_Difficulty());
}

/**
 * Not seen in original.
 */
HouseClass::HouseClass(const HouseClass &that) :
    m_RTTI(that.m_RTTI),
    m_HeapID(that.m_HeapID),
    m_Class(that.m_Class),
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
    m_Spied(that.m_Spied),
    m_Infiltrated(that.m_Infiltrated),
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
    m_BScan(that.m_BScan),
    m_UScan(that.m_UScan),
    m_IScan(that.m_IScan),
    m_AScan(that.m_AScan),
    m_VScan(that.m_VScan),
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
    m_UnkHouseType(that.m_UnkHouseType),
    m_BaseCenter(that.m_BaseCenter),
    m_Radius(that.m_Radius),
    m_LastAttackFrame(that.m_LastAttackFrame),
    m_LastAttackType(that.m_LastAttackType),
    m_LastAttackZone(that.m_LastAttackZone),
    m_LastAttackHouse(that.m_LastAttackHouse),
    m_LastCapBldTarget(that.m_LastCapBldTarget),
    m_RadarSpied(that.m_RadarSpied),
    m_Score(that.m_Score),
    m_PreferredTarget(that.m_PreferredTarget),
    m_AttackTime(that.m_AttackTime),
    m_Enemy(that.m_Enemy),
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
    m_UnkTimer(that.m_UnkTimer),
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
    memcpy(m_PlayerName, that.m_PlayerName, sizeof(m_PlayerName));
    memcpy(m_PlayerHandle, that.m_PlayerHandle, sizeof(m_PlayerHandle));
}

/**
 * 0x004D3E38
 */
HouseClass::~HouseClass()
{
    m_Class = nullptr;
    if (g_Session.Game_To_Play() == GAME_INTERNET) {
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

/**
 * 0x004D33A4
 */
void *HouseClass::operator new(size_t size)
{
    HouseClass *this_ptr = g_Houses.Alloc();
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = true;
    }
    return this_ptr;
}

/**
 * 0x004D33C4
 */
void HouseClass::operator delete(void *ptr)
{
    HouseClass *this_ptr = static_cast<HouseClass *>(ptr);
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = false;
    }
    g_Houses.Free(this_ptr);
}

BOOL HouseClass::Flag_Remove(target_t target, int a2)
{
#ifdef GAME_DLL
    BOOL (*func)(HouseClass *, target_t, int) = reinterpret_cast<BOOL (*)(HouseClass *, target_t, int)>(0x004D7F80);
    return func(this, target, a2);
#else
    return false;
#endif
}

BOOL HouseClass::Flag_Attach(cell_t cell, BOOL a2)
{
#ifdef GAME_DLL
    BOOL (*func)(HouseClass *, cell_t, int) = reinterpret_cast<BOOL (*)(HouseClass *, cell_t, int)>(0x004D8070);
    return func(this, cell, a2);
#else
    return false;
#endif
}

/**
 *
 *
 */
BOOL HouseClass::Flag_Attach(UnitClass *unit, int a2)
{
    if (unit != nullptr && !unit->In_Limbo()) {
        Flag_Remove(m_FlagHolder, a2);
        unit->Flag_Attach(What_Type());
        m_FlagHolder = unit->As_Target();
        return true;
    }
    return false;
}

void HouseClass::MPlayer_Defeated()
{
#ifdef GAME_DLL
    void (*func)(HouseClass *) = reinterpret_cast<void (*)(HouseClass *)>(0x004D8270);
    func(this);
#endif
}

void HouseClass::Tally_Score()
{
#ifdef GAME_DLL
    void (*func)(HouseClass *) = reinterpret_cast<void (*)(HouseClass *)>(0x004D8624);
    func(this);
#endif
}

void HouseClass::Blowup_All()
{
#ifdef GAME_DLL
    void (*func)(HouseClass *) = reinterpret_cast<void (*)(HouseClass *)>(0x004D8814);
    func(this);
#endif
}

/**
 * Sets some initial values for the house.
 *
 * 0x004D8CA8
 */
void HouseClass::Init_Data(PlayerColorType color, HousesType house, int credits)
{
    m_Static.Set_Credits(credits);
    m_Credits = credits;
    m_Color = color;
    m_ActsLike = house;
}

/**
 * Updates the amount of ore harvested.
 *
 * 0x004D5D9C
 */
void HouseClass::Harvested(unsigned amount)
{
    unsigned old_ore = m_Ore;
    m_Ore += amount;

    if (m_Ore > m_Capacity) {
        m_Ore = m_Capacity;
        m_MaxCapacity = true;
    }

    m_Harvested += amount;
    Silo_Redraw_Check(old_ore, m_Capacity);
}

/**
 * Spends an amount of money, prefers to take from ore reserves.
 *
 * 0x004D5E18
 */
void HouseClass::Spend_Money(unsigned amount)
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

/**
 * Adjusts silo capacity by provided amount.
 *
 * 0x004D5E80
 */
int HouseClass::Adjust_Capacity(int amount, BOOL cap)
{
    int lost = 0;
    unsigned old_cap = m_Capacity;
    m_Capacity = max<int>(0, old_cap + amount);

    if (m_Ore > m_Capacity) {
        lost = m_Ore - m_Capacity;
        m_Ore = m_Capacity;

        if (cap) {
            m_MaxCapacity = true;
        } else {
            Refund_Money(lost);
            lost = 0;
        }
    }

    Silo_Redraw_Check(m_Ore, old_cap);

    return lost;
}

/**
 * Checks if a new ore amount and/or capacity will require a silo redraw.
 *
 * 0x004D5EF4
 */
void HouseClass::Silo_Redraw_Check(unsigned ore, unsigned capacity)
{
    unsigned new_val = capacity != 0 ? 5 * ore / capacity : 0;
    unsigned old_val = m_Capacity != 0 ? 5 * m_Ore / m_Capacity : 0;

    if (new_val == old_val) {
        return;
    }

    for (int i = 0; i < g_Buildings.Count(); ++i) {
        BuildingClass *bptr = &g_Buildings[i];

        if (bptr != nullptr && !bptr->In_Limbo() && bptr->Get_Owner_House() == this) {
            if (bptr->What_Type() == BUILDING_SILO) {
                bptr->Mark(MARK_REDRAW);
            }
        }
    }
}

/**
 * Returns the fraction of total silo capacity that is filled with ore.
 *
 * 0x004D2C78
 */
fixed_t HouseClass::Ore_Fraction() const
{
    if (m_Ore > 0) {
        return fixed_t(m_Ore, m_Capacity);
    }

    return fixed_t(0, 0);
}

/**
 * Returns the fraction current demand that we have sufficient power for.
 *
 * 0x004D8CA8
 */
fixed_t HouseClass::Power_Fraction() const
{
    if (m_Power < m_Drain && m_Drain != 0) {
        if (m_Power != 0) {
            return fixed_t(m_Power, m_Drain);
        } else {
            return fixed_t(0);
        }
    }

    return fixed_t(1);
}

/**
 *
 *
 */
void HouseClass::Adjust_Power(int value)
{
    m_Power += value;
    Update_Spied_Power_Plants();
}

/**
 *
 *
 */
void HouseClass::Adjust_Drain(int value)
{
    m_Drain += value;
    Update_Spied_Power_Plants();
}

/**
 * Handicaps or enhances some of the houses multipliers based on difficulty.
 *
 * 0x004D2D48
 */
DiffType HouseClass::Assign_Handicap(DiffType diff)
{
    DiffType prevdiff = m_AIDifficulty;

    m_AIDifficulty = diff;

    // If the game isn't the single player campaign, apply house bonuses from ActsLike house.
    if (g_Session.Game_To_Play() != GAME_CAMPAIGN) {
        HouseTypeClass &act_like = HouseTypeClass::As_Reference(m_ActsLike);
        m_FirepowerMult = act_like.Get_Firepower() * g_Rule.Difficulty_Control(diff).Firepower;
        m_GroundspeedMult = act_like.Get_Groundspeed() * g_Rule.Difficulty_Control(diff).Groundspeed;
        m_GroundspeedMult *= g_Rule.GameSpeed_Bias();
        m_AirspeedMult = act_like.Get_Airspeed() * g_Rule.Difficulty_Control(diff).Airspeed;
        m_AirspeedMult *= g_Rule.GameSpeed_Bias();
        m_ArmorMult = act_like.Get_Armor() * g_Rule.Difficulty_Control(diff).Armor;
        m_ROFMult = act_like.Get_ROF() * g_Rule.Difficulty_Control(diff).ROF;
        m_CostMult = act_like.Get_Cost() * g_Rule.Difficulty_Control(diff).Cost;
        m_RepairDelay = g_Rule.Difficulty_Control(diff).RepairDelay;
        m_BuildDelay = g_Rule.Difficulty_Control(diff).BuildDelay;
        m_BuildTimerMult = act_like.Get_BuildTime() * g_Rule.Difficulty_Control(diff).BuildTime;
        m_BuildTimerMult *= g_Rule.GameSpeed_Bias();
    } else {
        m_FirepowerMult = g_Rule.Difficulty_Control(diff).Firepower;
        m_GroundspeedMult = g_Rule.Difficulty_Control(diff).Groundspeed;
        m_GroundspeedMult *= g_Rule.GameSpeed_Bias();
        m_AirspeedMult = g_Rule.Difficulty_Control(diff).Airspeed;
        m_AirspeedMult *= g_Rule.GameSpeed_Bias();
        m_ArmorMult = g_Rule.Difficulty_Control(diff).Armor;
        m_ROFMult = g_Rule.Difficulty_Control(diff).ROF;
        m_CostMult = g_Rule.Difficulty_Control(diff).Cost;
        m_RepairDelay = g_Rule.Difficulty_Control(diff).RepairDelay;
        m_BuildDelay = g_Rule.Difficulty_Control(diff).BuildDelay;
        m_BuildTimerMult = g_Rule.Difficulty_Control(diff).BuildTime;
        m_BuildTimerMult *= g_Rule.GameSpeed_Bias();
    }

    return prevdiff;
}

/**
 * Check if this house can build the object type. Passed HousesType is used for owner check.
 *
 * 0x004D4014
 */
BOOL HouseClass::Can_Build(TechnoTypeClass *obj, HousesType house)
{
    // Trivial check for special "Cannot build" value.
    if (obj->Get_TechLevel() == -1) {
        return false;
    }

    // Handle Aftermath units being disabled by default regardless of tech level.
    if (!g_Rule.New_Units_Enabled()) {
        switch (obj->What_Am_I()) {
            case RTTI_INFANTRYTYPE:
                if (obj->Get_Heap_ID() >= INFANTRY_NOEXP_COUNT) {
                    return false;
                }
                break;
            case RTTI_UNITTYPE:
                if (obj->Get_Heap_ID() >= UNIT_NOAM_COUNT) {
                    return false;
                }
                break;
            case RTTI_VESSELTYPE:
                if (obj->Get_Heap_ID() >= VESSEL_NOEXP_COUNT) {
                    return false;
                }
                break;
            default:
                break;
        }
    }

    // Campaign AI can build anything it wants if its scripted to do so.
    if (!m_IsHuman && g_Session.Game_To_Play() == GAME_CAMPAIGN) {
        return true;
    }

    // Check ownable against specified house, may not match our current house.
    if (obj->Get_Ownable() & (1 << house)) {
        uint32_t prereq_mask = m_IsHuman ? m_BScan.m_HumanPrereqs : m_BScan.m_AIPrereqs;

        if (prereq_mask & 0x40000) {
            prereq_mask |= 0x20000;
        }

        if (g_Session.Game_To_Play() != GAME_CAMPAIGN && (prereq_mask & 0x80001)) {
            prereq_mask |= 0x80001;
        }

        if ((obj->Get_Prerequisites() & prereq_mask) == obj->Get_Prerequisites()
            && m_Static.Get_TechLevel() >= obj->Get_TechLevel()) {
            return true;
        }
    }

    return false;
}

/**
 * Makes the given house an ally of this house, but not vice versa.
 *
 * 0x004D6060
 */
void HouseClass::Make_Ally(HousesType house)
{
    if (!Is_Allowed_To_Ally(house)) {
        return;
    }

    // Set the bit corresponding the the house to ally with.
    m_Allies |= 1 << house;

    // If we were enemies before then we are no longer.
    if (m_Enemy == house) {
        m_Enemy = HOUSES_NONE;
    }

    if (g_ScenarioInit) {
        m_Static.Add_Ally(house);
    }

    // It we aren't in a campaign, set up the alliance.
    if (g_Session.Game_To_Play() != GAME_CAMPAIGN && !g_ScenarioInit) {
        // If computer can go paranoid, it will at this point.
        if (As_Pointer(house) != nullptr && m_IsHuman && g_Rule.Computer_Paranoid()) {
            Computer_Paranoid();
        }

        // Clears existing targetting orders against units from now allied side?
        for (int index = 0; index < g_Logic.Count(); ++index) {
            ObjectClass *objptr = g_Logic[index];

            if (objptr != nullptr) {
                if (!objptr->In_Limbo() && objptr->Is_Techno()) {
                    TechnoClass *tptr = reinterpret_cast<TechnoClass *>(objptr);

                    if (tptr != nullptr && tptr->Owner() == What_Type() && As_Techno(tptr->Get_TarCom()) != nullptr) {
                        TechnoClass *tarcomptr = As_Techno(tptr->Get_TarCom());

                        if (tarcomptr != nullptr) {
                            if (Is_Ally(tarcomptr)) {
                                tptr->Assign_Target(0);
                            }
                        }
                    }
                }
            }
        }

        // Reveals around our buildings for our ally.
        if (g_Rule.Ally_Reveal() && g_PlayerPtr->What_Type() == house) {
            for (int index = 0; index < g_Buildings.Count(); ++index) {
                BuildingClass *bptr = &g_Buildings[index];

                if (bptr != nullptr && !bptr->In_Limbo()) {
                    if (bptr->Get_Owner_House() == this) {
                        g_Map.Sight_From(bptr->Center_Cell(), bptr->Class_Of().Get_Sight(), g_PlayerPtr, false);
                    }
                }
            }
        }

        // If we are human we need to be told about the alliance.
        if (m_IsHuman) {
            // Prepare and queue the display of the "Player has allied with OtherPlayer" message.
            char buff[80];
            snprintf(buff, sizeof(buff), Text_String(TXT_HAS_ALLIED), m_PlayerName, As_Pointer(house)->m_PlayerName);
            g_Session.Get_Messages().Add_Message(
                nullptr, 0, buff, m_Color, TPF_6PT_GRAD | TPF_OUTLINE | TPF_USE_GRAD_PAL, 900 * g_Rule.Message_Delay());
            g_Map.Flag_To_Redraw(false);
        }
    }
}

/**
 * Makes the given house an enemy of this house, and vice versa unlike Make_Ally.
 *
 * 0x004D6370
 */
void HouseClass::Make_Enemy(HousesType house)
{
    if (house == HOUSES_NONE) {
        return;
    }

    if (!Is_Ally(house)) {
        return;
    }

    // Clear the bit corresponding the the house to ally with.
    m_Allies &= ~(1 << house);

    if (g_ScenarioInit) {
        m_Static.Remove_Ally(house);
    }

    // Clear the new enemies alliances with us as well so we can't abuse it.
    HouseClass *hptr = As_Pointer(house);

    if (hptr != nullptr && hptr->Is_Ally(this)) {
        hptr->m_Allies &= ~(1 << m_Class->What_Type());

        if (g_ScenarioInit) {
            hptr->m_Static.Remove_Ally(m_Class->What_Type());
        }
    }

    if (g_Session.Game_To_Play() != GAME_CAMPAIGN && !g_ScenarioInit && m_IsHuman) {
        // Prepare and queue the display of the "Player has declared war on OtherPlayer" message.
        char buff[80];
        snprintf(buff, sizeof(buff), Text_String(TXT_AT_WAR), m_PlayerName, As_Pointer(house)->m_PlayerName);
        g_Session.Get_Messages().Add_Message(
            nullptr, 0, buff, m_Color, TPF_6PT_GRAD | TPF_OUTLINE | TPF_USE_GRAD_PAL, 900 * g_Rule.Message_Delay());
        g_Map.Flag_To_Redraw(false);
    }
}

/**
 * Checks if the given house is an ally of this house already.
 *
 * 0x004D5FC8
 */
BOOL HouseClass::Is_Ally(HousesType type) const
{
    if (type == HOUSES_NONE) {
        return false;
    }

    return ((1 << type) & m_Allies) != 0;
}

/**
 * Checks if the given house is an ally of this house already.
 *
 * 0x004D5FF8
 */
BOOL HouseClass::Is_Ally(HouseClass *house) const
{
    if (house == nullptr) {
        return false;
    }

    return Is_Ally(house->m_Class->Get_Type());
}

/**
 * Checks if the owner of the given object is an ally of this house already.
 *
 * 0x004D603C
 */
BOOL HouseClass::Is_Ally(ObjectClass *object) const
{
    if (object == nullptr) {
        return false;
    }

    return Is_Ally(object->Owner());
}

/**
 * Flags the house to die.
 *
 * 0x004D8B40
 */
BOOL HouseClass::Flag_To_Die()
{
    if (!m_ToWin) {
        if (!m_ToDie && !m_ToLose) {
            m_ToDie = true;
            m_BorrowedTime = (g_Rule.Savour_Delay() * 900);
        }
    }

    return m_ToDie;
}

/**
 * Flags the house to win.
 *
 * 0x004D8BB8
 */
BOOL HouseClass::Flag_To_Win()
{
    if (!m_ToWin) {
        if (!m_ToDie && !m_ToLose) {
            m_ToWin = true;
            m_BorrowedTime = (g_Rule.Savour_Delay() * 900);
        }
    }

    return m_ToWin;
}

/**
 * Flags the house to lose.
 *
 * 0x004D8C30
 */
BOOL HouseClass::Flag_To_Lose()
{
    m_ToWin = false;

    if (!m_ToDie && !m_ToLose) {
        m_ToLose = true;
        m_BorrowedTime = (g_Rule.Savour_Delay() * 900);
    }

    return m_ToLose;
}

/**
 * Adjusts the risk in a given map region.
 *
 * 0x004D658C
 */
void HouseClass::Adjust_Threat(int region, int risk)
{
    static int _val[] = { -35, -34, -33, -1, 0, 1, 33, 34, 35 };
    static int _thr[] = { 2, 1, 2, 1, 0, 1, 2, 1, 2 };
    bool reverse = false;

    if (risk >= 0) {
        reverse = false;
    } else {
        risk = -risk;
        reverse = true;
    }

    for (int i = 0; i < ARRAY_SIZE(_val); ++i) {
        m_ThreatRegions[region + _val[i]].Adjust_Value(risk >> _thr[i], reverse);
    }
}

/**
 * Decrements tracking counts for the given object.
 *
 * 0x004DCB14
 */
void HouseClass::Tracking_Remove(TechnoClass *object)
{
    switch (object->What_Am_I()) {
        case RTTI_BUILDING:
            --m_CurrentBuildingCount;
            --m_BuildingQuantity[reinterpret_cast<const BuildingTypeClass &>(object->Class_Of()).What_Type()];
            break;

        case RTTI_AIRCRAFT:
            --m_CurrentAircraftCount;
            --m_AircraftQuantity[reinterpret_cast<const AircraftTypeClass &>(object->Class_Of()).What_Type()];
            break;

        case RTTI_INFANTRY:
            --m_CurrentInfantryCount;
            --m_InfantryQuantity[reinterpret_cast<const InfantryTypeClass &>(object->Class_Of()).What_Type()];
            break;

        case RTTI_UNIT:
            --m_CurrentUnitCount;
            --m_UnitQuantity[reinterpret_cast<const UnitTypeClass &>(object->Class_Of()).What_Type()];
            break;

        case RTTI_VESSEL:
            --m_CurrentVesselCount;
            --m_VesselQuantity[reinterpret_cast<const VesselTypeClass &>(object->Class_Of()).What_Type()];
            break;

        default:
            break;
    };
}

/**
 * Increments tracking counts for the given object.
 *
 * 0x004DCC30
 */
void HouseClass::Tracking_Add(TechnoClass *object)
{
    switch (object->What_Am_I()) {
        case RTTI_BUILDING:
            ++m_CurrentBuildingCount;
            ++m_BuildingQuantity[reinterpret_cast<const BuildingTypeClass &>(object->Class_Of()).What_Type()];

            if (g_Session.Game_To_Play() == GAME_INTERNET) {
                m_BScan.m_HaveBuilt |= 1 << reinterpret_cast<const BuildingTypeClass &>(object->Class_Of()).What_Type();
            }
            break;

        case RTTI_AIRCRAFT:
            ++m_CurrentAircraftCount;
            ++m_AircraftQuantity[reinterpret_cast<const AircraftTypeClass &>(object->Class_Of()).What_Type()];

            if (g_Session.Game_To_Play() == GAME_INTERNET) {
                m_AScan.m_HaveBuilt |= 1 << reinterpret_cast<const AircraftTypeClass &>(object->Class_Of()).What_Type();
            }
            break;

        case RTTI_INFANTRY:
            ++m_CurrentInfantryCount;
            ++m_InfantryQuantity[reinterpret_cast<const InfantryTypeClass &>(object->Class_Of()).What_Type()];

            if (g_Session.Game_To_Play() == GAME_INTERNET) {
                m_IScan.m_HaveBuilt |= 1 << reinterpret_cast<const InfantryTypeClass &>(object->Class_Of()).What_Type();
            }
            break;

        case RTTI_UNIT:
            ++m_CurrentUnitCount;
            ++m_UnitQuantity[reinterpret_cast<const UnitTypeClass &>(object->Class_Of()).What_Type()];

            if (g_Session.Game_To_Play() == GAME_INTERNET) {
                m_UScan.m_HaveBuilt |= 1 << reinterpret_cast<const UnitTypeClass &>(object->Class_Of()).What_Type();
            }
            break;

        case RTTI_VESSEL:
            ++m_CurrentVesselCount;
            ++m_VesselQuantity[reinterpret_cast<const VesselTypeClass &>(object->Class_Of()).What_Type()];

            if (g_Session.Game_To_Play() == GAME_INTERNET) {
                m_VScan.m_HaveBuilt |= 1 << reinterpret_cast<const VesselTypeClass &>(object->Class_Of()).What_Type();
            }
            break;

        default:
            break;
    };
}

/**
 * Retrieves a pointer to the factory counter for the given type.
 *
 * 0x004DCEB4
 */
int *HouseClass::Factory_Counter(RTTIType type)
{
    switch (type) {
        case RTTI_AIRCRAFT:
        case RTTI_AIRCRAFTTYPE: // Fallthrough
            return &m_AircraftFactoryCount;
        case RTTI_BUILDING:
        case RTTI_BUILDINGTYPE: // Fallthrough
            return &m_BuildingFactoryCount;
        case RTTI_INFANTRY:
        case RTTI_INFANTRYTYPE: // Fallthrough
            return &m_InfantryFactoryCount;
        case RTTI_UNIT:
        case RTTI_UNITTYPE: // Fallthrough
            return &m_UnitFactoryCount;
        case RTTI_VESSEL:
        case RTTI_VESSELTYPE: // Fallthrough
            return &m_VesselFactoryCount;

        default:
            return nullptr;
    };
}

/**
 * Retrieves the current count of a given object type.
 *
 * 0x004DDCD0
 */
int HouseClass::Factory_Count(RTTIType type)
{
    int *counter = Factory_Counter(type);

    return counter != nullptr ? *counter : 0;
}

/**
 * Decrements the factory count for the type of object the given object builds.
 *
 * 0x004DCF1C
 */
void HouseClass::Active_Remove(TechnoClass *object)
{
    if (object->What_Am_I() == RTTI_BUILDING) {
        int *counter = Factory_Counter(reinterpret_cast<BuildingClass *>(object)->Class_Of().Factory_Type());

        if (counter != nullptr) {
            --(*counter);
        }
    }
}

/**
 * Increments the factory count for the type of object the given object builds.
 *
 * 0x004DCF6C
 */
void HouseClass::Active_Add(TechnoClass *object)
{
    if (object->What_Am_I() == RTTI_BUILDING) {
        int *counter = Factory_Counter(reinterpret_cast<BuildingClass *>(object)->Class_Of().Factory_Type());

        if (counter != nullptr) {
            ++(*counter);
        }
    }
}

/**
 * Fetches a pointer to this houses FactoryClass objects for the given type.
 *
 * @param rtti The RTTIType to fetch a factory for.
 *
 * 0x004DDBD0
 */
FactoryClass *HouseClass::Fetch_Factory(RTTIType rtti) const
{
    switch (rtti) {
        case RTTI_AIRCRAFT:
        case RTTI_AIRCRAFTTYPE: // Fallthrough
            return m_AircraftFactory;
        case RTTI_BUILDING:
        case RTTI_BUILDINGTYPE: // Fallthrough
            return m_BuildingFactory;
        case RTTI_INFANTRY:
        case RTTI_INFANTRYTYPE: // Fallthrough
            return m_InfantryFactory;
        case RTTI_UNIT:
        case RTTI_UNITTYPE: // Fallthrough
            return m_UnitFactory;
        case RTTI_VESSEL:
        case RTTI_VESSELTYPE: // Fallthrough
            return m_VesselFactory;
        default:
            break;
    }

    return nullptr;
}

/**
 * Sets the pointer for a factory for the given type.
 *
 * @param rtti The RTTIType to set a factory for.
 * @param factory Pointer to the factory to set.
 *
 * 0x004DDC58
 */
void HouseClass::Set_Factory(RTTIType rtti, FactoryClass *factory)
{
    switch (rtti) {
        case RTTI_AIRCRAFT:
        case RTTI_AIRCRAFTTYPE: // Fallthrough
            m_AircraftFactory = factory;
            break;
        case RTTI_BUILDING:
        case RTTI_BUILDINGTYPE: // Fallthrough
            m_BuildingFactory = factory;
            break;
        case RTTI_INFANTRY:
        case RTTI_INFANTRYTYPE: // Fallthrough
            m_InfantryFactory = factory;
            break;
        case RTTI_UNIT:
        case RTTI_UNITTYPE: // Fallthrough
            m_UnitFactory = factory;
            break;
        case RTTI_VESSEL:
        case RTTI_VESSELTYPE: // Fallthrough
            m_VesselFactory = factory;
            break;
        default:
            break;
    }
}

/**
 * Retrieves the remap table to use for objects belonging to this house.
 *
 * @param alt Retrieves DisplayClass::s_FadingLight instead of the remap table.
 * @param type Type of 0 returns a null pointer instead of remap table.
 *
 * 0x004D6528
 */
const uint8_t *HouseClass::Remap_Table(BOOL alt, RemapType type)
{
    if (alt) {
        // TODO Make fading table types consistent across project.
        return DisplayClass::s_FadingLight;
    }

    if (type == REMAP_0) {
        return nullptr;
    }
    return g_ColorRemaps[m_Color].RemapPalette;
}

/**
 * Performs a sell on the wall overlay at the given cell number.
 *
 * 0x004D8D1C
 */
void HouseClass::Sell_Wall(cell_t cellnum)
{
    if (cellnum != 0) {
        CellClass &cell = g_Map[cellnum];

        if (cell.Get_Overlay() != OVERLAY_NONE && cell.Owner() == What_Type()) {
            OverlayTypeClass &overlay = OverlayTypeClass::As_Reference(cell.Get_Overlay());

            if (overlay.Is_Wall()) {
                BuildingTypeClass *bptr = nullptr;

                switch (cell.Get_Overlay()) {
                    case OVERLAY_CYCLONE_FENCE:
                        bptr = &BuildingTypeClass::As_Reference(BUILDING_CYCL);
                        break;

                    case OVERLAY_SANDBAG:
                        bptr = &BuildingTypeClass::As_Reference(BUILDING_SBAG);
                        break;

                    case OVERLAY_BRICK_WALL:
                        bptr = &BuildingTypeClass::As_Reference(BUILDING_BRIK);
                        break;

                    case OVERLAY_BARB_WIRE:
                        bptr = &BuildingTypeClass::As_Reference(BUILDING_BARB);
                        break;

                    case OVERLAY_WOOD_FENCE:
                        bptr = &BuildingTypeClass::As_Reference(BUILDING_WOOD);
                        break;

                    case OVERLAY_FENCE:
                        bptr = &BuildingTypeClass::As_Reference(BUILDING_FENC);
                        break;

                    default:
                        break;
                };

                if (bptr != nullptr && !bptr->Unsellable()) {
                    if (Is_Player()) {
                        Sound_Effect(VOC_CASHTURN);
                    }

                    Refund_Money(bptr->Raw_Cost() * g_Rule.Refund_Percent());
                    // Original fetched same cell reference again here, unneeded? Inlined function?
                    cell.Set_Overlay(OVERLAY_NONE);
                    cell.Set_Overlay_Frame(0);
                    cell.Set_Owner(HOUSES_NONE);
                    cell.Wall_Update();
                    cell.Recalc_Attributes();
                    // Fetched again here.
                    cell.Redraw_Objects();
                    g_Map.Radar_Pixel(cellnum);
                    ObjectClass::Detach_This_From_All(As_Target(cellnum), 1);
                    g_Map.Zone_Reset(overlay.Is_Crushable() ? ZONE_NORTH : ZONE_SOUTH);
                }
            }
        }
    }
}

const BuildingTypeClass *HouseClass::Suggest_New_Building() const
{
    if (m_ChosenBuilding != BUILDING_NONE) {
        return BuildingTypeClass::As_Pointer(m_ChosenBuilding);
    }
    return nullptr;
}

void HouseClass::Decode_Pointers()
{
    Init_Data(m_Color, m_ActsLike, m_Credits);
}

/**
 * Find a building matching the required type and zone that belongs to this house.
 *
 * 0x004D8F34
 */
BuildingClass *HouseClass::Find_Building(BuildingType to_find, ZoneType zone) const
{
    if (m_BuildingQuantity[to_find] <= 0) {
        return nullptr;
    }

    for (int i = 0; i < g_Buildings.Count(); ++i) {
        BuildingClass *bptr = &g_Buildings[i];

        if (bptr != nullptr && !bptr->In_Limbo() && bptr->Get_Owner_House() == this) {
            if (bptr->What_Type() == to_find && (zone == ZONE_NONE || zone == Which_Zone(bptr))) {
                return bptr;
            }
        }
    }

    return nullptr;
}

/**
 * Find a suitable location to place a building.
 *
 * 0x004D9020
 */
coord_t HouseClass::Find_Build_Location(BuildingClass *building) const
{
    DEBUG_ASSERT(building != nullptr);
    static ZoneType _zones[ZONE_COUNT] = { ZONE_CORE, ZONE_NORTH, ZONE_SOUTH, ZONE_EAST, ZONE_WEST };
    ZoneInfo zoneinfo = { 0, 0, 0 };
    int zone_scores[ZONE_COUNT];
    int anti_air = building->Anti_Air();
    int anti_armor = building->Anti_Armor();
    int anti_inf = building->Anti_Infantry();

    // Sum up and average the scores outside of the core zone.
    for (ZoneType zone = ZONE_NORTH; zone < ZONE_COUNT; ++zone) {
        zoneinfo.Air += m_Zones[zone].Air;
        zoneinfo.Armor += m_Zones[zone].Armor;
        zoneinfo.Infantry += m_Zones[zone].Infantry;
    }

    zoneinfo.Air /= 4;
    zoneinfo.Armor /= 4;
    zoneinfo.Infantry /= 4;
    memset(zone_scores, 0, sizeof(zone_scores));

    // Add to the zone score if average beats the actual for that zone.
    for (ZoneType zone = ZONE_CORE; zone < ZONE_COUNT; ++zone) {
        int diff = zoneinfo.Air - m_Zones[zone].Air;

        if (zone == ZONE_CORE) {
            diff /= 2;
        }

        if (diff > 0) {
            zone_scores[zone] += min(diff, anti_air);
        }

        diff = zoneinfo.Armor - m_Zones[zone].Armor;

        if (zone == ZONE_CORE) {
            diff /= 2;
        }

        if (diff > 0) {
            zone_scores[zone] += min(diff, anti_armor);
        }

        diff = zoneinfo.Infantry - m_Zones[zone].Infantry;

        if (zone == ZONE_CORE) {
            diff /= 2;
        }

        if (diff > 0) {
            zone_scores[zone] += min(diff, anti_inf);
        }
    }

    // Use the zone scores to determine which zone is best, using a random one if none are.
    ZoneType find_zone = (ZoneType)g_Scen.Get_Random_Value(ZONE_CORE, ZONE_COUNT - 1);
    int last_score = 0;

    for (ZoneType zone = ZONE_CORE; zone < ZONE_COUNT; ++zone) {
        if (last_score < zone_scores[zone]) {
            find_zone = zone;
            last_score = zone_scores[zone];
        }
    }

    // Find a cell in the zone and return is as a coord.
    cell_t zone_cell = Find_Cell_In_Zone(building, find_zone);

    if (Valid_Cell(zone_cell)) {
        return Cell_To_Coord(zone_cell);
    } else {
        for (ZoneType zone = (ZoneType)g_Scen.Get_Random_Value(ZONE_FIRST, ZONE_COUNT - 1); zone < ZONE_COUNT; ++zone) {
            cell_t zonecell = Find_Cell_In_Zone(building, _zones[zone % ZONE_COUNT]);

            if (Valid_Cell(zonecell)) {
                return Cell_To_Coord(zonecell);
            }
        }
    }

    return 0;
}

/**
 * Find a cell in a given zone that can hold the given object.
 *
 * 0x004DE81C
 */
cell_t HouseClass::Find_Cell_In_Zone(TechnoClass *object, ZoneType zone) const
{
    if (object != nullptr) {
        int last_distance = -1;
        cell_t zone_cell = 0;
        const int16_t *olist = nullptr;
        cell_t random_cell = Random_Cell_In_Zone(zone);
        const TechnoTypeClass &tech_type = reinterpret_cast<const TechnoTypeClass &>(object->Class_Of());

        if (object->What_Am_I() == RTTI_BUILDING) {
            olist = object->Occupy_List(true);
        }

        for (cell_t cellnum = 0; cellnum < MAP_MAX_AREA; ++cellnum) {
            if (g_Map.In_Radar(cellnum) && Which_Zone(cellnum) != ZONE_NONE) {
                bool can_place = tech_type.Legal_Placement(cellnum);

                if (can_place && olist != nullptr) {
                    // Cast away const here, function perhaps should take const ObjectTypeClass  and const int16_t* instead?
                    if (!g_Map.Passes_Proximity_Check((ObjectTypeClass *)&tech_type, What_Type(), (int16_t *)olist, cellnum)) {
                        can_place = false;
                    }
                }

                if (can_place) {
                    int distance = Distance(Cell_To_Coord(cellnum), Cell_To_Coord(random_cell));

                    if (last_distance == -1 || distance < last_distance) {
                        last_distance = distance;
                        zone_cell = cellnum;
                    }
                }
            }
        }

        return zone_cell;
    }

    return 0;
}

/**
 * Gets a random cell from the given zone.
 *
 * 0x004DE9C0
 */
cell_t HouseClass::Random_Cell_In_Zone(ZoneType zone) const
{
    DEBUG_ASSERT(zone != ZONE_NONE);
    DEBUG_ASSERT(zone < ZONE_COUNT);

    coord_t random_coord = 0;

    switch (zone) {
        case ZONE_CORE: {
            random_coord = Coord_Scatter(m_BaseCenter, g_Scen.Get_Random_Value(0, m_Radius), true);
            break;
        }

        case ZONE_NORTH: {
            lepton_t map_top = Coord_Cell_To_Lepton(g_Map.Get_Map_Cell_Y());
            int bound = min(3 * m_Radius, Coord_Lepton_Y(m_BaseCenter) - map_top - 256);

            if (bound >= 0) {
                int upper_bound = min(bound, 3 * m_Radius);
                int lower_bound = min(bound, 2 * m_Radius);
                random_coord = Coord_Move(m_BaseCenter,
                    (DirType)g_Scen.Get_Random_Value(DIR_NORTH, DIR_EAST) - 32,
                    g_Scen.Get_Random_Value(lower_bound, upper_bound));
            }
            break;
        }

        case ZONE_EAST: {
            lepton_t map_left = Coord_Cell_To_Lepton(g_Map.Get_Map_Cell_X());
            int bound = min(3 * m_Radius, Coord_Lepton_X(m_BaseCenter) - map_left - 256);

            if (bound >= 0) {
                int upper_bound = min(bound, 3 * m_Radius);
                int lower_bound = min(bound, 2 * m_Radius);
                random_coord = Coord_Move(m_BaseCenter,
                    (DirType)g_Scen.Get_Random_Value(DIR_SOUTH_EAST, DIR_SOUTH_WEST),
                    g_Scen.Get_Random_Value(lower_bound, upper_bound));
            }
            break;
        }

        case ZONE_SOUTH: {
            lepton_t map_top = Coord_Cell_To_Lepton(g_Map.Get_Map_Cell_Y());
            int bound = min(3 * m_Radius, Coord_Lepton_Y(m_BaseCenter) - map_top - 256);

            if (bound >= 0) {
                int upper_bound = min(bound, 3 * m_Radius);
                int lower_bound = min(bound, 2 * m_Radius);
                random_coord = Coord_Move(m_BaseCenter,
                    (DirType)g_Scen.Get_Random_Value(DIR_SOUTH_EAST, DIR_SOUTH_WEST),
                    g_Scen.Get_Random_Value(lower_bound, upper_bound));
            }
            break;
        }

        case ZONE_WEST: {
            lepton_t map_left = Coord_Cell_To_Lepton(g_Map.Get_Map_Cell_X());
            int bound = min(3 * m_Radius, Coord_Lepton_X(m_BaseCenter) - map_left - 256);

            if (bound >= 0) {
                int upper_bound = min(bound, 3 * m_Radius);
                int lower_bound = min(bound, 2 * m_Radius);
                random_coord = Coord_Move(m_BaseCenter,
                    (DirType)g_Scen.Get_Random_Value(DIR_SOUTH_WEST, DIR_NORTH_WEST),
                    g_Scen.Get_Random_Value(lower_bound, upper_bound));
            }
            break;
        }

        default:
            break;
    };

    cell_t random_cell = 0;

    if (Valid_Cell(random_coord) && g_Map.In_Radar(Coord_To_Cell(random_coord))) {
        random_cell = Coord_To_Cell(random_coord);
    } else {
        if (zone != ZONE_CORE) {
            random_cell = Random_Cell_In_Zone(ZONE_CORE);
        } else {
            random_cell = Coord_To_Cell(m_BaseCenter);
        }
    }

    if (!g_Map.In_Radar(random_cell)) {
        random_cell = g_Map.Clamp_To_Radar(random_cell);
    }

    return random_cell;
}

/**
 * Determines which zone an object is in.
 *
 * 0x004DD07C
 */
ZoneType HouseClass::Which_Zone(ObjectClass *object) const
{
    if (object != nullptr) {
        return Which_Zone(object->Center_Coord());
    }

    return ZONE_NONE;
}

/**
 * Determines which zone a cell is in.
 *
 * 0x004DD0A4
 */
ZoneType HouseClass::Which_Zone(cell_t cellnum) const
{
    return Which_Zone(Cell_To_Coord(cellnum));
}

/**
 * Determines which zone coord is in.
 *
 * 0x004DCFBC
 */
ZoneType HouseClass::Which_Zone(coord_t coord) const
{
    if (coord != 0) {
        int distance = Distance(m_BaseCenter, coord);

        if (distance <= m_Radius) {
            return ZONE_CORE;
        }

        if (distance > (m_Radius * 4)) {
            return ZONE_NONE;
        }

        DirType facing = Desired_Facing256(
            Coord_Lepton_X(m_BaseCenter), Coord_Lepton_Y(m_BaseCenter), Coord_Lepton_X(coord), Coord_Lepton_Y(coord));

        if (facing < DIR_NORTH_EAST || facing > DIR_NORTH_WEST) {
            return ZONE_NORTH;
        }

        if (facing >= DIR_SOUTH_EAST && facing < DIR_SOUTH_WEST) {
            return ZONE_SOUTH;
        }

        if (facing >= DIR_NORTH_EAST && facing < DIR_SOUTH_EAST) {
            return ZONE_EAST;
        }

        if (facing >= DIR_SOUTH_WEST && facing < DIR_NORTH_WEST) {
            return ZONE_WEST;
        }
    }

    return ZONE_NONE;
}

void HouseClass::Attacked()
{
#ifdef GAME_DLL
    void (*func)(HouseClass *) = reinterpret_cast<void (*)(HouseClass *)>(0x004D5C1C);
    func(this);
#endif
}

const TeamTypeClass *HouseClass::Suggested_New_Team(int a1)
{
#ifdef GAME_DLL
    const TeamTypeClass *(*func)(HouseClass *, int) = reinterpret_cast<const TeamTypeClass *(*)(HouseClass *, int)>(0x004D6560);
    return func(this, a1);
#else
    return nullptr;
#endif
}

/**
 *
 *
 */
ProdFailType HouseClass::Begin_Production(RTTIType rtti, int id)
{
    TechnoTypeClass *techtype = Fetch_Techno_Type(rtti, id);
    BOOL set = true;
    FactoryClass *fptr = Fetch_Factory(rtti);
    if (fptr != nullptr) {
        if (fptr->Is_Building()){
            DEBUG_LOG("Request to Begin_Production of '%s' was rejected. Cannot queue, factory is busy.\n", techtype->Get_Name());
            return PROD_REJECTED;
        }
    } else {
        fptr = new FactoryClass;
        if (fptr == nullptr) {
            DEBUG_LOG("Request to Begin_Production of '%s' was rejected. Unable to create factory\n", techtype->Get_Name());
            return PROD_REJECTED;
        }
        Set_Factory(rtti, fptr);
        set = fptr->Set(*techtype, *this);
    }

    if (set == false) {
        DEBUG_LOG("Request to Begin_Production of '%s' was rejected. Factory was unable to create the requested object\n",
            techtype->Get_Name());
        if (fptr != nullptr) {
            delete fptr;
        }
        return PROD_REJECTED;
    }
    fptr->Start();
    if (Is_Player()) {
        g_Map.Factory_Link(fptr->Get_Heap_ID(), rtti, id);
    }
    return PROD_APPROVED;
}

/**
 *
 *
 */
ProdFailType HouseClass::Suspend_Production(RTTIType rtti)
{
    FactoryClass *fptr = Fetch_Factory(rtti);
    if (fptr == nullptr) {
        return PROD_REJECTED;
    }

    fptr->Suspend();
    if (Is_Player()) {
        g_Map.Flag_Sidebar_To_Redraw();
        g_Map.Flag_To_Redraw();
    }
    return PROD_APPROVED;
}

/**
 *
 *
 */
ProdFailType HouseClass::Abandon_Production(RTTIType rtti)
{
    FactoryClass *fptr = Fetch_Factory(rtti);
    if (fptr == nullptr) {
        return PROD_REJECTED;
    }
    if (Is_Player()) {
        g_Map.Abandon_Production(rtti, fptr->Get_Heap_ID());
        if (rtti == RTTI_BUILDINGTYPE || rtti == RTTI_BUILDING) {
            g_Map.Reset_Pending_Object(false);
            g_Map.Set_Cursor_Shape();
        }
    }
    fptr->Abandon();
    Set_Factory(rtti, nullptr);
    if (fptr != nullptr) {
        delete fptr;
    }
    return PROD_APPROVED;
}

void HouseClass::Production_Begun(TechnoClass *object)
{
    switch (object->What_Am_I()) {
        case RTTI_AIRCRAFT:
            if (reinterpret_cast<AircraftClass *>(object)->What_Type() == m_ChosenAircraft) {
                m_ChosenAircraft = AIRCRAFT_NONE;
            }
            break;
        case RTTI_BUILDING:
            if (reinterpret_cast<BuildingClass *>(object)->What_Type() == m_ChosenBuilding) {
                m_ChosenBuilding = BUILDING_NONE;
            }
            break;
        case RTTI_INFANTRY:
            if (reinterpret_cast<InfantryClass *>(object)->What_Type() == m_ChosenInfantry) {
                m_ChosenInfantry = INFANTRY_NONE;
            }
            break;
        case RTTI_UNIT:
            if (reinterpret_cast<UnitClass *>(object)->What_Type() == m_ChosenUnit) {
                m_ChosenUnit = UNIT_NONE;
            }
            break;
        default:
            DEBUG_LOG("HouseClass::Production_Begun got unsupported RTTI! %d\n", object->What_Am_I());
            break;
    }
}

void HouseClass::Recalc_Attributes()
{
#ifdef GAME_DLL
    void (*func)(HouseClass *) = reinterpret_cast<void (*)(HouseClass *)>(0x004DD0DC);
    func(this);
#endif
}

/**
 * Gets the most extreme cell of a houses zone.
 *
 * 0x004DD920
 */
cell_t HouseClass::Zone_Cell(ZoneType zone) const
{
    DEBUG_ASSERT(zone != ZONE_NONE);
    DEBUG_ASSERT(zone < ZONE_COUNT);

    switch (zone) {
        case ZONE_CORE:
            return Coord_To_Cell(m_BaseCenter);

        case ZONE_NORTH:
            return Coord_To_Cell(Coord_Move(m_BaseCenter, DIR_NORTH, m_Radius * 3));

        case ZONE_EAST:
            return Coord_To_Cell(Coord_Move(m_BaseCenter, DIR_EAST, m_Radius * 3));

        case ZONE_SOUTH:
            return Coord_To_Cell(Coord_Move(m_BaseCenter, DIR_SOUTH, m_Radius * 3));

        case ZONE_WEST: {
            return Coord_To_Cell(Coord_Move(m_BaseCenter, DIR_WEST, m_Radius * 3));
        }

        default:
            break;
    }

    return 0;
}

/**
 * Decide which cell to go to.
 *
 * 0x004DD9FC
 */
cell_t HouseClass::Where_To_Go(FootClass *obj) const
{
    DEBUG_ASSERT(obj != nullptr);

    ZoneType zone;

    int value = obj->Anti_Air();
    value += obj->Anti_Armor();
    value += obj->Anti_Infantry();

    if (value > 0) {
        zone = (ZoneType)g_Scen.Get_Random_Value(ZONE_FIRST, ZONE_COUNT - 1);
    } else {
        zone = ZONE_CORE;
    }

    cell_t cellnum = Random_Cell_In_Zone(zone);

    return g_Map.Nearby_Location(cellnum, SPEED_TRACK, g_Map[cellnum].Get_Zone(MZONE_NORMAL), MZONE_NORMAL);
}

/**
 * Pick a vulnerable target belonging to one of this houses enemies close to the provided coord.
 *
 * 0x004DDA80
 */
target_t HouseClass::Find_Juicy_Target(coord_t coord) const
{
    AbstractClass *targptr = nullptr;
    int last_dist = 0;

    for (int i = 0; i < g_Units.Count(); ++i) {
        UnitClass *uptr = &g_Units[i];

        // If we have a valid unit that doesn't belong to an ally, check if its out of the houses zone
        // and thus ripe for the taking.
        if (uptr != nullptr && !uptr->In_Limbo() && !Is_Ally(uptr)) {
            if (uptr->Get_Owner_House()->Which_Zone(uptr) == ZONE_NONE) {
                int dist = Distance(uptr->Center_Coord(), coord);

                // Give anti air units a wide berth.
                if (uptr->Anti_Air()) {
                    dist *= 2;
                }

                // Prioritise going after those sweet sweet harvesters.
                if (uptr->What_Type() == UNIT_HARVESTER) {
                    dist /= 2;
                }

                if (last_dist == 0 || dist < last_dist) {
                    last_dist = dist;
                    targptr = uptr;
                }
            }
        }
    }

    return targptr != nullptr ? targptr->As_Target() : 0;
}

/**
 * Check if all our airfields are spoken for so can't build any more planes.
 *
 * 0x004DE094
 */
BOOL HouseClass::Is_No_Yak_Mig() const
{
    // Get a count of how many airfield bound aircraft we have.
    int count = m_AircraftQuantity[AIRCRAFT_YAK] + m_AircraftQuantity[AIRCRAFT_MIG];

    // Fetch the primary factory for Aircrafts.
    FactoryClass *fptr = Fetch_Factory(RTTI_AIRCRAFT);

    // Work out if we are building an aircraft so it technically doesn't exist yet.
    if (fptr != nullptr) {
        if (fptr->Get_Object()) {
            AircraftClass *aptr = reinterpret_cast<AircraftClass *>(fptr->Get_Object());
            DEBUG_ASSERT(aptr != nullptr);

            if (aptr->What_Type() == AIRCRAFT_MIG || aptr->What_Type() == AIRCRAFT_YAK) {
                --count;
            }
        }
    }

    // If we have more than or equal aircraft to airfields, we can't build any more.
    return count >= m_BuildingQuantity[BUILDING_AIRFIELD];
}

/**
 * From the name and usage this appears to be for preventing building too many planes.
 *
 * 0x004DE154
 */
BOOL HouseClass::Is_Hack_Prevented(RTTIType rtti, int id) const
{
    if (rtti == RTTI_AIRCRAFT && (id == AIRCRAFT_MIG || id == AIRCRAFT_YAK)) {
        return Is_No_Yak_Mig();
    }

    return false;
}

void HouseClass::Update_Spied_Power_Plants()
{
#ifdef GAME_DLL
    void (*func)(HouseClass *) = reinterpret_cast<void (*)(HouseClass *)>(0x004DE710);
    func(this);
#endif
}

void HouseClass::AI()
{
#ifdef GAME_DLL
    void (*func)(HouseClass *) = reinterpret_cast<void (*)(HouseClass *)>(0x004D4134);
    func(this);
#endif
}

/**
 *
 *
 */
void HouseClass::Recalc_Center()
{
    m_BaseCenter = 0;
    m_Radius = 0;

    for (int i = 0; i < ZONE_COUNT; ++i) {
        m_Zones[i].Air = 0;
        m_Zones[i].Armor = 0;
        m_Zones[i].Infantry = 0;
    }
    if (m_CurrentBuildingCount > 0) {
        int coord_x = 0;
        int coord_y = 0;
        int calc = 0;

        for (int i = 0; i < g_Buildings.Count(); ++i) {
            BuildingClass *bptr = &g_Buildings[i];
            if (bptr != nullptr && !bptr->In_Limbo() && bptr->Get_Owner_House() == this && bptr->Get_Health() > 0) {
                int cost = bptr->Class_Of().Get_Cost() / 1000 + 1;
                for (int k = 0; k < cost; ++k) {
                    coord_x += Coord_Lepton_X(bptr->Center_Coord());
                    coord_y += Coord_Lepton_Y(bptr->Center_Coord());
                    ++calc;
                }
            }
        }

        if (calc > 0) {
            m_BaseCenter = Coord_From_Lepton_XY(coord_x, coord_y);
        }

        if (calc > 1) {
            int dist = 0;
            for (int i = 0; i < g_Buildings.Count(); ++i) {
                BuildingClass *bptr = &g_Buildings[i];
                if (bptr != nullptr && !bptr->In_Limbo() && bptr->Get_Owner_House() == this && bptr->Get_Health() > 0) {
                    dist += Distance(m_BaseCenter, bptr->Center_Coord());
                }
            }
            m_Radius = std::max(dist / calc, 512);

            for (int i = 0; i < g_Buildings.Count(); ++i) {
                BuildingClass *bptr = &g_Buildings[i];
                if (bptr != nullptr && !bptr->In_Limbo() && bptr->Get_Owner_House() == this && bptr->Get_Health() > 0) {
                    ZoneType zone = Which_Zone(bptr);
                    if (zone != ZONE_NONE) {
                        m_Zones[zone].Air += bptr->Anti_Armor();
                        m_Zones[zone].Armor += bptr->Anti_Air();
                        m_Zones[zone].Infantry += bptr->Anti_Infantry();
                    }
                }
            }
        } else {
            m_Radius = 512;
        }
    }
}

int HouseClass::Expert_AI()
{
#ifdef GAME_DLL
    int (*func)(HouseClass *) = reinterpret_cast<int (*)(HouseClass *)>(0x004D9610);
    return func(this);
#else
    return 0;
#endif
}

/**
 * Performs one time intialisation that supports this class.
 *
 * 0x004D2D28
 */
void HouseClass::One_Time()
{
    g_BuildChoice.Set_Heap(BUILDING_COUNT);
}

/**
 * Performs intialisation that supports this class.
 *
 * 0x004D40F4
 */
void HouseClass::Init()
{
    g_Houses.Free_All();

    for (HousesType i = HOUSES_FIRST; i < HOUSES_COUNT; ++i) {
        g_HouseTriggers[i].Clear();
    }
}

/**
 * Converts a HousesType to a HouseClass reference.
 */
HouseClass &HouseClass::As_Reference(HousesType type)
{
    return *As_Pointer(type);
}

/**
 * Converts a HousesType to a HouseClass pointer.
 *
 * 0x004D2CB0
 */
HouseClass *HouseClass::As_Pointer(HousesType type)
{
    if (type != HOUSES_NONE && type < HOUSES_COUNT) {
        for (int index = 0; index < g_Houses.Count(); ++index) {
            if (g_Houses[index].What_Type() == type) {
                return &g_Houses[index];
            }
        }
    }

    return nullptr;
}

/**
 * Makes AI players go into paranoid mode, allying with each other against human players.
 *
 * 0x004DE640
 */
void HouseClass::Computer_Paranoid()
{
    for (HousesType i = HOUSES_MULTI_FIRST; i < HOUSES_COUNT; ++i) {
        HouseClass *hptr = HouseClass::As_Pointer(i);

        if (hptr != nullptr && hptr->m_IsActive && !hptr->m_Defeated && !hptr->m_IsHuman) {
            hptr->m_Paranoid = true;

            for (HousesType j = HOUSES_MULTI_FIRST; j < HOUSES_COUNT; ++j) {
                HouseClass *hptr2 = HouseClass::As_Pointer(j);

                if (hptr2 != nullptr && hptr2->m_IsActive && !hptr2->m_Defeated) {
                    if (hptr2->m_IsHuman) {
                        hptr->Make_Enemy(j);
                    } else {
                        hptr->Make_Ally(j);
                    }
                }
            }
        }
    }
}

/**
 * Read in houses from an ini file.
 *
 * 0x004DDCFC
 */
void HouseClass::Read_INI(GameINIClass &ini)
{
    for (HousesType house = HOUSES_FIRST; house < HOUSES_COUNT; ++house) {
        const char *house_name = HouseTypeClass::Name_From(house);
        HouseClass *hptr = new HouseClass(house);
        hptr->m_Static.Read_INI(ini, house_name);

        // Get House IQ and Credits from Static data just loaded.
        hptr->m_CurrentIQ = hptr->m_Static.Get_IQ();
        hptr->m_Credits = hptr->m_Static.Get_Credits();

        // Does this house have player control?
        hptr->m_PlayerControl = ini.Get_Bool(house_name, "PlayerControl", hptr->m_PlayerControl);
        int owners_bit = ini.Get_Owners(house_name, "Allies", OWNER_NEUTRAL);
        hptr->Make_Ally(house);
        hptr->Make_Ally(HOUSES_NEUTRAL);

        for (HousesType ally = HOUSES_FIRST; ally < HOUSES_COUNT; ++ally) {
            if ((1 << ally) & owners_bit) {
                hptr->Make_Ally(ally);
            }
        }
    }
}

/**
 * Write out houses to an ini file.
 *
 * 0x004DDEB0
 */
void HouseClass::Write_INI(GameINIClass &ini)
{
    for (HousesType house = HOUSES_FIRST; house < HOUSES_COUNT; ++house) {
        char const *house_name = HouseTypeClass::Name_From(house);
        ini.Clear(house_name);

        // Don't store multiplayer houses.
        if (house < HOUSES_MULTI_FIRST) {
            HouseClass *hptr = As_Pointer(house);

            if (hptr != nullptr) {
                hptr->m_Static.Write_INI(ini, house_name);

                if (hptr->m_PlayerControl && hptr != g_PlayerPtr) {
                    ini.Put_Bool(house_name, "PlayerControl", hptr->m_PlayerControl);
                }

                // Remove ourself and Civilians from the list of allies since they are always
                // set as allies by the read function and don't need to be stored.
                int allies_mask = (1 << hptr->What_Type());
                allies_mask |= OWNER_NEUTRAL;
                ini.Put_Owners(house_name, "Allies", hptr->m_Static.Get_Allies() & ~allies_mask);
            }
        }
    }
}

/**
 * Checks if the given house can ally with this house.
 *
 * 0x004DE56C
 */
BOOL HouseClass::Is_Allowed_To_Ally(HousesType house)
{
    DEBUG_ASSERT(house < HOUSES_COUNT);

    // No allying with a none house
    if (house == HOUSES_NONE) {
        return false;
    }

    // We are already allies, we can't be more ally than we currently are.
    if (Is_Ally(house)) {
        return false;
    }

    // We can ally if its scenario setup time to set map configured alliances.
    if (g_ScenarioInit) {
        return true;
    }

    // No allying once in game for a campaign game though.
    if (g_Session.Game_To_Play() == GAME_CAMPAIGN) {
        return false;
    }

    // Can't ally if I'm dead.
    if (m_Defeated) {
        return false;
    }

    // No allying with AI?
    if (!As_Pointer(house)->m_IsHuman) {
        return false;
    }

    int mp_houses = 0;
    int allied_houses = 1; // Allied with ourselves

    for (HousesType i = HOUSES_MULTI_1; i < HOUSES_COUNT; ++i) {
        HouseClass *hptr = As_Pointer(i);

        if (hptr != nullptr && hptr->m_IsActive && !hptr->m_Defeated) {
            ++mp_houses;

            if (Is_Ally(hptr)) {
                ++allied_houses;
            }
        }
    }

    // If there are more houses than we are currently allied with, we can ally.
    if (mp_houses != allied_houses) {
        return true;
    }

    return false;
}

int HouseClass::AI_Aircraft()
{
#ifdef GAME_DLL
    int (*func)(HouseClass *) = reinterpret_cast<int (*)(HouseClass *)>(0x004DC770);
    return func(this);
#else
    return 0;
#endif
}

int HouseClass::AI_Infantry()
{
#ifdef GAME_DLL
    int (*func)(HouseClass *) = reinterpret_cast<int (*)(HouseClass *)>(0x004DC158);
    return func(this);
#else
    return 0;
#endif
}

int HouseClass::AI_Vessel()
{
#ifdef GAME_DLL
    int (*func)(HouseClass *) = reinterpret_cast<int (*)(HouseClass *)>(0x004DBD08);
    return func(this);
#else
    return 0;
#endif
}

int HouseClass::AI_Unit()
{
#ifdef GAME_DLL
    int (*func)(HouseClass *) = reinterpret_cast<int (*)(HouseClass *)>(0x004DB7EC);
    return func(this);
#else
    return 0;
#endif
}

int HouseClass::AI_Building()
{
#ifdef GAME_DLL
    int (*func)(HouseClass *) = reinterpret_cast<int (*)(HouseClass *)>(0x004DA288);
    return func(this);
#else
    return 0;
#endif
}

int HouseClass::AI_Raise_Money(UrgencyType urgency)
{
#ifdef GAME_DLL
    int (*func)(HouseClass *, UrgencyType) = reinterpret_cast<int (*)(HouseClass *, UrgencyType)>(0x004DA22C);
    return func(this, urgency);
#else
    return 0;
#endif
}

int HouseClass::AI_Raise_Power(UrgencyType urgency)
{
#ifdef GAME_DLL
    int (*func)(HouseClass *, UrgencyType) = reinterpret_cast<int (*)(HouseClass *, UrgencyType)>(0x004DA1D0);
    return func(this, urgency);
#else
    return 0;
#endif
}

/**
 *
 *
 */
int HouseClass::AI_Lower_Power(UrgencyType urgency)
{
    BuildingClass *bptr = Find_Building(BUILDING_POWR, ZONE_NONE);
    if (bptr != nullptr) {
        bptr->Sell_Back(1);
        return true;
    }
    bptr = Find_Building(BUILDING_APWR, ZONE_NONE);
    if (bptr != nullptr) {
        bptr->Sell_Back(1);
        return true;
    }
    return false;
}

/**
 *
 *
 */
int HouseClass::AI_Build_Engineer(UrgencyType urgency)
{
    // empty
    return 0;
}

/**
 *
 *
 */
int HouseClass::AI_Fire_Sale(UrgencyType urgency)
{
    if (m_CurrentBuildingCount > 0 && urgency == URGENCY_FIRE_SALE) {
        Fire_Sale();
        Do_All_To_Hunt();
        return 1;
    }
    return 0;
}

/**
 *
 *
 */
int HouseClass::AI_Build_Income(UrgencyType urgency)
{
    // empty
    return 0;
}

/**
 *
 *
 */
int HouseClass::AI_Build_Offense(UrgencyType urgency)
{
    // empty
    return 0;
}

/**
 *
 *
 */
int HouseClass::AI_Build_Defense(UrgencyType urgency)
{
    // empty
    return 0;
}

/**
 *
 *
 */
int HouseClass::AI_Build_Power(UrgencyType urgency)
{
    // empty
    return 0;
}

int HouseClass::AI_Attack(UrgencyType urgency)
{
#ifdef GAME_DLL
    int (*func)(HouseClass *, UrgencyType) = reinterpret_cast<int (*)(HouseClass *, UrgencyType)>(0x004D9DBC);
    return func(this, urgency);
#else
    return 0;
#endif
}

/**
 *
 *
 */
UrgencyType HouseClass::Check_Raise_Power()
{
    if (Power_Fraction() < g_Rule.Power_Emergency() && m_Drain - 400 > m_Power) {
        if (m_Smarties == URGENCY_3) {
            return URGENCY_3;
        }
        return URGENCY_LOW_POWER;
    }
    return URGENCY_NOTHING;
}

/**
 *
 *
 */
UrgencyType HouseClass::Check_Lower_Power()
{
    return UrgencyType(m_Drain + 300 < m_Power);
}

UrgencyType HouseClass::Check_Raise_Money()
{
#ifdef GAME_DLL
    UrgencyType (*func)(HouseClass *) = reinterpret_cast<UrgencyType (*)(HouseClass *)>(0x004D9CCC);
    return func(this);
#else
    return UrgencyType();
#endif
}

/**
 *
 *
 */
UrgencyType HouseClass::Check_Build_Engineer()
{
    // empty
    return URGENCY_NOTHING;
}

UrgencyType HouseClass::Check_Fire_Sale()
{
#ifdef GAME_DLL
    UrgencyType (*func)(HouseClass *) = reinterpret_cast<UrgencyType (*)(HouseClass *)>(0x004D9C8C);
    return func(this);
#else
    return UrgencyType();
#endif
}

/**
 *
 *
 */
UrgencyType HouseClass::Check_Build_Income()
{
    // empty
    return URGENCY_NOTHING;
}

UrgencyType HouseClass::Check_Attack()
{
#ifdef GAME_DLL
    UrgencyType (*func)(HouseClass *) = reinterpret_cast<UrgencyType (*)(HouseClass *)>(0x004D9C1C);
    return func(this);
#else
    return UrgencyType();
#endif
}

UrgencyType HouseClass::Check_Build_Offense()
{
    // empty
    return URGENCY_NOTHING;
}

UrgencyType HouseClass::Check_Build_Defense()
{
    //empty
    return URGENCY_NOTHING;
}

UrgencyType HouseClass::Check_Build_Power()
{
#ifdef GAME_DLL
    UrgencyType (*func)(HouseClass *) = reinterpret_cast<UrgencyType (*)(HouseClass *)>(0x004D9B70);
    return func(this);
#else
    return UrgencyType();
#endif
}

/**
 *
 *
 */
BOOL HouseClass::Fire_Sale()
{
    if (m_CurrentBuildingCount > 0) {
        for (int i = 0; i < g_Buildings.Count(); ++i) {
            BuildingClass *bptr = &g_Buildings[i];
            if (bptr != nullptr && !bptr->In_Limbo() && bptr->Get_Owner_House() == this && bptr->Get_Health() > 0) {
                bptr->Sell_Back(1);
            }
        }
        return true;
    }
    return false;
}

void HouseClass::Do_All_To_Hunt() const
{
#ifdef GAME_DLL
    void (*func)(const HouseClass *) = reinterpret_cast<void (*)(const HouseClass *)>(0x004DE1FC);
    func(this);
#endif
}

void HouseClass::Super_Weapon_Handler()
{
#ifdef GAME_DLL
    void (*func)(HouseClass *) = reinterpret_cast<void (*)(HouseClass *)>(0x004D50D0);
    func(this);
#endif
}

/**
 * Finds and targets the most threatening building, fires super weapon at it
 *
 */
void HouseClass::Special_Weapon_AI(SpecialWeaponType special)
{
    BuildingClass *building = nullptr;
    int threat_value = -1;

    for (int i = 0; i < g_Buildings.Count(); ++i) {
        BuildingClass *bptr = &g_Buildings[i];

        if (bptr != nullptr && !bptr->In_Limbo() && bptr->Get_Health() > 0) {

            if (!Is_Ally(bptr) && g_Scen.Get_Random_Value(0, 99) < 90 && bptr->Value() > threat_value
                || threat_value == -1) {
                threat_value = bptr->Value();
                building = bptr;
            }
        }
    }

    if (building != nullptr) {
        DEBUG_LOG("HouseClass::Special_Weapon_AI - Placing special blast at building '%s' (threat value '%d')", building->Class_Of().Get_Name(), threat_value);
        Place_Special_Blast(special, building->Center_Cell());
    }
}

BOOL HouseClass::Place_Special_Blast(SpecialWeaponType special, cell_t cell)
{
#ifdef GAME_DLL
    BOOL(*func)
    (HouseClass *, SpecialWeaponType, cell_t) =
        reinterpret_cast<BOOL (*)(HouseClass *, SpecialWeaponType, cell_t)>(0x004D68CC);
    return func(this, special, cell);
#else
    return false;
#endif
}

/**
 *
 *
 */
BOOL HouseClass::Place_Object(RTTIType rtti, cell_t cell)
{
    FactoryClass *fptr = Fetch_Factory(rtti);

    if (fptr == nullptr || !fptr->Has_Completed()) {
        return true;
    }

    TechnoClass *tptr = fptr->Get_Object();

    // If cell is valid then we assume we are placing a building.
    if (cell != -1) {
        if (tptr == nullptr) {
            DEBUG_LOG("HouseClass::Place_Object - Building pointer was nullptr\n");
            return false;
        }

        BuildingClass *builder = tptr->Who_Can_Build_Me();

        // This returns nullptr when the building either doesn't exist or denies the request.
        if (builder == nullptr) {
            DEBUG_LOG("HouseClass::Place_Object - Unable to build Building, cancelling\n");
            return false;
        }

        builder->Transmit_Message(RADIO_HELLO, tptr);
        DEBUG_LOG("HouseClass::Place_Object - Contacted Builder\n");

        if (tptr->Unlimbo(Cell_To_Coord(cell))) {
            // Flag production as completed.
            fptr->Completed();
            // Release factory so it can produce again.
            Abandon_Production(rtti);

            if (!Is_Player()) {
                return true;
            }

            Sound_Effect(VOC_PLACE_BUILDING);
            g_Map.Set_Cursor_Shape();
            g_Map.Clear_Pending_Object();
            DEBUG_LOG("HouseClass::Place_Object - Placed Building\n");
            return true;
        }

        // Failed to deploy.

        if (Is_Player()) {
            Speak(VOX_NO_DEPLOY);
        }

        builder->Transmit_Message(RADIO_OVER_AND_OUT);
        DEBUG_LOG("HouseClass::Place_Object - Failed to Deploy, terminated Radio with Builder\n");

        return false;
    }

    // Following code handles placing objects.

    if (tptr == nullptr) {
        DEBUG_LOG("HouseClass::Place_Object - Object pointer was nullptr\n");
        return true;
    }

    bool not_airplane = false;

    if (tptr->What_Am_I() == RTTI_AIRCRAFT) {
        if (!reinterpret_cast<AircraftClass *>(tptr)->Class_Of().Is_Airplane()) {
            not_airplane = true;
        }
    }

    BuildingClass *builder = tptr->Who_Can_Build_Me(not_airplane);

    // This returns nullptr when the building either doesn't exist or denies the request.
    if (builder == nullptr) {
        DEBUG_LOG("HouseClass::Place_Object - Unable to build Object, cancelling\n");
        return false;
    }

    // When this fails usually that means the builder was blocked, like for example walled in.
    if (!builder->Exit_Object(tptr)) {
        DEBUG_LOG("HouseClass::Place_Object - Failed to exit Object from Factory\n");
        return false;
    }

    // Flag production as completed.
    fptr->Completed();
    // Release factory so it can produce again.
    Abandon_Production(rtti);

    // Register just built object.
    return Register_Just_Built(tptr);
}

/**
 *
 *
 */
BOOL HouseClass::Register_Just_Built(TechnoClass *tptr)
{
    switch (tptr->What_Am_I()) {
        case RTTI_AIRCRAFT:
            m_JustAircraft = reinterpret_cast<AircraftClass *>(tptr)->What_Type();
            m_BuiltSomething = true;
            return true;

        case RTTI_BUILDING:
            m_JustBuilding = reinterpret_cast<BuildingClass *>(tptr)->What_Type();
            m_BuiltSomething = true;
            return true;

        case RTTI_INFANTRY:
            m_JustInfantry = reinterpret_cast<InfantryClass *>(tptr)->What_Type();
            m_BuiltSomething = true;
            return true;

        case RTTI_UNIT:
            m_JustUnit = reinterpret_cast<UnitClass *>(tptr)->What_Type();
            m_BuiltSomething = true;
            return true;

        case RTTI_VESSEL:
            m_JustVessel = reinterpret_cast<VesselClass *>(tptr)->What_Type();
            m_BuiltSomething = true;
            return true;

        default:
            break;
    }
    return false;
}

/**
 *
 *
 */
BOOL HouseClass::Manual_Place(BuildingClass *builder, BuildingClass *pending_building)
{
    if (Is_Player() && g_Map.Pending_ObjectType() == nullptr && builder != nullptr && pending_building != nullptr) {
        Unselect_All();

        g_Map.Repair_Mode_Control(0);
        g_Map.Sell_Mode_Control(0);

        g_Map.Set_Pending_Object(
            pending_building, const_cast<BuildingTypeClass *>(&pending_building->Class_Of()), What_Type());

        // Set current occupy list to the list of the building.
        g_Map.Set_Cursor_Shape((int16_t *)pending_building->Occupy_List(true));

        // Place the foundation preview near the builder.
        // Doesn't actually set the mouse cursor.
        g_Map.Set_Cursor_Pos(builder->Get_Cell());

        builder->Mark(MARK_REDRAW);

        return true;
    }

    return false;
}

void HouseClass::Clobber_All()
{
#ifdef GAME_DLL
    void (*func)(HouseClass *) = reinterpret_cast<void (*)(HouseClass *)>(0x004D7B10);
    func(this);
#endif
}

/**
 *
 *
 */
void HouseClass::Detach(target_t a1, int a2)
{
    if (a1 == m_LastCapBldTarget) {
        m_LastCapBldTarget = 0;
    }
    if (Target_Get_RTTI(a1) == RTTI_TRIGGER) {
        g_HouseTriggers[m_HeapID].Delete(As_Trigger(a1));
    }
}

/**
 *
 *
 */
BOOL HouseClass::Does_Enemy_Building_Exist(BuildingType type) const
{
    for (HousesType i = HOUSES_FIRST; i < HOUSES_COUNT; ++i) {
        HouseClass *hptr = HouseClass::As_Pointer(i);
        if (hptr != nullptr) {
            if (!Is_Ally(hptr) && (type & m_BScan.m_HumanPrereqs)) {
                return true;
            }
        }
    }
    return false;
}

/**
 *
 *
 */
const TechnoTypeClass *HouseClass::Suggest_New_Object(RTTIType rtti, BOOL is_kennel) const
{
    switch (rtti) {
        case RTTI_AIRCRAFT:
        case RTTI_AIRCRAFTTYPE: // Fallthrough
            if (m_ChosenAircraft != AIRCRAFT_NONE) {
                return reinterpret_cast<TechnoTypeClass *>(&AircraftTypeClass::As_Reference(m_ChosenAircraft));
            }
        case RTTI_BUILDING:
        case RTTI_BUILDINGTYPE: // Fallthrough
            if (m_ChosenBuilding != BUILDING_NONE) {
                return reinterpret_cast<TechnoTypeClass *>(&BuildingTypeClass::As_Reference(m_ChosenBuilding));
            }
        case RTTI_INFANTRY:
        case RTTI_INFANTRYTYPE: // Fallthrough
            if (m_ChosenInfantry != INFANTRY_NONE) {
                // disallow barracks to produce dogs
                if (!is_kennel && m_ChosenInfantry == INFANTRY_DOG) {
                    return nullptr;
                }

                // disallow kennel's to produce anything that isn't a dog
                if (is_kennel && m_ChosenInfantry != INFANTRY_DOG) {
                    return nullptr;
                }
                return reinterpret_cast<TechnoTypeClass *>(&InfantryTypeClass::As_Reference(m_ChosenInfantry));
            }
        case RTTI_UNIT:
        case RTTI_UNITTYPE: // Fallthrough
            if (m_ChosenUnit != UNIT_NONE) {
                return reinterpret_cast<TechnoTypeClass *>(&UnitTypeClass::As_Reference(m_ChosenUnit));
            }
        case RTTI_VESSEL:
        case RTTI_VESSELTYPE: // Fallthrough
            if (m_ChosenVessel != VESSEL_NONE) {
                return reinterpret_cast<TechnoTypeClass *>(&VesselTypeClass::As_Reference(m_ChosenVessel));
            }
        default:
            break;
    }
    return nullptr;
}
