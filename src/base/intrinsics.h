/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Implements a few intrinsics from other compilers in watcom.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */

#ifndef INTRINSICS_H
#define INTRINSICS_H

#include "compiler.h"
#include "config.h"

#ifdef HAVE_INTRIN_H
#include <intrin.h>
#endif

#ifdef HAVE_X86INTRIN_H
#include <x86intrin.h>
#endif

#ifdef HAVE_CPUID_H
#include <cpuid.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Watcom doesn't have the modern intrinsics, implement them here.
#ifdef __WATCOMC__
unsigned int __readeflags(void);
#pragma aux __readeflags = \
    "pushfd" \
    "pop eax" value[eax] modify[eax];
#define HAVE__READEFLAGS

unsigned int __writeeflags(unsigned int eflg);
#pragma aux __writeeflags = \
    "push eax" \
    "popfd" parm[eax] modify[];
#define HAVE__WRITEEFLAGS

void __cpuid(int cpuinfo[4], int function_id);
#pragma aux __cpuid = \
    ".586" \
    "cpuid" \
    "mov [esi+0],eax" \
    "mov [esi+4],ebx" \
    "mov [esi+8],ecx" \
    "mov [esi+12],edx" parm[esi][eax] modify[ebx ecx edx];
#define HAVE__CPUID_MSVC

void __cpuidex(int cpuinfo[4], int function_id, int subfunction_id);
#pragma aux __cpuidex = \
    ".586" \
    "cpuid" \
    "mov [esi+0],eax" \
    "mov [esi+4],ebx" \
    "mov [esi+8],ecx" \
    "mov [esi+12],edx" parm[esi][eax][ecx] modify[ebx ecx edx];
#define HAVE__CPUIDEX

unsigned long long __rdtsc(void);
#pragma aux __rdtsc = \
    " .586 " \
    "rdtsc" value[edx eax] modify[] nomemory;
#define HAVE__RDTSC

int _interlockedbittestandset(volatile long *base, long offset);
#pragma aux _interlockedbittestandset = \
    " .586 " \
    "lock bts [eax],edx" \
    "setb cl" \
    "movzx eax,cl" parm[eax edx] value[eax] modify[cl]

void __debugbreak(void);
#pragma aux __debugbreak = "int 3"

#elif !defined _WIN32

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
#error __debugbreak not currently supported on this processor platform, see base/intrinsics.h
#endif // CPU architectures on GCC like compilers
#else
#error __debugbreak not currently supported on this compiler, see base/intrinsics.h
#endif // compiler defines
#endif // __WATCOMC__

#ifdef __cplusplus
}
#endif

#endif // INTRINSICS_H
