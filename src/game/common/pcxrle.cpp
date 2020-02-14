/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief PCX RLE algorithm specialised for pipe/straw stream system.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "pcxrle.h"
#include <algorithm>
#include <captainslog.h>

#ifdef COMPILER_WATCOM
enum EncodingFlags
#else
enum EncodingFlags : uint8_t
#endif
{
    RLE_FLAG = 0xC0,
    RLE_MAX_RUN = 63,
};

/**
 * Encodes data with the PCX RLE algorithm.
 */
int PCX_Encode(void const **source, int srclength, void *dest, int destlength)
{
    const uint8_t *getp = static_cast<const uint8_t *>(*source);
    const uint8_t *getendp = getp + srclength;
    uint8_t *putp = static_cast<uint8_t *>(dest);
    uint8_t *putendp = putp + destlength;

    if (!*source || !dest || srclength < 1 || destlength < 1) {
        return 0;
    }

    uint8_t current;
    uint8_t last = *getp++;
    uint8_t count = 1;

    while (getp != getendp && putp != putendp) {
        current = *getp++;

        if (last == current && count < 63) {
            count++;
        } else {
            while (count && putp != putendp) {
                if (count == 1 && last < 0xC0) {
                    *putp++ = last;
                    count = 0;
                } else {
                    // int write = count > 63 ? 63 : count;
                    int write = std::min(count, (uint8_t)RLE_MAX_RUN);
                    *putp++ = 0xC0 | write;
                    *putp++ = last;
                    count -= write;
                }
            }

            count = 1;
            last = current;
        }
    }

    while (count && putp != putendp) {
        if (count == 1 && last < 0xC0) {
            *putp++ = last;
            count = 0;
        } else {
            int write = std::min(count, (uint8_t)RLE_MAX_RUN);
            *putp++ = 0xC0 | write;
            *putp++ = last;
            count -= write;
        }
    }

    // Set source pointer to new location in data buffer
    *source = getp;
    return putp - static_cast<uint8_t *>(dest);
}

/**
 * Encodes data with the PCX RLE algorithm.
 */
int PCX_Decode(void const **source, int srclength, void *dest, int destlength)
{
    const uint8_t *getp = static_cast<const uint8_t *>(*source);
    const uint8_t *getendp = getp + srclength;
    uint8_t *putp = static_cast<uint8_t *>(dest);
    uint8_t *putendp = putp + destlength;

    if (!*source || !dest || srclength < 1 || destlength < 1) {
        return 0;
    }

    // RLE decoding loop
    while (getp != getendp && putp != putendp) {
        uint8_t value = *getp++;

        if ((value & RLE_FLAG) == RLE_FLAG) {
            uint8_t count = value & ~RLE_FLAG;
            if (getp == getendp) {
                // get_next = true;
                break;
            }
            uint8_t copy = *getp++;

            while (count--) {
                if (putp == putendp) {
                    break;
                }

                *putp++ = copy;
            }
        } else {
            *putp++ = value;
        }
    }

    // Set source pointer to new location in the data buffer
    *source = getp;
    return putp - static_cast<uint8_t *>(dest);
}
