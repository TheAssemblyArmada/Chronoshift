/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class holding various game settings that can be loaded from rules.ini.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
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

class GameINIClass;

struct IQStruct
{
/*public:
    int Get_MaxIQLevels() const { return MaxIQLevels; }
    int Get_SuperWeapons() const { return SuperWeapons; }
    int Get_Production() const { return Production; }
    int Get_GuardArea() const { return GuardArea; }
    int Get_RepairSell() const { return RepairSell; }
    int Get_AutoCrush() const { return AutoCrush; }
    int Get_Scatter() const { return Scatter; }
    int Get_ContentScan() const { return ContentScan; }
    int Get_Aircraft() const { return Aircraft; }
    int Get_Harvester() const { return Harvester; }
    int Get_SellBack() const { return SellBack; }*/

//private:
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

    BOOL Process(GameINIClass &ini);
    void Addition(GameINIClass &ini);

    BOOL Aftermath(GameINIClass &ini);
    BOOL General(GameINIClass &ini);
    BOOL MPlayer(GameINIClass &ini);
    BOOL Recharge(GameINIClass &ini);
    BOOL Heap_Maximums(GameINIClass &ini);
    BOOL AI(GameINIClass &ini);
    BOOL Powerups(GameINIClass &ini);
    BOOL Land_Types(GameINIClass &ini);
    BOOL Themes(GameINIClass &ini);
    BOOL IQ(GameINIClass &ini);
    BOOL Objects(GameINIClass &ini);
    BOOL Difficulty(GameINIClass &ini);

    const fixed &Turbo_Boost() const { return TurboBoost; }
    const fixed &Attack_Interval() const { return AttackInterval; }
    const fixed &Attack_Delay() const { return AttackDelay; }
    const fixed &Power_Emergency() const { return PowerEmergency; }
    int Badger_Bomb_Count() const { return BadgerBombCount; }
    const fixed &Airstrip_Ratio() const { return AirstripRatio; }
    int Airstrip_Limit() const { return AirstripLimit; }
    const fixed &Helipad_Ratio() const { return HelipadRatio; }
    int Helipad_Limit() const { return HelipadLimit; }
    const fixed &Tesla_Ratio() const { return TeslaRatio; }
    int Tesla_Limit() const { return TeslaLimit; }
    const fixed &AA_Ratio() const { return AARatio; }
    int AA_Limit() const { return AALimit; }
    const fixed &Defense_Ratio() const { return DefenseRatio; }
    int Defense_Limit() const { return DefenseLimit; }
    const fixed &War_Factory_Ratio() const { return WarRatio; }
    int War_Factory_Limit() const { return WarLimit; }
    const fixed &Barracks_Ratio() const { return BarracksRatio; }
    int Barracks_Limit() const { return BarracksLimit; }
    int Refinery_Limit() const { return RefineryLimit; }
    const fixed &Refinery_Ratio() const { return RefineryRatio; }
    int Base_Size_Add() const { return BaseSizeAdd; }
    int Power_Surplus() const { return PowerSurplus; }
    int Infantry_Reserve() const { return InfantryReserve; }
    int Infantry_Base_Mult() const { return InfantryBaseMult; }
    const fixed &Chrono_Duration() const { return ChronoDuration; }
    const fixed &Water_Crate_Chance() const { return WaterCrateChance; }
    int Solo_Crate_Money() const { return SoloCrateMoney; }
    int GPS_Tech_Level() const { return GPSTechLevel; }
    UnitType Unit_CrateType() const { return UnitCrateType; }
    const fixed &Patrol_Scan() const { return PatrolScan; }
    const fixed &Team_Delay() const { return TeamDelay; }
    const fixed &Submerge_Delay() const { return SubmergeDelay; }
    const fixed &GameSpeed_Bias() const { return GameSpeedBias; }
    const fixed &Base_Bias() const { return BaseBias; }
    lepton_t Vortex_Range() const { return VortexRange; }
    MPHType Vortex_Speed() const { return VortexSpeed; }
    int Vortex_Damage() const { return VortexDamage; }
    const fixed &Vortex_Chance() const { return VortexChance; }
    const fixed &Exp_Spread() const { return ExpSpread; }
    lepton_t Fire_Supress() const { return FireSupress; }
    int Para_Tech() const { return ParaTech; }
    int Spy_Plane_Tech() const { return SpyPlaneTech; }
    int Parabomb_Tech() const { return ParabombTech; }
    const IQStruct &IQ_Controls() const { return IQControls; }
    CrateType Silver_Crate() const { return SilverCrate; }
    CrateType Wood_Crate() const { return WoodCrate; }
    CrateType Water_Crate() const { return WaterCrate; }
    int Crate_Minimum() const { return CrateMinimum; }
    int Crate_Maximum() const { return CrateMaximum; }
    lepton_t LZ_Scan_Radius() const { return LZScanRadius; }
    int MPlayer_Money() const { return MPlayerMoney; }
    int MPlayer_Max_Money() const { return MPlayerMaxMoney; }
    bool MPlayer_ShadowGrow() const { return MPlayerShadowGrow; }
    bool MPlayer_Bases() const { return MPlayerBases; }
    bool MPlayer_OreGrows() const { return MPlayerOreGrows; }
    bool MPlayer_Crates() const { return MPlayerCrates; }
    bool MPlayer_AIPlayers() const { return MPlayerAIPlayers; }
    bool MPlayer_Capture_The_Flag() const { return MPlayerCaptureTheFlag; }
    lepton_t Drop_Zone_Radius() const { return DropZoneRadius; }
    const fixed &Message_Delay() const { return MessageDelay; }
    const fixed &Savour_Delay() const { return SavourDelay; }
    int AV_Mine_Damage() const { return AVMineDamage; }
    int AP_Mine_Damage() const { return APMineDamage; }
    int Max_Players() const { return Players; }
    const fixed &Base_Defense_Delay() const { return BaseDefenseDelay; }
    int Suspend_Priority() const { return SuspendPriority; }
    const fixed &Suspend_Delay() const { return SuspendDelay; }
    const fixed &Survivor_Rate() const { return SurvivorRate; }
    const fixed &Reload_Rate() const { return ReloadRate; }
    const fixed &Autocreate_Time() const { return AutocreateTime; }
    const fixed &Buildup_Time() const { return BuildupTime; }
    int Ore_Truck_Rate() const { return OreTruckRate; }
    int Atom_Damage() const { return AtomDamage; }
    const DifficultyClass &Difficulty_Control(DiffType diff) const { return Difficulties[diff]; }
    bool Computer_Paranoid() const { return Paranoid; }
    bool Aircraft_Curley_Shuffle() const { return CurleyShuffle; }
    bool Flash_Low_Power() const { return FlashLowPower; }
    bool Computer_Easy_Bonus() const { return CompEasyBonus; }
    bool Fine_Diff_Control() const { return FineDiffControl; }
    bool Is_Ore_Explosive() const { return OreExplosive; }
    bool Show_Enemy_Health() const { return EnemyHealth; }
    bool Allow_MCV_Undeploy() const { return MCVUndeploy; }
    bool Ally_Reveal() const { return AllyReveal; }
    bool Separate_Aircraft() const { return SeparateAircraft; }
    bool Allow_Tree_Targeting() const { return TreeTargeting; }
    bool Mine_Aware() const { return MineAware; }
    bool Ore_Grows() const { return OreGrows; }
    bool Ore_Spreads() const { return OreSpreads; }
    bool Named_Civilians() const { return NamedCivilians; }
    bool Player_Auto_Crush() const { return PlayerAutoCrush; }
    bool Player_Return_Fire() const { return PlayerReturnFire; }
    bool Player_Scatter() const { return PlayerScatter; }
    bool Chrono_Kill_Cargo() const { return ChronoKillCargo; }
    bool Secret_Units_Enabled() const { return SecretUnitsEnabled; }
    bool New_Units_Enabled() const { return NewUnitsEnabled; }
    const fixed &Prone_Damage() const { return ProneDamage; }
    const fixed &Quake_Damage() const { return QuakeDamage; }
    const fixed &Quake_Chance() const { return QuakeChance; }
    const fixed &Ore_Growth_Rate() const { return GrowthRate; }
    const fixed &Shroud_Rate() const { return ShroudRate; }
    const fixed &Crate_Regen() const { return CrateRegen; }
    const fixed &Get_Timer_Warning() const { return TimerWarning; }
    const fixed &Get_Recharge_Sonar() const { return RechargeSonar; }
    const fixed &Get_Recharge_Chrono() const { return RechargeChrono; }
    const fixed &Get_Recharge_ParaBomb() const { return RechargeParaBomb; }
    const fixed &Get_Recharge_Paratrooper() const { return RechargeParatrooper; }
    const fixed &Get_Recharge_ParaSaboteur() const { return RechargeParaSaboteur; }
    const fixed &Get_Recharge_SpyPlane() const { return RechargeSpyPlane; }
    const fixed &Get_Recharge_IronCurtain() const { return RechargeIronCurtain; }
    const fixed &Get_Recharge_GPS() const { return RechargeGPS; }
    const fixed &Get_Recharge_Nuke() const { return RechargeNuke; }
    const fixed &Get_Speak_Delay() const { return SpeakDelay; }
    const fixed &Get_Damage_Delay() const { return DamageDelay; }
    int Get_Gravity() const { return Gravity; }
    int Gap_Radius() const { return GapRadius; }
    const fixed &Gap_Regen_Interval() const { return GapRegenInterval; }
    lepton_t Radar_Jam_Radius() const { return RadarJamRadius; }
    MPHType Get_Incoming() const { return Incoming; }
    int Min_Damage() const { return MinDamage; }
    int Max_Damage() const { return MaxDamage; }
    int Get_Repair_Step() const { return RepairStep; }
    const fixed &Get_Repair_Percent() const { return RepairPercent; }
    int Get_Unit_Repair_Step() const { return URepairStep; }
    const fixed &Get_Unit_Repair_Percent() const { return URepairPercent; }
    const fixed &Get_Repair_Rate() const { return RepairRate; }
    const fixed &Condition_Green() const { return ConditionGreen; }
    const fixed &Condition_Yellow() const { return ConditionYellow; }
    const fixed &Condition_Red() const { return ConditionRed; }
    const fixed &Idle_Action_Frequency() const { return IdleActionFrequency; }
    int Bail_Count() const { return BailCount; }
    int Get_Gold_Value() const { return GoldValue; }
    int Get_Gem_Value() const { return GemValue; }
    int Max_Aircraft() const { return MaxAircraft; }
    int Max_Anim() const { return MaxAnim; }
    int Max_Building() const { return MaxBuilding; }
    int Max_Bullet() const { return MaxBullet; }
    int Max_Factory() const { return MaxFactory; }
    int Max_Infantry() const { return MaxInfantry; }
    int Max_Overlay() const { return MaxOverlay; }
    int Max_Smudge() const { return MaxSmudge; }
    int Max_Team() const { return MaxTeam; }
    int Max_TeamType() const { return MaxTeamType; }
    int Max_Template() const { return MaxTemplate; }
    int Max_Terrain() const { return MaxTerrain; }
    int Max_Trigger() const { return MaxTrigger; }
    int Max_Unit() const { return MaxUnit; }
    int Max_Vessel() const { return MaxVessel; }
    int Max_Projectile() const { return MaxProjectile; }
    int Max_Weapon() const { return MaxWeapon; }
    int Max_Warhead() const { return MaxWarhead; }
    int Max_TrigType() const { return MaxTrigType; }
    lepton_t Close_Enough_Distance() const { return CloseEnough; }
    lepton_t Stray_Distance() const { return Stray; }
    lepton_t Crush_Distance() const { return Crush; }
    lepton_t Crate_Radius() const { return CrateRadius; }
    lepton_t Homing_Scatter_Distance() const { return HomingScatter; }
    lepton_t Ballistic_Scatter_Distance() const { return BallisticScatter; }
    const fixed &Refund_Percent() const { return RefundPercent; }
    const fixed &Get_Iron_Curtain() const { return IronCurtain; }
    int Bridge_Strength() const { return BridgeStrength; }
    const fixed &Get_Build_Speed() const { return BuildSpeed; }
    const fixed &C4_Delay() const { return C4Delay; }
    int Credit_Reserve() const { return CreditReserve; }
    const fixed &Path_Delay() const { return PathDelay; }
    const fixed &Movie_Time() const { return MovieTime; }
    int Chrono_Tech_Level() const { return ChronoTechLevel; }
    lepton_t Ore_Near_Scan() const { return OreNearScan; }
    lepton_t Ore_Far_Scan() const { return OreFarScan; }
    const fixed &Engineer_Damage() const { return EngineerDamage; }
    const fixed &Engineer_Capture_Level() const { return EngineerCaptureLevel; }
    const fixed &Chrono_Tank_Duration() const { return ChronoTankDuration; }
    int MTank_Distance() const { return MTankDistance; }
    int Carrier_Launch_Delay() const { return CarrierLaunchDelay; }
    const fixed &Quake_Unit_Damage() const { return QuakeUnitDamage; }
    const fixed &Quake_Vessel_Damage() const { return QuakeVesselDamage; }
    const fixed &Quake_Building_Damage() const { return QuakeBuildingDamage; }
    int Quake_Infantry_Damage() const { return QuakeInfantryDamage; }
    int Quake_Delay() const { return QuakeDelay; }
    const fixed &Cloak_Delay() const { return CloakDelay; }

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
    lepton_t VortexRange;
    MPHType VortexSpeed;
    int VortexDamage;
    fixed VortexChance;
    fixed ExpSpread;
    lepton_t FireSupress;
    int ParaTech;
    int SpyPlaneTech;
    int ParabombTech;
    IQStruct IQControls;
    CrateType SilverCrate;
    CrateType WoodCrate;
    CrateType WaterCrate;
    int CrateMinimum;
    int CrateMaximum;
    lepton_t LZScanRadius; // Maximum radius to scan for alternate landing zone if otherwise occupied.
    int MPlayerMoney;
    int MPlayerMaxMoney;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool MPlayerShadowGrow : 1; // 1
            bool MPlayerBases : 1; // 2
            bool MPlayerOreGrows : 1; // 4
            bool MPlayerCrates : 1; // 8
            bool MPlayerAIPlayers : 1; // 16
            bool MPlayerCaptureTheFlag : 1; // 32
        };
        int MPBitfield;
    };
#else
    bool MPlayerShadowGrow; // 1
    bool MPlayerBases; // 2
    bool MPlayerOreGrows; // 4
    bool MPlayerCrates; // 8
    bool MPlayerAIPlayers; // 16
    bool MPlayerCaptureTheFlag; // 32
#endif
    lepton_t DropZoneRadius;
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
#ifndef CHRONOSHIFT_NO_BITFIELDS
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
    lepton_t RadarJamRadius;
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
    lepton_t CloseEnough;
    lepton_t Stray;
    lepton_t Crush;
    lepton_t CrateRadius;
    lepton_t HomingScatter;
    lepton_t BallisticScatter;
    fixed RefundPercent;
    fixed IronCurtain;
    int BridgeStrength;
    fixed BuildSpeed;
    fixed C4Delay;
    int CreditReserve;
    fixed PathDelay;
    fixed MovieTime;
    int ChronoTechLevel;
    lepton_t OreNearScan;
    lepton_t OreFarScan;
#ifndef CHRONOSHIFT_STANDALONE
    static fixed &EngineerDamage;
    static fixed &EngineerCaptureLevel;
    static fixed &ChronoTankDuration;
    static int &MTankDistance;
    static int &CarrierLaunchDelay;
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
    int CarrierLaunchDelay;
    fixed QuakeUnitDamage;
    fixed QuakeVesselDamage; // New additional control for Chronoshift.
    fixed QuakeBuildingDamage;
    int QuakeInfantryDamage;
    int QuakeDelay;
    fixed CloakDelay; // New additional control for Chronoshift, to be moved next to SubmergeDelay.
#endif
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"

extern RulesClass &Rule;
#else
extern RulesClass Rule;
#endif

#endif // RULES_H
