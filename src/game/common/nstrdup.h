/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief strdup using operator new[] rather than malloc.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef NSTRDUP_H
#define NSTRDUP_H

#include "always.h"
#include <cstring>

inline char *nstrdup(const char *str)
{
    char *nstr = nullptr;

    if (str != nullptr) {
        nstr = new char[std::strlen(str) + 1];

        if (nstr != nullptr) {
            std::strcpy(nstr, str);
        }
    }

    return nstr;
}

#endif // NSTRDUP_H
