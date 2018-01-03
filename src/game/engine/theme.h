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
#pragma once

#ifndef THEME_H
#define THEME_H

#include "always.h"
#include "audio.h"
#include "language.h"
#include "random.h"
#include "vector.h"

enum ThemeType
{
    THEME_SMTHNG3 = -3, // Think these are play modes.
    THEME_NEXT = -2,
    THEME_NONE = -1,
    THEME_BIG_FOOT = 0, // Themes available in playlist
    THEME_CRUSH = 1,
    THEME_FAC1 = 2,
    THEME_FAC2 = 3,
    THEME_HELL_MARCH = 4,
    THEME_RUN1 = 5,
    THEME_SMSH = 6,
    THEME_TREN = 7,
    THEME_WORK = 8,
    THEME_AWAIT = 9,
    THEME_DENSE_R = 10,
    THEME_FOGGER1A = 11,
    THEME_MUD1A = 12,
    THEME_RADIO2 = 13,
    THEME_ROLLOUT = 14,
    THEME_SNAKE = 15,
    THEME_TERMINAT = 16,
    THEME_TWIN = 17,
    THEME_VECTOR1A = 18,
    THEME_MAP = 19, // Not available in playlist
    THEME_SCORE = 20,
    THEME_INTRO = 21,
    THEME_CREDITS = 22,
    THEME_2ND_HAND = 23, // Expansion themes
    THEME_ARAZOID = 24,
    THEME_BACKSTAB = 25,
    THEME_CHAOS2 = 26,
    THEME_SHUT_IT = 27,
    THEME_TWIN_MIX = 28,
    THEME_UNDER3 = 29,
    THEME_VR2 = 30,
    THEME_BOG = 31,
    THEME_FLOAT_V2 = 32,
    THEME_GLOOM = 33,
    THEME_GRNDWIRE = 34,
    THEME_RPT = 35,
    THEME_SEARCH = 36,
    THEME_TRACTION = 37,
    THEME_WASTELAND = 38,
    THEME_COUNT
};

DEFINE_ENUMERATION_OPERATORS(ThemeType);

class ThemeClass
{
private:
    struct ThemeControl
    {
        const char *BaseName; // Internal base name for the theme.
        int Name; // Display name of the theme (TextEnum value).
        int Scenario; // The scenario when the theme becomes available.
        int Length; // length of the theme (in minutes).
        BOOL IsNormal; // Is this theme available through the in-game theme play list [def = true]?
        BOOL IsRepeat; // Does this track repeat.
        BOOL IsAvailable; // Is this theme available (scanned and found) [def = false]?
        int Side; // Which side [or sides] get to hear this theme. (see 'gametypes.h' for all options)
    };

public:
    ThemeClass() : ThemeHandle(-1), CurrentTheme(THEME_NONE), QueuedTheme(THEME_NONE) {}
    ~ThemeClass();
    void AI();
    ThemeType Next_Song(ThemeType theme) const;
    void Queue_Song(ThemeType theme);
    int Play_Song(ThemeType theme);
    int Track_Length(ThemeType theme) const;
    void Stop();
    void Suspend();
    BOOL Still_Playing() const;
    BOOL Is_Allowed(ThemeType theme) const;
    const char *Base_Name(ThemeType theme) const;
    const char *Full_Name(ThemeType theme) const;
    ThemeType From_Name(const char *name) const;
    void Set_Theme_Data(ThemeType theme, int scenario, int side);
    void Fade_Out() { Queue_Song(THEME_NONE); }
    ThemeType Get_Random_Theme() const { return ThemeType(g_nonCriticalRandom(THEME_BIG_FOOT, THEME_WASTELAND)); }
    ThemeType What_Is_Playing() { return CurrentTheme; }
    ThemeType What_Is_Queued() { return QueuedTheme; }
    int Max_Themes() const { return THEME_COUNT; }
    static void Scan();
    static const char *Theme_File_Name(ThemeType theme);

private:
    int ThemeHandle;
    ThemeType CurrentTheme;
    ThemeType QueuedTheme;
#ifndef RAPP_STANDALONE
    static ThemeControl *Themes;
#else
    static ThemeControl Themes[];
#endif
};

#ifndef RAPP_STANDALONE
#include "hooker.h"

extern ThemeClass &Theme;
#else
extern ThemeClass Theme;
#endif

#endif // THEME_H
