/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Includes for defined width integers and aliasing versions for type punning.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef BITTYPE_H
#define BITTYPE_H

#include "compiler.h"
#include <inttypes.h>
#include <limits.h>

// Use these when using pointers to do type punning.
#if __has_attribute(__may_alias__)
    typedef float __attribute__((__may_alias__)) float_a;
    typedef int32_t __attribute__((__may_alias__)) int32_a;
    typedef uint32_t __attribute__((__may_alias__)) uint32_a;
    typedef double __attribute__((__may_alias__)) double_a;
    typedef int64_t __attribute__((__may_alias__)) int64_a;
    typedef uint64_t __attribute__((__may_alias__)) uint64_a;
    typedef wchar_t __attribute__((__may_alias__)) wchar_a;
    typedef int BOOL; // Temp.
#else   
    // MSVC doesn't currently enforce strict aliasing.
    typedef float float_a;
    typedef int32_t int32_a;
    typedef uint32_t uint32_a;
    typedef double double_a;
    typedef int64_t int64_a;
    typedef uint64_t uint64_a;
    typedef wchar_t wchar_a;
#endif
    
#endif // BITTYPE_H
