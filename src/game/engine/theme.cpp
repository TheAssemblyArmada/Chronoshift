/**
 * @file
 *
 * @Author CCHyper, OmniBlade
 *
 * @brief Class handling the order and availability of background music tracks.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "theme.h"
#include "audio.h"
#include "ccfileclass.h"
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

#ifndef RAPP_STANDALONE
ThemeClass &Theme = Make_Global<ThemeClass>(0x00668248);
ThemeClass::ThemeControl *ThemeClass::Themes = Make_Pointer<ThemeClass::ThemeControl>(0x006052FC);
#else
ThemeClass Theme;

// BaseName; Name; Scenario; Length; IsNormal; IsRepeat; IsAvailable; Side;
ThemeClass::ThemeControl ThemeClass::Themes[THEME_COUNT] = {
    { "BIGF226M", (TextEnum)435, 0, 307, true, false, true, SIDE_ALLIES },
    { "CRUS226M", (TextEnum)436, 0, 222, true, false, true, SIDE_SOVIET },
    { "FAC1226M", (TextEnum)437, 0, 271, true, false, true, SIDE_ALLIES },
    { "FAC2226M", (TextEnum)438, 0, 328, true, false, true, SIDE_SOVIET },
    { "HELL226M", (TextEnum)439, 0, 375, true, false, true, SIDE_ALLIES },
    { "RUN1226M", (TextEnum)440, 0, 312, true, false, true, SIDE_SOVIET },
    { "SMSH226M", (TextEnum)441, 0, 272, true, false, true, SIDE_ALLIES },
    { "TREN226M", (TextEnum)442, 0, 312, true, false, true, SIDE_SOVIET },
    { "WORK226M", (TextEnum)443, 0, 277, true, false, true, SIDE_ALLIES },
    { "AWAIT", (TextEnum)444, 0, 259, true, false, true, SIDE_ALLIES },
    { "DENSE_R", (TextEnum)445, 0, 294, true, false, true, SIDE_ALLIES },
    { "FOGGER1A", (TextEnum)447, 0, 297, true, false, true, SIDE_ALLIES },
    { "MUD1A", (TextEnum)448, 0, 280, true, false, true, SIDE_ALLIES },
    { "RADIO2", (TextEnum)449, 0, 237, true, false, true, SIDE_ALLIES },
    { "ROLLOUT", (TextEnum)450, 0, 227, true, false, true, SIDE_ALLIES },
    { "SNAKE", (TextEnum)451, 0, 277, true, false, true, SIDE_ALLIES },
    { "TERMINAT", (TextEnum)452, 0, 310, true, false, true, SIDE_ALLIES },
    { "TWIN", (TextEnum)453, 0, 229, true, false, true, SIDE_ALLIES },
    { "VECTOR1A", (TextEnum)454, 0, 252, true, false, true, SIDE_ALLIES },
    // Unselectable themes.
    { "MAP", (TextEnum)446, 0, 63, false, true, true, OWNER_NONE },
    { "SCORE", (TextEnum)478, 0, 106, false, true, true, OWNER_NONE },
    { "INTRO", (TextEnum)475, 0, 205, false, true, true, OWNER_NONE },
    { "CREDITS", (TextEnum)512, 0, 163, false, true, true, OWNER_NONE },
    // Expansion themes. Some soviet missions use SPAIN as a player house (Aftermath and Counterstrike)
    { "2ND_HAND", (TextEnum)526, 0, 268, true, false, true, SIDE_ALLIES },
    { "ARAZOID", (TextEnum)527, 0, 257, true, false, true, SIDE_SOVIET | OWNER_SPAIN },
    { "BACKSTAB", (TextEnum)528, 0, 278, true, false, true, SIDE_ALLIES },
    { "CHAOS2", (TextEnum)529, 0, 250, true, false, true, SIDE_SOVIET | OWNER_SPAIN },
    { "SHUT_IT", (TextEnum)530, 0, 261, true, false, true, SIDE_ALLIES },
    { "TWINMIX1", (TextEnum)531, 0, 222, true, false, true, SIDE_SOVIET | OWNER_SPAIN },
    { "UNDER3", (TextEnum)532, 0, 246, true, false, true, SIDE_ALLIES },
    { "VR2", TXT_THEME_VR2, 0, 255, true, false, true, SIDE_SOVIET | OWNER_SPAIN },
    { "BOG", (TextEnum)546, 0, 212, true, false, true, SIDE_ALLIES },
    { "FLOAT_V2", (TextEnum)547, 0, 274, true, false, true, SIDE_SOVIET | OWNER_SPAIN },
    { "GLOOM", (TextEnum)548, 0, 236, true, false, true, SIDE_ALLIES },
    { "GRNDWIRE", (TextEnum)549, 0, 228, true, false, true, SIDE_SOVIET | OWNER_SPAIN },
    { "RPT", (TextEnum)550, 0, 275, true, false, true, SIDE_ALLIES },
    { "SEARCH", TXT_THEME_SEARCH, 0, 276, true, false, true, SIDE_SOVIET | OWNER_SPAIN },
    { "TRACTION", (TextEnum)552, 0, 237, true, false, true, SIDE_ALLIES },
    { "WASTELND", (TextEnum)553, 0, 242, true, false, true, SIDE_SOVIET | OWNER_SPAIN }
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
        return Themes[theme].BaseName;
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
        return Fetch_String(Themes[theme].Name);
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
    if (!DebugQuiet) {
        if (ScoresPresent && Options.Get_Score_Volume() != 0) {
            if (!Still_Playing()) {
                if (QueuedTheme != THEME_NONE) {
                    if (QueuedTheme == THEME_NEXT) {
                        QueuedTheme = Next_Song(CurrentTheme);
                        DEBUG_LOG("ThemeClass::AI(Next song = %d : %s)\n", QueuedTheme, Base_Name(QueuedTheme));
                    }

                    Play_Song(QueuedTheme);
                    QueuedTheme = THEME_NEXT;
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
        || theme != THEME_STOP && !Themes[theme].IsRepeat && !Options.Get_Repeat()) {
        // If we are shuffling the score, select a random score. If its the same as the one we were passed or isn't allowed,
        // roll for another.
        if (Options.Get_Shuffle()) {
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

    if (ScoresPresent && !DebugQuiet && Options.Get_Score_Volume() != 0) {
        if (QueuedTheme == THEME_NONE || QueuedTheme == THEME_NEXT || theme == THEME_NONE || theme == THEME_STOP) {
            QueuedTheme = theme;

            DEBUG_LOG("ThemeClass::Queue_Song(%d : %s)\n", QueuedTheme, Base_Name(QueuedTheme));

            if (Still_Playing()) {
                DEBUG_LOG("ThemeClass::Queue_Song() - Fading out %s\n", Base_Name(CurrentTheme));
                Fade_Sample(ThemeHandle, 60);
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

    if (ScoresPresent && !DebugQuiet && Options.Get_Score_Volume() != 0) {
        Stop();
        CurrentTheme = theme;

        if (theme != THEME_NONE && theme != THEME_STOP) {
            StreamLowImpact = true;
            ThemeHandle = File_Stream_Sample_Vol(Theme_File_Name(theme), 256, true);
            StreamLowImpact = false;
        }
    }

    return ThemeHandle;
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
        const char *base_name = Themes[theme].BaseName;

        // From C&C/Sole, allow remix versions of tracks if available.
        if (Special.Allow_Remixes()) {
            snprintf(_name, sizeof(_name), "%s.var", base_name);
            CCFileClass varfile(_name);

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
        return Themes[theme].Length;
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
    if (ScoresPresent && !DebugQuiet && ThemeHandle != -1) {
        Stop_Sample(ThemeHandle);
        ThemeHandle = -1;
        CurrentTheme = THEME_NONE;
        QueuedTheme = THEME_NONE;
    }
}

/**
 * @brief Suspends the background music.
 *
 * 0x0056C1C0
 */
void ThemeClass::Suspend()
{
    if (ScoresPresent && !DebugQuiet && ThemeHandle != -1) {
        Stop_Sample(ThemeHandle);
        ThemeHandle = -1;
        QueuedTheme = CurrentTheme;
        CurrentTheme = THEME_NONE;
    }
}

/**
 * @brief Checks if the current song is still playing.
 *
 * 0x0056C208
 */
BOOL ThemeClass::Still_Playing() const
{
    if (ScoresPresent && ThemeHandle != -1 && !DebugQuiet) {
        return Sample_Status(ThemeHandle);
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
#ifndef RAPP_STANDALONE
    BOOL (*call_Is_Allowed)
    (const ThemeClass *, ThemeType) = reinterpret_cast<BOOL (*)(const ThemeClass *, ThemeType)>(0x0056C240);
    return call_Is_Allowed(this, theme);
#elif 0
    if (theme >= THEME_COUNT) {
        return true;
    }
    // Is this theme allowed and available (is it as normal theme) to be played in the ingame? If either of these are false,
    // then return false.
    if (!Themes[theme].IsAvailable || !Themes[theme].IsNormal) {
        return false;
    }

    // Is the player part of the allowed side or is a allowed owner for this theme?
    if (PlayerPtr != nullptr) {
        if (!(Themes[theme].Side & (1 << PlayerPtr->Side))) {
            return false;
        }
    }

    if (Session.Game_To_Play() == GAME_CAMPAIGN || Scen.ScenarioIndex >= Themes[theme].Scenario) {
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
            if (strstr(Fetch_String(Themes[theme].Name), name) != nullptr) {
                return theme;
            }
        }
    }

    return THEME_NONE;
}

/**
 * @brief Scans to check which songs are actually available on disk.
 *
 * 0x0056C3D8
 */
void ThemeClass::Scan()
{
    for (ThemeType theme = THEME_FIRST; theme < THEME_COUNT; ++theme) {
        CCFileClass file(Theme_File_Name(theme));
        Themes[theme].IsAvailable = file.Is_Available();
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
        Themes[theme].IsNormal = true;
        Themes[theme].Scenario = scenario;
        Themes[theme].Side = side;
    }
}
