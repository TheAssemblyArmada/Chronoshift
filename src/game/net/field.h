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
public:
    FieldClass(char *fieldname, uint8_t data);
    FieldClass(char *fieldname, bool data);
    FieldClass(char *fieldname, int16_t data);
    FieldClass(char *fieldname, uint16_t data);
    FieldClass(char *fieldname, int32_t data);
    FieldClass(char *fieldname, uint32_t data);
    FieldClass(char *fieldname, char *data);
    FieldClass(char *fieldname, void *data, int size);
    ~FieldClass()
    {
        delete[] m_data;
        m_data = nullptr;
    }
    void Host_To_Net();
    void Net_To_Host();

protected:
    char m_name[4];
    int16_t m_type;
    int16_t m_size;
    char *m_data;
    int32_t m_unknown; // whatever it is its used in PacketClass::Find_Field
};
#endif // FIELD_H
