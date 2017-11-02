/**
 * @file
 *
 * @author tomsons26
 * @author OmniBlade
 *
 * @brief Class for packing network fields used for packets and the statistics function.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "field.h"
#include "endiantype.h"
#include <cstring>

FieldClass::FieldClass(char *fieldname, uint8_t data)
{
    strncpy(m_name, fieldname, sizeof(m_name));
    m_type = FD_UINT8;
    m_size = sizeof(data);
    m_data = new char[m_size];
    memcpy(m_data, &data, m_size);
    m_unknown = 0;
}

FieldClass::FieldClass(char *fieldname, bool data)
{
    strncpy(m_name, fieldname, sizeof(m_name));
    m_type = FD_BOOLEAN;
    m_size = sizeof(data);
    m_data = new char[m_size];
    memcpy(m_data, &data, m_size);
    m_unknown = 0;
}

FieldClass::FieldClass(char *fieldname, int16_t data)
{
    strncpy(m_name, fieldname, sizeof(m_name));
    m_type = FD_INT16;
    m_size = sizeof(data);
    m_data = new char[m_size];
    memcpy(m_data, &data, m_size);
    m_unknown = 0;
}

FieldClass::FieldClass(char *fieldname, uint16_t data)
{
    strncpy(m_name, fieldname, sizeof(m_name));
    m_type = FD_UINT16;
    m_size = sizeof(data);
    m_data = new char[m_size];
    memcpy(m_data, &data, m_size);
    m_unknown = 0;
}

FieldClass::FieldClass(char *fieldname, int32_t data)
{
    strncpy(m_name, fieldname, sizeof(m_name));
    m_type = FD_INT32;
    m_size = sizeof(data);
    m_data = new char[m_size];
    memcpy(m_data, &data, m_size);
    m_unknown = 0;
}

FieldClass::FieldClass(char *fieldname, uint32_t data)
{
    strncpy(m_name, fieldname, sizeof(m_name));
    m_type = FD_UINT32;
    m_size = sizeof(data);
    m_data = new char[m_size];
    memcpy(m_data, &data, m_size);
    m_unknown = 0;
}

FieldClass::FieldClass(char *fieldname, char *data)
{
    strncpy(m_name, fieldname, sizeof(m_name));
    m_type = FD_STRING;
    m_size = (int16_t)strlen(data) + 1;
    m_data = new char[m_size];
    memcpy(m_data, &data, m_size);
    m_unknown = 0;
}

FieldClass::FieldClass(char *fieldname, void *data, int size)
{
    strncpy(m_name, fieldname, sizeof(m_name));
    m_type = FD_CUSTOM;
    m_size = size;
    m_data = new char[m_size];
    memcpy(m_data, data, m_size);
    m_unknown = 0;
}

void FieldClass::Host_To_Net()
{
    switch (m_type) {
        case FD_INT16:
        case FD_UINT16:
            *reinterpret_cast<uint16_t *>(m_data) = htobe16(*reinterpret_cast<uint16_t *>(m_data));
            break;
        case FD_INT32:
        case FD_UINT32:
            *reinterpret_cast<uint32_t *>(m_data) = htobe32(*reinterpret_cast<uint32_t *>(m_data));
            break;
        default:
            break;
    }

    m_type = htobe16(m_type);
    m_size = htobe16(m_size);
}

void FieldClass::Net_To_Host()
{
    m_size = be16toh(m_size);
    m_type = be16toh(m_type);

    switch (m_type) {
        case FD_INT16:
        case FD_UINT16:
            *reinterpret_cast<uint16_t *>(m_data) = be16toh(*reinterpret_cast<uint16_t *>(m_data));
            break;
        case FD_INT32:
        case FD_UINT32:
            *reinterpret_cast<uint32_t *>(m_data) = be32toh(*reinterpret_cast<uint32_t *>(m_data));
            break;
        default:
            break;
    }
}
