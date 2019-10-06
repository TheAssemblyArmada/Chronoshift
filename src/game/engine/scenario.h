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
        WAYPOINT_COUNT = 101,
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
    void Set_Difficulties(DiffType human, DiffType ai) { HumanDifficulty = human; AIDifficulty = ai; }
    TCountDownTimerClass<FrameTimerClass> &Elapsed_Timer() { return ElapsedTimer; }
    int Get_Elapsed_Time() { return ElapsedTimer.Time(); }
    TCountDownTimerClass<FrameTimerClass> &Global_Timer() { return GlobalTimer; }
    int Get_Global_Time() { return GlobalTimer.Time(); }
    BOOL Global_Timer_Running() { return GlobalTimer != 0; }
    TCountDownTimerClass<FrameTimerClass> &Some_Timer() { return SomeTimer; }
    int Get_Scenario_Index() const { return ScenarioIndex; }
    TheaterType Get_Theater() const { return Theater; }
    void Set_Theater(TheaterType theater) { Theater = theater; }
    const char *Scenario_Name() const { return ScenarioName; }
    const char *Scenario_Description() const { return ScenarioDescription; }
    cell_t Get_Waypoint(int waypoint_num) const { return Waypoints[waypoint_num]; }
    void Set_Waypoint(int waypoint_num, cell_t cell) { Waypoints[waypoint_num] = cell; }
    void Set_Carry_Over_Money(fixed_t value) { CarryOverMoney = value; }
    cell_t Get_View(int index) const { return Views[index]; }
    void Set_View(int index, cell_t cell) { Views[index] = cell; }
    void Set_Bridge_Count(int count) { BridgeCount = count; }

    BOOL Civ_Evac() const { return CivEvac; }

    BOOL Get_field_7CF() const { return field_7CF; }
    BOOL Get_field_7D3() const { return field_7D3; }

#ifdef GAME_DLL
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
    cell_t Waypoints[WAYPOINT_COUNT]; // 98 = unknown, 99 = unknown, 100 = Home cell, 101 = Super cell.
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
    BOOL _DestroyBridges : 1; // & 1 BridgeDestroyed?
    BOOL _GlobalsChanged : 1; // & 2 VariablesChanged?
    BOOL ToCarryOver : 1; // & 4
    BOOL ToInherit : 1; // & 8
    BOOL CivEvac : 1; // & 16
    BOOL FadeOutBW : 1; // & 32
    BOOL FadeInBW : 1; // & 64
    BOOL EndOfGame : 1; // & 128

    BOOL TimerInherit : 1; // & 1
    BOOL NoSpyPlane : 1; // & 2
    BOOL SkipScore : 1; // & 4
    BOOL OneTimeOnly : 1; // & 8
    BOOL SkipMapSelect : 1; // & 16
    BOOL TruckCrate : 1; // & 32
    BOOL FillSilos : 1; // & 64
#else
    bool _DestroyBridges; // BridgeDestroyed?
    bool _GlobalsChanged; // VariablesChanged?
    bool ToCarryOver;
    bool ToInherit;
    bool CivEvac;
    bool FadeOutBW;
    bool FadeInBW;
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

#ifdef GAME_DLL
extern ScenarioClass &Scen;
#else
extern ScenarioClass Scen;
#endif

BOOL Start_Scenario(const char *filename, BOOL play_movies = false);
BOOL Read_Scenario(const char *filename);

#endif // SCENARIO_H
