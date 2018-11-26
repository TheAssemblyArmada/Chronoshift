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
#pragma once

#ifndef HOUSE_H
#define HOUSE_H

#include "always.h"
#include "aircrafttype.h"
#include "buildingtype.h"
#include "difficulty.h"
#include "factory.h"
#include "fixed.h"
#include "globals.h"
#include "gameptr.h"
#include "gametypes.h"
#include "housestatic.h"
#include "infantrytype.h"
#include "region.h"
#include "super.h"
#include "ttimer.h"
#include "unittype.h"
#include "utracker.h"
#include "vesseltype.h"

class HouseTypeClass;
class NoInitClass;

template<class T>
class TFixedIHeapClass;

class HouseClass;
#ifndef CHRONOSHIFT_STANDALONE
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
private:
    // a class of all the buildings (BUILDING_COUNT) and with settings on if they are chosen, i think...
    class BuildChoiceClass
    {
    public:
        BuildChoiceClass() : field_0(0), Choice(BUILDING_NONE) {}
        BuildChoiceClass(const BuildChoiceClass &that) : field_0(0), Choice(BUILDING_NONE) {}
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

    const char *Get_Name() const { return m_Type->Get_Name(); }

    void Stole(unsigned int amount) { m_Stolen += amount; }
    unsigned int Available_Money() const { return m_Credits + m_Ore; }
    void Spend_Money(unsigned int amount);
    void Refund_Money(unsigned int amount) { m_Credits += amount; }
    void Silo_Redraw_Check(unsigned int a1, unsigned int a2);

    DiffType Assign_Handicap(DiffType diff);

    void Make_Ally(HousesType type);
    void Make_Enemy(HousesType type);

    BOOL Flag_To_Die();
    BOOL Flag_To_Win();
    BOOL Flag_To_Lose();

    void Tracking_Remove(TechnoClass *object);
    void Tracking_Add(TechnoClass *object);
    int *Factory_Counter(RTTIType type);
    void Active_Remove(TechnoClass *object);
    void Active_Add(TechnoClass *object);

    const uint8_t *Remap_Table(BOOL unk1, RemapType type);

    int Get_Heap_ID() const { return m_HeapID; }

    HousesType What_Type() const { return m_Type->What_Type(); }

    BOOL Is_Player() const { return this == g_PlayerPtr; }
    DiffType Get_AI_Difficulty() const { return m_AIDifficulty; }

    fixed_t Cost_Multiplier() const { return m_CostMult; }
    HousesType Acts_Like() const { return m_ActsLike; }

    void Set_AI_Difficulty(DiffType value) { m_AIDifficulty = value; }
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
    int Get_Current_IQ() const { return m_CurrentIQ; }
    void Set_IQ_Level(int value) { m_CurrentIQ = value; }

    SuperClass &Special_Weapons(SpecialWeaponType type) { return m_Specials[type]; }

    int Currently_Owned_Unit_Count() const { return m_CurrentUnitCount; }
    int Currently_Owned_Building_Count() const { return m_CurrentBuildingCount; }
    int Currently_Owned_Infantry_Count() const { return m_CurrentInfantryCount; }
    int Currently_Owned_Vessel_Count() const { return m_CurrentVesselCount; }
    int Currently_Owned_Aircraft_Count() const { return m_CurrentAircraftCount; }

    coord_t Base_Center() const { return m_BaseCenter; }

    void Code_Pointers() {}
    void Decode_Pointers() {}

    static HouseClass &As_Reference(HousesType type);
    static HouseClass *As_Pointer(HousesType type);

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
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool m_IsActive : 1; // 1
            bool m_IsHuman : 1; // 2
            bool m_PlayerControl : 1; // 4
            bool m_Production : 1; // 8
            bool m_Autocreate : 1; // 16
            bool m_AutoBaseAI : 1; // 32
            bool m_Discovered : 1; // 64
            bool m_MaxCapacity : 1; // 128

            bool m_Defeated : 1; // 1
            bool m_ToDie : 1; // 2
            bool m_ToWin : 1; // 4
            bool m_ToLose : 1; // 8
            bool m_CivEvac : 1; // 16
            bool m_RecalcNeeded : 1; // 32
            bool m_Visionary : 1; // 64
            bool m_OreShort : 1; // 128

            bool m_Bit3_1 : 1; // 1
            bool m_Bit3_2 : 1; // 2
            bool m_Repairing : 1; // 4
            bool m_MapIsClear : 1; // 8
            bool m_BuiltSomething : 1; // 16
            bool m_Resigned : 1; // 32
            bool m_GaveUp : 1; // 64
            bool m_Paranoid : 1; // 128

            bool m_AllToLook : 1; // 1
        };
        int m_Bitfield;
    };
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
    bool m_Bit3_1; // TODO: Alerted?
    bool m_Bit3_2; // TODO: Thieved / Infiltrated / Spied?
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
    int m_field_137; // BuildingType's
    int m_field_13B; // BuildingType's
    int m_field_13F; // BuildingType's       // BScan in EDWIN?
    int m_field_143; // UnitType's
    int m_field_147; // UnitType's
    int m_field_14B; // UnitType's           // UScan in EDWIN?
    int m_field_14F; // InfantryType's
    int m_field_153; // InfantryType's
    int m_field_157; // InfantryType's       // IScan in EDWIN?
    int m_field_15B; // AircraftType's
    int m_field_15F; // AircraftType's
    int m_field_163; // AircraftType's       // AScan in EDWIN?
    int m_field_167; // VesselType's
    int m_field_16B; // VesselType's
    int m_field_16F; // VesselType's         // VScan in EDWIN?
    int m_Spent;
    int m_Harvested;
    int m_Stolen;
    int m_CurrentUnitCount;
    int m_CurrentBuildingCount;
    int m_CurrentInfantryCount;
    int m_CurrentVesselCount;
    int m_CurrentAircraftCount;
    unsigned int m_Ore;
    unsigned int m_Credits;
    unsigned int m_Capacity;
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
    HousesType m_field_2AD;
    coord_t m_BaseCenter;
    int m_Radius;
    ZoneInfo m_Zones[ZONE_COUNT];
    int m_LastAttackFrame;
    RTTIType m_LastAttackType;
    ZoneType m_LastAttackZone;
    HousesType m_LastAttackHouse;
    target_t m_field_2F9; // coord of some sorts, last captured building coord? see Building::Captured().
    int m_field_2FD; // spied house bit?
    int m_Score;
    QuarryType m_PreferredTarget;
    int m_BuildingQuantity[84 /*BUILDING_COUNT*/]; //TODO: replace with enum for pre-expansion count.
    int m_UnitQuantity[14 /*UNIT_COUNT*/]; // TODO: replace with enum for pre-expansion count.
    int m_InfantryQuantity[24 /*INFANTRY_COUNT*/]; // TODO: replace with enum for pre-expansion count.
    int m_AircraftQuantity[AIRCRAFT_COUNT];
    int m_VesselQuantity[5 /*VESSEL_COUNT*/]; // TODO: replace with enum for pre-expansion count.
    TCountDownTimerClass<FrameTimerClass> m_AttackTime;
    HousesType m_field_527; //"favourite enemy", the HousesType of the one house considered the current main enemy?
    TCountDownTimerClass<FrameTimerClass> m_AITime;
    target_t m_ChronoObject;
    BuildingType m_ChosenBuilding;
    UnitType m_ChosenUnit;
    InfantryType m_ChosenInfantry;
    AircraftType m_ChosenAircraft;
    VesselType m_ChosenVessel;
    RegionClass m_ThreatRegions[34 * 34];
    cell_t m_MissileTargetCell;
    int m_Allies;
    TCountDownTimerClass<FrameTimerClass> m_DamageTime;
    TCountDownTimerClass<FrameTimerClass> m_TeamTime;
    TCountDownTimerClass<FrameTimerClass> m_TriggerTime;
    TCountDownTimerClass<FrameTimerClass> m_AttackedSpeechTime;
    TCountDownTimerClass<FrameTimerClass> m_LowPowerSpeechTime;
    TCountDownTimerClass<FrameTimerClass> m_field_177D;
    TCountDownTimerClass<FrameTimerClass> m_LowCreditsSpeechTime;
    PlayerColorType m_Color;
    char m_field_1790[12];
    char m_PlayerHandle[12];

private:
    static TFixedIHeapClass<BuildChoiceClass> g_BuildChoice;
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
extern TFixedIHeapClass<HouseClass> &g_Houses;
extern HouseClass *&g_PlayerPtr;
#else
extern TFixedIHeapClass<HouseClass> g_Houses;
extern HouseClass *g_PlayerPtr;
#endif

#endif // HOUSE_H
