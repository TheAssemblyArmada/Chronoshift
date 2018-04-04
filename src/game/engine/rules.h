/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class holding various game settings that can be loaded from rules.ini.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef RULES_H
#define RULES_H

#include "always.h"
#include "crate.h"
#include "difficulty.h"
#include "fixed.h"
#include "gametypes.h"
#include "unittype.h"

class CCINIClass;

struct IQStruct
{
    int MaxIQLevels; // the maximum number of discrete IQ levels
    int SuperWeapons; // super weapons are automatically fired by computer
    int Production; // building/unit production is automatically controlled by computer
    int GuardArea; // newly produced units start in guard area mode
    int RepairSell; // allowed to choose repair or sell of damaged buildings
    int AutoCrush; // automatically try to crush antogonists if possible
    int Scatter; // will scatter from incoming threats [grenades and such]
    int ContentScan; // will consider contents of transport when picking good target
    int Aircraft; // automatically replace aircraft
    int Harvester; // automatically replace lost harvesters
    int SellBack; // allowed to sell buildings
};

class RulesClass
{
public:
    RulesClass();
    ~RulesClass() {}

    BOOL Process(CCINIClass &ini);
    void Addition(CCINIClass &ini);

    BOOL Aftermath(CCINIClass &ini);
    BOOL General(CCINIClass &ini);
    BOOL MPlayer(CCINIClass &ini);
    BOOL Recharge(CCINIClass &ini);
    BOOL Heap_Maximums(CCINIClass &ini);
    BOOL AI(CCINIClass &ini);
    BOOL Powerups(CCINIClass &ini);
    BOOL Land_Types(CCINIClass &ini);
    BOOL Themes(CCINIClass &ini);
    BOOL IQ(CCINIClass &ini);
    BOOL Objects(CCINIClass &ini);
    BOOL Difficulty(CCINIClass &ini);

    BOOL Ore_Grows() const { return OreGrows; }
    BOOL Ore_Spreads() const { return OreSpreads; }
    fixed Ore_Growth_Rate() const { return GrowthRate; }
    int Get_Gap_Radius() const { return GapRadius; }
    int Get_Gold_Value() const { return GoldValue; }
    int Get_Gem_Value() const { return GemValue; }
    const fixed &Get_Build_Speed() const { return BuildSpeed; }
    int Get_Repair_Step() const { return RepairStep; }
    const fixed &Get_Repair_Percent() const { return RepairPercent; }
    int Get_Unit_Repair_Step() const { return URepairStep; }
    const fixed &Get_Unit_Repair_Percent() const { return URepairPercent; }

private:
    fixed TurboBoost;
    fixed AttackInterval;
    fixed AttackDelay;
    fixed PowerEmergency;
    int BadgerBombCount;
    fixed AirstripRatio;
    int AirstripLimit;
    fixed HelipadRatio;
    int HelipadLimit;
    fixed TeslaRatio;
    int TeslaLimit;
    fixed AARatio;
    int AALimit;
    fixed DefenseRatio;
    int DefenseLimit;
    fixed WarRatio;
    int WarLimit;
    fixed BarracksRatio;
    int BarracksLimit;
    int RefineryLimit;
    fixed RefineryRatio;
    int BaseSizeAdd;
    int PowerSurplus;
    int InfantryReserve;
    int InfantryBaseMult;
    fixed ChronoDuration;
    fixed WaterCrateChance;
    int SoloCrateMoney;
    int GPSTechLevel;
    UnitType UnitCrateType;
    fixed PatrolScan;
    fixed TeamDelay;
    fixed SubmergeDelay;
    fixed GameSpeedBias;
    fixed BaseBias;
    int16_t VortexRange;
    MPHType VortexSpeed;
    int VortexDamage;
    fixed VortexChance;
    fixed ExpSpread;
    int16_t FireSupress;
    int ParaTech;
    int SpyPlaneTech;
    int ParabombTech;
    IQStruct IQControls;
    CrateType SilverCrate;
    CrateType WoodCrate;
    CrateType WaterCrate;
    int CrateMinimum;
    int CrateMaximum;
    int16_t LZScanRadius; // Maximum radius to scan for alternate landing zone if otherwise occupied.
    int MPlayerMoney;
    int MPlayerMaxMoney;
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool MPlayerShadowGrow : 1; // 1
            bool MPlayerBases : 1; // 2
            bool MPlayerOreGrows : 1; // 4
            bool MPlayerCrates : 1; // 8
            bool MPlayerBit16 : 1; // 16
            bool MPlayerCaptureTheFlag : 1; // 32
        };
        int MPBitfield;
    };
#else
    bool MPlayerShadowGrow; // 1
    bool MPlayerBases; // 2
    bool MPlayerOreGrows; // 4
    bool MPlayerCrates; // 8
    bool MPlayerBit16; // 16
    bool MPlayerCaptureTheFlag; // 32
#endif
    int16_t DropZoneRadius;
    fixed MessageDelay;
    fixed SavourDelay;
    int AVMineDamage;
    int APMineDamage;
    int Players;
    fixed BaseDefenseDelay;
    int SuspendPriority;
    fixed SuspendDelay;
    fixed SurvivorRate;
    fixed ReloadRate;
    fixed AutocreateTime;
    fixed BuildupTime;
    int OreTruckRate;
    int AtomDamage;
    DifficultyClass Difficulties[3];
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool Paranoid : 1; // 1
            bool CurleyShuffle : 1; // 2
            bool FlashLowPower : 1; // 4
            bool CompEasyBonus : 1; // 8
            bool FineDiffControl : 1; // 16
            bool OreExplosive : 1; // 32
            bool EnemyHealth : 1; // 64
            bool MCVUndeploy : 1; // 128
            bool AllyReveal : 1; // 1
            bool SeparateAircraft : 1; // 2
            bool TreeTargeting : 1; // 4
            bool MineAware : 1; // 8
            bool OreGrows : 1; // 16
            bool OreSpreads : 1; // 32
            bool NamedCivilians : 1; // 64
            bool PlayerAutoCrush : 1; // 128
            bool PlayerReturnFire : 1; // 1
            bool PlayerScatter : 1; // 2
            bool ChronoKillCargo : 1; // 4
            bool SecretUnitsEnabled : 1;
            bool NewUnitsEnabled : 1;
        };
        int Bitfield;
    };
#else
    bool Paranoid; // 1
    bool CurleyShuffle; // 2
    bool FlashLowPower; // 4
    bool CompEasyBonus; // 8
    bool FineDiffControl; // 16
    bool OreExplosive; // 32
    bool EnemyHealth; // 64
    bool MCVUndeploy; // 128
    bool AllyReveal; // 1
    bool SeparateAircraft; // 2
    bool TreeTargeting; // 4
    bool MineAware; // 8
    bool OreGrows; // 16
    bool OreSpreads; // 32
    bool NamedCivilians; // 64
    bool PlayerAutoCrush; // 128
    bool PlayerReturnFire; // 1
    bool PlayerScatter; // 2
    bool ChronoKillCargo; // 4
    bool SecretUnitsEnabled;
    bool NewUnitsEnabled;
#endif
    fixed ProneDamage;
    fixed QuakeDamage;
    fixed QuakeChance;
    fixed GrowthRate;
    fixed ShroudRate;
    fixed CrateRegen;
    fixed TimerWarning;
    fixed RechargeSonar;
    fixed RechargeChrono;
    fixed RechargeParaBomb;
    fixed RechargeParatrooper;
    fixed RechargeParaSaboteur;
    fixed RechargeSpyPlane;
    fixed RechargeIronCurtain;
    fixed RechargeGPS;
    fixed RechargeNuke;
    fixed SpeakDelay;
    fixed DamageDelay;
    int Gravity;
    int GapRadius;
    fixed GapRegenInterval;
    int16_t RadarJamRadius;
    MPHType Incoming;
    int MinDamage;
    int MaxDamage;
    int RepairStep;
    fixed RepairPercent;
    int URepairStep;
    fixed URepairPercent;
    fixed RepairRate;
    fixed ConditionGreen;
    fixed ConditionYellow;
    fixed ConditionRed;
    fixed IdleActionFrequency;
    int BailCount;
    int GoldValue;
    int GemValue;
    int MaxAircraft;
    int MaxAnim;
    int MaxBuilding;
    int MaxBullet;
    int MaxFactory;
    int MaxInfantry;
    int MaxOverlay;
    int MaxSmudge;
    int MaxTeam;
    int MaxTeamType;
    int MaxTemplate;
    int MaxTerrain;
    int MaxTrigger;
    int MaxUnit;
    int MaxVessel;
    int MaxProjectile;
    int MaxWeapon;
    int MaxWarhead;
    int MaxTrigType;
    int16_t CloseEnough;
    int16_t Stray;
    int16_t Crush;
    int16_t CrateRadius;
    int16_t HomingScatter;
    int16_t BallisticScatter;
    fixed RefundPercent;
    fixed IronCurtain;
    int BridgeStrength;
    fixed BuildSpeed;
    fixed C4Delay;
    int CreditReserve;
    fixed PathDelay;
    fixed MovieTime;
    int ChronoTechLevel;
    int16_t OreNearScan;
    int16_t OreFarScan;
#ifndef RAPP_STANDALONE
    static fixed &EngineerDamage;
    static fixed &EngineerCaptureLevel;
    static fixed &ChronoTankDuration;
    static int &MTankDistance;
    static int CarrierLaunchDelay;
    static fixed &QuakeUnitDamage;
    static fixed QuakeVesselDamage;
    static fixed &QuakeBuildingDamage;
    static int &QuakeInfantryDamage;
    static int &QuakeDelay;
    static BOOL &OrigNewUnitsEnabled;
    static BOOL &OrigSecretUnitsEnabled;
    static fixed CloakDelay;
#else
    // If we aren't trying to match the ABI, make these part of the class.
    fixed EngineerDamage;
    fixed EngineerCaptureLevel;
    fixed ChronoTankDuration;
    int MTankDistance;
    int CarrierLaunchDelay; // New additional control for RA++.
    fixed QuakeUnitDamage;
    fixed QuakeVesselDamage; // New additional control for RA++.
    fixed QuakeBuildingDamage;
    int QuakeInfantryDamage;
    int QuakeDelay;
    fixed CloakDelay; // New additional control for RA++.
#endif
};

#ifndef RAPP_STANDALONE
#include "hooker.h"

extern RulesClass &Rule;
#else
extern RulesClass Rule;
#endif

#endif // RULES_H
