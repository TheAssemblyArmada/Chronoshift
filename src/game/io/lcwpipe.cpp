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
#include "lcw.h"
#include <cstring>
#include <algorithm>
#include <captainslog.h>

using std::memcpy;
using std::memmove;

LCWPipe::LCWPipe(PipeControl mode, int size) :
    m_Mode(mode),
    m_DataInBuffer(0),
    m_InBuffer(nullptr),
    m_OutBuffer(nullptr),
    m_BlockSize(size),
    m_MaxBlockSize(LCW_Worst_Case(size))
{
    m_InBuffer = new uint8_t[m_MaxBlockSize + m_BlockSize];
    m_OutBuffer = new uint8_t[m_MaxBlockSize + m_BlockSize];

    m_CompressedBytes = -1;
}

LCWPipe::~LCWPipe()
{
    delete[] m_InBuffer;
    m_InBuffer = nullptr;

    delete[] m_OutBuffer;
    m_OutBuffer = nullptr;
}

/**
 * @brief Writes data from the buffer to the next pipe in the chain, processing it with LCW.
 */
int LCWPipe::Put(const void *buffer, int length)
{
    captainslog_assert(buffer != nullptr);
    captainslog_assert(length > 0);

    const uint8_t *src = static_cast<const uint8_t *>(buffer);
    int putbytes = 0;

    if (!src || length < 1) {
        return Pipe::Put(buffer, length);
    }

    if (m_Mode == PIPE_UNCOMPRESS) {
        while (length > 0) {
            if (m_CompressedBytes == -1) {
                int datasize = std::min(length, 4 - m_DataInBuffer);

                memcpy(m_InBuffer + m_DataInBuffer, src, datasize);
                src += datasize;
                m_DataInBuffer += datasize;
                length -= datasize;

                if (m_DataInBuffer == 4) {
                    int16_t tmp;
                    memcpy(&tmp, m_InBuffer, sizeof(tmp));
                    m_CompressedBytes = le16toh(tmp);
                    memcpy(&tmp, m_InBuffer + sizeof(tmp), sizeof(tmp));
                    m_UncompressedBytes = le16toh(tmp);
                    m_DataInBuffer = 0;
                }
            }

            if (length <= 0) {
                break;
            }

            int bytesloaded = std::min(length, m_CompressedBytes - m_DataInBuffer);

            memmove(m_InBuffer + m_DataInBuffer, src, bytesloaded);
            m_DataInBuffer += bytesloaded;
            length -= bytesloaded;
            src += bytesloaded;

            if (m_DataInBuffer == m_CompressedBytes) {
                LCW_Uncomp(m_InBuffer, m_OutBuffer, 0);
                putbytes += Pipe::Put(m_OutBuffer, m_UncompressedBytes);
                m_DataInBuffer = 0;
                m_CompressedBytes = -1;
            }
        }
    } else {
        if (m_DataInBuffer > 0) {
            int datasize = std::min(length, m_BlockSize - m_DataInBuffer);

            memmove(m_InBuffer + m_DataInBuffer, src, datasize);
            src += datasize;
            m_DataInBuffer += datasize;
            length -= datasize;

            if (m_DataInBuffer == m_BlockSize) {
                int bytescompressed = LCW_Comp(m_InBuffer, m_OutBuffer, m_BlockSize);
                m_CompressedBytes = bytescompressed;
                m_UncompressedBytes = m_BlockSize;
                putbytes = Pipe::Put(&m_CompressedBytes, 2);
                putbytes += Pipe::Put(&m_UncompressedBytes, 2);
                putbytes += Pipe::Put(m_OutBuffer, bytescompressed);
                m_DataInBuffer = 0;
            }
        }

        while (length >= m_BlockSize) {
            int outbytes = LCW_Comp(src, m_OutBuffer, m_BlockSize);
            m_UncompressedBytes = m_BlockSize;
            src += m_BlockSize;
            length -= m_BlockSize;
            m_CompressedBytes = outbytes;

            //
            // On disk format is little endian so need to convert.
            //
            int16_t tmp = htole16(m_CompressedBytes);
            putbytes += Pipe::Put(&tmp, sizeof(tmp));
            tmp = htole16(m_UncompressedBytes);
            putbytes += Pipe::Put(&tmp, sizeof(tmp));

            putbytes += Pipe::Put(m_OutBuffer, outbytes);
        }

        if (length > 0) {
            memmove(m_InBuffer, src, length);
            m_DataInBuffer = length;
        }
    }

    return putbytes;
}

/**
 * @brief Flushes any remaining data that is waiting to be compressed.
 */
int LCWPipe::Flush()
{
    captainslog_assert(m_InBuffer != nullptr);

    int putbytes = 0;

    if (m_DataInBuffer > 0) {
        if (m_Mode == PIPE_UNCOMPRESS) {
            if (m_CompressedBytes == -1) {
                putbytes += Pipe::Put(m_InBuffer, m_DataInBuffer);

                m_DataInBuffer = 0;
            }

            if (m_DataInBuffer > 0) {
                // On disk format is little endian so need to convert.
                int16_t tmp = htole16(m_CompressedBytes);
                putbytes += Pipe::Put(&tmp, sizeof(tmp));
                tmp = htole16(m_UncompressedBytes);
                putbytes += Pipe::Put(&tmp, sizeof(tmp));

                putbytes += Pipe::Put(m_InBuffer, m_DataInBuffer);

                m_DataInBuffer = 0;
                m_CompressedBytes = -1;
            }
        } else {
            int outsize = LCW_Comp(m_InBuffer, m_OutBuffer, m_DataInBuffer);

            m_UncompressedBytes = m_DataInBuffer;
            m_CompressedBytes = outsize;

            // On disk format is little endian so need to convert.
            int16_t tmp = htole16(m_CompressedBytes);
            putbytes += Pipe::Put(&tmp, sizeof(tmp));
            tmp = htole16(m_UncompressedBytes);
            putbytes += Pipe::Put(&tmp, sizeof(tmp));

            putbytes += Pipe::Put(m_OutBuffer, outsize);

            m_DataInBuffer = 0;
        }
    }

    return putbytes + Pipe::Flush();
}
