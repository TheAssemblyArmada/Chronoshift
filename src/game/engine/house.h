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
class CCINIClass;

template<class T>
class TFixedIHeapClass;

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

    void Stole(unsigned int amount) { Stolen += amount; }
    unsigned int Available_Money() const { return Credits + Ore; }
    void Spend_Money(unsigned int amount);
    void Refund_Money(unsigned int amount) { Credits += amount; }
    void Silo_Redraw_Check(unsigned int a1, unsigned int a2);

    DiffType Assign_Handicap(DiffType diff);

    void Make_Ally(HousesType type);
    void Make_Enemy(HousesType type);

    fixed_t Cost_Multiplier() const { return CostMult; }
    const uint8_t *Remap_Table(BOOL unk1, RemapType type);

    BOOL Is_Human() const { return IsHuman; }

    void Code_Pointers() {}
    void Decode_Pointers() {}

private:
    RTTIType RTTI;
    int HeapID;
    GamePtr<HouseTypeClass> Type;
    DiffType AIDifficulty;
    fixed_t FirepowerMult;
    fixed_t GroundspeedMult;
    fixed_t AirspeedMult;
    fixed_t ArmorMult;
    fixed_t ROFMult;
    fixed_t CostMult;
    fixed_t BuildTimerMult;
    fixed_t RepairDelay;
    fixed_t BuildDelay;
    HouseStaticClass Static;
    HousesType ActsLike;

#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool IsActive : 1; // 1
            bool IsHuman : 1; // 2
            bool PlayerControl : 1; // 4
            bool Production : 1; // 8
            bool Autocreate : 1; // 16
            bool AutoBaseAI : 1; // 32
            bool Discovered : 1; // 64
            bool MaxCapacity : 1; // 128

            bool Defeated : 1; // 1
            bool ToDie : 1; // 2
            bool ToWin : 1; // 4
            bool ToLose : 1; // 8
            bool CivEvac : 1; // 16
            bool RecalcNeeded : 1; // 32
            bool Visionary : 1; // 64
            bool OreShort : 1; // 128

            bool Bit3_1 : 1; // 1
            bool Bit3_2 : 1; // 2
            bool Repairing : 1; // 4
            bool MapIsClear : 1; // 8
            bool BuiltSomething : 1; // 16
            bool Resigned : 1; // 32
            bool GaveUp : 1; // 64
            bool Paranoid : 1; // 128

            bool AllToLook : 1; // 1
        };
        int m_Bitfield;
    };
#else
    bool IsActive; // Is this object allocated and active for use? (def = false)
                   // NOTE: This should be set to true on class creation.
    bool IsHuman; // Is this house controlled by a human player rather than an AI [local or otherwise]?
    bool PlayerControl; // Is this house controlled by the human player [the house set to PlayerPtr]?
    bool Production;
    bool Autocreate; // Initiates autocreate for the house. This will cause the
                     // computer's house to build autocreate teams as it sees fit.
    bool AutoBaseAI; // Initialize the computer skirmish mode build control to
                     // either 'on' or 'off' state. When 'on, the computer takes
                     // over as if it were in skirmish mode.
                     // (NOTE, make sure he has at least a con yard).
    bool Discovered;
    bool MaxCapacity;
    bool Defeated;
    bool ToDie;
    bool ToWin;
    bool ToLose;
    bool CivEvac;
    bool RecalcNeeded;
    bool Visionary;
    bool OreShort;
    bool Bit3_1; // TODO: Alerted?
    bool Bit3_2; // TODO: Thieved / Infiltrated / Spied?
    bool Repairing;
    bool MapIsClear;
    bool BuiltSomething;
    bool Resigned;
    bool GaveUp;
    bool Paranoid;
    bool AllToLook;
#endif

    int CurrentIQ;
    UrgencyType Smarties;
    SuperClass Specials[SPECIAL_COUNT];
    BuildingType JustBuilding;
    InfantryType JustInfantry;
    UnitType JustUnit;
    AircraftType JustAircraft;
    VesselType JustVessel;
    int WinBlocks;
    TCountDownTimerClass<FrameTimerClass> RepairTime;
    TCountDownTimerClass<FrameTimerClass> AlertTime;
    TCountDownTimerClass<FrameTimerClass> BorrowedTime;
    int field_137; // BuildingType's
    int field_13B; // BuildingType's
    int field_13F; // BuildingType's       // BScan in EDWIN?
    int field_143; // UnitType's
    int field_147; // UnitType's
    int field_14B; // UnitType's           // UScan in EDWIN?
    int field_14F; // InfantryType's
    int field_153; // InfantryType's
    int field_157; // InfantryType's       // IScan in EDWIN?
    int field_15B; // AircraftType's
    int field_15F; // AircraftType's
    int field_163; // AircraftType's       // AScan in EDWIN?
    int field_167; // VesselType's
    int field_16B; // VesselType's
    int field_16F; // VesselType's         // VScan in EDWIN?
    int Spent;
    int Harvested;
    int Stolen;
    int CurrentUnitCount;
    int CurrentBuildingCount;
    int CurrentInfantryCount;
    int CurrentVesselCount;
    int CurrentAircraftCount;
    unsigned int Ore;
    unsigned int Credits;
    unsigned int Capacity;
    UnitTrackerClass *AircraftBought;
    UnitTrackerClass *InfantryBought;
    UnitTrackerClass *UnitsBought;
    UnitTrackerClass *BuildingsBought;
    UnitTrackerClass *VesselsBought;
    UnitTrackerClass *AircraftKilled;
    UnitTrackerClass *InfantryKilled;
    UnitTrackerClass *UnitsKilled;
    UnitTrackerClass *BuildingsKilled;
    UnitTrackerClass *VesselsKilled;
    UnitTrackerClass *BuildingsCaptured;
    UnitTrackerClass *CratesFound;
    int AircraftFactoryCount;
    int InfantryFactoryCount;
    int UnitFactoryCount;
    int VesselFactoryCount;
    int BuildingFactoryCount;
    int Power;
    int Drain;
    GamePtr<FactoryClass> AircraftFactory;
    GamePtr<FactoryClass> InfantryFactory;
    GamePtr<FactoryClass> UnitFactory;
    GamePtr<FactoryClass> VesselFactory;
    GamePtr<FactoryClass> BuildingFactory;
    target_t FlagHolder;
    cell_t FlagLocation;
    int UnitsDestroyed[HOUSES_COUNT];
    int UnitsLost;
    int BuildingsDestroyed[HOUSES_COUNT];
    int BuildingsLost;
    HousesType field_2AD;
    coord_t BaseCenter;
    int Radius;
    ZoneInfo Zones[ZONE_COUNT];
    int LastAttackFrame;
    RTTIType LastAttackType;
    ZoneType LastAttackZone;
    HousesType LastAttackHouse;
    target_t field_2F9; // coord of some sorts, last captured building coord? see Building::Captured().
    int field_2FD; // spied house bit?
    int Score;
    QuarryType PreferredTarget;
    int BuildingQuantity[84 /*BUILDING_COUNT*/]; //TODO: replace with enum for pre-expansion count.
    int UnitQuantity[14 /*UNIT_COUNT*/]; // TODO: replace with enum for pre-expansion count.
    int InfantryQuantity[24 /*INFANTRY_COUNT*/]; // TODO: replace with enum for pre-expansion count.
    int AircraftQuantity[AIRCRAFT_COUNT];
    int VesselQuantity[5 /*VESSEL_COUNT*/]; // TODO: replace with enum for pre-expansion count.
    TCountDownTimerClass<FrameTimerClass> AttackTime;
    HousesType field_527; //"favourite enemy", the HousesType of the one house considered the current main enemy?
    TCountDownTimerClass<FrameTimerClass> AITime;
    target_t ChronoObject;
    BuildingType ChosenBuilding;
    UnitType ChosenUnit;
    InfantryType ChosenInfantry;
    AircraftType ChosenAircraft;
    VesselType ChosenVessel;
    RegionClass ThreatRegions[34 * 34];
    cell_t MissileTargetCell;
    int Allies;
    TCountDownTimerClass<FrameTimerClass> DamageTime;
    TCountDownTimerClass<FrameTimerClass> TeamTime;
    TCountDownTimerClass<FrameTimerClass> TriggerTime;
    TCountDownTimerClass<FrameTimerClass> AttackedSpeechTime;
    TCountDownTimerClass<FrameTimerClass> LowPowerSpeechTime;
    TCountDownTimerClass<FrameTimerClass> field_177D;
    TCountDownTimerClass<FrameTimerClass> LowCreditsSpeechTime;
    PlayerColorType Color;
    char field_1790[12];
    char PlayerHandle[12];

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
