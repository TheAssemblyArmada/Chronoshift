/**
 * @file
 *
 * @author tomsons26
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
#include <cstring>
#include "endiantype.h"

FieldClass::FieldClass(char *fieldname, char data)
{
    strncpy(m_name, fieldname, sizeof(m_name));
    m_type = FD_CHAR;
    m_size = 1;
    m_data = (char*)operator new[](m_size);
    memcpy(m_data, &data, m_size);
    m_unknown = 0;
}

FieldClass::FieldClass(char *fieldname, bool data)
{
    strncpy(m_name, fieldname, sizeof(m_name));
    m_type = FD_BOOLEAN;
    m_size = 1;
    m_data = (char*)operator new[](m_size);
    memcpy(m_data, &data, m_size);
    m_unknown = 0;
}

FieldClass::FieldClass(char *fieldname, short data)
{
    strncpy(m_name, fieldname, sizeof(m_name));
    m_type = FD_SHORT;
    m_size = 2;
    m_data = (char*)operator new[](m_size);
    memcpy(m_data, &data, m_size);
    m_unknown = 0;
}

FieldClass::FieldClass(char *fieldname, unsigned short data)
{
    strncpy(m_name, fieldname, sizeof(m_name));
    m_type = FD_USHORT;
    m_size = 2;
    m_data = (char*)operator new[](m_size);
    memcpy(m_data, &data, m_size);
    m_unknown = 0;
}

FieldClass::FieldClass(char *fieldname, long data)
{
    strncpy(m_name, fieldname, sizeof(m_name));
    m_type = FD_LONG;
    m_size = 4;
    m_data = (char*)operator new[](m_size);
    memcpy(m_data, &data, m_size);
    m_unknown = 0;
}

FieldClass::FieldClass(char *fieldname, unsigned long data)
{
    strncpy(m_name, fieldname, sizeof(m_name));
    m_type = FD_ULONG;
    m_size = 4;
    m_data = (char*)operator new[](m_size);
    memcpy(m_data, &data, m_size);
    m_unknown = 0;
}

FieldClass::FieldClass(char *fieldname, char *data)
{
    strncpy(m_name, fieldname, sizeof(m_name));
    m_type = FD_STRING;
    m_size = strlen(data) + 1;
    m_data = (char*)operator new[](m_size);
    memcpy(m_data, &data, m_size);
    m_unknown = 0;
}

FieldClass::FieldClass(char *fieldname, void *data, int size)
{
    strncpy(m_name, fieldname, sizeof(m_name));
    m_type = FD_CUSTOM;
    m_size = (short)size;
    m_data = (char*)operator new[](m_size);
    memcpy(m_data, data, m_size);
    m_unknown = 0;
}

void FieldClass::Host_To_Net()
{
    if (m_type < FD_LONG) {
        if (m_type >= FD_CHAR && m_type > FD_BOOLEAN) {
            *m_data = htobe16(*m_data);
        }
    } else if (m_type <= FD_ULONG) {
        *m_data = htobe32(*m_data);
    }
    m_type = htobe16(m_type);
    m_size = htobe16(m_size);
}
void FieldClass::Net_To_Host()
{
    m_size = be16toh(m_size);
    m_type = be16toh(m_type);
    if (m_type < FD_LONG) {
        if (m_type >= FD_CHAR && m_type > FD_BOOLEAN) {
            *m_data = be16toh(*m_data);
        }
    } else if (m_type <= FD_ULONG) {
        *m_data = be32toh(*m_data);
    }
}