/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
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
#include "gameevent.h"
#include "target.h"
#include "globals.h"
#include "house.h"
#include "session.h"

#ifndef GAME_DLL
TEventQueueClass<GameEventClass, OUTGOING_SIZE> g_OutgoingEvents;
TEventQueueClass<GameEventClass, SCHEDULED_SIZE> g_ScheduledEvents;
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
#ifdef GAME_DLL
    void (*func)() = reinterpret_cast<void (*)()>(0x0052BD20);
    return func();
#else
    int event_index = 0;
    for (int index = 0; index < g_ScheduledEvents.Get_Count(); ++index) {
        GameEventClass ev = g_ScheduledEvents[index];
        if (ev.Get_Event_Frame() == g_GameFrame && !ev.Is_Executed()) {
            ++event_index;
        }
    }
    //g_Session.RecordFile.Write(&event_index, sizeof(event_index));

    for (int index = 0; index < g_ScheduledEvents.Get_Count(); ++index) {
        GameEventClass ev = g_ScheduledEvents.Fetch_From_Tail(index);
        if (ev.Get_Event_Frame() == g_GameFrame && !ev.Is_Executed()) {
            //g_Session.RecordFile.Write(&ev, sizeof(ev));
            --event_index;
        }
    }
#endif
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
            case 3:
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
#ifdef GAME_DLL
    void (*func)() = reinterpret_cast<void (*)()>(0x00528F20);
    return func();
#else
    while (g_ScheduledEvents.Get_Count() > 0) {
        GameEventClass ev = g_ScheduledEvents.Fetch_Head();
    }
    /*if (g_Session.Record_Game()) {
        bool executed = Execute_ScheduledEvents(1, g_PlayerPtr->What_Type());
        //captainslog_debug_CONDITIONAL(!executed, "Execute_ScheduledEvents() returned false in Queue_AI_Normal()!\n");
        if (executed) {
            Clean_ScheduledEvents();
        } else {
            captainslog_debug("Queue_AI_Normal() - Execute_ScheduledEvents() failed, setting g_GameActive to false");
            g_GameActive = false;
        }
    }*/
#endif
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

BOOL Queue_Mission_Formation(TargetClass whom, MissionType mission, target_t target, target_t dest, SpeedType form_speed, MPHType form_max_speed)
{
    GameEventClass ev(whom, mission, target, dest, form_speed, form_max_speed);
    return g_OutgoingEvents.Add(ev);
}

BOOL Execute_ScheduledEvents(int house_count, HousesType houses, ConnectionManagerClass *conn_mgr, TCountDownTimerClass<FrameTimerClass> *a4, signed int *a5, unsigned short *a6, unsigned short *a7)
{
#ifdef GAME_DLL
    BOOL (*func)(int, HousesType, ConnectionManagerClass *, TCountDownTimerClass<FrameTimerClass> *, signed int *, unsigned short *, unsigned short *) = reinterpret_cast<BOOL (*)(int, HousesType, ConnectionManagerClass *, TCountDownTimerClass<FrameTimerClass> *, signed int *, unsigned short *, unsigned short *)>(0x0052B69C);
    return func(house_count, houses, conn_mgr, a4, a5, a6, a7);
#else
    return false;
#endif
}

void Clean_ScheduledEvents(ConnectionManagerClass *conn_mgr)
{
#ifdef GAME_DLL
    void (*func)(ConnectionManagerClass *) = reinterpret_cast<void (*)(ConnectionManagerClass *)>(0x0052BC94);
    return func(conn_mgr);
#else
    while (g_ScheduledEvents.Get_Count() > 0) {
        GameEventClass ev = g_ScheduledEvents.Fetch_Tail();
        if (!ev.Is_Executed()) {
            if (ev.Get_Event_Frame() >= g_GameFrame) {
                break;
            }
        }
        g_ScheduledEvents.Remove_Tail();
    }
#endif
}
