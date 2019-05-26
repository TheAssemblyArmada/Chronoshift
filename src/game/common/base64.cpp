/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Functions for base64 encoding and decoding.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "base64.h"
#include "gamedebug.h"
#include "endiantype.h"

static const char _pad = '=';
static const char _encoder[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const uint8_t _decoder[256] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,
    52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,
    255, 254, 255, 255, 255,   0,   1,   2,   3,   4,   5,   6,
    7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,
    19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,
    255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,
    37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
    49,  50,  51, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255 
};

/**
 * @brief Encode data to Base64 format.
 */
int Base64_Encode(const void *src, int slen, void *dst, int dlen)
{
    char *putp = static_cast<char *>(dst);
    const uint8_t *getp = static_cast<const uint8_t *>(src);
    int total = 0;

    if (src != nullptr && slen > 0 && dst != nullptr && dlen > 0) {
        while (slen > 0) {
            // Avoid buffer overrun
            if (dlen < 4) {
                break;
            }

            int padding = 0;
            uint32_t packedbytes = 0;

            GETBYTE32(packedbytes, 2) = *getp++;
            --slen;

            if (slen) {
                GETBYTE32(packedbytes, 1) = *getp++;
                --slen;
            } else {
                padding = 1;
            }

            if (slen) {
                GETBYTE32(packedbytes, 0) = *getp++;
                --slen;
            } else {
                padding++;
            }

            // Bytes will be in wrong order on big endian arch, so handle here.
            packedbytes = htole32(packedbytes);
            *putp++ = _encoder[(packedbytes << 8) >> 26];
            *putp++ = _encoder[(packedbytes << 14) >> 26];
            *putp++ = padding >= 2 ? _pad : _encoder[(packedbytes << 20) >> 26];
            *putp++ = padding >= 1 ? _pad : _encoder[packedbytes & 0x3F];
            dlen -= 4;
            total += 4;
        }

        // Null terminate our C string if buffer is longer than resulting data.
        if (dlen > 0) {
            *putp = '\0';
        }
    }

    return total;
}

/**
 * @brief Decode data from Base64 format.
 */
int Base64_Decode(const void *src, int slen, void *dst, int dlen)
{
    const uint8_t *getp = static_cast<const uint8_t *>(src);
    uint8_t *putp = static_cast<uint8_t *>(dst);
    int total = 0;

    if (src != nullptr && slen > 0 && dst != nullptr && dlen > 0) {
        while (slen > 0 && dlen > 0) {
            int blocksize = 0;
            uint32_t packedbytes = 0;

            // This loop unpacks 4 chars into an int
            while (slen > 0 && blocksize < 4) {
                char c = _decoder[*getp++];
                --slen;

                if (c != -2) {
                    // This is an error case and should never be reached on valid input
                    if (c == -1) {
                        slen = 0;
                        break;
                    }

                    switch (blocksize) {
                        case 0:
                            packedbytes = (packedbytes & 0xFF03FFFF) | ((c & 0x3F) << 18);
                            break;

                        case 1:
                            packedbytes = (packedbytes & 0xFFFC0FFF) | ((c & 0x3F) << 12);
                            break;

                        case 2:
                            packedbytes = (packedbytes & 0xFFFFF03F) | ((c & 0x3F) << 6);
                            break;

                        case 3:
                            packedbytes = (packedbytes & 0xFFFFFFC0) | (c & 0x3F);
                            break;

                        default:
                            break;
                    }

                    ++blocksize;
                }
            }

            // Handle system endianness so bytes are where we expect.
            packedbytes = htole32(packedbytes);

            *putp++ = GETBYTE32(packedbytes, 2);
            ++total;
            --dlen;

            if (dlen > 0 && blocksize > 2) {
                *putp++ = GETBYTE32(packedbytes, 1);
                --dlen;
                ++total;
            }

            if (dlen > 0 && blocksize > 3) {
                *putp++ = GETBYTE32(packedbytes, 0);
                --dlen;
                ++total;
            }
        }
    }

    return total;
}
