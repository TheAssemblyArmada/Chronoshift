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
#include "gameini.h"
#include "globals.h"
#include "housetype.h"
#include "land.h"
#include "stringex.h"
#include "theme.h"

#ifndef CHRONOSHIFT_STANDALONE
fixed &RulesClass::EngineerDamage = Make_Global<fixed>(0x00665E02);
fixed &RulesClass::EngineerCaptureLevel = Make_Global<fixed>(0x00665E04);
fixed &RulesClass::ChronoTankDuration = Make_Global<fixed>(0x00665E00);
int &RulesClass::MTankDistance = Make_Global<int>(0x006016A4);
int RulesClass::CarrierLaunchDelay;
fixed &RulesClass::QuakeUnitDamage = Make_Global<fixed>(0x00665DF8);
fixed RulesClass::QuakeVesselDamage;
fixed &RulesClass::QuakeBuildingDamage = Make_Global<fixed>(0x00665DFA);
int &RulesClass::QuakeInfantryDamage = Make_Global<int>(0x006016AC);
int &RulesClass::QuakeDelay = Make_Global<int>(0x00665DFC);
BOOL &RulesClass::OrigNewUnitsEnabled = Make_Global<BOOL>(0x00665DE0);
BOOL &RulesClass::OrigSecretUnitsEnabled = Make_Global<BOOL>(0x00665DE4);
RulesClass &Rule = Make_Global<RulesClass>(0x00666704);
fixed RulesClass::CloakDelay;
#else
RulesClass Rule;
#endif

RulesClass::RulesClass() :
    TurboBoost("1.5"),
    AttackInterval("3.0"),
    AttackDelay("5.0"),
    PowerEmergency("0.75"),
    BadgerBombCount(1),
    AirstripRatio("0.12"),
    AirstripLimit(5),
    HelipadRatio("0.12"),
    HelipadLimit(5),
    TeslaRatio("0.16"),
    TeslaLimit(10),
    AARatio("0.14"),
    AALimit(10),
    DefenseRatio("0.5"),
    DefenseLimit(40),
    WarRatio("0.1"),
    WarLimit(2),
    BarracksRatio("0.16"),
    BarracksLimit(2),
    RefineryLimit(4),
    RefineryRatio("0.16"),
    BaseSizeAdd(3),
    PowerSurplus(50),
    InfantryReserve(2000),
    InfantryBaseMult(1),
    ChronoDuration("3.0"),
    WaterCrateChance("0.2"),
    SoloCrateMoney(2000),
    GPSTechLevel(0),
    UnitCrateType(UNIT_NONE),
    PatrolScan("0.016"),
    TeamDelay("0.6"),
    SubmergeDelay("0.02"),
    GameSpeedBias("1.0"),
    BaseBias("1.0"),
    VortexRange(2560), // 10 cells in leptons
    VortexSpeed((MPHType)10),
    VortexDamage(200),
    VortexChance("0.2"),
    ExpSpread("0.5"),
    FireSupress(256), // 1 cell in leptons
    ParaTech(10),
    SpyPlaneTech(10),
    ParabombTech(10),
    SilverCrate(CRATE_NONE),
    WoodCrate(CRATE_NONE),
    WaterCrate(CRATE_NONE),
    CrateMinimum(1),
    CrateMaximum(255),
    LZScanRadius(4096),		//16 cells in leptons
    MPlayerMoney(3000),
    MPlayerMaxMoney(10000),
    MPlayerShadowGrow(true),
    MPlayerBases(true),
    MPlayerOreGrows(true),
    MPlayerCrates(true),
    MPlayerBit16(false),
    MPlayerCaptureTheFlag(false),
    DropZoneRadius(1024), // 4 cells in leptons
    MessageDelay("0.6"),
    SavourDelay("0.03"),
    AVMineDamage(1200),
    APMineDamage(1000),
    Players(8),
    BaseDefenseDelay("0.25"),
    SuspendPriority(20),
    SuspendDelay("2.0"),
    SurvivorRate("0.5"),
    ReloadRate("0.05"),
    AutocreateTime("5.0"),
    BuildupTime("0.05"),
    OreTruckRate(2),
    AtomDamage(1000),
    Paranoid(true),
    CurleyShuffle(false),
    FlashLowPower(true),
    CompEasyBonus(true),
    FineDiffControl(false),
    OreExplosive(false),
    EnemyHealth(true),
    MCVUndeploy(false),
    AllyReveal(true),
    SeparateAircraft(false),
    TreeTargeting(false),
    MineAware(true),
    OreGrows(true),
    OreSpreads(true),
    NamedCivilians(false),
    PlayerAutoCrush(false),
    PlayerReturnFire(false),
    PlayerScatter(false),
    ChronoKillCargo(true),
    SecretUnitsEnabled(false),
    NewUnitsEnabled(false),
    ProneDamage("0.5"),
    QuakeDamage("0.33"),
    QuakeChance("0.2"),
    GrowthRate("2.0"),
    ShroudRate("4.0"),
    CrateRegen("10.0"),
    TimerWarning("2.0"),
    RechargeSonar("14.0"),
    RechargeChrono("3.0"),
    RechargeParaBomb("14.0"),
    RechargeParatrooper("2.0"),
    RechargeParaSaboteur("14.0"),
    RechargeSpyPlane("2.0"),
    RechargeIronCurtain("14.0"),
    RechargeGPS("1.0"),	
    RechargeNuke("14.0"),
    SpeakDelay("2.0"),
    DamageDelay("1.0"),
    Gravity(3),
    GapRadius(10),
    GapRegenInterval("0.1"),
    RadarJamRadius(2560), // 10 cells in leptons
    Incoming((MPHType)0),
    MinDamage(1),
    MaxDamage(1000),
    RepairStep(5),
    RepairPercent("0.25"),
    URepairStep(5),
    URepairPercent("0.25"),
    RepairRate("0.016"),
    ConditionGreen("1.0"),
    ConditionYellow("0.50"),
    ConditionRed("0.25"),
    IdleActionFrequency("0.083"),
    BailCount(28),
    GoldValue(35),
    GemValue(110),
    MaxAircraft(100),
    MaxAnim(100),
    MaxBuilding(500),
    MaxBullet(40),
    MaxFactory(20),
    MaxInfantry(500),
    MaxOverlay(1),
    MaxSmudge(1),
    MaxTeam(60),
    MaxTeamType(60),
    MaxTemplate(1),
    MaxTerrain(500),
    MaxTrigger(60),
    MaxUnit(500),
    MaxVessel(100),
    MaxProjectile(20),
    MaxWeapon(20),
    MaxWarhead(20),
    MaxTrigType(60),
    CloseEnough(640), // 2.5 cells in leptons
    Stray(512),// 2 cells in leptons
    Crush(384), // .5 cells in leptons
    CrateRadius(640), // 2.5 cells. leptons
    HomingScatter(512), //2 cells. leptons
    BallisticScatter(256), //1 cells. leptons
    RefundPercent("0.5"),
    IronCurtain("0.5"),
    BridgeStrength(1000),
    BuildSpeed("1.0"),
    C4Delay("0.03"),
    CreditReserve(1000),
    PathDelay("0.016"),
    MovieTime("0.25"),
    ChronoTechLevel(1),
    OreNearScan(1536), //6 cells. leptons
    OreFarScan(8192) //32 cells. leptons
#ifdef CHRONOSHIFT_STANDALONE
    ,
    ChronoTankDuration(fixed::_0_1),
    MTankDistance(30),
    CarrierLaunchDelay(60),
    QuakeUnitDamage(fixed::_1_2),
    QuakeVesselDamage(fixed::_1_2),
    QuakeBuildingDamage(fixed::_1_4),
    QuakeInfantryDamage(0),
    QuakeDelay(120),
    CloakDelay("0.02"))
#endif
{
    IQControls.MaxIQLevels = 5;
    IQControls.SuperWeapons = 4;
    IQControls.Production = 5;
    IQControls.GuardArea = 4;
    IQControls.RepairSell = 1;
    IQControls.AutoCrush = 2;
    IQControls.Scatter = 3;
    IQControls.ContentScan = 4;
    IQControls.Aircraft = 4;
    IQControls.Harvester = 2;
    IQControls.SellBack = 2;

#ifndef CHRONOSHIFT_STANDALONE
    ChronoTankDuration = fixed::_0_1;
    MTankDistance = 30;
    CarrierLaunchDelay = 60;
    QuakeUnitDamage = fixed::_1_2;
    QuakeVesselDamage = fixed::_1_2;
    QuakeBuildingDamage = fixed::_1_4;
    QuakeInfantryDamage = 0;
    QuakeDelay = 120;
    CloakDelay = "0.02";
#endif
}

BOOL RulesClass::Process(GameINIClass &ini)
{
    //Aftermath(ini); // General is called standalone from other places in code.
    General(ini);
    MPlayer(ini);
    Recharge(ini);
    Heap_Maximums(ini);
    AI(ini);
    Powerups(ini);
    Land_Types(ini);
    Themes(ini);
    IQ(ini);
    Objects(ini);
    Difficulty(ini);

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
    Themes(ini);
    IQ(ini);
    Objects(ini);
    Difficulty(ini);
}

BOOL RulesClass::Aftermath(GameINIClass &ini)
{
    if (ini.Find_Section("Aftermath") != nullptr) {
        // i think they made these global so they didnt break save games...
        NewUnitsEnabled = ini.Get_Bool("Aftermath", "NewUnitsEnabled", false);

#ifndef CHRONOSHIFT_STANDALONE
        OrigNewUnitsEnabled = NewUnitsEnabled;
#endif

        MTankDistance = ini.Get_Int("Aftermath", "MTankDistance", MTankDistance);
        QuakeUnitDamage = ini.Get_Fixed("Aftermath", "QuakeUnitDamage", QuakeUnitDamage);
        QuakeVesselDamage = ini.Get_Fixed("Aftermath", "QuakeVesselDamage", QuakeVesselDamage);

        if (QuakeVesselDamage <= (fixed)0) {
            QuakeVesselDamage = QuakeUnitDamage;
        }

        QuakeBuildingDamage = ini.Get_Fixed("Aftermath", "QuakeBuildingDamage", QuakeBuildingDamage);
        QuakeInfantryDamage = ini.Get_Int("Aftermath", "QuakeInfantryDamage", QuakeInfantryDamage);
        QuakeDelay = ini.Get_Int("Aftermath", "QuakeDelay", QuakeDelay);
        ChronoTankDuration = ini.Get_Fixed("Aftermath", "ChronoTankDuration", ChronoTankDuration);
        CarrierLaunchDelay = ini.Get_Int("Aftermath", "CarrierLaunchDelay", CarrierLaunchDelay);
        EngineerDamage = ini.Get_Fixed("Aftermath", "EngineerDamage", EngineerDamage);
        EngineerCaptureLevel = ini.Get_Fixed("Aftermath", "EngineerCaptureLevel", EngineerCaptureLevel);

        return true;
    }

    return false;
}

BOOL RulesClass::General(GameINIClass &ini)
{
    // TODO Requires GameINIClass::Get_UnitType, ChronalVortexClass
#ifndef CHRONOSHIFT_STANDALONE
    BOOL(*func)(const RulesClass *, GameINIClass &) = reinterpret_cast<BOOL(*)(const RulesClass *, GameINIClass &)>(0x005342DC);
    return func(this, ini);
#elif 0
    Aftermath(ini);

    if (ini.Find_Section("General") != nullptr) {
        TurboBoost = ini.Get_Fixed("General", "TurboBoost", TurboBoost);
        BadgerBombCount = ini.Get_Int("General", "BadgerBombCount", BadgerBombCount);
        CurleyShuffle = ini.Get_Bool("General", "CurleyShuffle", CurleyShuffle);
        FlashLowPower = ini.Get_Bool("General", "FlashLowPower", FlashLowPower);
        ChronoKillCargo = ini.Get_Bool("General", "ChronoKillCargo", ChronoKillCargo);
        ChronoDuration = ini.Get_Fixed("General", "ChronoDuration", ChronoDuration);
        FineDiffControl = ini.Get_Bool("General", "FineDiffControl", FineDiffControl);
        WaterCrateChance = ini.Get_Fixed("General", "WaterCrateChance", WaterCrateChance);
        SoloCrateMoney = ini.Get_Int("General", "SoloCrateMoney", SoloCrateMoney);
        ParabombTech = ini.Get_Int("General", "ParabombTech", ParabombTech);
        GPSTechLevel = ini.Get_Int("General", "GPSTechLevel", GPSTechLevel);
        UnitCrateType = ini.Get_UnitType("General", "UnitCrateType", UnitCrateType);

        // CCHyper Fixed by from original RA binary, this entry was loaded with Get_Fixed.
        OreExplosive = ini.Get_Bool("General", "OreExplosive", OreExplosive);
        GapRegenInterval = ini.Get_Fixed("General", "GapRegenInterval", GapRegenInterval);
        TeamDelay = ini.Get_Fixed("General", "TeamDelay", TeamDelay);
        SubmergeDelay = ini.Get_Fixed("General", "SubmergeDelay", SubmergeDelay);
        CloakDelay = ini.Get_Fixed("General", "CloakDelay", CloakDelay);
        GameSpeedBias = ini.Get_Fixed("General", "GameSpeedBias", GameSpeedBias);

        // Original doesn't appear to load this, rule.ini value loaded as 16th of lepton?
        // LZScanRadius = ini.Get_Lepton("General", "LZScanRadius", LZScanRadius);

        BaseBias = ini.Get_Fixed("General", "BaseBias", BaseBias);
        ExpSpread = ini.Get_Fixed("General", "ExpSpread", ExpSpread);
        FireSupress = ini.Get_Lepton("General", "FireSupress", FireSupress);
        ParaTech = ini.Get_Int("General", "ParaTech", ParaTech);
        SpyPlaneTech = ini.Get_Int("General", "SpyPlaneTech", SpyPlaneTech);
        SilverCrate = ini.Get_CrateType("General", "SilverCrate", SilverCrate);
        WoodCrate = ini.Get_CrateType("General", "WoodCrate", WoodCrate);
        WaterCrate = ini.Get_CrateType("General", "WaterCrate", WaterCrate);
        CrateMinimum = ini.Get_Int("General", "CrateMinimum", CrateMinimum);
        CrateMaximum = ini.Get_Int("General", "CrateMaximum", CrateMaximum);
        PlayerScatter = ini.Get_Bool("General", "PlayerScatter", PlayerScatter);
        PlayerReturnFire = ini.Get_Bool("General", "PlayerReturnFire", PlayerReturnFire);
        PlayerAutoCrush = ini.Get_Bool("General", "PlayerAutoCrush", PlayerAutoCrush);
        NamedCivilians = ini.Get_Bool("General", "NamedCivilians", NamedCivilians);
        OreGrows = ini.Get_Bool("General", "OreGrows", OreGrows);
        OreSpreads = ini.Get_Bool("General", "OreSpreads", OreSpreads);
        MineAware = ini.Get_Bool("General", "MineAware", MineAware);
        TreeTargeting = ini.Get_Bool("General", "TreeTargeting", TreeTargeting);
        SeparateAircraft = ini.Get_Bool("General", "SeparateAircraft", SeparateAircraft);
        DropZoneRadius = ini.Get_Lepton("General", "DropZoneRadius", DropZoneRadius);
        MessageDelay = ini.Get_Fixed("General", "MessageDelay", MessageDelay);
        SavourDelay = ini.Get_Fixed("General", "SavourDelay", SavourDelay);
        AVMineDamage = ini.Get_Int("General", "AVMineDamage", AVMineDamage);
        APMineDamage = ini.Get_Int("General", "APMineDamage", APMineDamage);
        BaseDefenseDelay = ini.Get_Fixed("General", "BaseDefenseDelay", BaseDefenseDelay);
        SuspendPriority = ini.Get_Int("General", "SuspendPriority", SuspendPriority);
        SuspendDelay = ini.Get_Fixed("General", "SuspendDelay", SuspendDelay);
        SurvivorRate = ini.Get_Fixed("General", "SurvivorRate", SurvivorRate);
        RadarJamRadius = ini.Get_Lepton("General", "RadarJamRadius", RadarJamRadius);
        ReloadRate = ini.Get_Fixed("General", "ReloadRate", ReloadRate);
        IdleActionFrequency = ini.Get_Fixed("General", "IdleActionFrequency", IdleActionFrequency);
        BuildupTime = ini.Get_Fixed("General", "BuildupTime", BuildupTime);
        OreTruckRate = ini.Get_Int("General", "OreTruckRate", OreTruckRate);
        AtomDamage = ini.Get_Int("General", "AtomDamage", AtomDamage);
        BailCount = ini.Get_Int("General", "BailCount", BailCount);
        BallisticScatter = ini.Get_Lepton("General", "BallisticScatter", BallisticScatter);
        BridgeStrength = ini.Get_Int("General", "BridgeStrength", BridgeStrength);
        BuildSpeed = ini.Get_Fixed("General", "BuildSpeed", BuildSpeed);
        ConditionGreen = ini.Get_Fixed("General", "ConditionGreen", ConditionGreen);
        ConditionYellow = ini.Get_Fixed("General", "ConditionYellow", ConditionYellow);
        ConditionRed = ini.Get_Fixed("General", "ConditionRed", ConditionRed);
        CrateRadius = ini.Get_Lepton("General", "CrateRadius", CrateRadius);
        Crush = ini.Get_Lepton("General", "Crush", Crush);
        DamageDelay = ini.Get_Fixed("General", "DamageDelay", DamageDelay);
        GapRadius = ini.Get_Int("General", "GapRadius", GapRadius);
        GemValue = ini.Get_Int("General", "GemValue", GemValue);
        GoldValue = ini.Get_Int("General", "GoldValue", GoldValue);
        GrowthRate = ini.Get_Fixed("General", "GrowthRate", GrowthRate);
        HomingScatter = ini.Get_Lepton("General", "HomingScatter", HomingScatter);
        Incoming = ini.Get_MPHType("General", "Incoming", Incoming);
        IronCurtain = ini.Get_Fixed("General", "IronCurtain", IronCurtain);
        AllyReveal = ini.Get_Bool("General", "AllyReveal", AllyReveal);
        EnemyHealth = ini.Get_Bool("General", "EnemyHealth", EnemyHealth);
        MCVUndeploy = ini.Get_Bool("General", "MCVUndeploy", MCVUndeploy);
        MaxDamage = ini.Get_Int("General", "MaxDamage", MaxDamage);
        MinDamage = ini.Get_Int("General", "MinDamage", MinDamage);
        ProneDamage = ini.Get_Fixed("General", "ProneDamage", ProneDamage);
        QuakeDamage = ini.Get_Fixed("General", "QuakeDamage", QuakeDamage);
        QuakeChance = ini.Get_Fixed("General", "QuakeChance", QuakeChance);
        RefundPercent = ini.Get_Fixed("General", "RefundPercent", RefundPercent);
        RepairPercent = ini.Get_Fixed("General", "RepairPercent", RepairPercent);
        RepairStep = ini.Get_Int("General", "RepairStep", RepairStep);
        URepairPercent = ini.Get_Fixed("General", "URepairPercent", URepairPercent);
        URepairStep = ini.Get_Int("General", "URepairStep", URepairStep);
        RepairRate = ini.Get_Fixed("General", "RepairRate", RepairRate);
        ShroudRate = ini.Get_Fixed("General", "ShroudRate", ShroudRate);
        SpeakDelay = ini.Get_Fixed("General", "SpeakDelay", SpeakDelay);
        Stray = ini.Get_Lepton("General", "Stray", Stray);
        CloseEnough = ini.Get_Lepton("General", "CloseEnough", CloseEnough);
        TimerWarning = ini.Get_Fixed("General", "TimerWarning", TimerWarning);
        MovieTime = ini.Get_Fixed("General", "MovieTime", MovieTime);
        C4Delay = ini.Get_Fixed("General", "C4Delay", C4Delay);
        ChronoTechLevel = ini.Get_Int("General", "ChronoTechLevel", ChronoTechLevel);
        CrateRegen = ini.Get_Fixed("General", "CrateRegen", CrateRegen);
        VortexRange = ini.Get_Lepton("General", "VortexRange", VortexRange);
        VortexSpeed = ini.Get_MPHType("General", "VortexSpeed", VortexSpeed); // The original code for this is odd.
        VortexDamage = ini.Get_Int("General", "VortexDamage", VortexDamage);
        VortexChance = ini.Get_Fixed("General", "VortexChance", VortexChance);

        // Set variables within the vortex class global instance.
        ChronalVortex.VortexRange = VortexRange / 256;
        ChronalVortex.VortexSpeed = VortexSpeed;
        ChronalVortex.VortexDamage = VortexDamage;

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
        MPlayerMoney = ini.Get_Int("MultiplayerDefaults", "Money", MPlayerMoney);
        MPlayerMaxMoney = ini.Get_Int("MultiplayerDefaults", "MaxMoney", MPlayerMaxMoney);
        MPlayerShadowGrow = ini.Get_Bool("MultiplayerDefaults", "ShadowGrow", MPlayerShadowGrow);
        MPlayerBases = ini.Get_Bool("MultiplayerDefaults", "Bases", MPlayerBases);
        MPlayerOreGrows = ini.Get_Bool("MultiplayerDefaults", "OreGrows", MPlayerOreGrows);
        MPlayerCrates = ini.Get_Bool("MultiplayerDefaults", "Crates", MPlayerCrates);
        MPlayerBit16 = ini.Get_Bool("MultiplayerDefaults", "Bit16", MPlayerBit16);
        MPlayerCaptureTheFlag = ini.Get_Bool("MultiplayerDefaults", "CaptureTheFlag", MPlayerCaptureTheFlag);

        return true;
    }

    return false;
}

BOOL RulesClass::Recharge(GameINIClass &ini)
{
    if (ini.Find_Section("Recharge") != nullptr) {
        RechargeSonar = ini.Get_Fixed("Recharge", "Sonar", RechargeSonar);
        RechargeChrono = ini.Get_Fixed("Recharge", "Chrono", RechargeChrono);
        RechargeParaBomb = ini.Get_Fixed("Recharge", "ParaBomb", RechargeParaBomb);
        RechargeParatrooper = ini.Get_Fixed("Recharge", "Paratrooper", RechargeParatrooper);
        RechargeParaSaboteur = ini.Get_Fixed("Recharge", "Saboteur", RechargeParaSaboteur);
        RechargeSpyPlane = ini.Get_Fixed("Recharge", "SpyPlane", RechargeSpyPlane);
        RechargeIronCurtain = ini.Get_Fixed("Recharge", "IronCurtain", RechargeIronCurtain);
        RechargeGPS = ini.Get_Fixed("Recharge", "GPS", RechargeGPS);
        RechargeNuke = ini.Get_Fixed("Recharge", "Nuke", RechargeNuke);

        return true;
    }

    return false;
}

BOOL RulesClass::Heap_Maximums(GameINIClass &ini)
{
    // TODO Requires WarheadTypeClass and WeaponTypeClass or those heap inits moving elsewhere.
#ifndef CHRONOSHIFT_STANDALONE
    BOOL(*func)(const RulesClass *, GameINIClass &) = reinterpret_cast<BOOL(*)(const RulesClass *, GameINIClass &)>(0x00535F38);
    return func(this, ini);
#elif 0
    if (ini.Find_Section("Maximums") != nullptr) {
        Players = ini.Get_Int("Maximums", "Players", Players);
        MaxAircraft = ini.Get_Int("Maximums", "Aircraft", MaxAircraft);
        MaxAnim = ini.Get_Int("Maximums", "Anim", MaxAnim);
        MaxBuilding = ini.Get_Int("Maximums", "Building", MaxBuilding);
        MaxBullet = ini.Get_Int("Maximums", "Bullet", MaxBullet);
        MaxFactory = ini.Get_Int("Maximums", "Factory", MaxFactory);
        MaxInfantry = ini.Get_Int("Maximums", "Infantry", MaxInfantry);
        MaxOverlay = ini.Get_Int("Maximums", "Overlay", MaxOverlay);
        MaxSmudge = ini.Get_Int("Maximums", "Smudge", MaxSmudge);
        MaxTeam = ini.Get_Int("Maximums", "Team", MaxTeam);
        MaxTeamType = ini.Get_Int("Maximums", "TeamType", MaxTeamType);
        MaxTemplate = ini.Get_Int("Maximums", "Template", MaxTemplate);
        MaxTerrain = ini.Get_Int("Maximums", "Terrain", MaxTerrain);
        MaxTrigger = ini.Get_Int("Maximums", "Trigger", MaxTrigger);
        MaxUnit = ini.Get_Int("Maximums", "Unit", MaxUnit);
        MaxVessel = ini.Get_Int("Maximums", "Vessel", MaxVessel);
        MaxProjectile = ini.Get_Int("Maximums", "Projectile", MaxProjectile);
        MaxWeapon = ini.Get_Int("Maximums", "Weapon", MaxWeapon);
        MaxWarhead = ini.Get_Int("Maximums", "Warhead", MaxWarhead);
        MaxTrigType = ini.Get_Int("Maximums", "TrigType", MaxTrigType);

        // Moved, see warhead and weapon headers for more info
        // TODO, move Set_Heap()'s to RTS_Init() in init.cpp'

        //
        //
        //
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
        AttackInterval = ini.Get_Fixed("AI", "AttackInterval", AttackInterval);
        AttackDelay = ini.Get_Fixed("AI", "AttackDelay", AttackDelay);
        PatrolScan = ini.Get_Fixed("AI", "PatrolScan", PatrolScan);
        CreditReserve = ini.Get_Int("AI", "CreditReserve", CreditReserve);
        PathDelay = ini.Get_Fixed("AI", "PathDelay", PathDelay);
        OreNearScan = ini.Get_Lepton("AI", "OreNearScan", OreNearScan);
        OreFarScan = ini.Get_Lepton("AI", "OreFarScan", OreFarScan);
        AutocreateTime = ini.Get_Fixed("AI", "AutocreateTime", AutocreateTime);
        InfantryReserve = ini.Get_Int("AI", "InfantryReserve", InfantryReserve);
        InfantryBaseMult = ini.Get_Int("AI", "InfantryBaseMult", InfantryBaseMult);
        PowerSurplus = ini.Get_Int("AI", "PowerSurplus", PowerSurplus);
        BaseSizeAdd = ini.Get_Int("AI", "BaseSizeAdd", BaseSizeAdd);
        RefineryRatio = ini.Get_Fixed("AI", "RefineryRatio", RefineryRatio);
        RefineryLimit = ini.Get_Int("AI", "RefineryLimit", RefineryLimit);
        BarracksRatio = ini.Get_Fixed("AI", "BarracksRatio", BarracksRatio);
        BarracksLimit = ini.Get_Int("AI", "BarracksLimit", BarracksLimit);
        WarRatio = ini.Get_Fixed("AI", "WarRatio", WarRatio);
        WarLimit = ini.Get_Int("AI", "WarLimit", WarLimit);
        DefenseRatio = ini.Get_Fixed("AI", "DefenseRatio", DefenseRatio);
        DefenseLimit = ini.Get_Int("AI", "DefenseLimit", DefenseLimit);
        AARatio = ini.Get_Fixed("AI", "AARatio", AARatio);
        AALimit = ini.Get_Int("AI", "AALimit", AALimit);
        TeslaRatio = ini.Get_Fixed("AI", "TeslaRatio", TeslaRatio);
        TeslaLimit = ini.Get_Int("AI", "TeslaLimit", TeslaLimit);
        HelipadRatio = ini.Get_Fixed("AI", "HelipadRatio", HelipadRatio);
        HelipadLimit = ini.Get_Int("AI", "HelipadLimit", HelipadLimit);
        AirstripRatio = ini.Get_Fixed("AI", "AirstripRatio", AirstripRatio);
        AirstripLimit = ini.Get_Int("AI", "AirstripLimit", AirstripLimit);
        CompEasyBonus = ini.Get_Bool("AI", "CompEasyBonus", CompEasyBonus);
        Paranoid = ini.Get_Bool("AI", "Paranoid", Paranoid);
        PowerEmergency = ini.Get_Fixed("AI", "PowerEmergency", PowerEmergency);
    }

    return false;
}

BOOL RulesClass::Powerups(GameINIClass &ini)
{
    // TODO Needs AnimTypeClass.
#ifndef CHRONOSHIFT_STANDALONE
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
                    CrateClass::CrateShares[crate] = strtol(share, nullptr, 10);
                }

                char *anim = strtok(value, ",");

                if (anim) {
                    strtrim(anim);
                    CrateClass::CrateAnims[crate] = AnimTypeClass::From_Name(anim);
                }

                char *data = strtok(value, ",");

                if (strpbrk(value, ".%")) {
                    CrateClass::CrateData[crate] = fixed(data).To_Int();
                } else {
                    strtrim(data);
                    CrateClass::CrateData[crate] = strtol(data, nullptr, 10);
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

BOOL RulesClass::Themes(GameINIClass &ini)
{
    char entry[INIClass::MAX_LINE_LENGTH];

    if (ini.Find_Section("ThemeControl") != nullptr) {
        for (ThemeType theme = THEME_FIRST; theme < THEME_COUNT; ++theme) {
            const char *theme_name = Theme.Base_Name(theme);
            // Already found section, so we know it exists
            // if ( theme_name ? ini.Find_Entry("ThemeControl", theme_name) != nullptr : ini.Find_Section("ThemeControl") !=
            // nullptr ) {
            if (theme_name && ini.Find_Entry("ThemeControl", theme_name) != nullptr) {
                ini.Get_String("ThemeControl", theme_name, "", entry, sizeof(entry));

                char *scen_val = strtok(entry, ",");
                int scen = scen_val ? strtol(scen_val, nullptr, 10) : 1;

                char *owner_val = strtok(entry, ",");
                int owner = owner_val ? HouseTypeClass::Owner_From_Name(owner_val) : -1;

                Theme.Set_Theme_Data(theme, scen, owner);
            }
        }

        return true;
    }

    return false;
}

BOOL RulesClass::IQ(GameINIClass &ini)
{
    if (ini.Find_Section("IQ") != nullptr) {
        IQControls.MaxIQLevels = ini.Get_Int("IQ", "MaxIQLevels", IQControls.MaxIQLevels);
        IQControls.SuperWeapons = ini.Get_Int("IQ", "SuperWeapons", IQControls.SuperWeapons);
        IQControls.Production = ini.Get_Int("IQ", "Production", IQControls.Production);
        IQControls.GuardArea = ini.Get_Int("IQ", "GuardArea", IQControls.GuardArea);
        IQControls.RepairSell = ini.Get_Int("IQ", "RepairSell", IQControls.RepairSell);
        IQControls.AutoCrush = ini.Get_Int("IQ", "AutoCrush", IQControls.AutoCrush);
        IQControls.Scatter = ini.Get_Int("IQ", "Scatter", IQControls.Scatter);
        IQControls.ContentScan = ini.Get_Int("IQ", "ContentScan", IQControls.ContentScan);
        IQControls.Aircraft = ini.Get_Int("IQ", "Aircraft", IQControls.Aircraft);
        IQControls.Harvester = ini.Get_Int("IQ", "Harvester", IQControls.Harvester);
        IQControls.SellBack = ini.Get_Int("IQ", "SellBack", IQControls.SellBack);

        return true;
    }

    return false;
}

BOOL RulesClass::Objects(GameINIClass &ini)
{
    // TODO Needs most *TypeClass classes.
#ifndef CHRONOSHIFT_STANDALONE
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
        MissionControlClass::MissionControl[mission].Read_INI(ini);
    }

    return true;
#else
    return false;
#endif
}

BOOL RulesClass::Difficulty(GameINIClass &ini)
{
    Difficulty_Get(ini, Difficulties[0], "Easy");
    Difficulty_Get(ini, Difficulties[1], "Normal");
    Difficulty_Get(ini, Difficulties[2], "Difficult");

    return true;
}

BOOL RulesClass::Land_Types(GameINIClass &ini)
{
    // TODO Needs most *TypeClass classes.
#ifndef CHRONOSHIFT_STANDALONE
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
