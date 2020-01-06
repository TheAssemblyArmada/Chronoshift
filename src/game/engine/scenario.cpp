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
#include "scenario.h"
#include "gamefile.h"
#include "gameoptions.h"
#include "palette.h"
#include "stringex.h"
#include <cstdio>

using std::snprintf;

#ifndef GAME_DLL
ScenarioClass g_Scen;
#endif

ScenarioClass::ScenarioClass() :
    m_SyncRandom(),
    m_HumanDifficulty(DIFF_NONE),
    m_AIDifficulty(DIFF_NONE),
    m_ElapsedTimer(),
    m_GlobalTimer(),
    m_ScenarioIndex(-1),
    m_Theater(THEATER_NONE),
    m_IntroMovie(MOVIE_NONE),
    m_BriefMovie(MOVIE_NONE),
    m_WinMovie(MOVIE_NONE),
    m_LoseMovie(MOVIE_NONE),
    m_ActionMovie(MOVIE_NONE),
    m_TransitTheme(THEME_NONE),
    m_SomeHouse(HOUSES_NONE),
    m_CarryOverMoney(0),
    m_CarryOverMoneyAmount(0),
    m_CarryOverCap(0),
    m_CarryOverPercent(0),
    m_BridgeCount(0),
    m_CarryOverTime(0),
    m_DestroyBridges(false),
    m_GlobalsChanged(false),
    m_ToCarryOver(false),
    m_ToInherit(false),
    m_CivEvac(false),
    m_FadeOutBW(false),
    m_FadeInBW(false),
    m_EndOfGame(false),
    m_TimerInherit(false),
    m_NoSpyPlane(false),
    m_SkipScore(false),
    m_OneTimeOnly(false),
    m_SkipMapSelect(false),
    m_TruckCrate(false),
    m_FillSilos(false),
    m_FadeTimer(),
    m_AutoSonarTimer()
{
    for (int index = 0; index < ARRAY_SIZE(m_Waypoints); ++index) {
        m_Waypoints[index] = -1;
    }

    m_ScenarioName[0] = '\0';
    m_ScenarioDescription[0] = '\0';
    m_BriefingText[0] = '\0';
    memset(m_GlobalVariables, 0, sizeof(m_GlobalVariables));
    memset(m_Views, 0, sizeof(m_Views));
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

    strlcpy(m_ScenarioName, scen_name, sizeof(m_ScenarioName));
    DEBUG_LOG("Set_Scenario_Name() - ScenarioName set to '%s'\n", m_ScenarioName);

    // Get the number part of the scenario name and calculate the ScenarioIndex.
    memcpy(buffer, &m_ScenarioName[3], sizeof(buffer));
    buffer[2] = '\0';

    if (buffer[0] <= '9' && buffer[1] <= '9') {
        m_ScenarioIndex = atoi(buffer);
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

        m_ScenarioIndex = digit2 + 36 * digit1;
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

    m_ScenarioIndex = index;

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
            if (g_Scen.Check_Random_Chance(50)) {
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

                GameFileClass file(filename);

                if (!file.Is_Available()) {
                    break;
                }
            }

            if (i != 0) {
                varvalue = g_Scen.Get_Random_Value(0, (i - 1)) + 'A';
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

        snprintf(m_ScenarioName,
            sizeof(m_ScenarioName),
            "sc%c%c%c%c%c.ini",
            tolower(prefixvalue),
            tolower(index / '$' + 'A'),
            tolower(index2),
            tolower(dirvalue),
            tolower(varvalue));
    } else {
        snprintf(m_ScenarioName,
            sizeof(m_ScenarioName),
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
    m_FadeOutBW = true;
    m_FadeInBW = false;
    m_FadeTimer = 15;
}

/**
 * @brief Process fade state for this frame.
 *
 * 0x00539C40
 */
void ScenarioClass::Do_Fade_AI()
{
    if (m_FadeInBW) {
        if (m_FadeTimer <= 0) {
            m_FadeInBW = false;
        }

        g_Options.Adjust_Palette(g_OriginalPalette,
            g_GamePalette,
            g_Options.Get_Brightness(),
            g_Options.Get_Saturation() * fixed_t((15 - m_FadeTimer), 15),
            g_Options.Get_Tint(),
            g_Options.Get_Contrast());

        g_GamePalette.Set();
    }

    if (m_FadeOutBW) {
        if (m_FadeTimer <= 0) {
            m_FadeOutBW = false;
        }

        g_Options.Adjust_Palette(g_OriginalPalette,
            g_GamePalette,
            g_Options.Get_Brightness(),
            g_Options.Get_Saturation() * fixed_t(m_FadeTimer, 15),
            g_Options.Get_Tint(),
            g_Options.Get_Contrast());

        g_GamePalette.Set();

        if (!m_FadeOutBW) {
            m_FadeInBW = true;
            m_FadeTimer = 15;
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
#ifdef GAME_DLL
    BOOL (*func)(int, BOOL) = reinterpret_cast<BOOL (*)(int, BOOL)>(0x00539EF4);
    return func(global, value);
#else
    return 0;
#endif
}

BOOL Start_Scenario(const char *filename, BOOL play_movies)
{
#ifdef GAME_DLL
    BOOL (*func)(const char *, BOOL) = reinterpret_cast<BOOL (*)(const char *, BOOL)>(0x0053A0A4);
    return func(filename, play_movies);
#else
    return false;
#endif
}

BOOL Read_Scenario(const char *filename)
{
#ifdef GAME_DLL
    BOOL (*func)(const char *) = reinterpret_cast<BOOL (*)(const char *)>(0x0053A390);
    return func(filename);
#else
    return false;
#endif
}
