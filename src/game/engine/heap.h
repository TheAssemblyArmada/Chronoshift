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
#pragma once

#ifndef HEAP_H
#define HEAP_H

#include "gamedebug.h"
#include "noinit.h"
#include "pipe.h"
#include "straw.h"
#include "vector.h"

#define FIXED_HEAP_MAX 500

class FixedHeapClass
{
public:
    FixedHeapClass();
    FixedHeapClass(int size);
    virtual ~FixedHeapClass();

    char &operator[](int index)
    {
        DEBUG_ASSERT(index < ActiveCount);
        return *(HeapSize * index + Pointer);
    }

    char const &operator[](int index) const
    {
        DEBUG_ASSERT(index < ActiveCount);
        return *(HeapSize * index + Pointer);
    }

    virtual int ID(const void *object) const;
    virtual BOOL Set_Heap(int size, void *buffer = nullptr);
    virtual void *Allocate();
    virtual void Clear();
    virtual BOOL Free(void *object);
    virtual BOOL Free_All();

    int Length() { return HeapLength; }
    int Heap_Size() const { return HeapSize; }
    int Count() const { return ActiveCount; }
    void *Get_Pointer() const { return Pointer; }

#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
#endif

protected:
    BOOL IsAllocated; // Did this class allocate the memory for the heap?
    int HeapSize; // Size of each object in the heap.
    int HeapLength; // Max number of objects the heap is sized for.
    int ActiveCount; // Number of currently allocated objects in heap.
    char *Pointer;
    BooleanVectorClass BitArray; // Tracks free blocks.
};

class FixedIHeapClass : public FixedHeapClass
{
public:
    FixedIHeapClass();
    FixedIHeapClass(int size);
    virtual ~FixedIHeapClass();

    virtual BOOL Set_Heap(int size, void *buffer = nullptr) override;
    virtual void *Allocate() override;
    virtual void Clear() override;
    virtual BOOL Free(void *object) override;
    virtual BOOL Free_All() override;
    virtual int Logical_ID(void *object) const;
    virtual int Logical_ID(int index) const;
    virtual void *Active_Ptr(int index);
    virtual void *Active_Ptr(int index) const;

#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
#endif

protected:
    DynamicVectorClass<void *> Objects;
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"

inline void FixedHeapClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x004CD60C, *FixedHeapClass::Allocate);
    Hook_Function(0x004CD744, *FixedHeapClass::Clear);
    Hook_Function(0x004CD6A0, *FixedHeapClass::Free);
    Hook_Function(0x004CD78C, *FixedHeapClass::Free_All);
    Hook_Function(0x004CD594, *FixedHeapClass::Set_Heap);
    //Hook_Function(0x004CD718, *FixedHeapClass::ID);
#endif
}

inline void FixedIHeapClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x004CD818, *FixedIHeapClass::Allocate);
    Hook_Function(0x004CD7C4, *FixedIHeapClass::Clear);
    Hook_Function(0x004CD850, *FixedIHeapClass::Free);
    Hook_Function(0x004CD7A8, *FixedIHeapClass::Free_All);
    Hook_Function(0x004CD7E4, *FixedIHeapClass::Set_Heap);
    //Hook_Function();
#endif
}

#endif

template<class T>
class TFixedIHeapClass : public FixedIHeapClass
{
public:
    TFixedIHeapClass() : FixedIHeapClass(sizeof(T)) {}
    TFixedIHeapClass(int size) : FixedIHeapClass(size) {}
    virtual ~TFixedIHeapClass();

    T &operator[](int index)
    {
        DEBUG_ASSERT(index < ActiveCount);
        return *((T *)&FixedHeapClass::operator[](index));
    }

    T const &operator[](int index) const
    {
        DEBUG_ASSERT(index < ActiveCount);
        return *((T *)&FixedHeapClass::operator[](index));
    }

    // these overide the ones in FixedIHeapClass
    virtual BOOL Free(void *object) { return FixedIHeapClass::Free(object); }
    virtual int Logical_ID(int index) const { return FixedIHeapClass::Logical_ID(index); }
    virtual int ID(T const *object) const { return FixedIHeapClass::ID(object); }
    virtual int Logical_ID(T *object) const { return FixedIHeapClass::Logical_ID((void *)object); }
    virtual T *Alloc() { return (T *)FixedIHeapClass::Allocate(); }
    virtual BOOL Free(T *object) { return FixedIHeapClass::Free(object); }
    virtual BOOL Save(Pipe &pipe) const;
    virtual BOOL Load(Straw &straw);
    virtual void Code_Pointers();
    virtual void Decode_Pointers();
    virtual T *Ptr(int id) const { return (T *)Objects[id]; }
    virtual T *Raw_Ptr(int id) { return &operator[](id); } //<RA_TODO> - needs recoding to match with operators now
};

template<class T>
TFixedIHeapClass<T>::~TFixedIHeapClass()
{
    Objects.Delete_All();
}

template<class T>
BOOL TFixedIHeapClass<T>::Save(Pipe &pipe) const
{
    int32_t active_count = ActiveCount;

    // Write the number of active objects in the heap.
    if (pipe.Put(&active_count, sizeof(active_count)) == sizeof(active_count)) {
        // Write the ID and object binary data to the pipe.
        // TODO A new save format will be needed for cross platform portability.
        for (int i = 0; i < ActiveCount; ++i) {
            int32_t id = ID(Ptr(i));

            if (pipe.Put(&id, sizeof(id)) != sizeof(id)) {
                return false;
            }

            if (pipe.Put(Ptr(i), sizeof(T)) != sizeof(T)) {
                return false;
            }
        }

        return true;
    }

    return false;
}

template<class T>
BOOL TFixedIHeapClass<T>::Load(Straw &straw)
{
    int32_t heaplen;

    // Get number of objects in the saved heap and iterate through them.
    if (straw.Get(&heaplen, sizeof(heaplen)) == sizeof(heaplen) && heaplen <= HeapLength) {
        for (int i = 0; i < heaplen; ++i) {
            int32_t id;

            // Load what id the object had in the heap.
            if (straw.Get(&id, sizeof(id)) != sizeof(id)) {
                return false;
            }

            T *obj = &(*this)[id];
            BitArray[id] = true;
            ++ActiveCount;
            Objects.Add(obj);

            // Load the object from disk.
            // TODO A new save format will be needed for cross platform portability.
            if (straw.Get(obj, sizeof(T)) != sizeof(T)) {
                return false;
            }

            // Call the no init ctor so no changes are made to loaded memory.
            NoInitClass noinit;
            new(obj) T(noinit);
        }

        return true;
    }

    return false;
}

template<class T>
void TFixedIHeapClass<T>::Code_Pointers()
{
    for (int index = 0; index < Count(); ++index) {
        Ptr(index)->Code_Pointers();
    }
}

template<class T>
void TFixedIHeapClass<T>::Decode_Pointers()
{
    for (int index = 0; index < Count(); ++index) {
        Ptr(index)->Decode_Pointers();
    }
}

#endif // HEAP_H
