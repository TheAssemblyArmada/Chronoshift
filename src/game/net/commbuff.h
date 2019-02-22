/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Class for buffering data from networking IO.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef COMMBUFF_H
#define COMMBUFF_H

#include "always.h"

#define COMM_FLAG_INUSE 1
#define COMM_FLAG_SNDACK 2
#define COMM_FLAG_RECACK 4

class CommBufferClass
{
public:
    struct SendQueueType
    {
        uint32_t m_Flags;
        uint32_t m_InitialSendTime; // first send time?
        uint32_t m_LastSendTime; // last send attempt time if not acked?
        uint32_t m_AckToConnection;
        int32_t m_DataLength;
        uint8_t *m_DataBuffer;
        int32_t m_AckToConnectionLength;
        uint8_t *m_AckToConnectionBuffer;
    };

    struct ReceiveQueueType
    {
        uint32_t m_Flags;
        int32_t m_DataLength;
        uint8_t *m_DataBuffer;
        int32_t m_AckToConnectionLength;
        uint8_t *m_AckToConnectionBuffer;
    };

public:
    CommBufferClass(int snd_buff_size, int rcv_buff_size, int buff1_size, int buff2_size);
    virtual ~CommBufferClass();

    void Init();
    void Init_Send_Queue();
    int Queue_Send(void *src, int src_len, void *buff2, int buff2_len);
    int UnQueue_Send(void *dst, int *dst_len, int index, void *buff2, int *buff2_len);
    SendQueueType *Get_Send(int index);
    int Num_Send() { return m_SendQueuePos; }
    int Max_Send() { return m_SendQueueLength; }
    void Grow_Send(int amount);
    int Queue_Receive(void *src, int src_len, void *buff2, int buff2_len);
    int UnQueue_Receive(void *dst, int *dst_len, int index, void *buff2, int *buff2_len);
    ReceiveQueueType *Get_Receive(int index);
    int Num_Receive() { return m_RecvQueuePos; }
    int Max_Receive() { return m_RecvQueueLength; }
    void Grow_Receive(int amount);
    void Add_Delay(uint32_t delay);
    uint32_t Avg_Response_Time() { return m_AvgResponse; }
    uint32_t Max_Response_Time() { return m_MaxResponse; }
    void Reset_Response_Time();
    void Configure_Debug(int debug1, int debug2, char **debug3, int debug4, int debug5);
    void Mono_Debug_Print(int clear);
    void Mono_Debug_Print2(int clear);

protected:
    int m_SendQueueLength; // How many send queue objects does this buffer instance supports
    int m_RecvQueueLength; // How many recv queue objects does this buffer instance supports
    int m_MaxPacketSize; // Length of the first buffer in the Queue types
    int m_MaxAckPacketSize; // Length of the second buffer in the Queue types
    int m_Delay; // Related to delay somehow?
    int m_ResponseCount; // Used in calculating average response?
    uint32_t m_AvgResponse;
    uint32_t m_MaxResponse;
    SendQueueType *m_SendQueue;
    int m_SendQueuePos;
    int m_SendQueuedCounter; // Always counts up? Count of total items processed?
    uint32_t *m_SendIndex;
    ReceiveQueueType *m_RecvQueue;
    int m_RecvQueuePos;
    int m_RecvQueuedCounter; // Always counts up? Count of total items processed?
    uint32_t *m_RecvIndex;
    int m_Debug1;
    int m_Debug2;
    char **m_Debug3;
    int m_Debug4;
    int m_Debug5;
};

#endif // COMMBUFF_H
