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
    m_Mode(mode),
    m_Carryover(0),
    m_InBuffer(nullptr),
    m_OutBuffer(nullptr),
    m_BlockSize(size),
    m_CompressedBlockSize(LCW_Worst_Case(size)),
    m_CompressedBytes(0),
    m_UncompressedBytes(0)
{
    m_InBuffer = new uint8_t[m_CompressedBlockSize + m_BlockSize];
    m_OutBuffer = new uint8_t[m_CompressedBlockSize + m_BlockSize];
}

LCWStraw::~LCWStraw()
{
    delete[] m_InBuffer;
    m_InBuffer = nullptr;

    delete[] m_OutBuffer;
    m_OutBuffer = nullptr;
}

/**
 * @brief Reads data to the buffer from the next straw in the chain, processing it with LCW.
 */
int LCWStraw::Get(void *buffer, int length)
{
    DEBUG_ASSERT_PRINT(m_InBuffer != nullptr, "m_InBuffer is a null pointer.\n");

    int bytesread = 0;
    uint8_t *dst = static_cast<uint8_t *>(buffer);

    if (!buffer || (length < 1)) {
        return 0;
    }

    while (length > 0) {
        if (m_Carryover > 0) {
            int datasize = std::min(m_Carryover, length);

            uint8_t *srcbuff = m_Mode == STRAW_UNCOMPRESS ? m_OutBuffer + m_UncompressedBytes - m_Carryover :
                                                            m_OutBuffer + m_CompressedBytes - m_Carryover + 4;

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
            int16_t tmp;

            if (Straw::Get(&tmp, sizeof(tmp)) != sizeof(tmp)) {
                break;
            }

            m_CompressedBytes = le16toh(tmp);

            if (Straw::Get(&tmp, sizeof(tmp)) != sizeof(tmp)) {
                break;
            }

            m_UncompressedBytes = le16toh(tmp);

            if (Straw::Get(m_InBuffer, m_CompressedBytes) != m_CompressedBytes) {
                break;
            }

            LCW_Uncomp(m_InBuffer, m_OutBuffer, m_UncompressedBytes);
            m_Carryover = m_UncompressedBytes;
        } else {
            m_UncompressedBytes = Straw::Get(m_InBuffer, m_BlockSize);

            if (!m_UncompressedBytes) {
                break;
            }

            // Compress to m_outBuffer, offset by 4 to make room for block header
            m_CompressedBytes = LCW_Comp(m_InBuffer, m_OutBuffer + 4, m_UncompressedBytes);

            // Create block header. On disk format is little endian so need to
            // convert.
            int16_t tmp = htole16(m_CompressedBytes);
            memcpy(m_OutBuffer, &tmp, sizeof(tmp));
            tmp = htole16(m_UncompressedBytes);
            memcpy(m_OutBuffer, &tmp, sizeof(tmp));

            m_Carryover = m_CompressedBytes + 4;
        }
    }

    return bytesread;
}
