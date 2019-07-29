/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
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
#include "globals.h"

#ifdef PLATFORM_WINDOWS
#ifndef CHRONOSHIFT_STANDALONE
HWND &MainWindow = Make_Global<HWND>(0x006B1498);
HMODULE &ProgramInstance = Make_Global<HMODULE>(0x0068A4BC);
#else
HWND MainWindow = nullptr;
// HMODULE ProgramInstance = nullptr; // Only used in gameres packet, won't be needed in final
#endif
#else

#endif

#ifndef CHRONOSHIFT_STANDALONE
int &g_iniFormat = Make_Global<int>(0x00665DE8);
int &g_GameFrame = Make_Global<int>(0x006680C4);
BOOL &g_gameInFocus = Make_Global<BOOL>(0x00665F64);
BOOL &g_inMapEditor = Make_Global<BOOL>(0x0065D7E8);
char *&Metal12FontPtr = Make_Global<char *>(0x0066992C);
char *&MapFontPtr = Make_Global<char *>(0x00669930);
char *&Font6Ptr = Make_Global<char *>(0x0066993C);
char *&GradFont6Ptr = Make_Global<char *>(0x00669954);
char *&EditorFont = Make_Global<char *>(0x00669940);
char *&Font8Ptr = Make_Global<char *>(0x00669944);
char *&Font3Ptr = Make_Global<char *>(0x00669938);
char *&ScoreFontPtr = Make_Global<char *>(0x00669950);
char *&FontLEDPtr = Make_Global<char *>(0x00669948);
char *&VCRFontPtr = Make_Global<char *>(0x0066994C);
char *&TypeFontPtr = Make_Global<char *>(0x00669934);
BOOL &g_AllowVoice = Make_Global<BOOL>(0x006016C0);
BOOL &GameActive = Make_Global<BOOL>(0x00669924);
BOOL &ScenarioInit = Make_Global<BOOL>(0x00669914);
BOOL &DebugUnshroud = Make_Global<BOOL>(0x0065D7F0);
BOOL &DebugQuiet = Make_Global<BOOL>(0x0065D7CC);
BOOL &ScoresPresent = Make_Global<BOOL>(0x006680C0);
BOOL &StreamLowImpact = Make_Global<BOOL>(0x006ABFCC);
BOOL &g_cancelCurrentMsgBox = Make_Global<BOOL>(0x00685170);
BOOL &VideoBackBufferAllowed = Make_Global<BOOL>(0x00604DE8);
BOOL &AllowHardwareFilledBlits = Make_Global<BOOL>(0x0060BA70);
BOOL &g_OverlappedVideoBlits = Make_Global<BOOL>(0x0060BE54);
BOOL &g_soundOn = Make_Global<BOOL>(0x006807F4);
BOOL &g_slowPalette = Make_Global<BOOL>(0x006678E4);
BOOL &g_breakoutAllowed = Make_Global<BOOL>(0x006016B8);
SpecialDialogType &g_SpecialDialog = Make_Global<SpecialDialogType>(0x00680834);
BOOL &g_IsTanyaDead = Make_Global<BOOL>(0x00665DD4);
BOOL &g_SaveTanya = Make_Global<BOOL>(0x00665DD8);
BOOL &g_AntsEnabled = Make_Global<BOOL>(0x00665DDC);
BOOL &g_AftermathMultiplayer = Make_Global<BOOL>(0x00680538);
BOOL &g_NewUnitsEnabled = Make_Global<BOOL>(0x00665DE0);
BOOL &g_SecretUnitsEnabled = Make_Global<BOOL>(0x00665DE4);
BOOL &g_NewMaxAheadFrame1 = Make_Global<BOOL>(0x00680530);
BOOL &g_NewMaxAheadFrame2 = Make_Global<BOOL>(0x00680534);
int &g_UnitBuildPenalty = Make_Global<int>(0x006017D8);
BOOL &g_TimeQuake = Make_Global<BOOL>(0x00665DEC);
BOOL &g_PendingTimeQuake = Make_Global<BOOL>(0x00665DF0);
int &g_TimeQuakeCenter = Make_Global<int>(0x00665DF4);
BOOL &g_GameStatisticsPacketSent = Make_Global<BOOL>(0x006ABBB8);
BOOL &g_PlayerWins = Make_Global<BOOL>(0x006680C8);
BOOL &g_PlayerLoses = Make_Global<BOOL>(0x006680CC);
BOOL &g_PlayerRestarts = Make_Global<BOOL>(0x006680D0);
BOOL g_PlayerAborts = false;
int &g_BuildLevel = Make_Global<int>(0x006016C8);
char **TutorialText = reinterpret_cast<char **>(0x00666304);
BOOL &MouseInstalled = Make_Global<BOOL>(0x00680838);
int &g_seed = Make_Global<int>(0x00680654);
int &CustomSeed = Make_Global<int>(0x00680658);
int &RandNumb = Make_Global<int>(0x0060D61C);
int &g_SpareTicks = Make_Global<int>(0x006670E8);
int &g_readyToQuit = Make_Global<int>(0x00680880);
HousesType &Whom = Make_Global<HousesType>(0x00669910);
void *&g_WakeShapes = Make_Global<void *>(0x0068D2DC);
void *&g_TurretShapes = Make_Global<void *>(0x0068D2E0);
void *&g_SamShapes = Make_Global<void *>(0x0068D2E4);
void *&g_MGunShapes = Make_Global<void *>(0x0068D2E8);
BOOL &g_FormMove = Make_Global<BOOL>(0x0065E0CC);
SpeedType &g_FormSpeed = Make_Global<SpeedType>(0x0065E0D0);
MPHType &g_FormMaxSpeed = Make_Global<MPHType>(0x0065E0D1);

BOOL &g_Debug_MotionCapture = Make_Global<BOOL>(0x0065D7C4);
BOOL &g_Debug_Rotate = Make_Global<BOOL>(0x0065D7C8);
BOOL &g_Debug_Quiet = Make_Global<BOOL>(0x0065D7CC);
BOOL &g_Debug_Cheat = Make_Global<BOOL>(0x0065D7D0);
BOOL &g_Debug_Remap = Make_Global<BOOL>(0x0065D7D4);
BOOL &g_Debug_Icon = Make_Global<BOOL>(0x0065D7D8);
BOOL &g_Debug_Flag = Make_Global<BOOL>(0x0065D7DC);
BOOL &g_Debug_Lose = Make_Global<BOOL>(0x0065D7E0);
BOOL &g_Debug_Win = Make_Global<BOOL>(0x0065D7E4);
BOOL &g_Debug_Map = Make_Global<BOOL>(0x0065D7E8);
BOOL &g_Debug_Passable = Make_Global<BOOL>(0x0065D7EC);
BOOL &g_Debug_Unshroud = Make_Global<BOOL>(0x0065D7F0);
BOOL &g_Debug_Threat = Make_Global<BOOL>(0x0065D7F4);
BOOL &g_Debug_Find_Path = Make_Global<BOOL>(0x0065D7F8);
BOOL &g_Debug_Check_Map = Make_Global<BOOL>(0x0065D7FC);
BOOL &g_Debug_Playtest = Make_Global<BOOL>(0x0065D800);
BOOL &g_Debug_Heap_Dump = Make_Global<BOOL>(0x0065D804);
BOOL &g_Debug_Smart_Print = Make_Global<BOOL>(0x0065D808);
BOOL &g_Debug_Trap_Check_Heap = Make_Global<BOOL>(0x0065D80C);
BOOL &g_Debug_Modem_Dump = Make_Global<BOOL>(0x0065D810);
BOOL &g_Debug_Print_Events = Make_Global<BOOL>(0x0065D814);

BOOL &g_MonoEnabled = Make_Global<BOOL>(0x006AC288); // Actually a part of MonoClass.
#else
int g_iniFormat = 0;
int g_GameFrame = 0;
BOOL g_gameInFocus = false;
BOOL g_inMapEditor = false;
char *Metal12FontPtr = nullptr;
char *MapFontPtr = nullptr;
char *Font6Ptr = nullptr;
char *GradFont6Ptr = nullptr;
char *EditorFont = nullptr;
char *Font8Ptr = nullptr;
char *Font3Ptr = nullptr;
char *ScoreFontPtr = nullptr;
char *FontLEDPtr = nullptr;
char *VCRFontPtr = nullptr;
char *TypeFontPtr = nullptr;
BOOL g_AllowVoice = true;
BOOL GameActive = false;
BOOL ScenarioInit;
BOOL DebugUnshroud = false;
BOOL DebugQuiet = false;
BOOL ScoresPresent = false;
BOOL StreamLowImpact = false;
BOOL g_cancelCurrentMsgBox = false;
BOOL VideoBackBufferAllowed = false;
BOOL g_OverlappedVideoBlits = true;
BOOL g_soundOn = false;
BOOL g_slowPalette = false;
BOOL g_breakoutAllowed = true;
SpecialDialogType g_SpecialDialog = SPECIAL_DLG_NONE;
BOOL g_IsTanyaDead = false;
BOOL g_SaveTanya = false;
BOOL g_AntsEnabled = false;
BOOL g_AftermathMultiplayer = false;
BOOL g_NewUnitsEnabled = false;
BOOL g_SecretUnitsEnabled = false;
BOOL g_NewMaxAheadFrame1 = false;
BOOL g_NewMaxAheadFrame2 = false;
int g_UnitBuildPenalty = 100;
BOOL g_TimeQuake = false;
BOOL g_PendingTimeQuake = false;
target_t g_TimeQuakeCenter = 0;
BOOL g_GameStatisticsPacketSent = false;
BOOL g_PlayerWins = false;
BOOL g_PlayerLoses = false;
BOOL g_PlayerRestarts = false;
BOOL g_PlayerAborts = false;
int g_BuildLevel = 10;
char *TutorialText = nullptr;
BOOL MouseInstalled = false;
int g_seed = 0;
int CustomSeed = 0;
int RandNumb = 0x12349876;
int g_readyToQuit = false;
int g_SpareTicks = 0;
HousesType Whom = HOUSES_NONE;
void *g_WakeShapes = nullptr;
void *g_TurretShapes = nullptr;
void *g_SamShapes = nullptr;
void *g_MGunShapes = nullptr;
BOOL g_FormMove = false;
SpeedType g_FormSpeed = SPEED_NONE;
MPHType g_FormMaxSpeed = MPH_MAX;

BOOL g_Debug_MotionCapture = false;
BOOL g_Debug_Rotate = false;
BOOL g_Debug_Quiet = false;
BOOL g_Debug_Cheat = false;
BOOL g_Debug_Remap = false;
BOOL g_Debug_Icon = false;
BOOL g_Debug_Flag = false;
BOOL g_Debug_Lose = false;
BOOL g_Debug_Win = false;
BOOL g_Debug_Map = false;
BOOL g_Debug_Passable = false;
BOOL g_Debug_Unshroud = false;
BOOL g_Debug_Threat = false;
BOOL g_Debug_Find_Path = false;
BOOL g_Debug_Check_Map = false;
BOOL g_Debug_Playtest = false;
BOOL g_Debug_Heap_Dump = false;
BOOL g_Debug_Smart_Print = false;
BOOL g_Debug_Trap_Check_Heap = false;
BOOL g_Debug_Modem_Dump = false;
BOOL g_Debug_Print_Events = false;

BOOL g_MonoEnabled = false; // Actually a part of MonoClass.
#endif

int g_mapBinaryVersion; // For handling C&C and Sole Survivor map formats.

BOOL g_Debug_Paused = false;
BOOL g_Debug_Step = false;
int g_Debug_StepCount = 0;

BOOL g_Debug_SightRange = false;
BOOL g_Debug_GuardRange = false;
BOOL g_Debug_WeaponRange = false;
BOOL g_Debug_AttackFriendlies = false;
BOOL g_Debug_NavList = false;
BOOL g_Debug_Damage = false;
BOOL g_Debug_AIControl = false;
BOOL g_Debug_InstantBuild = false;
BOOL g_Debug_BuildCheat = false;
BOOL g_Debug_CursorCoords = false;

BOOL g_Cheat_OreCivilians = false;
BOOL g_Cheat_TeslaChoppers = false;

BOOL g_ConfirmExit = false;
