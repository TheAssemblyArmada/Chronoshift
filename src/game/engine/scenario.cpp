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
#include "scenario.h"

#ifndef RAPP_STANDALONE
ScenarioClass &Scen = Make_Global<ScenarioClass>(0x006678E8);
#else
ScenarioClass Scen;
#endif

ScenarioClass::ScenarioClass() :
    SyncRandom(),
    HumanDifficulty(DIFF_NONE),
    AIDifficulty(DIFF_NONE),
    ElapsedTimer(),
    GlobalTimer(),
    ScenarioIndex(-1),
    Theater(THEATER_NONE),
    IntroMovie(MOVIE_NONE),
    BriefMovie(MOVIE_NONE),
    WinMovie(MOVIE_NONE),
    LoseMovie(MOVIE_NONE),
    ActionMovie(MOVIE_NONE),
    TransitTheme(-1/*THEME_NONE*/),
    SomeHouse(HOUSES_NONE),
    CarryOverMoney(0),
    CarryOverMoneyAmount(0),
    CarryOverCap(0),
    CarryOverPercent(0),
    BridgeCount(0),
    CarryOverTime(0),
    _DestroyBridges(false),
    _GlobalsChanged(false),
    ToCarryOver(false),
    ToInherit(false),
    CivEvac(false),
    Bit1_32(false),
    Bit1_64(false),
    EndOfGame(false),
    TimerInherit(false),
    NoSpyPlane(false),
    SkipScore(false),
    OneTimeOnly(false),
    SkipMapSelect(false),
    TruckCrate(false),
    FillSilos(false),
    FadeTimer(),
    AutoSonarTime()
{
    for (int index = 0; index < ARRAY_SIZE(Waypoints); ++index) {
        Waypoints[index] = -1;
    }

    UnkWaypoint = -1;
    ScenarioName[0] = '\0';
    ScenarioDescription[0] = '\0';
    BriefingText[0] = '\0';
    memset(GlobalVariables, 0, sizeof(GlobalVariables));
    memset(Views, 0, sizeof(Views));
}
