/**
* @file
*
* @Author CCHyper, OmniBlade
*
* @brief  Function for reversing bytes in memory.
*
* @copyright Redalert++ is free software: you can redistribute it and/or
*            modify it under the terms of the GNU General Public License
*            as published by the Free Software Foundation, either version
*            2 of the License, or (at your option) any later version.
*
*            A full copy of the GNU General Public License can be found in
*            LICENSE
*/
#pragma once

#ifndef MEMREV_H
#define MEMREV_H

#include	"always.h"

inline void *memrev(void *dest, void *src, size_t length)
{
    char *d = (char*)dest;
    char *s = (char*)src;

    if (length > 0) {

        char *src_sz = s + length;
        char *dest_sz = d + length;

        while (src_sz > src) {
            char t = *--src_sz;
            *--dest_sz = *s++;
            *d++ = t;
        }
    }

    return dest;
}

#endif //MEMREV_H
