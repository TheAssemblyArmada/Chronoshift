/**
 * @file
 *
 * @author tomsons26
 * @author OmniBlade
 *
 * @brief Class for packing network fields used for packets and the statistics function.
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

#ifndef FIELD_H
#define FIELD_H

enum FieldDataType
{
    FD_UINT8 = 1,
    FD_BOOLEAN = 2,
    FD_INT16 = 3,
    FD_UINT16 = 4,
    FD_INT32 = 5,
    FD_UINT32 = 6,
    FD_STRING = 7,
    FD_CUSTOM = 20
};

class FieldClass
{
    friend class PacketClass;

public:
    FieldClass() : m_DataType(0), m_Size(0), m_Data(nullptr), m_Next(nullptr) {}
    FieldClass(char *id, uint8_t data);
    FieldClass(char *id, bool data);
    FieldClass(char *id, int16_t data);
    FieldClass(char *id, uint16_t data);
    FieldClass(char *id, int32_t data);
    FieldClass(char *id, uint32_t data);
    FieldClass(char *id, char *data);
    FieldClass(char *id, void *data, int size);
    ~FieldClass()
    {
        delete[] m_Data;
        m_Data = nullptr;
    }
    void Host_To_Net();
    void Net_To_Host();

protected:
    char m_ID[4];
    int16_t m_DataType;
    int16_t m_Size;
    uint8_t *m_Data;
    FieldClass *m_Next;
};
#endif // FIELD_H
