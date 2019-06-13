/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Straw interface for reading LZO encoded data.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "lzostraw.h"
#include "endiantype.h"
#include "gamedebug.h"
#include <cstring>
#include <algorithm>

using std::memcpy;

#ifdef CHRONOSHIFT_USE_MINILZO
#include <minilzo/minilzo.h>
#else
#include <lzo/lzo1x.h>
#endif

LZOStraw::LZOStraw(StrawControl mode, int size) :
    m_mode(mode),
    m_carryover(0),
    m_inBuffer(nullptr),
    m_outBuffer(nullptr),
    m_blockSize(size),
    m_compressedBlockSize(size),
    m_compressedBytes(0),
    m_uncompressedBytes(0)
{
    m_inBuffer = new uint8_t[m_blockSize + m_compressedBlockSize];
    m_outBuffer = new uint8_t[m_blockSize + m_compressedBlockSize];
}

LZOStraw::~LZOStraw()
{
    delete[] m_inBuffer;
    m_inBuffer = nullptr;

    delete[] m_outBuffer;
    m_outBuffer = nullptr;
}

/**
 * @brief Reads data to the buffer from the next straw in the chain, processing it with LZO.
 */
int LZOStraw::Get(void *buffer, int length)
{
    DEBUG_ASSERT(buffer != nullptr);

    int bytesread = 0;
    uint8_t *dst = static_cast<uint8_t *>(buffer);

    if (!buffer || (length < 1)) {
        return 0;
    }

    while (length > 0) {
        if (m_carryover > 0) {
            int datasize = std::min(m_carryover, length);

            uint8_t *srcbuff =
                m_mode == STRAW_UNCOMPRESS ? m_inBuffer + m_uncompressedBytes - m_carryover : m_outBuffer + m_compressedBytes - m_carryover + 4;

            memcpy(dst, srcbuff, datasize);
            dst += datasize;
            length -= datasize;
            m_carryover -= datasize;
            bytesread += datasize;
        }

        if (!length) {
            break;
        }

        if (m_mode == STRAW_UNCOMPRESS) {
            // LCW stream format compresses in blocks, with the compressed and
            // uncompressed sizes of the blocks recorded just before the
            // compressed data itself.
            int16_t tmp[2];

            if (Straw::Get(tmp, sizeof(tmp)) != sizeof(tmp)) {
                break;
            }

            m_compressedBytes = le16toh(tmp[0]);
            m_uncompressedBytes = le16toh(tmp[1]);

            // TODO replace this workmem with m_outBuffer when original LZO calls replaced as original doesn't allocate
            // m_outBuffer in its ctor and does this temp allocation instead.
            uint8_t *workmem = new uint8_t[m_compressedBytes];

            if (Straw::Get(workmem, m_compressedBytes) != m_compressedBytes) {
                break;
            }

            lzo_uint outsize = 4;
            lzo1x_decompress(workmem, m_compressedBytes, m_inBuffer, &outsize, nullptr);
            delete[] workmem;

            m_carryover = m_uncompressedBytes = outsize;
        } else {
            m_uncompressedBytes = Straw::Get(m_inBuffer, m_blockSize);

            if (!m_uncompressedBytes) {
                break;
            }

            lzo_uint outsize = 0;
            uint8_t *workmem = new uint8_t[0x10000];
            lzo1x_1_compress(m_inBuffer, m_blockSize, m_outBuffer + 4, &outsize, workmem);
            delete[] workmem;

            // Create block header. On disk format is little endian so need to convert.
            int16_t tmp[2];
            tmp[0] = htole16(m_compressedBytes);
            tmp[1] = htole16(m_uncompressedBytes);
            memcpy(m_outBuffer, tmp, sizeof(tmp));
            m_carryover = m_compressedBytes + 4;
        }
    }

    return bytesread;
}
