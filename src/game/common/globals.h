/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
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
#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include "always.h"

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
#ifdef PLATFORM_WINDOWS
extern HWND &MainWindow;
#endif
extern char **TutorialText;
extern BOOL &MouseInstalled;
extern int &g_seed;
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
#ifdef PLATFORM_WINDOWS
extern HWND MainWindow;
#endif
extern char *TutorialText;
extern BOOL MouseInstalled;
extern int g_seed;
#endif

#endif
