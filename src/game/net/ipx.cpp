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
#include "ipx.h"
#include "endiantype.h"
#include <captainslog.h>
#include <cstring>

using std::memcpy;
using std::memset;

#pragma pack(push, 1)
struct IPXAddressData
{
    int32_t m_AdapterNum;
    uint8_t m_NetNum[4];
    uint8_t m_NodeNum[6];
    bool m_Wan;
    bool m_Status; // Up or not.
    int32_t m_MaxPkt; // Max packet size not including the header.
    int32_t m_LinkSpeed; // Speed in 100B/s increments.
};
#pragma pack(pop)

IPXInterfaceClass::IPXInterfaceClass()
{
    memset(m_NetworkNumber, 0xFF, sizeof(m_NetworkNumber));
    memset(m_OutBoundNodeNum, 0xFF, sizeof(m_InBoundNodeNum));
    memset(m_InBoundNodeNum, 0xFF, sizeof(m_InBoundNodeNum));
}

/**
 * Opens a socket on a given port.
 *
 * 0x005A8D64
 */
int IPXInterfaceClass::Open_Socket(unsigned port)
{
#ifdef PLATFORM_WINDOWS
    sockaddr_ipx addr;
    uint16_t s_port;

    if (!m_IsOpen && !Init()) {
        return 0;
    }

    s_port = m_Port = port;
    addr.sa_family = AF_IPX;
    memset(addr.sa_netnum, 0, sizeof(addr.sa_netnum));
    memset(addr.sa_nodenum, 0xFFFFFFFF, sizeof(addr.sa_nodenum));
    addr.sa_socket = htobe16(s_port);
    m_Socket = socket(AF_IPX, SOCK_DGRAM, NSPROTO_IPX);

    if (m_Socket == INVALID_SOCKET) {
        captainslog_debug("Failed to create IPX socket - error code %d.", LastSocketError);
        closesocket(m_Socket);

        return 0;
    }

    if (!Get_Network_Card_Address(0, &addr)) {
        closesocket(m_Socket);

        return 0;
    }

    if (bind(m_Socket, (const sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
        captainslog_debug("IPX socket bind failed with error code %d.", LastSocketError);
        closesocket(m_Socket);

        return 0;
    }

    uint32_t broadcast = 1;
    uint32_t type = 4;

    if (setsockopt(m_Socket, SOL_SOCKET, SO_BROADCAST, (const char *)&broadcast, sizeof(broadcast)) == SOCKET_ERROR) {
        captainslog_debug("Failed to set IPX socket option SO_BROADCAST - error code %d.", LastSocketError);
    }

    // These options don't appear to exist on linux.
    if (setsockopt(m_Socket, NSPROTO_IPX, IPX_PTYPE, (const char *)&type, sizeof(type)) == SOCKET_ERROR) {
        captainslog_debug("Failed to set IPX socket option IPX_PTYPE - error code %d.", LastSocketError);
    }

    if (setsockopt(m_Socket, NSPROTO_IPX, IPX_FILTERPTYPE, (const char *)&type, sizeof(type)) == SOCKET_ERROR) {
        captainslog_debug("Failed to set IPX socket option IPX_FILTERPTYPE - error code %d.", LastSocketError);
    }

    Set_Socket_Options();

    return 1;
#else 
    return 0;
#endif
}

/**
 * Handles network messages.
 *
 * 0x005A8F88
 */
int IPXInterfaceClass::Message_Handler(void *hwnd, uint32_t msg, uint32_t wparam, int32_t lparam)
{
#ifdef PLATFORM_WINDOWS
    struct sockaddr_ipx from;
    socklen_t fromlen = sizeof(from);
    int recv_len;
    WinsockBufferType *buff = nullptr;
    char node_num[6];
    char net_num[4];

    if (msg != IPX_MESSAGE) {
        return 1;
    }

    switch (lparam & 0xFFFF) {
        case SOCKET_READ:
            if ((lparam & 0xFFFF0000) >> 16) {
                Clear_Socket_Error(m_Socket);

                return 0;
            }

            if ((recv_len = recvfrom(m_Socket, m_RecvBuffer, sizeof(m_RecvBuffer), 0, (struct sockaddr *)&from, &fromlen))
                == SOCKET_ERROR) {
                if (LastSocketError == SOCKEWOULDBLOCK) {
                    Clear_Socket_Error(m_Socket);
                }

                break;
            }

            if (recv_len == 0) {
                return 0;
            }

            memcpy(net_num, from.sa_netnum, sizeof(net_num));
            memcpy(node_num, from.sa_nodenum, sizeof(node_num));

            if (memcmp(net_num, m_NetworkNumber, sizeof(net_num)) == 0
                || memcmp(node_num, m_InBoundNodeNum, sizeof(net_num)) == 0) {
                return 0;
            }

            // Queue the data for processing in our in buffer queue.
            buff = new WinsockBufferType;
            buff->m_Length = recv_len;
            memcpy(buff->m_Data, m_RecvBuffer, recv_len);
            reinterpret_cast<IPXAddressClass *>(buff->m_Header)->Set_Address(net_num, node_num);
            m_InBuffers.Add(buff);

            return 0;
        case SOCKET_WRITE:
            if ((lparam & 0xFFFF0000) >> 16) {
                Clear_Socket_Error(m_Socket);

                return 0;
            }

            while (m_OutBuffers.Count()) {
                buff = m_OutBuffers[0];
                from.sa_family = AF_IPX;
                from.sa_socket = htobe16(m_Port);

                if (buff->m_Broadcast) {
                    memcpy(from.sa_netnum, m_NetworkNumber, sizeof(m_NetworkNumber));
                    memcpy(from.sa_nodenum, m_OutBoundNodeNum, sizeof(m_OutBoundNodeNum));
                } else {
                    reinterpret_cast<IPXAddressClass *>(buff->m_Header)->Get_Address(net_num, node_num);
                    memcpy(from.sa_netnum, net_num, sizeof(net_num));
                    memcpy(from.sa_nodenum, node_num, sizeof(node_num));
                }

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
#endif
    return 0;
}

/**
 * Internal function to get adapter address.
 *
 * 0x005A8C3C
 */
#ifdef PLATFORM_WINDOWS
BOOL IPXInterfaceClass::Get_Network_Card_Address(int32_t option, sockaddr_ipx *addr)
{
    // This interface detection mechanism doesn't appear to exist on linux.
    SOCKET s = socket(AF_IPX, SOCK_DGRAM, NSPROTO_IPX);
    sockaddr_ipx tmp_addr;

    if (s == INVALID_SOCKET) {
        return false;
    }

    memset(&tmp_addr, 0, sizeof(tmp_addr));
    tmp_addr.sa_family = AF_IPX;

    if (bind(s, (const sockaddr *)&tmp_addr, sizeof(tmp_addr)) == SOCKET_ERROR) {
        closesocket(s);

        return false;
    }

    IPXAddressData opt_val;
    memset(&opt_val, 0, sizeof(opt_val));
    opt_val.m_AdapterNum = option;
    socklen_t opt_len = sizeof(opt_val);

    if (getsockopt(s, NSPROTO_IPX, IPX_ADDRESS, (char *)&opt_val, &opt_len) == SOCKET_ERROR) {
        closesocket(s);

        return false;
    }

    memcpy(addr->sa_netnum, opt_val.m_NetNum, sizeof(addr->sa_netnum));
    memcpy(m_NetworkNumber, opt_val.m_NetNum, sizeof(m_NetworkNumber));
    memcpy(addr->sa_nodenum, opt_val.m_NodeNum, sizeof(addr->sa_nodenum));
    closesocket(s);

    return true;
}
#endif
