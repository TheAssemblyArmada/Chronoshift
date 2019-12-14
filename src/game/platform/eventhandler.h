/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Platform/Framework specific event handling code.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "always.h"

typedef void (*focusfunc_t)();

#if defined PLATFORM_WINDOWS
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

#ifdef GAME_DLL
extern unsigned &g_CCFocusMessage;
extern focusfunc_t &g_AudioFocusLoss;
extern focusfunc_t &g_GBufferFocusLoss;
#else
extern unsigned g_CCFocusMessage;
extern focusfunc_t g_AudioFocusLoss;
extern focusfunc_t g_GBufferFocusLoss;
#endif

#endif // EVENTHANDLER_H
