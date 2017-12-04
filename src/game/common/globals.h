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
#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include "always.h"

// Hook the original binary's globals until standalone or have implemented and hooked all references.
#ifndef RAPP_STANDALONE
#include "hooker.h"

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
#ifdef PLATFORM_WINDOWS
extern HWND &MainWindow;
#endif
#else
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
#ifdef PLATFORM_WINDOWS
extern HWND MainWindow;
extern BOOL GameActive;
extern BOOL ScenarioInit;
extern BOOL DebugUnshroud;
#endif
#endif

#endif
