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
#pragma once

#ifndef IPXADDRESS_H
#define IPXADDRESS_H

#include "always.h"

#pragma pack(push,1)
struct IPXHEADER
{
    char checksum[2];
    char length[2];
    char transport_control;
    char packet_type;
    char dst_net[4];
    char dst_node[6];
    char dst_sock[2];
    char src_net[4];
    char src_node[6];
    char src_sock[2];
};
#pragma pack(pop)

class IPXAddressClass
{
public:
    IPXAddressClass();
    IPXAddressClass(char *net, char *node);
    IPXAddressClass(IPXHEADER *header);

    void Set_Address(char *net, char *node);
    void Set_Address(IPXHEADER *header);
    void Get_Address(char *net, char *node);
    void Get_Address(IPXHEADER *header);
    BOOL Is_Broadcast();

    BOOL operator==(IPXAddressClass &that);
    BOOL operator!=(IPXAddressClass &that);
    BOOL operator>(IPXAddressClass &that);
    BOOL operator<(IPXAddressClass &that);
    BOOL operator>=(IPXAddressClass &that);
    BOOL operator<=(IPXAddressClass &that);

private:
    uint8_t NetNum[4];
    uint8_t NodeNum[6];
};

#endif // IPXADDRESS_H
