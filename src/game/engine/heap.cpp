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

/**
 * @brief Returns the position of the object in the heap if present, otherwise returns -1.
 */
int FixedHeapClass::ID(const void *object) const
{
    if (object != nullptr && HeapSize > 0) {
        return (static_cast<const char *>(object) - Pointer) / HeapSize;
    }

    return -1;
}

/**
 * @brief Sets the number of objects the heap can contain and optionally the memory to use for it.
 */
BOOL FixedHeapClass::Set_Heap(int size, void *buffer)
{
    Clear();

    if (HeapSize) {
        if (size) {
            if (!BitArray.Resize(size)) {
                return false;
            }

            if (buffer == nullptr) {
                buffer = new char[HeapSize * size];

                if (buffer == nullptr) {
                    BitArray.Clear();
                    return false;
                }

                IsAllocated = true;
            }

            Pointer = static_cast<char *>(buffer);
            HeapLength = size;
        }

        return true;
    }

    return false;
}

/**
 * @brief Allocate a "HeapSize" block of memory from the heap.
 */
void *FixedHeapClass::Allocate()
{
    if (ActiveCount >= HeapLength) {
        return nullptr;
    }

    int free_index = BitArray.First_False();

    if (free_index == -1) {
        return nullptr;
    }

    ++ActiveCount;
    BitArray[free_index] = true;

    return &(*this)[free_index];
}

/**
 * @brief Clear the heap and free memory in use if it was allocated by the class.
 */
void FixedHeapClass::Clear()
{
    if (Pointer != nullptr && IsAllocated) {
        delete[] Pointer;
    }

    Pointer = nullptr;
    IsAllocated = false;
    ActiveCount = 0;
    HeapLength = 0;
    BitArray.Clear();
}

/**
 * @brief Frees a block of memory allocated from the heap.
 */
BOOL FixedHeapClass::Free(void *object)
{
    if (object == nullptr || ActiveCount == 0) {
        return false;
    }

    int index = ID(object);

    if (index >= HeapLength) {
        DEBUG_LOG("Recovered index %d is outside the heap of size %d.\n", index, HeapLength);
        return false;
    }

    if (BitArray[index]) {
        --ActiveCount;
        BitArray[index] = false;

        return true;
    }

    return false;
}

/**
 * @brief Frees all objects allocated from the heap.
 */
BOOL FixedHeapClass::Free_All()
{
    ActiveCount = 0;
    BitArray.Reset();

    return true;
}

FixedIHeapClass::FixedIHeapClass()
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

/**
 * @brief Sets the number of objects the heap can contain and optionally the memory to use for it.
 */
BOOL FixedIHeapClass::Set_Heap(int size, void *buffer)
{
    Clear();

    if (FixedHeapClass::Set_Heap(size, buffer)) {
        Objects.Resize(size);

        return true;
    }

    return false;
}

/**
 * @brief Allocate a "HeapSize" block of memory from the heap.
 */
void *FixedIHeapClass::Allocate()
{
    void *object = FixedHeapClass::Allocate();

    if (object != nullptr) {
        Objects.Add(object);
        memset(object, 0, HeapSize);
    }

    return object;
}

/**
 * @brief Clear the heap and free memory in use if it was allocated by the class.
 */
void FixedIHeapClass::Clear()
{
    FixedHeapClass::Clear();
    Objects.Clear();
}

/**
 * @brief Frees a block of memory allocated from the heap.
 */
BOOL FixedIHeapClass::Free(void *object)
{
    if (FixedHeapClass::Free(object)) {
        Objects.Delete(object);

        return true;
    }

    return false;
}

/**
 * @brief Frees a all memory allocated from the heap.
 */
BOOL FixedIHeapClass::Free_All()
{
    Objects.Reset_Active();

    return FixedHeapClass::Free_All();
}

/**
 * @brief Gets the logical ID in the index heap from an object pointer.
 */
int FixedIHeapClass::Logical_ID(void *object) const
{
    if (object != nullptr) {
        for (int index = 0; index < ActiveCount; ++index) {
            if (Active_Ptr(index) == object) {
                return index;
            }
        }
    }

    return -1;
}

/**
 * @brief Gets the logical ID in the index heap from an heap ID.
 */
int FixedIHeapClass::Logical_ID(int index) const
{
    return Logical_ID((char *)&FixedHeapClass::operator[](index));
}

/**
 * @brief Gets a pointer to an object from its logical ID.
 */
void *FixedIHeapClass::Active_Ptr(int index)
{
    return Objects[index];
}

/**
 * @brief Gets a pointer to an object from its logical ID (const).
 */
void *FixedIHeapClass::Active_Ptr(int index) const
{
    return Objects[index];
}
