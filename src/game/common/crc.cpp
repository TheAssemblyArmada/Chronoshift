/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief CRC calculating classes.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "crc.h"
#include "endiantype.h"
#include "gamedebug.h"

void CRCEngine::operator()(char datum)
{
    m_buffer.bytes[m_index] = datum;
    ++m_index;

    if (m_index == ARRAY_SIZE(m_buffer.bytes)) {
        m_crc = Value();
        m_buffer.integer = 0;
        m_index = 0;
    }
}

int32_t CRCEngine::Value()
{
    if (Buffer_Needs_Data()) {
        return __rotl32(m_crc, 1) + m_buffer.integer;
    }

    return m_crc;
}

int32_t CRCEngine::operator()(const void *data, unsigned int length)
{
    if (data != nullptr && length > 0) {
        const char *getp = static_cast<const char *>(data);
        unsigned int remaining = length;

        // Process first 4 bytes if we already had some bytes
        while (remaining != 0) {
            if (!Buffer_Needs_Data()) {
                break;
            }

            (*this)(*getp++);
            --remaining;
        }

        // Do some rotl on the data in blocks.
        const int32_t *getpi = reinterpret_cast<const int32_t *>(getp);
        int blockcount = remaining / 4;
        
		while (blockcount != 0) {
            m_crc = *getpi + __rotl32(m_crc, 1);
            ++getpi;
            remaining -= sizeof(*getpi);
            --blockcount;
        }

        // Move getp to where getpi got to.
        getp = reinterpret_cast<const char *>(getpi);

        // This will catch any bytes remaining.
        while (remaining != 0) {
            (*this)(*getp++);
            --remaining;
        }
    }

    return Value();
}

int32_t __cdecl Calculate_CRC(const void *buffer, unsigned int length)
{
    return CRCEngine()(buffer, length);
}
