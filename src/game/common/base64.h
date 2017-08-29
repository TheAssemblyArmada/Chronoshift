/**
 * @file
 *
 * @Author CCHyper, OmniBlade
 *
 * @brief Functions for base64 encoding and decoding.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#ifndef BASE64_H
#define BASE64_H

#include "always.h"

#define BASE64_OK 0
#define BASE64_ERROR -1
#define BASE64_BUFFER_OVERFLOW -2

/**
 * @brief Get the worst case size of Base64 encoded data given an initial size in bytes.
 */
inline int Base64_Enc_Size(int bytes)
{
    return ((bytes + 2) / 3) * 4;
}

/**
 * @brief Get number of bytes in the decoded stream given the encoded size.
 */
inline int Base64_Dec_Size(int bytes)
{
    int len = (bytes / 4) * 3;

    if ((bytes % 4) == 3) {
        len += 2;
    }

    if ((bytes % 4) == 2) {
        len += 1;
    }

    return len;
}

int Base64_Encode(void const *src, int slen, void *dst, int dlen);
int Base64_Decode(void const *src, int slen, void *dst, int dlen);

#endif // BASE64_H
