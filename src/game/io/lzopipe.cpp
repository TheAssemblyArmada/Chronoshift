/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Pipe interface for writing LZO encoded data.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "lzopipe.h"
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

LZOPipe::LZOPipe(PipeControl mode, int size) :
    m_Mode(mode),
    m_DataInBuffer(0),
    m_InBuffer(nullptr),
    m_OutBuffer(nullptr),
    m_BlockSize(size),
    m_MaxBlockSize(size),
    m_CompressedBytes(-1),
    m_UncompressedBytes(0)
{
    m_InBuffer = new uint8_t[m_MaxBlockSize + m_BlockSize];
    m_OutBuffer = new uint8_t[m_MaxBlockSize + m_BlockSize];
}

LZOPipe::~LZOPipe()
{
    delete[] m_InBuffer;
    m_InBuffer = nullptr;

    delete[] m_OutBuffer;
    m_OutBuffer = nullptr;
}

/**
 * @brief Writes data from the buffer to the next pipe in the chain, processing it with LZO.
 */
int LZOPipe::Put(const void *source, int length)
{
    int putbytes = 0;

    if (source == nullptr || length < 1) {
        return Pipe::Put(source, length);
    }

    DEBUG_ASSERT(m_InBuffer != nullptr);

    uint8_t const *src = static_cast<uint8_t const *>(source);

    if (m_Mode == PIPE_UNCOMPRESS) {
        while (length > 0) {
            if (m_CompressedBytes == -1) {
                int datasize = std::min(length, 4 - m_DataInBuffer);

                memcpy(m_InBuffer + m_DataInBuffer, src, datasize);
                src += datasize;
                m_DataInBuffer += datasize;
                length -= datasize;

                // On disk format is little endian so need to convert.
                if (m_DataInBuffer == 4) {
                    int16_t tmp[2];
                    memcpy(tmp, m_InBuffer, sizeof(tmp));
                    m_CompressedBytes = le16toh(tmp[0]);
                    m_UncompressedBytes = le16toh(tmp[1]);
                    m_DataInBuffer = 0;
                }
            }

            if (length <= 0) {
                break;
            }

            int bytesloaded = std::min(length, m_CompressedBytes - m_DataInBuffer);

            memcpy(m_InBuffer + m_DataInBuffer, src, bytesloaded);
            m_DataInBuffer += bytesloaded;
            length -= bytesloaded;
            src += bytesloaded;

            // If we have a complete compressed chunk, decompress it.
            if (m_DataInBuffer == m_CompressedBytes) {
                lzo_uint outsize = 4;
                lzo1x_decompress(m_InBuffer, m_CompressedBytes, m_OutBuffer, &outsize, nullptr);
                putbytes += Pipe::Put(m_OutBuffer, m_UncompressedBytes);
                m_DataInBuffer = 0;
                m_CompressedBytes = -1;
            }
        }
    } else {
        // Check if we already have some buffered data that wasn't enough for a
        // whole compressed chunk.
        if (m_DataInBuffer > 0) {
            int datasize = std::min(length, m_BlockSize - m_DataInBuffer);

            memcpy(m_InBuffer + m_DataInBuffer, src, datasize);
            src += datasize;
            m_DataInBuffer += datasize;
            length -= datasize;

            // When we have enough data buffered for a complete chunk, proceed
            // to compress.
            if (m_DataInBuffer == m_BlockSize) {
                lzo_uint outsize = 4;
                uint8_t *workmem = new uint8_t[0x10000];
                lzo1x_1_compress(m_InBuffer, m_DataInBuffer, m_OutBuffer, &outsize, workmem);
                delete[] workmem;
                m_UncompressedBytes = m_BlockSize;
                m_CompressedBytes = outsize;
                
                // On disk format is little endian so need to convert.
                int16_t tmp[2];
                tmp[0] = htole16(m_CompressedBytes);
                tmp[1] = htole16(m_UncompressedBytes);
                putbytes += Pipe::Put(tmp, sizeof(tmp));
                putbytes += Pipe::Put(m_OutBuffer, outsize);
                m_DataInBuffer = 0;
            }
        }

        // While we have more data than our block size, loop and keep
        // compressing in chunks.
        while (length >= m_BlockSize) {
            lzo_uint outsize = 4;
            uint8_t *workmem = new uint8_t[0x10000];
            lzo1x_1_compress(src, m_BlockSize, m_OutBuffer, &outsize, workmem);
            delete[] workmem;
            m_UncompressedBytes = m_BlockSize;
            m_CompressedBytes = outsize;
            src += m_BlockSize;
            length -= m_BlockSize;
            
            // On disk format is little endian so need to convert.
            int16_t tmp[2];
            tmp[0] = htole16(m_CompressedBytes);
            tmp[1] = htole16(m_UncompressedBytes);
            putbytes += Pipe::Put(tmp, sizeof(tmp));
            putbytes += Pipe::Put(m_OutBuffer, outsize);
        }

        // If we still have data left but not enough for a block, buffer it for
        // the next call or flush.
        if (length > 0) {
            memcpy(m_InBuffer, src, length);
            m_DataInBuffer = length;
        }
    }

    return putbytes;
}

/**
 * @brief Flushes any remaining data that is waiting to be compressed.
 */
int LZOPipe::Flush()
{
    int putbytes = 0;

    if (m_DataInBuffer > 0) {
        if (m_Mode == PIPE_UNCOMPRESS) {
            if (m_CompressedBytes == -1) {
                putbytes = Pipe::Put(m_InBuffer, m_DataInBuffer);
                m_DataInBuffer = 0;
            }

            if (m_DataInBuffer > 0) {
                // On disk format is little endian so need to convert.
                int16_t tmp[2];
                tmp[0] = htole16(m_CompressedBytes);
                tmp[1] = htole16(m_UncompressedBytes);
                putbytes += Pipe::Put(tmp, sizeof(tmp));
                putbytes += Pipe::Put(m_InBuffer, m_DataInBuffer);
                m_DataInBuffer = 0;
                m_CompressedBytes = -1;
            }

        } else {
            lzo_uint outsize = 4;
            uint8_t *workmem = new uint8_t[0x10000];
            lzo1x_1_compress(m_InBuffer, m_DataInBuffer, m_OutBuffer, &outsize, workmem);
            delete[] workmem;

            m_CompressedBytes = outsize;
            m_UncompressedBytes = m_DataInBuffer;
            
            // On disk format is little endian so need to convert.
            int16_t tmp[2];
            tmp[0] = htole16(m_CompressedBytes);
            tmp[1] = htole16(m_UncompressedBytes);
            putbytes += Pipe::Put(tmp, sizeof(tmp));
            putbytes += Pipe::Put(m_OutBuffer, outsize);
            m_DataInBuffer = 0;
        }
    }

    return putbytes + Pipe::Flush();
}
