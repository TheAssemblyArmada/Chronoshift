/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class information relating to the current scenario.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
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
#include "random.h"
#include "ttimer.h"

class ScenarioClass
{
public:
    ScenarioClass();

private:
    RandomClass SyncRandom;
    DiffType HumanDifficulty;
    DiffType AIDifficulty;
    BasicTimerClass<FrameTimerClass> ElapsedTimer;
    int16_t Waypoints[100];
    int16_t UnkWaypoint;
    BasicTimerClass<FrameTimerClass> GlobalTimer;
    BasicTimerClass<FrameTimerClass> SomeTimer;
    int ScenarioIndex;
    char Theater;
    char ScenarioName[512];
    char ScenarioDescription[44];
    char IntroMovie;
    char BriefMovie;
    char WinMovie;
    char LoseMovie;
    char ActionMovie;
    char BriefingText[1024];
    char TransitTheme;
    char SomeHouse;
    fixed CarryOverMoney;
    int CarryOverMoneyAmount;
    int CarryOverCap;
    int CarryOverPercent;
    int GlobalVariables[30];
    int16_t Views[4];
    int BridgeCount;
    int CarryOverTime;
#ifndef RAPP_NO_BITFIELDS
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
            bool Bit1_32 : 1; // & 32 looks like FadeToBW
            bool Bit1_64 : 1; // & 64 and this looks like BWFadeReset
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
    bool _DestroyBridges; //BridgeDestroyed?
    bool _GlobalsChanged; //VariablesChanged?
    bool ToCarryOver;
    bool ToInherit;
    bool CivEvac;
    bool Bit1_32; //looks like FadeToBW
    bool Bit1_64; //and this looks like BWFadeReset
    bool EndOfGame;
    bool TimerInherit;
    bool NoSpyPlane;
    bool SkipScore;
    bool OneTimeOnly;
    bool SkipMapSelect;
    bool TruckCrate;
    bool FillSilos;
#endif
    BasicTimerClass<FrameTimerClass> FadeTimer;
    BasicTimerClass<FrameTimerClass> AutoSonarTime;
    int field_7CF;
    int field_7D3;
};

#ifndef RAPP_STANDALONE
#include "hooker.h"

extern ScenarioClass &Scen;
#else
extern ScenarioClass Scen;
#endif

#endif // SCENARIO_H
