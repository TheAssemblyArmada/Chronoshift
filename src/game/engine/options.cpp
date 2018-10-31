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
#include "ini.h"
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
    INIClass ini;

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

    ini.Put_Int("WinHotkeys", "KeyForceMove1", KeyForceMove1);
    ini.Put_Int("WinHotkeys", "KeyForceMove2", KeyForceMove2);
    ini.Put_Int("WinHotkeys", "KeyForceAttack1", KeyForceAttack1);
    ini.Put_Int("WinHotkeys", "KeyForceAttack2", KeyForceAttack2);
    ini.Put_Int("WinHotkeys", "KeySelect1", KeySelect1);
    ini.Put_Int("WinHotkeys", "KeySelect2", KeySelect2);
    ini.Put_Int("WinHotkeys", "KeyScatter", KeyScatter);
    ini.Put_Int("WinHotkeys", "KeyStop", KeyStop);
    ini.Put_Int("WinHotkeys", "KeyGuard", KeyGuard);
    ini.Put_Int("WinHotkeys", "KeyNext", KeyNext);
    ini.Put_Int("WinHotkeys", "KeyPrevious", KeyPrevious);
    ini.Put_Int("WinHotkeys", "KeyFormation", KeyFormation);
    ini.Put_Int("WinHotkeys", "KeyHome1", KeyHome1);
    ini.Put_Int("WinHotkeys", "KeyHome2", KeyHome2);
    ini.Put_Int("WinHotkeys", "KeyBase", KeyBase);
    ini.Put_Int("WinHotkeys", "KeyResign", KeyResign);
    ini.Put_Int("WinHotkeys", "KeyAlliance", KeyAlliance);
    ini.Put_Int("WinHotkeys", "KeyBookmark1", KeyBookmark1);
    ini.Put_Int("WinHotkeys", "KeyBookmark2", KeyBookmark2);
    ini.Put_Int("WinHotkeys", "KeyBookmark3", KeyBookmark3);
    ini.Put_Int("WinHotkeys", "KeyBookmark4", KeyBookmark4);
    ini.Put_Int("WinHotkeys", "KeySelectView", KeySelectView);
    ini.Put_Int("WinHotkeys", "KeyRepairToggle", KeyRepairToggle);
    ini.Put_Int("WinHotkeys", "KeyRepairOn", KeyRepairOn);
    ini.Put_Int("WinHotkeys", "KeyRepairOff", KeyRepairOff);
    ini.Put_Int("WinHotkeys", "KeySellToggle", KeySellToggle);
    ini.Put_Int("WinHotkeys", "KeySellOn", KeySellOn);
    ini.Put_Int("WinHotkeys", "KeySellOff", KeySellOff);
    ini.Put_Int("WinHotkeys", "KeyMapToggle", KeyMapToggle);
    ini.Put_Int("WinHotkeys", "KeySidebarUp", KeySidebarUp);
    ini.Put_Int("WinHotkeys", "KeySidebarDown", KeySidebarDown);
    ini.Put_Int("WinHotkeys", "KeyOption1", KeyOption1);
    ini.Put_Int("WinHotkeys", "KeyOption2", KeyOption2);
    ini.Put_Int("WinHotkeys", "KeyScrollLeft", KeyScrollLeft);
    ini.Put_Int("WinHotkeys", "KeyScrollRight", KeyScrollRight);
    ini.Put_Int("WinHotkeys", "KeyScrollUp", KeyScrollUp);
    ini.Put_Int("WinHotkeys", "KeyScrollDown", KeyScrollDown);
    ini.Put_Int("WinHotkeys", "KeyQueueMove1", KeyQueueMove1);
    ini.Put_Int("WinHotkeys", "KeyQueueMove2", KeyQueueMove2);
    ini.Put_Int("WinHotkeys", "KeyTeam1", KeyTeam1);
    ini.Put_Int("WinHotkeys", "KeyTeam2", KeyTeam2);
    ini.Put_Int("WinHotkeys", "KeyTeam3", KeyTeam3);
    ini.Put_Int("WinHotkeys", "KeyTeam4", KeyTeam4);
    ini.Put_Int("WinHotkeys", "KeyTeam5", KeyTeam5);
    ini.Put_Int("WinHotkeys", "KeyTeam6", KeyTeam6);
    ini.Put_Int("WinHotkeys", "KeyTeam7", KeyTeam7);
    ini.Put_Int("WinHotkeys", "KeyTeam8", KeyTeam8);
    ini.Put_Int("WinHotkeys", "KeyTeam9", KeyTeam9);
    ini.Put_Int("WinHotkeys", "KeyTeam10", KeyTeam10);

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

    INIClass ini;
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

    KeyForceMove1 = ini.Get_Int("WinHotkeys", "KeyForceMove1", KeyForceMove1) & 0xEFFF;
    KeyForceMove2 = ini.Get_Int("WinHotkeys", "KeyForceMove2", KeyForceMove2) & 0xEFFF;
    KeyForceAttack1 = ini.Get_Int("WinHotkeys", "KeyForceAttack1", KeyForceAttack1) & 0xEFFF;
    KeyForceAttack2 = ini.Get_Int("WinHotkeys", "KeyForceAttack2", KeyForceAttack2) & 0xEFFF;
    KeySelect1 = ini.Get_Int("WinHotkeys", "KeySelect1", KeySelect1) & 0xEFFF;
    KeySelect2 = ini.Get_Int("WinHotkeys", "KeySelect2", KeySelect2) & 0xEFFF;
    KeyScatter = ini.Get_Int("WinHotkeys", "KeyScatter", KeyScatter) & 0xEFFF;
    KeyStop = ini.Get_Int("WinHotkeys", "KeyStop", KeyStop) & 0xEFFF;
    KeyGuard = ini.Get_Int("WinHotkeys", "KeyGuard", KeyGuard) & 0xEFFF;
    KeyNext = ini.Get_Int("WinHotkeys", "KeyNext", KeyNext) & 0xEFFF;
    KeyPrevious = ini.Get_Int("WinHotkeys", "KeyPrevious", KeyPrevious) & 0xEFFF;
    KeyFormation = ini.Get_Int("WinHotkeys", "KeyFormation", KeyFormation) & 0xEFFF;
    KeyHome1 = ini.Get_Int("WinHotkeys", "KeyHome1", KeyHome1) & 0xEFFF;
    KeyHome2 = ini.Get_Int("WinHotkeys", "KeyHome2", KeyHome2) & 0xEFFF;
    KeyBase = ini.Get_Int("WinHotkeys", "KeyBase", KeyBase) & 0xEFFF;
    KeyResign = ini.Get_Int("WinHotkeys", "KeyResign", KeyResign) & 0xEFFF;
    KeyAlliance = ini.Get_Int("WinHotkeys", "KeyAlliance", KeyAlliance) & 0xEFFF;
    KeyBookmark1 = ini.Get_Int("WinHotkeys", "KeyBookmark1", KeyBookmark1) & 0xEFFF;
    KeyBookmark2 = ini.Get_Int("WinHotkeys", "KeyBookmark2", KeyBookmark2) & 0xEFFF;
    KeyBookmark3 = ini.Get_Int("WinHotkeys", "KeyBookmark3", KeyBookmark3) & 0xEFFF;
    KeyBookmark4 = ini.Get_Int("WinHotkeys", "KeyBookmark4", KeyBookmark4) & 0xEFFF;
    KeySelectView = ini.Get_Int("WinHotkeys", "KeySelectView", KeySelectView) & 0xEFFF;
    KeyRepairToggle = ini.Get_Int("WinHotkeys", "KeyRepairToggle", KeyRepairToggle) & 0xEFFF;
    KeyRepairOn = ini.Get_Int("WinHotkeys", "KeyRepairOn", KeyRepairOn) & 0xEFFF;
    KeyRepairOff = ini.Get_Int("WinHotkeys", "KeyRepairOff", KeyRepairOff) & 0xEFFF;
    KeySellToggle = ini.Get_Int("WinHotkeys", "KeySellToggle", KeySellToggle) & 0xEFFF;
    KeySellOn = ini.Get_Int("WinHotkeys", "KeySellOn", KeySellOn) & 0xEFFF;
    KeySellOff = ini.Get_Int("WinHotkeys", "KeySellOff", KeySellOff) & 0xEFFF;
    KeyMapToggle = ini.Get_Int("WinHotkeys", "KeyMapToggle", KeyMapToggle) & 0xEFFF;
    KeySidebarUp = ini.Get_Int("WinHotkeys", "KeySidebarUp", KeySidebarUp) & 0xEFFF;
    KeySidebarDown = ini.Get_Int("WinHotkeys", "KeySidebarDown", KeySidebarDown) & 0xEFFF;
    KeyOption1 = ini.Get_Int("WinHotkeys", "KeyOption1", KeyOption1) & 0xEFFF;
    KeyOption2 = ini.Get_Int("WinHotkeys", "KeyOption2", KeyOption2) & 0xEFFF;
    KeyScrollLeft = ini.Get_Int("WinHotkeys", "KeyScrollLeft", KeyScrollLeft) & 0xEFFF;
    KeyScrollRight = ini.Get_Int("WinHotkeys", "KeyScrollRight", KeyScrollRight) & 0xEFFF;
    KeyScrollUp = ini.Get_Int("WinHotkeys", "KeyScrollUp", KeyScrollUp) & 0xEFFF;
    KeyScrollDown = ini.Get_Int("WinHotkeys", "KeyScrollDown", KeyScrollDown) & 0xEFFF;
    KeyQueueMove1 = ini.Get_Int("WinHotkeys", "KeyQueueMove1", KeyQueueMove1) & 0xEFFF;
    KeyQueueMove2 = ini.Get_Int("WinHotkeys", "KeyQueueMove2", KeyQueueMove2) & 0xEFFF;
    KeyTeam1 = ini.Get_Int("WinHotkeys", "KeyTeam1", KeyTeam1) & 0xEFFF;
    KeyTeam2 = ini.Get_Int("WinHotkeys", "KeyTeam2", KeyTeam2) & 0xEFFF;
    KeyTeam3 = ini.Get_Int("WinHotkeys", "KeyTeam3", KeyTeam3) & 0xEFFF;
    KeyTeam4 = ini.Get_Int("WinHotkeys", "KeyTeam4", KeyTeam4) & 0xEFFF;
    KeyTeam5 = ini.Get_Int("WinHotkeys", "KeyTeam5", KeyTeam5) & 0xEFFF;
    KeyTeam6 = ini.Get_Int("WinHotkeys", "KeyTeam6", KeyTeam6) & 0xEFFF;
    KeyTeam7 = ini.Get_Int("WinHotkeys", "KeyTeam7", KeyTeam7) & 0xEFFF;
    KeyTeam8 = ini.Get_Int("WinHotkeys", "KeyTeam8", KeyTeam8) & 0xEFFF;
    KeyTeam9 = ini.Get_Int("WinHotkeys", "KeyTeam9", KeyTeam9) & 0xEFFF;
    KeyTeam10 = ini.Get_Int("WinHotkeys", "KeyTeam10", KeyTeam10) & 0xEFFF;
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
