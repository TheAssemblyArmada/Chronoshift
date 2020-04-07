/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Class for building data packets such as those used for sending match statistics.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#include "always.h"

#ifndef PACKET_H
#define PACKET_H

class FieldClass;

class PacketClass
{
public:
    PacketClass() : m_Size(0), m_ID(0), m_Head(nullptr), m_Current(nullptr) {}
    PacketClass(const uint8_t *data);
    ~PacketClass();

    void Add_Field(FieldClass *field);
    uint8_t *Create_Comm_Packet(unsigned &size);
    FieldClass *Find_Field(const char *id);
    BOOL Get_Field(const char *id, int8_t &data);
    BOOL Get_Field(const char *id, uint8_t &data);
    BOOL Get_Field(const char *id, int16_t &data);
    BOOL Get_Field(const char *id, uint16_t &data);
    BOOL Get_Field(const char *id, int32_t &data);
    BOOL Get_Field(const char *id, uint32_t &data);
    BOOL Get_Field(const char *id, char *data);
    BOOL Get_Field(const char *id, void *data, unsigned &size);

private:
    int16_t m_Size;
    int16_t m_ID;
    FieldClass *m_Head;
    FieldClass *m_Current;
};

#endif // PACKET_H
