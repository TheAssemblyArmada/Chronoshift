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
#ifdef PLATFORM_WINDOWS
HWND &MainWindow = Make_Global<HWND>(0x006B1498);
#endif
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
#ifdef PLATFORM_WINDOWS
HWND MainWindow;
#endif
#endif