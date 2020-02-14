/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Implementation of LCW, a custom compression format used in many Westwood games.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "lcw.h"
#include <captainslog.h>
#include <limits.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
//  Notes
////////////////////////////////////////////////////////////////////////////////
//
// LCW streams should always start and end with the fill command (& 0x80) though
// the decompressor doesn't strictly require that it start with one the ability
// to use the offset commands in place of the RLE command early in the stream
// relies on it. Streams larger than 64k that need the relative versions of the
// 3 and 5 byte commands should start with a null byte before the first 0x80
// command to flag that they are relative compressed.
//
// LCW uses the following rules to decide which command to use:
// 1. Runs of the same colour should only use 4 byte RLE command if longer than
//    64 bytes. 2 and 3 byte offset commands are more efficient otherwise.
// 2. Runs of less than 3 should just be stored as is with the one byte fill
//    command.
// 3. Runs greater than 10 or if the relative offset is greater than
//    4095 use an absolute copy. Less than 64 bytes uses 3 byte command, else it
//    uses the 5 byte command.
// 4. If Absolute rule isn't met then copy from a relative offset with 2 byte
//    command.
//
// Absolute LCW can efficiently compress data that is 64k in size, much greater
// and relative offsets for the 3 and 5 byte commands are needed.
//
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Decompresses data in the proprietary LCW format used in many games developed by Westwood Studios.
 * @warning Data starting with a 0 will be treated as being compressed with relative offsets rather than absolute to start of
 * buffer.
 */
int __cdecl LCW_Uncomp(const void *src, void *dst, unsigned int bytes)
{
    captainslog_assert(src != nullptr);
    captainslog_assert(dst != nullptr);

    uint8_t *putstart = static_cast<uint8_t *>(dst);
    uint8_t *putend = putstart + bytes;
    uint8_t *putp = putstart;
    const uint8_t *getp = static_cast<const uint8_t *>(src);
    // const uint8_t *getstart = getp;

    // If first byte is 0, the all offsets are relative to current position. Otherwise some are absolute to the start of the
    // buffer, meaning only ~64KB can be compressed effectively. Compressor implemented in this file uses size to determine
    // compression scheme used.
    if (*getp == 0) {
        // captainslog_debug("LCW Relative Decompression... ");
        getp++;

        while (putp < putend) {
            uint8_t flag;
            uint16_t cpysize;
            uint16_t offset;

            flag = *getp++;

            if (flag & 0x80) {
                if (flag & 0x40) {
                    cpysize = (flag & 0x3F) + 3;
                    // long set 0b11111110
                    if (flag == 0xFE) {
                        cpysize = *getp++;
                        cpysize += (*getp++) << 8;

                        if (cpysize > putend - putp) {
                            cpysize = putend - putp;
                        }

                        // captainslog_debug("0b11111110 Source Pos %ld, Dest Pos %ld, Count %d", source - sstart - 3, dest -
                        // start, cpysize);
                        memset(putp, (*getp++), cpysize);
                        putp += cpysize;
                    } else {
                        uint8_t *s;
                        // long move, abs 0b11111111
                        if (flag == 0xFF) {
                            cpysize = *getp++;
                            cpysize += (*getp++) << 8;

                            if (cpysize > putend - putp) {
                                cpysize = putend - putp;
                            }

                            offset = *getp++;
                            offset += (*getp++) << 8;

                            // extended format for VQA32 and large WSA files.
                            s = putp - offset;

                            // captainslog_debug("0b11111111 Source Pos %ld, Dest Pos %ld, Count %d, Offset %d", source - sstart -
                            // 5, dest - start, cpysize, offset);
                            for (; cpysize > 0; --cpysize) {
                                *putp++ = *s++;
                            }
                            // short move abs 0b11??????
                        } else {
                            if (cpysize > putend - putp) {
                                cpysize = putend - putp;
                            }

                            offset = *getp++;
                            offset += (*getp++) << 8;

                            // extended format for VQA32 and large WSA files.
                            s = putp - offset;

                            // captainslog_debug("0b11?????? Source Pos %ld, Dest Pos %ld, Count %d, Offset %d", source - sstart -
                            // 3, dest - start, cpysize, offset);
                            for (; cpysize > 0; --cpysize) {
                                *putp++ = *s++;
                            }
                        }
                    }
                    // short copy 0b10??????
                } else {
                    if (flag == 0x80) {
                        // captainslog_debug("0b10?????? Source Pos %ld, Dest Pos %ld, Count %d", source - sstart - 1, dest -
                        // start, 0);
                        return putp - putstart;
                    }

                    cpysize = flag & 0x3F;

                    if (cpysize > putend - putp) {
                        cpysize = putend - putp;
                    }

                    // captainslog_debug("0b10?????? Source Pos %ld, Dest Pos %ld, Count %d", source - sstart - 1, dest - start,
                    // cpysize);
                    for (; cpysize > 0; --cpysize) {
                        *putp++ = *getp++;
                    }
                }
                // short move rel 0b0???????
            } else {
                cpysize = (flag >> 4) + 3;

                if (cpysize > putend - putp) {
                    cpysize = putend - putp;
                }

                offset = ((flag & 0xF) << 8) + (*getp++);
                // captainslog_debug("0b0??????? Source Pos %ld, Dest Pos %ld, Count %d, Offset %d", source - sstart - 2, dest -
                // start, cpysize, offset);
                for (; cpysize > 0; --cpysize) {
                    *putp = *(putp - offset);
                    putp++;
                }
            }
        }
    } else {
        // captainslog_debug("LCW Decompression... ");

        while (putp < putend) {
            uint8_t flag;
            uint16_t cpysize;
            uint16_t offset;

            flag = *getp++;

            if (flag & 0x80) {
                if (flag & 0x40) {
                    cpysize = (flag & 0x3F) + 3;
                    // long set 0b11111110
                    if (flag == 0xFE) {
                        cpysize = *getp++;
                        cpysize += (*getp++) << 8;

                        if (cpysize > putend - putp) {
                            cpysize = putend - putp;
                        }

                        // captainslog_debug("0b11111110 Source Pos %ld, Dest Pos %ld, Count %d", source - sstart - 3, dest -
                        // start, cpysize);
                        memset(putp, (*getp++), cpysize);
                        putp += cpysize;
                    } else {
                        uint8_t *s;
                        // long move, abs 0b11111111
                        if (flag == 0xFF) {
                            cpysize = *getp++;
                            cpysize += (*getp++) << 8;

                            if (cpysize > putend - putp) {
                                cpysize = putend - putp;
                            }

                            offset = *getp++;
                            offset += (*getp++) << 8;
                            s = putstart + offset;

                            // captainslog_debug("0b11111111 Source Pos %ld, Dest Pos %ld, Count %d, Offset %d", source - sstart -
                            // 5, dest - start, cpysize, offset);
                            for (; cpysize > 0; --cpysize) {
                                *putp++ = *s++;
                            }
                            // short move abs 0b11??????
                        } else {
                            if (cpysize > putend - putp) {
                                cpysize = putend - putp;
                            }

                            offset = *getp++;
                            offset += (*getp++) << 8;
                            s = putstart + offset;

                            // captainslog_debug("0b11?????? Source Pos %ld, Dest Pos %ld, Count %d, Offset %d", source - sstart -
                            // 3, dest - start, cpysize, offset);
                            for (; cpysize > 0; --cpysize) {
                                *putp++ = *s++;
                            }
                        }
                    }
                    // short copy 0b10??????
                } else {
                    if (flag == 0x80) {
                        // captainslog_debug("0b10?????? Source Pos %ld, Dest Pos %ld, Count %d", source - sstart - 1, dest -
                        // start, 0);
                        return putp - putstart;
                    }

                    cpysize = flag & 0x3F;

                    if (cpysize > putend - putp) {
                        cpysize = putend - putp;
                    }

                    // captainslog_debug("0b10?????? Source Pos %ld, Dest Pos %ld, Count %d", source - sstart - 1, dest - start,
                    // cpysize);
                    for (; cpysize > 0; --cpysize) {
                        *putp++ = *getp++;
                    }
                }
                // short move rel 0b0???????
            } else {
                cpysize = (flag >> 4) + 3;

                if (cpysize > putend - putp) {
                    cpysize = putend - putp;
                }

                offset = ((flag & 0xF) << 8) + (*getp++);
                // captainslog_debug("0b0??????? Source Pos %ld, Dest Pos %ld, Count %d, Offset %d", source - sstart - 2, dest -
                // start, cpysize, offset);
                for (; cpysize > 0; --cpysize) {
                    *putp = *(putp - offset);
                    putp++;
                }
            }
        }
    }

    return putp - putstart;
}

/**
 * @brief Compresses data to the proprietary LCW format used in many games developed by Westwood Studios.
 * @warning Worst case can have the compressed data larger than the original.
 */
int __cdecl LCW_Comp(const void *src, void *dst, unsigned int bytes)
{
    captainslog_assert(src != nullptr);
    captainslog_assert(dst != nullptr);

    if (!bytes) {
        return 0;
    }

    // Decide if we are going to do relative offsets for 3 and 5 byte commands
    bool relative = bytes > UINT16_MAX;

    const uint8_t *getp = static_cast<const uint8_t *>(src);
    uint8_t *putp = static_cast<uint8_t *>(dst);
    const uint8_t *getstart = getp;
    const uint8_t *getend = getp + bytes;
    uint8_t *putstart = putp;

    // captainslog_debug("LCW Compression... ");

    // relative LCW starts with 0 to flag as relative for decoder
    if (relative) {
        // captainslog_debug("LCW Relative compressor.");
        *putp++ = 0;
    }

    // Write a starting cmd1 and set bool to have cmd1 in progress
    uint8_t *cmd_onep = putp;
    *putp++ = 0x81;
    *putp++ = *getp++;
    bool cmd_one = true;

    // Compress data
    while (getp < getend) {
        // Is RLE encode (4bytes) worth evaluating?
        if (getend - getp > 64 && *getp == *(getp + 64)) {
            // RLE run length is encoded as a short so max is UINT16_MAX
            const uint8_t *rlemax = (getend - getp) < UINT16_MAX ? getend : getp + UINT16_MAX;
            const uint8_t *rlep;

            for (rlep = getp + 1; *rlep == *getp && rlep < rlemax; ++rlep)
                ;

            uint16_t run_length = rlep - getp;

            // If run length is long enough, write the command and start loop again
            if (run_length >= 0x41) {
                // captainslog_debug("0b11111110 Source Pos %ld, Dest Pos %ld, Count %d", getp - getstart, putp - putstart,
                // run_length);  write 4byte command 0b11111110
                cmd_one = false;
                *putp++ = 0xFE;
                *putp++ = run_length;
                *putp++ = run_length >> 8;
                *putp++ = *getp;
                getp = rlep;
                continue;
            }
        }

        // current block size for an offset copy
        int block_size = 0;
        const uint8_t *offstart;

        // Set where we start looking for matching runs.
        if (relative) {
            offstart = (getp - getstart) < UINT16_MAX ? getstart : getp - UINT16_MAX;
        } else {
            offstart = getstart;
        }

        // Look for matching runs
        const uint8_t *offchk = offstart;
        const uint8_t *offsetp = getp;
        while (offchk < getp) {
            // Move offchk to next matching position
            while (offchk < getp && *offchk != *getp) {
                ++offchk;
            }

            // If the checking pointer has reached current pos, break
            if (offchk >= getp) {
                break;
            }

            // find out how long the run of matches goes for
            //<= because it can consider the current pixel as part of a run
            int i;
            for (i = 1; &getp[i] < getend; ++i) {
                if (offchk[i] != getp[i]) {
                    break;
                }
            }

            if (i >= block_size) {
                block_size = i;
                offsetp = offchk;
            }

            ++offchk;
        }

        // decide what encoding to use for current run
        if (block_size <= 2) {
            // short copy 0b10??????
            // check we have an existing 1 byte command and if its value is still
            // small enough to handle additional bytes
            // start a new command if current one doesn't have space or we don't
            // have one to continue
            if (cmd_one && *cmd_onep < 0xBF) {
                // increment command value
                ++*cmd_onep;
                // captainslog_debug("0b10?????? Source Pos %ld, Dest Pos %ld, Count %d", getp - getstart, cmd_onep - putstart,
                // *cmd_onep - 0x80);
                *putp++ = *getp++;
            } else {
                cmd_onep = putp;
                // captainslog_debug("0b10?????? Source Pos %ld, Dest Pos %ld, Count %d", getp - getstart, cmd_onep - putstart, 1);
                *putp++ = 0x81;
                *putp++ = *getp++;
                cmd_one = true;
            }
        } else {
            uint16_t offset;
            uint16_t rel_offset = getp - offsetp;
            if (block_size > 0xA || (rel_offset > 0xFFF)) {
                // write 5 byte command 0b11111111
                if (block_size > 0x40) {
                    // captainslog_debug("0b11111111 Source Pos %ld, Dest Pos %ld, Count %d, ", getp - getstart, putp - putstart,
                    // block_size);
                    *putp++ = 0xFF;
                    *putp++ = block_size;
                    *putp++ = block_size >> 8;
                    // write 3 byte command 0b11??????
                } else {
                    // captainslog_debug("0b11?????? Source Pos %ld, Dest Pos %ld, Count %d, ", getp - getstart, putp - putstart,
                    // block_size);
                    *putp++ = (block_size - 3) | 0xC0;
                }

                offset = relative ? rel_offset : offsetp - getstart;
                // captainslog_debug("Offset %d", offset);
                // write 2 byte command? 0b0???????
            } else {
                // captainslog_debug("0b0??????? Source Pos %ld, Dest Pos %ld, Count %d, ", getp - getstart, putp - putstart,
                // block_size);  uint8_t cmd_two_val = 16 * (block_size - 3) + (rel_offset >> 8);  captainslog_debug("Offset %d",
                // rel_offset);
                offset = rel_offset << 8 | (16 * (block_size - 3) + (rel_offset >> 8));
            }
            *putp++ = offset;
            *putp++ = offset >> 8;
            getp += block_size;
            cmd_one = false;
        }
    }

    // write final 0x80, this is why its also known as format80 compression
    *putp++ = 0x80;
    return putp - putstart;
}
