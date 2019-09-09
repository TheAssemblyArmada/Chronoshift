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

    Map.Input(key, mouse_x, mouse_y);

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
    g_SpareTicks += FrameTimer.Time();

    while (FrameTimer.Time() > 0) {

        Color_Cycle();

        Call_Back();

        /*if (Session.Game_To_Play() != GAME_CAMPAIGN && Session.Game_To_Play() != GAME_SKIRMISH) {
            PlatformTimer->Sleep(4);
        }*/

        if (g_SpecialDialog == SPECIAL_DLG_NONE) {
            g_mouse->Erase_Mouse(g_hidPage, true);
            Process_Input();
            Map.Render();
        }

        // Yield remaining timeslice to other processes?
        PlatformTimer->Sleep(1); // why not 0? 1 Frees up processor time?
    }

    Color_Cycle();

    Call_Back();
}

#ifdef CHRONOSHIFT_DEBUG
void Debug_Check_Map()
{
    if (g_Debug_Check_Map) {
        if (!Map.Validate()) {
            MessageBoxClass errormsgbox;
            if (!errormsgbox.Process("Map Error!", "Continue", "Stop")) {
                GameActive = false;
            }
            Map.Validate();
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

BOOL Main_Loop()
{
    if (GameActive) {

#ifdef CHRONOSHIFT_DEBUG

        // This chunk both handles debug frame stepping after we have flushed all the
        // requested steps, or if the pause flag has been enabled. The game will continue
        // to render the last frame and accept input, and the engine will sleep.
        if (g_Debug_Step && g_Debug_StepCount <= 0 || g_Debug_Paused) {

            // We need this here so the audio stream updates.
            Call_Back();

            // And this to make sure we still render the game viewport and handle input still.
            if (!Session.Playback_Game() && g_SpecialDialog == SPECIAL_DLG_NONE) {
                if (g_gameInFocus) {
                    g_mouse->Erase_Mouse(g_hidPage, true);
                    Process_Input();
                    Map.Render();
                }
            }

            // Force a redraw if there are any pending messages. The debug key system needs this.
            if (Session.Get_Messages().Manage()) {
                g_hiddenPage.Clear();
                Map.Flag_To_Redraw(true);
            }

            // Yield remaining timeslice to other processes?
            PlatformTimer->Sleep(1); // why not 0? 1 frees up processor time?

        } else {

            // Run the normal game loop body if frame stepping is disabled or if
            // we have remaining frames to flush before we go back to a paused state.
            if (!g_Debug_Step || (g_Debug_Step && g_Debug_StepCount > 0)) {
#endif

                Check_For_Focus_Loss();

                Reallocate_Big_Shape_Buffer();

                if (g_GameFrame >= Session.Trap_Frame()) {
                    Session.Trap_Object();
                }

                Session.Set_Processing_Start_Tick(TickCountTimer.Time());

#ifdef CHRONOSHIFT_DEBUG
                if (Session.Trap_Check_Heap()) {
                    g_Debug_Trap_Check_Heap = true;
                }
#endif

                if (Theme.What_Is_Playing() == THEME_NONE) {
                    Theme.Play_Next_Song();
                }

                if (Session.Game_To_Play() != GAME_CAMPAIGN && Session.Game_To_Play() != GAME_SKIRMISH
                    && Session.Packet_Protocol() == COMPROTO_TWO) {
                    if (Session.Packet_Protocol() == COMPROTO_ONE && Session.Playback_Game()) {
                        FrameTimer = 0;
                    } else {
                        if (Session.Desired_Frame_Rate() == 0) {
                            Session.Set_Desired_Frame_Rate(60);
                        }
                        FrameTimer = 60 / Session.Desired_Frame_Rate();
                    }
                } else {
                    int game_speed = Options.Game_Speed();
                    DiffType ai_diff = g_PlayerPtr->Get_AI_Difficulty();
                    if (game_speed > 0) {
                        FrameTimer = game_speed + ((ai_diff == DIFF_EASIEST) - (ai_diff == DIFF_NORMAL));
                    } else {
                        FrameTimer = game_speed + (ai_diff == DIFF_EASIEST);
                    }
                }

                if (!Session.Playback_Game() && g_SpecialDialog == SPECIAL_DLG_NONE) {
                    if (g_gameInFocus) {
                        g_mouse->Erase_Mouse(g_hidPage, true);
                        Process_Input();
                        Map.Render();
                    }
                }

                if (Session.Record_Game() || Session.Playback_Game()) {
                    Do_Record_Playback();
                }

                DisplayClass::Layers[LAYER_GROUND].Sort();

                Logic.AI();

                g_TimeQuake = false;

                if (!g_PendingTimeQuake) {
                    g_TimeQuakeCenter = 0;
                }

                // Force a redraw if there are any pending messages to be drawn to the screen.
                if (Session.Get_Messages().Manage()) {
                    g_hiddenPage.Clear();
                    Map.Flag_To_Redraw(true);
                }

                Session.Tick_Processing_Frame();

                // Session.Update_Processing_Tick_Value(std::min<int>((TickCountTimer.Time() - Session.Processing_Start_Tick()), MAX_PROCESSING_TICKS));
                Session.Update_Processing_Tick_Value(TickCountTimer.Time() - Session.Processing_Start_Tick());

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
                    DEBUG_LOG("Player wins.\n");
                    if (Session.Game_To_Play() == GAME_INTERNET && !g_GameStatisticsPacketSent) {
                        Register_Game_End_Time();
                        Send_Statistics_Packet();
                    }
                    g_PlayerLoses = false;
                    g_PlayerWins = false;
                    g_PlayerRestarts = false;
                    g_PlayerAborts = false;
                    g_mouse->Erase_Mouse(g_hidPage, true);
                    Map.Help_Text(TXT_NULL);
                    Do_Win();

                } else if (g_PlayerLoses) {
                    DEBUG_LOG("Player loses.\n");
                    if (Session.Game_To_Play() == GAME_INTERNET && !g_GameStatisticsPacketSent) {
                        Register_Game_End_Time();
                        Send_Statistics_Packet();
                    }
                    g_PlayerLoses = false;
                    g_PlayerWins = false;
                    g_PlayerRestarts = false;
                    g_PlayerAborts = false;
                    g_mouse->Erase_Mouse(g_hidPage, true);
                    Map.Help_Text(TXT_NULL);
                    Do_Lose();

                } else if (g_PlayerRestarts) {
                    DEBUG_LOG("Player restarted.\n");
                    g_PlayerLoses = false;
                    g_PlayerWins = false;
                    g_PlayerRestarts = false;
                    g_PlayerAborts = false;
                    g_mouse->Erase_Mouse(g_hidPage, true);
                    Map.Help_Text(TXT_NULL);
                    Do_Restart();

                } else if (g_PlayerAborts) {
                    DEBUG_LOG("Player aborted.\n");
                    g_PlayerLoses = false;
                    g_PlayerWins = false;
                    g_PlayerRestarts = false;
                    g_PlayerAborts = false;
                    g_mouse->Erase_Mouse(g_hidPage, true);
                    Map.Help_Text(TXT_NULL);
                    Do_Abort();

                } else if (Session.Game_To_Play() != GAME_CAMPAIGN && Session.Game_To_Play() != GAME_SKIRMISH
                    && Session.Players_List().Count() == 2 && Scen.Get_field_7CF() && Scen.Get_field_7D3()) {
                    DEBUG_LOG("Game is a draw.\n");
                    if (Session.Game_To_Play() == GAME_INTERNET && !g_GameStatisticsPacketSent) {
                        Register_Game_End_Time();
                        Send_Statistics_Packet();
                    }
                    g_mouse->Erase_Mouse(g_hidPage, true);
                    Map.Help_Text(TXT_NULL);
                    Do_Draw();

                } else {
                    ++g_GameFrame;

#ifdef CHRONOSHIFT_DEBUG
                    Debug_Check_Map();
                    Debug_Motion_Capture();
#endif

                    Sync_Delay();

                }

#ifdef CHRONOSHIFT_DEBUG
                // derecment the step count after we have processed one frame iteration.
                if (g_Debug_Step && g_Debug_StepCount > 0) {
                    --g_Debug_StepCount;
                }

            } // if case
        } // sleep case
#endif
    }

    return !GameActive;
}
