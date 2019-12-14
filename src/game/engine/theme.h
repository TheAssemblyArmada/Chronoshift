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
#pragma once

#ifndef THEME_H
#define THEME_H

#include "always.h"
#include "audio.h"
#include "language.h"
#include "random.h"

enum ThemeType
{
    THEME_BIG_FOOT,
    THEME_CRUSH,
    THEME_FAC1,
    THEME_FAC2,
    THEME_HELL_MARCH,
    THEME_RUN1,
    THEME_SMSH,
    THEME_TREN,
    THEME_WORK,
    THEME_AWAITING,
    THEME_DENSE_R,
    THEME_FOGGER,
    THEME_MUD,
    THEME_RADIO2,
    THEME_ROLLOUT,
    THEME_SNAKE,
    THEME_TERMINAT,
    THEME_TWIN,
    THEME_VECTOR,
    THEME_MAP,
    THEME_SCORE,
    THEME_INTRO,
    THEME_CREDITS,
    THEME_2ND_HAND,
    THEME_ARAZOID,
    THEME_BACKSTAB,
    THEME_CHAOS2,
    THEME_SHUT_IT,
    THEME_TWIN_MIX,
    THEME_UNDER3,
    THEME_VR2,
    THEME_BOG,
    THEME_FLOAT_V2,
    THEME_GLOOM,
    THEME_GRNDWIRE,
    THEME_RPT,
    THEME_SEARCH,
    THEME_TRACTION,
    THEME_WASTELAND,

    THEME_COUNT,

    THEME_NONE = -1,
    THEME_NEXT = -2,
    THEME_STOP = -3,

    THEME_FIRST = THEME_BIG_FOOT,
    THEME_LAST = (THEME_COUNT - 1)
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
        BOOL IsAvailable; // Is this theme available (scanned and found)?
        int Side; // Which side [or sides] get to hear this theme. (see OwnerType for all options)
    };

public:
    ThemeClass() : m_ThemeHandle(-1), m_CurrentTheme(THEME_NONE), m_QueuedTheme(THEME_NONE) {}
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
    const char *Name_From(ThemeType theme);

    void Set_Theme_Data(ThemeType theme, int scenario, int side);

    void Fade_Out() { Queue_Song(THEME_NONE); }
    void Stop_Playing_Song() { Queue_Song(THEME_STOP); }
    void Play_Next_Song() { Queue_Song(THEME_NEXT); }

    void Play_Prev();
    void Play_Next();

    ThemeType Get_Random_Theme() const { return ThemeType(g_NonCriticalRandom(THEME_FIRST, THEME_LAST)); }
    ThemeType What_Is_Playing() { return m_CurrentTheme; }
    ThemeType What_Is_Queued() { return m_QueuedTheme; }

    int Max_Themes() const { return THEME_COUNT; }

    static void Scan();
    static const char *Theme_File_Name(ThemeType theme);

private:
    int m_ThemeHandle;
    ThemeType m_CurrentTheme;
    ThemeType m_QueuedTheme;
#ifdef GAME_DLL
    static ThemeControl *s_Themes;
#else
    static ThemeControl s_Themes[];
#endif
};

#ifdef GAME_DLL
extern ThemeClass &g_Theme;
#else
extern ThemeClass g_Theme;
#endif

#endif // THEME_H
