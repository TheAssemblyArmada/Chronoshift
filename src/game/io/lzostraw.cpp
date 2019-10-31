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
    m_Mode(mode),
    m_Carryover(0),
    m_InBuffer(nullptr),
    m_OutBuffer(nullptr),
    m_BlockSize(size),
    m_CompressedBlockSize(size),
    m_CompressedBytes(0),
    m_UncompressedBytes(0)
{
    m_InBuffer = new uint8_t[m_BlockSize + m_CompressedBlockSize];
    m_OutBuffer = new uint8_t[m_BlockSize + m_CompressedBlockSize];
}

LZOStraw::~LZOStraw()
{
    delete[] m_InBuffer;
    m_InBuffer = nullptr;

    delete[] m_OutBuffer;
    m_OutBuffer = nullptr;
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
        if (m_Carryover > 0) {
            int datasize = std::min(m_Carryover, length);

            uint8_t *srcbuff =
                m_Mode == STRAW_UNCOMPRESS ? m_InBuffer + m_UncompressedBytes - m_Carryover : m_OutBuffer + m_CompressedBytes - m_Carryover + 4;

            memcpy(dst, srcbuff, datasize);
            dst += datasize;
            length -= datasize;
            m_Carryover -= datasize;
            bytesread += datasize;
        }

        if (!length) {
            break;
        }

        if (m_Mode == STRAW_UNCOMPRESS) {
            // LCW stream format compresses in blocks, with the compressed and
            // uncompressed sizes of the blocks recorded just before the
            // compressed data itself.
            int16_t tmp[2];

            if (Straw::Get(tmp, sizeof(tmp)) != sizeof(tmp)) {
                break;
            }

            m_CompressedBytes = le16toh(tmp[0]);
            m_UncompressedBytes = le16toh(tmp[1]);

            // TODO replace this workmem with m_outBuffer when original LZO calls replaced as original doesn't allocate
            // m_outBuffer in its ctor and does this temp allocation instead.
            uint8_t *workmem = new uint8_t[m_CompressedBytes];

            if (Straw::Get(workmem, m_CompressedBytes) != m_CompressedBytes) {
                break;
            }

            lzo_uint outsize = 4;
            lzo1x_decompress(workmem, m_CompressedBytes, m_InBuffer, &outsize, nullptr);
            delete[] workmem;

            m_Carryover = m_UncompressedBytes = outsize;
        } else {
            m_UncompressedBytes = Straw::Get(m_InBuffer, m_BlockSize);

            if (!m_UncompressedBytes) {
                break;
            }

            lzo_uint outsize = 0;
            uint8_t *workmem = new uint8_t[0x10000];
            lzo1x_1_compress(m_InBuffer, m_BlockSize, m_OutBuffer + 4, &outsize, workmem);
            delete[] workmem;

            // Create block header. On disk format is little endian so need to convert.
            int16_t tmp[2];
            tmp[0] = htole16(m_CompressedBytes);
            tmp[1] = htole16(m_UncompressedBytes);
            memcpy(m_OutBuffer, tmp, sizeof(tmp));
            m_Carryover = m_CompressedBytes + 4;
        }
    }

    return bytesread;
}
