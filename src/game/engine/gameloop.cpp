/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Main game loop for the game engine.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "gameloop.h"
#include "bench.h"
#include "callback.h"
#include "gamekeyboard.h"
#include "gameoptions.h"
#include "gbuffer.h"
#include "globals.h"
#include "house.h"
#include "iomap.h"
#include "logic.h"
#include "mouse.h"
#include "msgbox.h"
#include "ostimer.h"
#include "queue.h"
#include "scenario.h"
#include "session.h"
#include "shape.h"
#include "theme.h"
#include "ttimer.h"

void Check_For_Focus_Loss()
{
#ifdef GAME_DLL
    void (*func)() = reinterpret_cast<void (*)()>(0x005B3624);
    func();
#endif
}

void Do_Record_Playback()
{
#ifdef GAME_DLL
    void (*func)() = reinterpret_cast<void (*)()>(0x004AB140);
    func();
#endif
}

void Register_Game_End_Time()
{
#ifdef GAME_DLL
    void (*func)() = reinterpret_cast<void (*)()>(0x005B7974);
    func();
#endif
}

void Send_Statistics_Packet()
{
#ifdef GAME_DLL
    void (*func)() = reinterpret_cast<void (*)()>(0x005B653C);
    func();
#endif
}

void Do_Win()
{
#ifdef GAME_DLL
    void (*func)() = reinterpret_cast<void (*)()>(0x0053AC78);
    func();
#endif
}

void Do_Lose()
{
#ifdef GAME_DLL
    void (*func)() = reinterpret_cast<void (*)()>(0x0053B2A0);
    func();
#endif
}

void Do_Restart()
{
#ifdef GAME_DLL
    void (*func)() = reinterpret_cast<void (*)()>(0x0053B708);
    func();
#endif
}

void Do_Abort() {}

void Do_Draw()
{
#ifdef GAME_DLL
    void (*func)() = reinterpret_cast<void (*)()>(0x0053B590);
    func();
#endif
}

void Process_Input()
{
    KeyNumType key = KN_NONE;
    int mouse_x = 0;
    int mouse_y = 0;

    g_Map.Input(key, mouse_x, mouse_y);

    if (key != KN_NONE) {
        Keyboard_Process(key);
    }
}

void Color_Cycle()
{
#ifdef GAME_DLL
    void (*func)() = reinterpret_cast<void (*)()>(0x004A735C);
    func();
#endif
}

void Sync_Delay()
{
    g_SpareTicks += g_FrameTimer.Time();

    while (g_FrameTimer.Time() > 0) {

        Color_Cycle();

        Call_Back();

        /*if (g_Session.Game_To_Play() != GAME_CAMPAIGN && g_Session.Game_To_Play() != GAME_SKIRMISH) {
            g_PlatformTimer->Sleep(4);
        }*/

        if (g_SpecialDialog == SPECIAL_DLG_NONE) {
            g_Mouse->Erase_Mouse(g_HidPage, true);
            Process_Input();
            g_Map.Render();
        }

        // Yield remaining timeslice to other processes?
        g_PlatformTimer->Sleep(1); // why not 0? 1 Frees up processor time?
    }

    Color_Cycle();

    Call_Back();
}

#ifdef CHRONOSHIFT_DEBUG
void Debug_Check_Map()
{
    if (g_Debug_Check_Map) {
        if (!g_Map.Validate()) {
            MessageBoxClass errormsgbox;
            if (!errormsgbox.Process("Map Error!", "Continue", "Stop")) {
                g_GameActive = false;
            }
            g_Map.Validate();
        }
    }
}

void Debug_Motion_Capture()
{
    if (g_Debug_MotionCapture) {
        // TODO!
    }
}
#endif

void Map_Editor_Main_Loop()
{
    g_Map.Render();
    g_Mouse->Erase_Mouse(g_HidPage, true);
    Process_Input();
    Call_Back();
    Color_Cycle();
}

BOOL Main_Loop()
{
    if (g_InMapEditor) {

        Map_Editor_Main_Loop();
    }
    if (!g_GameActive) {
        return false;
    }

#ifdef CHRONOSHIFT_DEBUG
    // This chunk both handles debug frame stepping after we have flushed all the
    // requested steps, or if the pause flag has been enabled. The game will continue
    // to render the last frame and accept input, and the engine will sleep.
    if (g_Debug_Step && g_Debug_StepCount <= 0 || g_Debug_Paused) {
        // We need this here so the audio stream updates.
        Call_Back();

        // And this to make sure we still render the game viewport and handle input still.
        if (!g_Session.Playback_Game() && g_SpecialDialog == SPECIAL_DLG_NONE) {
            if (g_GameInFocus) {
                g_Mouse->Erase_Mouse(g_HidPage, true);
                Process_Input();
                g_Map.Render();
            }
        }

        // Force a redraw if there are any pending messages. The debug key system needs this.
        if (g_Session.Get_Messages().Manage()) {
            g_HiddenPage.Clear();
            g_Map.Flag_To_Redraw(true);
        }

        // Yield remaining timeslice to other processes?
        g_PlatformTimer->Sleep(1); // why not 0? 1 frees up processor time?
        return !g_GameActive;
    }
    // Run the normal game loop body if frame stepping is disabled or if
    // we have remaining frames to flush before we go back to a paused state.
    if (!g_Debug_Step || (g_Debug_Step && g_Debug_StepCount > 0)) {
#endif

        Check_For_Focus_Loss();

        Reallocate_Big_Shape_Buffer();

        if (g_GameFrame >= g_Session.Trap_Frame()) {
            g_Session.Trap_Object();
        }

        g_Session.Set_Processing_Start_Tick(g_TickCountTimer.Time());

#ifdef CHRONOSHIFT_DEBUG
        if (g_Session.Trap_Check_Heap()) {
            g_Debug_Trap_Check_Heap = true;
        }
#endif
        BENCHMARK_START(BENCH_MAIN);

        if (g_Theme.What_Is_Playing() == THEME_NONE) {
            g_Theme.Play_Next_Song();
        }

        if (g_Session.Game_To_Play() != GAME_CAMPAIGN && g_Session.Game_To_Play() != GAME_SKIRMISH
            && g_Session.Packet_Protocol() == COMPROTO_TWO) {
            if (g_Session.Packet_Protocol() == COMPROTO_ONE && g_Session.Playback_Game()) {
                g_FrameTimer = 0;
            } else {
                if (g_Session.Desired_Frame_Rate() == 0) {
                    g_Session.Set_Desired_Frame_Rate(60);
                }
                g_FrameTimer = 60 / g_Session.Desired_Frame_Rate();
            }
        } else {
            int game_speed = g_Options.Get_Game_Speed();
            DiffType ai_diff = g_PlayerPtr->Get_AI_Difficulty();
            if (game_speed > 0) {
                g_FrameTimer = game_speed + ((ai_diff == DIFF_EASIEST) - (ai_diff == DIFF_NORMAL));
            } else {
                g_FrameTimer = game_speed + (ai_diff == DIFF_EASIEST);
            }
        }

        if (!g_Session.Playback_Game() && g_SpecialDialog == SPECIAL_DLG_NONE) {
            if (g_GameInFocus) {
                g_Mouse->Erase_Mouse(g_HidPage, true);
                Process_Input();
                g_Map.Render();
            }
        }

        if (g_Session.Record_Game() || g_Session.Playback_Game()) {
            Do_Record_Playback();
        }

        DisplayClass::s_Layers[LAYER_GROUND].Sort();

        g_Logic.AI();

        g_TimeQuake = false;

        if (!g_PendingTimeQuake) {
            g_TimeQuakeCenter = 0;
        }

        // Force a redraw if there are any pending messages to be drawn to the screen.
        if (g_Session.Get_Messages().Manage()) {
            g_HiddenPage.Clear();
            g_Map.Flag_To_Redraw(true);
        }

        g_Session.Tick_Processing_Frame();

        // g_Session.Update_Processing_Tick_Value(std::min<int>((g_TickCountTimer.Time() - g_Session.Processing_Start_Tick()), MAX_PROCESSING_TICKS));
        g_Session.Update_Processing_Tick_Value(g_TickCountTimer.Time() - g_Session.Processing_Start_Tick());

        Queue_AI();

        // TODO, Requires ScoreClass
#ifdef GAME_DLL
        int &score_field_28 = Make_Global<int>(0x006698E4);
        score_field_28 += 4;
#else
        // Score.field_28 += 4;
#endif

        Call_Back();

        if (g_PlayerWins) {
            captainslog_debug("Player wins.");
            if (g_Session.Game_To_Play() == GAME_INTERNET && !g_GameStatisticsPacketSent) {
                Register_Game_End_Time();
                Send_Statistics_Packet();
            }
            g_PlayerLoses = false;
            g_PlayerWins = false;
            g_PlayerRestarts = false;
            g_PlayerAborts = false;
            g_Mouse->Erase_Mouse(g_HidPage, true);
            g_Map.Help_Text(TXT_NULL);
            Do_Win();
            return !g_GameActive;
        }

        if (g_PlayerLoses) {
            captainslog_debug("Player loses.");
            if (g_Session.Game_To_Play() == GAME_INTERNET && !g_GameStatisticsPacketSent) {
                Register_Game_End_Time();
                Send_Statistics_Packet();
            }
            g_PlayerLoses = false;
            g_PlayerWins = false;
            g_PlayerRestarts = false;
            g_PlayerAborts = false;
            g_Mouse->Erase_Mouse(g_HidPage, true);
            g_Map.Help_Text(TXT_NULL);
            Do_Lose();
            return !g_GameActive;
        }

        if (g_PlayerRestarts) {
            captainslog_debug("Player restarted.");
            g_PlayerLoses = false;
            g_PlayerWins = false;
            g_PlayerRestarts = false;
            g_PlayerAborts = false;
            g_Mouse->Erase_Mouse(g_HidPage, true);
            g_Map.Help_Text(TXT_NULL);
            Do_Restart();
            return !g_GameActive;
        }

        if (g_PlayerAborts) {
            captainslog_debug("Player aborted.");
            g_PlayerLoses = false;
            g_PlayerWins = false;
            g_PlayerRestarts = false;
            g_PlayerAborts = false;
            g_Mouse->Erase_Mouse(g_HidPage, true);
            g_Map.Help_Text(TXT_NULL);
            Do_Abort();
            return !g_GameActive;
        }

        if (g_Session.Game_To_Play() != GAME_CAMPAIGN && g_Session.Game_To_Play() != GAME_SKIRMISH
            && g_Session.Players_List().Count() == 2 && g_Scen.Get_field_7CF() && g_Scen.Get_field_7D3()) {
            captainslog_debug("Game is a draw.");
            if (g_Session.Game_To_Play() == GAME_INTERNET && !g_GameStatisticsPacketSent) {
                Register_Game_End_Time();
                Send_Statistics_Packet();
            }
            g_Mouse->Erase_Mouse(g_HidPage, true);
            g_Map.Help_Text(TXT_NULL);
            Do_Draw();
            return !g_GameActive;
        }

        ++g_GameFrame;

#ifdef CHRONOSHIFT_DEBUG
        Debug_Check_Map();
        Debug_Motion_Capture();
#endif

        BENCHMARK_END(BENCH_MAIN);

        Sync_Delay();

#ifdef CHRONOSHIFT_DEBUG
        // decrement the step count after we have processed one frame iteration.
        if (g_Debug_Step && g_Debug_StepCount > 0) {
            --g_Debug_StepCount;
        }

    } // debug step if case
#endif

    return !g_GameActive;
}
