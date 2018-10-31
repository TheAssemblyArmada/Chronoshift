/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Global variables that are used amongst many classes.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef OPTIONS_H
#define OPTIONS_H

#include "always.h"
#include "fixed.h"
#include "keyboard.h"
#include "palette.h"

enum GameSpeedEnum
{
    GAMESPEED_0 = 0,
    GAMESPEED_1 = 1,
    GAMESPEED_2 = 2,
    GAMESPEED_3 = 3,
    GAMESPEED_4 = 4,
    GAMESPEED_5 = 5,
    GAMESPEED_6 = 6,
    GAMESPEED_7 = 7,
};

enum ScrollSpeedEnum
{
    SCROLLSPEED_0 = 0,
    SCROLLSPEED_1 = 1,
    SCROLLSPEED_2 = 2,
    SCROLLSPEED_3 = 3,
    SCROLLSPEED_4 = 4,
    SCROLLSPEED_5 = 5,
};

class OptionsClass
{
public:
    OptionsClass();

    void One_Time();
    void Process() {}
    void Save_Settings();
    void Load_Settings();
    void Adjust_Palette(
        PaletteClass &src, PaletteClass &dst, fixed brightness, fixed saturation, fixed tint, fixed contrast) const;
    void Fixup_Palette() const;
    int Normalize_Volume(int volume) const { return volume * Volume; }
    int Normalize_Delay(int delay) const;
    void Set_Shuffle(BOOL shuffle) { ScoreShuffles = shuffle; }
    void Set_Repeat(BOOL repeat) { ScoreRepeats = repeat; }
    void Set_Score_Volume(fixed volume, BOOL beep = false);
    void Set_Sound_Volume(fixed volume, BOOL beep = false);
    void Set_Brightness(fixed brightness);
    void Set_Saturation(fixed saturation);
    void Set_Contrast(fixed contrast);
    void Set_Tint(fixed tint);
    void Set();
    int Get_Scroll_Rate() const { return ScrollRate; }
    const fixed &Get_Score_Volume() const { return ScoreVolume; }
    BOOL Get_Auto_Scroll() const { return AutoScroll; }
    BOOL Get_Repeat() const { return ScoreRepeats; }
    BOOL Get_Shuffle() const { return ScoreShuffles; }
    BOOL Sidebar_Toggle_Allowed() const { return AllowSidebarToggle; }
    BOOL Is_Counterstrike_Enabled() const { return CounterstrikeEnabled; }
    BOOL Is_Aftermath_Enabled() const { return AftermathEnabled; }
    fixed Get_Brightness() const { return (Brightness - fixed::_1_4) / fixed::_1_2; }
    fixed Get_Saturation() const { return Saturation; }
    fixed Get_Contrast() const { return (Contrast - fixed::_1_4) / fixed::_1_2; }
    fixed Get_Tint() const { return Tint; }

#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
#endif

private:
    int GameSpeed;
    int ScrollRate;
    fixed Volume;
    fixed ScoreVolume;
    fixed MultiplayerScoreVolume;
    fixed Brightness;
    fixed Tint;
    fixed Saturation;
    fixed Contrast;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool AutoScroll : 1; // & 1
            bool ScoreRepeats : 1; // & 2
            bool ScoreShuffles : 1; // & 4
            bool PaletteScroll : 1; // & 8
            bool AllowSidebarToggle : 1; // & 16 Chronoshift option.
            bool CounterstrikeEnabled : 1; // & 32 Chronoshift option.
            bool AftermathEnabled : 1; // & 64 Chronoshift option.
        };
        int Bitfield;
    };
#else
    bool AutoScroll;
    bool ScoreRepeats;
    bool ScoreShuffles;
    bool PaletteScroll;
    bool AllowSidebarToggle; // Chronoshift option.
    bool CounterstrikeEnabled; // Chronoshift option.
    bool AftermathEnabled; // Chronoshift option.
#endif
    KeyNumType KeyForceMove1;
    KeyNumType KeyForceMove2;
    KeyNumType KeyForceAttack1;
    KeyNumType KeyForceAttack2;
    KeyNumType KeySelect1;
    KeyNumType KeySelect2;
    KeyNumType KeyScatter;
    KeyNumType KeyStop;
    KeyNumType KeyGuard;
    KeyNumType KeyNext;
    KeyNumType KeyPrevious;
    KeyNumType KeyFormation;
    KeyNumType KeyHome1;
    KeyNumType KeyHome2;
    KeyNumType KeyBase;
    KeyNumType KeyResign;
    KeyNumType KeyAlliance;
    KeyNumType KeyBookmark1;
    KeyNumType KeyBookmark2;
    KeyNumType KeyBookmark3;
    KeyNumType KeyBookmark4;
    KeyNumType KeySelectView;
    KeyNumType KeyRepairToggle;
    KeyNumType KeyRepairOn;
    KeyNumType KeyRepairOff;
    KeyNumType KeySellToggle;
    KeyNumType KeySellOn;
    KeyNumType KeySellOff;
    KeyNumType KeyMapToggle;
    KeyNumType KeySidebarUp;
    KeyNumType KeySidebarDown;
    KeyNumType KeyOption1;
    KeyNumType KeyOption2;
    KeyNumType KeyScrollLeft;
    KeyNumType KeyScrollRight;
    KeyNumType KeyScrollUp;
    KeyNumType KeyScrollDown;
    KeyNumType KeyQueueMove1;
    KeyNumType KeyQueueMove2;
    KeyNumType KeyTeam1;
    KeyNumType KeyTeam2;
    KeyNumType KeyTeam3;
    KeyNumType KeyTeam4;
    KeyNumType KeyTeam5;
    KeyNumType KeyTeam6;
    KeyNumType KeyTeam7;
    KeyNumType KeyTeam8;
    KeyNumType KeyTeam9;
    KeyNumType KeyTeam10;
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"

inline void OptionsClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    //Hook_Function(0x00525884, *OptionsClass::Adjust_Palette);
    Hook_Function(0x00525A24, *OptionsClass::Load_Settings);
    Hook_Function(0x005263A8, *OptionsClass::Save_Settings);
#endif
}
#endif

#endif // OPTIONS_H
