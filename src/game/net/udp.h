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

#ifndef UDP_H
#define UDP_H

#include "always.h"
#include "vector.h"
#include "wsock.h"

extern const uint16_t g_ChronoshiftPortNumber;

class UDPInterfaceClass : public WinsockInterfaceClass
{
public:
    UDPInterfaceClass() {}
    virtual ~UDPInterfaceClass() {}

    virtual void Broadcast(void *src, int src_len) override;
    virtual void Set_Broadcast_Address(void *address) override;
    virtual ProtocolEnum Get_Protocol() override { return PROTOCOL_UDP; }
    // This one will need replacing with something for SDL messages
    virtual int Protocol_Event_Message() override { return UDP_MESSAGE; }
    virtual int Open_Socket(unsigned port) override;
    virtual int32_t Message_Handler(void *hwnd, uint32_t msg, uint32_t wparam, int32_t lparam) override;

private:
    DynamicVectorClass<uint8_t *> m_BroadcastAddresses;
    DynamicVectorClass<char *> m_LocalAddresses;
};

#endif // _UDP_H_
