/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Classes for resizable arrays ("vectors").
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef VECTOR_H
#define VECTOR_H

#include "always.h"
#include "bitarray.h"
#include "gamedebug.h"
#include <new>
#include <stdlib.h>
#include <string.h>

class NoInitClass;

template<typename T>
class VectorClass
{
public:
    VectorClass(int size = 0, const T *array = 0);
    VectorClass(const NoInitClass &noinit) {};
    VectorClass(VectorClass<T> const &);
    virtual ~VectorClass();

    T &operator[](int index);
    const T &operator[](int index) const;
    virtual VectorClass<T> &operator=(VectorClass<T> const &);
    virtual BOOL operator==(VectorClass<T> const &) const;

    virtual BOOL Resize(int newsize, const T *array = 0);
    virtual void Clear();
    virtual int ID(const T *ptr);
    virtual int ID(const T &ptr);

    int Length() const { return VectorMax; }

protected:
    T *Vector;
    int VectorMax;
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool IsAllocated : 1; // & 1
        };
        int m_vectorFlags;
    };
#else
    bool IsAllocated;
#endif
};

template<typename T>
T &VectorClass<T>::operator[](int index)
{
    DEBUG_ASSERT(unsigned(index) < unsigned(VectorMax));
    return Vector[index];
}

template<typename T>
const T &VectorClass<T>::operator[](int index) const
{
    DEBUG_ASSERT(unsigned(index) < unsigned(VectorMax));
    return Vector[index];
}

template<typename T>
VectorClass<T>::VectorClass(int size, const T *array) : Vector(0), VectorMax(size), IsAllocated(false)
{
    //	Allocate the vector. The default constructor will be called for every
    //	object in this vector.
    if (size > 0) {
        if (array != nullptr) {
            Vector = new ((void *)array) T[size];
        } else {
            Vector = new T[size];
            IsAllocated = true;
        }
    }
}

template<typename T>
VectorClass<T>::~VectorClass()
{
    VectorClass<T>::Clear();
}

template<typename T>
VectorClass<T>::VectorClass(VectorClass<T> const &vector) : Vector(0), VectorMax(0), IsAllocated(false)
{
    *this = vector;
}

template<typename T>
VectorClass<T> &VectorClass<T>::operator=(VectorClass<T> const &vector)
{
    if (this != &vector) {
        Clear();
        VectorMax = vector.Length();

        if (VectorMax) {
            Vector = new T[VectorMax];

            if (Vector) {
                IsAllocated = true;

                for (int index = 0; index < VectorMax; index++) {
                    Vector[index] = vector[index];
                }
            }
        } else {
            Vector = 0;
            IsAllocated = false;
        }
    }

    return *this;
}

template<typename T>
BOOL VectorClass<T>::operator==(VectorClass<T> const &vector) const
{
    if (VectorMax == vector.Length()) {
        for (int index = 0; index < VectorMax; index++) {
            if (Vector[index] != vector[index]) {
                return false;
            }
        }

        return true;
    }

    return false;
}

template<typename T>
inline int VectorClass<T>::ID(const T *ptr)
{
    return ((uintptr_t)ptr - (uintptr_t)Vector) / sizeof(T);
}

template<typename T>
int VectorClass<T>::ID(const T &object)
{
    for (int index = 0; index < VectorMax; index++) {
        if ((*this)[index] == object) {
            return index;
        }
    }

    return -1;
}

template<typename T>
void VectorClass<T>::Clear()
{
    if (Vector && IsAllocated) {
        delete[] Vector;
        Vector = nullptr;
    }

    IsAllocated = false;
    VectorMax = 0;
}

template<typename T>
BOOL VectorClass<T>::Resize(int newsize, const T *array)
{
    if (newsize != 0) {
        T *newptr;

        if (array == nullptr) {
            newptr = new T[newsize];
        } else {
            newptr = new ((void *)array) T[newsize];
        }

        if (!newptr) {
            return false;
        }

        if (Vector != nullptr) {
            int copy_count = (newsize < VectorMax) ? newsize : VectorMax;

            for (int i = 0; i < copy_count; i++) {
                newptr[i] = Vector[i];
            }

            if (IsAllocated) {
                delete[] Vector;
                Vector = nullptr;
            }
        }

        Vector = newptr;
        VectorMax = newsize;
        IsAllocated = (Vector && !array);

    } else {
        Clear();
    }

    return true;
}

template<typename T>
class DynamicVectorClass : public VectorClass<T>
{
protected:
#ifndef COMPILER_WATCOM
    // Looks like watcom doesn't like these declarations, newer compilers need them for standards compliance related to
    // template lookup.
    using VectorClass<T>::Vector;
    using VectorClass<T>::VectorMax;
    using VectorClass<T>::IsAllocated;
#endif

public:
    DynamicVectorClass(unsigned size = 0, const T *array = 0);

    BOOL operator==(const DynamicVectorClass &src) { return false; }
    BOOL operator!=(const DynamicVectorClass &src) { return true; }
    DynamicVectorClass<T> &operator=(DynamicVectorClass<T> const &rvalue);

    virtual BOOL Resize(int newsize, const T *array = 0) override;
    virtual void Clear() override;
    virtual int ID(const T *ptr) override { return VectorClass<T>::ID(ptr); };
    virtual int ID(const T &ptr) override;

    void Reset_Active() { ActiveCount = 0; }
    void Set_Active(int count) { ActiveCount = count; }
    int Count() const { return (ActiveCount); }
    BOOL Add(const T &object);
    BOOL Add_Head(const T &object);
    BOOL Insert(int index, const T &object);
    BOOL Delete(const T &object);
    BOOL Delete(int index);
    void Delete_All();
    int Set_Growth_Step(int step) { return GrowthStep = step; }
    int Growth_Step() { return GrowthStep; }
    T *Uninitialized_Add();

protected:
    int ActiveCount;
    int GrowthStep;
};

template<typename T>
void DynamicVectorClass<T>::Clear()
{
    ActiveCount = 0;
    VectorClass<T>::Clear();
};

template<typename T>
DynamicVectorClass<T> &DynamicVectorClass<T>::operator=(DynamicVectorClass<T> const &rvalue)
{
    VectorClass<T>::operator=(rvalue);
    ActiveCount = rvalue.ActiveCount;
    GrowthStep = rvalue.GrowthStep;

    return *this;
}

template<typename T>
DynamicVectorClass<T>::DynamicVectorClass(unsigned size, const T *array) : VectorClass<T>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}

template<typename T>
BOOL DynamicVectorClass<T>::Resize(int newsize, const T *array)
{
    if (VectorClass<T>::Resize(newsize, array)) {
        if (VectorMax < ActiveCount)
            ActiveCount = VectorMax;
        return true;
    }
    return false;
}

template<typename T>
int DynamicVectorClass<T>::ID(const T &object)
{
    for (int index = 0; index < Count(); index++) {
        if ((*this)[index] == object)
            return index;
    }

    return -1;
}

template<typename T>
BOOL DynamicVectorClass<T>::Add(const T &object)
{
    if (ActiveCount >= VectorMax) {
        if ((IsAllocated || !VectorMax) && GrowthStep > 0) {
            if (!Resize(VectorMax + GrowthStep)) {
                return false;
            }
        } else {
            return false;
        }
    }

    (*this)[ActiveCount++] = object;

    return true;
}

template<typename T>
BOOL DynamicVectorClass<T>::Add_Head(const T &object)
{
    if (ActiveCount >= VectorMax) {
        if ((IsAllocated || !VectorMax) && GrowthStep > 0) {
            if (!Resize(VectorMax + GrowthStep)) {
                return false;
            }
        } else {
            return false;
        }
    }

    if (ActiveCount) {
        memmove(&(*this)[1], &(*this)[0], ActiveCount * sizeof(T));
    }

    (*this)[0] = object;
    ActiveCount++;

    return true;
}

template<typename T>
BOOL DynamicVectorClass<T>::Insert(int index, const T &object)
{
    if (index < 0)
        return false;
    if (index > ActiveCount)
        return false;

    if (ActiveCount >= VectorMax) {
        if ((IsAllocated || !VectorMax) && GrowthStep > 0) {
            if (!Resize(VectorMax + GrowthStep)) {
                return false;
            }
        } else {
            return false;
        }
    }

    if (index < ActiveCount) {
        memmove(&(*this)[index + 1], &(*this)[index], (ActiveCount - index) * sizeof(T));
    }

    (*this)[index] = object;
    ActiveCount++;

    return true;
}

template<typename T>
BOOL DynamicVectorClass<T>::Delete(const T &object)
{
    int id = ID(object);

    if (id != -1) {
        return Delete(id);
    }

    return false;
}

template<typename T>
BOOL DynamicVectorClass<T>::Delete(int index)
{
    if (index < ActiveCount) {
        ActiveCount--;

        for (int i = index; i < ActiveCount; i++) {
            (*this)[i] = (*this)[i + 1];
        }

        return true;
    }

    return false;
}

template<typename T>
void DynamicVectorClass<T>::Delete_All()
{
    int len = VectorMax;
    Clear();
    Resize(len);
}

template<typename T>
T *DynamicVectorClass<T>::Uninitialized_Add()
{
    if (ActiveCount >= VectorMax) {
        if (GrowthStep > 0) {
            if (!Resize(VectorMax + GrowthStep)) {
                return nullptr;
            }
        } else {
            return nullptr;
        }
    }

    return &((*this)[ActiveCount++]);
}

class BooleanVectorClass
{
public:
    BooleanVectorClass(unsigned size = 0, uint8_t *array = 0);
    BooleanVectorClass(BooleanVectorClass const &vector);

    BooleanVectorClass &operator=(BooleanVectorClass const &vector);
    BOOL operator==(BooleanVectorClass const &vector) const;

    void Init(unsigned size, uint8_t *array);
    void Init(unsigned size);
    int Length() { return BitCount; };
    void Reset();
    void Set();
    void Clear();
    int Resize(unsigned size);

    BOOL const &operator[](int index) const
    {
        if (LastIndex != index) {
            Fixup(index);
        }

        return Copy;
    };

    BOOL &operator[](int index)
    {
        if (LastIndex != index) {
            Fixup(index);
        }

        return Copy;
    };

    BOOL Is_True(int index) const
    {
        if (index == LastIndex) {
            return Copy;
        }

        return Get_Bit(&BitArray[0], index);
    };

    int First_False() const
    {
        if (LastIndex != -1) {
            Fixup(-1);
        }

        int retval = First_False_Bit(&BitArray[0], BitArray.Length());

        if (retval < BitCount) {
            return retval;
        }

        return -1;
    }

    int First_True() const
    {
        if (LastIndex != -1)
            Fixup(-1);

        int retval = First_True_Bit(&BitArray[0], BitArray.Length());

        if (retval < BitCount) {
            return retval;
        }

        return -1;
    }

    const VectorClass<uint8_t> &Get_Bit_Array() { return BitArray; }

protected:
    void Fixup(int index = -1) const;
    int BitCount;
    mutable BOOL Copy;
    mutable int LastIndex;

    VectorClass<uint8_t> BitArray;
};

template<typename T>
int Pointer_Vector_Add(T *ptr, VectorClass<T *> &vec)
{
    int id = 0;
    BOOL foundspot = false;

    for (int index = 0; index < vec.Length(); index++) {
        if (vec[index] == nullptr) {
            id = index;
            foundspot = true;
            break;
        }
    }

    if (!foundspot) {
        id = vec.Length();
        vec.Resize((vec.Length() + 1) * 2);

        for (int index = id; index < vec.Length(); index++) {
            vec[index] = nullptr;
        }
    }

    vec[id] = ptr;

    return id;
}

template<typename T>
BOOL Pointer_Vector_Remove(const T *ptr, VectorClass<T *> &vec)
{
    int id = vec.ID((T *)ptr);

    if (id != -1) {
        vec[id] = nullptr;

        return true;
    }

    return false;
}

#endif
