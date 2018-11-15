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
#include "commbuff.h"
#include <cstring>

using std::memcpy;

/**
 * 0x004A3A60
 */
CommBufferClass::CommBufferClass(int snd_buff_size, int rcv_buff_size, int buff1_size, int buff2_size) :
    m_SendQueueLength(snd_buff_size),
    m_RecvQueueLength(rcv_buff_size),
    m_QueueTypeBuff1Len(buff1_size),
    m_QueueTypeBuff2Len(buff2_size),
    m_SendQueue(new SendQueueType[snd_buff_size]),
    m_SendIndex(new uint32_t[snd_buff_size]),
    m_RecvQueue(new ReceiveQueueType[rcv_buff_size]),
    m_RecvIndex(new uint32_t[rcv_buff_size])
{
    for (int i = 0; i < m_SendQueueLength; ++i) {
        m_SendQueue[i].m_Buff1 = new uint8_t[buff1_size];

        if (m_QueueTypeBuff2Len <= 0) {
            m_SendQueue[i].m_Buff2 = nullptr;
        } else {
            m_SendQueue[i].m_Buff2 = new uint8_t[buff2_size];
        }
    }

    for (int i = 0; i < m_RecvQueueLength; ++i) {
        m_RecvQueue[i].m_Buff1 = new uint8_t[buff1_size];

        if (m_QueueTypeBuff2Len <= 0) {
            m_RecvQueue[i].m_Buff2 = nullptr;
        } else {
            m_RecvQueue[i].m_Buff2 = new uint8_t[buff2_size];
        }
    }

    Init();
}

/**
 * 0x004A3B60
 */
CommBufferClass::~CommBufferClass()
{
    for (int i = 0; i < m_SendQueueLength; ++i) {
        delete[] m_SendQueue[i].m_Buff1;

        if (m_SendQueue[i].m_Buff2) {
            delete[] m_SendQueue[i].m_Buff2;
        }
    }

    for (int i = 0; i < m_RecvQueueLength; ++i) {
        delete[] m_RecvQueue[i].m_Buff1;

        if (m_RecvQueue[i].m_Buff2) {
            delete[] m_RecvQueue[i].m_Buff2;
        }
    }

    delete[] m_SendQueue;
    delete[] m_RecvQueue;
    delete[] m_SendIndex;
    delete[] m_RecvIndex;
}

/**
 * Initialise a comm buffer instance.
 *
 * 0x004A3C34
 */
void CommBufferClass::Init()
{
    m_SendQueuedCounter = 0;
    m_RecvQueuedCounter = 0;
    m_Delay = 0;
    m_ResponseCount = 0;
    m_AvgResponse = 0;
    m_MaxResponse = 0;
    m_SendQueuePos = 0;
    m_RecvQueuePos = 0;

    for (int i = 0; i < m_SendQueueLength; ++i) {
        // GETBYTE32(m_SendQueue[i].m_Flags, 0) &= 0xFEu;
        // GETBYTE32(m_SendQueue[i].m_Flags, 0) &= 0xFDu;
        // Do same as above with one AND, probably macros originally or enum
        m_SendQueue[i].m_Flags &= ~(COMM_FLAG_INUSE | COMM_FLAG_SNDACK); // clear bits 1 and 2
        m_SendQueue[i].m_InitialSendTime = 0;
        m_SendQueue[i].m_LastSendTime = 0;
        m_SendQueue[i].m_Retries = 0;
        m_SendQueue[i].m_Buff1DataLen = 0;
        m_SendQueue[i].m_Buff2DataLen = 0;
        m_SendIndex[i] = 0;
    }

    for (int i = 0; i < m_RecvQueueLength; ++i) {
        // GETBYTE32(m_RecvQueue[i].m_Flags) &= 0xFEu;
        // GETBYTE32(m_RecvQueue[i].m_Flags) &= 0xFDu;
        // GETBYTE32(m_RecvQueue[i].m_Flags) &= 0xFBu;
        // Do same as above with one AND, probably macros originally or enum
        m_RecvQueue[i].m_Flags &= ~(COMM_FLAG_INUSE | COMM_FLAG_SNDACK | COMM_FLAG_RECACK); // clear bits 1, 2 and 3
        m_RecvQueue[i].m_Buff1DataLen = 0;
        m_RecvQueue[i].m_Buff2DataLen = 0;
        m_RecvIndex[i] = 0;
    }

    m_Debug1 = 0;
    m_Debug2 = 0;
    m_Debug3 = nullptr;
    m_Debug5 = 0;
}

/**
 * Initialise the send queue.
 *
 * 0x004A3D54
 */
void CommBufferClass::Init_Send_Queue()
{
    m_SendQueuePos = 0;
    for (int i = 0; i < m_SendQueueLength; ++i) {
        // GETBYTE32(m_SendQueue[i].m_Flags, 0) &= 0xFEu;
        // GETBYTE32(m_SendQueue[i].m_Flags, 0) &= 0xFDu;
        // Do same as above with one AND, probably macros originally or enum
        m_SendQueue[i].m_Flags &= ~(COMM_FLAG_INUSE | COMM_FLAG_SNDACK); // clear bits 1 and 2
        m_SendQueue[i].m_InitialSendTime = 0;
        m_SendQueue[i].m_LastSendTime = 0;
        m_SendQueue[i].m_Retries = 0;
        m_SendQueue[i].m_Buff1DataLen = 0;
        m_SendQueue[i].m_Buff2DataLen = 0;
        m_SendIndex[i] = 0;
    }
}

/**
 * Queue some data to the send queue.
 *
 * 0x004A3DD4
 */
int CommBufferClass::Queue_Send(void *src, int src_len, void *buff2, int buff2_len)
{
    if (m_SendQueuePos != m_SendQueueLength && src_len <= m_QueueTypeBuff1Len) {
        int free_index = -1;
        for (int i = 0; i < m_SendQueueLength; ++i) {
            if ((m_SendQueue[i].m_Flags & COMM_FLAG_INUSE) == 0) {
                free_index = i;
                break;
            }
        }

        if (free_index == -1) {
            return 0;
        } else {
            m_SendQueue[free_index].m_Flags |= COMM_FLAG_INUSE; // set bit 1
            m_SendQueue[free_index].m_Flags &= ~(COMM_FLAG_SNDACK); // clear bit 2
            m_SendQueue[free_index].m_InitialSendTime = 0;
            m_SendQueue[free_index].m_LastSendTime = 0;
            m_SendQueue[free_index].m_Retries = 0;
            m_SendQueue[free_index].m_Buff1DataLen = src_len;
            memcpy(m_SendQueue[free_index].m_Buff1, src, src_len);

            // If we have a second buffer (possibly a header or meta data) and
            // it will fit in the queue object second buffer, copy it in.
            if (buff2 && buff2_len > 0 && buff2_len <= m_QueueTypeBuff2Len) {
                memcpy(m_SendQueue[free_index].m_Buff2, buff2, buff2_len);
                m_SendQueue[free_index].m_Buff2DataLen = buff2_len;
            } else {
                m_SendQueue[free_index].m_Buff2DataLen = 0;
            }

            m_SendIndex[m_SendQueuePos] = free_index;
            ++m_SendQueuedCounter;
            ++m_SendQueuePos;

            return 1;
        }
    }

    return 0;
}

/**
 * Remove some data from the send queue.
 *
 * 0x004A3F24
 */
int CommBufferClass::UnQueue_Send(void *dst, int *dst_len, int index, void *buff2, int *buff2_len)
{
    SendQueueType *entry;

    if (m_SendQueuePos && (entry = &m_SendQueue[m_SendIndex[index]], entry->m_Flags & 1)) {
        if (dst) {
            memcpy(dst, entry->m_Buff1, entry->m_Buff1DataLen);
            *dst_len = entry->m_Buff1DataLen;
        }

        if (buff2 && buff2_len) {
            memcpy(buff2, entry->m_Buff2, entry->m_Buff2DataLen);
            *buff2_len = entry->m_Buff2DataLen;
        }

        // GETBYTE32(m_SendQueue[i].m_Flags, 0) &= 0xFEu;
        // GETBYTE32(m_SendQueue[i].m_Flags, 0) &= 0xFDu;
        // Do same as above with one AND, probably macros originally or enum
        entry->m_Flags &= ~(COMM_FLAG_INUSE | COMM_FLAG_SNDACK); // clear bits 1 and 2
        entry->m_InitialSendTime = 0;
        entry->m_LastSendTime = 0;
        entry->m_Retries = 0;
        entry->m_Buff1DataLen = 0;
        entry->m_Buff2DataLen = 0;
        --m_SendQueuePos;

        while (index < m_SendQueuePos) {
            ++index;
            m_SendIndex[index - 1] = m_SendIndex[index];
        }

        m_SendIndex[m_SendQueuePos] = 0;

        return 1;
    }

    return 0;
}

/**
 * Get a specific entry in the send queue.
 *
 * 0x004A40F0
 */
CommBufferClass::SendQueueType *CommBufferClass::Get_Send(int index)
{
    SendQueueType *result;

    result = &m_SendQueue[m_SendIndex[index]];

    if (!(result->m_Flags & COMM_FLAG_INUSE)) {
        return nullptr;
    }

    return result;
}

/**
 * Grow the send queue by the specified number of entries.
 */
void CommBufferClass::Grow_Send(int amount)
{
    int newlength = amount + m_SendQueueLength;
    SendQueueType *newqueue = new SendQueueType[newlength];
    uint32_t *newindex = new uint32_t[newlength];

    for (int i = 0; i < m_SendQueueLength; ++i) {
        memcpy(&newqueue[i], &m_SendQueue[i], sizeof(SendQueueType));
        newindex[i] = m_SendIndex[i];
    }

    for (int j = m_SendQueueLength; j < newlength; ++j) {
        newqueue[j].m_Buff1 = new uint8_t[m_QueueTypeBuff1Len];

        if (m_QueueTypeBuff2Len <= 0) {
            newqueue[j].m_Buff2 = nullptr;
        } else {
            newqueue[j].m_Buff2 = new uint8_t[m_QueueTypeBuff2Len];
        }

        // GETBYTE32(m_SendQueue[i].m_Flags, 0) &= 0xFEu;
        // GETBYTE32(m_SendQueue[i].m_Flags, 0) &= 0xFDu;
        // Do same as above with one AND, probably macros originally or enum
        newqueue[j].m_Flags &= ~(COMM_FLAG_INUSE | COMM_FLAG_SNDACK); // clear bits 1 and 2
        newqueue[j].m_InitialSendTime = 0;
        newqueue[j].m_LastSendTime = 0;
        newqueue[j].m_Retries = 0;
        newqueue[j].m_Buff1DataLen = 0;
        newqueue[j].m_Buff2DataLen = 0;
        newindex[j] = 0;
    }

    delete[] m_SendQueue;
    delete[] m_SendIndex;
    m_SendQueueLength = newlength;
    m_SendQueue = newqueue;
    m_SendIndex = newindex;
}

/**
 * Queue some data to the receive queue.
 *
 * 0x004A4118
 */
int CommBufferClass::Queue_Receive(void *src, int src_len, void *buff2, int buff2_len)
{
    if (m_RecvQueuePos != m_RecvQueueLength && src_len <= m_QueueTypeBuff1Len) {
        int free_index = -1;
        for (int i = 0; i < m_RecvQueueLength; ++i) {
            if ((m_RecvQueue[i].m_Flags & COMM_FLAG_INUSE) == 0) {
                free_index = i;
                break;
            }
        }

        if (free_index == -1) {
            return 0;
        } else {
            m_RecvQueue[free_index].m_Flags |= COMM_FLAG_INUSE; // set bit 1
            // GETBYTE32(m_RecvQueue[free_index].m_Flags, 0) &= 0xFDu; //clear bit 2
            // GETBYTE32(m_RecvQueue[free_index].m_Flags, 0) &= 0xFBu; //clear bit 3
            m_RecvQueue[free_index].m_Flags &= ~(COMM_FLAG_SNDACK | COMM_FLAG_RECACK); // clear bit 2 and 3
            m_RecvQueue[free_index].m_Buff1DataLen = src_len;
            memcpy(m_RecvQueue[free_index].m_Buff1, src, src_len);

            // If we have a second buffer (possible a header or meta data) and
            // it will fit in the queue object second buffer, copy it in.
            if (buff2 && buff2_len > 0 && buff2_len <= m_QueueTypeBuff2Len) {
                memcpy(m_RecvQueue[free_index].m_Buff2, buff2, buff2_len);
                m_RecvQueue[free_index].m_Buff2DataLen = buff2_len;
            } else {
                m_RecvQueue[free_index].m_Buff2DataLen = 0;
            }

            m_RecvIndex[m_RecvQueuePos] = free_index;
            ++m_RecvQueuedCounter;
            ++m_RecvQueuePos;

            return 1;
        }
    }

    return 0;
}

/**
 * Remove some data from the receive queue.
 *
 * 0x004A4260
 */
int CommBufferClass::UnQueue_Receive(void *dst, int *dst_len, int index, void *buff2, int *buff2_len)
{
    ReceiveQueueType *entry;

    if (m_RecvQueuePos && (entry = &m_RecvQueue[m_RecvIndex[index]], entry->m_Flags & COMM_FLAG_INUSE)) {
        if (dst) {
            memcpy(dst, entry->m_Buff1, entry->m_Buff1DataLen);
            *dst_len = entry->m_Buff1DataLen;
        }

        if (buff2 && buff2_len) {
            memcpy(buff2, entry->m_Buff2, entry->m_Buff2DataLen);
            *buff2_len = entry->m_Buff2DataLen;
        }

        // GETBYTE32(m_RecvQueue[i].m_Flags, 0) &= 0xFEu;
        // GETBYTE32(m_RecvQueue[i].m_Flags, 0) &= 0xFDu;
        // GETBYTE32(m_RecvQueue[i].m_Flags, 0) &= 0xFBu;
        // Do same as above with one AND, probably macros originally or enum
        entry->m_Flags &= ~(COMM_FLAG_INUSE | COMM_FLAG_SNDACK | COMM_FLAG_RECACK); // clear bits 1, 2 and 3
        entry->m_Buff1DataLen = 0;
        entry->m_Buff2DataLen = 0;
        --m_RecvQueuePos;

        while (index < m_RecvQueuePos) {
            ++index;
            m_RecvIndex[index - 1] = m_RecvIndex[index];
        }

        m_RecvIndex[m_RecvQueuePos] = 0;

        return 1;
    }

    return 0;
}

/**
 * Get the specified entry in the recieve queue.
 *
 * 0x004A4438
 */
CommBufferClass::ReceiveQueueType *CommBufferClass::Get_Receive(int index)
{
    ReceiveQueueType *result;

    result = &m_RecvQueue[m_RecvIndex[index]];

    if (!(result->m_Flags & COMM_FLAG_INUSE)) {
        result = nullptr;
    }

    return result;
}

/**
 * Grow the recieve queue by the specified number of entries.
 */
void CommBufferClass::Grow_Receive(int amount)
{
    int newlength = amount + m_RecvQueueLength;
    ReceiveQueueType *newqueue = new ReceiveQueueType[newlength];
    uint32_t *newindex = new uint32_t[newlength];

    for (int i = 0; i < m_RecvQueueLength; ++i) {
        memcpy(&newqueue[i], &m_RecvQueue[i], sizeof(ReceiveQueueType));
        newindex[i] = m_RecvIndex[i];
    }

    for (int j = m_RecvQueueLength; j < newlength; ++j) {
        newqueue[j].m_Buff1 = new uint8_t[m_QueueTypeBuff1Len];

        if (m_QueueTypeBuff2Len <= 0) {
            newqueue[j].m_Buff2 = nullptr;
        } else {
            newqueue[j].m_Buff2 = new uint8_t[m_QueueTypeBuff2Len];
        }

        // GETBYTE32(m_RecvQueue[i].m_Flags, 0) &= 0xFEu;
        // GETBYTE32(m_RecvQueue[i].m_Flags, 0) &= 0xFDu;
        // Do same as above with one AND, probably macros originally or enum
        newqueue[j].m_Flags &= ~(COMM_FLAG_INUSE | COMM_FLAG_SNDACK); // clear bits 1 and 2
        newqueue[j].m_Buff1DataLen = 0;
        newqueue[j].m_Buff2DataLen = 0;
        newindex[j] = 0;
    }

    delete[] m_RecvQueue;
    delete[] m_RecvIndex;
    m_RecvQueueLength = newlength;
    m_RecvQueue = newqueue;
    m_RecvIndex = newindex;
}

/**
 * Add to the allowable send/recieve ack delay maybe?
 *
 * 0x004A4464
 */
void CommBufferClass::Add_Delay(uint32_t delay)
{
    if (m_ResponseCount == 256) {
        m_Delay -= m_AvgResponse;
        m_Delay += delay;
        m_AvgResponse = (uint32_t)m_Delay / 256;

        if (m_Delay < 256) {
            ++m_AvgResponse;
        }
    } else {
        m_Delay += delay;
        ++m_ResponseCount;
        m_AvgResponse = m_Delay / m_ResponseCount;
    }

    if (delay > m_MaxResponse) {
        m_MaxResponse = delay;
    }
}

/**
 * Resets response time related data.
 *
 * 0x004A44D4
 */
void CommBufferClass::Reset_Response_Time()
{
    m_Delay = 0;
    m_ResponseCount = 0;
    m_AvgResponse = 0;
    m_MaxResponse = 0;
}

/**
 * Configures data for the mono debug print functions.
 *
 * 0x004A4510
 */
void CommBufferClass::Configure_Debug(int debug1, int debug2, char **debug3, int debug4, int debug5)
{
    m_Debug1 = debug1;
    m_Debug2 = debug2;
    m_Debug3 = debug3;
    m_Debug4 = debug4;
    m_Debug5 = debug5;
}

/**
 * 0x004A4530
 */
void CommBufferClass::Mono_Debug_Print(int clear)
{
    // TODO prints debug information for a mono debug terminal.
}

/**
 * 0x004A47B0
 */
void CommBufferClass::Mono_Debug_Print2(int clear)
{
    // TODO prints debug information for a mono debug terminal.
}
