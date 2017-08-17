/**
* @file
*
* @Author CCHyper, OmniBlade
*
* @brief CRC calculating classes.
*
* @copyright Redalert++ is free software: you can redistribute it and/or
*            modify it under the terms of the GNU General Public License
*            as published by the Free Software Foundation, either version
*            2 of the License, or (at your option) any later version.
*
*            A full copy of the GNU General Public License can be found in
*            LICENSE
*/
#include "crc.h"
#include "endiantype.h"
#include "gamedebug.h"
#include "rotl.h"

void CRCEngine::operator()(char datum)
{
    m_buffer.bytes[m_index] = datum;

    ++m_index;

    if (m_index == ARRAY_SIZE(m_buffer.bytes)) {
        m_crc = Result();
        m_buffer.integer = 0;
        m_index = 0;
    }
}

uint32_t CRCEngine::Result()
{
    if (m_index > 0) {
        return RotateLeft(m_crc, 1) + m_buffer.integer;
    }

    return m_crc;
}

int32_t CRCEngine::operator()(const void *data, unsigned int length)
{
    const char *getp;
    unsigned int remaining;

    if (data != nullptr && length > 0) {
        getp = static_cast<const char *>(data);

        // process first 4 bytes if we already had some bytes
        for (remaining = length; remaining != 0 && m_index != 0; --remaining) {
            (*this)(*getp++);
        }

        // do some rotl on the data in blocks.
        int blockcount = (remaining / 4) - 1;
        if (remaining / 4) {
            while (blockcount != -1) {
                m_crc = *reinterpret_cast<const int32_t *>(getp) + RotateLeft(m_crc, 1);
                getp += 4;
                remaining -= 4;
                --blockcount;
            }
        }

        // This will catch any bytes remaining.
        for (; remaining; --remaining) {
            (*this)(*getp++);
        }
    }

    return Result();
}

int32_t __cdecl Calculate_CRC(const void *buffer, unsigned int length)
{
    CRCEngine crc;

    return crc(buffer, length);
}
