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

int g_mapBinaryVersion; // For handling C&C and Sole Survivor map formats.

#ifndef CHRONOSHIFT_STANDALONE
int &g_iniFormat = Make_Global<int>(0x00665DE8);
int &g_frame = Make_Global<int>(0x006680C4);
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
BOOL &GameActive = Make_Global<BOOL>(0x00669924);
BOOL &ScenarioInit = Make_Global<BOOL>(0x00669914);
BOOL &DebugUnshroud = Make_Global<BOOL>(0x0065D7F0);
BOOL &DebugQuiet = Make_Global<BOOL>(0x0065D7CC);
BOOL &ScoresPresent = Make_Global<BOOL>(0x006680C0);
BOOL &StreamLowImpact = Make_Global<BOOL>(0x006ABFCC);
BOOL &g_cancelCurrentMsgBox = Make_Global<BOOL>(0x00685170);
BOOL &VideoBackBufferAllowed = Make_Global<BOOL>(0x00604DE8);
BOOL &AllowHardwareFilledBlits = Make_Global<BOOL>(0x0060BA70);
BOOL &g_soundOn = Make_Global<BOOL>(0x006807F4);
BOOL &g_slowPalette = Make_Global<BOOL>(0x006678E4);
BOOL &g_breakoutAllowed = Make_Global<BOOL>(0x006016B8);
HWND &MainWindow = Make_Global<HWND>(0x006B1498);
#ifdef PLATFORM_WINDOWS
HMODULE &ProgramInstance = Make_Global<HMODULE>(0x0068A4BC); // Only used in gameres packet, won't be needed in final
#endif
char **TutorialText = reinterpret_cast<char **>(0x00666304);
BOOL &MouseInstalled = Make_Global<BOOL>(0x00680838);
int &g_seed = Make_Global<int>(0x00680654);
int &CustomSeed = Make_Global<int>(0x00680658);
int &RandNumb = Make_Global<int>(0x0060D61C);
int &g_readyToQuit = Make_Global<int>(0x00680880);
HousesType &Whom = Make_Global<HousesType>(0x00669910);
void *&g_WakeShapes = Make_Global<void *>(0x0068D2DC);
void *&g_TurretShapes = Make_Global<void *>(0x0068D2E0);
void *&g_SamShapes = Make_Global<void *>(0x0068D2E4);
void *&g_MGunShapes = Make_Global<void *>(0x0068D2E8);

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
int g_iniFormat;
int g_frame;
BOOL g_gameInFocus;
BOOL g_inMapEditor;
char *Metal12FontPtr;
char *MapFontPtr;
char *Font6Ptr;
char *GradFont6Ptr;
char *EditorFont;
char *Font8Ptr;
char *Font3Ptr;
char *ScoreFontPtr;
char *FontLEDPtr;
char *VCRFontPtr;
char *TypeFontPtr;
BOOL GameActive;
BOOL ScenarioInit;
BOOL DebugUnshroud;
BOOL DebugQuiet;
BOOL ScoresPresent;
BOOL StreamLowImpact;
BOOL g_cancelCurrentMsgBox;
BOOL g_soundOn;
BOOL g_slowPalette;
BOOL g_breakoutAllowed = true;
#ifdef PLATFORM_WINDOWS
HWND MainWindow;
#endif
char *TutorialText;
BOOL MouseInstalled;
int g_seed;
int CustomSeed;
int RandNumb = 0x12349876;
int g_readyToQuit;
HousesType Whom;
void *g_WakeShapes;
void *g_TurretShapes;
void *g_SamShapes;
void *g_MGunShapes;

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
