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
#include "eventhandler.h"
#include "audio.h"
#include "focus.h"
#include "gbuffer.h"
#include "globals.h"
#include "initvideo.h"
#include "keyboard.h"
#include "mouse.h"
#include "ostimer.h"
#include "session.h"
#include "surfacemonitor.h"
#include "theme.h"
#include "wsock.h"

#ifndef GAME_DLL
unsigned CCFocusMessage = 0x432;
focusfunc_t AudioFocusLoss;
focusfunc_t GBufferFocusLoss;
#endif

// TODO Refactor so this cleanup stuff is done with atexit perhaps once standalone?
void Prog_End()
{
    Sound_End();

    delete g_mouse;
    g_mouse = nullptr;

    delete PlatformTimer;
    PlatformTimer = nullptr;
}

// This probably needs moving later as its also called in Game_Main.
void Shutdown_Network()
{
    Session.MPlayer_Clear_Game_Name();
}

#if defined PLATFORM_WINDOWS
LRESULT __stdcall WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Handle a focus change message.
    if (uMsg == CCFocusMessage) {
        Start_Primary_Sound_Buffer(true);

        if (!g_InMovie) {
            Theme.Stop();
            Theme.Queue_Song(THEME_NEXT);
        }

        return 0;
    }

    // Handle a message for the network interface.
    if (g_packetTransport != nullptr && uMsg == g_packetTransport->Protocol_Event_Message()) {
        if (g_packetTransport->Message_Handler(hwnd, uMsg, wParam, lParam) != 0) {
            return DefWindowProcA(hwnd, uMsg, wParam, lParam);
        }

        return 0;
    }

    // Handle input messages.
    if (g_keyboard->Message_Handler(hwnd, uMsg, wParam, lParam)) {
        return 1;
    }

    switch (uMsg) {
        case WM_DESTROY:
            Prog_End();
            g_visiblePage.Un_Init();
            g_hiddenPage.Un_Init();
            g_allSurfaces.Release();

            // Original checks a bool here called InDebugger which is never true in the final build.
            Reset_Video_Mode();

            // Original calls Stop_Profiler() here.

            PostQuitMessage(0);

            switch (g_readyToQuit) {
                case 0: // normal exit.
                    Shutdown_Network();
                    ExitProcess(0);
                    break;
                case 1: // exit due to memory or failure in main.
                    g_readyToQuit = 2; // Seems to mean abnormal exit reached.
                    break;
                case 3: // Doing an emergency exit.
                    Shutdown_Network();
                    g_readyToQuit = 2; // Seems to mean abnormal exit reached.
                    break;
                default:
                    break;
            }

            return 0;
        case WM_SYSCOMMAND:
            switch (wParam) {
                case SC_CLOSE: // Ignore a close message, likely F4 in a full screen program.
                case SC_KEYMENU: // Ignore attempts to activate the games "menu" which doesn't exist.
                case SC_SCREENSAVE: // Supress attempts to start a screen saver.
                    return 0;
                default: // Otherwise we are going to go to default handler.
                    break;
            }

            break;
        case WM_ACTIVATEAPP:
            // Handle focus change.
            g_gameInFocus = wParam;

            if (!g_gameInFocus) {
                Focus_Loss();
            }

            g_allSurfaces.Set_Surface_Focus(g_gameInFocus);
            g_allSurfaces.Restore_Surfaces();

            return 0;
        default:
            break;
    }

    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}
#endif
