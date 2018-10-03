/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Implementation of LCW, a custom compression format used in many Westwood games.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef LCW_H
#define LCW_H

#include "always.h"

/**
 * @brief Get the worst case for LCW compression given the starting size.
 */
inline int LCW_Worst_Case(int bytes)
{
    return bytes + (bytes / 128) + 1;
}

int __cdecl LCW_Uncomp(const void *src, void *dst, unsigned int bytes = 0);
int __cdecl LCW_Comp(const void *src, void *dst, unsigned int bytes = 0);

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"

namespace Lcw {
inline void Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x005D6880, LCW_Uncomp);
    Hook_Function(0x005DD28C, LCW_Comp);
#endif
}
}
#endif

#endif // LCW_H
