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
#include "options.h"
#include "audio.h"
#include "gamefile.h"
#include "hsv.h"
#include "gameini.h"
#include "rgb.h"
#include "session.h"
#include "expansion.h"

OptionsClass::OptionsClass() :
    GameSpeed(GAMESPEED_3),
    ScrollRate(SCROLLSPEED_3),
    Volume(".40"),
    ScoreVolume(".25"),
    MultiplayerScoreVolume("0"),
    Brightness(fixed::_1_2),
    Tint(fixed::_1_2),
    Saturation(fixed::_1_2),
    Contrast(fixed::_1_2),
    AutoScroll(true),
    ScoreRepeats(false),
    ScoreShuffles(false),
    PaletteScroll(true),
    AllowSidebarToggle(false),
    CounterstrikeEnabled(false),
    AftermathEnabled(false),
    KeyForceMove1(KN_LALT),
    KeyForceMove2(KN_RALT),
    KeyForceAttack1(KN_LCTRL),
    KeyForceAttack2(KN_RCTRL),
    KeySelect1(KN_LSHIFT),
    KeySelect2(KN_RSHIFT),
    KeyScatter(KN_X),
    KeyStop(KN_S),
    KeyGuard(KN_G),
    KeyNext(KN_N),
    KeyPrevious(KN_B),
    KeyFormation(KN_F),
    KeyHome1(KN_HOME),
    KeyHome2(KN_E_HOME),
    KeyBase(KN_H),
    KeyResign(KN_R),
    KeyAlliance(KN_A),
    KeyBookmark1(KN_F9),
    KeyBookmark2(KN_F10),
    KeyBookmark3(KN_F11),
    KeyBookmark4(KN_F12),
    KeySelectView(KN_E),
    KeyRepairToggle(KN_T),
    KeyRepairOn(KN_NONE),
    KeyRepairOff(KN_NONE),
    KeySellToggle(KN_Y),
    KeySellOn(KN_NONE),
    KeySellOff(KN_NONE),
    KeyMapToggle(KN_U),
    KeySidebarUp(KN_UP),
    KeySidebarDown(KN_DOWN),
    KeyOption1(KN_ESC),
    KeyOption2(KN_SPACE),
    KeyScrollLeft(KN_NONE),
    KeyScrollRight(KN_NONE),
    KeyScrollUp(KN_NONE),
    KeyScrollDown(KN_NONE),
    KeyQueueMove1(KN_Q),
    KeyQueueMove2(KN_Q),
    KeyTeam1(KN_1),
    KeyTeam2(KN_2),
    KeyTeam3(KN_3),
    KeyTeam4(KN_4),
    KeyTeam5(KN_5),
    KeyTeam6(KN_6),
    KeyTeam7(KN_7),
    KeyTeam8(KN_8),
    KeyTeam9(KN_9),
    KeyTeam10(KN_0)
{
}

/**
 * @brief Carries out one time tasks for the class.
 *
 * 0x005251A8
 */
void OptionsClass::One_Time()
{
    Set_Score_Vol(ScoreVolume * 256);
}

/**
 * @brief Saves the options out to the configuration file (redalert.ini in Red Alert for example).
 *
 * 0x005263A8
 */
void OptionsClass::Save_Settings()
{
    // TODO Some global to handle if the game is TD or RA for later TD support.
    GameFileClass fc("redalert.ini");
    GameINIClass ini;

    if (fc.Is_Available()) {
        ini.Load(fc);
    }

    ini.Put_Int("Options", "GameSpeed", GameSpeed);
    ini.Put_Int("Options", "ScrollRate", ScrollRate);
    ini.Put_Fixed("Options", "Volume", Volume);

    if (Session.Game_To_Play() == GAME_CAMPAIGN) {
        ini.Put_Fixed("Options", "ScoreVolume", ScoreVolume);
    }

    ini.Put_Fixed("Options", "MultiplayerScoreVolume", MultiplayerScoreVolume);
    ini.Put_Fixed("Options", "Brightness", Brightness);
    ini.Put_Fixed("Options", "Contrast", Contrast);
    ini.Put_Fixed("Options", "Color", Saturation);
    ini.Put_Fixed("Options", "Tint", Tint);
    ini.Put_Bool("Options", "IsScoreRepeat", ScoreRepeats);
    ini.Put_Bool("Options", "IsScoreShuffle", ScoreShuffles);
    ini.Put_Bool("Options", "PaletteScroll", PaletteScroll);
    ini.Put_Bool("Options", "AllowSidebarToggle", AllowSidebarToggle);

    ini.Put_Bool("Expansions", "CounterstrikeEnabled", CounterstrikeEnabled);
    ini.Put_Bool("Expansions", "AftermathEnabled", AftermathEnabled);

    ini.Put_KeyNumType("WinHotkeys", "KeyForceMove1", KeyForceMove1);
    ini.Put_KeyNumType("WinHotkeys", "KeyForceMove2", KeyForceMove2);
    ini.Put_KeyNumType("WinHotkeys", "KeyForceAttack1", KeyForceAttack1);
    ini.Put_KeyNumType("WinHotkeys", "KeyForceAttack2", KeyForceAttack2);
    ini.Put_KeyNumType("WinHotkeys", "KeySelect1", KeySelect1);
    ini.Put_KeyNumType("WinHotkeys", "KeySelect2", KeySelect2);
    ini.Put_KeyNumType("WinHotkeys", "KeyScatter", KeyScatter);
    ini.Put_KeyNumType("WinHotkeys", "KeyStop", KeyStop);
    ini.Put_KeyNumType("WinHotkeys", "KeyGuard", KeyGuard);
    ini.Put_KeyNumType("WinHotkeys", "KeyNext", KeyNext);
    ini.Put_KeyNumType("WinHotkeys", "KeyPrevious", KeyPrevious);
    ini.Put_KeyNumType("WinHotkeys", "KeyFormation", KeyFormation);
    ini.Put_KeyNumType("WinHotkeys", "KeyHome1", KeyHome1);
    ini.Put_KeyNumType("WinHotkeys", "KeyHome2", KeyHome2);
    ini.Put_KeyNumType("WinHotkeys", "KeyBase", KeyBase);
    ini.Put_KeyNumType("WinHotkeys", "KeyResign", KeyResign);
    ini.Put_KeyNumType("WinHotkeys", "KeyAlliance", KeyAlliance);
    ini.Put_KeyNumType("WinHotkeys", "KeyBookmark1", KeyBookmark1);
    ini.Put_KeyNumType("WinHotkeys", "KeyBookmark2", KeyBookmark2);
    ini.Put_KeyNumType("WinHotkeys", "KeyBookmark3", KeyBookmark3);
    ini.Put_KeyNumType("WinHotkeys", "KeyBookmark4", KeyBookmark4);
    ini.Put_KeyNumType("WinHotkeys", "KeySelectView", KeySelectView);
    ini.Put_KeyNumType("WinHotkeys", "KeyRepairToggle", KeyRepairToggle);
    ini.Put_KeyNumType("WinHotkeys", "KeyRepairOn", KeyRepairOn);
    ini.Put_KeyNumType("WinHotkeys", "KeyRepairOff", KeyRepairOff);
    ini.Put_KeyNumType("WinHotkeys", "KeySellToggle", KeySellToggle);
    ini.Put_KeyNumType("WinHotkeys", "KeySellOn", KeySellOn);
    ini.Put_KeyNumType("WinHotkeys", "KeySellOff", KeySellOff);
    ini.Put_KeyNumType("WinHotkeys", "KeyMapToggle", KeyMapToggle);
    ini.Put_KeyNumType("WinHotkeys", "KeySidebarUp", KeySidebarUp);
    ini.Put_KeyNumType("WinHotkeys", "KeySidebarDown", KeySidebarDown);
    ini.Put_KeyNumType("WinHotkeys", "KeyOption1", KeyOption1);
    ini.Put_KeyNumType("WinHotkeys", "KeyOption2", KeyOption2);
    ini.Put_KeyNumType("WinHotkeys", "KeyScrollLeft", KeyScrollLeft);
    ini.Put_KeyNumType("WinHotkeys", "KeyScrollRight", KeyScrollRight);
    ini.Put_KeyNumType("WinHotkeys", "KeyScrollUp", KeyScrollUp);
    ini.Put_KeyNumType("WinHotkeys", "KeyScrollDown", KeyScrollDown);
    ini.Put_KeyNumType("WinHotkeys", "KeyQueueMove1", KeyQueueMove1);
    ini.Put_KeyNumType("WinHotkeys", "KeyQueueMove2", KeyQueueMove2);
    ini.Put_KeyNumType("WinHotkeys", "KeyTeam1", KeyTeam1);
    ini.Put_KeyNumType("WinHotkeys", "KeyTeam2", KeyTeam2);
    ini.Put_KeyNumType("WinHotkeys", "KeyTeam3", KeyTeam3);
    ini.Put_KeyNumType("WinHotkeys", "KeyTeam4", KeyTeam4);
    ini.Put_KeyNumType("WinHotkeys", "KeyTeam5", KeyTeam5);
    ini.Put_KeyNumType("WinHotkeys", "KeyTeam6", KeyTeam6);
    ini.Put_KeyNumType("WinHotkeys", "KeyTeam7", KeyTeam7);
    ini.Put_KeyNumType("WinHotkeys", "KeyTeam8", KeyTeam8);
    ini.Put_KeyNumType("WinHotkeys", "KeyTeam9", KeyTeam9);
    ini.Put_KeyNumType("WinHotkeys", "KeyTeam10", KeyTeam10);

    ini.Save(fc);
}

/**
 * @brief Loads the options from the configuration file (redalert.ini in Red Alert for example).
 *
 * 0x00525A24
 */
void OptionsClass::Load_Settings()
{
    GameFileClass fc("redalert.ini");

    GameINIClass ini;
    ini.Load(fc);

    GameSpeed = ini.Get_Int("Options", "GameSpeed", GameSpeed);
    ScrollRate = ini.Get_Int("Options", "ScrollRate", ScrollRate);
    Set_Sound_Volume(ini.Get_Fixed("Options", "Volume", Volume));
    Set_Score_Volume(ini.Get_Fixed("Options", "ScoreVolume", ScoreVolume));
    MultiplayerScoreVolume = ini.Get_Fixed("Options", "MultiplayerScoreVolume", MultiplayerScoreVolume);
    Set_Brightness(ini.Get_Fixed("Options", "Brightness", Brightness));
    Set_Contrast(ini.Get_Fixed("Options", "Contrast", Contrast));
    Set_Saturation(ini.Get_Fixed("Options", "Color", Saturation));
    Set_Tint(ini.Get_Fixed("Options", "Tint", Tint));
    AutoScroll = ini.Get_Bool("Options", "AutoScroll", AutoScroll);
    ScoreRepeats = ini.Get_Bool("Options", "IsScoreRepeat", ScoreRepeats);
    ScoreShuffles = ini.Get_Bool("Options", "IsScoreShuffle", ScoreShuffles);
    PaletteScroll = ini.Get_Bool("Options", "PaletteScroll", PaletteScroll);
    AllowSidebarToggle = ini.Get_Bool("Options", "AllowSidebarToggle", false); // TODO use variable as default when ctor used.

    CounterstrikeEnabled = ini.Get_Bool("Expansions", "CounterstrikeEnabled", false); // TODO use variable as default when ctor used.
    AftermathEnabled = ini.Get_Bool("Expansions", "AftermathEnabled", false); // TODO use variable as default when ctor used.

    KeyForceMove1 = ini.Get_KeyNumType("WinHotkeys", "KeyForceMove1", KeyForceMove1);
    KeyForceMove2 = ini.Get_KeyNumType("WinHotkeys", "KeyForceMove2", KeyForceMove2);
    KeyForceAttack1 = ini.Get_KeyNumType("WinHotkeys", "KeyForceAttack1", KeyForceAttack1);
    KeyForceAttack2 = ini.Get_KeyNumType("WinHotkeys", "KeyForceAttack2", KeyForceAttack2);
    KeySelect1 = ini.Get_KeyNumType("WinHotkeys", "KeySelect1", KeySelect1);
    KeySelect2 = ini.Get_KeyNumType("WinHotkeys", "KeySelect2", KeySelect2);
    KeyScatter = ini.Get_KeyNumType("WinHotkeys", "KeyScatter", KeyScatter);
    KeyStop = ini.Get_KeyNumType("WinHotkeys", "KeyStop", KeyStop);
    KeyGuard = ini.Get_KeyNumType("WinHotkeys", "KeyGuard", KeyGuard);
    KeyNext = ini.Get_KeyNumType("WinHotkeys", "KeyNext", KeyNext);
    KeyPrevious = ini.Get_KeyNumType("WinHotkeys", "KeyPrevious", KeyPrevious);
    KeyFormation = ini.Get_KeyNumType("WinHotkeys", "KeyFormation", KeyFormation);
    KeyHome1 = ini.Get_KeyNumType("WinHotkeys", "KeyHome1", KeyHome1);
    KeyHome2 = ini.Get_KeyNumType("WinHotkeys", "KeyHome2", KeyHome2);
    KeyBase = ini.Get_KeyNumType("WinHotkeys", "KeyBase", KeyBase);
    KeyResign = ini.Get_KeyNumType("WinHotkeys", "KeyResign", KeyResign);
    KeyAlliance = ini.Get_KeyNumType("WinHotkeys", "KeyAlliance", KeyAlliance);
    KeyBookmark1 = ini.Get_KeyNumType("WinHotkeys", "KeyBookmark1", KeyBookmark1);
    KeyBookmark2 = ini.Get_KeyNumType("WinHotkeys", "KeyBookmark2", KeyBookmark2);
    KeyBookmark3 = ini.Get_KeyNumType("WinHotkeys", "KeyBookmark3", KeyBookmark3);
    KeyBookmark4 = ini.Get_KeyNumType("WinHotkeys", "KeyBookmark4", KeyBookmark4);
    KeySelectView = ini.Get_KeyNumType("WinHotkeys", "KeySelectView", KeySelectView);
    KeyRepairToggle = ini.Get_KeyNumType("WinHotkeys", "KeyRepairToggle", KeyRepairToggle);
    KeyRepairOn = ini.Get_KeyNumType("WinHotkeys", "KeyRepairOn", KeyRepairOn);
    KeyRepairOff = ini.Get_KeyNumType("WinHotkeys", "KeyRepairOff", KeyRepairOff);
    KeySellToggle = ini.Get_KeyNumType("WinHotkeys", "KeySellToggle", KeySellToggle);
    KeySellOn = ini.Get_KeyNumType("WinHotkeys", "KeySellOn", KeySellOn);
    KeySellOff = ini.Get_KeyNumType("WinHotkeys", "KeySellOff", KeySellOff);
    KeyMapToggle = ini.Get_KeyNumType("WinHotkeys", "KeyMapToggle", KeyMapToggle);
    KeySidebarUp = ini.Get_KeyNumType("WinHotkeys", "KeySidebarUp", KeySidebarUp);
    KeySidebarDown = ini.Get_KeyNumType("WinHotkeys", "KeySidebarDown", KeySidebarDown);
    KeyOption1 = ini.Get_KeyNumType("WinHotkeys", "KeyOption1", KeyOption1);
    KeyOption2 = ini.Get_KeyNumType("WinHotkeys", "KeyOption2", KeyOption2);
    KeyScrollLeft = ini.Get_KeyNumType("WinHotkeys", "KeyScrollLeft", KeyScrollLeft);
    KeyScrollRight = ini.Get_KeyNumType("WinHotkeys", "KeyScrollRight", KeyScrollRight);
    KeyScrollUp = ini.Get_KeyNumType("WinHotkeys", "KeyScrollUp", KeyScrollUp);
    KeyScrollDown = ini.Get_KeyNumType("WinHotkeys", "KeyScrollDown", KeyScrollDown);
    KeyQueueMove1 = ini.Get_KeyNumType("WinHotkeys", "KeyQueueMove1", KeyQueueMove1);
    KeyQueueMove2 = ini.Get_KeyNumType("WinHotkeys", "KeyQueueMove2", KeyQueueMove2);
    KeyTeam1 = ini.Get_KeyNumType("WinHotkeys", "KeyTeam1", KeyTeam1);
    KeyTeam2 = ini.Get_KeyNumType("WinHotkeys", "KeyTeam2", KeyTeam2);
    KeyTeam3 = ini.Get_KeyNumType("WinHotkeys", "KeyTeam3", KeyTeam3);
    KeyTeam4 = ini.Get_KeyNumType("WinHotkeys", "KeyTeam4", KeyTeam4);
    KeyTeam5 = ini.Get_KeyNumType("WinHotkeys", "KeyTeam5", KeyTeam5);
    KeyTeam6 = ini.Get_KeyNumType("WinHotkeys", "KeyTeam6", KeyTeam6);
    KeyTeam7 = ini.Get_KeyNumType("WinHotkeys", "KeyTeam7", KeyTeam7);
    KeyTeam8 = ini.Get_KeyNumType("WinHotkeys", "KeyTeam8", KeyTeam8);
    KeyTeam9 = ini.Get_KeyNumType("WinHotkeys", "KeyTeam9", KeyTeam9);
    KeyTeam10 = ini.Get_KeyNumType("WinHotkeys", "KeyTeam10", KeyTeam10);
}

/**
 * @brief Carries out a palette adjustment.
 *
 * 0x00525884
 */
void OptionsClass::Adjust_Palette(
    PaletteClass &src, PaletteClass &dst, fixed brightness, fixed saturation, fixed tint, fixed contrast) const
{
    if (&src[0] != nullptr && &dst[0] != nullptr) {
        for (int index = 0; index < PaletteClass::PALETTE_ENTRIES; ++index) {
            // Index 16 is pure white, we do not want to adjust this.
            if (index == 16) {
                dst[index] = src[index];
            } else {
                HSVClass hsv = src[index];
                hsv.Adjust(brightness, saturation, tint, contrast);
                dst[index] = hsv;
            }
        }
    }
}

/**
 * @brief Fixes up the CCPalette with the current display adjustment values.
 *
 * 0x00526BB0
 */
void OptionsClass::Fixup_Palette() const
{
    Adjust_Palette(OriginalPalette, GamePalette, Brightness, Saturation, Tint, Contrast);
    CCPalette = GamePalette;
}

/**
 * @brief Returns a delay normalized to the game speed.
 *
 * 0x00526B7C
 */
int OptionsClass::Normalize_Delay(int delay) const
{
    static int _adjust[4][8] = {
        { 2, 2, 1, 1, 1, 1, 1, 1 },
        { 3, 3, 3, 2, 2, 2, 1, 1 },
        { 5, 4, 4, 3, 3, 2, 2, 1 },
        { 7, 6, 5, 4, 4, 4, 3, 2 }
    };

    if (delay == 0) {
        return 0;
    }

    if (delay < 5) {
        return _adjust[delay][GameSpeed];
    }

    return 8 * delay / (GameSpeed + 1);
}

/**
 * @brief Sets the volume for background music.
 *
 * 0x0052520C
 */
void OptionsClass::Set_Score_Volume(fixed volume, BOOL beep)
{
    if (volume >= 1) {
        volume.Set_Word(255);
    }

    Set_Score_Vol(256 * volume);

    Volume = volume;

    if (beep) {
        // TODO requires ThemeClass
        // if (Theme.Still_Playing()) {
        //    Sound_Effect(VOC_RABEEP1, fixed::_1_1, 1, 0, HOUSES_NONE);
        //}
    }
}

/**
 * @brief Sets the volume for in game sounds.
 *
 * 0x005252BC
 */
void OptionsClass::Set_Sound_Volume(fixed volume, BOOL beep)
{
    if (volume >= 1) {
        volume.Set_Word(255);
    }

    Volume = volume;

    if (beep) {
        Sound_Effect(VOC_RABEEP1, fixed::_1_1, 1, 0, HOUSES_NONE);
    }
}

/**
 * @brief Sets the display brightness.
 *
 * 0x0052534C
 */
void OptionsClass::Set_Brightness(fixed brightness)
{
    Brightness = brightness * fixed::_1_2 + fixed::_1_4;
    Adjust_Palette(OriginalPalette, GamePalette, Brightness, Saturation, Tint, Contrast);
    GamePalette.Set();
}

/**
 * @brief Sets the display saturation.
 *
 * 0x0052551C
 */
void OptionsClass::Set_Saturation(fixed saturation)
{
    Saturation = saturation;
    Adjust_Palette(OriginalPalette, GamePalette, Brightness, Saturation, Tint, Contrast);
    GamePalette.Set();
}

/**
 * @brief Sets the display contrast.
 *
 * 0x005255E8
 */
void OptionsClass::Set_Contrast(fixed contrast)
{
    Contrast = contrast * fixed::_1_2 + fixed::_1_4;
    Adjust_Palette(OriginalPalette, GamePalette, Brightness, Saturation, Tint, Contrast);
    GamePalette.Set();
}

/**
 * @brief Sets the display tint.
 *
 * 0x005257B8
 */
void OptionsClass::Set_Tint(fixed tint)
{
    Tint = tint;
    Adjust_Palette(OriginalPalette, GamePalette, Brightness, Saturation, Tint, Contrast);
    GamePalette.Set();
}

/**
 * @brief Calls most of the set functions with the members existing values.
 *
 * 0x00526A88
 */
void OptionsClass::Set()
{
    Set_Brightness(Brightness);
    Set_Contrast(Contrast);
    Set_Saturation(Saturation);
    Set_Tint(Tint);
    Set_Sound_Volume(Volume);
    Set_Score_Volume(ScoreVolume);
    Set_Shuffle(ScoreShuffles);
    Set_Repeat(ScoreRepeats);
}
