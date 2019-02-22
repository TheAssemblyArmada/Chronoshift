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
#include "connection.h"
#include "commbuff.h"
#include "endiantype.h"
#include "gamedebug.h"
#include "ostimer.h"
#include "ttimer.h"
#include <cstring>

using std::memcpy;

// Some type defs to make referring to these shorter.
typedef CommBufferClass::ReceiveQueueType *RQEntry;
typedef CommBufferClass::SendQueueType *SQEntry;

const char *ConnectionClass::s_Commands[COMMAND_COUNT] = {
    "ADATA",
    "NDATA",
    "ACK",
};

/**
 * 0x004A4C90
 */
ConnectionClass::ConnectionClass(
    int32_t squeue_size, int32_t rqueue_size, int32_t buff_size, uint16_t type, uint32_t retry_delay, uint32_t max_retries, uint32_t retry_timeout, int32_t exbuffsize)
{
    m_BufferSize = buff_size + sizeof(PacketHeader);
    m_PacketType = type;
    m_SendRetryDelay = retry_delay;
    m_SendMaxRetries = max_retries;
    m_SendRetryTimeout = retry_timeout;
    m_PacketBuffer = new uint8_t[m_BufferSize];
    m_CommBuffer = new CommBufferClass(squeue_size, rqueue_size, m_BufferSize, exbuffsize);
}

/**
 * 0x004A4CF0
 */
ConnectionClass::~ConnectionClass()
{
    delete[] m_PacketBuffer;
    delete m_CommBuffer;
}

/**
 * Initialise the class instance.
 *
 * 0x004A4D20
 */
void ConnectionClass::Init()
{
    m_PacketsReceived = 0;
    m_QueuedPackets = 0;
    m_SequenceNum = 0;
    m_SequenceNumAck = 0;
    m_QueuedSeqenceNum = -1;
    m_RecvSequenceNum = -1;
    m_CommBuffer->Init();
}

/**
 * Constructs a 7 byte packet header, appends the data and sumbits it to the send queue.
 *
 * 0x004A4D58
 */
BOOL ConnectionClass::Send_Packet(void *data, uint32_t datalen, BOOL is_ack)
{
    // Prepare packet header.
    int32_t packet_num = 0;
    PacketHeader *buff_header = reinterpret_cast<PacketHeader *>(m_PacketBuffer);
    buff_header->Type = htole16(m_PacketType);

    if (is_ack) {
        buff_header->Command = COMMAND_ADATA;
        packet_num = m_SequenceNumAck;
    } else {
        buff_header->Command = COMMAND_DATA;
        packet_num = m_SequenceNum;
    }

    buff_header->Number = htole32(packet_num);

    // Copy packet payload after the header and queue to send.
    memcpy(&buff_header[1], data, datalen);

    if (m_CommBuffer->Queue_Send(m_PacketBuffer, datalen + 7, 0, 0)) {
        // Increment sequence numbers fo relevant packet type.
        if (is_ack) {
            ++m_SequenceNumAck;
        } else {
            ++m_SequenceNum;
        }

        return true;
    }

    return false;
}

/**
 * Put data into the recieved packet queue.
 *
 * 0x004A4E00
 */
BOOL ConnectionClass::Receive_Packet(void *data, int32_t datalen)
{
    uint32_t packet_num = 0;
    uint8_t *datap = static_cast<uint8_t *>(data);
    PacketHeader *header = static_cast<PacketHeader *>(data);
    PacketHeader ack_header;
    
    if (header->Type != htole16(m_PacketType)) {
        return false;
    }

    uint8_t packet_flag = header->Command;

    if (packet_flag != COMMAND_ACK) {
        if (packet_flag == COMMAND_DATA) {
            if (m_CommBuffer->Max_Receive() - m_CommBuffer->Num_Receive() > 1) {
                if (m_CommBuffer->Queue_Receive(datap, datalen, 0, 0)) {
                    ++m_PacketsReceived;
                    return true;
                }

                return false;
            }
        } else if (packet_flag == COMMAND_ADATA) {
            packet_num = le32toh(header->Number);
            BOOL packet_not_queued = false;

            if (packet_num > m_QueuedSeqenceNum || m_QueuedSeqenceNum == -1) {
                packet_not_queued = true;

                // Search commbuffer queue for packet with this number
                for (int i = 0; i < m_CommBuffer->Num_Receive(); ++i) {
                    RQEntry qentry = m_CommBuffer->Get_Receive(i);
                    
                    if (qentry) {
                        PacketHeader *qbuff = reinterpret_cast<PacketHeader *>(qentry->m_DataBuffer);
                        
                        if (qbuff->Command == COMMAND_ADATA) {
                            uint32_t queued_packet = le32toh(qbuff->Number);
                            
                            if (queued_packet == packet_num) {
                                packet_not_queued = false;
                                break;
                            }
                        }
                    }
                }
            }

            if (packet_not_queued) {
                if (m_CommBuffer->Max_Receive() - m_CommBuffer->Num_Receive() <= 1 && m_QueuedSeqenceNum + 1 != packet_num) {
                    return false;
                }

                if (!m_CommBuffer->Queue_Receive(datap, datalen, 0, 0)) {
                    return false;
                }

                ++m_QueuedPackets;

                if (m_QueuedSeqenceNum + 1 == packet_num) {
                    m_QueuedSeqenceNum = packet_num;
                    BOOL next_packet_found;

                    do {
                        next_packet_found = false;

                        for (int i = 0; i < m_CommBuffer->Num_Receive(); ++i) {
                            RQEntry qentry = m_CommBuffer->Get_Receive(i);

                            if (qentry) {
                                PacketHeader *qbuff = reinterpret_cast<PacketHeader *>(qentry->m_DataBuffer);

                                if (qbuff->Command == COMMAND_ADATA) {
                                    int32_t next_packet = m_QueuedSeqenceNum + 1;

                                    if (next_packet == le32toh(qbuff->Number)) {
                                        next_packet_found = true;
                                        m_QueuedSeqenceNum = next_packet;
                                        break;
                                    }
                                }
                            }
                        }
                    } while (next_packet_found);
                }
            }

            ack_header.Type = htole16(m_PacketType);
            ack_header.Command = COMMAND_ACK;
            ack_header.Number = htole32(packet_num);
            Send((uint8_t *)&ack_header, sizeof(header), 0, 0);

            return true;
        }

        return false;
    }

    for (int j = 0; j < m_CommBuffer->Num_Send(); ++j) {
        SQEntry qentry = m_CommBuffer->Get_Send(j);

        if (qentry) {
            PacketHeader *qbuff = reinterpret_cast<PacketHeader *>(qentry->m_DataBuffer);

            if (packet_num == le32toh(qbuff->Number) && qbuff->Command == COMMAND_ADATA) {
                qentry->m_Flags |= COMM_FLAG_SNDACK;
                return true;
            }
        }
    }

    return true;
}

/**
 * Retrieve data from recieved packets.
 *
 * 0x004A4FDC
 */
BOOL ConnectionClass::Get_Packet(void *data, int32_t *datalen)
{
    RQEntry qentry;

    for (int i = 0;; ++i) {
        if (i >= m_CommBuffer->Num_Receive()) {
            return false;
        }

        qentry = m_CommBuffer->Get_Receive(i);

        if (qentry!= nullptr) {
            if (!(qentry->m_Flags & COMM_FLAG_SNDACK)) {
                uint8_t *qbuff = qentry->m_DataBuffer;

                if (!*(qbuff + 2)) {
                    int next_packet = m_RecvSequenceNum + 1;

                    if (next_packet == *(uint32_t *)(qbuff + 3)) {
                        m_RecvSequenceNum = next_packet;
                        int qdata_len = qentry->m_DataLength - 7;
                        qentry->m_Flags |= COMM_FLAG_SNDACK;

                        if (qdata_len > 0) {
                            memcpy(data, qentry->m_DataBuffer + 7, qdata_len);
                        }

                        *datalen = qdata_len;

                        return true;
                    }
                }

                if (*(qbuff + 2) == 1) {
                    break;
                }
            }
        }
    }

    qentry->m_Flags |= 0x2;
    int buff_datalen = qentry->m_DataLength - 7;

    if (buff_datalen > 0) {
        memcpy(data, qentry->m_DataBuffer + 7, buff_datalen);
    }

    *datalen = buff_datalen;

    return true;
}

/**
 * Perform processing on the send and recieve queues.
 *
 * 0x004A50B0
 */
BOOL ConnectionClass::Service()
{
    return Service_Send_Queue() && Service_Receive_Queue();
}

/**
 * Perform processing on the send queue.
 *
 * 0x004A50E0
 */
BOOL ConnectionClass::Service_Send_Queue()
{
    BOOL failed = false;

    // Loop through and handle ack data first.
    for (int i = 0; i < m_CommBuffer->Num_Send(); ++i) {
        SQEntry entry = m_CommBuffer->Get_Send(i);

        if (entry->m_Flags & 2) {
            if (*(entry->m_DataBuffer + 2) == COMMAND_ADATA) {
                // Record delay between send and ack.
                m_CommBuffer->Add_Delay(Time() - entry->m_InitialSendTime);
            }

            m_CommBuffer->UnQueue_Send(0, 0, i--, 0, 0);
        }
    }

    // Now handle any data.
    for (int i = 0; i < m_CommBuffer->Num_Send(); ++i) {
        SQEntry entry = m_CommBuffer->Get_Send(i);

        if (!(entry->m_Flags & 2)) {
            uint32_t cur_time = Time();

            if ((cur_time - entry->m_LastSendTime) > m_SendRetryDelay) {
                Send(entry->m_DataBuffer, entry->m_DataLength, entry->m_AckToConnectionBuffer, entry->m_AckToConnectionLength);
                entry->m_LastSendTime = cur_time;

                if (!entry->m_AckToConnection) {
                    entry->m_InitialSendTime = cur_time;

                    if (*(entry->m_DataBuffer + 2) == COMMAND_DATA) {
                        entry->m_Flags |= 0x2;
                    }
                }

                ++entry->m_AckToConnection;

                if (m_SendMaxRetries != INT32_MAX && entry->m_AckToConnection > m_SendMaxRetries) {
                    failed = true;
                }

                if (m_SendRetryTimeout != INT32_MAX && (entry->m_LastSendTime - entry->m_InitialSendTime) > m_SendRetryTimeout) {
                    failed = true;
                }
            }
        }
    }

    return !failed;
}

/**
 * Perform processing on the recieve queue.
 *
 * 0x004A5244
 */
BOOL ConnectionClass::Service_Receive_Queue()
{
    for (int i = 0; i < m_CommBuffer->Num_Receive(); ++i) {
        RQEntry entry = m_CommBuffer->Get_Receive(i);

        if (entry->m_Flags & 2) {
            if (*(entry->m_DataBuffer + 2) == 1) {
                m_CommBuffer->UnQueue_Receive(0, 0, i--, 0, 0);
            } else if (*(uint32_t *)(entry->m_DataBuffer + 3) < m_QueuedSeqenceNum) {
                m_CommBuffer->UnQueue_Receive(0, 0, i--, 0, 0);
            }
        }
    }

    return true;
}

/**
 * Get a current tick count.
 *
 * 0x004A52B4
 */
uint32_t ConnectionClass::Time()
{
    if (PlatformTimerClass::s_timerSystemOn) {
        return TickCountTimer.Time();
    }

    DEBUG_ASSERT_PRINT(false, "Timer system is not active!");

    return 0;
}

/**
 * Get the command type as a string.
 *
 * 0x004A533C
 */
const char *ConnectionClass::Command_Name(CommandType command)
{
    if (command != COMMAND_NONE || command < COMMAND_COUNT) {
        return s_Commands[command];
    }

    return nullptr;
}
