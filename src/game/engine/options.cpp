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
#include "expansion.h"
#include "gamefile.h"
#include "gameini.h"
#include "globals.h"
#include "hsv.h"
#include "palette.h"
#include "session.h"
#include "voc.h"

#if defined(GAME_DLL) && defined(CHRONOSHIFT_DEBUG)
KeyNumType OptionsClass::m_DebugKeyToggleDebug = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyToggleVortex = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyForceRedraw = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyRandomCrate = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyRandomCrateAtMouse = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyFreeMoney = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyForceWin = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyForceLose = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyForceDie = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyBailOut = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyCellIcon = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyUnshroud = KN_NONE;
KeyNumType OptionsClass::m_DebugKeySightRange = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyGuardRange = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyWeaponRange = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyAttackFriendlies = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyFindPath = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyNavList = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyToggleMono = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyPrevMonoPage = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyNextMonoPage = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyPaused = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyFrameStep = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyOneFrameStep = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyFiveFrameStep = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyTenFrameStep = KN_NONE;
KeyNumType OptionsClass::m_DebugKeySpawnMCV = KN_NONE;
KeyNumType OptionsClass::m_DebugKeySpawnHarvester = KN_NONE;
KeyNumType OptionsClass::m_DebugKeySpawnHeli = KN_NONE;
KeyNumType OptionsClass::m_DebugKeySpawnHind = KN_NONE;
KeyNumType OptionsClass::m_DebugKeySpawnTransport = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyAtomExplosion = KN_NONE;
KeyNumType OptionsClass::m_DebugKeySuperExplosion = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyExplosion = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyDeleteObject = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyScreenshot = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyMotionCapture = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyThemeNext = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyThemePrev = KN_NONE;
KeyNumType OptionsClass::m_DebugKeySpecialSonarPulse = KN_NONE;
KeyNumType OptionsClass::m_DebugKeySpecialAtomBomb = KN_NONE;
KeyNumType OptionsClass::m_DebugKeySpecialWarpSphere = KN_NONE;
KeyNumType OptionsClass::m_DebugKeySpecialParaBomb = KN_NONE;
KeyNumType OptionsClass::m_DebugKeySpecialParaInfantry = KN_NONE;
KeyNumType OptionsClass::m_DebugKeySpecialSpyPlane = KN_NONE;
KeyNumType OptionsClass::m_DebugKeySpecialIronCurtain = KN_NONE;
KeyNumType OptionsClass::m_DebugKeySpecialGPS = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyInstantBuild = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyBuildCheat = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyAIControl = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyStealObject = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyToggleDamage = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyToggleCloakable = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyApplyDamage = KN_NONE;
KeyNumType OptionsClass::m_DebugKeyToggleFullMap = KN_NONE;
KeyNumType OptionsClass::m_DebugKeySpecialDialog = KN_NONE;
#endif

KeyNumType OptionsClass::m_KeyEditorToggle = KN_NONE;

OptionsClass::OptionsClass() :
    m_GameSpeed(GAMESPEED_3),
    m_ScrollRate(SCROLLSPEED_3),
    m_Volume(".40"),
    m_ScoreVolume(".25"),
    m_MultiplayerScoreVolume("0"),
    m_Brightness(fixed_t::_1_2),
    m_Tint(fixed_t::_1_2),
    m_Saturation(fixed_t::_1_2),
    m_Contrast(fixed_t::_1_2),
    m_AutoScroll(true),
    m_ScoreRepeats(false),
    m_ScoreShuffles(false),
    m_PaletteScroll(true),
    m_FreeScrolling(false),
    m_DeathAnnounce(true),
    m_AllowSidebarToggle(false),
    m_CounterstrikeEnabled(false),
    m_AftermathEnabled(false),
    m_ExpansionOptionsPresent(false),
    m_KeyForceMove1(KN_LALT),
    m_KeyForceMove2(KN_RALT),
    m_KeyForceAttack1(KN_LCTRL),
    m_KeyForceAttack2(KN_RCTRL),
    m_KeySelect1(KN_LSHIFT),
    m_KeySelect2(KN_RSHIFT),
    m_KeyScatter(KN_X),
    m_KeyStop(KN_S),
    m_KeyGuard(KN_G),
    m_KeyNext(KN_N),
    m_KeyPrevious(KN_B),
    m_KeyFormation(KN_F),
    m_KeyHome1(KN_HOME),
    m_KeyHome2(KN_E_HOME),
    m_KeyBase(KN_H),
    m_KeyResign(KN_R),
    m_KeyAlliance(KN_A),
    m_KeyBookmark1(KN_F9),
    m_KeyBookmark2(KN_F10),
    m_KeyBookmark3(KN_F11),
    m_KeyBookmark4(KN_F12),
    m_KeySelectView(KN_E),
    m_KeyRepairToggle(KN_T),
    m_KeyRepairOn(KN_NONE),
    m_KeyRepairOff(KN_NONE),
    m_KeySellToggle(KN_Y),
    m_KeySellOn(KN_NONE),
    m_KeySellOff(KN_NONE),
    m_KeyMapToggle(KN_U),
    m_KeySidebarUp(KN_UP),
    m_KeySidebarDown(KN_DOWN),
    m_KeyOption1(KN_ESC),
    m_KeyOption2(KN_SPACE),
    m_KeyScrollLeft(KN_NONE),
    m_KeyScrollRight(KN_NONE),
    m_KeyScrollUp(KN_NONE),
    m_KeyScrollDown(KN_NONE),
    m_KeyQueueMove1(KN_Q),
    m_KeyQueueMove2(KN_Q),
    m_KeyTeam1(KN_1),
    m_KeyTeam2(KN_2),
    m_KeyTeam3(KN_3),
    m_KeyTeam4(KN_4),
    m_KeyTeam5(KN_5),
    m_KeyTeam6(KN_6),
    m_KeyTeam7(KN_7),
    m_KeyTeam8(KN_8),
    m_KeyTeam9(KN_9),
    m_KeyTeam10(KN_0)
    // m_KeyEditorToggle(KN_NONE),
#if !defined(GAME_DLL) && defined(CHRONOSHIFT_DEBUG)
    ,
    m_DebugKeyToggleDebug(KN_NONE),
    m_DebugKeyToggleVortex(KN_NONE),
    m_DebugKeyForceRedraw(KN_NONE),
    m_DebugKeyRandomCrate(KN_NONE),
    m_DebugKeyRandomCrateAtMouse(KN_NONE),
    m_DebugKeyFreeMoney(KN_NONE),
    m_DebugKeyForceWin(KN_NONE),
    m_DebugKeyForceLose(KN_NONE),
    m_DebugKeyForceDie(KN_NONE),
    m_DebugKeyBailOut(KN_NONE),
    m_DebugKeyCellIcon(KN_NONE),
    m_DebugKeyUnshroud(KN_NONE),
    m_DebugKeySightRange(KN_NONE),
    m_DebugKeyGuardRange(KN_NONE),
    m_DebugKeyWeaponRange(KN_NONE),
    m_DebugKeyAttackFriendlies(KN_NONE),
    m_DebugKeyFindPath(KN_NONE),
    m_DebugKeyNavList(KN_NONE),
    m_DebugKeyToggleMono(KN_NONE),
    m_DebugKeyPrevMonoPage(KN_NONE),
    m_DebugKeyNextMonoPage(KN_NONE),
    m_DebugKeyPaused(KN_NONE),
    m_DebugKeyFrameStep(KN_NONE),
    m_DebugKeyOneFrameStep(KN_NONE),
    m_DebugKeyFiveFrameStep(KN_NONE),
    m_DebugKeyTenFrameStep(KN_NONE),
    m_DebugKeySpawnMCV(KN_NONE),
    m_DebugKeySpawnHarvester(KN_NONE),
    m_DebugKeySpawnHeli(KN_NONE),
    m_DebugKeySpawnHind(KN_NONE),
    m_DebugKeySpawnTransport(KN_NONE),
    m_DebugKeyAtomExplosion(KN_NONE),
    m_DebugKeySuperExplosion(KN_NONE),
    m_DebugKeyExplosion(KN_NONE),
    m_DebugKeyDeleteObject(KN_NONE),
    m_DebugKeyScreenshot(KN_NONE),
    m_DebugKeyMotionCapture(KN_NONE),
    m_DebugKeyThemeNext(KN_NONE),
    m_DebugKeyThemePrev(KN_NONE),
    m_DebugKeySpecialSonarPulse(KN_NONE),
    m_DebugKeySpecialAtomBomb(KN_NONE),
    m_DebugKeySpecialWarpSphere(KN_NONE),
    m_DebugKeySpecialParaBomb(KN_NONE),
    m_DebugKeySpecialParaInfantry(KN_NONE),
    m_DebugKeySpecialSpyPlane(KN_NONE),
    m_DebugKeySpecialIronCurtain(KN_NONE),
    m_DebugKeySpecialGPS(KN_NONE),
    m_DebugKeyInstantBuild(KN_NONE),
    m_DebugKeyBuildCheat(KN_NONE),
    m_DebugKeyAIControl(KN_NONE),
    m_DebugKeyStealObject(KN_NONE),
    m_DebugKeyToggleDamage(KN_NONE),
    m_DebugKeyToggleCloakable(KN_NONE),
    m_DebugKeyApplyDamage(KN_NONE),
    m_DebugKeyToggleFullMap(KN_NONE),
    m_DebugKeySpecialDialog(KN_NONE)
#endif // GAME_DLL && CHRONOSHIFT_DEBUG
{
}

/**
 * @brief Carries out one time tasks for the class.
 *
 * 0x005251A8
 */
void OptionsClass::One_Time()
{
    Set_Score_Vol(m_ScoreVolume * 256);
}

/**
 * @brief Saves the options out to the configuration file (redalert.ini in Red Alert for example).
 *
 * 0x005263A8
 */
void OptionsClass::Save_Settings()
{
    // TODO Some global to handle if the game is TD or RA for later TD support.
    GameFileClass fc(g_SettingsFilename);
    GameINIClass ini;

    if (fc.Is_Available()) {
        ini.Load(fc);
    }

    ini.Put_Int("Options", "GameSpeed", m_GameSpeed);
    ini.Put_Int("Options", "ScrollRate", m_ScrollRate);
    ini.Put_Fixed("Options", "Volume", m_Volume);

    if (g_Session.Game_To_Play() == GAME_CAMPAIGN) {
        ini.Put_Fixed("Options", "ScoreVolume", m_ScoreVolume);
    }

    ini.Put_Fixed("Options", "MultiplayerScoreVolume", m_MultiplayerScoreVolume);
    ini.Put_Fixed("Options", "Brightness", m_Brightness);
    ini.Put_Fixed("Options", "Contrast", m_Contrast);
    ini.Put_Fixed("Options", "Color", m_Saturation);
    ini.Put_Fixed("Options", "Tint", m_Tint);

    ini.Put_Bool("Options", "IsScoreRepeat", m_ScoreRepeats);
    ini.Put_Bool("Options", "IsScoreShuffle", m_ScoreShuffles);
    ini.Put_Bool("Options", "PaletteScroll", m_PaletteScroll);
    ini.Put_Bool("Options", "FreeScrolling", m_FreeScrolling);
    ini.Put_Bool("Options", "DeathAnnounce", m_DeathAnnounce);
    ini.Put_Bool("Options", "AllowSidebarToggle", m_AllowSidebarToggle);

    // Only add the expansion enable bools if having the options at all is enabled.
    if (m_ExpansionOptionsPresent) {
        ini.Put_Bool("Expansions", "CounterstrikeEnabled", m_CounterstrikeEnabled);
        ini.Put_Bool("Expansions", "AftermathEnabled", m_AftermathEnabled);
    }

    ini.Put_KeyNumType("WinHotkeys", "KeyForceMove1", m_KeyForceMove1);
    ini.Put_KeyNumType("WinHotkeys", "KeyForceMove2", m_KeyForceMove2);
    ini.Put_KeyNumType("WinHotkeys", "KeyForceAttack1", m_KeyForceAttack1);
    ini.Put_KeyNumType("WinHotkeys", "KeyForceAttack2", m_KeyForceAttack2);
    ini.Put_KeyNumType("WinHotkeys", "KeySelect1", m_KeySelect1);
    ini.Put_KeyNumType("WinHotkeys", "KeySelect2", m_KeySelect2);
    ini.Put_KeyNumType("WinHotkeys", "KeyScatter", m_KeyScatter);
    ini.Put_KeyNumType("WinHotkeys", "KeyStop", m_KeyStop);
    ini.Put_KeyNumType("WinHotkeys", "KeyGuard", m_KeyGuard);
    ini.Put_KeyNumType("WinHotkeys", "KeyNext", m_KeyNext);
    ini.Put_KeyNumType("WinHotkeys", "KeyPrevious", m_KeyPrevious);
    ini.Put_KeyNumType("WinHotkeys", "KeyFormation", m_KeyFormation);
    ini.Put_KeyNumType("WinHotkeys", "KeyHome1", m_KeyHome1);
    ini.Put_KeyNumType("WinHotkeys", "KeyHome2", m_KeyHome2);
    ini.Put_KeyNumType("WinHotkeys", "KeyBase", m_KeyBase);
    ini.Put_KeyNumType("WinHotkeys", "KeyResign", m_KeyResign);
    ini.Put_KeyNumType("WinHotkeys", "KeyAlliance", m_KeyAlliance);
    ini.Put_KeyNumType("WinHotkeys", "KeyBookmark1", m_KeyBookmark1);
    ini.Put_KeyNumType("WinHotkeys", "KeyBookmark2", m_KeyBookmark2);
    ini.Put_KeyNumType("WinHotkeys", "KeyBookmark3", m_KeyBookmark3);
    ini.Put_KeyNumType("WinHotkeys", "KeyBookmark4", m_KeyBookmark4);
    ini.Put_KeyNumType("WinHotkeys", "KeySelectView", m_KeySelectView);
    ini.Put_KeyNumType("WinHotkeys", "KeyRepairToggle", m_KeyRepairToggle);
    ini.Put_KeyNumType("WinHotkeys", "KeyRepairOn", m_KeyRepairOn);
    ini.Put_KeyNumType("WinHotkeys", "KeyRepairOff", m_KeyRepairOff);
    ini.Put_KeyNumType("WinHotkeys", "KeySellToggle", m_KeySellToggle);
    ini.Put_KeyNumType("WinHotkeys", "KeySellOn", m_KeySellOn);
    ini.Put_KeyNumType("WinHotkeys", "KeySellOff", m_KeySellOff);
    ini.Put_KeyNumType("WinHotkeys", "KeyMapToggle", m_KeyMapToggle);
    ini.Put_KeyNumType("WinHotkeys", "KeySidebarUp", m_KeySidebarUp);
    ini.Put_KeyNumType("WinHotkeys", "KeySidebarDown", m_KeySidebarDown);
    ini.Put_KeyNumType("WinHotkeys", "KeyOption1", m_KeyOption1);
    ini.Put_KeyNumType("WinHotkeys", "KeyOption2", m_KeyOption2);
    ini.Put_KeyNumType("WinHotkeys", "KeyScrollLeft", m_KeyScrollLeft);
    ini.Put_KeyNumType("WinHotkeys", "KeyScrollRight", m_KeyScrollRight);
    ini.Put_KeyNumType("WinHotkeys", "KeyScrollUp", m_KeyScrollUp);
    ini.Put_KeyNumType("WinHotkeys", "KeyScrollDown", m_KeyScrollDown);
    ini.Put_KeyNumType("WinHotkeys", "KeyQueueMove1", m_KeyQueueMove1);
    ini.Put_KeyNumType("WinHotkeys", "KeyQueueMove2", m_KeyQueueMove2);
    ini.Put_KeyNumType("WinHotkeys", "KeyTeam1", m_KeyTeam1);
    ini.Put_KeyNumType("WinHotkeys", "KeyTeam2", m_KeyTeam2);
    ini.Put_KeyNumType("WinHotkeys", "KeyTeam3", m_KeyTeam3);
    ini.Put_KeyNumType("WinHotkeys", "KeyTeam4", m_KeyTeam4);
    ini.Put_KeyNumType("WinHotkeys", "KeyTeam5", m_KeyTeam5);
    ini.Put_KeyNumType("WinHotkeys", "KeyTeam6", m_KeyTeam6);
    ini.Put_KeyNumType("WinHotkeys", "KeyTeam7", m_KeyTeam7);
    ini.Put_KeyNumType("WinHotkeys", "KeyTeam8", m_KeyTeam8);
    ini.Put_KeyNumType("WinHotkeys", "KeyTeam9", m_KeyTeam9);
    ini.Put_KeyNumType("WinHotkeys", "KeyTeam10", m_KeyTeam10);

    ini.Save(fc);
}

/**
 * @brief Loads the options from the configuration file (redalert.ini in Red Alert for example).
 *
 * 0x00525A24
 */
void OptionsClass::Load_Settings()
{
    GameFileClass fc(g_SettingsFilename);

    GameINIClass ini;
    ini.Load(fc);

    m_GameSpeed = ini.Get_Int("Options", "GameSpeed", m_GameSpeed);
    m_ScrollRate = ini.Get_Int("Options", "ScrollRate", m_ScrollRate);

    Set_Sound_Volume(ini.Get_Fixed("Options", "Volume", m_Volume));
    Set_Score_Volume(ini.Get_Fixed("Options", "ScoreVolume", m_ScoreVolume));
    Set_MPlayer_Score_Volume(ini.Get_Fixed("Options", "MultiplayerScoreVolume", m_MultiplayerScoreVolume));
    Set_Brightness(ini.Get_Fixed("Options", "Brightness", m_Brightness));
    Set_Contrast(ini.Get_Fixed("Options", "Contrast", m_Contrast));
    Set_Saturation(ini.Get_Fixed("Options", "Color", m_Saturation));
    Set_Tint(ini.Get_Fixed("Options", "Tint", m_Tint));

    m_AutoScroll = ini.Get_Bool("Options", "AutoScroll", m_AutoScroll);
    m_ScoreRepeats = ini.Get_Bool("Options", "IsScoreRepeat", m_ScoreRepeats);
    m_ScoreShuffles = ini.Get_Bool("Options", "IsScoreShuffle", m_ScoreShuffles);
    m_PaletteScroll = ini.Get_Bool("Options", "PaletteScroll", m_PaletteScroll);
    m_FreeScrolling = ini.Get_Bool("Options", "FreeScrolling", m_FreeScrolling);
    m_DeathAnnounce = ini.Get_Bool("Options", "DeathAnnounce", m_DeathAnnounce);
    m_AllowSidebarToggle = ini.Get_Bool("Options", "AllowSidebarToggle", false); // TODO use variable as default when ctor used.
    m_CounterstrikeEnabled = ini.Get_Bool("Expansions", "CounterstrikeEnabled", false); // TODO use variable as default when ctor used.
    m_AftermathEnabled = ini.Get_Bool("Expansions", "AftermathEnabled", false); // TODO use variable as default when ctor used.
    m_ExpansionOptionsPresent = ini.Section_Present("Expansions");

    m_KeyForceMove1 = ini.Get_KeyNumType("WinHotkeys", "KeyForceMove1", m_KeyForceMove1);
    m_KeyForceMove2 = ini.Get_KeyNumType("WinHotkeys", "KeyForceMove2", m_KeyForceMove2);
    m_KeyForceAttack1 = ini.Get_KeyNumType("WinHotkeys", "KeyForceAttack1", m_KeyForceAttack1);
    m_KeyForceAttack2 = ini.Get_KeyNumType("WinHotkeys", "KeyForceAttack2", m_KeyForceAttack2);
    m_KeySelect1 = ini.Get_KeyNumType("WinHotkeys", "KeySelect1", m_KeySelect1);
    m_KeySelect2 = ini.Get_KeyNumType("WinHotkeys", "KeySelect2", m_KeySelect2);
    m_KeyScatter = ini.Get_KeyNumType("WinHotkeys", "KeyScatter", m_KeyScatter);
    m_KeyStop = ini.Get_KeyNumType("WinHotkeys", "KeyStop", m_KeyStop);
    m_KeyGuard = ini.Get_KeyNumType("WinHotkeys", "KeyGuard", m_KeyGuard);
    m_KeyNext = ini.Get_KeyNumType("WinHotkeys", "KeyNext", m_KeyNext);
    m_KeyPrevious = ini.Get_KeyNumType("WinHotkeys", "KeyPrevious", m_KeyPrevious);
    m_KeyFormation = ini.Get_KeyNumType("WinHotkeys", "KeyFormation", m_KeyFormation);
    m_KeyHome1 = ini.Get_KeyNumType("WinHotkeys", "KeyHome1", m_KeyHome1);
    m_KeyHome2 = ini.Get_KeyNumType("WinHotkeys", "KeyHome2", m_KeyHome2);
    m_KeyBase = ini.Get_KeyNumType("WinHotkeys", "KeyBase", m_KeyBase);
    m_KeyResign = ini.Get_KeyNumType("WinHotkeys", "KeyResign", m_KeyResign);
    m_KeyAlliance = ini.Get_KeyNumType("WinHotkeys", "KeyAlliance", m_KeyAlliance);
    m_KeyBookmark1 = ini.Get_KeyNumType("WinHotkeys", "KeyBookmark1", m_KeyBookmark1);
    m_KeyBookmark2 = ini.Get_KeyNumType("WinHotkeys", "KeyBookmark2", m_KeyBookmark2);
    m_KeyBookmark3 = ini.Get_KeyNumType("WinHotkeys", "KeyBookmark3", m_KeyBookmark3);
    m_KeyBookmark4 = ini.Get_KeyNumType("WinHotkeys", "KeyBookmark4", m_KeyBookmark4);
    m_KeySelectView = ini.Get_KeyNumType("WinHotkeys", "KeySelectView", m_KeySelectView);
    m_KeyRepairToggle = ini.Get_KeyNumType("WinHotkeys", "KeyRepairToggle", m_KeyRepairToggle);
    m_KeyRepairOn = ini.Get_KeyNumType("WinHotkeys", "KeyRepairOn", m_KeyRepairOn);
    m_KeyRepairOff = ini.Get_KeyNumType("WinHotkeys", "KeyRepairOff", m_KeyRepairOff);
    m_KeySellToggle = ini.Get_KeyNumType("WinHotkeys", "KeySellToggle", m_KeySellToggle);
    m_KeySellOn = ini.Get_KeyNumType("WinHotkeys", "KeySellOn", m_KeySellOn);
    m_KeySellOff = ini.Get_KeyNumType("WinHotkeys", "KeySellOff", m_KeySellOff);
    m_KeyMapToggle = ini.Get_KeyNumType("WinHotkeys", "KeyMapToggle", m_KeyMapToggle);
    m_KeySidebarUp = ini.Get_KeyNumType("WinHotkeys", "KeySidebarUp", m_KeySidebarUp);
    m_KeySidebarDown = ini.Get_KeyNumType("WinHotkeys", "KeySidebarDown", m_KeySidebarDown);
    m_KeyOption1 = ini.Get_KeyNumType("WinHotkeys", "KeyOption1", m_KeyOption1);
    m_KeyOption2 = ini.Get_KeyNumType("WinHotkeys", "KeyOption2", m_KeyOption2);
    m_KeyScrollLeft = ini.Get_KeyNumType("WinHotkeys", "KeyScrollLeft", m_KeyScrollLeft);
    m_KeyScrollRight = ini.Get_KeyNumType("WinHotkeys", "KeyScrollRight", m_KeyScrollRight);
    m_KeyScrollUp = ini.Get_KeyNumType("WinHotkeys", "KeyScrollUp", m_KeyScrollUp);
    m_KeyScrollDown = ini.Get_KeyNumType("WinHotkeys", "KeyScrollDown", m_KeyScrollDown);
    m_KeyQueueMove1 = ini.Get_KeyNumType("WinHotkeys", "KeyQueueMove1", m_KeyQueueMove1);
    m_KeyQueueMove2 = ini.Get_KeyNumType("WinHotkeys", "KeyQueueMove2", m_KeyQueueMove2);
    m_KeyTeam1 = ini.Get_KeyNumType("WinHotkeys", "KeyTeam1", m_KeyTeam1);
    m_KeyTeam2 = ini.Get_KeyNumType("WinHotkeys", "KeyTeam2", m_KeyTeam2);
    m_KeyTeam3 = ini.Get_KeyNumType("WinHotkeys", "KeyTeam3", m_KeyTeam3);
    m_KeyTeam4 = ini.Get_KeyNumType("WinHotkeys", "KeyTeam4", m_KeyTeam4);
    m_KeyTeam5 = ini.Get_KeyNumType("WinHotkeys", "KeyTeam5", m_KeyTeam5);
    m_KeyTeam6 = ini.Get_KeyNumType("WinHotkeys", "KeyTeam6", m_KeyTeam6);
    m_KeyTeam7 = ini.Get_KeyNumType("WinHotkeys", "KeyTeam7", m_KeyTeam7);
    m_KeyTeam8 = ini.Get_KeyNumType("WinHotkeys", "KeyTeam8", m_KeyTeam8);
    m_KeyTeam9 = ini.Get_KeyNumType("WinHotkeys", "KeyTeam9", m_KeyTeam9);
    m_KeyTeam10 = ini.Get_KeyNumType("WinHotkeys", "KeyTeam10", m_KeyTeam10);

    m_KeyEditorToggle = ini.Get_KeyNumType("WinHotkeys", "KeyEditorToggle", m_KeyEditorToggle);

#if defined(CHRONOSHIFT_DEBUG)
    m_DebugKeyToggleDebug = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyToggleDebug", m_DebugKeyToggleDebug);
    m_DebugKeyToggleVortex = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyToggleVortex", m_DebugKeyToggleVortex);
    m_DebugKeyForceRedraw = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyForceRedraw", m_DebugKeyForceRedraw);
    m_DebugKeyRandomCrate = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyRandomCrate", m_DebugKeyRandomCrate);
    m_DebugKeyRandomCrateAtMouse = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyRandomCrateAtMouse", m_DebugKeyRandomCrateAtMouse);
    m_DebugKeyFreeMoney = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyFreeMoney", m_DebugKeyFreeMoney);
    m_DebugKeyForceWin = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyForceWin", m_DebugKeyForceWin);
    m_DebugKeyForceLose = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyForceLose", m_DebugKeyForceLose);
    m_DebugKeyForceDie = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyForceDie", m_DebugKeyForceDie);
    m_DebugKeyBailOut = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyBailOut", m_DebugKeyBailOut);
    m_DebugKeyCellIcon = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyCellIcon", m_DebugKeyCellIcon);
    m_DebugKeyUnshroud = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyUnshroud", m_DebugKeyUnshroud);
    m_DebugKeySightRange = ini.Get_KeyNumType("DebugHotkeys", "DebugKeySightRange", m_DebugKeySightRange);
    m_DebugKeyGuardRange = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyGuardRange", m_DebugKeyGuardRange);
    m_DebugKeyWeaponRange = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyWeaponRange", m_DebugKeyWeaponRange);
    m_DebugKeyAttackFriendlies = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyAttackFriendlies", m_DebugKeyAttackFriendlies);
    m_DebugKeyFindPath = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyFindPath", m_DebugKeyFindPath);
    m_DebugKeyNavList = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyNavList", m_DebugKeyNavList);
    m_DebugKeyToggleMono = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyToggleMono", m_DebugKeyToggleMono);
    m_DebugKeyPrevMonoPage = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyPrevMonoPage", m_DebugKeyPrevMonoPage);
    m_DebugKeyNextMonoPage = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyNextMonoPage", m_DebugKeyNextMonoPage);
    m_DebugKeyPaused = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyPaused", m_DebugKeyPaused);
    m_DebugKeyFrameStep = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyFrameStep", m_DebugKeyFrameStep);
    m_DebugKeyOneFrameStep = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyOneFrameStep", m_DebugKeyOneFrameStep);
    m_DebugKeyFiveFrameStep = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyFiveFrameStep", m_DebugKeyFiveFrameStep);
    m_DebugKeyTenFrameStep = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyTenFrameStep", m_DebugKeyTenFrameStep);
    m_DebugKeySpawnMCV = ini.Get_KeyNumType("DebugHotkeys", "DebugKeySpawnMCV", m_DebugKeySpawnMCV);
    m_DebugKeySpawnHarvester = ini.Get_KeyNumType("DebugHotkeys", "DebugKeySpawnHarvester", m_DebugKeySpawnHarvester);
    m_DebugKeySpawnHeli = ini.Get_KeyNumType("DebugHotkeys", "DebugKeySpawnHeli", m_DebugKeySpawnHeli);
    m_DebugKeySpawnHind = ini.Get_KeyNumType("DebugHotkeys", "DebugKeySpawnHind", m_DebugKeySpawnHind);
    m_DebugKeySpawnTransport = ini.Get_KeyNumType("DebugHotkeys", "DebugKeySpawnTransport", m_DebugKeySpawnTransport);
    m_DebugKeyAtomExplosion = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyAtomExplosion", m_DebugKeyAtomExplosion);
    m_DebugKeySuperExplosion = ini.Get_KeyNumType("DebugHotkeys", "DebugKeySuperExplosion", m_DebugKeySuperExplosion);
    m_DebugKeyExplosion = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyExplosion", m_DebugKeyExplosion);
    m_DebugKeyDeleteObject = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyDeleteObject", m_DebugKeyDeleteObject);
    m_DebugKeyScreenshot = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyScreenshot", m_DebugKeyScreenshot);
    m_DebugKeyMotionCapture = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyMotionCapture", m_DebugKeyMotionCapture);
    m_DebugKeyThemeNext = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyThemeNext", m_DebugKeyThemeNext);
    m_DebugKeyThemePrev = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyThemePrev", m_DebugKeyThemePrev);
    m_DebugKeySpecialSonarPulse = ini.Get_KeyNumType("DebugHotkeys", "DebugKeySpecialSonarPulse", m_DebugKeySpecialSonarPulse);
    m_DebugKeySpecialAtomBomb = ini.Get_KeyNumType("DebugHotkeys", "DebugKeySpecialAtomBomb", m_DebugKeySpecialAtomBomb);
    m_DebugKeySpecialWarpSphere = ini.Get_KeyNumType("DebugHotkeys", "DebugKeySpecialWarpSphere", m_DebugKeySpecialWarpSphere);
    m_DebugKeySpecialParaBomb = ini.Get_KeyNumType("DebugHotkeys", "DebugKeySpecialParaBomb", m_DebugKeySpecialParaBomb);
    m_DebugKeySpecialParaInfantry = ini.Get_KeyNumType("DebugHotkeys", "DebugKeySpecialParaInfantry", m_DebugKeySpecialParaInfantry);
    m_DebugKeySpecialSpyPlane = ini.Get_KeyNumType("DebugHotkeys", "DebugKeySpecialSpyPlane", m_DebugKeySpecialSpyPlane);
    m_DebugKeySpecialIronCurtain = ini.Get_KeyNumType("DebugHotkeys", "DebugKeySpecialIronCurtain", m_DebugKeySpecialIronCurtain);
    m_DebugKeySpecialGPS = ini.Get_KeyNumType("DebugHotkeys", "DebugKeySpecialGPS", m_DebugKeySpecialGPS);
    m_DebugKeyInstantBuild = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyInstantBuild", m_DebugKeyInstantBuild);
    m_DebugKeyBuildCheat = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyBuildCheat", m_DebugKeyBuildCheat);
    m_DebugKeyAIControl = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyAIControl", m_DebugKeyAIControl);
    m_DebugKeyStealObject = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyStealObject", m_DebugKeyStealObject);
    m_DebugKeyToggleDamage = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyToggleDamage", m_DebugKeyToggleDamage);
    m_DebugKeyToggleCloakable = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyToggleCloakable", m_DebugKeyToggleCloakable);
    m_DebugKeyApplyDamage = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyApplyDamage", m_DebugKeyApplyDamage);
    m_DebugKeyToggleFullMap = ini.Get_KeyNumType("DebugHotkeys", "DebugKeyToggleFullMap", m_DebugKeyToggleFullMap);
#endif // CHRONOSHIFT_DEBUG
}

/**
 * @brief Carries out a palette adjustment.
 *
 * 0x00525884
 */
void OptionsClass::Adjust_Palette(
    PaletteClass &src, PaletteClass &dst, fixed_t brightness, fixed_t saturation, fixed_t tint, fixed_t contrast) const
{
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

/**
 * @brief Fixes up the g_CCPalette with the current display adjustment values.
 *
 * 0x00526BB0
 */
void OptionsClass::Fixup_Palette() const
{
    Adjust_Palette(g_OriginalPalette, g_GamePalette, m_Brightness, m_Saturation, m_Tint, m_Contrast);
    g_CCPalette = g_GamePalette;
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
        return _adjust[delay][m_GameSpeed];
    }

    return 8 * delay / (m_GameSpeed + 1);
}

/**
 * @brief Sets the volume for background music.
 *
 * 0x0052520C
 */
void OptionsClass::Set_Score_Volume(fixed_t volume, BOOL feedback)
{
    m_ScoreVolume = std::clamp<fixed_t>(volume, fixed_t(0), fixed_t(255, 256));

    //this takes a int so we need to scale the fixed to the 256 range
    Set_Score_Vol(256 * m_ScoreVolume);

    if (feedback) {
        if (!g_Theme.Still_Playing()) {
            Sound_Effect(VOC_RABEEP1, m_ScoreVolume, 1, 0, HOUSES_NONE);
        }
    }
}

/**
 * @brief Sets the volume for in game sounds.
 *
 * 0x005252BC
 */
void OptionsClass::Set_Sound_Volume(fixed_t volume, BOOL feedback)
{
    m_Volume = std::clamp<fixed_t>(volume, fixed_t(0), fixed_t(255, 256));

    if (feedback) {
        Sound_Effect(VOC_RABEEP1);
    }
}

/**
 * @brief Sets the display brightness.
 *
 * 0x0052534C
 */
void OptionsClass::Set_Brightness(fixed_t brightness)
{
    m_Brightness = brightness * fixed_t::_1_2 + fixed_t::_1_4;
    Adjust_Palette(g_OriginalPalette, g_GamePalette, m_Brightness, m_Saturation, m_Tint, m_Contrast);
    g_GamePalette.Set();
}

/**
 * @brief Sets the display saturation.
 *
 * 0x0052551C
 */
void OptionsClass::Set_Saturation(fixed_t saturation)
{
    m_Saturation = saturation;
    Adjust_Palette(g_OriginalPalette, g_GamePalette, m_Brightness, m_Saturation, m_Tint, m_Contrast);
    g_GamePalette.Set();
}

/**
 * @brief Sets the display contrast.
 *
 * 0x005255E8
 */
void OptionsClass::Set_Contrast(fixed_t contrast)
{
    m_Contrast = contrast * fixed_t::_1_2 + fixed_t::_1_4;
    Adjust_Palette(g_OriginalPalette, g_GamePalette, m_Brightness, m_Saturation, m_Tint, m_Contrast);
    g_GamePalette.Set();
}

/**
 * @brief Sets the display tint.
 *
 * 0x005257B8
 */
void OptionsClass::Set_Tint(fixed_t tint)
{
    m_Tint = tint;
    Adjust_Palette(g_OriginalPalette, g_GamePalette, m_Brightness, m_Saturation, m_Tint, m_Contrast);
    g_GamePalette.Set();
}

/**
 * @brief Calls most of the set functions with the members existing values.
 *
 * 0x00526A88
 */
void OptionsClass::Set()
{
    Set_Brightness(m_Brightness);
    Set_Contrast(m_Contrast);
    Set_Saturation(m_Saturation);
    Set_Tint(m_Tint);
    Set_Sound_Volume(m_Volume);
    Set_Score_Volume(m_ScoreVolume);
    Set_Shuffle(m_ScoreShuffles);
    Set_Repeat(m_ScoreRepeats);
}
