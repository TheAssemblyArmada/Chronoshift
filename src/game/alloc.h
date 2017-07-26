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
#pragma once

#ifndef ALLOC_H
#define ALLOC_H

#include "always.h"

enum MemoryFlagType
{
	MEM_NORMAL = 0,
	MEM_NEW = 1,
	MEM_CLEAR = 2,
};

void *Alloc(unsigned int bytes_to_alloc, MemoryFlagType flags);
void Free(void *pointer);
void *Resize_Alloc(void *original_ptr, unsigned int new_size_in_bytes);
int Ram_Free(MemoryFlagType flag);
int Heap_Size(MemoryFlagType flag);
int Total_Ram_Free(MemoryFlagType flag);

#endif
