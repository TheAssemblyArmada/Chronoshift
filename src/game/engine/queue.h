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
#pragma once

#ifndef QUEUE_H
#define QUEUE_H

#include "always.h"
#include "gametypes.h"
#include "mission.h"
#include "ostimer.h"
#include <captainslog.h>

class TargetClass;
class GameEventClass;
class ConnectionManagerClass;

enum RetcodeEnum
{
    // TODO
};

enum EventQueueSize
{
    OUTGOING_SIZE = 64,
    SCHEDULED_SIZE = 4096
};

template<class EVENT, int SIZE>
class TEventQueueClass
{
public:
    TEventQueueClass() : m_List() { Clear(); }
    ~TEventQueueClass() {}

    EVENT &Fetch_Head() { return m_List[m_Head]; }

    EVENT &Fetch_From_Head(int index) { return m_List[(index + m_Head) % SIZE]; }

    unsigned int Get_Count() const { return m_Count; }
    unsigned int Get_Capacity() const { return SIZE; }

    void Clear() { m_Count = 0; m_Head = 0; m_Tail = 0; }

    bool Add(EVENT &element)
    {
        if (m_Count >= SIZE) {
            captainslog_debug("TEventQueueClass::Add() - Failed! (Count:%d, Size:%d, Tail:%d, Head:%d).", m_Count, SIZE, m_Tail, m_Head);
            return false;
        }

        captainslog_debug("TEventQueueClass::Add() - Adding event '%s' to queue.", element.Name());

        m_List[m_Tail] = element;

#if (CHRONOSHIFT_STANDALONE && CHRONOSHIFT_DEBUG)
        m_Time[m_Tail] = PlatformTimerClass::Get_Time();
#endif
        m_Tail = (m_Tail + 1) % SIZE;

        ++m_Count;

        return true;
    }

    unsigned int Remove_Head()
    {
        if (m_Count > 0) {
            m_Head = (m_Head + 1) % SIZE;
            --m_Count;
        }

        return m_Count;
    }

private:
    unsigned int m_Count; // The number of items in the queue.
    unsigned int m_Head; // Index to the head (start) of the queue.
    unsigned int m_Tail; // Index to the tail (end) of the queue.
    EVENT m_List[SIZE];
#if (CHRONOSHIFT_STANDALONE && CHRONOSHIFT_DEBUG)
    unsigned int m_Time[SIZE]; // Time in milliseconds when the event was added.
#endif
};

BOOL Queue_Options();
BOOL Queue_Exit();

void Queue_Record();
void Queue_Playback();

void Queue_AI();
void Queue_AI_Normal();
void Queue_AI_Network();
void Queue_AI_Multiplayer();

BOOL Queue_Mission(TargetClass whom, MissionType mission, target_t target, target_t dest);
BOOL Queue_Mission_Formation(TargetClass whom, MissionType mission, target_t target, target_t dest, SpeedType form_speed, MPHType form_max_speed);

RetcodeEnum Wait_For_Players(int, ConnectionManagerClass *conn_mgr, int, int, int, char *, int, long *, uint16_t *, uint16_t *);

void Generate_Timing_Event(ConnectionManagerClass *conn_mgr, int);
void Generate_Real_Timing_Event(ConnectionManagerClass *conn_mgr, int);
void Generate_Process_Time_Event(ConnectionManagerClass *conn_mgr);

BOOL Process_Send_Period(ConnectionManagerClass *conn_mgr);

int Send_Packets(ConnectionManagerClass *conn_mgr, char *, int, int, int);
void Send_FrameSync(ConnectionManagerClass *conn_mgr, int);

RetcodeEnum Process_Receive_Packet(ConnectionManagerClass *conn_mgr, char *, int, int, long *, uint16_t *, uint16_t *);
RetcodeEnum Process_Serial_Packet(char *, int);

BOOL Can_Advance(ConnectionManagerClass *conn_mgr, int, long *, uint16_t *, uint16_t *);
int Process_Reconnect_Dialog(TCountDownTimerClass<SystemTimerClass> *, long *, int, int, int);
int Handle_Timeout(ConnectionManagerClass *conn_mgr, long *, uint16_t *, uint16_t *);

int Build_Send_Packet(char *, int, int, int, int);
int Add_Uncompressed_Events(char *, int, int, int, int);
int Add_Compressed_Events(char *, int, int, int, int);
int Breakup_Receive_Packet(void *, int);
int Extract_Uncompressed_Events(void *, int);
int Extract_Compressed_Events(void *, int);
BOOL Execute_Scheduled_Events(int house_count, HousesType houses, ConnectionManagerClass *conn_mgr = nullptr, TCountDownTimerClass<FrameTimerClass> *a4 = nullptr, int *a5 = nullptr, uint16_t *a6 = nullptr, uint16_t *a7 = nullptr);
void Clean_Scheduled_Events(ConnectionManagerClass *conn_mgr = nullptr);

void Stop_Game();

void Compute_Game_CRC();
void Add_CRC(uint32_t *, uint32_t);
void Print_CRCs(GameEventClass *event = nullptr);

void Init_Queue_Mono(ConnectionManagerClass *conn_mgr);
void Update_Queue_Mono(ConnectionManagerClass *conn_mgr, int);
void Print_Framesync_Values(long, uint32_t, int, uint16_t *, uint16_t *, uint16_t);
void Dump_Packet_Too_Late_Stuff(GameEventClass *, ConnectionManagerClass *conn_mgr, long *, uint16_t *, uint16_t *);

void Check_Mirror();

extern uint32_t g_GameCRC;
extern uint32_t g_CRC[32];

#ifdef GAME_DLL
extern TEventQueueClass<GameEventClass, OUTGOING_SIZE> &g_OutgoingEvents;
extern TEventQueueClass<GameEventClass, SCHEDULED_SIZE> &g_ScheduledEvents;
#else

extern TEventQueueClass<GameEventClass, OUTGOING_SIZE> g_OutgoingEvents;
extern TEventQueueClass<GameEventClass, SCHEDULED_SIZE> g_ScheduledEvents;
#endif

#endif // QUEUE_H
