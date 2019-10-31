/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Pipe interface for writing Base64 encoded data.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "always.h"
#include "b64pipe.h"
#include "base64.h"
#include "gamedebug.h"
#include <cstring>
#include <algorithm>

using std::memcpy;
using std::memmove;

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

        switch (m_Mode) {
            case PIPE_ENCODE:
                incount = sizeof(m_PBuffer);
                srcbuffp = m_PBuffer;
                destbuffp = m_CBuffer;
                outcount = sizeof(m_CBuffer);
                break;

            case PIPE_DECODE:
                incount = sizeof(m_CBuffer);
                srcbuffp = m_CBuffer;
                destbuffp = m_PBuffer;
                outcount = sizeof(m_PBuffer);
                break;

            default:
                break;
        }

        if (m_Counter > 0) {
            int cpylen = std::min(slen, incount - m_Counter);
            memcpy(srcbuffp + m_Counter, source, cpylen);
            m_Counter += cpylen;
            slen -= cpylen;

            if (m_Counter == incount) {
                if (m_Mode == PIPE_DECODE) {
                    b64len = Base64_Decode(srcbuffp, incount, destbuffp, outcount);
                } else {
                    b64len = Base64_Encode(srcbuffp, incount, destbuffp, outcount);
                }

                source = static_cast<const char *>(source) + cpylen;
                result += Pipe::Put(destbuffp, b64len);
                m_Counter = 0;
            }
        }

        for (; slen >= incount; slen -= incount) {
            b64len = m_Mode == PIPE_DECODE ? Base64_Decode(source, incount, destbuffp, outcount) :
                                             Base64_Encode(source, incount, destbuffp, outcount);

            source = static_cast<const char *>(source) + incount;
            result += Pipe::Put(destbuffp, b64len);
        }

        if (slen > 0) {
            memmove(srcbuffp, source, slen);
            m_Counter = slen;
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

    if (m_Counter > 0) {
        if (m_Mode == PIPE_DECODE) {
            buff = m_PBuffer;
            b64len = Base64_Decode(m_CBuffer, m_Counter, m_PBuffer, sizeof(m_PBuffer));
        } else {
            buff = m_CBuffer;
            b64len = Base64_Encode(m_PBuffer, m_Counter, m_CBuffer, sizeof(m_CBuffer));
        }

        len = Pipe::Put(buff, b64len);
        m_Counter = 0;
    }

    return len + Pipe::Flush();
}
