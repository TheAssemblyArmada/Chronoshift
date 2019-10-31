/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Straw stream interface for reading PCX RLE compressed data.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "always.h"
#include "pcxstraw.h"
#include "gamedebug.h"
#include "pcxrle.h"
#include <climits>
#include <cstring>
#include <algorithm>

using std::memcpy;

PCXStraw::PCXStraw(StrawControl mode, unsigned pitch, unsigned width) :
    Straw(),
    m_Mode(mode),
    m_LinePitch(pitch),
    m_LineWidth(width),
    m_Carryover(0),
    m_Remaining(0),
    m_InBuffer(nullptr),
    m_OutBuffer(nullptr)
{
    // Must be less than half of int max
    DEBUG_ASSERT(m_LinePitch < (INT_MAX / 2) && m_LinePitch > 0);

    m_LineWidth = std::min(m_LineWidth, m_LinePitch);

    if (m_Mode == STRAW_ENCODE) {
        m_InBuffer = new uint8_t[m_LinePitch];
        m_OutBuffer = new uint8_t[2 * m_LinePitch];
    } else {
        m_InBuffer = new uint8_t[2 * m_LinePitch];
        m_OutBuffer = new uint8_t[m_LinePitch];
    }
}

PCXStraw::~PCXStraw()
{
    delete[] m_InBuffer;
    delete[] m_OutBuffer;
}

/**
 * reads data from the straw strream to the buffer.
 */
int PCXStraw::Get(void *buffer, int length)
{
    int bytes_read = 0;
    int retrieved = 0;

    if (!buffer || (length < 1)) {
        return bytes_read;
    }

    while (length > 0) {
        // Handle where we have carryover from the encode/decode
        if (m_Carryover > 0) {
            int to_copy = std::min(m_Carryover, length);
            memcpy(buffer, m_OutBuffer + (m_LinePitch - m_Carryover), to_copy);
            buffer = static_cast<char *>(buffer) + to_copy;
            length -= to_copy;
            m_Carryover -= to_copy;
            bytes_read += to_copy;
        }

        if (length < 0) {
            break;
        }

        if (m_Mode == STRAW_DECODE) {
            // Read in worstcase number of bytes and decode
            uint8_t *in_buff = m_InBuffer;
            retrieved = Straw::Get(in_buff + m_Remaining, 2 * m_LinePitch - m_Remaining);
            int decoded = PCX_Decode((void const **)&in_buff, 2 * m_LinePitch, m_OutBuffer, m_LinePitch);
            if (decoded != m_LinePitch) {
                DEBUG_LOG("PCXStraw failed to decode an entire line\n");
                break;
            }

            // Calculate remaining encoded bytes (if any) and move to start of buffer.
            m_Remaining = 2 * m_LinePitch - (in_buff - m_InBuffer);

            if (m_Remaining > 0) {
                memmove(m_InBuffer, in_buff, m_Remaining);
            }

            int to_copy = std::min(m_LineWidth, length);

            memcpy(buffer, m_OutBuffer, to_copy);
            buffer = static_cast<char *>(buffer) + to_copy;
            length -= to_copy;
            bytes_read += to_copy;
            m_Carryover = m_LineWidth - to_copy;

            // Are we out of data to handle? If so, exit loop and return.
            if (!retrieved && !m_Remaining && !m_Carryover) {
                break;
            }
        } else {
            // TODO Reading to RLE encoded format not supported yet
            return Straw::Get(buffer, length);
        }
    }

    return bytes_read;
}
