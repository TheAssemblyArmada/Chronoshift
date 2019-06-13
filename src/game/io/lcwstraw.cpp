/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Straw interface for reading LCW encoded data.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "lcwstraw.h"
#include "endiantype.h"
#include "gamedebug.h"
#include "lcw.h"
#include <cstring>
#include <algorithm>

using std::memcpy;

LCWStraw::LCWStraw(StrawControl mode, int size) :
    m_mode(mode),
    m_carryOver(0),
    m_inBuffer(nullptr),
    m_outBuffer(nullptr),
    m_blockSize(size),
    m_compressedBlockSize(LCW_Worst_Case(size)),
    m_compressedBytes(0),
    m_uncompressedBytes(0)
{
    m_inBuffer = new uint8_t[m_compressedBlockSize + m_blockSize];
    m_outBuffer = new uint8_t[m_compressedBlockSize + m_blockSize];
}

LCWStraw::~LCWStraw()
{
    delete[] m_inBuffer;
    m_inBuffer = nullptr;

    delete[] m_outBuffer;
    m_outBuffer = nullptr;
}

/**
 * @brief Reads data to the buffer from the next straw in the chain, processing it with LCW.
 */
int LCWStraw::Get(void *buffer, int length)
{
    DEBUG_ASSERT_PRINT(m_inBuffer != nullptr, "m_inBuffer is a null pointer.\n");

    int bytesread = 0;
    uint8_t *dst = static_cast<uint8_t *>(buffer);

    if (!buffer || (length < 1)) {
        return 0;
    }

    while (length > 0) {
        if (m_carryOver > 0) {
            int datasize = std::min(m_carryOver, length);

            uint8_t *srcbuff = m_mode == STRAW_UNCOMPRESS ? m_outBuffer + m_uncompressedBytes - m_carryOver :
                                                            m_outBuffer + m_compressedBytes - m_carryOver + 4;

            memcpy(dst, srcbuff, datasize);
            dst += datasize;
            length -= datasize;
            m_carryOver -= datasize;
            bytesread += datasize;
        }

        if (!length) {
            break;
        }

        if (m_mode == STRAW_UNCOMPRESS) {
            // LCW stream format compresses in blocks, with the compressed and
            // uncompressed sizes of the blocks recorded just before the
            // compressed data itself.
            int16_t tmp;

            if (Straw::Get(&tmp, sizeof(tmp)) != sizeof(tmp)) {
                break;
            }

            m_compressedBytes = le16toh(tmp);

            if (Straw::Get(&tmp, sizeof(tmp)) != sizeof(tmp)) {
                break;
            }

            m_uncompressedBytes = le16toh(tmp);

            if (Straw::Get(m_inBuffer, m_compressedBytes) != m_compressedBytes) {
                break;
            }

            LCW_Uncomp(m_inBuffer, m_outBuffer, m_uncompressedBytes);
            m_carryOver = m_uncompressedBytes;
        } else {
            m_uncompressedBytes = Straw::Get(m_inBuffer, m_blockSize);

            if (!m_uncompressedBytes) {
                break;
            }

            // Compress to m_outBuffer, offset by 4 to make room for block header
            m_compressedBytes = LCW_Comp(m_inBuffer, m_outBuffer + 4, m_uncompressedBytes);

            // Create block header. On disk format is little endian so need to
            // convert.
            int16_t tmp = htole16(m_compressedBytes);
            memcpy(m_outBuffer, &tmp, sizeof(tmp));
            tmp = htole16(m_uncompressedBytes);
            memcpy(m_outBuffer, &tmp, sizeof(tmp));

            m_carryOver = m_compressedBytes + 4;
        }
    }

    return bytesread;
}
