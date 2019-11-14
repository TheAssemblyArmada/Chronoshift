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

enum ScenarioEnums
{
    VIEW_COUNT = 4,
    WAYPOINT_COUNT = 101,
};

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
public:
    ScenarioClass();

    void Set_Scenario_Name(const char *scen_name);
    void Set_Scenario_Name(int index, ScenarioPlayerEnum player, ScenarioDirEnum dir, ScenarioVarEnum var);
    void Do_BW_Fade();
    void Do_Fade_AI();
    BOOL Set_Global_To(int global, BOOL value);
    int Get_Random_Value(int min, int max) { return m_SyncRandom(min, max); }
    void Set_Random_Seed(int seed) { m_SyncRandom = seed; }

    DiffType Get_Human_Difficulty() const { return m_HumanDifficulty; }
    DiffType Get_AI_Difficulty() const { return m_AIDifficulty; }
    void Set_Difficulties(DiffType human, DiffType ai) { m_HumanDifficulty = human; m_AIDifficulty = ai; }
    TCountDownTimerClass<FrameTimerClass> &Elapsed_Timer() { return m_ElapsedTimer; }
    int Get_Elapsed_Time() { return m_ElapsedTimer.Time(); }
    TCountDownTimerClass<FrameTimerClass> &Global_Timer() { return m_GlobalTimer; }
    int Get_Global_Time() { return m_GlobalTimer.Time(); }
    void Set_Global_Time(int value) { m_GlobalTimer.Set(value); }
    BOOL Global_Timer_Running() const { return m_GlobalTimer.Started(); }
    void Start_Global_Timer() { return m_GlobalTimer.Start(); }
    void Stop_Global_Timer() { return m_GlobalTimer.Stop(); }
    TCountDownTimerClass<FrameTimerClass> &Some_Timer() { return m_SomeTimer; }
    int Get_Scenario_Index() const { return m_ScenarioIndex; }
    TheaterType Get_Theater() const { return m_Theater; }
    void Set_Theater(TheaterType theater) { m_Theater = theater; }
    const char *Scenario_Name() const { return m_ScenarioName; }
    const char *Scenario_Description() const { return m_ScenarioDescription; }
    cell_t Get_Waypoint(int waypoint_num) const { return m_Waypoints[waypoint_num]; }
    void Set_Waypoint(int waypoint_num, cell_t cell) { m_Waypoints[waypoint_num] = cell; }
    void Set_Carry_Over_Money(fixed_t value) { m_CarryOverMoney = value; }
    cell_t Get_View(int index) const { return m_Views[index]; }
    void Set_View(int index, cell_t cell) { m_Views[index] = cell; }
    int Get_Bridge_Count() const { return m_BridgeCount; }
    void Set_Bridge_Count(int count) { m_BridgeCount = count; }
    BOOL Get_Global(int index) const { return m_GlobalVariables[index]; }
    BOOL Get_field_7CF() const { return m_field_7CF; }
    BOOL Get_field_7D3() const { return m_field_7D3; }
    BOOL Evacuate_Civilians() const { return m_CivEvac; }

#ifdef GAME_DLL
    void Hook_Set_Scenario_Name1(const char *scen_name) { Set_Scenario_Name(scen_name); }
    void Hook_Set_Scenario_Name2(int index, ScenarioPlayerEnum player, ScenarioDirEnum dir, ScenarioVarEnum var)
    {
        Set_Scenario_Name(index, player, dir, var);
    }

#endif

private:
    RandomClass m_SyncRandom;
    DiffType m_HumanDifficulty;
    DiffType m_AIDifficulty;
    TCountDownTimerClass<FrameTimerClass> m_ElapsedTimer;
    cell_t m_Waypoints[WAYPOINT_COUNT]; // 98 = unknown, 99 = unknown, 100 = Home cell, 101 = Super cell.
    TCountDownTimerClass<FrameTimerClass> m_GlobalTimer;
    TCountDownTimerClass<FrameTimerClass> m_SomeTimer;
    int m_ScenarioIndex;
    TheaterType m_Theater;
    char m_ScenarioName[512];
    char m_ScenarioDescription[44];
    MovieType m_IntroMovie;
    MovieType m_BriefMovie;
    MovieType m_WinMovie;
    MovieType m_LoseMovie;
    MovieType m_ActionMovie;
    char m_BriefingText[1024];
    ThemeType m_TransitTheme;
    HousesType m_SomeHouse;
    fixed_t m_CarryOverMoney;
    int m_CarryOverMoneyAmount;
    int m_CarryOverCap;
    int m_CarryOverPercent;
    int m_GlobalVariables[30];
    cell_t m_Views[VIEW_COUNT];
    int m_BridgeCount;
    int m_CarryOverTime;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_DestroyBridges : 1; // & 1 BridgeDestroyed?
    BOOL m_GlobalsChanged : 1; // & 2 VariablesChanged?
    BOOL m_ToCarryOver : 1; // & 4
    BOOL m_ToInherit : 1; // & 8
    BOOL m_CivEvac : 1; // & 16
    BOOL m_FadeOutBW : 1; // & 32
    BOOL m_FadeInBW : 1; // & 64
    BOOL m_EndOfGame : 1; // & 128

    BOOL m_TimerInherit : 1; // & 1
    BOOL m_NoSpyPlane : 1; // & 2
    BOOL m_SkipScore : 1; // & 4
    BOOL m_OneTimeOnly : 1; // & 8
    BOOL m_SkipMapSelect : 1; // & 16
    BOOL m_TruckCrate : 1; // & 32
    BOOL m_FillSilos : 1; // & 64
#else
    bool m_DestroyBridges; // BridgeDestroyed?
    bool m_GlobalsChanged; // VariablesChanged?
    bool m_ToCarryOver;
    bool m_ToInherit;
    bool m_CivEvac;
    bool m_FadeOutBW;
    bool m_FadeInBW;
    bool m_EndOfGame;
    bool m_TimerInherit;
    bool m_NoSpyPlane;
    bool m_SkipScore;
    bool m_OneTimeOnly;
    bool m_SkipMapSelect;
    bool m_TruckCrate;
    bool m_FillSilos;
#endif
    TCountDownTimerClass<FrameTimerClass> m_FadeTimer;
    TCountDownTimerClass<FrameTimerClass> m_AutoSonarTimer;
    BOOL m_field_7CF; // These two are related to proposing a game draw within onlined games.
    BOOL m_field_7D3;
};

#ifdef GAME_DLL
extern ScenarioClass &Scen;
#else
extern ScenarioClass Scen;
#endif

BOOL Start_Scenario(const char *filename, BOOL play_movies = false);
BOOL Read_Scenario(const char *filename);

#endif // SCENARIO_H
