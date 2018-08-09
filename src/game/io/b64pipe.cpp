/**
 * @file
 *
 * @Author CCHyper, OmniBlade
 *
 * @brief Pipe interface for writing Base64 encoded data.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "always.h"
#include "b64pipe.h"
#include "base64.h"
#include "gamedebug.h"
#include "minmax.h"

int Base64Pipe::Put(const void *source, int slen)
{
    char *srcbuffp = nullptr;
    char *destbuffp = nullptr;
    int outcount = 0;
    int incount = 0;
    int b64len = 0;

    DEBUG_ASSERT(source != nullptr);
    DEBUG_ASSERT(slen > 0);

    if (source && slen > 0) {
        int result = 0;

        switch (m_mode) {
            case PIPE_ENCODE:
                incount = sizeof(m_pBuffer);
                srcbuffp = m_pBuffer;
                destbuffp = m_cBuffer;
                outcount = sizeof(m_cBuffer);
                break;

            case PIPE_DECODE:
                incount = sizeof(m_cBuffer);
                srcbuffp = m_cBuffer;
                destbuffp = m_pBuffer;
                outcount = sizeof(m_pBuffer);
                break;

            default:
                break;
        }

        if (m_counter > 0) {
            int cpylen = Min(slen, incount - m_counter);
            memcpy(srcbuffp + m_counter, source, cpylen);
            m_counter += cpylen;
            slen -= cpylen;

            if (m_counter == incount) {
                if (m_mode == PIPE_DECODE) {
                    b64len = Base64_Decode(srcbuffp, incount, destbuffp, outcount);
                } else {
                    b64len = Base64_Encode(srcbuffp, incount, destbuffp, outcount);
                }

                source = static_cast<char const *>(source) + cpylen;
                result += Pipe::Put(destbuffp, b64len);
                m_counter = 0;
            }
        }

        for (; slen >= incount; slen -= incount) {
            b64len = m_mode == PIPE_DECODE ? Base64_Decode(source, incount, destbuffp, outcount) :
                                             Base64_Encode(source, incount, destbuffp, outcount);

            source = static_cast<char const *>(source) + incount;
            result += Pipe::Put(destbuffp, b64len);
        }

        if (slen > 0) {
            memmove(srcbuffp, source, slen);
            m_counter = slen;
        }

        return result;
    }

    return Pipe::Put(source, slen);
}

int Base64Pipe::Flush()
{
    char *buff;
    int len = 0;
    int b64len = 0;

    if (m_counter > 0) {
        if (m_mode == PIPE_DECODE) {
            buff = m_pBuffer;
            b64len = Base64_Decode(m_cBuffer, m_counter, m_pBuffer, sizeof(m_pBuffer));
        } else {
            buff = m_cBuffer;
            b64len = Base64_Encode(m_pBuffer, m_counter, m_cBuffer, sizeof(m_cBuffer));
        }

        len = Pipe::Put(buff, b64len);
        m_counter = 0;
    }

    return len + Pipe::Flush();
}
