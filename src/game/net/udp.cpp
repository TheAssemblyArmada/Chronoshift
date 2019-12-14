/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Networkng interface class utilising the UDP protocol.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "udp.h"
#include "endiantype.h"
#include "gamedebug.h"
#include "globals.h"
#include "keyboard.h"
#include <cstdio>

using std::sprintf;
using std::sscanf;

const uint16_t g_ChronoshiftPortNumber = 1234;

/**
 * Writes data to the sockets interface for broadcast.
 *
 * 0x005A8844
 */
void UDPInterfaceClass::Broadcast(void *src, int src_len)
{
    // For each address to broadcast to, create a buffer holding the data to send and queue it.
    for (int i = 0; i < m_BroadcastAddresses.Count(); ++i) {
        WinsockBufferType *wsockbuffer = new WinsockBufferType;
        memcpy(wsockbuffer->m_Data, src, src_len);
        wsockbuffer->m_Length = src_len;
        wsockbuffer->m_Broadcast = 1;
        memset(wsockbuffer->m_Header, 0, 64);
        memcpy(&wsockbuffer->m_Header[4], m_BroadcastAddresses[i], 4);
        m_OutBuffers.Add(wsockbuffer);

#if defined(PLATFORM_WINDOWS)
        SendMessageA(g_MainWindow, Protocol_Event_Message(), 0, FD_WRITE);
#else
        // TODO Need SDL vesion
#endif // PLATFORM_WINDOWS

        g_Keyboard->Check();
    }
}

/**
 * Adds an address to broadcast to.
 *
 * 0x005A8650
 */
void UDPInterfaceClass::Set_Broadcast_Address(void *address)
{
    int fst, snd, trd, fth;
    uint8_t *addr_num = new uint8_t[4];

    // Treat address as a string of integers separated by '.'.
    sscanf(static_cast<char *>(address), "%d.%d.%d.%d", &fst, &snd, &trd, &fth);

    addr_num[0] = fst;
    addr_num[1] = snd;
    addr_num[2] = trd;
    addr_num[3] = fth;

    m_BroadcastAddresses.Add(addr_num);
}

/**
 * Opens a socket on a given port.
 *
 * 0x005A8698
 */
int UDPInterfaceClass::Open_Socket(unsigned port)
{
    struct sockaddr_in addr;
    char addrname[128];
    struct hostent *host;
    uint32_t optval;

    if (!m_IsOpen && !Init()) {
        return 0;
    }

    m_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);

    if (m_Socket == INVALID_SOCKET) {
        DEBUG_LOG("Failed to create UDP socket - error code %d.\n", LastSocketError);

        return 0;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htobe16(g_ChronoshiftPortNumber);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_Socket, (sockaddr const *)&addr, sizeof(addr)) == SOCKET_ERROR) {
        DEBUG_LOG("UDP socket bind failed with error code %d.\n", LastSocketError);
        Close_Socket();

        return 0;
    }

    gethostname(addrname, 128);
    DEBUG_LOG("Hostname is %s\n", addrname);
    host = gethostbyname(addrname);

    // Clear the existing list of local addresses
    while (m_LocalAddresses.Count()) {
        delete m_LocalAddresses[0];
        m_LocalAddresses.Delete(0);
    }

    char **addr_list = host->h_addr_list;

    while (*addr_list != nullptr) {
        uint32_t addr_int = *(uint32_t *)(*addr_list);
        DEBUG_LOG("UDP OpenSocket found local address: %u.%u.%u.%u\n",
            (uint8_t)(*addr_list)[0],
            (uint8_t)(*addr_list)[1],
            (uint8_t)(*addr_list)[2],
            (uint8_t)(*addr_list)[3]);
        uint32_t *new_addr = new uint32_t;
        *new_addr = addr_int;
        m_LocalAddresses.Add((char *)new_addr);
        ++addr_list;
    }

    optval = 0;
    setsockopt(m_Socket, SOL_SOCKET, SO_LINGER, (char *)(&optval), sizeof(optval));
    Set_Socket_Options();

    return 1;
}

/**
 * Handles network messages.
 *
 * 0x005A8940
 */
int32_t UDPInterfaceClass::Message_Handler(void *hwnd, uint32_t msg, uint32_t wparam, int32_t lparam)
{
    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);
    int recv_len;
    WinsockBufferType *buff = nullptr;

    if (msg != UDP_MESSAGE) {
        return 1;
    }

    if ((lparam & 0xFFFF0000) >> 16) {
        Clear_Socket_Error(m_Socket);

        return 0;
    }

    switch (lparam & 0xFFFF) {
        case SOCKET_READ:
            if ((recv_len = recvfrom(m_Socket, m_RecvBuffer, sizeof(m_RecvBuffer), 0, (struct sockaddr *)&from, &fromlen))
                == SOCKET_ERROR) {
                break;
            }

            if (!recv_len) {
                return 0;
            }

            // If the data wasn't intended for us, discard it.
            for (int i = 0; i < m_LocalAddresses.Count(); ++i) {
                if (!memcmp(m_LocalAddresses[i], &from.sin_addr, sizeof(from.sin_addr))) {
                    return 0;
                }
            }

            // Queue the data for processing in our in buffer queue.
            buff = new WinsockBufferType;
            buff->m_Length = recv_len;
            memcpy(buff->m_Data, m_RecvBuffer, recv_len);
            memset(buff->m_Header, 0, 64);
            memcpy(&buff->m_Header[4], &from.sin_addr, sizeof(from.sin_addr));
            m_InBuffers.Add(buff);

            return 0;
        case SOCKET_WRITE:
            if (m_OutBuffers.Count()) {
                buff = m_OutBuffers[0];
                from.sin_family = AF_INET;
                from.sin_port = htobe16(g_ChronoshiftPortNumber);
                memcpy(&from.sin_addr, &buff->m_Header[4], sizeof(from.sin_addr));

                if (sendto(m_Socket, buff->m_Data, buff->m_Length, 0, (sockaddr const *)&from, sizeof(from)) != SOCKET_ERROR
                    || LastSocketError == SOCKEWOULDBLOCK) {
                    m_OutBuffers.Delete(0);
                    delete buff;
                } else {
                    Clear_Socket_Error(m_Socket);
                }
            }

            return 0;
        default:
            break;
    }

    return 0;
}
