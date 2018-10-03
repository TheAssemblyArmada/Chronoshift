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

#ifndef WATCOMINTRIN_H
#define WATCOMINTRIN_H

#ifdef __WATCOMC__

unsigned int __readeflags(void);
#pragma aux __readeflags = \
    "pushfd" \
    "pop eax" \
value[eax] modify[eax];

unsigned int __writeeflags(unsigned int eflg);
#pragma aux __writeeflags = \
    "push eax" \
    "popfd" \
parm[eax] modify[];

void __cpuid(int cpuinfo[4], int function_id);
#pragma aux __cpuid = \
    ".586" \
    "cpuid" \
    "mov [esi+0],eax" \
    "mov [esi+4],ebx" \
    "mov [esi+8],ecx" \
    "mov [esi+12],edx" \
parm[esi][eax] modify[ebx ecx edx];

void __cpuidex(int cpuinfo[4], int function_id, int subfunction_id);
#pragma aux __cpuidex = \
    ".586" \
    "cpuid" \
    "mov [esi+0],eax" \
    "mov [esi+4],ebx" \
    "mov [esi+8],ecx" \
    "mov [esi+12],edx" \
parm[esi][eax][ecx] modify[ebx ecx edx];

unsigned long long __rdtsc(void);
#pragma aux __rdtsc = \
    " .586 " \
    "rdtsc" \
value[edx eax] modify[] nomemory;

int _interlockedbittestandset(volatile long *base, long offset);
#pragma aux _interlockedbittestandset = \
    " .586 " \
    "lock bts [eax],edx" \
    "setb cl" \
    "movzx eax,cl" \
parm[eax edx] value[eax] modify[cl]

#endif

#endif
