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
#include "globals.h"
#include "heap.h"
#include "rules.h"
#include "scenario.h"
#include "session.h"

#ifndef CHRONOSHIFT_STANDALONE
TFixedIHeapClass<HouseClass> &g_Houses = Make_Global<TFixedIHeapClass<HouseClass> >(0x0065D994);
#else
TFixedIHeapClass<HouseClass> g_Houses;
#endif

HouseClass::HouseClass(HousesType type) :
    RTTI(RTTI_HOUSE),
    HeapID(g_Houses.ID(this)),
    Type(HouseTypes.Ptr(type)),
    AIDifficulty(Scen.Get_AI_Difficulty()),
    FirepowerMult("1.0"),
    GroundspeedMult("1.0"),
    AirspeedMult("1.0"),
    ArmorMult("1.0"),
    ROFMult("1.0"),
    CostMult("1.0"),
    BuildTimerMult("1.0"),
    RepairDelay("0.02"),
    BuildDelay("0.03"),
    Static(),
    ActsLike(Type->Get_Type()),
    IsHuman(false),
    PlayerControl(false),
    Production(false),
    Autocreate(false),
    AutoBaseAI(false),
    Discovered(false),
    MaxCapacity(false),
    Defeated(false),
    ToDie(false),
    ToWin(false),
    ToLose(false),
    CivEvac(false),
    RecalcNeeded(true),
    Visionary(false),
    OreShort(false),
    Bit3_1(false),
    Bit3_2(false),
    Repairing(false),
    MapIsClear(false),
    BuiltSomething(false),
    Resigned(false),
    GaveUp(false),
    Paranoid(false),
    AllToLook(true),
    CurrentIQ(Static.Get_IQ()),
    Smarties(URGENCY_NOTHING),
    JustBuilding(BUILDING_NONE),
    JustInfantry(INFANTRY_NONE),
    JustUnit(UNIT_NONE),
    JustAircraft(AIRCRAFT_NONE),
    JustVessel(VESSEL_NONE),
    WinBlocks(0),
    RepairTime(0),
    AlertTime(0),
    BorrowedTime(0),
    field_137(0),
    field_13B(0),
    field_13F(0),
    field_143(0),
    field_147(0),
    field_14B(0),
    field_14F(0),
    field_153(0),
    field_157(0),
    field_15B(0),
    field_15F(0),
    field_163(0),
    field_167(0),
    field_16B(0),
    field_16F(0),
    Spent(0),
    Harvested(0),
    Stolen(0),
    CurrentUnitCount(0),
    CurrentBuildingCount(0),
    CurrentInfantryCount(0),
    CurrentVesselCount(0),
    CurrentAircraftCount(0),
    Ore(0),
    Credits(0),
    Capacity(0),
    AircraftBought(nullptr),
    InfantryBought(nullptr),
    UnitsBought(nullptr),
    BuildingsBought(nullptr),
    VesselsBought(nullptr),
    AircraftKilled(nullptr),
    InfantryKilled(nullptr),
    UnitsKilled(nullptr),
    BuildingsKilled(nullptr),
    VesselsKilled(nullptr),
    BuildingsCaptured(nullptr),
    CratesFound(nullptr),
    AircraftFactoryCount(0),
    InfantryFactoryCount(0),
    UnitFactoryCount(0),
    VesselFactoryCount(0),
    BuildingFactoryCount(0),
    Power(0),
    Drain(0),
    AircraftFactory(nullptr),
    InfantryFactory(nullptr),
    UnitFactory(nullptr),
    VesselFactory(nullptr),
    BuildingFactory(nullptr),
    FlagHolder(0),
    FlagLocation(0),
    UnitsLost(0),
    BuildingsLost(0),
    field_2AD(HOUSES_NONE),
    BaseCenter(0),
    Radius(0),
    LastAttackFrame(0),
    LastAttackType(RTTI_NONE),
    LastAttackZone(ZONE_NONE),
    LastAttackHouse(HOUSES_NONE),
    field_2F9(0),
    field_2FD(0),
    Score(0),
    PreferredTarget(QUARRY_ANYTHING),
    AttackTime(Rule.Attack_Delay() * Scen.Get_Random_Value(450, 1800)),
    field_527(HOUSES_NONE),
    AITime(0),
    ChronoObject(0),
    ChosenBuilding(BUILDING_NONE),
    ChosenUnit(UNIT_NONE),
    ChosenInfantry(INFANTRY_NONE),
    ChosenAircraft(AIRCRAFT_NONE),
    ChosenVessel(VESSEL_NONE),
    MissileTargetCell(0),
    Allies(0),
    DamageTime(Rule.Get_Damage_Delay() * 900),
    TeamTime(Rule.Team_Delay() * 900),
    TriggerTime(0),
    AttackedSpeechTime(1),
    LowPowerSpeechTime(1),
    field_177D(1),
    LowCreditsSpeechTime(1),
    Color(Type->Get_Color())
{
    memset(Zones, 0, sizeof(Zones));

    memset(UnitsDestroyed, 0, sizeof(UnitsDestroyed));
    memset(BuildingsDestroyed, 0, sizeof(BuildingsDestroyed));
    memset(BuildingQuantity, 0, sizeof(BuildingQuantity));
    memset(UnitQuantity, 0, sizeof(UnitQuantity));
    memset(InfantryQuantity, 0, sizeof(InfantryQuantity));
    memset(AircraftQuantity, 0, sizeof(AircraftQuantity));
    memset(VesselQuantity, 0, sizeof(VesselQuantity));

    memset(ThreatRegions, 0, sizeof(ThreatRegions));

    new (&Specials[SPECIAL_SONAR_PULSE]) SuperClass(Rule.Get_Recharge_Sonar() * 900, false, VOX_NONE, VOX_SONAR_PULSE_AVAILABLE, VOX_NONE, VOX_NONE);
    new (&Specials[SPECIAL_ATOM_BOMB]) SuperClass(Rule.Get_Recharge_Nuke() * 900, true, VOX_ABOMB_PREPING, VOX_ABOMB_READY, VOX_NONE, VOX_NO_POWER);
    new (&Specials[SPECIAL_WARP_SPHERE]) SuperClass(Rule.Get_Recharge_Chrono() * 900, true, VOX_CHROCHR1, VOX_CHRORDY1, VOX_NONE, VOX_NO_POWER);
    new (&Specials[SPECIAL_PARA_BOMB]) SuperClass(Rule.Get_Recharge_ParaBomb() * 900, false, VOX_NONE, VOX_NONE, VOX_NONE, VOX_NONE);
    new (&Specials[SPECIAL_PARA_INFANTRY]) SuperClass(Rule.Get_Recharge_Paratrooper() * 900, false, VOX_NONE, VOX_NONE, VOX_NONE, VOX_NONE);
    new (&Specials[SPECIAL_SPY_PLANE]) SuperClass(Rule.Get_Recharge_SpyPlane() * 900, false, VOX_NONE, VOX_SPY_PLANE, VOX_NONE, VOX_NONE);
    new (&Specials[SPECIAL_IRON_CURTAIN]) SuperClass(Rule.Get_Recharge_IronCurtain() * 900, true, VOX_IRON_CHARGING, VOX_IRON_READY, VOX_NONE, VOX_NO_POWER);
    new (&Specials[SPECIAL_GPS]) SuperClass(Rule.Get_Recharge_GPS() * 900, true, VOX_NONE, VOX_NONE, VOX_NONE, VOX_NO_POWER);

    strncpy(field_1790, Fetch_String(TXT_COMPUTER), sizeof(field_1790));
    memset(PlayerHandle, 0, sizeof(PlayerHandle));

    if (Session.Game_To_Play() == GAME_INTERNET) {
        AircraftBought = new UnitTrackerClass(AIRCRAFT_COUNT);
        InfantryBought = new UnitTrackerClass(INFANTRY_COUNT);
        UnitsBought = new UnitTrackerClass(UNIT_COUNT);
        BuildingsBought = new UnitTrackerClass(BUILDING_COUNT);
        VesselsBought = new UnitTrackerClass(VESSEL_COUNT);
        AircraftKilled = new UnitTrackerClass(AIRCRAFT_COUNT);
        InfantryKilled = new UnitTrackerClass(INFANTRY_COUNT);
        UnitsKilled = new UnitTrackerClass(UNIT_COUNT);
        BuildingsKilled = new UnitTrackerClass(BUILDING_COUNT);
        VesselsKilled = new UnitTrackerClass(VESSEL_COUNT);
        BuildingsCaptured = new UnitTrackerClass(BUILDING_COUNT);
        CratesFound = new UnitTrackerClass(CRATE_COUNT);
    }

    // HouseTriggers

    // Make_Ally(house);
    BOOL (*make_ally)(HouseClass *, HousesType) = reinterpret_cast<BOOL (*)(HouseClass *, HousesType)>(0x004D6060);
    make_ally(this, type);

    // Assign_Handicap(Scen.Get_AI_Difficulty());
    DiffType (*assign_handicap)(HouseClass *, DiffType) = reinterpret_cast<DiffType (*)(HouseClass *, DiffType)>(0x004D2D48);
    assign_handicap(this, Scen.Get_AI_Difficulty());
}

HouseClass::HouseClass(const HouseClass &that) :
    RTTI(that.RTTI),
    HeapID(that.HeapID),
    Type(that.Type),
    AIDifficulty(that.AIDifficulty),
    FirepowerMult(that.FirepowerMult),
    GroundspeedMult(that.GroundspeedMult),
    AirspeedMult(that.AirspeedMult),
    ArmorMult(that.ArmorMult),
    ROFMult(that.ROFMult),
    CostMult(that.CostMult),
    BuildTimerMult(that.BuildTimerMult),
    RepairDelay(that.RepairDelay),
    BuildDelay(that.BuildDelay),
    Static(that.Static),
    ActsLike(that.ActsLike),
    IsActive(that.IsActive),
    IsHuman(that.IsHuman),
    PlayerControl(that.PlayerControl),
    Production(that.Production),
    Autocreate(that.Autocreate),
    AutoBaseAI(that.AutoBaseAI),
    Discovered(that.Discovered),
    MaxCapacity(that.MaxCapacity),
    Defeated(that.Defeated),
    ToDie(that.ToDie),
    ToWin(that.ToWin),
    ToLose(that.ToLose),
    CivEvac(that.CivEvac),
    RecalcNeeded(that.RecalcNeeded),
    Visionary(that.Visionary),
    OreShort(that.OreShort),
    Bit3_1(that.Bit3_1),
    Bit3_2(that.Bit3_2),
    Repairing(that.Repairing),
    MapIsClear(that.MapIsClear),
    BuiltSomething(that.BuiltSomething),
    Resigned(that.Resigned),
    GaveUp(that.GaveUp),
    Paranoid(that.Paranoid),
    AllToLook(that.AllToLook),
    CurrentIQ(that.CurrentIQ),
    Smarties(that.Smarties),
    JustBuilding(that.JustBuilding),
    JustInfantry(that.JustInfantry),
    JustUnit(that.JustUnit),
    JustAircraft(that.JustAircraft),
    JustVessel(that.JustVessel),
    WinBlocks(that.WinBlocks),
    RepairTime(that.RepairTime),
    AlertTime(that.AlertTime),
    BorrowedTime(that.BorrowedTime),
    field_137(that.field_137),
    field_13B(that.field_13B),
    field_13F(that.field_13F),
    field_143(that.field_143),
    field_147(that.field_147),
    field_14B(that.field_14B),
    field_14F(that.field_14F),
    field_153(that.field_153),
    field_157(that.field_157),
    field_15B(that.field_15B),
    field_15F(that.field_15F),
    field_163(that.field_163),
    field_167(that.field_167),
    field_16B(that.field_16B),
    field_16F(that.field_16F),
    Spent(that.Spent),
    Harvested(that.Harvested),
    Stolen(that.Stolen),
    CurrentUnitCount(that.CurrentUnitCount),
    CurrentBuildingCount(that.CurrentBuildingCount),
    CurrentInfantryCount(that.CurrentInfantryCount),
    CurrentVesselCount(that.CurrentVesselCount),
    CurrentAircraftCount(that.CurrentAircraftCount),
    Ore(that.Ore),
    Credits(that.Credits),
    Capacity(that.Capacity),
    AircraftBought(that.AircraftBought),
    InfantryBought(that.InfantryBought),
    UnitsBought(that.UnitsBought),
    BuildingsBought(that.BuildingsBought),
    VesselsBought(that.VesselsBought),
    AircraftKilled(that.AircraftKilled),
    InfantryKilled(that.InfantryKilled),
    UnitsKilled(that.UnitsKilled),
    BuildingsKilled(that.BuildingsKilled),
    VesselsKilled(that.VesselsKilled),
    BuildingsCaptured(that.BuildingsCaptured),
    CratesFound(that.CratesFound),
    AircraftFactoryCount(that.AircraftFactoryCount),
    InfantryFactoryCount(that.InfantryFactoryCount),
    UnitFactoryCount(that.UnitFactoryCount),
    VesselFactoryCount(that.VesselFactoryCount),
    BuildingFactoryCount(that.BuildingFactoryCount),
    Power(that.Power),
    Drain(that.Drain),
    AircraftFactory(that.AircraftFactory),
    InfantryFactory(that.InfantryFactory),
    UnitFactory(that.UnitFactory),
    VesselFactory(that.VesselFactory),
    BuildingFactory(that.BuildingFactory),
    FlagHolder(that.FlagHolder),
    FlagLocation(that.FlagLocation),
    UnitsLost(that.UnitsLost),
    BuildingsLost(that.BuildingsLost),
    field_2AD(that.field_2AD),
    BaseCenter(that.BaseCenter),
    Radius(that.Radius),
    LastAttackFrame(that.LastAttackFrame),
    LastAttackType(that.LastAttackType),
    LastAttackZone(that.LastAttackZone),
    LastAttackHouse(that.LastAttackHouse),
    field_2F9(that.field_2F9),
    field_2FD(that.field_2FD),
    Score(that.Score),
    PreferredTarget(that.PreferredTarget),
    AttackTime(that.AttackTime),
    field_527(that.field_527),
    AITime(that.AITime),
    ChronoObject(that.ChronoObject),
    ChosenBuilding(that.ChosenBuilding),
    ChosenUnit(that.ChosenUnit),
    ChosenInfantry(that.ChosenInfantry),
    ChosenAircraft(that.ChosenAircraft),
    ChosenVessel(that.ChosenVessel),
    MissileTargetCell(that.MissileTargetCell),
    Allies(that.Allies),
    DamageTime(that.DamageTime),
    TeamTime(that.TeamTime),
    TriggerTime(that.TriggerTime),
    AttackedSpeechTime(that.AttackedSpeechTime),
    LowPowerSpeechTime(that.LowPowerSpeechTime),
    field_177D(that.field_177D),
    LowCreditsSpeechTime(that.LowCreditsSpeechTime),
    Color(that.Color)
{
    memcpy(Specials, that.Specials, sizeof(Specials));
    memcpy(UnitsDestroyed, that.UnitsDestroyed, sizeof(UnitsDestroyed));
    memcpy(BuildingsDestroyed, that.BuildingsDestroyed, sizeof(UnitsDestroyed));
    memcpy(Zones, that.Zones, sizeof(Zones));
    memcpy(BuildingQuantity, that.BuildingQuantity, sizeof(BuildingQuantity));
    memcpy(UnitQuantity, that.UnitQuantity, sizeof(UnitQuantity));
    memcpy(InfantryQuantity, that.InfantryQuantity, sizeof(InfantryQuantity));
    memcpy(AircraftQuantity, that.AircraftQuantity, sizeof(AircraftQuantity));
    memcpy(VesselQuantity, that.VesselQuantity, sizeof(VesselQuantity));
    memcpy(ThreatRegions, that.ThreatRegions, sizeof(ThreatRegions));
    memcpy(field_1790, that.field_1790, sizeof(field_1790));
    memcpy(PlayerHandle, that.PlayerHandle, sizeof(PlayerHandle));
}

HouseClass::~HouseClass()
{
    if (Session.Game_To_Play() == GAME_INTERNET) {
        delete AircraftBought;
        delete InfantryBought;
        delete UnitsBought;
        delete BuildingsBought;
        delete VesselsBought;
        delete AircraftKilled;
        delete InfantryKilled;
        delete UnitsKilled;
        delete BuildingsKilled;
        delete VesselsKilled;
        delete BuildingsCaptured;
        delete CratesFound;
    }
}

void *HouseClass::operator new(size_t size)
{
    HouseClass *this_ptr = g_Houses.Alloc();
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->IsActive = true;
    }
    return this_ptr;
}

void HouseClass::operator delete(void *ptr)
{
    HouseClass *this_ptr = static_cast<HouseClass *>(ptr);
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->IsActive = false;
    }
    g_Houses.Free(this_ptr);
}

void HouseClass::Spend_Money(unsigned int amount)
{
    if (amount <= Ore) {
        Ore -= amount;
    } else {
        amount -= Ore;
        Ore = 0;
        Credits -= amount;
    }
    Silo_Redraw_Check(amount, Capacity);
    Spent += amount;
}

void HouseClass::Silo_Redraw_Check(unsigned int a1, unsigned int a2)
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(HouseClass *, unsigned int, unsigned int) =
        reinterpret_cast<void (*)(HouseClass *, unsigned int, unsigned int)>(0x004D5EF4);
    func(this, a1, a2);
#else
    return 0;
#endif
}
