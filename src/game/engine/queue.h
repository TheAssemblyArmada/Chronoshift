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
#pragma once

#ifndef QUEUE_H
#define QUEUE_H

#include "always.h"
#include "gametypes.h"
#include "mission.h"

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
    TEventQueueClass(void) : Count(0), Head(0), Tail(0), List() {}
    ~TEventQueueClass(void) {}

    EVENT &operator[](unsigned int index)
    {
        DEBUG_ASSERT(index < SIZE);
        return List[index % SIZE];
    }

    const EVENT &operator[](unsigned int index) const
    {
        DEBUG_ASSERT(index < SIZE);
        return List[index % SIZE];
    }

    TEventQueueClass(const TEventQueueClass &that) :
        Count(that.Count), Head(that.Head), Tail(that.Tail)
    {
        memcpy(List, that.List, sizeof(List));
    }

    TEventQueueClass &operator=(const TEventQueueClass &that)
    {
        if (this != &that) {
            Count = that.Count;
            Head = that.Head;
            Tail = that.Tail;
            memcpy(List, that.List, sizeof(List));
        }
        return *this;
    }

    EVENT &Fetch_Head() { return List[Head]; }
    EVENT &Fetch_Tail() { return List[Tail]; }

    EVENT &Fetch_From_Head(int index) { return List[(index + Head) % SIZE]; }
    EVENT &Fetch_From_Tail(int index) { return List[(index + Tail) % SIZE]; }

    unsigned int Get_Count() const { return Count; }
    unsigned int Get_Capacity() const { return SIZE; }

    void Clear() { Count = 0; Head = 0; Tail = 0; memset(List, 0, sizeof(List)); }

    bool Add(EVENT &element)
    {
        if (Count >= SIZE) {
            DEBUG_LOG("TEventQueueClass::Add() - Failed! (Count:%d, Capacity:%d, Tail:%d, Head:%d).\n", Count, SIZE, Tail, Head);
            return false;
        }

        DEBUG_LOG("TEventQueueClass::Add() - Adding event '%s' to queue.\n", element.Name());

        List[Tail] = element;
        Tail = (Tail + 1) % SIZE;

        ++Count;

        return true;
    }

    unsigned int Remove_Head()
    {
        if (Count > 0) {
            Head = (Head + 1) % SIZE;
            --Count;
        }

        return Count;
    }

    unsigned int Remove_Tail()
    {
        if (Count > 0) {
            Tail = (Tail + 1) % SIZE;
            --Count;
        }

        return Count;
    }

private:
    unsigned int Count; // The number of items in the queue.
    unsigned int Head; // Index to the head (start) of the queue.
    unsigned int Tail; // Index to the tail (end) of the queue.
    EVENT List[SIZE];
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

RetcodeEnum Wait_For_Players(int, ConnectionManagerClass *conn_mgr, int, int, int, char *, int, long *, unsigned short *, unsigned short *);

void Generate_Timing_Event(ConnectionManagerClass *conn_mgr, int);
void Generate_Real_Timing_Event(ConnectionManagerClass *conn_mgr, int);
void Generate_Process_Time_Event(ConnectionManagerClass *conn_mgr);

BOOL Process_Send_Period(ConnectionManagerClass *conn_mgr);

int Send_Packets(ConnectionManagerClass *conn_mgr, char *, int, int, int);
void Send_FrameSync(ConnectionManagerClass *conn_mgr, int);

RetcodeEnum Process_Receive_Packet(ConnectionManagerClass *conn_mgr, char *, int, int, long *, unsigned short *, unsigned short *);
RetcodeEnum Process_Serial_Packet(char *, int);

BOOL Can_Advance(ConnectionManagerClass *conn_mgr, int, long *, unsigned short *, unsigned short *);
int Process_Reconnect_Dialog(TCountDownTimerClass<SystemTimerClass> *, long *, int, int, int);
int Handle_Timeout(ConnectionManagerClass *conn_mgr, long *, unsigned short *, unsigned short *);

int Build_Send_Packet(char *, int, int, int, int);
int Add_Uncompressed_Events(char *, int, int, int, int);
int Add_Compressed_Events(char *, int, int, int, int);
int Breakup_Receive_Packet(void *, int);
int Extract_Uncompressed_Events(void *, int);
int Extract_Compressed_Events(void *, int);
BOOL Execute_ScheduledEvents(int house_count, HousesType houses, ConnectionManagerClass *conn_mgr = nullptr, TCountDownTimerClass<FrameTimerClass> *a4 = nullptr, signed int *a5 = nullptr, unsigned short *a6 = nullptr, unsigned short *a7 = nullptr);
void Clean_ScheduledEvents(ConnectionManagerClass *conn_mgr = nullptr);

void Stop_Game();

void Compute_Game_CRC();
void Add_CRC(uint32_t *, uint32_t);
void Print_CRCs(GameEventClass *event = nullptr);

void Init_Queue_Mono(ConnectionManagerClass *conn_mgr);
void Update_Queue_Mono(ConnectionManagerClass *conn_mgr, int);
void Print_Framesync_Values(long, uint32_t, int, unsigned short *, unsigned short *, unsigned short);
void Dump_Packet_Too_Late_Stuff(GameEventClass *, ConnectionManagerClass *conn_mgr, long *, unsigned short *, unsigned short *);

void Check_Mirror();

extern uint32_t GameCRC;
extern uint32_t CRC[32];

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
extern TEventQueueClass<GameEventClass, OUTGOING_SIZE> &OutgoingEvents;
extern TEventQueueClass<GameEventClass, SCHEDULED_SIZE> &ScheduledEvents;
#else
extern TEventQueueClass<GameEventClass, OUTGOING_SIZE> OutgoingEvents;
extern TEventQueueClass<GameEventClass, SCHEDULED_SIZE> ScheduledEvents;
#endif

#endif // QUEUE_H
