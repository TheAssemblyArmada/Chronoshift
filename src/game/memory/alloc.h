/**
 * @file
 *
 * @author tomsons26
 * @author OmniBlade
 *
 * @brief Memory allocation wrapper functions.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
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

#ifndef RAPP_STANDALONE
#include "hooker.h"
#include <malloc.h>
#endif

enum MemoryFlagType
{
    MEM_NORMAL = 0,
    MEM_NEW = 1,
    MEM_CLEAR = 2,
};

typedef int(*memerror_t)();
extern memerror_t g_memoryError; // Memory error handler function pointer.

void *Alloc(unsigned int bytes_to_alloc, MemoryFlagType flags);
void Free(void *pointer);
void *Resize_Alloc(void *original_ptr, unsigned int new_size_in_bytes);
int Ram_Free();
int Heap_Size(MemoryFlagType flag);
int Total_Ram_Free(MemoryFlagType flag);

#ifndef RAPP_STANDALONE
inline void Memory_Hook_Me(void)
{
    Hook_Function(0x005C5965, &malloc);
    Hook_Function(0x005C3945, &free);
    Hook_Function(0x005DE4DE, &realloc);
    Hook_Function(0x005D5FC0, &Alloc);
    Hook_Function(0x005D6010, &Free);
    Hook_Function(0x005D6020, &Resize_Alloc);
}
#endif

#endif
