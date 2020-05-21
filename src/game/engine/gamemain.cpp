/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief The game engines main routine to be called from the user entry point.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "gamemain.h"
#include "gameevent.h"
#include "gameloop.h"
#include "gameoptions.h"
#include "globals.h"
#include "init.h"
#include "iomap.h"
#include "keyboard.h"
#include "language.h"
#include "mainmenu.h"
#include "mouse.h"
#include "palette.h"
#include "queue.h"
#include "scenario.h"
#include "session.h"
#include "vortex.h"

// TODO: These wrapped functions will need moving to a final home when implemented. Probable a multiplayer related cpp.
void Register_Game_Start_Time()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x005B794C, void);
    func();
#endif
}

// temp forward declare, see gameloop.cpp
void Send_Statistics_Packet();

void Special_Dialog(BOOL skip_events)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00550DF0, void, BOOL);
    func(skip_events);
#endif
}

// temp forward declare, see eventhandler.cpp
void Shutdown_Network();

BOOL Surrender_Dialog(int txt_id)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00503DD0, BOOL, int);
    return func(txt_id);
#else
    return false;
#endif
}

void Main_Game(int argc, char **argv)
{
    static BOOL _fade = true;

    // If the initialisation failed in some way we cannot continue.
    if (!Init_Game(argc, argv)) {
        return;
    }

    while (Select_Game(_fade)) {
        g_ScenarioInit = 0;
        _fade = true;
        g_ChronalVortex.Stop();
        g_ChronalVortex.Setup_Remap_Tables(g_Scen.Get_Theater());
        g_GamePalette.Set(15);
        g_Keyboard->Clear();

        if (g_Session.Playback_Game()) {
            g_Mouse->Hide_Mouse();
            g_TeamEvent = 0;
            g_TeamNumber = 0;
            g_FormationEvent = 0;
        } else {
            g_Mouse->Show_Mouse();
        }

        if (g_Session.Game_To_Play() == GAME_INTERNET) {
            Register_Game_Start_Time();
            g_GameStatisticsPacketSent = false;
            g_PacketLater = nullptr;
            g_ConnectionLost = false;
        }

        while (true) {
            g_TimeQuake = g_PendingTimeQuake;
            g_PendingTimeQuake = false;

            if (Main_Loop()) {
                break;
            }

            switch (g_SpecialDialog) {
                case SPECIAL_DLG_OPTIONS:
                    g_Map.Help_Text(TXT_NULL);
                    g_Map.Override_Mouse_Shape(MOUSE_POINTER);
                    g_Options.Process();
                    g_Map.Revert_Mouse_Shape();
                    g_SpecialDialog = SPECIAL_DLG_NONE;
                    break;
                case SPECIAL_DLG_SURRENDER:
                    g_Map.Help_Text(TXT_NULL);
                    g_Map.Override_Mouse_Shape(MOUSE_POINTER);

                    if (Surrender_Dialog(TXT_SURRENDER)) {
                        GameEventClass tmp(GameEventClass::EVENT_DESTRUCT);
                        g_OutgoingEvents.Add(tmp);
                    }

                    g_Map.Revert_Mouse_Shape();
                    g_SpecialDialog = SPECIAL_DLG_NONE;
                    break;
                case SPECIAL_DLG_SPECIAL:
                    g_Map.Help_Text(TXT_NULL);
                    g_Map.Override_Mouse_Shape(MOUSE_POINTER);
                    Special_Dialog(false);
                    g_Map.Revert_Mouse_Shape();
                    g_SpecialDialog = SPECIAL_DLG_NONE;
                    break;
                default:
                    break;
            }
        }

        if (!g_GameStatisticsPacketSent && g_PacketLater != nullptr) {
            Send_Statistics_Packet();
        }

        g_BlackPalette.Set(30);
        g_VisiblePage.Clear();

        if (g_Session.Record_Game() || g_Session.Playback_Game()) {
            g_Session.Recording_File().Close();
        }

        // Old modem and null modem signoff was also handled here for those types of game.
        if (g_Session.Game_To_Play() == GAME_LAN && !g_Session.Playback_Game()) {
            Shutdown_Network();
        }

        if (g_Session.Playback_Game()) {
            g_Mouse->Show_Mouse();
            g_Session.Set_Game_To_Play(GAME_CAMPAIGN);
            g_Session.Set_Playback_Game(false);
        }
    }

    g_Session.Free_Scenario_Descriptions();
}
