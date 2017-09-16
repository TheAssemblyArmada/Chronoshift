/**
 * @file
 *
 * @Author OmniBlade
 *
 * @brief Implementation of XORDelta, a custom binary diff format used in many Westwood games.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "xordelta.h"
#include "gamedebug.h"

#define XOR_SMALL 127
#define XOR_MED 255
#define XOR_LARGE 16383
#define XOR_MAX 32767

////////////////////////////////////////////////////////////////////////////////
//  Notes
////////////////////////////////////////////////////////////////////////////////
// The delta generator encoder works to the following assumptions
// 1. Any skip command is preferable if source and base are same
// 2. Fill is preferable to XOR if 4 or larger, XOR takes same data plus at
//    least 1 byte
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Applies a binary delta to a buffer.
 * @warning Assumes the destination buffer is of the correct size.
 */
void __cdecl Apply_XOR_Delta(void *dst, void *src)
{
    int8_t *putp = static_cast<int8_t *>(dst);
    int8_t *getp = static_cast<int8_t *>(src);
    uint16_t count = 0;
    int8_t value = 0;
    int8_t cmd = 0;

    while (true) {
        // DEBUG_LOG("XOR_Delta Put pos: %u, Get pos: %u.... ", putp - static_cast<int8_t*>(dest), getp -
        // static_cast<int8_t*>(source));
        cmd = *getp++;
        count = cmd;
        bool xorval = false;

        if (cmd >= 0) {
            // 0b00000000
            if (cmd == 0) {
                count = *getp++ & 0xFF;
                value = *getp++;
                xorval = true;
                // DEBUG_LOG("0b00000000 Val Count %d ", count);
                // 0b0???????
            } else {
                // DEBUG_LOG("0b0??????? XOR Count %d ", count);
            }
        } else {
            // 0b1??????? remove most significant bit
            count &= 0x7F;
            if (count != 0) {
                putp += count;
                // DEBUG_LOG("0b1??????? Skip Count %d\n", count);
                continue;
            }

            count = (*getp & 0xFF) + (*(getp + 1) << 8);
            getp += 2;

            // DEBUG_LOG("Eval %u ", count);

            // 0b10000000 0 0
            if (count == 0) {
                // DEBUG_LOG("0b10000000 Count %d to end delta\n", count);
                return;
            }

            // 0b100000000 0?
            if ((count & 0x8000) == 0) {
                putp += count;
                // DEBUG_LOG("0b100000000 0? Skip Count %d\n", count);
                continue;
            } else {
                // 0b10000000 11
                if (count & 0x4000) {
                    count &= 0x3FFF;
                    value = *getp++;
                    // DEBUG_LOG("0b10000000 11 Val Count %d ", count);
                    xorval = true;
                    // 0b10000000 10
                } else {
                    count &= 0x3FFF;
                    // DEBUG_LOG("0b10000000 10 XOR Count %d ", count);
                }
            }
        }

        if (xorval) {
            // DEBUG_LOG("XOR Val %d\n", value);
            for (; count > 0; --count) {
                *putp++ ^= value;
            }
        } else {
            // DEBUG_LOG("XOR Source to Dest\n");
            for (; count > 0; --count) {
                *putp++ ^= *getp++;
            }
        }
    }
}

void Copy_Delta_Buffer(int width, void *offset, void *delta, int pitch)
{
    int8_t *putp = static_cast<int8_t *>(offset);
    int8_t *getp = static_cast<int8_t *>(delta);
    int8_t value = 0;
    int8_t cmd = 0;
    int length = 0;
    int count = 0;

    while (true) {
        // DEBUG_LOG("XOR_Delta Put pos: %u, Get pos: %u.... ", putp - static_cast<int8_t*>(dest), getp -
        // static_cast<int8_t*>(source));
        cmd = *getp++;
        count = cmd;
        bool xorval = false;

        if (cmd >= 0) {
            // 0b00000000
            if (cmd == 0) {
                count = *getp++ & 0xFF;
                value = *getp++;
                xorval = true;
                // DEBUG_LOG("0b00000000 Val Count %d ", count);
                // 0b0???????
            } else {
                // DEBUG_LOG("0b0??????? XOR Count %d ", count);
            }
        } else {
            // 0b1??????? remove most significant bit
            count &= 0x7F;
            if (count != 0) {
                putp -= length;
                length += count;

                while (length >= width) {
                    length -= width;
                    putp += pitch;
                }

                putp += length;
                // DEBUG_LOG("0b1??????? Skip Count %d\n", count);
                continue;
            }

            count = (*getp & 0xFF) + (*(getp + 1) << 8);
            getp += 2;

            // DEBUG_LOG("Eval %u ", count);

            // 0b10000000 0 0
            if (count == 0) {
                // DEBUG_LOG("0b10000000 Count %d to end delta\n", count);
                return;
            }

            // 0b100000000 0?
            if ((count & 0x8000) == 0) {
                putp -= length;
                length += count;

                while (length >= width) {
                    length -= width;
                    putp += pitch;
                }

                putp += length;
                // DEBUG_LOG("0b100000000 0? Skip Count %d\n", count);
                continue;
            } else {
                // 0b10000000 11
                if (count & 0x4000) {
                    count &= 0x3FFF;
                    value = *getp++;
                    // DEBUG_LOG("0b10000000 11 Val Count %d ", count);
                    xorval = true;
                    // 0b10000000 10
                } else {
                    count &= 0x3FFF;
                    // DEBUG_LOG("0b10000000 10 XOR Count %d ", count);
                }
            }
        }

        if (xorval) {
            // DEBUG_LOG("XOR Val %d\n", value);
            for (; count > 0; --count) {
                *putp++ = value;
                ++length;

                if (length == width) {
                    length = 0;
                    putp += pitch - width;
                }
            }
        } else {
            // DEBUG_LOG("XOR Source to Dest\n");
            for (; count > 0; --count) {
                *putp++ = *getp++;
                ++length;

                if (length == width) {
                    length = 0;
                    putp += pitch - width;
                }
            }
        }
    }
}

void XOR_Delta_Buffer(int width, void *offset, void *delta, int pitch)
{
    int8_t *putp = static_cast<int8_t *>(offset);
    int8_t *getp = static_cast<int8_t *>(delta);
    int8_t value = 0;
    int8_t cmd = 0;
    int length = 0;
    int count = 0;

    while (true) {
        // DEBUG_LOG("XOR_Delta Put pos: %u, Get pos: %u.... ", putp - static_cast<int8_t*>(dest), getp -
        // static_cast<int8_t*>(source));
        cmd = *getp++;
        count = cmd;
        bool xorval = false;

        if (cmd >= 0) {
            // 0b00000000
            if (cmd == 0) {
                count = *getp++ & 0xFF;
                value = *getp++;
                xorval = true;
                // DEBUG_LOG("0b00000000 Val Count %d ", count);
                // 0b0???????
            } else {
                // DEBUG_LOG("0b0??????? XOR Count %d ", count);
            }
        } else {
            // 0b1??????? remove most significant bit
            count &= 0x7F;
            if (count != 0) {
                putp -= length;
                length += count;

                while (length >= width) {
                    length -= width;
                    putp += pitch;
                }

                putp += length;
                // DEBUG_LOG("0b1??????? Skip Count %d\n", count);
                continue;
            }

            count = (*getp & 0xFF) + (*(getp + 1) << 8);
            getp += 2;

            // DEBUG_LOG("Eval %u ", count);

            // 0b10000000 0 0
            if (count == 0) {
                // DEBUG_LOG("0b10000000 Count %d to end delta\n", count);
                return;
            }

            // 0b100000000 0?
            if ((count & 0x8000) == 0) {
                putp -= length;
                length += count;

                while (length >= width) {
                    length -= width;
                    putp += pitch;
                }

                putp += length;
                // DEBUG_LOG("0b100000000 0? Skip Count %d\n", count);
                continue;
            } else {
                // 0b10000000 11
                if (count & 0x4000) {
                    count &= 0x3FFF;
                    value = *getp++;
                    // DEBUG_LOG("0b10000000 11 Val Count %d ", count);
                    xorval = true;
                    // 0b10000000 10
                } else {
                    count &= 0x3FFF;
                    // DEBUG_LOG("0b10000000 10 XOR Count %d ", count);
                }
            }
        }

        if (xorval) {
            // DEBUG_LOG("XOR Val %d\n", value);
            for (; count > 0; --count) {
                *putp++ ^= value;
                ++length;

                if (length == width) {
                    length = 0;
                    putp += pitch - width;
                }
            }
        } else {
            // DEBUG_LOG("XOR Source to Dest\n");
            for (; count > 0; --count) {
                *putp++ ^= *getp++;
                ++length;

                if (length == width) {
                    length = 0;
                    putp += pitch - width;
                }
            }
        }
    }
}

/**
 * @brief Applies a binary delta to a viewport.
 * @warning Assumes the destination buffer is of the correct size.
 */
void __cdecl Apply_XOR_Delta_To_Page_Or_Viewport(void *offset, void *delta, int width, int pitch, BOOL use_xor)
{
    DEBUG_LOG("Applying delta to viewport.\n");
    if (use_xor) {
        XOR_Delta_Buffer(width, offset, delta, pitch);
    } else {
        Copy_Delta_Buffer(width, offset, delta, pitch);
    }
}

/**
 * @brief Generates a binary delta between two buffers. Mainly used for image data.
 * @warning Delta can be larger than the source buffers in worse case, allocate storage accordingly.
 */
int Generate_XOR_Delta(void *dst, void *src, void *base, int size)
{
    uint8_t *putp = static_cast<uint8_t *>(dst); // our delta
    uint8_t *getsp = static_cast<uint8_t *>(src); // This is the image we go to
    uint8_t *getbp = static_cast<uint8_t *>(base); // This is image we come from
    uint8_t *getsendp = getsp + size;
    // uint8_t *getsstartp = getsp;				//This is only needed for the //DEBUG_LOG calls

    // only check getsp. Both source and base should be same size and both pointers
    // should be incremented at the same time.
    while (getsp < getsendp) {
        // DEBUG_LOG("XOR_Delta Put pos: %u, Get pos: %u.... ", putp - static_cast<uint8_t*>(dest), getsp - getsstartp);
        unsigned fillcount = 0;
        unsigned xorcount = 0;
        unsigned skipcount = 0;
        uint8_t lastxor = *getsp ^ *getbp;
        uint8_t *testsp = getsp;
        uint8_t *testbp = getbp;

        // Only evaluate other options if we don't have a matched pair
        while (*testsp != *testbp && testsp < getsendp) {
            if ((*testsp ^ *testbp) == lastxor) {
                ++fillcount;
                ++xorcount;
            } else {
                if (fillcount > 3) {
                    break;
                } else {
                    lastxor = *testsp ^ *testbp;
                    fillcount = 1;
                    ++xorcount;
                }
            }
            ++testsp;
            ++testbp;
        }

        DEBUG_ASSERT_PRINT(fillcount <= xorcount, "fillcount %d has exceeded xorcount %d.\n", fillcount, xorcount);

        fillcount = fillcount > 3 ? fillcount : 0;

        // Okay, lets see if we have any xor bytes we need to handle
        xorcount -= fillcount;
        while (xorcount) {
            uint16_t count = 0;
            // cmd 0???????
            if (xorcount < XOR_MED) {
                count = xorcount <= XOR_SMALL ? xorcount : XOR_SMALL;
                // DEBUG_LOG("0b0??????? XOR Count %u\n", count);
                *putp++ = count;
                // cmd 10000000 10?????? ??????
            } else {
                count = xorcount <= XOR_LARGE ? xorcount : XOR_LARGE;
                // DEBUG_LOG("0b10000000 10 XOR Count %u\n", count);
                *putp++ = 0x80;
                *putp++ = count;
                *putp++ = (count >> 8) | 0x80;
            }

            while (count) {
                *putp++ = *getsp++ ^ *getbp++;
                --count;
                --xorcount;
            }
        }

        // lets handle the bytes that are best done as xorfill
        while (fillcount) {
            uint16_t count = 0;
            // cmd 00000000 ????????
            if (fillcount <= XOR_MED) {
                // DEBUG_LOG("0b00000000 Val Count %u\n", count);
                count = fillcount;
                *putp++ = 0;
                *putp++ = count;
                // cmd 10000000 11?????? ??????
            } else {
                count = fillcount <= XOR_LARGE ? fillcount : XOR_LARGE;
                // DEBUG_LOG("0b10000000 11 Val Count %u\n", count);
                *putp++ = 0x80;
                *putp++ = count;
                *putp++ = (count >> 8) | 0xC0;
            }
            *putp++ = *getsp ^ *getbp;
            fillcount -= count;
            getsp += count;
            getbp += count;
        }

        // Handle regions that match exactly
        while (*testsp == *testbp && testsp < getsendp) {
            ++skipcount;
            ++testsp;
            ++testbp;
        }

        while (skipcount) {
            uint16_t count = 0;
            if (skipcount < XOR_MED) {
                count = skipcount <= XOR_SMALL ? skipcount : XOR_SMALL;
                // DEBUG_LOG("0b1??????? Skip Count %u\n", count);
                *putp++ = count | 0x80;
                // cmd 10000000 0??????? ????????
            } else {
                count = skipcount <= XOR_MAX ? skipcount : XOR_MAX;
                // DEBUG_LOG("0b100000000 0? Skip Count %u\n", count);
                *putp++ = 0x80;
                *putp++ = count;
                *putp++ = count >> 8;
            }
            skipcount -= count;
            getsp += count;
            getbp += count;
        }
    }

    // final skip command of 0;
    // DEBUG_LOG("0b10000000 Count %d to end delta\n", 0);
    *putp++ = 0x80;
    *putp++ = 0;
    *putp++ = 0;

    return putp - static_cast<uint8_t *>(dst);
}
