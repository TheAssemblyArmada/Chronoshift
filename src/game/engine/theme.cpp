/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class handling the order and availability of background music tracks.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "theme.h"
#include "audio.h"
#include "gamefile.h"
#include "gamedebug.h"
#include "gameoptions.h"
#include "gametypes.h"
#include "globals.h"
#include "language.h"
//#include "scenario.h"
#include "session.h"
#include "special.h"
#include <cstdio>

using std::snprintf;

#ifndef GAME_DLL
ThemeClass g_Theme;

// BaseName; Name; Scenario; Length; IsNormal; IsRepeat; IsAvailable; Side;
ThemeClass::ThemeControl ThemeClass::s_Themes[THEME_COUNT] = {
    { "BIGF226M", TXT_THEME_BIGFOOT, 0, 307, true, false, true, SIDE_ALLIES },
    { "CRUS226M", TXT_THEME_CRUSH, 0, 222, true, false, true, SIDE_SOVIET },
    { "FAC1226M", TXT_THEME_FACE_ENEMY_1, 0, 271, true, false, true, SIDE_ALLIES },
    { "FAC2226M", TXT_THEME_FACE_ENEMY_2, 0, 328, true, false, true, SIDE_SOVIET },
    { "HELL226M", TXT_THEME_HELL_MARCH, 0, 375, true, false, true, SIDE_ALLIES },
    { "RUN1226M", TXT_THEME_RUN_LIFE, 0, 312, true, false, true, SIDE_SOVIET },
    { "SMSH226M", TXT_THEME_SMASH, 0, 272, true, false, true, SIDE_ALLIES },
    { "TREN226M", TXT_THEME_TRENCHES, 0, 312, true, false, true, SIDE_SOVIET },
    { "WORK226M", TXT_THEME_WORKMEN, 0, 277, true, false, true, SIDE_ALLIES },
    { "AWAIT", TXT_THEME_AWAIT, 0, 259, true, false, true, SIDE_ALLIES },
    { "DENSE_R", TXT_THEME_DENSE, 0, 294, true, false, true, SIDE_ALLIES },
    { "FOGGER1A", TXT_THEME_FOGGER, 0, 297, true, false, true, SIDE_ALLIES },
    { "MUD1A", TXT_THEME_MUD, 0, 280, true, false, true, SIDE_ALLIES },
    { "RADIO2", TXT_THEME_RADIO2, 0, 237, true, false, true, SIDE_ALLIES },
    { "ROLLOUT", TXT_THEME_ROLL_OUT, 0, 227, true, false, true, SIDE_ALLIES },
    { "SNAKE", TXT_THEME_SNAKE, 0, 277, true, false, true, SIDE_ALLIES },
    { "TERMINAT", TXT_THEME_TERMINATE, 0, 310, true, false, true, SIDE_ALLIES },
    { "TWIN", TXT_THEME_TWIN, 0, 229, true, false, true, SIDE_ALLIES },
    { "VECTOR1A", TXT_THEME_VECTOR, 0, 252, true, false, true, SIDE_ALLIES },

    // Unselectable themes.
    { "MAP", TXT_THEME_MAP_SEL, 0, 63, false, true, true, OWNER_NONE },
    { "SCORE", TXT_THEME_SCORE, 0, 106, false, true, true, OWNER_NONE },
    { "INTRO", TXT_THEME_MENU, 0, 205, false, true, true, OWNER_NONE },
    { "CREDITS", TXT_THEME_CREDITS, 0, 163, false, true, true, OWNER_NONE },

    // Expansion themes. Some Soviet missions use SPAIN as a player house (see Aftermath and Counterstrike).
    { "2ND_HAND", TXT_THEME_2NDHAND, 0, 268, true, false, true, SIDE_ALLIES },
    { "ARAZOID", TXT_THEME_ARAZOID, 0, 257, true, false, true, SIDE_SOVIET | OWNER_SPAIN },
    { "BACKSTAB", TXT_THEME_BACKSTAB, 0, 278, true, false, true, SIDE_ALLIES },
    { "CHAOS2", TXT_THEME_CHAOS2, 0, 250, true, false, true, SIDE_SOVIET | OWNER_SPAIN },
    { "SHUT_IT", TXT_THEME_SHUT_IT, 0, 261, true, false, true, SIDE_ALLIES },
    { "TWINMIX1", TXT_THEME_TWINMIX1, 0, 222, true, false, true, SIDE_SOVIET | OWNER_SPAIN },
    { "UNDER3", TXT_THEME_UNDER3, 0, 246, true, false, true, SIDE_ALLIES },
    { "VR2", TXT_THEME_VR2, 0, 255, true, false, true, SIDE_SOVIET | OWNER_SPAIN },
    { "BOG", TXT_THEME_BOG, 0, 212, true, false, true, SIDE_ALLIES },
    { "FLOAT_V2", TXT_THEME_FLOATING, 0, 274, true, false, true, SIDE_SOVIET | OWNER_SPAIN },
    { "GLOOM", TXT_THEME_GLOOM, 0, 236, true, false, true, SIDE_ALLIES },
    { "GRNDWIRE", TXT_THEME_GROUNDWIRE, 0, 228, true, false, true, SIDE_SOVIET | OWNER_SPAIN },
    { "RPT", TXT_THEME_MECHMAN2, 0, 275, true, false, true, SIDE_ALLIES },
    { "SEARCH", TXT_THEME_SEARCH, 0, 276, true, false, true, SIDE_SOVIET | OWNER_SPAIN },
    { "TRACTION", TXT_THEME_TRACTION, 0, 237, true, false, true, SIDE_ALLIES },
    { "WASTELND", TXT_THEME_WASTELAND, 0, 242, true, false, true, SIDE_SOVIET | OWNER_SPAIN }
};
#endif

ThemeClass::~ThemeClass()
{
    Stop();
}

/**
 * @brief Gets the name of the song without an extension.
 *
 * 0x0056BE60
 */
const char *ThemeClass::Base_Name(ThemeType theme) const
{
    if (theme > THEME_NONE) {
        return s_Themes[theme].BaseName;
    }

    return "No theme";
}

/**
 * @brief Gets the title of the song from the string table.
 *
 * 0x0056BE9C
 */
const char *ThemeClass::Full_Name(ThemeType theme) const
{
    if (theme > THEME_NONE && theme < THEME_COUNT) {
        return Text_String(s_Themes[theme].Name);
    }

    return nullptr;
}

/**
 * @brief Performs per tick processing on the object.
 *
 * 0x0056BF00
 */
void ThemeClass::AI()
{
    if (!g_DebugQuiet) {
        if (g_ScoresPresent && g_Options.Get_Score_Volume() != 0) {
            if (!Still_Playing()) {
                if (m_QueuedTheme != THEME_NONE) {
                    if (m_QueuedTheme == THEME_NEXT) {
                        m_QueuedTheme = Next_Song(m_CurrentTheme);
                        DEBUG_LOG("ThemeClass::AI(Next song = %d : %s)\n", m_QueuedTheme, Base_Name(m_QueuedTheme));
                    }

                    Play_Song(m_QueuedTheme);
                    m_QueuedTheme = THEME_NEXT;
                }
            }
        }

        Sound_Callback();
    }
}

/**
 * @brief Checks which is the next song to play.
 *
 * 0x0056BF80
 */
ThemeType ThemeClass::Next_Song(ThemeType theme) const
{
    DEBUG_ASSERT(theme < THEME_COUNT);

    if (theme == THEME_NONE || theme == THEME_NEXT
        || (theme != THEME_STOP && !s_Themes[theme].IsRepeat && !g_Options.Get_Repeat())) {
        // If we are shuffling the score, select a random score. If its the same as the one we were passed or isn't allowed,
        // roll for another.
        if (g_Options.Get_Shuffle()) {
            ThemeType next;

            do {
                do {
                    next = Get_Random_Theme();
                } while (next == theme);
            } while (!Is_Allowed(next));

            theme = next;
        } else {
            // Check if the next theme is allowed and wrap theme back to first if theme has gone over the max.
            while (theme < THEME_COUNT) {
                ++theme;

                if (theme == THEME_COUNT) {
                    theme = THEME_BIG_FOOT;
                }

                if (Is_Allowed(theme)) {
                    break;
                }
            }
        }
    }

    return theme;
}

/**
 * @brief Queues the next song to play.
 *
 * 0x0056C020
 */
void ThemeClass::Queue_Song(ThemeType theme)
{
    DEBUG_ASSERT(theme < THEME_COUNT);

    if (g_ScoresPresent && !g_DebugQuiet && g_Options.Get_Score_Volume() != 0) {
        if (m_QueuedTheme == THEME_NONE || m_QueuedTheme == THEME_NEXT || theme == THEME_NONE || theme == THEME_STOP) {
            m_QueuedTheme = theme;

            DEBUG_LOG("ThemeClass::Queue_Song(%d : %s)\n", m_QueuedTheme, Base_Name(m_QueuedTheme));

            if (Still_Playing()) {
                DEBUG_LOG("ThemeClass::Queue_Song() - Fading out %s\n", Base_Name(m_CurrentTheme));
                Fade_Sample(m_ThemeHandle, 60);
            }
        }
    }
}

/**
 * @brief Sets the playing song.
 *
 * 0x0056C090
 */
int ThemeClass::Play_Song(ThemeType theme)
{
    DEBUG_ASSERT(theme < THEME_COUNT);

    if (g_ScoresPresent && !g_DebugQuiet && g_Options.Get_Score_Volume() != 0) {
        Stop();
        m_CurrentTheme = theme;

        if (theme != THEME_NONE && theme != THEME_STOP) {
            g_StreamLowImpact = true;
            m_ThemeHandle = File_Stream_Sample_Vol(Theme_File_Name(theme), 256, true);
            g_StreamLowImpact = false;
        }
    }

    return m_ThemeHandle;
}

/**
 * @brief Gets the filename of a song including extension.
 *
 * 0x0056C110
 */
const char *ThemeClass::Theme_File_Name(ThemeType theme)
{
    static char _name[512];

    if (theme > THEME_NONE && theme < THEME_COUNT) {
        const char *base_name = s_Themes[theme].BaseName;

        // From C&C/Sole, allow remix versions of tracks if available.
        if (s_Special.Allow_Remixes()) {
            snprintf(_name, sizeof(_name), "%s.var", base_name);
            GameFileClass varfile(_name);

            if (varfile.Is_Available()) {
                return _name;
            }
        }

        snprintf(_name, sizeof(_name), "%s.aud", base_name);

        return _name;
    }

    return "";
}

/**
 * @brief Gets the length of a song.
 *
 * 0x0056C154
 */
int ThemeClass::Track_Length(ThemeType theme) const
{
    if (theme > THEME_NONE && theme < THEME_COUNT) {
        return s_Themes[theme].Length;
    }

    return 0;
}

/**
 * @brief Stops the background music.
 *
 * 0x0056C178
 */
void ThemeClass::Stop()
{
    if (g_ScoresPresent && !g_DebugQuiet && m_ThemeHandle != -1) {
        Stop_Sample(m_ThemeHandle);
        m_ThemeHandle = -1;
        m_CurrentTheme = THEME_NONE;
        m_QueuedTheme = THEME_NONE;
    }
}

/**
 * @brief Suspends the background music.
 *
 * 0x0056C1C0
 */
void ThemeClass::Suspend()
{
    if (g_ScoresPresent && !g_DebugQuiet && m_ThemeHandle != -1) {
        Stop_Sample(m_ThemeHandle);
        m_ThemeHandle = -1;
        m_QueuedTheme = m_CurrentTheme;
        m_CurrentTheme = THEME_NONE;
    }
}

/**
 * @brief Checks if the current song is still playing.
 *
 * 0x0056C208
 */
BOOL ThemeClass::Still_Playing() const
{
    if (g_ScoresPresent && m_ThemeHandle != -1 && !g_DebugQuiet) {
        return Sample_Status(m_ThemeHandle);
    }

    return false;
}

/**
 * @brief Checks if a song is allowed to be played based on the song, the player side, the game mode and the map.
 *
 * 0x0056C240
 */
BOOL ThemeClass::Is_Allowed(ThemeType theme) const
{
    // TODO Requires HouseClass and ScenarioClass
#ifdef GAME_DLL
    BOOL (*call_Is_Allowed)
    (const ThemeClass *, ThemeType) = reinterpret_cast<BOOL (*)(const ThemeClass *, ThemeType)>(0x0056C240);
    return call_Is_Allowed(this, theme);
#elif 0
    if (theme >= THEME_COUNT) {
        return true;
    }
    // Is this theme allowed and available (is it as normal theme) to be played in the ingame? If either of these are false,
    // then return false.
    if (!s_Themes[theme].IsAvailable || !s_Themes[theme].IsNormal) {
        return false;
    }

    // Is the player part of the allowed side or is a allowed owner for this theme?
    if (PlayerPtr != nullptr) {
        if (!(s_Themes[theme].Side & (1 << PlayerPtr->Side))) {
            return false;
        }
    }

    if (g_Session.Game_To_Play() == GAME_CAMPAIGN || g_Scen.m_ScenarioIndex >= s_Themes[theme].Scenario) {
        return true;
    }

    return false;
#else
    return false;
#endif
}

/**
 * @brief Gets the theme type from the song name.
 *
 * 0x0056C2E0
 */
ThemeType ThemeClass::From_Name(const char *name) const
{
    DEBUG_ASSERT(name != nullptr);

    if (name && strlen(name) > 0) {
        for (ThemeType theme = THEME_FIRST; theme < THEME_COUNT; ++theme) {
            if (strcasecmp(Base_Name(theme), name) == 0) {
                return theme;
            }
        }

        for (ThemeType theme = THEME_FIRST; theme < THEME_COUNT; ++theme) {
            if (strstr(Text_String(s_Themes[theme].Name), name) != nullptr) {
                return theme;
            }
        }
    }

    return THEME_NONE;
}

const char *ThemeClass::Name_From(ThemeType theme)
{
    if (theme >= THEME_FIRST && theme < THEME_COUNT) {
        return Text_String(s_Themes[theme].Name);
    }

    return "None";
}

/**
 * @brief Scans to check which songs are actually available on disk.
 *
 * 0x0056C3D8
 */
void ThemeClass::Scan()
{
    for (ThemeType theme = THEME_FIRST; theme < THEME_COUNT; ++theme) {
        GameFileClass file(Theme_File_Name(theme));
        s_Themes[theme].IsAvailable = file.Is_Available();
    }
}

/**
 * @brief Sets a song to be available to select and sets the side and scenario it will be selectable for.
 *
 * 0x0056C448
 */
void ThemeClass::Set_Theme_Data(ThemeType theme, int scenario, int side)
{
    if (theme > THEME_NONE && theme < THEME_COUNT) {
        s_Themes[theme].IsNormal = true;
        s_Themes[theme].Scenario = scenario;
        s_Themes[theme].Side = side;
    }
}

void ThemeClass::Play_Prev()
{
    ThemeType theme = m_CurrentTheme;
    while (theme >= THEME_FIRST) {
        --theme;

        if (theme < THEME_FIRST) {
            theme = THEME_LAST;
        }

        if (Is_Allowed(theme)) {
            break;
        }
    }
    Queue_Song(theme);
}

void ThemeClass::Play_Next()
{
    ThemeType theme = m_CurrentTheme;
    while (theme < THEME_COUNT) {
        ++theme;

        if (theme == THEME_COUNT) {
            theme = THEME_FIRST;
        }

        if (Is_Allowed(theme)) {
            break;
        }
    }
    Queue_Song(theme);
}
