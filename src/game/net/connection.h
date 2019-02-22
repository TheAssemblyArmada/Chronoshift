/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Base class for handling network connections.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef CONNECTION_H
#define CONNECTION_H

#include "always.h"

class CommBufferClass;

enum CommandType
{
    COMMAND_ADATA,
    COMMAND_DATA,
    COMMAND_ACK,
    COMMAND_COUNT,

    COMMAND_FIRST = COMMAND_ADATA,
    COMMAND_NONE = -1,
};

class ConnectionClass
{
#pragma pack(push,1)
    struct PacketHeader {
        uint16_t Type;
        uint8_t Command;
        int32_t Number;
    };
#pragma pack(pop)

public:
    ConnectionClass(int32_t squeue_size, int32_t rqueue_size, int32_t buff_size, uint16_t type, uint32_t retry_delay,
        uint32_t max_retries, uint32_t retry_timeout, int32_t exbuffsize);
    virtual ~ConnectionClass();

    virtual void Init();
    virtual BOOL Send_Packet(void *data, uint32_t datalen, BOOL flag);
    virtual BOOL Receive_Packet(void *data, int32_t datalen);
    virtual BOOL Get_Packet(void *data, int32_t *datalen);
    virtual BOOL Service();
    virtual BOOL Service_Send_Queue();
    virtual BOOL Service_Receive_Queue();
    virtual int Send(uint8_t *data, int32_t datalen, void *ack_data, int32_t ack_datalen) = 0;

    static uint32_t Time();
    static char const *Command_Name(CommandType command);

protected:
    CommBufferClass *m_CommBuffer;
    int32_t m_BufferSize;
    uint8_t *m_PacketBuffer;
    uint16_t m_PacketType;
    uint32_t m_SendRetryDelay;
    uint32_t m_SendMaxRetries;
    uint32_t m_SendRetryTimeout;
    uint32_t m_PacketsReceived;
    int32_t m_QueuedPackets;
    uint32_t m_SequenceNum;
    uint32_t m_SequenceNumAck;
    uint32_t m_QueuedSeqenceNum;
    uint32_t m_RecvSequenceNum;

private:
    static char const *s_Commands[COMMAND_COUNT];
};

#endif // CONNECTION_H
