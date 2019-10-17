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
#include "gamedebug.h"
#include <cstring>

using std::memcpy;
using std::memmove;

int Base64Straw::Get(void *dest, int slen)
{
    char *from = nullptr;
    char *destbuffp = nullptr;
    int outcount = 0;
    int fromsize = 0;
    int total = 0;

    DEBUG_ASSERT(dest != nullptr);
    DEBUG_ASSERT(slen > 0);

    switch (m_mode) {
        case STRAW_ENCODE:
            from = m_cBuffer;
            fromsize = sizeof(m_pBuffer);
            destbuffp = m_cBuffer;
            outcount = sizeof(m_cBuffer);
            break;

        case STRAW_DECODE:
            from = m_pBuffer;
            fromsize = sizeof(m_cBuffer);
            destbuffp = m_pBuffer;
            outcount = sizeof(m_pBuffer);
            break;
        default:
            break;
    }

    while (slen > 0) {
        if (m_counter > 0) {
            int tocopy = slen;

            if (slen >= m_counter) {
                tocopy = m_counter;
            }

            memmove(dest, &destbuffp[outcount - m_counter], tocopy);
            m_counter -= tocopy;
            dest = static_cast<char *>(dest) + tocopy;
            slen -= tocopy;
            total += tocopy;
        }

        if (slen <= 0) {
            break;
        }

        int slen2 = Straw::Get(from, fromsize);

        m_counter = m_mode == STRAW_DECODE ? Base64_Decode(from, slen2, destbuffp, outcount) :
                                            Base64_Encode(from, slen2, destbuffp, outcount);

        if (m_counter <= 0) {
            break;
        }
    }

    return total;
}
