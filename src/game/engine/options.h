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

class PaletteClass;

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
        PaletteClass &src, PaletteClass &dst, fixed_t brightness, fixed_t saturation, fixed_t tint, fixed_t contrast) const;
    void Fixup_Palette() const;
    int Normalize_Volume(int volume) const { return volume * m_Volume; }
    int Normalize_Delay(int delay) const;
    void Set_Shuffle(BOOL shuffle) { m_ScoreShuffles = shuffle; }
    void Set_Repeat(BOOL repeat) { m_ScoreRepeats = repeat; }
    void Set_Score_Volume(fixed_t volume, BOOL feedback = false);
    void Set_Sound_Volume(fixed_t volume, BOOL feedback = false);
    void Set_MPlayer_Score_Volume(fixed_t volume) { m_MultiplayerScoreVolume = volume; }

    void Set_Brightness(fixed_t brightness);
    void Set_Saturation(fixed_t saturation);
    void Set_Contrast(fixed_t contrast);
    void Set_Tint(fixed_t tint);
    void Set();

    int Game_Speed() const { return m_GameSpeed; }
    int Get_Scroll_Rate() const { return m_ScrollRate; }
    const fixed_t &Get_Sound_Volume() const { return m_Volume; }
    const fixed_t &Get_Score_Volume() const { return m_ScoreVolume; }
    const fixed_t &Get_Multiplayer_Score_Volume() { return m_MultiplayerScoreVolume; }
    BOOL Get_Auto_Scroll() const { return m_AutoScroll; }
    BOOL Get_Repeat() const { return m_ScoreRepeats; }
    BOOL Get_Shuffle() const { return m_ScoreShuffles; }
    BOOL Sidebar_Toggle_Allowed() const { return m_AllowSidebarToggle; }
    BOOL Free_Scrolling() const { return m_FreeScrolling; }
    BOOL Is_Counterstrike_Enabled() const { return m_CounterstrikeEnabled; }
    BOOL Is_Aftermath_Enabled() const { return m_AftermathEnabled; }
    BOOL Expansion_Options_Present() const { return m_ExpansionOptionsPresent; }
    void Enable_Expansion_Options() { m_ExpansionOptionsPresent = true; }
    fixed_t Get_Brightness() const { return (m_Brightness - fixed_t::_1_4) / fixed_t::_1_2; }
    fixed_t Get_Saturation() const { return m_Saturation; }
    fixed_t Get_Contrast() const { return (m_Contrast - fixed_t::_1_4) / fixed_t::_1_2; }
    fixed_t Get_Tint() const { return m_Tint; }

    KeyNumType Get_KeySelectView() const { return m_KeySelectView; }
    KeyNumType Get_KeyRepairToggle() const { return m_KeyRepairToggle; }
    KeyNumType Get_KeySellToggle() const { return m_KeySellToggle; }
    KeyNumType Get_KeyMapToggle() const { return m_KeyMapToggle; }
    KeyNumType Get_KeySidebarUp() const { return m_KeySidebarUp; }
    KeyNumType Get_KeySidebarDown() const { return m_KeySidebarDown; }
    KeyNumType Get_KeyOption1() const { return m_KeyOption1; }
    KeyNumType Get_KeyOption2() const { return m_KeyOption2; }
    KeyNumType Get_KeyScrollLeft() const { return m_KeyScrollLeft; }
    KeyNumType Get_KeyScrollRight() const { return m_KeyScrollRight; }
    KeyNumType Get_KeyScrollUp() const { return m_KeyScrollUp; }
    KeyNumType Get_KeyScrollDown() const { return m_KeyScrollDown; }
    KeyNumType Get_KeyTeam1() const { return m_KeyTeam1; }
    KeyNumType Get_KeyTeam2() const { return m_KeyTeam2; }
    KeyNumType Get_KeyTeam3() const { return m_KeyTeam3; }
    KeyNumType Get_KeyTeam4() const { return m_KeyTeam4; }
    KeyNumType Get_KeyTeam5() const { return m_KeyTeam5; }
    KeyNumType Get_KeyTeam6() const { return m_KeyTeam6; }
    KeyNumType Get_KeyTeam7() const { return m_KeyTeam7; }
    KeyNumType Get_KeyTeam8() const { return m_KeyTeam8; }
    KeyNumType Get_KeyTeam9() const { return m_KeyTeam9; }
    KeyNumType Get_KeyTeam10() const { return m_KeyTeam10; }
    KeyNumType Get_KeyBookmark1() const { return m_KeyBookmark1; }
    KeyNumType Get_KeyBookmark2() const { return m_KeyBookmark2; }
    KeyNumType Get_KeyBookmark3() const { return m_KeyBookmark3; }
    KeyNumType Get_KeyBookmark4() const { return m_KeyBookmark4; }
    KeyNumType Get_KeyNext() const { return m_KeyNext; }
    KeyNumType Get_KeyPrevious() const { return m_KeyPrevious; }
    KeyNumType Get_KeyAlliance() const { return m_KeyAlliance; }
    KeyNumType Get_KeyFormation() const { return m_KeyFormation; }
    KeyNumType Get_KeyBase() const { return m_KeyBase; }
    KeyNumType Get_KeyHome1() const { return m_KeyHome1; }
    KeyNumType Get_KeyHome2() const { return m_KeyHome2; }
    KeyNumType Get_KeyStop() const { return m_KeyStop; }
    KeyNumType Get_KeyGuard() const { return m_KeyGuard; }
    KeyNumType Get_KeyScatter() const { return m_KeyScatter; }
    KeyNumType Get_KeyQueueMove1() const { return m_KeyQueueMove1; }
    KeyNumType Get_KeyQueueMove2() const { return m_KeyQueueMove2; }

    KeyNumType Get_KeyEditorToggle() const { return m_KeyEditorToggle; }

#if defined(CHRONOSHIFT_DEBUG)
    KeyNumType Get_DebugKeyToggleDebug() const { return m_DebugKeyToggleDebug; }
    KeyNumType Get_DebugKeyToggleVortex() const { return m_DebugKeyToggleVortex; }
    KeyNumType Get_DebugKeyForceRedraw() const { return m_DebugKeyForceRedraw; }
    KeyNumType Get_DebugKeyRandomCrate() const { return m_DebugKeyRandomCrate; }
    KeyNumType Get_DebugKeyRandomCrateAtMouse() const { return m_DebugKeyRandomCrateAtMouse; }
    KeyNumType Get_DebugKeyFreeMoney() const { return m_DebugKeyFreeMoney; }
    KeyNumType Get_DebugKeyForceWin() const { return m_DebugKeyForceWin; }
    KeyNumType Get_DebugKeyForceLose() const { return m_DebugKeyForceLose; }
    KeyNumType Get_DebugKeyForceDie() const { return m_DebugKeyForceDie; }
    KeyNumType Get_DebugKeyBailOut() const { return m_DebugKeyBailOut; }
    KeyNumType Get_DebugKeyCellIcon() const { return m_DebugKeyCellIcon; }
    KeyNumType Get_DebugKeyUnshroud() const { return m_DebugKeyUnshroud; }
    KeyNumType Get_DebugKeySightRange() const { return m_DebugKeySightRange; }
    KeyNumType Get_DebugKeyGuardRange() const { return m_DebugKeyGuardRange; }
    KeyNumType Get_DebugKeyWeaponRange() const { return m_DebugKeyWeaponRange; }
    KeyNumType Get_DebugKeyAttackFriendlies() const { return m_DebugKeyAttackFriendlies; }
    KeyNumType Get_DebugKeyFindPath() const { return m_DebugKeyFindPath; }
    KeyNumType Get_DebugKeyNavList() const { return m_DebugKeyNavList; }
    KeyNumType Get_DebugKeyToggleMono() const { return m_DebugKeyToggleMono; }
    KeyNumType Get_DebugKeyPrevMonoPage() const { return m_DebugKeyPrevMonoPage; }
    KeyNumType Get_DebugKeyNextMonoPage() const { return m_DebugKeyNextMonoPage; }
    KeyNumType Get_DebugKeyPaused() const { return m_DebugKeyPaused; }
    KeyNumType Get_DebugKeyFrameStep() const { return m_DebugKeyFrameStep; }
    KeyNumType Get_DebugKeyOneFrameStep() const { return m_DebugKeyOneFrameStep; }
    KeyNumType Get_DebugKeyFiveFrameStep() const { return m_DebugKeyFiveFrameStep; }
    KeyNumType Get_DebugKeyTenFrameStep() const { return m_DebugKeyTenFrameStep; }
    KeyNumType Get_DebugKeySpawnMCV() const { return m_DebugKeySpawnMCV; }
    KeyNumType Get_DebugKeySpawnHarvester() const { return m_DebugKeySpawnHarvester; }
    KeyNumType Get_DebugKeySpawnHeli() const { return m_DebugKeySpawnHeli; }
    KeyNumType Get_DebugKeySpawnHind() const { return m_DebugKeySpawnHind; }
    KeyNumType Get_DebugKeySpawnTransport() const { return m_DebugKeySpawnTransport; }
    KeyNumType Get_DebugKeyAtomExplosion() const { return m_DebugKeyAtomExplosion; }
    KeyNumType Get_DebugKeySuperExplosion() const { return m_DebugKeySuperExplosion; }
    KeyNumType Get_DebugKeyExplosion() const { return m_DebugKeyExplosion; }
    KeyNumType Get_DebugKeyDeleteObject() const { return m_DebugKeyDeleteObject; }
    KeyNumType Get_DebugKeyScreenshot() const { return m_DebugKeyScreenshot; }
    KeyNumType Get_DebugKeyMotionCapture() const { return m_DebugKeyMotionCapture; }
    KeyNumType Get_DebugKeyThemeNext() const { return m_DebugKeyThemeNext; }
    KeyNumType Get_DebugKeyThemePrev() const { return m_DebugKeyThemePrev; }
    KeyNumType Get_DebugKeySpecialSonarPulse() const { return m_DebugKeySpecialSonarPulse; }
    KeyNumType Get_DebugKeySpecialAtomBomb() const { return m_DebugKeySpecialAtomBomb; }
    KeyNumType Get_DebugKeySpecialWarpSphere() const { return m_DebugKeySpecialWarpSphere; }
    KeyNumType Get_DebugKeySpecialParaBomb() const { return m_DebugKeySpecialParaBomb; }
    KeyNumType Get_DebugKeySpecialParaInfantry() const { return m_DebugKeySpecialParaInfantry; }
    KeyNumType Get_DebugKeySpecialSpyPlane() const { return m_DebugKeySpecialSpyPlane; }
    KeyNumType Get_DebugKeySpecialIronCurtain() const { return m_DebugKeySpecialIronCurtain; }
    KeyNumType Get_DebugKeySpecialGPS() const { return m_DebugKeySpecialGPS; }
    KeyNumType Get_DebugKeyInstantBuild() const { return m_DebugKeyInstantBuild; }
    KeyNumType Get_DebugKeyBuildCheat() const { return m_DebugKeyBuildCheat; }
    KeyNumType Get_DebugKeyAIControl() const { return m_DebugKeyAIControl; }
    KeyNumType Get_DebugKeyStealObject() const { return m_DebugKeyStealObject; }
    KeyNumType Get_DebugKeyToggleDamage() const { return m_DebugKeyToggleDamage; }
    KeyNumType Get_DebugKeyToggleCloakable() const { return m_DebugKeyToggleCloakable; }
    KeyNumType Get_DebugKeyApplyDamage() const { return m_DebugKeyApplyDamage; }
    KeyNumType Get_DebugKeyToggleFullMap() const { return m_DebugKeyToggleFullMap; }
    KeyNumType Get_DebugKeySpecialDialog() const { return m_DebugKeySpecialDialog; }
#endif

private:
    int m_GameSpeed;
    int m_ScrollRate;
    fixed_t m_Volume;
    fixed_t m_ScoreVolume;
    fixed_t m_MultiplayerScoreVolume;
    fixed_t m_Brightness;
    fixed_t m_Tint;
    fixed_t m_Saturation;
    fixed_t m_Contrast;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_AutoScroll : 1; // & 1
    BOOL m_ScoreRepeats : 1; // & 2
    BOOL m_ScoreShuffles : 1; // & 4
    BOOL m_PaletteScroll : 1; // & 8
    BOOL m_FreeScrolling : 1; // & 16 Chronoshift option.
    BOOL m_DeathAnnounce : 1; // & 32 Chronoshift option.
    BOOL m_AllowSidebarToggle : 1; // & 64 Chronoshift option.
    BOOL m_CounterstrikeEnabled : 1; // & 128 Chronoshift option.
    BOOL m_AftermathEnabled : 1; // & 256 Chronoshift option.
    BOOL m_ExpansionOptionsPresent : 1; // & 512 Chronoshift option.
#else
    bool m_AutoScroll;
    bool m_ScoreRepeats;
    bool m_ScoreShuffles;
    bool m_PaletteScroll;
    bool m_FreeScrolling; // Chronoshift option.
    bool m_DeathAnnounce; // Chronoshift option.
    bool m_AllowSidebarToggle; // Chronoshift option.
    bool m_CounterstrikeEnabled; // Chronoshift option.
    bool m_AftermathEnabled; // Chronoshift option.
    bool m_ExpansionOptionsPresent; // Chronoshift option.
#endif
    KeyNumType m_KeyForceMove1;
    KeyNumType m_KeyForceMove2;
    KeyNumType m_KeyForceAttack1;
    KeyNumType m_KeyForceAttack2;
    KeyNumType m_KeySelect1;
    KeyNumType m_KeySelect2;
    KeyNumType m_KeyScatter;
    KeyNumType m_KeyStop;
    KeyNumType m_KeyGuard;
    KeyNumType m_KeyNext;
    KeyNumType m_KeyPrevious;
    KeyNumType m_KeyFormation;
    KeyNumType m_KeyHome1;
    KeyNumType m_KeyHome2;
    KeyNumType m_KeyBase;
    KeyNumType m_KeyResign;
    KeyNumType m_KeyAlliance;
    KeyNumType m_KeyBookmark1;
    KeyNumType m_KeyBookmark2;
    KeyNumType m_KeyBookmark3;
    KeyNumType m_KeyBookmark4;
    KeyNumType m_KeySelectView;
    KeyNumType m_KeyRepairToggle;
    KeyNumType m_KeyRepairOn;
    KeyNumType m_KeyRepairOff;
    KeyNumType m_KeySellToggle;
    KeyNumType m_KeySellOn;
    KeyNumType m_KeySellOff;
    KeyNumType m_KeyMapToggle;
    KeyNumType m_KeySidebarUp;
    KeyNumType m_KeySidebarDown;
    KeyNumType m_KeyOption1;
    KeyNumType m_KeyOption2;
    KeyNumType m_KeyScrollLeft;
    KeyNumType m_KeyScrollRight;
    KeyNumType m_KeyScrollUp;
    KeyNumType m_KeyScrollDown;
    KeyNumType m_KeyQueueMove1;
    KeyNumType m_KeyQueueMove2;
    KeyNumType m_KeyTeam1;
    KeyNumType m_KeyTeam2;
    KeyNumType m_KeyTeam3;
    KeyNumType m_KeyTeam4;
    KeyNumType m_KeyTeam5;
    KeyNumType m_KeyTeam6;
    KeyNumType m_KeyTeam7;
    KeyNumType m_KeyTeam8;
    KeyNumType m_KeyTeam9;
    KeyNumType m_KeyTeam10;

    static KeyNumType m_KeyEditorToggle;

#if defined(CHRONOSHIFT_DEBUG)
#if defined(GAME_DLL)
    // For now, these need to be static as they will increase the size of the class.
    static KeyNumType m_DebugKeyToggleDebug;
    static KeyNumType m_DebugKeyToggleVortex;
    static KeyNumType m_DebugKeyForceRedraw;
    static KeyNumType m_DebugKeyRandomCrate;
    static KeyNumType m_DebugKeyRandomCrateAtMouse;
    static KeyNumType m_DebugKeyFreeMoney;
    static KeyNumType m_DebugKeyForceWin;
    static KeyNumType m_DebugKeyForceLose;
    static KeyNumType m_DebugKeyForceDie;
    static KeyNumType m_DebugKeyBailOut;
    static KeyNumType m_DebugKeyCellIcon;
    static KeyNumType m_DebugKeyUnshroud;
    static KeyNumType m_DebugKeySightRange;
    static KeyNumType m_DebugKeyGuardRange;
    static KeyNumType m_DebugKeyWeaponRange;
    static KeyNumType m_DebugKeyAttackFriendlies;
    static KeyNumType m_DebugKeyFindPath;
    static KeyNumType m_DebugKeyNavList;
    static KeyNumType m_DebugKeyToggleMono;
    static KeyNumType m_DebugKeyPrevMonoPage;
    static KeyNumType m_DebugKeyNextMonoPage;
    static KeyNumType m_DebugKeyPaused;
    static KeyNumType m_DebugKeyFrameStep;
    static KeyNumType m_DebugKeyOneFrameStep;
    static KeyNumType m_DebugKeyFiveFrameStep;
    static KeyNumType m_DebugKeyTenFrameStep;
    static KeyNumType m_DebugKeySpawnMCV;
    static KeyNumType m_DebugKeySpawnHarvester;
    static KeyNumType m_DebugKeySpawnHeli;
    static KeyNumType m_DebugKeySpawnHind;
    static KeyNumType m_DebugKeySpawnTransport;
    static KeyNumType m_DebugKeyAtomExplosion;
    static KeyNumType m_DebugKeySuperExplosion;
    static KeyNumType m_DebugKeyExplosion;
    static KeyNumType m_DebugKeyDeleteObject;
    static KeyNumType m_DebugKeyScreenshot;
    static KeyNumType m_DebugKeyMotionCapture;
    static KeyNumType m_DebugKeyThemeNext;
    static KeyNumType m_DebugKeyThemePrev;
    static KeyNumType m_DebugKeySpecialSonarPulse;
    static KeyNumType m_DebugKeySpecialAtomBomb;
    static KeyNumType m_DebugKeySpecialWarpSphere;
    static KeyNumType m_DebugKeySpecialParaBomb;
    static KeyNumType m_DebugKeySpecialParaInfantry;
    static KeyNumType m_DebugKeySpecialSpyPlane;
    static KeyNumType m_DebugKeySpecialIronCurtain;
    static KeyNumType m_DebugKeySpecialGPS;
    static KeyNumType m_DebugKeyInstantBuild;
    static KeyNumType m_DebugKeyBuildCheat;
    static KeyNumType m_DebugKeyAIControl;
    static KeyNumType m_DebugKeyStealObject;
    static KeyNumType m_DebugKeyToggleDamage;
    static KeyNumType m_DebugKeyToggleCloakable;
    static KeyNumType m_DebugKeyApplyDamage;
    static KeyNumType m_DebugKeyToggleFullMap;
    static KeyNumType m_DebugKeySpecialDialog;
#else // GAME_DLL
    KeyNumType m_DebugKeyToggleDebug;
    KeyNumType m_DebugKeyToggleVortex;
    KeyNumType m_DebugKeyForceRedraw;
    KeyNumType m_DebugKeyRandomCrate;
    KeyNumType m_DebugKeyRandomCrateAtMouse;
    KeyNumType m_DebugKeyFreeMoney;
    KeyNumType m_DebugKeyForceWin;
    KeyNumType m_DebugKeyForceLose;
    KeyNumType m_DebugKeyForceDie;
    KeyNumType m_DebugKeyBailOut;
    KeyNumType m_DebugKeyCellIcon;
    KeyNumType m_DebugKeyUnshroud;
    KeyNumType m_DebugKeySightRange;
    KeyNumType m_DebugKeyGuardRange;
    KeyNumType m_DebugKeyWeaponRange;
    KeyNumType m_DebugKeyAttackFriendlies;
    KeyNumType m_DebugKeyFindPath;
    KeyNumType m_DebugKeyNavList;
    KeyNumType m_DebugKeyToggleMono;
    KeyNumType m_DebugKeyPrevMonoPage;
    KeyNumType m_DebugKeyNextMonoPage;
    KeyNumType m_DebugKeyPaused;
    KeyNumType m_DebugKeyFrameStep;
    KeyNumType m_DebugKeyOneFrameStep;
    KeyNumType m_DebugKeyFiveFrameStep;
    KeyNumType m_DebugKeyTenFrameStep;
    KeyNumType m_DebugKeySpawnMCV;
    KeyNumType m_DebugKeySpawnHarvester;
    KeyNumType m_DebugKeySpawnHeli;
    KeyNumType m_DebugKeySpawnHind;
    KeyNumType m_DebugKeySpawnTransport;
    KeyNumType m_DebugKeyAtomExplosion;
    KeyNumType m_DebugKeySuperExplosion;
    KeyNumType m_DebugKeyExplosion;
    KeyNumType m_DebugKeyDeleteObject;
    KeyNumType m_DebugKeyScreenshot;
    KeyNumType m_DebugKeyMotionCapture;
    KeyNumType m_DebugKeyThemeNext;
    KeyNumType m_DebugKeyThemePrev;
    KeyNumType m_DebugKeySpecialSonarPulse;
    KeyNumType m_DebugKeySpecialAtomBomb;
    KeyNumType m_DebugKeySpecialWarpSphere;
    KeyNumType m_DebugKeySpecialParaBomb;
    KeyNumType m_DebugKeySpecialParaInfantry;
    KeyNumType m_DebugKeySpecialSpyPlane;
    KeyNumType m_DebugKeySpecialIronCurtain;
    KeyNumType m_DebugKeySpecialGPS;
    KeyNumType m_DebugKeyInstantBuild;
    KeyNumType m_DebugKeyBuildCheat;
    KeyNumType m_DebugKeyAIControl;
    KeyNumType m_DebugKeyStealObject;
    KeyNumType m_DebugKeyToggleDamage;
    KeyNumType m_DebugKeyToggleCloakable;
    KeyNumType m_DebugKeyApplyDamage;
    KeyNumType m_DebugKeyToggleFullMap;
    KeyNumType m_DebugKeySpecialDialog;
#endif // GAME_DLL
#endif // CHRONOSHIFT_DEBUG
};

#endif // OPTIONS_H
