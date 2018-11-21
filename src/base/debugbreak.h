/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Implements __debugbreak intrinsic for compilers other than MSVC.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef DEBUGBREAK_H
#define DEBUGBREAK_H

#ifdef __cplusplus
extern "C" {
#endif

// If we have watcom, then use watcom inline assembly.
#ifdef __WATCOMC__
void __debugbreak(void);
#pragma aux __debugbreak = "int 3"

// If we have MSVC or MinGW then we have the intrinsic.
#elif !defined _WIN32

// If we dont have __has_builtin then ensure it evaluates to false.
#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

// Check for clang intrinsic.
#if __has_builtin(__builtin_debugtrap)
#define __debugbreak __builtin_debugtrap

// If we have GCC or compiler that tries to be compatible, use GCC inline assembly.
#elif defined __GNUC__ || defined __clang__
#if defined(__i386__) || defined(__x86_64__)
extern __attribute__((gnu_inline, always_inline)) inline void __debugbreak(void)
{
    __asm__ volatile("int $0x03");
}
#elif defined(__arm__)
extern __attribute__((gnu_inline, always_inline)) inline void __debugbreak(void)
{
    __asm__ volatile("bkpt #3");
}
#elif defined(__aarch64__)
extern __attribute__((gnu_inline, always_inline)) inline void __debugbreak(void)
{
    // same values as used by msvc __debugbreak on arm64
    __asm__ volatile("brk #0xF000");
}
#elif defined(__powerpc__)
extern __attribute__((gnu_inline, always_inline)) inline void __debugbreak(void)
{
    __asm__ volatile(".4byte 0x7d821008");
}
#else
#error __debugbreak not currently supported on this processor platform, see base/debugbreak.h
#endif // CPU architectures on GCC like compilers
#else
#error __debugbreak not currently supported on this compiler, see base/debugbreak.h
#endif // compiler defines

#endif // msvc exclusion and check for existing __debugbreak symbol.

#ifdef __cplusplus
}
#endif

#endif // DEBUGBREAK_H
