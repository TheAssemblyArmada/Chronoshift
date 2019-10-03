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

#if defined PLATFORM_WINDOWS && !defined GAME_DLL
HWND MainWindow = nullptr;
//HMODULE ProgramInstance = nullptr; // Only used in gameres packet, won't be needed in final
#endif

#ifndef GAME_DLL
int g_iniFormat = 0;
int g_GameFrame = 0;
BOOL g_gameInFocus = false;
BOOL g_InMovie = false;
BOOL g_InMapEditor = false;
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
