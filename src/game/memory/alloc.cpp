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
#include "alloc.h"
#include "gamedebug.h"
#include <malloc.h>
#include <stdlib.h>

#ifndef CHRONOSHIFT_STANDALONE
memerrorhandler_t &g_memoryError = Make_Global<memerrorhandler_t>(0x006B1A2C); // Memory error handler function pointer.
memexithandler_t &g_memoryErrorExit = Make_Global<memexithandler_t>(0x006B1A30);
#else
memerrorhandler_t g_memoryError = nullptr; // Memory error handler function pointer.
memexithandler_t g_memoryErrorExit = nullptr;
#endif

static int g_memoryCalls;
unsigned int g_totalRam;
unsigned int g_minRam;
unsigned int g_maxRam;

void *Alloc(unsigned int bytes_to_alloc, MemoryFlagType flags)
{
    // DEBUG_LOG("Attempting to allocate memory of size %d with flags %d.\n", bytes_to_alloc, flags);
    void *ptr = malloc(bytes_to_alloc);

    if (ptr == nullptr && g_memoryError != nullptr) {
        g_memoryError();
    }

    if (ptr != nullptr && (flags & MEM_CLEAR) != 0) {
        memset(ptr, 0, bytes_to_alloc);
    }

    ++g_memoryCalls;

    return ptr;
}

void Free(void *ptr)
{
    if (ptr != nullptr) {
        free(ptr);
        --g_memoryCalls;
    }
}

void *Resize_Alloc(void *original_ptr, unsigned int new_size_in_bytes)
{
    void *temp = nullptr;

    temp = realloc(original_ptr, new_size_in_bytes);

    if (temp == nullptr && g_memoryError != nullptr) {
        g_memoryError();
    }

    return temp;
}

int Ram_Free()
{
#if defined PLATFORM_WINDOWS
    MEMORYSTATUSEX mem;

    mem.dwLength = sizeof(mem);
    GlobalMemoryStatusEx(&mem);

    return mem.ullAvailPhys > INT32_MAX ? INT32_MAX : mem.ullAvailPhys;
#elif defined PLATFORM_LINUX
    struct sysinfo mem;
    sysinfo(&mem);
    return mem.freeram * mem.mem_unit > INT32_MAX ? INT32_MAX : mem.freeram * mem.mem_unit;
#else
#error Add code for this platform for detecting free memory in alloc.cpp.
#endif
}

int Heap_Size(MemoryFlagType flag)
{
    return Ram_Free();
}

int Total_Ram_Free(MemoryFlagType flag)
{
    return Ram_Free();
}
