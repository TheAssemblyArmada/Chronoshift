/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Pipe interface for writing LZO encoded data.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "lzopipe.h"
#include "endiantype.h"
#include "gamedebug.h"
#include "minmax.h"

#ifdef CHRONOSHIFT_USE_MINILZO
#include <minilzo/minilzo.h>
#else
#include <lzo/lzo1x.h>
#endif

LZOPipe::LZOPipe(PipeControl mode, int size) :
    m_mode(mode),
    m_dataInBuffer(0),
    m_inBuffer(nullptr),
    m_outBuffer(nullptr),
    m_blockSize(size),
    m_maxBlockSize(size),
    m_compressedBytes(-1),
    m_uncompressedBytes(0)
{
    m_inBuffer = new uint8_t[m_maxBlockSize + m_blockSize];
    m_outBuffer = new uint8_t[m_maxBlockSize + m_blockSize];
}

LZOPipe::~LZOPipe()
{
    delete[] m_inBuffer;
    m_inBuffer = nullptr;

    delete[] m_outBuffer;
    m_outBuffer = nullptr;
}

/**
 * @brief Writes data from the buffer to the next pipe in the chain, processing it with LZO.
 */
int LZOPipe::Put(void const *source, int length)
{
    int putbytes = 0;

    if (source == nullptr || length < 1) {
        return Pipe::Put(source, length);
    }

    DEBUG_ASSERT(m_inBuffer != nullptr);

    uint8_t const *src = static_cast<uint8_t const *>(source);

    if (m_mode == PIPE_UNCOMPRESS) {
        while (length > 0) {
            if (m_compressedBytes == -1) {
                int datasize = Min(length, 4 - m_dataInBuffer);

                memcpy(m_inBuffer + m_dataInBuffer, src, datasize);
                src += datasize;
                m_dataInBuffer += datasize;
                length -= datasize;

                // On disk format is little endian so need to convert.
                if (m_dataInBuffer == 4) {
                    int16_t tmp[2];
                    memcpy(tmp, m_inBuffer, sizeof(tmp));
                    m_compressedBytes = le16toh(tmp[0]);
                    m_uncompressedBytes = le16toh(tmp[1]);
                    m_dataInBuffer = 0;
                }
            }

            if (length <= 0) {
                break;
            }

            int bytesloaded = Min(length, m_compressedBytes - m_dataInBuffer);

            memcpy(m_inBuffer + m_dataInBuffer, src, bytesloaded);
            m_dataInBuffer += bytesloaded;
            length -= bytesloaded;
            src += bytesloaded;

            // If we have a complete compressed chunk, decompress it.
            if (m_dataInBuffer == m_compressedBytes) {
                lzo_uint outsize = 4;
                lzo1x_decompress(m_inBuffer, m_compressedBytes, m_outBuffer, &outsize, nullptr);
                putbytes += Pipe::Put(m_outBuffer, m_uncompressedBytes);
                m_dataInBuffer = 0;
                m_compressedBytes = -1;
            }
        }
    } else {
        // Check if we already have some buffered data that wasn't enough for a
        // whole compressed chunk.
        if (m_dataInBuffer > 0) {
            int datasize = Min(length, m_blockSize - m_dataInBuffer);

            memcpy(m_inBuffer + m_dataInBuffer, src, datasize);
            src += datasize;
            m_dataInBuffer += datasize;
            length -= datasize;

            // When we have enough data buffered for a complete chunk, proceed
            // to compress.
            if (m_dataInBuffer == m_blockSize) {
                lzo_uint outsize = 4;
                uint8_t *workmem = new uint8_t[0x10000];
                lzo1x_1_compress(m_inBuffer, m_dataInBuffer, m_outBuffer, &outsize, workmem);
                delete[] workmem;
                m_uncompressedBytes = m_blockSize;
                m_compressedBytes = outsize;
                
                // On disk format is little endian so need to convert.
                int16_t tmp[2];
                tmp[0] = htole16(m_compressedBytes);
                tmp[1] = htole16(m_uncompressedBytes);
                putbytes += Pipe::Put(tmp, sizeof(tmp));
                putbytes += Pipe::Put(m_outBuffer, outsize);
                m_dataInBuffer = 0;
            }
        }

        // While we have more data than our block size, loop and keep
        // compressing in chunks.
        while (length >= m_blockSize) {
            lzo_uint outsize = 4;
            uint8_t *workmem = new uint8_t[0x10000];
            lzo1x_1_compress(src, m_blockSize, m_outBuffer, &outsize, workmem);
            delete[] workmem;
            m_uncompressedBytes = m_blockSize;
            m_compressedBytes = outsize;
            src += m_blockSize;
            length -= m_blockSize;
            
            // On disk format is little endian so need to convert.
            int16_t tmp[2];
            tmp[0] = htole16(m_compressedBytes);
            tmp[1] = htole16(m_uncompressedBytes);
            putbytes += Pipe::Put(tmp, sizeof(tmp));
            putbytes += Pipe::Put(m_outBuffer, outsize);
        }

        // If we still have data left but not enough for a block, buffer it for
        // the next call or flush.
        if (length > 0) {
            memcpy(m_inBuffer, src, length);
            m_dataInBuffer = length;
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

    if (m_dataInBuffer > 0) {
        if (m_mode == PIPE_UNCOMPRESS) {
            if (m_compressedBytes == -1) {
                putbytes = Pipe::Put(m_inBuffer, m_dataInBuffer);
                m_dataInBuffer = 0;
            }

            if (m_dataInBuffer > 0) {
                // On disk format is little endian so need to convert.
                int16_t tmp[2];
                tmp[0] = htole16(m_compressedBytes);
                tmp[1] = htole16(m_uncompressedBytes);
                putbytes += Pipe::Put(tmp, sizeof(tmp));
                putbytes += Pipe::Put(m_inBuffer, m_dataInBuffer);
                m_dataInBuffer = 0;
                m_compressedBytes = -1;
            }

        } else {
            lzo_uint outsize = 4;
            uint8_t *workmem = new uint8_t[0x10000];
            lzo1x_1_compress(m_inBuffer, m_dataInBuffer, m_outBuffer, &outsize, workmem);
            delete[] workmem;

            m_compressedBytes = outsize;
            m_uncompressedBytes = m_dataInBuffer;
            
            // On disk format is little endian so need to convert.
            int16_t tmp[2];
            tmp[0] = htole16(m_compressedBytes);
            tmp[1] = htole16(m_uncompressedBytes);
            putbytes += Pipe::Put(tmp, sizeof(tmp));
            putbytes += Pipe::Put(m_outBuffer, outsize);
            m_dataInBuffer = 0;
        }
    }

    return putbytes + Pipe::Flush();
}
