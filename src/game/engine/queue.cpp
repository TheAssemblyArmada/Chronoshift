/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 * @author tomsons26
 *
 * @brief Message queue related functions.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "queue.h"
#include "eventhandler.h"
#include "gameevent.h"
#include "globals.h"
#include "house.h"
#include "mouse.h"
#include "session.h"
#include "target.h"

#ifndef GAME_DLL
TEventQueueClass<GameEventClass, OUTGOING_SIZE> g_OutgoingEvents; // was OutList
TEventQueueClass<GameEventClass, SCHEDULED_SIZE> g_ScheduledEvents; // was DoList
#endif

BOOL Queue_Options()
{
    GameEventClass ev(GameEventClass::EVENT_OPTIONS);
    return g_OutgoingEvents.Add(ev);
}

BOOL Queue_Exit()
{
    GameEventClass ev(GameEventClass::EVENT_EXIT);
    return g_OutgoingEvents.Add(ev);
}

void Queue_Record()
{
    int event_count = 0;

    for (int index = 0; index < g_ScheduledEvents.Get_Count(); ++index) {
        GameEventClass &ev = g_ScheduledEvents.Fetch_From_Head(index);

        if (g_GameFrame == ev.Get_Event_Frame() && !ev.Is_Executed()) {
            ++event_count;
        }
    }

    g_Session.Recording_File().Write(&event_count, sizeof(event_count));

    for (int index = 0; index < g_ScheduledEvents.Get_Count(); ++index) {
        GameEventClass &ev = g_ScheduledEvents.Fetch_From_Head(index);

        if (g_GameFrame == ev.Get_Event_Frame() && !ev.Is_Executed()) {
            g_Session.Recording_File().Write(&ev, sizeof(GameEventClass));
            --event_count;
        }
    }
}

void Queue_Playback()
{
#ifdef GAME_DLL
    void (*func)() = reinterpret_cast<void (*)()>(0x0052BDEC);
    return func();
#else
    // TODO
#endif
}

void Queue_AI()
{
    if (g_Session.Playback_Game()) {
        Queue_Playback();
    } else {
        switch (g_Session.Game_To_Play()) {
            case GAME_CAMPAIGN:
            case GAME_SKIRMISH:
                Queue_AI_Normal();
                break;
            case 1:
            case 2:
            case GAME_NETWORK:
            case GAME_INTERNET:
            case 6:
            case 7:
                Queue_AI_Multiplayer();
                break;
            default:
                break;
        };
    }
}

void Queue_AI_Normal()
{
    while (g_OutgoingEvents.Get_Count() > 0) {
        GameEventClass &ev = g_OutgoingEvents.Fetch_Head();
        ev.Set_Executed(false);
        g_ScheduledEvents.Add(ev);
        g_OutgoingEvents.Remove_Head();
    }

    if (g_Session.Record_Game()) {
        Queue_Record();
    }

    if (Execute_Scheduled_Events(1, g_PlayerPtr->What_Type())) {
        Clean_Scheduled_Events();
    } else {
        captainslog_debug("Queue_AI_Normal() - Execute_ScheduledEvents() failed");
        g_GameActive = false;
    }
}

void Queue_AI_Network()
{
#ifdef GAME_DLL
#else
#endif
}

void Queue_AI_Multiplayer()
{
#ifdef GAME_DLL
    void (*func)() = reinterpret_cast<void (*)()>(0x00529020);
    return func();
#else
#endif
}

BOOL Queue_Mission(TargetClass whom, MissionType mission, target_t target, target_t dest)
{
    GameEventClass ev(whom, mission, target, dest);
    return g_OutgoingEvents.Add(ev);
}

BOOL Queue_Mission_Formation(
    TargetClass whom, MissionType mission, target_t target, target_t dest, SpeedType form_speed, MPHType form_max_speed)
{
    GameEventClass ev(whom, mission, target, dest, form_speed, form_max_speed);
    return g_OutgoingEvents.Add(ev);
}

BOOL Execute_Scheduled_Events(int house_count, HousesType houses, ConnectionManagerClass *conn_mgr,
    TCountDownTimerClass<FrameTimerClass> *a4, int *a5, uint16_t *a6, uint16_t *a7)
{
#ifdef GAME_DLL
    BOOL(*func)
    (int, HousesType, ConnectionManagerClass *, TCountDownTimerClass<FrameTimerClass> *, int *, uint16_t *, uint16_t *) =
        reinterpret_cast<BOOL (*)(int,
            HousesType,
            ConnectionManagerClass *,
            TCountDownTimerClass<FrameTimerClass> *,
            signed int *,
            uint16_t *,
            uint16_t *)>(0x0052B69C);
    return func(house_count, houses, conn_mgr, a4, a5, a6, a7);
#else
    return false;
#endif
}

void Clean_Scheduled_Events(ConnectionManagerClass *conn_mgr)
{
    while (g_ScheduledEvents.Get_Count() > 0) {
        g_Keyboard->Check();

        if (conn_mgr != nullptr) {
            // Update_Queue_Mono(conn_mgr, 7);
        }

        GameEventClass &ev = g_ScheduledEvents.Fetch_Head();

        if (!ev.Is_Executed() && ev.Get_Event_Frame() >= g_GameFrame) {
            break;
        }

        g_ScheduledEvents.Remove_Head();
    }
}

void Compute_Game_CRC()
{
#ifdef GAME_DLL
    void (*func)() = reinterpret_cast<void (*)()>(0x0052C044);
    func();
#endif
}

void Print_CRCs(GameEventClass *event)
{
#ifdef GAME_DLL
    void (*func)(GameEventClass *) = reinterpret_cast<void (*)(GameEventClass *)>(0x0052C2B8);
    func(event);
#endif
}
