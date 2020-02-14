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
#include <captainslog.h>
#include <cstring>

using std::memcpy;

/**
 * 0x004A3A60
 */
CommBufferClass::CommBufferClass(int num_send, int num_receive, int max_len, int extra_len) :
    m_MaxSend(num_send),
    m_MaxReceive(num_receive),
    m_MaxPacketSize(max_len),
    m_MaxExtraSize(extra_len),
    m_SendQueue(new SendQueueType[num_send]),
    m_SendIndex(new int32_t[num_send]),
    m_ReceiveQueue(new ReceiveQueueType[num_receive]),
    m_ReceiveIndex(new int32_t[num_receive])
{
    for (int i = 0; i < m_MaxSend; ++i) {
        m_SendQueue[i].m_Buffer = new uint8_t[max_len];

        if (m_MaxExtraSize <= 0) {
            m_SendQueue[i].m_ExtraBuffer = nullptr;
        } else {
            m_SendQueue[i].m_ExtraBuffer = new uint8_t[extra_len];
        }
    }

    for (int i = 0; i < m_MaxReceive; ++i) {
        m_ReceiveQueue[i].m_Buffer = new uint8_t[max_len];

        if (m_MaxExtraSize <= 0) {
            m_ReceiveQueue[i].m_ExtraBuffer = nullptr;
        } else {
            m_ReceiveQueue[i].m_ExtraBuffer = new uint8_t[extra_len];
        }
    }

    Init();
}

/**
 * 0x004A3B60
 */
CommBufferClass::~CommBufferClass()
{
    for (int i = 0; i < m_MaxSend; ++i) {
        delete[] m_SendQueue[i].m_Buffer;

        if (m_SendQueue[i].m_ExtraBuffer) {
            delete[] m_SendQueue[i].m_ExtraBuffer;
        }
    }

    for (int i = 0; i < m_MaxReceive; ++i) {
        delete[] m_ReceiveQueue[i].m_Buffer;

        if (m_ReceiveQueue[i].m_ExtraBuffer) {
            delete[] m_ReceiveQueue[i].m_ExtraBuffer;
        }
    }

    delete[] m_SendQueue;
    delete[] m_ReceiveQueue;
    delete[] m_SendIndex;
    delete[] m_ReceiveIndex;
}

/**
 * Initialise a comm buffer instance.
 *
 * 0x004A3C34
 */
void CommBufferClass::Init()
{
    m_SendTotal = 0;
    m_ReceiveTotal = 0;
    m_DelaySum = 0;
    m_NumDelay = 0;
    m_MeanDelay = 0;
    m_MaxDelay = 0;

    Init_Send_Queue();
    Init_Receive_Queue();

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
    m_SendCount = 0;
    for (int i = 0; i < m_MaxSend; ++i) {
        m_SendQueue[i].m_Flags &= ~(FLAG_SEND_ISACTIVE | FLAG_SEND_ISACK); // clear bits 1 and 2
        m_SendQueue[i].m_FirstTime = 0;
        m_SendQueue[i].m_LastTime = 0;
        m_SendQueue[i].m_SendCount = 0;
        m_SendQueue[i].m_BufLen = 0;
        m_SendQueue[i].m_ExtraLen = 0;
        m_SendIndex[i] = 0;
    }
}

/**
 * Initialise the receive queue.
 *
 *
 */
void CommBufferClass::Init_Receive_Queue()
{
    m_ReceiveCount = 0;
    for (int i = 0; i < m_MaxReceive; ++i) {
        m_SendQueue[i].m_Flags &= ~(FLAG_RECEIVE_ISACTIVE | FLAG_RECEIVE_ISREAD | FLAG_RECEIVE_ISACK); // clear bits 1, 2 and 3
        m_ReceiveQueue[i].m_BufLen = 0;
        m_ReceiveQueue[i].m_ExtraLen = 0;
        m_ReceiveIndex[i] = 0;
    }
}

/**
 * Queue some data to the send queue.
 *
 * 0x004A3DD4
 */
int CommBufferClass::Queue_Send(void *buf, int buf_len, void *extra_buf, int extra_len)
{
    captainslog_dbgassert(m_SendCount != m_MaxSend, "No room in send queue.\n");
    captainslog_dbgassert(buf_len <= m_MaxPacketSize, "Input length greater than m_MaxPacketSize.\n");

    if (m_SendCount != m_MaxSend && buf_len <= m_MaxPacketSize) {
        int free_index = -1;
        for (int i = 0; i < m_MaxSend; ++i) {
            if ((m_SendQueue[i].m_Flags & FLAG_SEND_ISACTIVE) == 0) {
                free_index = i;
                break;
            }
        }

        if (free_index == -1) {
            return 0;
        } else {
            m_SendQueue[free_index].m_Flags |= FLAG_SEND_ISACTIVE; // set bit 1
            m_SendQueue[free_index].m_Flags &= ~(FLAG_SEND_ISACK); // clear bit 2
            m_SendQueue[free_index].m_FirstTime = 0;
            m_SendQueue[free_index].m_LastTime = 0;
            m_SendQueue[free_index].m_SendCount = 0;
            m_SendQueue[free_index].m_BufLen = buf_len;
            memcpy(m_SendQueue[free_index].m_Buffer, buf, buf_len);

            // If we have a second buffer (possibly a header or meta data) and
            // it will fit in the queue object second buffer, copy it in.
            if (extra_buf && extra_len > 0 && extra_len <= m_MaxExtraSize) {
                memcpy(m_SendQueue[free_index].m_ExtraBuffer, extra_buf, extra_len);
                m_SendQueue[free_index].m_ExtraLen = extra_len;
            } else {
                m_SendQueue[free_index].m_ExtraLen = 0;
            }

            m_SendIndex[m_SendCount] = free_index;
            ++m_SendTotal;
            ++m_SendCount;

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
int CommBufferClass::UnQueue_Send(void *buf, int *buf_len, int index, void *extra_buf, int *extra_len)
{
    SendQueueType *entry;

    if (m_SendCount && (entry = &m_SendQueue[m_SendIndex[index]], entry->m_Flags & FLAG_SEND_ISACTIVE)) {
        if (buf) {
            memcpy(buf, entry->m_Buffer, entry->m_BufLen);
            *buf_len = entry->m_BufLen;
        }

        if (extra_buf && extra_len) {
            memcpy(extra_buf, entry->m_ExtraBuffer, entry->m_ExtraLen);
            *extra_len = entry->m_ExtraLen;
        }

        entry->m_Flags &= ~(FLAG_SEND_ISACTIVE | FLAG_SEND_ISACK); // clear bits 1 and 2
        entry->m_FirstTime = 0;
        entry->m_LastTime = 0;
        entry->m_SendCount = 0;
        entry->m_BufLen = 0;
        entry->m_ExtraLen = 0;
        --m_SendCount;

        while (index < m_SendCount) {
            ++index;
            m_SendIndex[index - 1] = m_SendIndex[index];
        }

        m_SendIndex[m_SendCount] = 0;

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
    SendQueueType *entry = &m_SendQueue[m_SendIndex[index]];

    if (!(entry->m_Flags & FLAG_SEND_ISACTIVE)) {
        return nullptr;
    }

    return entry;
}

/**
 * Grow the send queue by the specified number of entries.
 */
void CommBufferClass::Grow_Send(int amount)
{
    int newlength = amount + m_MaxSend;
    SendQueueType *newqueue = new SendQueueType[newlength];
    int32_t *newindex = new int32_t[newlength];

    for (int i = 0; i < m_MaxSend; ++i) {
        memcpy(&newqueue[i], &m_SendQueue[i], sizeof(SendQueueType));
        newindex[i] = m_SendIndex[i];
    }

    for (int j = m_MaxSend; j < newlength; ++j) {
        newqueue[j].m_Buffer = new uint8_t[m_MaxPacketSize];

        if (m_MaxExtraSize <= 0) {
            newqueue[j].m_ExtraBuffer = nullptr;
        } else {
            newqueue[j].m_ExtraBuffer = new uint8_t[m_MaxExtraSize];
        }

        newqueue[j].m_Flags &= ~(FLAG_SEND_ISACTIVE | FLAG_SEND_ISACK); // clear bits 1 and 2
        newqueue[j].m_FirstTime = 0;
        newqueue[j].m_LastTime = 0;
        newqueue[j].m_SendCount = 0;
        newqueue[j].m_BufLen = 0;
        newqueue[j].m_ExtraLen = 0;
        newindex[j] = 0;
    }

    delete[] m_SendQueue;
    delete[] m_SendIndex;
    m_MaxSend = newlength;
    m_SendQueue = newqueue;
    m_SendIndex = newindex;
}

/**
 * Queue some data to the receive queue.
 *
 * 0x004A4118
 */
int CommBufferClass::Queue_Receive(void *buf, int buf_len, void *extra_buf, int extra_len)
{
    captainslog_dbgassert(m_ReceiveCount != m_MaxReceive, "No room in recieve queue.\n");
    captainslog_dbgassert(buf_len <= m_MaxPacketSize, "Incoming packet larger than m_MaxPacketSize.\n");

    if (m_ReceiveCount != m_MaxReceive && buf_len <= m_MaxPacketSize) {
        int free_index = -1;

        for (int i = 0; i < m_MaxReceive; ++i) {
            if ((m_ReceiveQueue[i].m_Flags & FLAG_RECEIVE_ISACTIVE) == 0) {
                free_index = i;
                break;
            }
        }

        if (free_index == -1) {
            captainslog_dbgassert(false, "Can't queue incoming packet - Unable to find free slot in array.\n");

            return 0;
        } else {
            m_ReceiveQueue[free_index].m_Flags |= FLAG_RECEIVE_ISACTIVE; // set bit 1
            m_ReceiveQueue[free_index].m_Flags &= ~(FLAG_RECEIVE_ISREAD | FLAG_RECEIVE_ISACK); // clear bit 2 and 3
            m_ReceiveQueue[free_index].m_BufLen = buf_len;
            memcpy(m_ReceiveQueue[free_index].m_Buffer, buf, buf_len);

            // If we have a second buffer (possible a header or meta data) and
            // it will fit in the queue object second buffer, copy it in.
            if (extra_buf && extra_len > 0 && extra_len <= m_MaxExtraSize) {
                memcpy(m_ReceiveQueue[free_index].m_ExtraBuffer, extra_buf, extra_len);
                m_ReceiveQueue[free_index].m_ExtraLen = extra_len;
            } else {
                m_ReceiveQueue[free_index].m_ExtraLen = 0;
            }

            m_ReceiveIndex[m_ReceiveCount] = free_index;
            ++m_ReceiveTotal;
            ++m_ReceiveCount;

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
int CommBufferClass::UnQueue_Receive(void *buf, int *buf_len, int index, void *extra_buf, int *extra_len)
{
    ReceiveQueueType *entry;

    if (m_ReceiveCount && (entry = &m_ReceiveQueue[m_ReceiveIndex[index]], entry->m_Flags & FLAG_RECEIVE_ISACTIVE)) {
        if (buf) {
            memcpy(buf, entry->m_Buffer, entry->m_BufLen);
            *buf_len = entry->m_BufLen;
        }

        if (extra_buf && extra_len) {
            memcpy(extra_buf, entry->m_ExtraBuffer, entry->m_ExtraLen);
            *extra_len = entry->m_ExtraLen;
        }

        entry->m_Flags &= ~(FLAG_RECEIVE_ISACTIVE | FLAG_RECEIVE_ISREAD | FLAG_RECEIVE_ISACK); // clear bits 1, 2 and 3
        entry->m_BufLen = 0;
        entry->m_ExtraLen = 0;
        --m_ReceiveCount;

        while (index < m_ReceiveCount) {
            ++index;
            m_ReceiveIndex[index - 1] = m_ReceiveIndex[index];
        }

        m_ReceiveIndex[m_ReceiveCount] = 0;

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
    ReceiveQueueType *entry = &m_ReceiveQueue[m_ReceiveIndex[index]];

    if (!(entry->m_Flags & FLAG_RECEIVE_ISACTIVE)) {
        entry = nullptr;
    }

    return entry;
}

/**
 * Grow the recieve queue by the specified number of entries.
 */
void CommBufferClass::Grow_Receive(int amount)
{
    int newlength = amount + m_MaxReceive;
    ReceiveQueueType *newqueue = new ReceiveQueueType[newlength];
    int32_t *newindex = new int32_t[newlength];

    for (int i = 0; i < m_MaxReceive; ++i) {
        memcpy(&newqueue[i], &m_ReceiveQueue[i], sizeof(ReceiveQueueType));
        newindex[i] = m_ReceiveIndex[i];
    }

    for (int j = m_MaxReceive; j < newlength; ++j) {
        newqueue[j].m_Buffer = new uint8_t[m_MaxPacketSize];

        if (m_MaxExtraSize <= 0) {
            newqueue[j].m_ExtraBuffer = nullptr;
        } else {
            newqueue[j].m_ExtraBuffer = new uint8_t[m_MaxExtraSize];
        }

        newqueue[j].m_Flags &= ~(FLAG_RECEIVE_ISACTIVE | FLAG_RECEIVE_ISREAD | FLAG_RECEIVE_ISACK); // clear bits 1, 2 and 3
        newqueue[j].m_BufLen = 0;
        newqueue[j].m_ExtraLen = 0;
        newindex[j] = 0;
    }

    delete[] m_ReceiveQueue;
    delete[] m_ReceiveIndex;
    m_MaxReceive = newlength;
    m_ReceiveQueue = newqueue;
    m_ReceiveIndex = newindex;
}

/**
 * Add to the allowable send/recieve ack delay maybe?
 *
 * 0x004A4464
 */
void CommBufferClass::Add_Delay(uint32_t delay)
{
    int roundoff = 0;
    if (m_NumDelay == 256) {
        m_DelaySum -= m_MeanDelay;
        m_DelaySum += delay;
        if ((m_DelaySum & 255) > 127) {
            roundoff = 1;
        }
        m_MeanDelay = roundoff + (m_DelaySum / 256);
    } else {
        m_DelaySum += delay;
        ++m_NumDelay;
        m_MeanDelay = m_DelaySum / m_NumDelay;
    }

    if (delay > m_MaxDelay) {
        m_MaxDelay = delay;
    }
}

/**
 * Resets response time related data.
 *
 * 0x004A44D4
 */
void CommBufferClass::Reset_Response_Time()
{
    m_DelaySum = 0;
    m_NumDelay = 0;
    m_MeanDelay = 0;
    m_MaxDelay = 0;
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
