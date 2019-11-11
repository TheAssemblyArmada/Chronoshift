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

//these were bits originally but to ensure the same packing across compilers we made it bit masks
enum CommFlags {
    FLAG_SEND_ISACTIVE = 1 << 0,
    FLAG_SEND_ISACK = 1 << 1,
    FLAG_RECEIVE_ISACTIVE = 1 << 0,
    FLAG_RECEIVE_ISREAD = 1 << 1,
    FLAG_RECEIVE_ISACK = 1 << 2
};

class CommBufferClass
{
public:
    struct SendQueueType
    {
        uint32_t m_Flags; // See CommFlags above.
        uint32_t m_FirstTime; // first send time?
        uint32_t m_LastTime; // last send attempt time if not acked?
        uint32_t m_SendCount;
        int32_t m_BufLen;
        uint8_t *m_Buffer;
        int32_t m_ExtraLen;
        uint8_t *m_ExtraBuffer;
    };

    struct ReceiveQueueType
    {
        uint32_t m_Flags; // See CommFlags above.
        int32_t m_BufLen;
        uint8_t *m_Buffer;
        int32_t m_ExtraLen;
        uint8_t *m_ExtraBuffer;
    };

public:
    CommBufferClass(int num_send, int num_receive, int max_len, int extra_len);
    virtual ~CommBufferClass();

    void Init();
    void Init_Send_Queue();
    void Init_Receive_Queue();
    int Queue_Send(void *packet, int buf_len, void *extra_buf, int extra_len);
    int UnQueue_Send(void *buf, int *buf_len, int index, void *extra_buf, int *extra_len);
    SendQueueType *Get_Send(int index);
    int Num_Send() { return m_SendCount; }
    int Max_Send() { return m_MaxSend; }
    void Grow_Send(int amount);
    int Queue_Receive(void *buf, int buf_len, void *extra_buf, int extra_len);
    int UnQueue_Receive(void *buf, int *buf_len, int index, void *extra_buf, int *extra_len);
    ReceiveQueueType *Get_Receive(int index);
    int Num_Receive() { return m_ReceiveCount; }
    int Max_Receive() { return m_MaxReceive; }
    void Grow_Receive(int amount);
    void Add_Delay(uint32_t delay);
    uint32_t Avg_Response_Time() { return m_MeanDelay; }
    uint32_t Max_Response_Time() { return m_MaxDelay; }
    void Reset_Response_Time();
    void Configure_Debug(int debug1, int debug2, char **debug3, int debug4, int debug5);
    void Mono_Debug_Print(int clear);
    void Mono_Debug_Print2(int clear);

protected:
    int32_t m_MaxSend; // How many send queue objects does this buffer instance supports
    int32_t m_MaxReceive; // How many recv queue objects does this buffer instance supports
    int32_t m_MaxPacketSize; // Length of the first buffer in the Queue types
    int32_t m_MaxExtraSize; // Length of the second buffer in the Queue types
    uint32_t m_DelaySum; // Related to delay somehow?
    uint32_t m_NumDelay; // Used in calculating average response?
    uint32_t m_MeanDelay;
    uint32_t m_MaxDelay;
    SendQueueType *m_SendQueue;
    int32_t m_SendCount;
    uint32_t m_SendTotal; // Always counts up? Count of total items processed?
    int32_t *m_SendIndex;
    ReceiveQueueType *m_ReceiveQueue;
    int32_t m_ReceiveCount;
    uint32_t m_ReceiveTotal; // Always counts up? Count of total items processed?
    int32_t *m_ReceiveIndex;
    int32_t m_Debug1;
    int32_t m_Debug2;
    char **m_Debug3;
    int32_t m_Debug4;
    int32_t m_Debug5;
};

#endif // COMMBUFF_H
