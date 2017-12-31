/**
 * @file
 *
 * @Author CCHyper, OmniBlade
 *
 * @brief Global variables that are used amongst many classes.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "options.h"
#include "audio.h"
#include "gbuffer.h"
#include "hsv.h"
#include "rgb.h"

#ifndef RAPP_STANDALONE
GameOptionsClass &Options = Make_Global<GameOptionsClass>(0x00668188);
#else
GameOptionsClass Options;
#endif

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
    IsScoreRepeat(false),
    IsScoreShuffle(false),
    PaletteScroll(true),
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
 * @brief Carries out a palette adjustment.
 *
 * 0x00525884
 */
void OptionsClass::Adjust_Palette(PaletteClass & src, PaletteClass & dst, fixed brightness, fixed saturation, fixed tint, fixed contrast)
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
 * @brief Performs processing for this object per frame.
 *
 * 0x004C9EE0
 */
void GameOptionsClass::Process()
{
    // TODO
#ifndef RAPP_STANDALONE
    void (*process)() = reinterpret_cast<void (*)()>(0x004C9EE0);
    process();
#endif
}

/**
 * @brief Adjusts the values of variables based on current resolution.
 *
 * 0x004CAC6C
 */
void GameOptionsClass::Adjust_Vars_For_Resolution()
{
    // TODO work out what these are all used for if anything.
    OptionsDialogWidth = 448;
    OptionsDialogHeight = 222;
    OptionsDialogCenterX = (g_seenBuff.Get_Width() - OptionsDialogWidth) / 2;
    OptionsDialogCenterY = (g_seenBuff.Get_Height() - OptionsDialogHeight) / 2;
    field_8C = 260;
    OptionsDialogButtonYSpacing = 18;
    field_94 = 10;
    OptionsDialogButtonOffsetFromTop = 42;
    field_9C = 144;
    field_A0 = 32;
    OptionsDialogBottomButtonOffsetTop = OptionsDialogHeight - 38;
}
