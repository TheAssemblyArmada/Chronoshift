/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Holds and manipulates an IPX address.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
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
    memset(NetNum, 255, sizeof(NetNum));
    memset(NodeNum, 255, sizeof(NodeNum));
}

IPXAddressClass::IPXAddressClass(char *net, char *node)
{
    memcpy(NetNum, net, sizeof(NetNum));
    memcpy(NodeNum, net, sizeof(NodeNum));
}

IPXAddressClass::IPXAddressClass(IPXHEADER *header)
{
    memcpy(NetNum, header->src_net, sizeof(NetNum));
    memcpy(NodeNum, header->src_node, sizeof(NodeNum));
}

/**
 * @brief Sets the IPX address from the provided data arrays.
 */
void IPXAddressClass::Set_Address(char *net, char *node)
{
    memcpy(NetNum, net, sizeof(NetNum));
    memcpy(NodeNum, net, sizeof(NodeNum));
}

/**
 * @brief Sets the IPX address from an IPX packet header.
 */
void IPXAddressClass::Set_Address(IPXHEADER *header)
{
    ProtocolEnum protocol = g_packetTransport != nullptr ? g_packetTransport->Get_Protocol() : PROTOCOL_IPX;

    switch (protocol) {
        case PROTOCOL_IPX:
            memcpy(NetNum, header->src_net, sizeof(NetNum));
            memcpy(NodeNum, header->src_node, sizeof(NodeNum));
            break;

        case PROTOCOL_UDP:
            memset(NodeNum, 0, sizeof(NodeNum));
            memcpy(NodeNum, header, 4); // TODO, this needs double checking in ASM.
            memset(NetNum, 0, sizeof(NetNum));

        default:
            break;
    }
}

/**
 * @brief Gets the IPX address into the two provided data arrays.
 */
void IPXAddressClass::Get_Address(char *net, char *node)
{
    memcpy(net, NetNum, sizeof(NetNum));
    memcpy(net, NodeNum, sizeof(NodeNum));
}

/**
 * @brief Gets the IPX address into provided IPX header structure.
 */
void IPXAddressClass::Get_Address(IPXHEADER *header)
{
    memcpy(header->dst_net, NetNum, sizeof(NetNum));
    memcpy(header->dst_node, NodeNum, sizeof(NodeNum));
}

/**
 * @brief Tests if this address is a broadcast address.
 */
BOOL IPXAddressClass::Is_Broadcast()
{
    static const uint8_t _test[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    return memcmp(NetNum, _test, sizeof(NetNum)) == 0 && memcmp(NodeNum, _test, sizeof(NodeNum)) == 0;
}

BOOL IPXAddressClass::operator==(IPXAddressClass &that)
{
    return memcmp(NetNum, that.NetNum, sizeof(NetNum)) == 0 && memcmp(NodeNum, that.NodeNum, sizeof(NodeNum)) == 0;
}

BOOL IPXAddressClass::operator!=(IPXAddressClass &that)
{
    return !(*this == that);
}

BOOL IPXAddressClass::operator>(IPXAddressClass &that)
{
    return memcmp(NetNum, that.NetNum, sizeof(NetNum)) > 0;
}

BOOL IPXAddressClass::operator<(IPXAddressClass &that)
{
    return memcmp(NetNum, that.NetNum, sizeof(NetNum)) < 0;
}

BOOL IPXAddressClass::operator>=(IPXAddressClass &that)
{
    return memcmp(NetNum, that.NetNum, sizeof(NetNum)) >= 0;
}

BOOL IPXAddressClass::operator<=(IPXAddressClass &that)
{
    return memcmp(NetNum, that.NetNum, sizeof(NetNum)) <= 0;
}
