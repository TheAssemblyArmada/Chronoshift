/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Control pre-allocation of memory to allow faster object creation/destruction.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "heap.h"

FixedHeapClass::FixedHeapClass() :
    IsAllocated(false),
    HeapSize(0),
    HeapLength(0),
    ActiveCount(0),
    Pointer(nullptr),
    BitArray()
{
    // empty
}

FixedHeapClass::FixedHeapClass(int size) :
    IsAllocated(false),
    HeapSize(size),
    HeapLength(0),
    ActiveCount(0),
    Pointer(nullptr),
    BitArray()
{
    // empty
}

FixedHeapClass::~FixedHeapClass()
{
    Clear();
}

int FixedHeapClass::ID(void const *object) const
{
    if (object && HeapSize) {
        return (static_cast<char const *>(object) - static_cast<char const *>(Pointer)) / HeapSize;
    }

    return -1;
}

bool FixedHeapClass::Set_Heap(int size, void *buffer)
{
    Clear();

    if (HeapSize) {
        if (size) {
            if (!BitArray.Resize(size)) {
                return false;
            }

            if (!buffer) {
                buffer = new char[HeapSize * size];
                if (!buffer) {
                    BitArray.Clear();
                    return false;
                }

                IsAllocated = true;
            }

            Pointer = buffer;
            HeapLength = size;
        }

        return true;
    }

    return false;
}

void *FixedHeapClass::Allocate()
{
    int free_index;

    if (Count() >= Length() || (free_index = BitArray.First_False(), free_index == -1)) {
        return nullptr;
    }

    ++ActiveCount;
    BitArray[free_index] = true;

    return &(*this)[free_index];
}

void FixedHeapClass::Clear()
{
    if (Pointer && IsAllocated) {
        delete[] static_cast<char *>(Pointer);
    }

    Pointer = nullptr;
    IsAllocated = false;
    ActiveCount = 0;
    HeapLength = 0;
    BitArray.Clear();
}

bool FixedHeapClass::Free(void *object)
{
    int index;

    if (!object || !Count() || (index = this->ID(object), index >= Count())) {
        return false;
    }

    if (BitArray[index]) {
        --ActiveCount;
        BitArray[index] = false;

        return true;
    }

    return false;
}

bool FixedHeapClass::Free_All()
{
    ActiveCount = 0;
    BitArray.Reset();

    return true;
}

FixedIHeapClass::FixedIHeapClass() : FixedHeapClass(), Objects()
{
    // empty
}

FixedIHeapClass::FixedIHeapClass(int size) : FixedHeapClass(size), Objects()
{
    // empty
}

FixedIHeapClass::~FixedIHeapClass()
{
    Objects.Clear();
}

bool FixedIHeapClass::Set_Heap(int size, void *buffer)
{
    Clear();

    if (FixedHeapClass::Set_Heap(size, buffer)) {
        Objects.Resize(size);

        return true;
    }

    return false;
}

void *FixedIHeapClass::Allocate()
{
    void *object = nullptr;

    if ((object = FixedHeapClass::Allocate()) != nullptr) {
        Objects.Add(object);
        memset(object, 0, HeapSize);
    }

    return object;
}

void FixedIHeapClass::Clear()
{
    FixedHeapClass::Clear();
    Objects.Clear();
}

bool FixedIHeapClass::Free(void *object)
{
    if (FixedHeapClass::Free(object)) {
        Objects.Delete(object);

        return true;
    }

    return false;
}

bool FixedIHeapClass::Free_All()
{
    Objects.Reset_Active();

    return FixedHeapClass::Free_All();
}

int FixedIHeapClass::Logical_ID(void *object) const
{
    if (object) {
        for (int index = 0; index < Count(); ++index) {
            if (Active_Ptr(index) == object) {
                return index;
            }
        }
    }

    return -1;
}

int FixedIHeapClass::Logical_ID(int index) const
{
    return Logical_ID((char *)&FixedHeapClass::operator[](index));
}

void *FixedIHeapClass::Active_Ptr(int index)
{
    return Objects[index];
}

void *FixedIHeapClass::Active_Ptr(int index) const
{
    return Objects[index];
}
