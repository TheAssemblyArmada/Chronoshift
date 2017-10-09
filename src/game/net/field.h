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
#pragma once

#include "always.h"

#ifndef FIELD_H
#define FIELD_H

enum FieldDataType
{
    FD_CHAR = 1,
    FD_BOOLEAN = 2,
    FD_SHORT = 3,
    FD_USHORT = 4,
    FD_LONG = 5,
    FD_ULONG = 6,
    FD_STRING = 7,
    FD_CUSTOM = 20
};

class FieldClass
{
public:
    FieldClass::FieldClass(char *fieldname, char data);
    FieldClass::FieldClass(char *fieldname, bool data);
    FieldClass::FieldClass(char *fieldname, short data);
    FieldClass::FieldClass(char *fieldname, unsigned short data);
    FieldClass::FieldClass(char *fieldname, long data);
    FieldClass::FieldClass(char *fieldname, unsigned long data);
    FieldClass::FieldClass(char *fieldname, char *data);
    FieldClass::FieldClass(char *fieldname, void *data, int size);
	~FieldClass(void) { delete[] m_data; m_data = nullptr; }
    void FieldClass::Host_To_Net();
    void FieldClass::Net_To_Host();

protected:
    char m_name[4];
    short m_type;
    short m_size;
    char *m_data;
    int m_unknown; //whatever it is its used in PacketClass::Find_Field
};
#endif // FIELD_H
