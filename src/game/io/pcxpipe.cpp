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
#include "gamedebug.h"
#include "minmax.h"
#include "pcxrle.h"
#include <climits>
#include <cstring>

using std::memcpy;

PCXPipe::PCXPipe(PipeControl mode, int size) :
    Pipe(),
    Mode(mode),
    LineLength(size),
    Carryover(0),
    Remaining(0),
    InBuffer(nullptr),
    OutBuffer(nullptr)
{
    // Must be less than half of int max
    DEBUG_ASSERT(LineLength < (INT_MAX / 2) && LineLength > 0);

    if (Mode == PIPE_ENCODE) {
        InBuffer = new uint8_t[LineLength];
        OutBuffer = new uint8_t[2 * LineLength];
    } else {
        InBuffer = new uint8_t[2 * LineLength];
        OutBuffer = new uint8_t[LineLength];
    }
}

PCXPipe::~PCXPipe()
{
    delete[] InBuffer;
    delete[] OutBuffer;
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
        if (Carryover) {
            int to_copy = Min(Carryover, length);
            bytes_put += Pipe::Put(OutBuffer + (LineLength - Carryover), to_copy);
            length -= to_copy;
            Carryover -= to_copy;
        }

        if (length < 0) {
            break;
        }

        if (Mode == PIPE_ENCODE) {
            // Read in bytes and encode
            uint8_t *in_buff = InBuffer;
            memcpy(in_buff + Remaining, buffer, LineLength - Remaining);
            buffer = static_cast<const uint8_t *>(buffer) + LineLength - Remaining;
            int enc = PCX_Encode((const void **)&in_buff, LineLength, OutBuffer, 2 * LineLength);

            // Calculate remaining decoded bytes (if any) and move to start of buffer.
            Remaining = LineLength - (in_buff - InBuffer);
            if (Remaining > 0) {
                memmove(InBuffer, in_buff, Remaining);
            }

            int to_copy = Min(enc, length);
            bytes_put += Pipe::Put(OutBuffer, to_copy);
            length -= LineLength - Remaining;
            Carryover = enc - to_copy;

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

    if (Carryover) {
        len += Pipe::Put(OutBuffer + (LineLength - Carryover), Carryover);
    }

    while (Remaining) {
        if (Mode == PIPE_ENCODE) {
            buff = OutBuffer;
            uint8_t *in_buff = InBuffer;
            rlelen = PCX_Encode((const void **)&in_buff, Remaining, OutBuffer, 2 * LineLength);
            len = Pipe::Put(buff, rlelen);
            Remaining -= (in_buff - InBuffer);
        } else {
            // TODO proper implementation of decoding to dest.
            len += Pipe::Put(InBuffer, Remaining);
            Remaining = 0;
        }
    }

    return len + Pipe::Flush();
}
