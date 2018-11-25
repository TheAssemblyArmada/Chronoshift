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
#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include "always.h"
#include "gametypes.h"

#define GAME_TICKS_PER_SECOND 15
#define GAME_TICKS_PER_MINUTE GAME_TICKS_PER_SECOND * 60

extern int g_mapBinaryVersion;

// Hook the original binary's globals until standalone or have implemented and hooked all references.
#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"

extern int &g_iniFormat;
extern int &g_frame;
extern BOOL &g_gameInFocus;
extern BOOL &g_inMapEditor;
extern char *&Metal12FontPtr;
extern char *&MapFontPtr;
extern char *&Font6Ptr;
extern char *&GradFont6Ptr;
extern char *&EditorFont;
extern char *&Font8Ptr;
extern char *&Font3Ptr;
extern char *&ScoreFontPtr;
extern char *&FontLEDPtr;
extern char *&VCRFontPtr;
extern char *&TypeFontPtr;
extern BOOL &GameActive;
extern BOOL &ScenarioInit;
extern BOOL &DebugUnshroud;
extern BOOL &DebugQuiet;
extern BOOL &ScoresPresent;
extern BOOL &StreamLowImpact;
extern BOOL &g_cancelCurrentMsgBox;
extern BOOL &VideoBackBufferAllowed;
extern BOOL &AllowHardwareFilledBlits;
extern BOOL &g_soundOn;
extern BOOL &g_slowPalette;
extern BOOL &g_breakoutAllowed;
#ifdef PLATFORM_WINDOWS
extern HWND &MainWindow;
extern HMODULE &ProgramInstance; // Only used in gameres packet, won't be needed in final
#endif
extern char **TutorialText;
extern BOOL &MouseInstalled;
extern int &g_seed;
extern int &CustomSeed;
extern int &RandNumb;
extern int &g_readyToQuit;
extern HousesType &Whom;
extern void *&g_WakeShapes;
extern void *&g_TurretShapes;
extern void *&g_SamShapes;
extern void *&g_MGunShapes;

extern BOOL &g_Debug_MotionCapture;
extern BOOL &g_Debug_Rotate;
extern BOOL &g_Debug_Quiet;
extern BOOL &g_Debug_Cheat;
extern BOOL &g_Debug_Remap;
extern BOOL &g_Debug_Icon;
extern BOOL &g_Debug_Flag;
extern BOOL &g_Debug_Lose;
extern BOOL &g_Debug_Win;
extern BOOL &g_Debug_Map;
extern BOOL &g_Debug_Passable;
extern BOOL &g_Debug_Unshroud;
extern BOOL &g_Debug_Threat;
extern BOOL &g_Debug_Find_Path;
extern BOOL &g_Debug_Check_Map;
extern BOOL &g_Debug_Playtest;
extern BOOL &g_Debug_Heap_Dump;
extern BOOL &g_Debug_Smart_Print;
extern BOOL &g_Debug_Trap_Check_Heap;
extern BOOL &g_Debug_Modem_Dump;
extern BOOL &g_Debug_Print_Events;

extern BOOL &g_MonoEnabled;
#else
extern int g_iniFormat;
extern int g_frame;
extern BOOL g_gameInFocus;
extern BOOL g_inMapEditor;
extern char *Metal12FontPtr;
extern char *MapFontPtr;
extern char *Font6Ptr;
extern char *GradFont6Ptr;
extern char *EditorFont;
extern char *Font8Ptr;
extern char *Font3Ptr;
extern char *ScoreFontPtr;
extern char *FontLEDPtr;
extern char *VCRFontPtr;
extern char *TypeFontPtr;
extern BOOL GameActive;
extern BOOL ScenarioInit;
extern BOOL DebugUnshroud;
extern BOOL DebugQuiet;
extern BOOL ScoresPresent;
extern BOOL StreamLowImpact;
extern BOOL g_cancelCurrentMsgBox;
extern BOOL VideoBackBufferAllowed;
extern BOOL AllowHardwareFilledBlits;
extern BOOL g_soundOn;
extern BOOL g_slowPalette;
extern BOOL g_breakoutAllowed;
#ifdef PLATFORM_WINDOWS
extern HWND MainWindow;
#endif
extern char *TutorialText;
extern BOOL MouseInstalled;
extern int g_seed;
extern int CustomSeed;
extern int RandNumb;
extern int g_readyToQuit;
extern HousesType Whom;
extern void *g_WakeShapes;
extern void *g_TurretShapes;
extern void *g_SamShapes;
extern void *g_MGunShapes;

extern BOOL g_Debug_MotionCapture;
extern BOOL g_Debug_Rotate;
extern BOOL g_Debug_Quiet;
extern BOOL g_Debug_Cheat;
extern BOOL g_Debug_Remap;
extern BOOL g_Debug_Icon;
extern BOOL g_Debug_Flag;
extern BOOL g_Debug_Lose;
extern BOOL g_Debug_Win;
extern BOOL g_Debug_Map;
extern BOOL g_Debug_Passable;
extern BOOL g_Debug_Unshroud;
extern BOOL g_Debug_Threat;
extern BOOL g_Debug_Find_Path;
extern BOOL g_Debug_Check_Map;
extern BOOL g_Debug_Playtest;
extern BOOL g_Debug_Heap_Dump;
extern BOOL g_Debug_Smart_Print;
extern BOOL g_Debug_Trap_Check_Heap;
extern BOOL g_Debug_Modem_Dump;
extern BOOL g_Debug_Print_Events;

extern BOOL g_MonoEnabled;
#endif

extern BOOL g_Debug_SightRange;
extern BOOL g_Debug_GuardRange;
extern BOOL g_Debug_WeaponRange;
extern BOOL g_Debug_AttackFriendlies;
extern BOOL g_Debug_NavList;
extern BOOL g_Debug_Damage;
extern BOOL g_Debug_AIControl;
extern BOOL g_Debug_InstantBuild;
extern BOOL g_Debug_BuildCheat;
extern BOOL g_Debug_CursorCoords;

extern BOOL g_Cheat_OreCivilians;
extern BOOL g_Cheat_TeslaChoppers;

#endif // GLOBALS_H
