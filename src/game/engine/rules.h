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
    int Get_MaxIQLevels() const { return m_MaxIQLevels; }
    int Get_SuperWeapons() const { return m_SuperWeapons; }
    int Get_Production() const { return m_Production; }
    int Get_GuardArea() const { return m_GuardArea; }
    int Get_RepairSell() const { return m_RepairSell; }
    int Get_AutoCrush() const { return m_AutoCrush; }
    int Get_Scatter() const { return m_Scatter; }
    int Get_ContentScan() const { return m_ContentScan; }
    int Get_Aircraft() const { return m_Aircraft; }
    int Get_Harvester() const { return m_Harvester; }
    int Get_SellBack() const { return m_SellBack; }*/

//private:
    int m_MaxIQLevels; // the maximum number of discrete IQ levels
    int m_SuperWeapons; // super weapons are automatically fired by computer
    int m_Production; // building/unit production is automatically controlled by computer
    int m_GuardArea; // newly produced units start in guard area mode
    int m_RepairSell; // allowed to choose repair or sell of damaged buildings
    int m_AutoCrush; // automatically try to crush antogonists if possible
    int m_Scatter; // will scatter from incoming threats [grenades and such]
    int m_ContentScan; // will consider contents of transport when picking good target
    int m_Aircraft; // automatically replace aircraft
    int m_Harvester; // automatically replace lost harvesters
    int m_SellBack; // allowed to sell buildings
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
    BOOL s_Themes(GameINIClass &ini);
    BOOL IQ(GameINIClass &ini);
    BOOL Objects(GameINIClass &ini);
    BOOL Difficulty(GameINIClass &ini);

    const fixed_t &Turbo_Boost() const { return m_TurboBoost; }
    const fixed_t &Attack_Interval() const { return m_AttackInterval; }
    const fixed_t &Attack_Delay() const { return m_AttackDelay; }
    const fixed_t &Power_Emergency() const { return m_PowerEmergency; }
    int Badger_Bomb_Count() const { return m_BadgerBombCount; }
    const fixed_t &Airstrip_Ratio() const { return m_AirstripRatio; }
    int Airstrip_Limit() const { return m_AirstripLimit; }
    const fixed_t &Helipad_Ratio() const { return m_HelipadRatio; }
    int Helipad_Limit() const { return m_HelipadLimit; }
    const fixed_t &Tesla_Ratio() const { return m_TeslaRatio; }
    int Tesla_Limit() const { return m_TeslaLimit; }
    const fixed_t &AA_Ratio() const { return m_AARatio; }
    int AA_Limit() const { return m_AALimit; }
    const fixed_t &Defense_Ratio() const { return m_DefenseRatio; }
    int Defense_Limit() const { return m_DefenseLimit; }
    const fixed_t &War_Factory_Ratio() const { return m_WarRatio; }
    int War_Factory_Limit() const { return m_WarLimit; }
    const fixed_t &Barracks_Ratio() const { return m_BarracksRatio; }
    int Barracks_Limit() const { return m_BarracksLimit; }
    int Refinery_Limit() const { return m_RefineryLimit; }
    const fixed_t &Refinery_Ratio() const { return m_RefineryRatio; }
    int Base_Size_Add() const { return m_BaseSizeAdd; }
    int Power_Surplus() const { return m_PowerSurplus; }
    int Infantry_Reserve() const { return m_InfantryReserve; }
    int Infantry_Base_Mult() const { return m_InfantryBaseMult; }
    const fixed_t &Chrono_Duration() const { return m_ChronoDuration; }
    const fixed_t &Water_Crate_Chance() const { return m_WaterCrateChance; }
    int Solo_Crate_Money() const { return m_SoloCrateMoney; }
    int GPS_Tech_Level() const { return m_GPSTechLevel; }
    UnitType Unit_CrateType() const { return m_UnitCrateType; }
    const fixed_t &Patrol_Scan() const { return m_PatrolScan; }
    const fixed_t &Team_Delay() const { return m_TeamDelay; }
    const fixed_t &Submerge_Delay() const { return m_SubmergeDelay; }
    const fixed_t &GameSpeed_Bias() const { return m_GameSpeedBias; }
    const fixed_t &Base_Bias() const { return m_BaseBias; }
    lepton_t Vortex_Range() const { return m_VortexRange; }
    MPHType Vortex_Speed() const { return m_VortexSpeed; }
    int Vortex_Damage() const { return m_VortexDamage; }
    const fixed_t &Vortex_Chance() const { return m_VortexChance; }
    const fixed_t &Exp_Spread() const { return m_ExpSpread; }
    lepton_t Fire_Supress() const { return m_FireSupress; }
    int Para_Tech() const { return m_ParaTech; }
    int Spy_Plane_Tech() const { return m_SpyPlaneTech; }
    int Parabomb_Tech() const { return m_ParabombTech; }
    const IQStruct &IQ_Controls() const { return m_IQControls; }
    CrateType Silver_Crate() const { return m_SilverCrate; }
    CrateType Wood_Crate() const { return m_WoodCrate; }
    CrateType Water_Crate() const { return m_WaterCrate; }
    int Crate_Minimum() const { return m_CrateMinimum; }
    int Crate_Maximum() const { return m_CrateMaximum; }
    lepton_t LZ_Scan_Radius() const { return m_LZScanRadius; }
    int MPlayer_Money() const { return m_MPlayerMoney; }
    int MPlayer_Max_Money() const { return m_MPlayerMaxMoney; }
    bool MPlayer_ShadowGrow() const { return m_MPlayerShadowGrow; }
    bool MPlayer_Bases() const { return m_MPlayerBases; }
    bool MPlayer_OreGrows() const { return m_MPlayerOreGrows; }
    bool MPlayer_Crates() const { return m_MPlayerCrates; }
    bool MPlayer_AIPlayers() const { return m_MPlayerAIPlayers; }
    bool MPlayer_Capture_The_Flag() const { return m_MPlayerCaptureTheFlag; }
    lepton_t Drop_Zone_Radius() const { return m_DropZoneRadius; }
    const fixed_t &Message_Delay() const { return m_MessageDelay; }
    const fixed_t &Savour_Delay() const { return m_SavourDelay; }
    int AV_Mine_Damage() const { return m_AVMineDamage; }
    int AP_Mine_Damage() const { return m_APMineDamage; }
    int Max_Players() const { return m_Players; }
    const fixed_t &Base_Defense_Delay() const { return m_BaseDefenseDelay; }
    int Suspend_Priority() const { return m_SuspendPriority; }
    const fixed_t &Suspend_Delay() const { return m_SuspendDelay; }
    const fixed_t &Survivor_Rate() const { return m_SurvivorRate; }
    const fixed_t &Reload_Rate() const { return m_ReloadRate; }
    const fixed_t &Autocreate_Time() const { return m_AutocreateTime; }
    const fixed_t &Buildup_Time() const { return m_BuildupTime; }
    int Ore_Truck_Rate() const { return m_OreTruckRate; }
    int Atom_Damage() const { return m_AtomDamage; }
    const DifficultyClass &Difficulty_Control(DiffType diff) const { return m_Difficulties[diff]; }
    bool Computer_Paranoid() const { return m_Paranoid; }
    bool Aircraft_Curley_Shuffle() const { return m_CurleyShuffle; }
    bool Flash_Low_Power() const { return m_FlashLowPower; }
    bool Computer_Easy_Bonus() const { return m_CompEasyBonus; }
    bool Fine_Diff_Control() const { return m_FineDiffControl; }
    bool Is_Ore_Explosive() const { return m_OreExplosive; }
    bool Show_Enemy_Health() const { return m_EnemyHealth; }
    bool Allow_MCV_Undeploy() const { return m_MCVUndeploy; }
    bool Ally_Reveal() const { return m_AllyReveal; }
    bool Separate_Aircraft() const { return m_SeparateAircraft; }
    bool Allow_Tree_Targeting() const { return m_TreeTargeting; }
    bool Mine_Aware() const { return m_MineAware; }
    bool Ore_Grows() const { return m_OreGrows; }
    bool Ore_Spreads() const { return m_OreSpreads; }
    bool Named_Civilians() const { return m_NamedCivilians; }
    bool Player_Auto_Crush() const { return m_PlayerAutoCrush; }
    bool Player_Return_Fire() const { return m_PlayerReturnFire; }
    bool Player_Scatter() const { return m_PlayerScatter; }
    bool Chrono_Kills_Cargo() const { return m_ChronoKillCargo; }
    bool Secret_Units_Enabled() const { return m_SecretUnitsEnabled; }
    bool New_Units_Enabled() const { return m_NewUnitsEnabled; }
    const fixed_t &Prone_Damage() const { return m_ProneDamage; }
    const fixed_t &Quake_Damage() const { return m_QuakeDamage; }
    const fixed_t &Quake_Chance() const { return m_QuakeChance; }
    const fixed_t &Ore_Growth_Rate() const { return m_GrowthRate; }
    const fixed_t &Shroud_Rate() const { return m_ShroudRate; }
    const fixed_t &Crate_Regen() const { return m_CrateRegen; }
    const fixed_t &Get_Timer_Warning() const { return m_TimerWarning; }
    const fixed_t &Get_Recharge_Sonar() const { return m_RechargeSonar; }
    const fixed_t &Get_Recharge_Chrono() const { return m_RechargeChrono; }
    const fixed_t &Get_Recharge_ParaBomb() const { return m_RechargeParaBomb; }
    const fixed_t &Get_Recharge_Paratrooper() const { return m_RechargeParatrooper; }
    const fixed_t &Get_Recharge_ParaSaboteur() const { return m_RechargeParaSaboteur; }
    const fixed_t &Get_Recharge_SpyPlane() const { return m_RechargeSpyPlane; }
    const fixed_t &Get_Recharge_IronCurtain() const { return m_RechargeIronCurtain; }
    const fixed_t &Get_Recharge_GPS() const { return m_RechargeGPS; }
    const fixed_t &Get_Recharge_Nuke() const { return m_RechargeNuke; }
    const fixed_t &Get_Speak_Delay() const { return m_SpeakDelay; }
    const fixed_t &Get_Damage_Delay() const { return m_DamageDelay; }
    int Get_Gravity() const { return m_Gravity; }
    int Gap_Radius() const { return m_GapRadius; }
    const fixed_t &Gap_Regen_Interval() const { return m_GapRegenInterval; }
    lepton_t Radar_Jam_Radius() const { return m_RadarJamRadius; }
    MPHType Get_Incoming() const { return m_Incoming; }
    int Min_Damage() const { return m_MinDamage; }
    int Max_Damage() const { return m_MaxDamage; }
    int Get_Repair_Step() const { return m_RepairStep; }
    const fixed_t &Get_Repair_Percent() const { return m_RepairPercent; }
    int Get_Unit_Repair_Step() const { return m_URepairStep; }
    const fixed_t &Get_Unit_Repair_Percent() const { return m_URepairPercent; }
    const fixed_t &Get_Repair_Rate() const { return m_RepairRate; }
    const fixed_t &Condition_Green() const { return m_ConditionGreen; }
    const fixed_t &Condition_Yellow() const { return m_ConditionYellow; }
    const fixed_t &Condition_Red() const { return m_ConditionRed; }
    const fixed_t &Idle_Action_Frequency() const { return m_IdleActionFrequency; }
    int Bail_Count() const { return m_BailCount; }
    int Get_Gold_Value() const { return m_GoldValue; }
    int Get_Gem_Value() const { return m_GemValue; }
    int Max_Aircraft() const { return m_MaxAircraft; }
    int Max_Anim() const { return m_MaxAnim; }
    int Max_Building() const { return m_MaxBuilding; }
    int Max_Bullet() const { return m_MaxBullet; }
    int Max_Factory() const { return m_MaxFactory; }
    int Max_Infantry() const { return m_MaxInfantry; }
    int Max_Overlay() const { return m_MaxOverlay; }
    int Max_Smudge() const { return m_MaxSmudge; }
    int Max_Team() const { return m_MaxTeam; }
    int Max_TeamType() const { return m_MaxTeamType; }
    int Max_Template() const { return m_MaxTemplate; }
    int Max_Terrain() const { return m_MaxTerrain; }
    int Max_Trigger() const { return m_MaxTrigger; }
    int Max_Unit() const { return m_MaxUnit; }
    int Max_Vessel() const { return m_MaxVessel; }
    int Max_Projectile() const { return m_MaxProjectile; }
    int Max_Weapon() const { return m_MaxWeapon; }
    int Max_Warhead() const { return m_MaxWarhead; }
    int Max_TrigType() const { return m_MaxTrigType; }
    lepton_t Close_Enough_Distance() const { return m_CloseEnough; }
    lepton_t Stray_Distance() const { return m_Stray; }
    lepton_t Crush_Distance() const { return m_Crush; }
    lepton_t Crate_Radius() const { return m_CrateRadius; }
    lepton_t Homing_Scatter_Distance() const { return m_HomingScatter; }
    lepton_t Ballistic_Scatter_Distance() const { return m_BallisticScatter; }
    const fixed_t &Refund_Percent() const { return m_RefundPercent; }
    const fixed_t &Get_Iron_Curtain() const { return m_IronCurtain; }
    int Bridge_Strength() const { return m_BridgeStrength; }
    const fixed_t &Get_Build_Speed() const { return m_BuildSpeed; }
    const fixed_t &C4_Delay() const { return m_C4Delay; }
    int Credit_Reserve() const { return m_CreditReserve; }
    const fixed_t &Path_Delay() const { return m_PathDelay; }
    const fixed_t &Movie_Time() const { return m_MovieTime; }
    int Chrono_Tech_Level() const { return m_ChronoTechLevel; }
    lepton_t Ore_Near_Scan() const { return m_OreNearScan; }
    lepton_t Ore_Far_Scan() const { return m_OreFarScan; }
    const fixed_t &Engineer_Damage() const { return m_EngineerDamage; }
    const fixed_t &Engineer_Capture_Level() const { return m_EngineerCaptureLevel; }
    const fixed_t &Chrono_Tank_Duration() const { return m_ChronoTankDuration; }
    int MTank_Distance() const { return m_MTankDistance; }
    int Carrier_Launch_Delay() const { return m_CarrierLaunchDelay; }
    const fixed_t &Quake_Unit_Damage() const { return m_QuakeUnitDamage; }
    const fixed_t &Quake_Vessel_Damage() const { return m_QuakeVesselDamage; }
    const fixed_t &Quake_Building_Damage() const { return m_QuakeBuildingDamage; }
    int Quake_Infantry_Damage() const { return m_QuakeInfantryDamage; }
    int Quake_Delay() const { return m_QuakeDelay; }
    const fixed_t &Cloak_Delay() const { return m_CloakDelay; }

private:
    fixed_t m_TurboBoost;
    fixed_t m_AttackInterval;
    fixed_t m_AttackDelay;
    fixed_t m_PowerEmergency;
    int m_BadgerBombCount;
    fixed_t m_AirstripRatio;
    int m_AirstripLimit;
    fixed_t m_HelipadRatio;
    int m_HelipadLimit;
    fixed_t m_TeslaRatio;
    int m_TeslaLimit;
    fixed_t m_AARatio;
    int m_AALimit;
    fixed_t m_DefenseRatio;
    int m_DefenseLimit;
    fixed_t m_WarRatio;
    int m_WarLimit;
    fixed_t m_BarracksRatio;
    int m_BarracksLimit;
    int m_RefineryLimit;
    fixed_t m_RefineryRatio;
    int m_BaseSizeAdd;
    int m_PowerSurplus;
    int m_InfantryReserve;
    int m_InfantryBaseMult;
    fixed_t m_ChronoDuration;
    fixed_t m_WaterCrateChance;
    int m_SoloCrateMoney;
    int m_GPSTechLevel;
    UnitType m_UnitCrateType;
    fixed_t m_PatrolScan;
    fixed_t m_TeamDelay;
    fixed_t m_SubmergeDelay;
    fixed_t m_GameSpeedBias;
    fixed_t m_BaseBias;
    lepton_t m_VortexRange;
    MPHType m_VortexSpeed;
    int m_VortexDamage;
    fixed_t m_VortexChance;
    fixed_t m_ExpSpread;
    lepton_t m_FireSupress;
    int m_ParaTech;
    int m_SpyPlaneTech;
    int m_ParabombTech;
    IQStruct m_IQControls;
    CrateType m_SilverCrate;
    CrateType m_WoodCrate;
    CrateType m_WaterCrate;
    int m_CrateMinimum;
    int m_CrateMaximum;
    lepton_t m_LZScanRadius; // Maximum radius to scan for alternate landing zone if otherwise occupied.
    int m_MPlayerMoney;
    int m_MPlayerMaxMoney;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_MPlayerShadowGrow : 1; // 1
    BOOL m_MPlayerBases : 1; // 2
    BOOL m_MPlayerOreGrows : 1; // 4
    BOOL m_MPlayerCrates : 1; // 8
    BOOL m_MPlayerAIPlayers : 1; // 16
    BOOL m_MPlayerCaptureTheFlag : 1; // 32
#else
    bool m_MPlayerShadowGrow; // 1
    bool m_MPlayerBases; // 2
    bool m_MPlayerOreGrows; // 4
    bool m_MPlayerCrates; // 8
    bool m_MPlayerAIPlayers; // 16
    bool m_MPlayerCaptureTheFlag; // 32
#endif
    lepton_t m_DropZoneRadius;
    fixed_t m_MessageDelay;
    fixed_t m_SavourDelay;
    int m_AVMineDamage;
    int m_APMineDamage;
    int m_Players;
    fixed_t m_BaseDefenseDelay;
    int m_SuspendPriority;
    fixed_t m_SuspendDelay;
    fixed_t m_SurvivorRate;
    fixed_t m_ReloadRate;
    fixed_t m_AutocreateTime;
    fixed_t m_BuildupTime;
    int m_OreTruckRate;
    int m_AtomDamage;
    DifficultyClass m_Difficulties[3];
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_Paranoid : 1; // 1
    BOOL m_CurleyShuffle : 1; // 2
    BOOL m_FlashLowPower : 1; // 4
    BOOL m_CompEasyBonus : 1; // 8
    BOOL m_FineDiffControl : 1; // 16
    BOOL m_OreExplosive : 1; // 32
    BOOL m_EnemyHealth : 1; // 64
    BOOL m_MCVUndeploy : 1; // 128

    BOOL m_AllyReveal : 1; // 1
    BOOL m_SeparateAircraft : 1; // 2
    BOOL m_TreeTargeting : 1; // 4
    BOOL m_MineAware : 1; // 8
    BOOL m_OreGrows : 1; // 16
    BOOL m_OreSpreads : 1; // 32
    BOOL m_NamedCivilians : 1; // 64
    BOOL m_PlayerAutoCrush : 1; // 128

    BOOL m_PlayerReturnFire : 1; // 1
    BOOL m_PlayerScatter : 1; // 2
    BOOL m_ChronoKillCargo : 1; // 4
    BOOL m_SecretUnitsEnabled : 1; // new in Chronoshift
    BOOL m_NewUnitsEnabled : 1; // new in Chronoshift
#else
    bool m_Paranoid;
    bool m_CurleyShuffle;
    bool m_FlashLowPower;
    bool m_CompEasyBonus;
    bool m_FineDiffControl;
    bool m_OreExplosive;
    bool m_EnemyHealth;
    bool m_MCVUndeploy;
    bool m_AllyReveal;
    bool m_SeparateAircraft;
    bool m_TreeTargeting;
    bool m_MineAware;
    bool m_OreGrows;
    bool m_OreSpreads;
    bool m_NamedCivilians;
    bool m_PlayerAutoCrush;
    bool m_PlayerReturnFire;
    bool m_PlayerScatter;
    bool m_ChronoKillCargo;
    bool m_SecretUnitsEnabled;
    bool m_NewUnitsEnabled;
#endif
    fixed_t m_ProneDamage;
    fixed_t m_QuakeDamage;
    fixed_t m_QuakeChance;
    fixed_t m_GrowthRate;
    fixed_t m_ShroudRate;
    fixed_t m_CrateRegen;
    fixed_t m_TimerWarning;
    fixed_t m_RechargeSonar;
    fixed_t m_RechargeChrono;
    fixed_t m_RechargeParaBomb;
    fixed_t m_RechargeParatrooper;
    fixed_t m_RechargeParaSaboteur;
    fixed_t m_RechargeSpyPlane;
    fixed_t m_RechargeIronCurtain;
    fixed_t m_RechargeGPS;
    fixed_t m_RechargeNuke;
    fixed_t m_SpeakDelay;
    fixed_t m_DamageDelay;
    int m_Gravity;
    int m_GapRadius;
    fixed_t m_GapRegenInterval;
    lepton_t m_RadarJamRadius;
    MPHType m_Incoming;
    int m_MinDamage;
    int m_MaxDamage;
    int m_RepairStep;
    fixed_t m_RepairPercent;
    int m_URepairStep;
    fixed_t m_URepairPercent;
    fixed_t m_RepairRate;
    fixed_t m_ConditionGreen;
    fixed_t m_ConditionYellow;
    fixed_t m_ConditionRed;
    fixed_t m_IdleActionFrequency;
    int m_BailCount;
    int m_GoldValue;
    int m_GemValue;
    int m_MaxAircraft;
    int m_MaxAnim;
    int m_MaxBuilding;
    int m_MaxBullet;
    int m_MaxFactory;
    int m_MaxInfantry;
    int m_MaxOverlay;
    int m_MaxSmudge;
    int m_MaxTeam;
    int m_MaxTeamType;
    int m_MaxTemplate;
    int m_MaxTerrain;
    int m_MaxTrigger;
    int m_MaxUnit;
    int m_MaxVessel;
    int m_MaxProjectile;
    int m_MaxWeapon;
    int m_MaxWarhead;
    int m_MaxTrigType;
    lepton_t m_CloseEnough;
    lepton_t m_Stray;
    lepton_t m_Crush;
    lepton_t m_CrateRadius;
    lepton_t m_HomingScatter;
    lepton_t m_BallisticScatter;
    fixed_t m_RefundPercent;
    fixed_t m_IronCurtain;
    int m_BridgeStrength;
    fixed_t m_BuildSpeed;
    fixed_t m_C4Delay;
    int m_CreditReserve;
    fixed_t m_PathDelay;
    fixed_t m_MovieTime;
    int m_ChronoTechLevel;
    lepton_t m_OreNearScan;
    lepton_t m_OreFarScan;
#ifdef GAME_DLL
    static fixed_t &m_EngineerDamage;
    static fixed_t &m_EngineerCaptureLevel;
    static fixed_t &m_ChronoTankDuration;
    static int &m_MTankDistance;
    static int &m_CarrierLaunchDelay;
    static fixed_t &m_QuakeUnitDamage;
    static fixed_t m_QuakeVesselDamage;
    static fixed_t &m_QuakeBuildingDamage;
    static int &m_QuakeInfantryDamage;
    static int &m_QuakeDelay;
    static BOOL &m_OrigNewUnitsEnabled;
    static BOOL &m_OrigSecretUnitsEnabled;
    static fixed_t m_CloakDelay;
#else
    // If we aren't trying to match the ABI, make these part of the class.
    fixed_t m_EngineerDamage;
    fixed_t m_EngineerCaptureLevel;
    fixed_t m_ChronoTankDuration;
    int m_MTankDistance;
    int m_CarrierLaunchDelay;
    fixed_t m_QuakeUnitDamage;
    fixed_t m_QuakeVesselDamage; // New additional control for Chronoshift.
    fixed_t m_QuakeBuildingDamage;
    int m_QuakeInfantryDamage;
    int m_QuakeDelay;
    fixed_t m_CloakDelay; // New additional control for Chronoshift, to be moved next to SubmergeDelay.
#endif
};

#ifdef GAME_DLL
extern RulesClass &g_Rule;
#else
extern RulesClass g_Rule;
#endif

#endif // RULES_H
