/**
 * @file
 *
 * @author tomsons26
 * @author OmniBlade
 *
 * @brief Memory allocation wrapper functions.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef ALLOC_H
#define ALLOC_H

#include "always.h"

enum MemoryFlagType
{
    MEM_NORMAL = 0x0,
    MEM_NEW = 0x1,
    MEM_CLEAR = 0x2,
    MEM_REAL = 0x4,
    MEM_TEMP = 0x8,
    MEM_LOCK = 0x10
};

DEFINE_ENUMERATION_BITWISE_OPERATORS(MemoryFlagType);

typedef void(*memerrorhandler_t)();
typedef void (*memexithandler_t)(const char *);

void *Alloc(unsigned int bytes_to_alloc, MemoryFlagType flags);
void Free(void *pointer);
void *Resize_Alloc(void *original_ptr, unsigned int new_size_in_bytes);
int Ram_Free();
int Heap_Size(MemoryFlagType flag);
int Total_Ram_Free(MemoryFlagType flag);

#ifdef GAME_DLL
extern memerrorhandler_t &g_MemoryError; // Memory error handler function pointer.
extern memexithandler_t &g_MemoryErrorExit;
#else
extern memerrorhandler_t g_MemoryError; // Memory error handler function pointer.
extern memexithandler_t g_MemoryErrorExit;
#endif

#endif
