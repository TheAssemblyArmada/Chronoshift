/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Pipe interface for writing LCW encoded data.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "lcwpipe.h"
#include "endiantype.h"
#include "gamedebug.h"
#include "lcw.h"
#include "minmax.h"

LCWPipe::LCWPipe(PipeControl mode, int size) :
    m_mode(mode),
    m_dataInBuffer(0),
    m_inBuffer(nullptr),
    m_outBuffer(nullptr),
    m_blockSize(size),
    m_maxBlockSize(LCW_Worst_Case(size))
{
    m_inBuffer = new uint8_t[m_maxBlockSize + m_blockSize];
    m_outBuffer = new uint8_t[m_maxBlockSize + m_blockSize];

    m_compressedBytes = -1;
}

LCWPipe::~LCWPipe()
{
    delete[] m_inBuffer;
    m_inBuffer = nullptr;

    delete[] m_outBuffer;
    m_outBuffer = nullptr;
}

/**
 * @brief Writes data from the buffer to the next pipe in the chain, processing it with LCW.
 */
int LCWPipe::Put(const void *buffer, int length)
{
    DEBUG_ASSERT(buffer != nullptr);
    DEBUG_ASSERT(length > 0);

    const uint8_t *src = static_cast<const uint8_t *>(buffer);
    int putbytes = 0;

    if (!src || length < 1) {
        return Pipe::Put(buffer, length);
    }

    if (m_mode == PIPE_UNCOMPRESS) {
        while (length > 0) {
            if (m_compressedBytes == -1) {
                int datasize = Min(length, 4 - m_dataInBuffer);

                memcpy(m_inBuffer + m_dataInBuffer, src, datasize);
                src += datasize;
                m_dataInBuffer += datasize;
                length -= datasize;

                if (m_dataInBuffer == 4) {
                    int16_t tmp;
                    memcpy(&tmp, m_inBuffer, sizeof(tmp));
                    m_compressedBytes = le16toh(tmp);
                    memcpy(&tmp, m_inBuffer + sizeof(tmp), sizeof(tmp));
                    m_uncompressedBytes = le16toh(tmp);
                    m_dataInBuffer = 0;
                }
            }

            if (length <= 0) {
                break;
            }

            int bytesloaded = Min(length, m_compressedBytes - m_dataInBuffer);

            memmove(m_inBuffer + m_dataInBuffer, src, bytesloaded);
            m_dataInBuffer += bytesloaded;
            length -= bytesloaded;
            src += bytesloaded;

            if (m_dataInBuffer == m_compressedBytes) {
                LCW_Uncomp(m_inBuffer, m_outBuffer, 0);
                putbytes += Pipe::Put(m_outBuffer, m_uncompressedBytes);
                m_dataInBuffer = 0;
                m_compressedBytes = -1;
            }
        }
    } else {
        if (m_dataInBuffer > 0) {
            int datasize = Min(length, m_blockSize - m_dataInBuffer);

            memmove(m_inBuffer + m_dataInBuffer, src, datasize);
            src += datasize;
            m_dataInBuffer += datasize;
            length -= datasize;

            if (m_dataInBuffer == m_blockSize) {
                int bytescompressed = LCW_Comp(m_inBuffer, m_outBuffer, m_blockSize);
                m_compressedBytes = bytescompressed;
                m_uncompressedBytes = m_blockSize;
                putbytes = Pipe::Put(&m_compressedBytes, 2);
                putbytes += Pipe::Put(&m_uncompressedBytes, 2);
                putbytes += Pipe::Put(m_outBuffer, bytescompressed);
                m_dataInBuffer = 0;
            }
        }

        while (length >= m_blockSize) {
            int outbytes = LCW_Comp(src, m_outBuffer, m_blockSize);
            m_uncompressedBytes = m_blockSize;
            src += m_blockSize;
            length -= m_blockSize;
            m_compressedBytes = outbytes;

            //
            // On disk format is little endian so need to convert.
            //
            int16_t tmp = htole16(m_compressedBytes);
            putbytes += Pipe::Put(&tmp, sizeof(tmp));
            tmp = htole16(m_uncompressedBytes);
            putbytes += Pipe::Put(&tmp, sizeof(tmp));

            putbytes += Pipe::Put(m_outBuffer, outbytes);
        }

        if (length > 0) {
            memmove(m_inBuffer, src, length);
            m_dataInBuffer = length;
        }
    }

    return putbytes;
}

/**
 * @brief Flushes any remaining data that is waiting to be compressed.
 */
int LCWPipe::Flush()
{
    DEBUG_ASSERT(m_inBuffer != nullptr);

    int putbytes = 0;

    if (m_dataInBuffer > 0) {
        if (m_mode == PIPE_UNCOMPRESS) {
            if (m_compressedBytes == -1) {
                putbytes += Pipe::Put(m_inBuffer, m_dataInBuffer);

                m_dataInBuffer = 0;
            }

            if (m_dataInBuffer > 0) {
                // On disk format is little endian so need to convert.
                int16_t tmp = htole16(m_compressedBytes);
                putbytes += Pipe::Put(&tmp, sizeof(tmp));
                tmp = htole16(m_uncompressedBytes);
                putbytes += Pipe::Put(&tmp, sizeof(tmp));

                putbytes += Pipe::Put(m_inBuffer, m_dataInBuffer);

                m_dataInBuffer = 0;
                m_compressedBytes = -1;
            }
        } else {
            int outsize = LCW_Comp(m_inBuffer, m_outBuffer, m_dataInBuffer);

            m_uncompressedBytes = m_dataInBuffer;
            m_compressedBytes = outsize;

            // On disk format is little endian so need to convert.
            int16_t tmp = htole16(m_compressedBytes);
            putbytes += Pipe::Put(&tmp, sizeof(tmp));
            tmp = htole16(m_uncompressedBytes);
            putbytes += Pipe::Put(&tmp, sizeof(tmp));

            putbytes += Pipe::Put(m_outBuffer, outsize);

            m_dataInBuffer = 0;
        }
    }

    return putbytes + Pipe::Flush();
}
