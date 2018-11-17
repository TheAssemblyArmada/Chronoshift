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
#include "minmax.h"
#include "pcxrle.h"
#include <climits>
#include <cstring>

using std::memcpy;

PCXStraw::PCXStraw(StrawControl mode, unsigned pitch, unsigned width) :
    Straw(),
    Mode(mode),
    LinePitch(pitch),
    LineWidth(width),
    Carryover(0),
    Remaining(0),
    InBuffer(nullptr),
    OutBuffer(nullptr)
{
    // Must be less than half of int max
    DEBUG_ASSERT(LinePitch < (INT_MAX / 2) && LinePitch > 0);

    LineWidth = Min(LineWidth, LinePitch);

    if (Mode == STRAW_ENCODE) {
        InBuffer = new uint8_t[LinePitch];
        OutBuffer = new uint8_t[2 * LinePitch];
    } else {
        InBuffer = new uint8_t[2 * LinePitch];
        OutBuffer = new uint8_t[LinePitch];
    }
}

PCXStraw::~PCXStraw()
{
    delete[] InBuffer;
    delete[] OutBuffer;
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
        if (Carryover > 0) {
            int to_copy = Min(Carryover, length);
            memcpy(buffer, OutBuffer + (LinePitch - Carryover), to_copy);
            buffer = static_cast<char *>(buffer) + to_copy;
            length -= to_copy;
            Carryover -= to_copy;
            bytes_read += to_copy;
        }

        if (length < 0) {
            break;
        }

        if (Mode == STRAW_DECODE) {
            // Read in worstcase number of bytes and decode
            uint8_t *in_buff = InBuffer;
            retrieved = Straw::Get(in_buff + Remaining, 2 * LinePitch - Remaining);
            int decoded = PCX_Decode((void const **)&in_buff, 2 * LinePitch, OutBuffer, LinePitch);
            if (decoded != LinePitch) {
                DEBUG_LOG("PCXStraw failed to decode an entire line\n");
                break;
            }

            // Calculate remaining encoded bytes (if any) and move to start of buffer.
            Remaining = 2 * LinePitch - (in_buff - InBuffer);

            if (Remaining > 0) {
                memmove(InBuffer, in_buff, Remaining);
            }

            int to_copy = Min(LineWidth, length);

            memcpy(buffer, OutBuffer, to_copy);
            buffer = static_cast<char *>(buffer) + to_copy;
            length -= to_copy;
            bytes_read += to_copy;
            Carryover = LineWidth - to_copy;

            // Are we out of data to handle? If so, exit loop and return.
            if (!retrieved && !Remaining && !Carryover) {
                break;
            }
        } else {
            // TODO Reading to RLE encoded format not supported yet
            return Straw::Get(buffer, length);
        }
    }

    return bytes_read;
}
