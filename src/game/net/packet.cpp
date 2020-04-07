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
#include "packet.h"
#include "endiantype.h"
#include "field.h"
#include <algorithm>
#include <cstring>

using std::strncmp;

/**
 * This constructor reconstitutes the class from raw packet data.
 */
PacketClass::PacketClass(const uint8_t *data) : m_Size(0), m_ID(0), m_Head(nullptr), m_Current(nullptr)
{
    const uint8_t *getp = data;
    memcpy(&m_Size, getp, sizeof(m_Size));
    getp += sizeof(m_Size);
    m_Size = be16toh(m_Size);
    memcpy(&m_ID, getp, sizeof(m_ID));
    getp += sizeof(m_ID);
    m_ID = be16toh(m_ID);

    int remaining = m_Size - (sizeof(m_Size) + sizeof(m_ID));

    while (remaining > 0) {
        FieldClass *field = new FieldClass;
        memcpy(field->m_ID, getp, sizeof(field->m_ID));
        getp += sizeof(field->m_ID);
        remaining -= sizeof(field->m_ID);
        memcpy(&field->m_DataType, getp, sizeof(field->m_DataType));
        getp += sizeof(field->m_DataType);
        remaining -= sizeof(field->m_DataType);
        memcpy(&field->m_Size, getp, sizeof(field->m_Size));
        getp += sizeof(field->m_Size);
        remaining -= sizeof(field->m_Size);
        field->m_Data = new uint8_t[be16toh(field->m_Size)];
        memcpy(field->m_Data, getp, be16toh(field->m_Size));
        getp += be16toh(field->m_Size);
        remaining -= be16toh(field->m_Size);
        // Skip past any padding.
        getp += (4 - (be16toh(field->m_Size) & 3)) & 3;
        remaining -= (4 - (be16toh(field->m_Size) & 3)) & 3;
        field->Net_To_Host();
        Add_Field(field);
    }
}

PacketClass::~PacketClass()
{
    FieldClass *next = m_Head;

    while (next != nullptr) {
        FieldClass *tmp = next;
        next = next->m_Next;
        delete tmp;
    }
}

/**
 * Adds a field to the packet.
 */
void PacketClass::Add_Field(FieldClass *field)
{
    field->m_Next = m_Head;
    m_Head = field;
}

/**
 * Allocates space for and populates a data packet from the contents of the class.
 */
uint8_t *PacketClass::Create_Comm_Packet(unsigned &size)
{
    size = sizeof(m_Size) + sizeof(m_ID);

    for (FieldClass *i = m_Head; i != nullptr; i = i->m_Next) {
        size += sizeof(i->m_ID) + sizeof(i->m_DataType) + sizeof(i->m_Size);
        size += i->m_Size;
        size += (4 - (size & 3)) & 3; // Calc extra size to round up to 4 byte aligned fields.
    }

    // Create packet data and populate packet header with packet info.
    uint8_t *packet = new uint8_t[size];
    uint8_t *putp = packet;
    *reinterpret_cast<int16_t *>(putp) = htobe16(m_Size);
    putp += sizeof(m_Size);
    *reinterpret_cast<int16_t *>(putp) = htobe16(m_ID);
    putp += sizeof(m_ID);

    // Populate packet with field data
    for (FieldClass *i = m_Head; i != nullptr; i = i->m_Next) {
        i->Host_To_Net();
        memcpy(putp, i->m_ID, sizeof(i->m_ID));
        putp += sizeof(i->m_ID);
        memcpy(putp, &i->m_DataType, sizeof(i->m_DataType));
        putp += sizeof(i->m_DataType);
        memcpy(putp, &i->m_Size, sizeof(i->m_Size));
        putp += sizeof(i->m_Size);
        memcpy(putp, i->m_Data, be16toh(i->m_Size));
        putp += be16toh(i->m_Size);
        unsigned pad = (4 - (be16toh(i->m_Size) & 3)) & 3;

        if (pad != 0) {
            memset(putp, 0, pad);
            putp += pad;
        }

        i->Net_To_Host();
    }

    return packet;
}

/**
 * Finds a field with the given ID.
 *
 * @info Only the first 4 bytes are used to compare the id, if any more are provided they are ignored.
 */
FieldClass *PacketClass::Find_Field(const char *id)
{
    for (FieldClass *i = m_Head; i != nullptr; i = i->m_Next) {
        if (strncmp(i->m_ID, id, sizeof(i->m_ID)) == 0) {
            return i;
        }
    }

    return nullptr;
}

/**
 * Retrieves an 8bit integer of data from a field.
 */
BOOL PacketClass::Get_Field(const char *id, int8_t &data)
{
    FieldClass *field = Find_Field(id);

    if (field != nullptr) {
        memcpy(&data, field->m_Data, sizeof(data));
    }

    return field != nullptr;
}

/**
 * Retrieves an unsigned 8bit integer of data from a field.
 */
BOOL PacketClass::Get_Field(const char *id, uint8_t &data)
{
    FieldClass *field = Find_Field(id);

    if (field != nullptr) {
        memcpy(&data, field->m_Data, sizeof(data));
    }

    return field != nullptr;
}

/**
 * Retrieves a 16bit integer of data from a field.
 */
BOOL PacketClass::Get_Field(const char *id, int16_t &data)
{
    FieldClass *field = Find_Field(id);

    if (field != nullptr) {
        memcpy(&data, field->m_Data, sizeof(data));
    }

    return field != nullptr;
}

/**
 * Retrieves an unsigned 16bit integer of data from a field.
 */
BOOL PacketClass::Get_Field(const char *id, uint16_t &data)
{
    FieldClass *field = Find_Field(id);

    if (field != nullptr) {
        memcpy(&data, field->m_Data, sizeof(data));
    }

    return field != nullptr;
}

/**
 * Retrieves a 32bit integer of data from a field.
 */
BOOL PacketClass::Get_Field(const char *id, int32_t &data)
{
    FieldClass *field = Find_Field(id);

    if (field != nullptr) {
        memcpy(&data, field->m_Data, sizeof(data));
    }

    return field != nullptr;
}

/**
 * Retrieves an unsigned 32bit integer of data from a field.
 */
BOOL PacketClass::Get_Field(const char *id, uint32_t &data)
{
    FieldClass *field = Find_Field(id);

    if (field != nullptr) {
        memcpy(&data, field->m_Data, sizeof(data));
    }

    return field != nullptr;
}

/**
 * Retrieves a string of data from a field.
 *
 * @warning No checking is done to make sure the provided string buffer can hold the retrieved string, overruns possible.
 */
BOOL PacketClass::Get_Field(const char *id, char *data)
{
    FieldClass *field = Find_Field(id);

    if (field != nullptr) {
        strcpy(data, reinterpret_cast<char *>(field->m_Data));
    }

    return field != nullptr;
}

/**
 * Retrieves an arbitrary blob of data from a field.
 *
 * @param size Provides the size of the buffer that will hold the data and will hold the actual size when retrieved.
 */
BOOL PacketClass::Get_Field(const char *id, void *data, unsigned &size)
{
    FieldClass *field = Find_Field(id);

    if (field != nullptr) {
        unsigned data_size = std::min<unsigned>(field->m_Size, size);
        memcpy(data, field->m_Data, data_size);
        size = field->m_Size;
    }

    return field != nullptr;
}
