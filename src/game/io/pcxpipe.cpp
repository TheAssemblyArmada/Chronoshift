/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Pipe stream interface for writing PCX RLE compressed data.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "always.h"
#include "pcxpipe.h"
#include "pcxrle.h"
#include <climits>
#include <cstring>
#include <algorithm>
#include <captainslog.h>

using std::memcpy;

PCXPipe::PCXPipe(PipeControl mode, int size) :
    Pipe(),
    m_Mode(mode),
    m_LineLength(size),
    m_Carryover(0),
    m_Remaining(0),
    m_InBuffer(nullptr),
    m_OutBuffer(nullptr)
{
    // Must be less than half of int max
    captainslog_assert(m_LineLength < (INT_MAX / 2) && m_LineLength > 0);

    if (m_Mode == PIPE_ENCODE) {
        m_InBuffer = new uint8_t[m_LineLength];
        m_OutBuffer = new uint8_t[2 * m_LineLength];
    } else {
        m_InBuffer = new uint8_t[2 * m_LineLength];
        m_OutBuffer = new uint8_t[m_LineLength];
    }
}

PCXPipe::~PCXPipe()
{
    delete[] m_InBuffer;
    delete[] m_OutBuffer;
}

/**
 * Writes data from the buffer to the pipe stream.
 */
int PCXPipe::Put(const void *buffer, int length)
{
    int bytes_put = 0;

    if (!buffer || (length < 1)) {
        return bytes_put;
    }

    while (length > 0) {
        if (m_Carryover) {
            int to_copy = std::min(m_Carryover, length);
            bytes_put += Pipe::Put(m_OutBuffer + (m_LineLength - m_Carryover), to_copy);
            length -= to_copy;
            m_Carryover -= to_copy;
        }

        if (length < 0) {
            break;
        }

        if (m_Mode == PIPE_ENCODE) {
            // Read in bytes and encode
            uint8_t *in_buff = m_InBuffer;
            memcpy(in_buff + m_Remaining, buffer, m_LineLength - m_Remaining);
            buffer = static_cast<const uint8_t *>(buffer) + m_LineLength - m_Remaining;
            int enc = PCX_Encode((const void **)&in_buff, m_LineLength, m_OutBuffer, 2 * m_LineLength);

            // Calculate remaining decoded bytes (if any) and move to start of buffer.
            m_Remaining = m_LineLength - (in_buff - m_InBuffer);
            if (m_Remaining > 0) {
                memmove(m_InBuffer, in_buff, m_Remaining);
            }

            int to_copy = std::min(enc, length);
            bytes_put += Pipe::Put(m_OutBuffer, to_copy);
            length -= m_LineLength - m_Remaining;
            m_Carryover = enc - to_copy;

        } else {
            // TODO Writing to RLE decoded format not supported yet
            return Pipe::Put(buffer, length);
        }
    }

    return bytes_put; // TODO based on LCW pipe
}

/**
 * Flushes the pipe streams buffers ensuring all changes are committed.
 */
int PCXPipe::Flush()
{
    uint8_t *buff;
    int len = 0;
    int rlelen = 0;

    if (m_Carryover) {
        len += Pipe::Put(m_OutBuffer + (m_LineLength - m_Carryover), m_Carryover);
    }

    while (m_Remaining) {
        if (m_Mode == PIPE_ENCODE) {
            buff = m_OutBuffer;
            uint8_t *in_buff = m_InBuffer;
            rlelen = PCX_Encode((const void **)&in_buff, m_Remaining, m_OutBuffer, 2 * m_LineLength);
            len = Pipe::Put(buff, rlelen);
            m_Remaining -= (in_buff - m_InBuffer);
        } else {
            // TODO proper implementation of decoding to dest.
            len += Pipe::Put(m_InBuffer, m_Remaining);
            m_Remaining = 0;
        }
    }

    return len + Pipe::Flush();
}
