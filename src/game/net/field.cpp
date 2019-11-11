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
#include "field.h"
#include "endiantype.h"
#include <cstring>

FieldClass::FieldClass(char *id, uint8_t data)
{
    strncpy(m_ID, id, sizeof(m_ID));
    m_DataType = FD_UINT8;
    m_Size = sizeof(data);
    m_Data = new char[m_Size];
    memcpy(m_Data, &data, m_Size);
    m_Next = nullptr;
}

FieldClass::FieldClass(char *id, bool data)
{
    strncpy(m_ID, id, sizeof(m_ID));
    m_DataType = FD_BOOLEAN;
    m_Size = sizeof(data);
    m_Data = new char[m_Size];
    memcpy(m_Data, &data, m_Size);
    m_Next = nullptr;
}

FieldClass::FieldClass(char *id, int16_t data)
{
    strncpy(m_ID, id, sizeof(m_ID));
    m_DataType = FD_INT16;
    m_Size = sizeof(data);
    m_Data = new char[m_Size];
    memcpy(m_Data, &data, m_Size);
    m_Next = nullptr;
}

FieldClass::FieldClass(char *id, uint16_t data)
{
    strncpy(m_ID, id, sizeof(m_ID));
    m_DataType = FD_UINT16;
    m_Size = sizeof(data);
    m_Data = new char[m_Size];
    memcpy(m_Data, &data, m_Size);
    m_Next = nullptr;
}

FieldClass::FieldClass(char *id, int32_t data)
{
    strncpy(m_ID, id, sizeof(m_ID));
    m_DataType = FD_INT32;
    m_Size = sizeof(data);
    m_Data = new char[m_Size];
    memcpy(m_Data, &data, m_Size);
    m_Next = nullptr;
}

FieldClass::FieldClass(char *id, uint32_t data)
{
    strncpy(m_ID, id, sizeof(m_ID));
    m_DataType = FD_UINT32;
    m_Size = sizeof(data);
    m_Data = new char[m_Size];
    memcpy(m_Data, &data, m_Size);
    m_Next = nullptr;
}

FieldClass::FieldClass(char *id, char *data)
{
    strncpy(m_ID, id, sizeof(m_ID));
    m_DataType = FD_STRING;
    m_Size = (int16_t)strlen(data) + 1;
    m_Data = new char[m_Size];
    memcpy(m_Data, &data, m_Size);
    m_Next = nullptr;
}

FieldClass::FieldClass(char *id, void *data, int size)
{
    strncpy(m_ID, id, sizeof(m_ID));
    m_DataType = FD_CUSTOM;
    m_Size = size;
    m_Data = new char[m_Size];
    memcpy(m_Data, data, m_Size);
    m_Next = nullptr;
}

void FieldClass::Host_To_Net()
{
    switch (m_DataType) {
        case FD_INT16:
        case FD_UINT16:
            *reinterpret_cast<uint16_t *>(m_Data) = htobe16(*reinterpret_cast<uint16_t *>(m_Data));
            break;
        case FD_INT32:
        case FD_UINT32:
            *reinterpret_cast<uint32_t *>(m_Data) = htobe32(*reinterpret_cast<uint32_t *>(m_Data));
            break;
        default:
            break;
    }

    m_DataType = htobe16(m_DataType);
    m_Size = htobe16(m_Size);
}

void FieldClass::Net_To_Host()
{
    m_Size = be16toh(m_Size);
    m_DataType = be16toh(m_DataType);

    switch (m_DataType) {
        case FD_INT16:
        case FD_UINT16:
            *reinterpret_cast<uint16_t *>(m_Data) = be16toh(*reinterpret_cast<uint16_t *>(m_Data));
            break;
        case FD_INT32:
        case FD_UINT32:
            *reinterpret_cast<uint32_t *>(m_Data) = be32toh(*reinterpret_cast<uint32_t *>(m_Data));
            break;
        default:
            break;
    }
}
