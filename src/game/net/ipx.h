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
#pragma once

#ifndef IPX_H
#define IPX_H

#include "always.h"
#include "ipxaddress.h"
#include "wsock.h"

#ifdef PLATFORM_WINDOWS
#include <wsipx.h>
#ifndef COMPILER_WATCOM
#include <wsnwlink.h>
#else
#define IPX_PTYPE 0x4000
#define IPX_FILTERPTYPE 0x4001
#define IPX_ADDRESS 0x4007
#endif
#else

#endif

struct IPXHeader
{
    uint16_t m_CheckSum;
    uint16_t m_Length;
    uint8_t m_TransportControl;
    uint8_t m_PacketType;
    IPXAddressClass m_Destination;
    IPXAddressClass m_Source;
};

class IPXInterfaceClass : public WinsockInterfaceClass
{
public:
    IPXInterfaceClass();
    virtual ~IPXInterfaceClass() {}

    virtual ProtocolEnum Get_Protocol() override { return PROTOCOL_IPX; }
    virtual int Protocol_Event_Message() override { return IPX_MESSAGE; }
    virtual int Open_Socket(unsigned port) override;
    virtual int Message_Handler(void *hwnd, uint32_t msg, uint32_t wparam, int32_t lparam) override;

private:
#ifdef PLATFORM_WINDOWS
    BOOL Get_Network_Card_Address(int32_t option, sockaddr_ipx *addr);
#endif

private:
    uint8_t m_NetworkNumber[4];
    uint8_t m_OutBoundNodeNum[6];
    uint8_t m_InBoundNodeNum[6];
    uint32_t m_Port;
};

#endif // IPX_H
