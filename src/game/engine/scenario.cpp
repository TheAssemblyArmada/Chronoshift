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
#include "ccfileclass.h"
#include "gameoptions.h"
#include "stringex.h"
#include <cstdio>

using std::snprintf;

#ifndef CHRONOSHIFT_STANDALONE
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
    TransitTheme(THEME_NONE),
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
    FadeOutBW(false),
    FadeInBW(false),
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

/**
 * @brief Sets the scenario name and calculates the scenario index from the string.
 *
 * 0x0053CFB0
 */
void ScenarioClass::Set_Scenario_Name(const char *scen_name)
{
    char buffer[3];

    if (scen_name == nullptr) {
        return;
    }

    strlcpy(ScenarioName, scen_name, sizeof(ScenarioName));
    DEBUG_LOG("Set_Scenario_Name() - ScenarioName set to '%s'\n", ScenarioName);

    // Get the number part of the scenario name and calculate the ScenarioIndex.
    memcpy(&buffer, &ScenarioName[3], sizeof(buffer));
    buffer[2] = '\0';

    if (buffer[0] <= '9' && buffer[1] <= '9') {
        ScenarioIndex = atoi(buffer);
    } else {
        char digit1;
        char digit2;

        if (buffer[0] > '9') {
            digit1 = buffer[0] - 'A';
        } else {
            digit1 = buffer[0] - '0';
        }

        if (buffer[1] > '9') {
            digit2 = buffer[1] - '7';
        } else {
            digit2 = buffer[1] - '0';
        }

        ScenarioIndex = digit2 + 36 * digit1;
    }
}

/**
 * @brief Constructs the scenario name from the index and variables.
 *
 * 0x0053CD80
 */
void ScenarioClass::Set_Scenario_Name(int index, ScenarioPlayerEnum player, ScenarioDirEnum dir, ScenarioVarEnum var)
{
    char prefixvalue = '\0';
    char dirvalue = '\0';
    char varvalue = '\0';
    ScenarioIndex = index;

    switch (player) {
        case SCEN_PLAYER_SPAIN:
            prefixvalue = HouseTypeClass::As_Reference(HOUSES_SPAIN).Get_Prefix();
            break;

        case SCEN_PLAYER_GREECE:
            prefixvalue = HouseTypeClass::As_Reference(HOUSES_GREECE).Get_Prefix();
            break;

        case SCEN_PLAYER_USSR:
            prefixvalue = HouseTypeClass::As_Reference(HOUSES_USSR).Get_Prefix();
            break;

        case SCEN_PLAYER_SPECIAL:
            prefixvalue = HouseTypeClass::As_Reference(HOUSES_SPECIAL).Get_Prefix();
            break;

        default:
            prefixvalue = HouseTypeClass::As_Reference(HOUSES_MULTI_1).Get_Prefix();
            break;
    };

    switch (dir) {
        case SCEN_DIR_EAST:
            dirvalue = 'E';
            break;

        case SCEN_DIR_WEST:
            dirvalue = 'W';
            break;

        default:
            if (Scen.Get_Random_Value(0, 99) < 50) {
                dirvalue = 'W';
            } else {
                dirvalue = 'E';
            }
    };

    switch (var) {
        // Get random variant.
        case SCEN_VAR_ANY: {
            int i = 0;
            char filename[256];

            for (i = 0; i < SCEN_VAR_COUNT; ++i) {
                snprintf(filename,
                    sizeof(filename),
                    "sc%c%02d%c%c.ini",
                    tolower(prefixvalue),
                    index,
                    tolower(dirvalue),
                    tolower('A' + i));

                CCFileClass file(filename);

                if (!file.Is_Available()) {
                    break;
                }
            }

            if (i != 0) {
                varvalue = Scen.Get_Random_Value(0, (i - 1)) + 'A';
            } else {
                varvalue = 'X';
            }

            break;
        }

        case SCEN_VAR_A:
            varvalue = 'A';
            break;

        case SCEN_VAR_B:
            varvalue = 'B';
            break;

        case SCEN_VAR_C:
            varvalue = 'C';
            break;

        case SCEN_VAR_D:
            varvalue = 'D';
            break;

        default:
            varvalue = 'L';
            break;
    };

    if (index >= 100) {
        char remainder = (index % '$');
        char index2 = remainder >= 10 ? remainder + '7' : remainder + '0';

        snprintf(ScenarioName,
            sizeof(ScenarioName),
            "sc%c%c%c%c%c.ini",
            tolower(prefixvalue),
            tolower(index / '$' + 'A'),
            tolower(index2),
            tolower(dirvalue),
            tolower(varvalue));
    } else {
        snprintf(ScenarioName,
            sizeof(ScenarioName),
            "sc%c%02d%c%c.ini",
            tolower(prefixvalue),
            index,
            tolower(dirvalue),
            tolower(varvalue));
    }
}

/**
 * @brief Set up a black and white fade.
 *
 * 0x00539BF8
 */
void ScenarioClass::Do_BW_Fade()
{
    FadeOutBW = true;
    FadeInBW = false;
    FadeTimer = 15;
}

/**
 * @brief Process fade state for this frame.
 *
 * 0x00539C40
 */
void ScenarioClass::Do_Fade_AI()
{
    if (FadeInBW) {
        if (FadeTimer <= 0) {
            FadeInBW = false;
        }

        Options.Adjust_Palette(OriginalPalette,
            GamePalette,
            Options.Get_Brightness(),
            Options.Get_Saturation() * fixed((15 - FadeTimer), 15),
            Options.Get_Tint(),
            Options.Get_Contrast());

        GamePalette.Set();
    }

    if (FadeOutBW) {
        if (FadeTimer <= 0) {
            FadeOutBW = false;
        }

        Options.Adjust_Palette(OriginalPalette,
            GamePalette,
            Options.Get_Brightness(),
            Options.Get_Saturation() * fixed(FadeTimer, 15),
            Options.Get_Tint(),
            Options.Get_Contrast());

        GamePalette.Set();

        if (!FadeOutBW) {
            FadeInBW = true;
            FadeTimer = 15;
        }
    }
}

/**
 * @brief Sets a global to a given state.
 *
 * 0x00539EF4
 */
BOOL ScenarioClass::Set_Global_To(int global, BOOL value)
{
    // TODO requires Trigger and TEvent classes.
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*call_Set_Global_To)(int, BOOL) = reinterpret_cast<BOOL (*)(int, BOOL)>(0x00539EF4);
    return call_Set_Global_To(global, value);
#else
    return 0;
#endif
}
