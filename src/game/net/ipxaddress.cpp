/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Holds and manipulates an IPX address.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "ipxaddress.h"
#include "wsock.h"

IPXAddressClass::IPXAddressClass()
{
    memset(m_NetNum, 255, sizeof(m_NetNum));
    memset(m_NodeNum, 255, sizeof(m_NodeNum));
}

IPXAddressClass::IPXAddressClass(char *net, char *node)
{
    memcpy(m_NetNum, net, sizeof(m_NetNum));
    memcpy(m_NodeNum, net, sizeof(m_NodeNum));
}

IPXAddressClass::IPXAddressClass(IPXHEADER *header)
{
    memcpy(m_NetNum, header->src_net, sizeof(m_NetNum));
    memcpy(m_NodeNum, header->src_node, sizeof(m_NodeNum));
}

/**
 * @brief Sets the IPX address from the provided data arrays.
 */
void IPXAddressClass::Set_Address(char *net, char *node)
{
    memcpy(m_NetNum, net, sizeof(m_NetNum));
    memcpy(m_NodeNum, net, sizeof(m_NodeNum));
}

/**
 * @brief Sets the IPX address from an IPX packet header.
 */
void IPXAddressClass::Set_Address(IPXHEADER *header)
{
    ProtocolEnum protocol = g_packetTransport != nullptr ? g_packetTransport->Get_Protocol() : PROTOCOL_IPX;

    switch (protocol) {
        case PROTOCOL_IPX:
            memcpy(m_NetNum, header->src_net, sizeof(m_NetNum));
            memcpy(m_NodeNum, header->src_node, sizeof(m_NodeNum));
            break;

        case PROTOCOL_UDP:
            memset(m_NodeNum, 0, sizeof(m_NodeNum));
            memcpy(m_NodeNum, header, 4); // TODO, this needs double checking in ASM.
            memset(m_NetNum, 0, sizeof(m_NetNum));

        default:
            break;
    }
}

/**
 * @brief Gets the IPX address into the two provided data arrays.
 */
void IPXAddressClass::Get_Address(char *net, char *node)
{
    memcpy(net, m_NetNum, sizeof(m_NetNum));
    memcpy(net, m_NodeNum, sizeof(m_NodeNum));
}

/**
 * @brief Gets the IPX address into provided IPX header structure.
 */
void IPXAddressClass::Get_Address(IPXHEADER *header)
{
    memcpy(header->dst_net, m_NetNum, sizeof(m_NetNum));
    memcpy(header->dst_node, m_NodeNum, sizeof(m_NodeNum));
}

/**
 * @brief Tests if this address is a broadcast address.
 */
BOOL IPXAddressClass::Is_Broadcast()
{
    static const uint8_t _test[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    return memcmp(m_NetNum, _test, sizeof(m_NetNum)) == 0 && memcmp(m_NodeNum, _test, sizeof(m_NodeNum)) == 0;
}

BOOL IPXAddressClass::operator==(IPXAddressClass &that)
{
    return memcmp(m_NetNum, that.m_NetNum, sizeof(m_NetNum)) == 0 && memcmp(m_NodeNum, that.m_NodeNum, sizeof(m_NodeNum)) == 0;
}

BOOL IPXAddressClass::operator!=(IPXAddressClass &that)
{
    return !(*this == that);
}

BOOL IPXAddressClass::operator>(IPXAddressClass &that)
{
    return memcmp(this, &that, sizeof(*this)) > 0;
}

BOOL IPXAddressClass::operator<(IPXAddressClass &that)
{
    return memcmp(this, &that, sizeof(*this)) < 0;
}

BOOL IPXAddressClass::operator>=(IPXAddressClass &that)
{
    return memcmp(this, &that, sizeof(*this)) >= 0;
}

BOOL IPXAddressClass::operator<=(IPXAddressClass &that)
{
    return memcmp(this, &that, sizeof(*this)) <= 0;
}
