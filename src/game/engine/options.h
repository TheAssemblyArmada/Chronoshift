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
        PaletteClass &src, PaletteClass &dst, fixed_t brightness, fixed_t saturation, fixed_t tint, fixed_t contrast) const;
    void Fixup_Palette() const;
    int Normalize_Volume(int volume) const { return volume * Volume; }
    int Normalize_Delay(int delay) const;
    void Set_Shuffle(BOOL shuffle) { ScoreShuffles = shuffle; }
    void Set_Repeat(BOOL repeat) { ScoreRepeats = repeat; }
    void Set_Score_Volume(fixed_t volume, BOOL beep = false);
    void Set_Sound_Volume(fixed_t volume, BOOL beep = false);
    void Set_Brightness(fixed_t brightness);
    void Set_Saturation(fixed_t saturation);
    void Set_Contrast(fixed_t contrast);
    void Set_Tint(fixed_t tint);
    void Set();

    int Game_Speed() const { return GameSpeed; }
    int Get_Scroll_Rate() const { return ScrollRate; }
    const fixed_t &Get_Score_Volume() const { return ScoreVolume; }
    BOOL Get_Auto_Scroll() const { return AutoScroll; }
    BOOL Get_Repeat() const { return ScoreRepeats; }
    BOOL Get_Shuffle() const { return ScoreShuffles; }
    BOOL Sidebar_Toggle_Allowed() const { return AllowSidebarToggle; }
    BOOL Free_Scrolling() const { return FreeScrolling; }
    BOOL Is_Counterstrike_Enabled() const { return CounterstrikeEnabled; }
    BOOL Is_Aftermath_Enabled() const { return AftermathEnabled; }
    fixed_t Get_Brightness() const { return (Brightness - fixed_t::_1_4) / fixed_t::_1_2; }
    fixed_t Get_Saturation() const { return Saturation; }
    fixed_t Get_Contrast() const { return (Contrast - fixed_t::_1_4) / fixed_t::_1_2; }
    fixed_t Get_Tint() const { return Tint; }

    KeyNumType Get_KeySelectView() const { return KeySelectView; }
    KeyNumType Get_KeyRepairToggle() const { return KeyRepairToggle; }
    KeyNumType Get_KeySellToggle() const { return KeySellToggle; }
    KeyNumType Get_KeyMapToggle() const { return KeyMapToggle; }
    KeyNumType Get_KeySidebarUp() const { return KeySidebarUp; }
    KeyNumType Get_KeySidebarDown() const { return KeySidebarDown; }
    KeyNumType Get_KeyOption1() const { return KeyOption1; }
    KeyNumType Get_KeyOption2() const { return KeyOption2; }
    KeyNumType Get_KeyScrollLeft() const { return KeyScrollLeft; }
    KeyNumType Get_KeyScrollRight() const { return KeyScrollRight; }
    KeyNumType Get_KeyScrollUp() const { return KeyScrollUp; }
    KeyNumType Get_KeyScrollDown() const { return KeyScrollDown; }
    KeyNumType Get_KeyTeam1() const { return KeyTeam1; }
    KeyNumType Get_KeyTeam2() const { return KeyTeam2; }
    KeyNumType Get_KeyTeam3() const { return KeyTeam3; }
    KeyNumType Get_KeyTeam4() const { return KeyTeam4; }
    KeyNumType Get_KeyTeam5() const { return KeyTeam5; }
    KeyNumType Get_KeyTeam6() const { return KeyTeam6; }
    KeyNumType Get_KeyTeam7() const { return KeyTeam7; }
    KeyNumType Get_KeyTeam8() const { return KeyTeam8; }
    KeyNumType Get_KeyTeam9() const { return KeyTeam9; }
    KeyNumType Get_KeyTeam10() const { return KeyTeam10; }
    KeyNumType Get_KeyBookmark1() const { return KeyBookmark1; }
    KeyNumType Get_KeyBookmark2() const { return KeyBookmark2; }
    KeyNumType Get_KeyBookmark3() const { return KeyBookmark3; }
    KeyNumType Get_KeyBookmark4() const { return KeyBookmark4; }
    KeyNumType Get_KeyNext() const { return KeyNext; }
    KeyNumType Get_KeyPrevious() const { return KeyPrevious; }
    KeyNumType Get_KeyAlliance() const { return KeyAlliance; }
    KeyNumType Get_KeyFormation() const { return KeyFormation; }
    KeyNumType Get_KeyBase() const { return KeyBase; }
    KeyNumType Get_KeyHome1() const { return KeyHome1; }
    KeyNumType Get_KeyHome2() const { return KeyHome2; }
    KeyNumType Get_KeyStop() const { return KeyStop; }
    KeyNumType Get_KeyGuard() const { return KeyGuard; }
    KeyNumType Get_KeyScatter() const { return KeyScatter; }

#if defined(CHRONOSHIFT_DEBUG)
    KeyNumType Get_DebugKeyToggleDebug() const { return DebugKeyToggleDebug; }
    KeyNumType Get_DebugKeyToggleVortex() const { return DebugKeyToggleVortex; }
    KeyNumType Get_DebugKeyForceRedraw() const { return DebugKeyForceRedraw; }
    KeyNumType Get_DebugKeyRandomCrate() const { return DebugKeyRandomCrate; }
    KeyNumType Get_DebugKeyRandomCrateAtMouse() const { return DebugKeyRandomCrateAtMouse; }
    KeyNumType Get_DebugKeyFreeMoney() const { return DebugKeyFreeMoney; }
    KeyNumType Get_DebugKeyForceWin() const { return DebugKeyForceWin; }
    KeyNumType Get_DebugKeyForceLose() const { return DebugKeyForceLose; }
    KeyNumType Get_DebugKeyForceDie() const { return DebugKeyForceDie; }
    KeyNumType Get_DebugKeyBailOut() const { return DebugKeyBailOut; }
    KeyNumType Get_DebugKeyCellIcon() const { return DebugKeyCellIcon; }
    KeyNumType Get_DebugKeyUnshroud() const { return DebugKeyUnshroud; }
    KeyNumType Get_DebugKeySightRange() const { return DebugKeySightRange; }
    KeyNumType Get_DebugKeyGuardRange() const { return DebugKeyGuardRange; }
    KeyNumType Get_DebugKeyWeaponRange() const { return DebugKeyWeaponRange; }
    KeyNumType Get_DebugKeyAttackFriendlies() const { return DebugKeyAttackFriendlies; }
    KeyNumType Get_DebugKeyFindPath() const { return DebugKeyFindPath; }
    KeyNumType Get_DebugKeyNavList() const { return DebugKeyNavList; }
    KeyNumType Get_DebugKeyToggleMono() const { return DebugKeyToggleMono; }
    KeyNumType Get_DebugKeyPrevMonoPage() const { return DebugKeyPrevMonoPage; }
    KeyNumType Get_DebugKeyNextMonoPage() const { return DebugKeyNextMonoPage; }
    KeyNumType Get_DebugKeyPaused() const { return DebugKeyPaused; }
    KeyNumType Get_DebugKeyFrameStep() const { return DebugKeyFrameStep; }
    KeyNumType Get_DebugKeyOneFrameStep() const { return DebugKeyOneFrameStep; }
    KeyNumType Get_DebugKeyFiveFrameStep() const { return DebugKeyFiveFrameStep; }
    KeyNumType Get_DebugKeyTenFrameStep() const { return DebugKeyTenFrameStep; }
    KeyNumType Get_DebugKeySpawnMCV() const { return DebugKeySpawnMCV; }
    KeyNumType Get_DebugKeySpawnHarvester() const { return DebugKeySpawnHarvester; }
    KeyNumType Get_DebugKeySpawnHeli() const { return DebugKeySpawnHeli; }
    KeyNumType Get_DebugKeySpawnHind() const { return DebugKeySpawnHind; }
    KeyNumType Get_DebugKeySpawnTransport() const { return DebugKeySpawnTransport; }
    KeyNumType Get_DebugKeyAtomExplosion() const { return DebugKeyAtomExplosion; }
    KeyNumType Get_DebugKeySuperExplosion() const { return DebugKeySuperExplosion; }
    KeyNumType Get_DebugKeyExplosion() const { return DebugKeyExplosion; }
    KeyNumType Get_DebugKeyDeleteObject() const { return DebugKeyDeleteObject; }
    KeyNumType Get_DebugKeyScreenshot() const { return DebugKeyScreenshot; }
    KeyNumType Get_DebugKeyMotionCapture() const { return DebugKeyMotionCapture; }
    KeyNumType Get_DebugKeyThemeNext() const { return DebugKeyThemeNext; }
    KeyNumType Get_DebugKeyThemePrev() const { return DebugKeyThemePrev; }
    KeyNumType Get_DebugKeySpecialSonarPulse() const { return DebugKeySpecialSonarPulse; }
    KeyNumType Get_DebugKeySpecialAtomBomb() const { return DebugKeySpecialAtomBomb; }
    KeyNumType Get_DebugKeySpecialWarpSphere() const { return DebugKeySpecialWarpSphere; }
    KeyNumType Get_DebugKeySpecialParaBomb() const { return DebugKeySpecialParaBomb; }
    KeyNumType Get_DebugKeySpecialParaInfantry() const { return DebugKeySpecialParaInfantry; }
    KeyNumType Get_DebugKeySpecialSpyPlane() const { return DebugKeySpecialSpyPlane; }
    KeyNumType Get_DebugKeySpecialIronCurtain() const { return DebugKeySpecialIronCurtain; }
    KeyNumType Get_DebugKeySpecialGPS() const { return DebugKeySpecialGPS; }
    KeyNumType Get_DebugKeyInstantBuild() const { return DebugKeyInstantBuild; }
    KeyNumType Get_DebugKeyBuildCheat() const { return DebugKeyBuildCheat; }
    KeyNumType Get_DebugKeyAIControl() const { return DebugKeyAIControl; }
    KeyNumType Get_DebugKeyStealObject() const { return DebugKeyStealObject; }
    KeyNumType Get_DebugKeyToggleDamage() const { return DebugKeyToggleDamage; }
    KeyNumType Get_DebugKeyToggleCloakable() const { return DebugKeyToggleCloakable; }
    KeyNumType Get_DebugKeyApplyDamage() const { return DebugKeyApplyDamage; }
    KeyNumType Get_DebugKeyToggleFullMap() const { return DebugKeyToggleFullMap; }
    KeyNumType Get_DebugKeySpecialDialog() const { return DebugKeySpecialDialog; }
#endif

#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
#endif

private:
    int GameSpeed;
    int ScrollRate;
    fixed_t Volume;
    fixed_t ScoreVolume;
    fixed_t MultiplayerScoreVolume;
    fixed_t Brightness;
    fixed_t Tint;
    fixed_t Saturation;
    fixed_t Contrast;
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
            bool FreeScrolling : 1; // & 16 Chronoshift option.
            bool DeathAnnounce : 1; // & 32 Chronoshift option.
            bool AllowSidebarToggle : 1; // & 64 Chronoshift option.
            bool CounterstrikeEnabled : 1; // & 128 Chronoshift option.
            bool AftermathEnabled : 1; // & 256 Chronoshift option.
        };
        int Bitfield;
    };
#else
    bool AutoScroll;
    bool ScoreRepeats;
    bool ScoreShuffles;
    bool PaletteScroll;
    bool FreeScrolling; // Chronoshift option.
    bool DeathAnnounce; // Chronoshift option.
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

#if defined(CHRONOSHIFT_DEBUG)
#if !defined(CHRONOSHIFT_STANDALONE)
    // For now, these need to be static as they will increase the size of the class.
    static KeyNumType DebugKeyToggleDebug;
    static KeyNumType DebugKeyToggleVortex;
    static KeyNumType DebugKeyForceRedraw;
    static KeyNumType DebugKeyRandomCrate;
    static KeyNumType DebugKeyRandomCrateAtMouse;
    static KeyNumType DebugKeyFreeMoney;
    static KeyNumType DebugKeyForceWin;
    static KeyNumType DebugKeyForceLose;
    static KeyNumType DebugKeyForceDie;
    static KeyNumType DebugKeyBailOut;
    static KeyNumType DebugKeyCellIcon;
    static KeyNumType DebugKeyUnshroud;
    static KeyNumType DebugKeySightRange;
    static KeyNumType DebugKeyGuardRange;
    static KeyNumType DebugKeyWeaponRange;
    static KeyNumType DebugKeyAttackFriendlies;
    static KeyNumType DebugKeyFindPath;
    static KeyNumType DebugKeyNavList;
    static KeyNumType DebugKeyToggleMono;
    static KeyNumType DebugKeyPrevMonoPage;
    static KeyNumType DebugKeyNextMonoPage;
    static KeyNumType DebugKeyPaused;
    static KeyNumType DebugKeyFrameStep;
    static KeyNumType DebugKeyOneFrameStep;
    static KeyNumType DebugKeyFiveFrameStep;
    static KeyNumType DebugKeyTenFrameStep;
    static KeyNumType DebugKeySpawnMCV;
    static KeyNumType DebugKeySpawnHarvester;
    static KeyNumType DebugKeySpawnHeli;
    static KeyNumType DebugKeySpawnHind;
    static KeyNumType DebugKeySpawnTransport;
    static KeyNumType DebugKeyAtomExplosion;
    static KeyNumType DebugKeySuperExplosion;
    static KeyNumType DebugKeyExplosion;
    static KeyNumType DebugKeyDeleteObject;
    static KeyNumType DebugKeyScreenshot;
    static KeyNumType DebugKeyMotionCapture;
    static KeyNumType DebugKeyThemeNext;
    static KeyNumType DebugKeyThemePrev;
    static KeyNumType DebugKeySpecialSonarPulse;
    static KeyNumType DebugKeySpecialAtomBomb;
    static KeyNumType DebugKeySpecialWarpSphere;
    static KeyNumType DebugKeySpecialParaBomb;
    static KeyNumType DebugKeySpecialParaInfantry;
    static KeyNumType DebugKeySpecialSpyPlane;
    static KeyNumType DebugKeySpecialIronCurtain;
    static KeyNumType DebugKeySpecialGPS;
    static KeyNumType DebugKeyInstantBuild;
    static KeyNumType DebugKeyBuildCheat;
    static KeyNumType DebugKeyAIControl;
    static KeyNumType DebugKeyStealObject;
    static KeyNumType DebugKeyToggleDamage;
    static KeyNumType DebugKeyToggleCloakable;
    static KeyNumType DebugKeyApplyDamage;
    static KeyNumType DebugKeyToggleFullMap;
    static KeyNumType DebugKeySpecialDialog;
#else // CHRONOSHIFT_STANDALONE
    KeyNumType DebugKeyToggleDebug;
    KeyNumType DebugKeyToggleVortex;
    KeyNumType DebugKeyForceRedraw;
    KeyNumType DebugKeyRandomCrate;
    KeyNumType DebugKeyRandomCrateAtMouse;
    KeyNumType DebugKeyFreeMoney;
    KeyNumType DebugKeyForceWin;
    KeyNumType DebugKeyForceLose;
    KeyNumType DebugKeyForceDie;
    KeyNumType DebugKeyBailOut;
    KeyNumType DebugKeyCellIcon;
    KeyNumType DebugKeyUnshroud;
    KeyNumType DebugKeySightRange;
    KeyNumType DebugKeyGuardRange;
    KeyNumType DebugKeyWeaponRange;
    KeyNumType DebugKeyAttackFriendlies;
    KeyNumType DebugKeyFindPath;
    KeyNumType DebugKeyNavList;
    KeyNumType DebugKeyToggleMono;
    KeyNumType DebugKeyPrevMonoPage;
    KeyNumType DebugKeyNextMonoPage;
    KeyNumType DebugKeyPaused;
    KeyNumType DebugKeyFrameStep;
    KeyNumType DebugKeyOneFrameStep;
    KeyNumType DebugKeyFiveFrameStep;
    KeyNumType DebugKeyTenFrameStep;
    KeyNumType DebugKeySpawnMCV;
    KeyNumType DebugKeySpawnHarvester;
    KeyNumType DebugKeySpawnHeli;
    KeyNumType DebugKeySpawnHind;
    KeyNumType DebugKeySpawnTransport;
    KeyNumType DebugKeyAtomExplosion;
    KeyNumType DebugKeySuperExplosion;
    KeyNumType DebugKeyExplosion;
    KeyNumType DebugKeyDeleteObject;
    KeyNumType DebugKeyScreenshot;
    KeyNumType DebugKeyMotionCapture;
    KeyNumType DebugKeyThemeNext;
    KeyNumType DebugKeyThemePrev;
    KeyNumType DebugKeySpecialSonarPulse;
    KeyNumType DebugKeySpecialAtomBomb;
    KeyNumType DebugKeySpecialWarpSphere;
    KeyNumType DebugKeySpecialParaBomb;
    KeyNumType DebugKeySpecialParaInfantry;
    KeyNumType DebugKeySpecialSpyPlane;
    KeyNumType DebugKeySpecialIronCurtain;
    KeyNumType DebugKeySpecialGPS;
    KeyNumType DebugKeyInstantBuild;
    KeyNumType DebugKeyBuildCheat;
    KeyNumType DebugKeyAIControl;
    KeyNumType DebugKeyStealObject;
    KeyNumType DebugKeyToggleDamage;
    KeyNumType DebugKeyToggleCloakable;
    KeyNumType DebugKeyApplyDamage;
    KeyNumType DebugKeyToggleFullMap;
    KeyNumType DebugKeySpecialDialog;
#endif // !CHRONOSHIFT_STANDALONE
#endif // CHRONOSHIFT_DEBUG
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
