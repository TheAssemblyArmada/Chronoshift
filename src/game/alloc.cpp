////////////////////////////////////////////////////////////////////////////////
//                            --  REDALERT++ --                               //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: Redalert++
//
//          File:: ALLOC.H
//
//        Author:: tomsons26
//
//  Contributors:: OmniBlade
//
//   Description:: Memory allocation functions.
//
//       License:: Redalert++ is free software: you can redistribute it and/or 
//                 modify it under the terms of the GNU General Public License 
//                 as published by the Free Software Foundation, either version 
//                 2 of the License, or (at your option) any later version.
//
//                 A full copy of the GNU General Public License can be found in
//                 LICENSE
//
////////////////////////////////////////////////////////////////////////////////
#include "alloc.h"
#include "gamedebug.h"
#include <stdlib.h>

typedef int(*memerror_t)();
static memerror_t g_memoryError = nullptr; // Memory error handler function pointer.
static int g_memoryCalls;
unsigned int TotalRam;
unsigned int MinRam;
unsigned int MaxRam;

void *Alloc(unsigned int bytes_to_alloc, MemoryFlagType flags)
{
    //DEBUG_LOG("Attempting to allocate memory of size %d with flags %d.\n", bytes_to_alloc, flags);
    void *ptr = malloc(bytes_to_alloc);

    if ( ptr == nullptr && g_memoryError != nullptr ) {
        g_memoryError();
    }
    
    if ( ptr != nullptr && (flags & MEM_CLEAR) != 0 ) {
        memset(ptr, 0, bytes_to_alloc);
    }
    
    ++g_memoryCalls;
    
    return ptr;
}

void Free(void *ptr)
{
    if ( ptr != nullptr ) {
        free(ptr);
        --g_memoryCalls;
    }
}

void *Resize_Alloc(void *original_ptr, unsigned int new_size_in_bytes)
{
    void *temp = nullptr;

    temp = realloc(original_ptr, new_size_in_bytes);

    if ( temp == nullptr && g_memoryError != nullptr ) {
        g_memoryError();
    }

    return temp;
}

int Ram_Free(MemoryFlagType flag)
{/*
    MEMORYSTATUSEX mem_struct; // [sp+0h] [bp-20h]@1

    mem_struct.dwLength = sizeof(mem_struct);
    GlobalMemoryStatusEx(&mem_struct);

    return mem_struct.ullAvailPhys > 0x7FFFFFFF ? 0x7FFFFFFF : mem_struct.ullAvailPhys;
    */
    return 0x7FFFFFFF;;
}

int Heap_Size(MemoryFlagType flag)
{
    return Ram_Free(flag);
}

int Total_Ram_Free(MemoryFlagType flag)
{
    return Ram_Free(flag);
}
