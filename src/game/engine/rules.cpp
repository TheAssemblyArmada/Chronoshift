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
#include "rules.h"
#include "bench.h"
#include "gameini.h"
#include "globals.h"
#include "housetype.h"
#include "land.h"
#include "stringex.h"
#include "theme.h"
#include "vortex.h"

#ifndef GAME_DLL
RulesClass g_Rule;
#endif

RulesClass::RulesClass() :
    m_TurboBoost("1.5"),
    m_AttackInterval("3.0"),
    m_AttackDelay("5.0"),
    m_PowerEmergency("0.75"),
    m_BadgerBombCount(1),
    m_AirstripRatio("0.12"),
    m_AirstripLimit(5),
    m_HelipadRatio("0.12"),
    m_HelipadLimit(5),
    m_TeslaRatio("0.16"),
    m_TeslaLimit(10),
    m_AARatio("0.14"),
    m_AALimit(10),
    m_DefenseRatio("0.5"),
    m_DefenseLimit(40),
    m_WarRatio("0.1"),
    m_WarLimit(2),
    m_BarracksRatio("0.16"),
    m_BarracksLimit(2),
    m_RefineryLimit(4),
    m_RefineryRatio("0.16"),
    m_BaseSizeAdd(3),
    m_PowerSurplus(50),
    m_InfantryReserve(2000),
    m_InfantryBaseMult(2),
    m_ChronoDuration("3.0"),
    m_WaterCrateChance("0.2"),
    m_SoloCrateMoney(2000),
    m_GPSTechLevel(0),
    m_UnitCrateType(UNIT_NONE),
    m_PatrolScan("0.016"),
    m_TeamDelay("0.6"),
    m_SubmergeDelay("0.00"),
    m_GameSpeedBias("1.0"),
    m_BaseBias("1.0"),
    m_VortexRange(2560), // 10 cells in leptons
    m_VortexSpeed(MPHType(10)),
    m_VortexDamage(200),
    m_VortexChance("0.2"),
    m_ExpSpread("0.5"),
    m_FireSupress(256), // 1 cell in leptons
    m_ParaTech(10),
    m_SpyPlaneTech(10),
    m_ParabombTech(10),
    m_SilverCrate(CRATE_HEAL_BASE),
    m_WoodCrate(CRATE_MONEY),
    m_WaterCrate(CRATE_MONEY),
    m_CrateMinimum(1),
    m_CrateMaximum(255),
    m_LZScanRadius(4096),        //16 cells in leptons
    m_MPlayerMoney(3000),
    m_MPlayerMaxMoney(10000),
    m_MPlayerShadowGrow(true),
    m_MPlayerBases(true),
    m_MPlayerOreGrows(true),
    m_MPlayerCrates(true),
    m_MPlayerAIPlayers(false),
    m_MPlayerCaptureTheFlag(false),
    m_DropZoneRadius(1024), // 4 cells in leptons
    m_MessageDelay("0.6"),
    m_SavourDelay("0.03"),
    m_AVMineDamage(1200),
    m_APMineDamage(1000),
    m_Players(8),
    m_BaseDefenseDelay("0.25"),
    m_SuspendPriority(20),
    m_SuspendDelay("2.0"),
    m_SurvivorRate("0.5"),
    m_ReloadRate("0.05"),
    m_AutocreateTime("5.0"),
    m_BuildupTime("0.05"),
    m_OreTruckRate(2),
    m_AtomDamage(1000),
    m_Paranoid(true),
    m_CurleyShuffle(false),
    m_FlashLowPower(true),
    m_CompEasyBonus(true),
    m_FineDiffControl(false),
    m_OreExplosive(false),
    m_EnemyHealth(true),
    m_MCVUndeploy(false),
    m_AllyReveal(true),
    m_SeparateAircraft(false),
    m_TreeTargeting(false),
    m_MineAware(true),
    m_OreGrows(true),
    m_OreSpreads(true),
    m_NamedCivilians(false),
    m_PlayerAutoCrush(false),
    m_PlayerReturnFire(false),
    m_PlayerScatter(false),
    m_ChronoKillCargo(true),
    m_SecretUnitsEnabled(false),
    m_NewUnitsEnabled(false),
    m_ProneDamage("0.5"),
    m_QuakeDamage("0.33"),
    m_QuakeChance("0.2"),
    m_GrowthRate("2.0"),
    m_ShroudRate("4.0"),
    m_CrateRegen("10.0"),
    m_TimerWarning("2.0"),
    m_RechargeSonar("14.0"),
    m_RechargeChrono("3.0"),
    m_RechargeParaBomb("14.0"),
    m_RechargeParatrooper("2.0"),
    m_RechargeParaSaboteur("14.0"),
    m_RechargeSpyPlane("2.0"),
    m_RechargeIronCurtain("14.0"),
    m_RechargeGPS("1.0"),    
    m_RechargeNuke("14.0"),
    m_SpeakDelay("2.0"),
    m_DamageDelay("1.0"),
    m_Gravity(3),
    m_GapRadius(10),
    m_GapRegenInterval("0.1"),
    m_RadarJamRadius(2560), // 10 cells in leptons
    m_Incoming(MPHType(0)),
    m_MinDamage(1),
    m_MaxDamage(1000),
    m_RepairStep(5),
    m_RepairPercent("0.25"),
    m_URepairStep(5),
    m_URepairPercent("0.25"),
    m_RepairRate("0.016"),
    m_ConditionGreen("1.0"),
    m_ConditionYellow("0.50"),
    m_ConditionRed("0.25"),
    m_IdleActionFrequency("0.083"),
    m_BailCount(28),
    m_GoldValue(35),
    m_GemValue(110),
    m_MaxAircraft(100),
    m_MaxAnim(100),
    m_MaxBuilding(500),
    m_MaxBullet(40),
    m_MaxFactory(20),
    m_MaxInfantry(500),
    m_MaxOverlay(1),
    m_MaxSmudge(1),
    m_MaxTeam(60),
    m_MaxTeamType(60),
    m_MaxTemplate(1),
    m_MaxTerrain(500),
    m_MaxTrigger(60),
    m_MaxUnit(500),
    m_MaxVessel(100),
    m_MaxProjectile(20),
    m_MaxWeapon(20),
    m_MaxWarhead(20),
    m_MaxTrigType(80),
    m_CloseEnough(640), // 2.5 cells in leptons
    m_Stray(512),// 2 cells in leptons
    m_Crush(384), // .5 cells in leptons
    m_CrateRadius(640), // 2.5 cells in leptons
    m_HomingScatter(512), //2 cells in leptons
    m_BallisticScatter(256), //1 cells in leptons
    m_RefundPercent("0.5"),
    m_IronCurtain("0.5"),
    m_BridgeStrength(1000),
    m_BuildSpeed("1.0"),
    m_C4Delay("0.03"),
    m_CreditReserve(1000),
    m_PathDelay("0.016"),
    m_MovieTime("0.25"),
    m_ChronoTechLevel(1),
    m_OreNearScan(1536), //6 cells in leptons
    m_OreFarScan(8192) //32 cells in leptons
#ifndef GAME_DLL
    ,
    m_ChronoTankDuration("0.0"),
    m_MTankDistance(30),
    m_CarrierLaunchDelay(60),
    m_QuakeUnitDamage("0.5"),
    m_QuakeVesselDamage("0.5"),
    m_QuakeBuildingDamage("0.25"),
    m_QuakeInfantryDamage(0),
    m_QuakeDelay(120),
    m_CloakDelay("0.02")
#endif
{
    m_IQControls.m_MaxIQLevels = 5;
    m_IQControls.m_SuperWeapons = 4;
    m_IQControls.m_Production = 5;
    m_IQControls.m_GuardArea = 4;
    m_IQControls.m_RepairSell = 3;
    m_IQControls.m_AutoCrush = 2;
    m_IQControls.m_Scatter = 3;
    m_IQControls.m_ContentScan = 4;
    m_IQControls.m_Aircraft = 4;
    m_IQControls.m_Harvester = 3;
    m_IQControls.m_SellBack = 2;

#ifdef GAME_DLL
    m_ChronoTankDuration = "0.0";
    m_MTankDistance = 30;
    m_CarrierLaunchDelay = 60;
    m_QuakeUnitDamage = "0.5";
    m_QuakeVesselDamage = "0.5";
    m_QuakeBuildingDamage = "0.25";
    m_QuakeInfantryDamage = 0;
    m_QuakeDelay = 120;
    m_CloakDelay = "0.02";
#endif
}

BOOL RulesClass::Process(GameINIClass &ini)
{
    BENCHMARK_START(BENCH_RULES);

    //Aftermath(ini); // General is called standalone from other places in code.
    General(ini);
    MPlayer(ini);
    Recharge(ini);
    Heap_Maximums(ini);
    AI(ini);
    Powerups(ini);
    Land_Types(ini);
    s_Themes(ini);
    IQ(ini);
    Objects(ini);
    Difficulty(ini);

    BENCHMARK_END(BENCH_RULES);

    return true;
}

void RulesClass::Addition(GameINIClass &ini)
{
    //Aftermath(ini); // General is called standalone from other places in code.
    General(ini);
    Recharge(ini);
    AI(ini);
    Powerups(ini);
    Land_Types(ini);
    s_Themes(ini);
    IQ(ini);
    Objects(ini);
    Difficulty(ini);
}

BOOL RulesClass::Aftermath(GameINIClass &ini)
{
    if (ini.Find_Section("Aftermath") != nullptr) {
        // i think they made these global so they didnt break save games...
        m_NewUnitsEnabled = ini.Get_Bool("Aftermath", "NewUnitsEnabled", false);

#ifdef GAME_DLL
        m_OrigNewUnitsEnabled = m_NewUnitsEnabled;
#endif

        m_MTankDistance = ini.Get_Int("Aftermath", "MTankDistance", m_MTankDistance);
        m_QuakeUnitDamage = ini.Get_Fixed("Aftermath", "QuakeUnitDamage", m_QuakeUnitDamage);
        m_QuakeVesselDamage = ini.Get_Fixed("Aftermath", "QuakeVesselDamage", m_QuakeVesselDamage);

        if (m_QuakeVesselDamage <= (fixed_t)0) {
            m_QuakeVesselDamage = m_QuakeUnitDamage;
        }

        m_QuakeBuildingDamage = ini.Get_Fixed("Aftermath", "QuakeBuildingDamage", m_QuakeBuildingDamage);
        m_QuakeInfantryDamage = ini.Get_Int("Aftermath", "QuakeInfantryDamage", m_QuakeInfantryDamage);
        m_QuakeDelay = ini.Get_Int("Aftermath", "QuakeDelay", m_QuakeDelay);
        m_ChronoTankDuration = ini.Get_Fixed("Aftermath", "ChronoTankDuration", m_ChronoTankDuration);
        m_CarrierLaunchDelay = ini.Get_Int("Aftermath", "CarrierLaunchDelay", m_CarrierLaunchDelay);
        m_EngineerDamage = ini.Get_Fixed("Aftermath", "EngineerDamage", m_EngineerDamage);
        m_EngineerCaptureLevel = ini.Get_Fixed("Aftermath", "EngineerCaptureLevel", m_EngineerCaptureLevel);

        return true;
    }

    return false;
}

BOOL RulesClass::General(GameINIClass &ini)
{
    // TODO Requires GameINIClass::Get_UnitType, ChronalVortexClass
#ifdef GAME_DLL
    BOOL(*func)(const RulesClass *, GameINIClass &) = reinterpret_cast<BOOL(*)(const RulesClass *, GameINIClass &)>(0x005342DC);
    return func(this, ini);
#elif 0
    Aftermath(ini);

    if (ini.Find_Section("General") != nullptr) {
        m_TurboBoost = ini.Get_Fixed("General", "TurboBoost", m_TurboBoost);
        m_BadgerBombCount = ini.Get_Int("General", "BadgerBombCount", m_BadgerBombCount);
        m_CurleyShuffle = ini.Get_Bool("General", "CurleyShuffle", m_CurleyShuffle);
        m_FlashLowPower = ini.Get_Bool("General", "FlashLowPower", m_FlashLowPower);
        m_ChronoKillCargo = ini.Get_Bool("General", "ChronoKillCargo", m_ChronoKillCargo);
        m_ChronoDuration = ini.Get_Fixed("General", "ChronoDuration", m_ChronoDuration);
        m_FineDiffControl = ini.Get_Bool("General", "FineDiffControl", m_FineDiffControl);
        m_WaterCrateChance = ini.Get_Fixed("General", "WaterCrateChance", m_WaterCrateChance);
        m_SoloCrateMoney = ini.Get_Int("General", "SoloCrateMoney", m_SoloCrateMoney);
        m_ParabombTech = ini.Get_Int("General", "ParabombTech", m_ParabombTech);
        m_GPSTechLevel = ini.Get_Int("General", "GPSTechLevel", m_GPSTechLevel);
        m_UnitCrateType = ini.Get_UnitType("General", "UnitCrateType", m_UnitCrateType);

        // BUGFIX: Fixed from original, this entry was loaded with Get_Fixed().
        m_OreExplosive = ini.Get_Bool("General", "OreExplosive", m_OreExplosive);
        m_GapRegenInterval = ini.Get_Fixed("General", "GapRegenInterval", m_GapRegenInterval);
        m_TeamDelay = ini.Get_Fixed("General", "TeamDelay", m_TeamDelay);
        m_SubmergeDelay = ini.Get_Fixed("General", "SubmergeDelay", m_SubmergeDelay);
        m_CloakDelay = ini.Get_Fixed("General", "CloakDelay", m_CloakDelay);
        m_GameSpeedBias = ini.Get_Fixed("General", "GameSpeedBias", m_GameSpeedBias);

        // Original doesn't appear to load this, rule.ini value loaded as 16th of lepton?
        // m_LZScanRadius = ini.Get_Lepton("General", "LZScanRadius", m_LZScanRadius);

        m_BaseBias = ini.Get_Fixed("General", "BaseBias", m_BaseBias);
        m_ExpSpread = ini.Get_Fixed("General", "ExpSpread", m_ExpSpread);
        m_FireSupress = ini.Get_Lepton("General", "FireSupress", m_FireSupress);
        m_ParaTech = ini.Get_Int("General", "ParaTech", m_ParaTech);
        m_SpyPlaneTech = ini.Get_Int("General", "SpyPlaneTech", m_SpyPlaneTech);
        m_SilverCrate = ini.Get_CrateType("General", "SilverCrate", m_SilverCrate);
        m_WoodCrate = ini.Get_CrateType("General", "WoodCrate", m_WoodCrate);
        m_WaterCrate = ini.Get_CrateType("General", "WaterCrate", m_WaterCrate);
        m_CrateMinimum = ini.Get_Int("General", "CrateMinimum", m_CrateMinimum);
        m_CrateMaximum = ini.Get_Int("General", "CrateMaximum", m_CrateMaximum);
        m_PlayerScatter = ini.Get_Bool("General", "PlayerScatter", m_PlayerScatter);
        m_PlayerReturnFire = ini.Get_Bool("General", "PlayerReturnFire", m_PlayerReturnFire);
        m_PlayerAutoCrush = ini.Get_Bool("General", "PlayerAutoCrush", m_PlayerAutoCrush);
        m_NamedCivilians = ini.Get_Bool("General", "NamedCivilians", m_NamedCivilians);
        m_OreGrows = ini.Get_Bool("General", "OreGrows", m_OreGrows);
        m_OreSpreads = ini.Get_Bool("General", "OreSpreads", m_OreSpreads);
        m_MineAware = ini.Get_Bool("General", "MineAware", m_MineAware);
        m_TreeTargeting = ini.Get_Bool("General", "TreeTargeting", m_TreeTargeting);
        m_SeparateAircraft = ini.Get_Bool("General", "SeparateAircraft", m_SeparateAircraft);
        m_DropZoneRadius = ini.Get_Lepton("General", "DropZoneRadius", m_DropZoneRadius);
        m_MessageDelay = ini.Get_Fixed("General", "MessageDelay", m_MessageDelay);
        m_SavourDelay = ini.Get_Fixed("General", "SavourDelay", m_SavourDelay);
        m_AVMineDamage = ini.Get_Int("General", "AVMineDamage", m_AVMineDamage);
        m_APMineDamage = ini.Get_Int("General", "APMineDamage", m_APMineDamage);
        m_BaseDefenseDelay = ini.Get_Fixed("General", "BaseDefenseDelay", m_BaseDefenseDelay);
        m_SuspendPriority = ini.Get_Int("General", "SuspendPriority", m_SuspendPriority);
        m_SuspendDelay = ini.Get_Fixed("General", "SuspendDelay", m_SuspendDelay);
        m_SurvivorRate = ini.Get_Fixed("General", "SurvivorRate", m_SurvivorRate);
        m_RadarJamRadius = ini.Get_Lepton("General", "RadarJamRadius", m_RadarJamRadius);
        m_ReloadRate = ini.Get_Fixed("General", "ReloadRate", m_ReloadRate);
        m_IdleActionFrequency = ini.Get_Fixed("General", "IdleActionFrequency", m_IdleActionFrequency);
        m_BuildupTime = ini.Get_Fixed("General", "BuildupTime", m_BuildupTime);
        m_OreTruckRate = ini.Get_Int("General", "OreTruckRate", m_OreTruckRate);
        m_AtomDamage = ini.Get_Int("General", "AtomDamage", m_AtomDamage);
        m_BailCount = ini.Get_Int("General", "BailCount", m_BailCount);
        m_BallisticScatter = ini.Get_Lepton("General", "BallisticScatter", m_BallisticScatter);
        m_BridgeStrength = ini.Get_Int("General", "BridgeStrength", m_BridgeStrength);
        m_BuildSpeed = ini.Get_Fixed("General", "BuildSpeed", m_BuildSpeed);
        m_ConditionGreen = ini.Get_Fixed("General", "ConditionGreen", m_ConditionGreen);
        m_ConditionYellow = ini.Get_Fixed("General", "ConditionYellow", m_ConditionYellow);
        m_ConditionRed = ini.Get_Fixed("General", "ConditionRed", m_ConditionRed);
        m_CrateRadius = ini.Get_Lepton("General", "CrateRadius", m_CrateRadius);
        m_Crush = ini.Get_Lepton("General", "Crush", m_Crush);
        m_DamageDelay = ini.Get_Fixed("General", "DamageDelay", m_DamageDelay);
        m_GapRadius = ini.Get_Int("General", "GapRadius", m_GapRadius);
        m_GemValue = ini.Get_Int("General", "GemValue", m_GemValue);
        m_GoldValue = ini.Get_Int("General", "GoldValue", m_GoldValue);
        m_GrowthRate = ini.Get_Fixed("General", "GrowthRate", m_GrowthRate);
        m_HomingScatter = ini.Get_Lepton("General", "HomingScatter", m_HomingScatter);
        m_Incoming = ini.Get_MPHType("General", "Incoming", m_Incoming);
        m_IronCurtain = ini.Get_Fixed("General", "IronCurtain", m_IronCurtain);
        m_AllyReveal = ini.Get_Bool("General", "AllyReveal", m_AllyReveal);
        m_EnemyHealth = ini.Get_Bool("General", "EnemyHealth", m_EnemyHealth);
        m_MCVUndeploy = ini.Get_Bool("General", "MCVUndeploy", m_MCVUndeploy);
        m_MaxDamage = ini.Get_Int("General", "MaxDamage", m_MaxDamage);
        m_MinDamage = ini.Get_Int("General", "MinDamage", m_MinDamage);
        m_ProneDamage = ini.Get_Fixed("General", "ProneDamage", m_ProneDamage);
        m_QuakeDamage = ini.Get_Fixed("General", "QuakeDamage", m_QuakeDamage);
        m_QuakeChance = ini.Get_Fixed("General", "QuakeChance", m_QuakeChance);
        m_RefundPercent = ini.Get_Fixed("General", "RefundPercent", m_RefundPercent);
        m_RepairPercent = ini.Get_Fixed("General", "RepairPercent", m_RepairPercent);
        m_RepairStep = ini.Get_Int("General", "RepairStep", m_RepairStep);
        m_URepairPercent = ini.Get_Fixed("General", "URepairPercent", m_URepairPercent);
        m_URepairStep = ini.Get_Int("General", "URepairStep", m_URepairStep);
        m_RepairRate = ini.Get_Fixed("General", "RepairRate", m_RepairRate);
        m_ShroudRate = ini.Get_Fixed("General", "ShroudRate", m_ShroudRate);
        m_SpeakDelay = ini.Get_Fixed("General", "SpeakDelay", m_SpeakDelay);
        m_Stray = ini.Get_Lepton("General", "Stray", m_Stray);
        m_CloseEnough = ini.Get_Lepton("General", "CloseEnough", m_CloseEnough);
        m_TimerWarning = ini.Get_Fixed("General", "TimerWarning", m_TimerWarning);
        m_MovieTime = ini.Get_Fixed("General", "MovieTime", m_MovieTime);
        m_C4Delay = ini.Get_Fixed("General", "C4Delay", m_C4Delay);
        m_ChronoTechLevel = ini.Get_Int("General", "ChronoTechLevel", m_ChronoTechLevel);
        m_CrateRegen = ini.Get_Fixed("General", "CrateRegen", m_CrateRegen);
        m_VortexRange = ini.Get_Lepton("General", "VortexRange", m_VortexRange);
        m_VortexSpeed = ini.Get_MPHType("General", "VortexSpeed", m_VortexSpeed);
        m_VortexDamage = ini.Get_Int("General", "VortexDamage", m_VortexDamage);
        m_VortexChance = ini.Get_Fixed("General", "VortexChance", m_VortexChance);

        // Set variables within the vortex class global instance.
        g_ChronalVortex.Set_Range(m_VortexRange / 256);
        g_ChronalVortex.Set_Speed(m_VortexSpeed);
        g_ChronalVortex.Set_Damage(m_VortexDamage);

        return true;
    }

    return false;
#else
    return false;
#endif
}

BOOL RulesClass::MPlayer(GameINIClass &ini)
{
    if (ini.Find_Section("MultiplayerDefaults") != nullptr) {
        m_MPlayerMoney = ini.Get_Int("MultiplayerDefaults", "Money", m_MPlayerMoney);
        m_MPlayerMaxMoney = ini.Get_Int("MultiplayerDefaults", "MaxMoney", m_MPlayerMaxMoney);
        m_MPlayerShadowGrow = ini.Get_Bool("MultiplayerDefaults", "ShadowGrow", m_MPlayerShadowGrow);
        m_MPlayerBases = ini.Get_Bool("MultiplayerDefaults", "Bases", m_MPlayerBases);
        m_MPlayerOreGrows = ini.Get_Bool("MultiplayerDefaults", "OreGrows", m_MPlayerOreGrows);
        m_MPlayerCrates = ini.Get_Bool("MultiplayerDefaults", "Crates", m_MPlayerCrates);
        m_MPlayerAIPlayers = ini.Get_Bool("MultiplayerDefaults", "AIPlayers", m_MPlayerAIPlayers);
        m_MPlayerCaptureTheFlag = ini.Get_Bool("MultiplayerDefaults", "CaptureTheFlag", m_MPlayerCaptureTheFlag);

        return true;
    }

    return false;
}

BOOL RulesClass::Recharge(GameINIClass &ini)
{
    if (ini.Find_Section("Recharge") != nullptr) {
        m_RechargeSonar = ini.Get_Fixed("Recharge", "Sonar", m_RechargeSonar);
        m_RechargeChrono = ini.Get_Fixed("Recharge", "Chrono", m_RechargeChrono);
        m_RechargeParaBomb = ini.Get_Fixed("Recharge", "ParaBomb", m_RechargeParaBomb);
        m_RechargeParatrooper = ini.Get_Fixed("Recharge", "Paratrooper", m_RechargeParatrooper);
        m_RechargeParaSaboteur = ini.Get_Fixed("Recharge", "Saboteur", m_RechargeParaSaboteur);
        m_RechargeSpyPlane = ini.Get_Fixed("Recharge", "SpyPlane", m_RechargeSpyPlane);
        m_RechargeIronCurtain = ini.Get_Fixed("Recharge", "IronCurtain", m_RechargeIronCurtain);
        m_RechargeGPS = ini.Get_Fixed("Recharge", "GPS", m_RechargeGPS);
        m_RechargeNuke = ini.Get_Fixed("Recharge", "Nuke", m_RechargeNuke);

        return true;
    }

    return false;
}

BOOL RulesClass::Heap_Maximums(GameINIClass &ini)
{
    // TODO Requires WarheadTypeClass and WeaponTypeClass or those heap inits moving elsewhere.
#ifdef GAME_DLL
    BOOL(*func)(const RulesClass *, GameINIClass &) = reinterpret_cast<BOOL(*)(const RulesClass *, GameINIClass &)>(0x00535F38);
    return func(this, ini);
#elif 0
    if (ini.Find_Section("Maximums") != nullptr) {
        m_Players = ini.Get_Int("Maximums", "Players", m_Players);
        m_MaxAircraft = ini.Get_Int("Maximums", "Aircraft", m_MaxAircraft);
        m_MaxAnim = ini.Get_Int("Maximums", "Anim", m_MaxAnim);
        m_MaxBuilding = ini.Get_Int("Maximums", "Building", m_MaxBuilding);
        m_MaxBullet = ini.Get_Int("Maximums", "Bullet", m_MaxBullet);
        m_MaxFactory = ini.Get_Int("Maximums", "Factory", m_MaxFactory);
        m_MaxInfantry = ini.Get_Int("Maximums", "Infantry", m_MaxInfantry);
        m_MaxOverlay = ini.Get_Int("Maximums", "Overlay", m_MaxOverlay);
        m_MaxSmudge = ini.Get_Int("Maximums", "Smudge", m_MaxSmudge);
        m_MaxTeam = ini.Get_Int("Maximums", "Team", m_MaxTeam);
        m_MaxTeamType = ini.Get_Int("Maximums", "TeamType", m_MaxTeamType);
        m_MaxTemplate = ini.Get_Int("Maximums", "Template", m_MaxTemplate);
        m_MaxTerrain = ini.Get_Int("Maximums", "Terrain", m_MaxTerrain);
        m_MaxTrigger = ini.Get_Int("Maximums", "Trigger", m_MaxTrigger);
        m_MaxUnit = ini.Get_Int("Maximums", "Unit", m_MaxUnit);
        m_MaxVessel = ini.Get_Int("Maximums", "Vessel", m_MaxVessel);
        m_MaxProjectile = ini.Get_Int("Maximums", "Projectile", m_MaxProjectile);
        m_MaxWeapon = ini.Get_Int("Maximums", "Weapon", m_MaxWeapon);
        m_MaxWarhead = ini.Get_Int("Maximums", "Warhead", m_MaxWarhead);
        m_MaxTrigType = ini.Get_Int("Maximums", "TrigType", m_MaxTrigType);

        // Moved, see warhead and weapon headers for more info
        // TODO, move Set_Heap()'s to RTS_Init() in init.cpp'

        return true;
    }

    return false;
#else
    return false;
#endif
}

BOOL RulesClass::AI(GameINIClass &ini)
{
    if (ini.Find_Section("AI") != nullptr) {
        m_AttackInterval = ini.Get_Fixed("AI", "AttackInterval", m_AttackInterval);
        m_AttackDelay = ini.Get_Fixed("AI", "AttackDelay", m_AttackDelay);
        m_PatrolScan = ini.Get_Fixed("AI", "PatrolScan", m_PatrolScan);
        m_CreditReserve = ini.Get_Int("AI", "CreditReserve", m_CreditReserve);
        m_PathDelay = ini.Get_Fixed("AI", "PathDelay", m_PathDelay);
        m_OreNearScan = ini.Get_Lepton("AI", "OreNearScan", m_OreNearScan);
        m_OreFarScan = ini.Get_Lepton("AI", "OreFarScan", m_OreFarScan);
        m_AutocreateTime = ini.Get_Fixed("AI", "AutocreateTime", m_AutocreateTime);
        m_InfantryReserve = ini.Get_Int("AI", "InfantryReserve", m_InfantryReserve);
        m_InfantryBaseMult = ini.Get_Int("AI", "InfantryBaseMult", m_InfantryBaseMult);
        m_PowerSurplus = ini.Get_Int("AI", "PowerSurplus", m_PowerSurplus);
        m_BaseSizeAdd = ini.Get_Int("AI", "BaseSizeAdd", m_BaseSizeAdd);
        m_RefineryRatio = ini.Get_Fixed("AI", "RefineryRatio", m_RefineryRatio);
        m_RefineryLimit = ini.Get_Int("AI", "RefineryLimit", m_RefineryLimit);
        m_BarracksRatio = ini.Get_Fixed("AI", "BarracksRatio", m_BarracksRatio);
        m_BarracksLimit = ini.Get_Int("AI", "BarracksLimit", m_BarracksLimit);
        m_WarRatio = ini.Get_Fixed("AI", "WarRatio", m_WarRatio);
        m_WarLimit = ini.Get_Int("AI", "WarLimit", m_WarLimit);
        m_DefenseRatio = ini.Get_Fixed("AI", "DefenseRatio", m_DefenseRatio);
        m_DefenseLimit = ini.Get_Int("AI", "DefenseLimit", m_DefenseLimit);
        m_AARatio = ini.Get_Fixed("AI", "AARatio", m_AARatio);
        m_AALimit = ini.Get_Int("AI", "AALimit", m_AALimit);
        m_TeslaRatio = ini.Get_Fixed("AI", "TeslaRatio", m_TeslaRatio);
        m_TeslaLimit = ini.Get_Int("AI", "TeslaLimit", m_TeslaLimit);
        m_HelipadRatio = ini.Get_Fixed("AI", "HelipadRatio", m_HelipadRatio);
        m_HelipadLimit = ini.Get_Int("AI", "HelipadLimit", m_HelipadLimit);
        m_AirstripRatio = ini.Get_Fixed("AI", "AirstripRatio", m_AirstripRatio);
        m_AirstripLimit = ini.Get_Int("AI", "AirstripLimit", m_AirstripLimit);
        m_CompEasyBonus = ini.Get_Bool("AI", "CompEasyBonus", m_CompEasyBonus);
        m_Paranoid = ini.Get_Bool("AI", "Paranoid", m_Paranoid);
        m_PowerEmergency = ini.Get_Fixed("AI", "PowerEmergency", m_PowerEmergency);
    }

    return false;
}

BOOL RulesClass::Powerups(GameINIClass &ini)
{
    // TODO Needs AnimTypeClass.
#ifdef GAME_DLL
    BOOL(*func)(const RulesClass *, GameINIClass &) = reinterpret_cast<BOOL(*)(const RulesClass *, GameINIClass &)>(0x00536D3C);
    return func(this, ini);
#elif 0
    if (ini.Find_Section("Powerups") != nullptr) {
        char value[INIClass::MAX_LINE_LENGTH];

        for (CrateType crate = CRATE_FIRST; crate < CRATE_COUNT; ++crate) {
            if (ini.Get_String("Powerups", CrateClass::Name_From_Crate(crate), "0, NONE", value, sizeof(value)) > 0) {
                char *share = strtok(value, ",");

                if (share) {
                    strtrim(share);
                    CrateClass::s_CrateShares[crate] = strtol(share, nullptr, 10);
                }

                char *anim = strtok(value, ",");

                if (anim) {
                    strtrim(anim);
                    CrateClass::s_CrateAnims[crate] = AnimTypeClass::From_Name(anim);
                }

                char *data = strtok(value, ",");

                if (strpbrk(value, ".%")) {
                    CrateClass::s_CrateData[crate] = fixed_t(data).To_Int();
                } else {
                    strtrim(data);
                    CrateClass::s_CrateData[crate] = strtol(data, nullptr, 10);
                }
            }
        }

        return true;
    }

    return false;
#else
    return false;
#endif
}

BOOL RulesClass::s_Themes(GameINIClass &ini)
{
    char entry[INIClass::MAX_LINE_LENGTH];

    if (ini.Is_Present("ThemeControl")) {
        for (ThemeType theme = THEME_FIRST; theme < THEME_COUNT; ++theme) {
            const char *theme_name = g_Theme.Base_Name(theme);
            if (theme_name && ini.Find_Entry("ThemeControl", theme_name) != nullptr) {
                ini.Get_String("ThemeControl", theme_name, "", entry, sizeof(entry));

                char *scen_val = strtok(entry, ",");
                int scen = scen_val ? strtol(scen_val, nullptr, 10) : 1;

                char *owner_val = strtok(entry, ",");
                int owner = owner_val ? HouseTypeClass::Owner_From_Name(owner_val) : -1;

                g_Theme.Set_Theme_Data(theme, scen, owner);
            }
        }

        return true;
    }

    return false;
}

BOOL RulesClass::IQ(GameINIClass &ini)
{
    if (ini.Find_Section("IQ") != nullptr) {
        m_IQControls.m_MaxIQLevels = ini.Get_Int("IQ", "MaxIQLevels", m_IQControls.m_MaxIQLevels);
        m_IQControls.m_SuperWeapons = ini.Get_Int("IQ", "SuperWeapons", m_IQControls.m_SuperWeapons);
        m_IQControls.m_Production = ini.Get_Int("IQ", "Production", m_IQControls.m_Production);
        m_IQControls.m_GuardArea = ini.Get_Int("IQ", "GuardArea", m_IQControls.m_GuardArea);
        m_IQControls.m_RepairSell = ini.Get_Int("IQ", "RepairSell", m_IQControls.m_RepairSell);
        m_IQControls.m_AutoCrush = ini.Get_Int("IQ", "AutoCrush", m_IQControls.m_AutoCrush);
        m_IQControls.m_Scatter = ini.Get_Int("IQ", "Scatter", m_IQControls.m_Scatter);
        m_IQControls.m_ContentScan = ini.Get_Int("IQ", "ContentScan", m_IQControls.m_ContentScan);
        m_IQControls.m_Aircraft = ini.Get_Int("IQ", "Aircraft", m_IQControls.m_Aircraft);
        m_IQControls.m_Harvester = ini.Get_Int("IQ", "Harvester", m_IQControls.m_Harvester);
        m_IQControls.m_SellBack = ini.Get_Int("IQ", "SellBack", m_IQControls.m_SellBack);

        return true;
    }

    return false;
}

BOOL RulesClass::Objects(GameINIClass &ini)
{
    // TODO Needs most *TypeClass classes.
#ifdef GAME_DLL
    BOOL(*func)(const RulesClass *, GameINIClass &) = reinterpret_cast<BOOL(*)(const RulesClass *, GameINIClass &)>(0x005373DC);
    return func(this, ini);
#elif 0
    for (WarheadType warhead = WARHEAD_FIRST; warhead < WARHEAD_COUNT; ++warhead) {
        WarheadTypeClass::As_Reference(warhead).Read_INI(ini);
    }

    for (BulletType bullet = BULLET_FIRST; bullet < BULLET_COUNT; ++bullet) {
        BulletTypeClass::As_Reference(bullet).Read_INI(ini);
    }

    for (WeaponType weapon = WEAPON_FIRST; weapon < WEAPON_COUNT; ++weapon) {
        WeaponTypeClass::As_Reference(weapon).Read_INI(ini);
    }

    for (UnitType unit = UNIT_FIRST; unit < UNIT_COUNT; ++unit) {
        UnitTypeClass::As_Reference(unit).Read_INI(ini);
    }

    for (InfantryType infantry = INFANTRY_FIRST; infantry < INFANTRY_COUNT; ++infantry) {
        InfantryTypeClass::As_Reference(infantry).Read_INI(ini);
    }

    for (VesselType vessel = VESSEL_FIRST; vessel < VESSEL_COUNT; ++vessel) {
        VesselTypeClass::As_Reference(vessel).Read_INI(ini);
    }

    for (AircraftType aircraft = AIRCRAFT_FIRST; aircraft < AIRCRAFT_COUNT; ++aircraft) {
        AircraftTypeClass::As_Reference(aircraft).Read_INI(ini);
    }

    for (BuildingType building = BUILDING_FIRST; building < BUILDING_COUNT; ++building) {
        BuildingTypeClass::As_Reference(building).Read_INI(ini);
    }

    for (HousesType house = HOUSES_FIRST; house < HOUSES_COUNT; ++house) {
        HouseTypeClass::As_Reference(house).Read_INI(ini);
    }

    for (MissionType mission = MISSION_FIRST; mission < MISSION_COUNT; ++mission) {
        MissionControlClass::s_MissionControl[mission].Read_INI(ini);
    }

    return true;
#else
    return false;
#endif
}

BOOL RulesClass::Difficulty(GameINIClass &ini)
{
    Difficulty_Get(ini, m_Difficulties[0], "Easy");
    Difficulty_Get(ini, m_Difficulties[1], "Normal");
    Difficulty_Get(ini, m_Difficulties[2], "Difficult");

    return true;
}

BOOL RulesClass::Land_Types(GameINIClass &ini)
{
    // TODO Needs most *TypeClass classes.
#ifdef GAME_DLL
    BOOL(*func)(const RulesClass *, GameINIClass &) = reinterpret_cast<BOOL(*)(const RulesClass *, GameINIClass &)>(0x00536E8C);
    return func(this, ini);
#elif 0
    for (LandType land = LAND_FIRST; land < LAND_COUNT; ++land) {
        
    }

    return true;
#else
    return false;
#endif
}
