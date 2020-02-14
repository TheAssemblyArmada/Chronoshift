/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Straw interface for reading Base64 encoded data.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "b64straw.h"
#include "base64.h"
#include <cstring>
#include <captainslog.h>

using std::memcpy;
using std::memmove;

int Base64Straw::Get(void *dest, int slen)
{
    char *from = nullptr;
    char *destbuffp = nullptr;
    int outcount = 0;
    int fromsize = 0;
    int total = 0;

    captainslog_assert(dest != nullptr);
    captainslog_assert(slen > 0);

    switch (m_Mode) {
        case STRAW_ENCODE:
            from = m_CBuffer;
            fromsize = sizeof(m_PBuffer);
            destbuffp = m_CBuffer;
            outcount = sizeof(m_CBuffer);
            break;

        case STRAW_DECODE:
            from = m_PBuffer;
            fromsize = sizeof(m_CBuffer);
            destbuffp = m_PBuffer;
            outcount = sizeof(m_PBuffer);
            break;
        default:
            break;
    }

    while (slen > 0) {
        if (m_Counter > 0) {
            int tocopy = slen;

            if (slen >= m_Counter) {
                tocopy = m_Counter;
            }

            memmove(dest, &destbuffp[outcount - m_Counter], tocopy);
            m_Counter -= tocopy;
            dest = static_cast<char *>(dest) + tocopy;
            slen -= tocopy;
            total += tocopy;
        }

        if (slen <= 0) {
            break;
        }

        int slen2 = Straw::Get(from, fromsize);

        m_Counter = m_Mode == STRAW_DECODE ? Base64_Decode(from, slen2, destbuffp, outcount) :
                                            Base64_Encode(from, slen2, destbuffp, outcount);

        if (m_Counter <= 0) {
            break;
        }
    }

    return total;
}
