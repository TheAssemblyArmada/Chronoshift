/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
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
#pragma once

#ifndef HOUSE_H
#define HOUSE_H

#include "always.h"
#include "aircrafttype.h"
#include "buildingtype.h"
#include "difficulty.h"
#include "factory.h"
#include "fixed.h"
#include "gameptr.h"
#include "gametypes.h"
#include "globals.h"
#include "heap.h"
#include "housestatic.h"
#include "infantrytype.h"
#include "region.h"
#include "super.h"
#include "ttimer.h"
#include "unittype.h"
#include "vesseltype.h"

class HouseTypeClass;
class HouseClass;
class FootClass;
class GameINIClass;
class UnitTrackerClass;

#ifdef GAME_DLL
extern HouseClass *&g_PlayerPtr;
#else
extern HouseClass *g_PlayerPtr;
#endif

struct ZoneInfo
{
    int Air;
    int Armor;
    int Infantry;
};

class HouseClass
{
    // a class of all the buildings (BUILDING_COUNT) and with settings on if they are chosen, i think...
    class BuildChoiceClass
    {
    public:
        BuildChoiceClass() : field_0(0), Choice(BUILDING_NONE) {}
        BuildChoiceClass(const BuildChoiceClass &that) : field_0(that.field_0), Choice(that.Choice) {}
        BuildChoiceClass(const NoInitClass &noinit) {}
        ~BuildChoiceClass() {}

        void Code_Pointers() {}
        void Decode_Pointers() {}

        char Get_Field_0() const { return field_0; }
        void Set_Field_0(char a1) { field_0 = a1; }
        BuildingType Get_Choice() const { return Choice; }
        void Set_Choice(BuildingType type) { Choice = type; }

    private:
        char field_0; // 0x0 		//could be a priority?
        BuildingType Choice; // 0x1
    };

    struct BuildReqTracker
    {
        uint32_t m_HaveBuilt;
        uint32_t m_HumanPrereqs;
        uint32_t m_AIPrereqs;
    };

public:
    HouseClass(HousesType type);
    HouseClass(const HouseClass &that);
    HouseClass(const NoInitClass &noinit) {}
    ~HouseClass();

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    void Init_Data(PlayerColorType color, HousesType house, int credits);
    const char *Get_Name() const { return m_Type->Get_Name(); }
    void Harvested(unsigned amount);
    void Stole(unsigned amount) { m_Stolen += amount; }
    int Available_Money() const { return m_Credits + m_Ore; }
    void Spend_Money(unsigned amount);
    int Adjust_Capacity(int amount, BOOL cap);
    void Refund_Money(unsigned amount) { m_Credits += amount; }
    void Silo_Redraw_Check(unsigned ore, unsigned capacity);
    fixed_t Ore_Fraction();
    fixed_t Power_Fraction();
    DiffType Assign_Handicap(DiffType diff);
    BOOL Can_Build(TechnoTypeClass *obj, HousesType house);
    void Make_Ally(HousesType type);
    void Make_Enemy(HousesType type);
    BOOL Is_Ally(HousesType type) const;
    BOOL Is_Ally(HouseClass *house) const;
    BOOL Is_Ally(ObjectClass *object) const;
    BOOL Flag_To_Die();
    BOOL Flag_To_Win();
    BOOL Flag_To_Lose();
    void Adjust_Threat(int region, int risk);
    void Tracking_Remove(TechnoClass *object);
    void Tracking_Add(TechnoClass *object);
    int *Factory_Counter(RTTIType type);
    int Factory_Count(RTTIType type);
    void Active_Remove(TechnoClass *object);
    void Active_Add(TechnoClass *object);
    FactoryClass *Fetch_Factory(RTTIType rtti) const;
    void Set_Factory(RTTIType rtti, FactoryClass *factory);
    const uint8_t *Remap_Table(BOOL alt, RemapType type);
    void Sell_Wall(cell_t cellnum);
    BuildingClass *Find_Building(BuildingType to_find, ZoneType zone) const;
    coord_t Find_Build_Location(BuildingClass *building) const;
    cell_t Find_Cell_In_Zone(TechnoClass *object, ZoneType zone) const;
    cell_t Random_Cell_In_Zone(ZoneType zone) const;
    ZoneType Which_Zone(ObjectClass *object) const;
    ZoneType Which_Zone(cell_t cellnum) const;
    ZoneType Which_Zone(coord_t coord) const;
    cell_t Zone_Cell(ZoneType zone) const;
    cell_t Where_To_Go(FootClass *obj) const;
    target_t Find_Juicy_Target(coord_t coord) const;
    BOOL Is_Hack_Prevented(RTTIType rtti, int id) const;
    BuildingTypeClass *Suggest_Building() const { return &BuildingTypeClass::As_Reference(m_ChosenBuilding); };
    int Get_Quantity(AircraftType type) const { return m_AircraftQuantity[type]; }
    int Get_Quantity(BuildingType type) const { return m_BuildingQuantity[type]; }
    int Get_Heap_ID() const { return m_HeapID; }
    HousesType What_Type() const { return m_Type->What_Type(); }
    BOOL Is_Player() const { return this == g_PlayerPtr; }
    DiffType Get_AI_Difficulty() const { return m_AIDifficulty; }
    fixed_t Cost_Multiplier() const { return m_CostMult; }
    HousesType Acts_Like() const { return m_ActsLike; }
    void Set_AI_Difficulty(DiffType value) { m_AIDifficulty = value; }
    BOOL Is_Active() const { return m_IsActive; }
    void Set_Active(BOOL value) { m_IsActive = value; }
    BOOL Is_Human() const { return m_IsHuman; }
    void Set_Human(BOOL value) { m_IsHuman = value; }
    BOOL Player_Has_Control() const { return m_PlayerControl; }
    void Set_Player_Control(BOOL value) { m_PlayerControl = value; }
    BOOL Production_Started() const { return m_Production; }
    void Set_Production(BOOL value) { m_Production = value; }
    BOOL Autocreate_Teams() const { return m_Autocreate; }
    void Set_Autocreate(BOOL value) { m_Autocreate = value; }
    BOOL Is_Defeated() const { return m_Defeated; }
    void Set_Defeated(BOOL value) { m_Defeated = value; }
    BOOL Is_Map_Clear() const { return m_MapIsClear; }
    void Set_Map_Clear(BOOL value) { m_MapIsClear = value; }
    int Get_Current_IQ() const { return m_CurrentIQ; }
    void Set_IQ_Level(int value) { m_CurrentIQ = value; }
    SuperClass &Special_Weapons(SpecialWeaponType type) { return m_Specials[type]; }
    int Currently_Owned_Unit_Count() const { return m_CurrentUnitCount; }
    int Currently_Owned_Building_Count() const { return m_CurrentBuildingCount; }
    int Currently_Owned_Infantry_Count() const { return m_CurrentInfantryCount; }
    int Currently_Owned_Vessel_Count() const { return m_CurrentVesselCount; }
    int Currently_Owned_Aircraft_Count() const { return m_CurrentAircraftCount; }
    coord_t Base_Center() const { return m_BaseCenter; }
    RegionClass *Threat_Regions() { return m_ThreatRegions; }
    target_t Chrono_Object() const { return m_ChronoObject; }
    void Code_Pointers() {}
    void Decode_Pointers() {}
    BOOL Has_Buildings() const { return m_BScan.m_HaveBuilt != 0; }
    BOOL Spied_My_Radar(HousesType house) const { return (m_RadarSpied & (1 << house)) != 0; }
    BOOL Spied_My_Radar(HouseClass *house) const { return Spied_My_Radar(house->What_Type()); }

    static void One_Time();
    static void Init();
    static HouseClass &As_Reference(HousesType type);
    static HouseClass *As_Pointer(HousesType type);
    static void Computer_Paranoid();
    static void Read_INI(GameINIClass &ini);
    static void Write_INI(GameINIClass &ini);

#ifdef GAME_DLL
    FactoryClass *Hook_Fetch_Factory(RTTIType rtti) { return Fetch_Factory(rtti); }
    BuildingClass *Hook_Find_Building(BuildingType to_find, ZoneType zone) { return Find_Building(to_find, zone); }
    coord_t Hook_Find_Build_Location(BuildingClass *building) { return Find_Build_Location(building); }
    cell_t Hook_Find_Cell_In_Zone(TechnoClass *object, ZoneType zone) { return Find_Cell_In_Zone(object, zone); }
    cell_t Hook_Random_Cell_In_Zone(ZoneType zone) { return Random_Cell_In_Zone(zone); }
    ZoneType Hook_Which_Zone_Obj(ObjectClass *object) { return Which_Zone(object); }
    ZoneType Hook_Which_Zone_Cell(cell_t cellnum) { return Which_Zone(cellnum); }
    ZoneType Hook_Which_Zone_Coord(coord_t coord) { return Which_Zone(coord); }
    cell_t Hook_Zone_Cell(ZoneType zone) { return Zone_Cell(zone); }
    cell_t Hook_Where_To_Go(FootClass *obj) { return Where_To_Go(obj); }
    target_t Hook_Find_Juicy_Target(coord_t coord) { return Find_Juicy_Target(coord); }
    BOOL Hook_Is_No_Yak_Mig() { return Is_No_Yak_Mig(); }
    BOOL Hook_Is_Hack_Prevented(RTTIType rtti, int id) { return Is_Hack_Prevented(rtti, id); }
#endif

private:
    BOOL Is_Allowed_To_Ally(HousesType house);
    BOOL Is_No_Yak_Mig() const;

private:
    RTTIType m_RTTI;
    int m_HeapID;
    GamePtr<HouseTypeClass> m_Type;
    DiffType m_AIDifficulty;
    fixed_t m_FirepowerMult;
    fixed_t m_GroundspeedMult;
    fixed_t m_AirspeedMult;
    fixed_t m_ArmorMult;
    fixed_t m_ROFMult;
    fixed_t m_CostMult;
    fixed_t m_BuildTimerMult;
    fixed_t m_RepairDelay;
    fixed_t m_BuildDelay;
    HouseStaticClass m_Static;
    HousesType m_ActsLike;

#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_IsActive : 1; // 1
    BOOL m_IsHuman : 1; // 2
    BOOL m_PlayerControl : 1; // 4
    BOOL m_Production : 1; // 8
    BOOL m_Autocreate : 1; // 16
    BOOL m_AutoBaseAI : 1; // 32
    BOOL m_Discovered : 1; // 64
    BOOL m_MaxCapacity : 1; // 128

    BOOL m_Defeated : 1; // 1
    BOOL m_ToDie : 1; // 2
    BOOL m_ToWin : 1; // 4
    BOOL m_ToLose : 1; // 8
    BOOL m_CivEvac : 1; // 16
    BOOL m_RecalcNeeded : 1; // 32
    BOOL m_Visionary : 1; // 64
    BOOL m_OreShort : 1; // 128

    BOOL m_Spied : 1; // 1
    BOOL m_Infiltrated : 1; // 2
    BOOL m_Repairing : 1; // 4
    BOOL m_MapIsClear : 1; // 8
    BOOL m_BuiltSomething : 1; // 16
    BOOL m_Resigned : 1; // 32
    BOOL m_GaveUp : 1; // 64
    BOOL m_Paranoid : 1; // 128

    BOOL m_AllToLook : 1; // 1
#else
    bool m_IsActive; // Is this object allocated and active for use? (def = false)
                     // NOTE: This should be set to true on class creation.
    bool m_IsHuman; // Is this house controlled by a human player rather than an AI [local or otherwise]?
    bool m_PlayerControl; // Is this house controlled by the human player [the house set to PlayerPtr]?
    bool m_Production;
    bool m_Autocreate; // Initiates autocreate for the house. This will cause the
                       // computer's house to build autocreate teams as it sees fit.
    bool m_AutoBaseAI; // Initialize the computer skirmish mode build control to
                       // either 'on' or 'off' state. When 'on, the computer takes
                       // over as if it were in skirmish mode.
                       // (NOTE, make sure he has at least a con yard).
    bool m_Discovered;
    bool m_MaxCapacity;
    bool m_Defeated;
    bool m_ToDie;
    bool m_ToWin;
    bool m_ToLose;
    bool m_CivEvac;
    bool m_RecalcNeeded;
    bool m_Visionary;
    bool m_OreShort;
    bool m_Spied; // TODO: Alerted?
    bool m_Infiltrated; // TODO: Thieved / Infiltrated / Spied?
    bool m_Repairing;
    bool m_MapIsClear;
    bool m_BuiltSomething;
    bool m_Resigned;
    bool m_GaveUp;
    bool m_Paranoid;
    bool m_AllToLook;
#endif

    int m_CurrentIQ;
    UrgencyType m_Smarties;
    SuperClass m_Specials[SPECIAL_COUNT];
    BuildingType m_JustBuilding;
    InfantryType m_JustInfantry;
    UnitType m_JustUnit;
    AircraftType m_JustAircraft;
    VesselType m_JustVessel;
    int m_WinBlocks;
    TCountDownTimerClass<FrameTimerClass> m_RepairTime;
    TCountDownTimerClass<FrameTimerClass> m_AlertTime;
    TCountDownTimerClass<FrameTimerClass> m_BorrowedTime;
    // int m_field_137; // BuildingType's
    // int m_field_13B; // BuildingType's
    // int m_field_13F; // BuildingType's       // BScan in EDWIN?
    BuildReqTracker m_BScan;
    // int m_field_143; // UnitType's
    // int m_field_147; // UnitType's
    // int m_field_14B; // UnitType's           // UScan in EDWIN?
    BuildReqTracker m_UScan;
    // int m_field_14F; // InfantryType's
    // int m_field_153; // InfantryType's
    // int m_field_157; // InfantryType's       // IScan in EDWIN?
    BuildReqTracker m_IScan;
    // int m_field_15B; // AircraftType's
    // int m_field_15F; // AircraftType's
    // int m_field_163; // AircraftType's       // AScan in EDWIN?
    BuildReqTracker m_AScan;
    // int m_field_167; // VesselType's
    // int m_field_16B; // VesselType's
    // int m_field_16F; // VesselType's         // VScan in EDWIN?
    BuildReqTracker m_VScan;
    int m_Spent;
    int m_Harvested;
    int m_Stolen;
    int m_CurrentUnitCount;
    int m_CurrentBuildingCount;
    int m_CurrentInfantryCount;
    int m_CurrentVesselCount;
    int m_CurrentAircraftCount;
    unsigned m_Ore;
    unsigned m_Credits;
    unsigned m_Capacity;
    UnitTrackerClass *m_AircraftBought;
    UnitTrackerClass *m_InfantryBought;
    UnitTrackerClass *m_UnitsBought;
    UnitTrackerClass *m_BuildingsBought;
    UnitTrackerClass *m_VesselsBought;
    UnitTrackerClass *m_AircraftKilled;
    UnitTrackerClass *m_InfantryKilled;
    UnitTrackerClass *m_UnitsKilled;
    UnitTrackerClass *m_BuildingsKilled;
    UnitTrackerClass *m_VesselsKilled;
    UnitTrackerClass *m_BuildingsCaptured;
    UnitTrackerClass *m_CratesFound;
    int m_AircraftFactoryCount;
    int m_InfantryFactoryCount;
    int m_UnitFactoryCount;
    int m_VesselFactoryCount;
    int m_BuildingFactoryCount;
    int m_Power;
    int m_Drain;
    GamePtr<FactoryClass> m_AircraftFactory;
    GamePtr<FactoryClass> m_InfantryFactory;
    GamePtr<FactoryClass> m_UnitFactory;
    GamePtr<FactoryClass> m_VesselFactory;
    GamePtr<FactoryClass> m_BuildingFactory;
    target_t m_FlagHolder;
    cell_t m_FlagLocation;
    int m_UnitsDestroyed[HOUSES_COUNT];
    int m_UnitsLost;
    int m_BuildingsDestroyed[HOUSES_COUNT];
    int m_BuildingsLost;
    HousesType m_UnkHouseType; // House we last captured something from?
    coord_t m_BaseCenter;
    int m_Radius;
    ZoneInfo m_Zones[ZONE_COUNT];
    int m_LastAttackFrame;
    RTTIType m_LastAttackType;
    ZoneType m_LastAttackZone;
    HousesType m_LastAttackHouse;
    target_t m_LastCapBldTarget; // last captured building coord? see Building::Captured().
    uint32_t m_RadarSpied; // spied house bit?
    int m_Score;
    QuarryType m_PreferredTarget;
#ifdef GAME_DLL
    int m_BuildingQuantity[BUILDING_NOEXP_COUNT]; // TODO: Must keep with pre expansion counts for ABI.
    int m_UnitQuantity[UNIT_NOEXP_COUNT]; // TODO: Must keep with pre expansion counts for ABI.
    int m_InfantryQuantity[INFANTRY_NOEXP_COUNT]; // TODO: Must keep with pre expansion counts for ABI.
    int m_AircraftQuantity[AIRCRAFT_COUNT];
    int m_VesselQuantity[VESSEL_NOEXP_COUNT]; // TODO: Must keep with pre expansion counts for ABI.
#else
    int m_BuildingQuantity[BUILDING_COUNT];
    int m_UnitQuantity[UNIT_COUNT];
    int m_InfantryQuantity[INFANTRY_COUNT];
    int m_AircraftQuantity[AIRCRAFT_COUNT];
    int m_VesselQuantity[VESSEL_COUNT];
#endif
    TCountDownTimerClass<FrameTimerClass> m_AttackTime;
    HousesType m_Enemy; // the HousesType of the one house considered the current main enemy?
    TCountDownTimerClass<FrameTimerClass> m_AITime;
    target_t m_ChronoObject;
    BuildingType m_ChosenBuilding;
    UnitType m_ChosenUnit;
    InfantryType m_ChosenInfantry;
    AircraftType m_ChosenAircraft;
    VesselType m_ChosenVessel;
    RegionClass m_ThreatRegions[34 * 34];
    cell_t m_MissileTargetCell;
    uint32_t m_Allies; // Stores a bitfield of houses, must be large enough type to hold all houses.
    TCountDownTimerClass<FrameTimerClass> m_DamageTime;
    TCountDownTimerClass<FrameTimerClass> m_TeamTime;
    TCountDownTimerClass<FrameTimerClass> m_TriggerTime;
    TCountDownTimerClass<FrameTimerClass> m_AttackedSpeechTime;
    TCountDownTimerClass<FrameTimerClass> m_LowPowerSpeechTime;
    TCountDownTimerClass<FrameTimerClass> m_UnkTimer; // Unused?
    TCountDownTimerClass<FrameTimerClass> m_LowCreditsSpeechTime;
    PlayerColorType m_Color;
    char m_PlayerName[12];
    char m_PlayerHandle[12];

private:
#ifdef GAME_DLL
    static TFixedIHeapClass<BuildChoiceClass> &g_BuildChoice;
#else
    static TFixedIHeapClass<BuildChoiceClass> g_BuildChoice;
#endif
};

#ifdef GAME_DLL
extern TFixedIHeapClass<HouseClass> &g_Houses;
#else
extern TFixedIHeapClass<HouseClass> g_Houses;
#endif

#endif // HOUSE_H
