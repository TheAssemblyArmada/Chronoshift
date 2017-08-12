////////////////////////////////////////////////////////////////////////////////
//                            --  REDALERT++ --                               //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: Redalert++
//
//          File:: NEWDEL.CPP
//
//        Author:: tomsons26
//
//  Contributors:: OmniBlade
//
//   Description:: Custom memory allocators for new and delete.
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
#include "newdel.h"
#include "alloc.h"

void* operator new(unsigned int size)
{
    return Alloc(size, MEM_NEW);
}

void* operator new[](unsigned int size)
{
    return Alloc(size, MEM_NEW);
}

void operator delete(void* ptr)
{
    Free(ptr);
}