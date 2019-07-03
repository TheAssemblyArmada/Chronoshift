/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class information relating to the current scenario.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef SCENARIO_H
#define SCENARIO_H

#include "always.h"
#include "difficulty.h"
#include "fixed.h"
#include "gametypes.h"
#include "housetype.h"
#include "movie.h"
#include "random.h"
#include "theater.h"
#include "theme.h"
#include "ttimer.h"

#define VIEW_COUNT 4

enum ScenarioPlayerEnum
{
    SCEN_PLAYER_SPAIN,
    SCEN_PLAYER_GREECE,
    SCEN_PLAYER_USSR,
    SCEN_PLAYER_SPECIAL,
};

enum ScenarioDirEnum
{
    SCEN_DIR_RANDOM = -1,
    SCEN_DIR_EAST,
    SCEN_DIR_WEST,
};

enum ScenarioVarEnum
{
    SCEN_VAR_ANY = -1,
    SCEN_VAR_A,
    SCEN_VAR_B,
    SCEN_VAR_C,
    SCEN_VAR_D,
    SCEN_VAR_COUNT,
};

class ScenarioClass
{
    enum
    {
        WAYPOINT_COUNT = 100,
    };

public:
    ScenarioClass();

    void Set_Scenario_Name(const char *scen_name);
    void Set_Scenario_Name(int index, ScenarioPlayerEnum player, ScenarioDirEnum dir, ScenarioVarEnum var);
    void Do_BW_Fade();
    void Do_Fade_AI();
    BOOL Set_Global_To(int global, BOOL value);
    int Get_Random_Value(int min, int max) { return SyncRandom(min, max); }
    void Set_Random_Seed(int seed) { SyncRandom = seed; }
    DiffType Get_Human_Difficulty() const { return HumanDifficulty; }
    DiffType Get_AI_Difficulty() const { return AIDifficulty; }
    int Get_Scenario_Index() const { return ScenarioIndex; }
    TheaterType Get_Theater() const { return Theater; }
    void Set_Theater(TheaterType theater) { Theater = theater; }
    cell_t Get_Waypoint(int waypoint_num) const { return Waypoints[waypoint_num]; }
    void Set_Waypoint(int waypoint_num, cell_t cell) { Waypoints[waypoint_num] = cell; }
    int Get_Elapsed_Time() { return ElapsedTimer.Time(); }
    int Get_Global_Time() { return GlobalTimer.Time(); }
    BOOL Global_Timer_Running() { return GlobalTimer != 0; }
    cell_t Get_View(int index) const { return Views[index]; }
    void Set_View(int index, cell_t cell) { Views[index] = cell; }

    BOOL Get_field_7CF() const { return field_7CF; }
    BOOL Get_field_7D3() const { return field_7D3; }

#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
    void Hook_Set_Scenario_Name1(const char *scen_name) { Set_Scenario_Name(scen_name); }
    void Hook_Set_Scenario_Name2(int index, ScenarioPlayerEnum player, ScenarioDirEnum dir, ScenarioVarEnum var)
    {
        Set_Scenario_Name(index, player, dir, var);
    }

#endif

private:
    RandomClass SyncRandom;
    DiffType HumanDifficulty;
    DiffType AIDifficulty;
    TCountDownTimerClass<FrameTimerClass> ElapsedTimer;
    cell_t Waypoints[WAYPOINT_COUNT];
    cell_t UnkWaypoint;
    TCountDownTimerClass<FrameTimerClass> GlobalTimer;
    TCountDownTimerClass<FrameTimerClass> SomeTimer;
    int ScenarioIndex;
    TheaterType Theater;
    char ScenarioName[512];
    char ScenarioDescription[44];
    MovieType IntroMovie;
    MovieType BriefMovie;
    MovieType WinMovie;
    MovieType LoseMovie;
    MovieType ActionMovie;
    char BriefingText[1024];
    ThemeType TransitTheme;
    HousesType SomeHouse;
    fixed_t CarryOverMoney;
    int CarryOverMoneyAmount;
    int CarryOverCap;
    int CarryOverPercent;
    int GlobalVariables[30];
    cell_t Views[VIEW_COUNT];
    int BridgeCount;
    int CarryOverTime;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool _DestroyBridges : 1; // & 1 BridgeDestroyed?
            bool _GlobalsChanged : 1; // & 2 VariablesChanged?
            bool ToCarryOver : 1; // & 4
            bool ToInherit : 1; // & 8
            bool CivEvac : 1; // & 16
            bool FadeOutBW : 1; // & 32 looks like FadeToBW
            bool FadeInBW : 1; // & 64 and this looks like BWFadeReset
            bool EndOfGame : 1; // & 128
            bool TimerInherit : 1; // & 1
            bool NoSpyPlane : 1; // & 2
            bool SkipScore : 1; // & 4
            bool OneTimeOnly : 1; // & 8
            bool SkipMapSelect : 1; // & 16
            bool TruckCrate : 1; // & 32
            bool FillSilos : 1; // & 64
        };
        int Bitfield;
    };
#else
    bool _DestroyBridges; // BridgeDestroyed?
    bool _GlobalsChanged; // VariablesChanged?
    bool ToCarryOver;
    bool ToInherit;
    bool CivEvac;
    bool FadeOutBW; // looks like FadeToBW
    bool FadeInBW; // and this looks like BWFadeReset
    bool EndOfGame;
    bool TimerInherit;
    bool NoSpyPlane;
    bool SkipScore;
    bool OneTimeOnly;
    bool SkipMapSelect;
    bool TruckCrate;
    bool FillSilos;
#endif
    TCountDownTimerClass<FrameTimerClass> FadeTimer;
    TCountDownTimerClass<FrameTimerClass> AutoSonarTime;
    BOOL field_7CF; // These two are related to proposing a game draw within onlined games.
    BOOL field_7D3;
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"

extern ScenarioClass &Scen;

inline void ScenarioClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x0053CFB0, *ScenarioClass::Hook_Set_Scenario_Name1);
    Hook_Function(0x0053CD80, *ScenarioClass::Hook_Set_Scenario_Name2);
    Hook_Function(0x00539BF8, *ScenarioClass::Do_BW_Fade);
    Hook_Function(0x00539C40, *ScenarioClass::Do_Fade_AI);
#endif
}
#else
extern ScenarioClass Scen;
#endif

#endif // SCENARIO_H
