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

//
// Controls ID's
//
#define BUTTON_NONE -1

// Standard Button ID's
#define BUTTON_OK 1
#define BUTTON_CANCEL 2
#define BUTTON_YES 3
#define BUTTON_NO 4

//
// Ingame UI Gadget ID's
//
#define BUTTON_REPAIR 101
//#define		BUTTON_
#define BUTTON_SELL 103
//#define		BUTTON_
#define BUTTON_ZOOM 105

// Sidebar UI ID's
#define GADGET_STRIP_COLUMN_LEFT 200
#define GADGET_STRIP_COLUMN_RIGHT 210


#define BUTTON_SOUND_STOP 605
#define BUTTON_SOUND_PLAY 606
#define BUTTON_SOUND_SUFFLE 607
#define BUTTON_SOUND_REPEAT 608
#define BUTTON_SOUND_OK 609
#define SLIDER_SCORE_VOL 610
#define SLIDER_SOUND_VOL 611

// Menu Button ID's
#define BUTTON_CS 1000
#define BUTTON_AM 1001
#define BUTTON_NEW_GAME 1002
#define BUTTON_LOAD_GAME 1003
#define BUTTON_MP_GAME 1004
#define BUTTON_SNEAK_PEAK 1005
#define BUTTON_MOVIES 1006
#define BUTTON_OPTIONS 1007
#define BUTTON_EXIT 1008

// Unknown
#define BUTTON__ 1010

// Mutliplayer Button IDs
#define BUTTON_MODEM_SERIAL 1020
#define BUTTON_SKIRMISH 1021
#define BUTTON_NETWORK 1022
#define BUTTON_INTERNET 1023

// Fetch Difficulty IDs
#define SLIDER_DIFFICULTY 1030

// Right Click Context Menu ID
#define CONTROL_CONTEXT_MENU 8000

//
#define GAME_TICKS_PER_SECOND 15
#define GAME_TICKS_PER_MINUTE GAME_TICKS_PER_SECOND * 60

#define TUTORIAL_TEXT_COUNT 225

extern const char g_SettingsFilename[];

#ifdef PLATFORM_WINDOWS
#ifdef GAME_DLL
extern HWND &g_MainWindow;
extern HMODULE &g_ProgramInstance;
#else
extern HWND g_MainWindow;
//extern HMODULE g_ProgramInstance; // Only used in gameres packet, won't be needed in final
#endif
#else

#endif

// For s_ScreenWidth and s_ScreenHeight, see public static members of GraphicViewPortClass.

// Hook the original binary's globals until standalone or have implemented and hooked all references.
#ifdef GAME_DLL
#include "hooker.h"

extern int &g_INIFormat;
extern int &g_GameFrame; // NOTE: previously g_Frame.
extern BOOL &g_GameInFocus;
extern BOOL &g_InMovie;
extern BOOL &g_InMapEditor;
extern char *&g_Metal12FontPtr;
extern char *&g_MapFontPtr;
extern char *&g_Font6Ptr;
extern char *&g_GradFont6Ptr;
extern char *&g_EditorFont;
extern char *&g_Font8Ptr;
extern char *&g_Font3Ptr;
extern char *&g_ScoreFontPtr;
extern char *&g_FontLEDPtr;
extern char *&g_VCRFontPtr;
extern char *&g_TypeFontPtr;
extern BOOL &g_AllowVoice;
extern BOOL &g_GameActive;
extern int &g_ScenarioInit;
extern BOOL &g_DebugUnshroud;
extern BOOL &g_DebugQuiet;
extern BOOL &g_ScoresPresent;
extern BOOL &g_StreamLowImpact;
extern BOOL &g_CancelCurrentMsgBox;
extern BOOL &g_VideoBackBufferAllowed;
extern BOOL &AllowHardwareFilledBlits;
extern BOOL &g_OverlappedVideoBlits;
extern BOOL &g_SoundOn;
extern BOOL &g_SlowPalette;
extern BOOL &g_BreakoutAllowed;
extern SpecialDialogType &g_SpecialDialog;
extern BOOL &g_IsTanyaDead;
extern BOOL &g_SaveTanya;
extern BOOL &g_AntsEnabled;
extern BOOL &g_AftermathMultiplayer;
extern BOOL &g_NewUnitsEnabled;
extern BOOL &g_SecretUnitsEnabled;
extern BOOL &g_NewMaxAheadFrame1;
extern BOOL &g_NewMaxAheadFrame2;
extern int &g_UnitBuildPenalty;
extern BOOL &g_TimeQuake;
extern BOOL &g_PendingTimeQuake;
extern int &g_TimeQuakeCenter;
extern BOOL &g_GameStatisticsPacketSent;
extern BOOL &g_PlayerWins;
extern BOOL &g_PlayerLoses;
extern BOOL &g_PlayerRestarts;
extern BOOL g_PlayerAborts;
extern char &g_TeamEvent;
extern char &g_TeamNumber;
extern char &g_FormationEvent;
extern int &g_BuildLevel;
extern BOOL &g_MouseInstalled;
extern int &g_Seed;
extern int &g_CustomSeed;
extern int &g_RandNumb;
extern int &g_SpareTicks;
extern int &g_ReadyToQuit;
extern HousesType &g_Whom;
extern void *&g_WakeShapes;
extern void *&g_TurretShapes;
extern void *&g_SamShapes;
extern void *&g_MGunShapes;
extern const char **g_TutorialText;
extern BOOL &g_FormMove;
extern SpeedType &g_FormSpeed;
extern MPHType &g_FormMaxSpeed;

extern BOOL &g_Debug_MotionCapture;
extern BOOL &g_Debug_Rotate;
extern BOOL &g_Debug_Quiet;
extern BOOL &g_Debug_Cheat;
extern BOOL &g_Debug_Remap;
extern BOOL &g_Debug_Icon;
extern BOOL &g_Debug_Flag;
extern BOOL &g_Debug_Lose;
extern BOOL &g_Debug_Win;
//extern BOOL &g_Debug_Map; // NOTE: Use g_InMapEditor
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
extern int g_INIFormat;
extern int g_GameFrame;
extern BOOL g_GameInFocus;
extern BOOL g_InMovie;
extern BOOL g_InMapEditor;
extern char *g_Metal12FontPtr;
extern char *g_MapFontPtr;
extern char *g_Font6Ptr;
extern char *g_GradFont6Ptr;
extern char *g_EditorFont;
extern char *g_Font8Ptr;
extern char *g_Font3Ptr;
extern char *g_ScoreFontPtr;
extern char *g_FontLEDPtr;
extern char *g_VCRFontPtr;
extern char *g_TypeFontPtr;
extern BOOL g_AllowVoice;
extern BOOL g_GameActive;
extern int g_ScenarioInit;
extern BOOL g_DebugUnshroud;
extern BOOL g_DebugQuiet;
extern BOOL g_ScoresPresent;
extern BOOL g_StreamLowImpact;
extern BOOL g_CancelCurrentMsgBox;
extern BOOL g_VideoBackBufferAllowed;
extern BOOL AllowHardwareFilledBlits;
extern BOOL g_OverlappedVideoBlits;
extern BOOL g_SoundOn;
extern BOOL g_SlowPalette;
extern BOOL g_BreakoutAllowed;
extern SpecialDialogType g_SpecialDialog;
extern BOOL g_IsTanyaDead;
extern BOOL g_SaveTanya;
extern BOOL g_AntsEnabled;
extern BOOL g_AftermathMultiplayer;
extern BOOL g_NewUnitsEnabled;
extern BOOL g_SecretUnitsEnabled;
extern BOOL g_NewMaxAheadFrame1;
extern BOOL g_NewMaxAheadFrame2;
extern int g_UnitBuildPenalty;
extern BOOL g_TimeQuake;
extern BOOL g_PendingTimeQuake;
extern target_t g_TimeQuakeCenter;
extern BOOL g_GameStatisticsPacketSent;
extern BOOL g_PlayerWins;
extern BOOL g_PlayerLoses;
extern BOOL g_PlayerRestarts;
extern BOOL g_PlayerAborts;
extern char g_TeamEvent;
extern char g_TeamNumber;
extern char g_FormationEvent;
extern int g_BuildLevel;
extern BOOL g_MouseInstalled;
extern int g_Seed;
extern int g_CustomSeed;
extern int g_RandNumb;
extern int g_SpareTicks;
extern int g_ReadyToQuit;
extern HousesType g_Whom;
extern void *g_WakeShapes;
extern void *g_TurretShapes;
extern void *g_SamShapes;
extern void *g_MGunShapes;
extern const char *g_TutorialText[TUTORIAL_TEXT_COUNT];
extern BOOL g_FormMove;
extern SpeedType g_FormSpeed;
extern MPHType g_FormMaxSpeed;

extern BOOL g_Debug_MotionCapture;
extern BOOL g_Debug_Rotate;
extern BOOL g_Debug_Quiet;
extern BOOL g_Debug_Cheat;
extern BOOL g_Debug_Remap;
extern BOOL g_Debug_Icon;
extern BOOL g_Debug_Flag;
extern BOOL g_Debug_Lose;
extern BOOL g_Debug_Win;
//extern BOOL g_Debug_Map; // NOTE: Use g_InMapEditor
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

extern int g_MapBinaryVersion;

extern BOOL g_Debug_Paused;
extern BOOL g_Debug_Step;
extern int g_Debug_StepCount;

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

extern BOOL g_ConfirmExit;

#endif // GLOBALS_H
