/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Wrapper around heap allocation to make heap IDs behave like pointers.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef GAMEPTR_H
#define GAMEPTR_H

#include "always.h"
#include "gamedebug.h"
#include "heap.h"

class NoInitClass;

template<class T>
class GamePtr
{
public:
    GamePtr() : ID(-1) {}
    GamePtr(T *ptr) : ID(-1)
    {
        if (ptr != nullptr) {
            ID = ptr->Get_Heap_ID();
        }
    }
    GamePtr(const GamePtr &that) {}
    GamePtr(const NoInitClass &noinit) {}
    ~GamePtr() { ID = -1; }

    //<RA_TODO>, these two might be the wrong way around.  code in TEventClass::Build_INI_Entry() is not correct
    T *operator->() const
    {
        DEBUG_ASSERT(Heap != nullptr && ID < Heap->Length());
        return !Is_Valid() ? nullptr : (T *)(&(*Heap)[ID]);
    }

    //<RA_TODO>, this might not be reference? code in TEventClass::Build_INI_Entry() is not correct
    T &operator*() const
    {
        DEBUG_ASSERT(Heap != nullptr && ID < Heap->Length());
        return *(T *)(&(*Heap)[ID]);
    }

    // Fairly certain this works the same way as operator->
    operator T *() const
    {
        DEBUG_ASSERT(Heap != nullptr && ID < Heap->Length());
        return !Is_Valid() ? nullptr : (T *)(&(*Heap)[ID]);
    }

    GamePtr &operator=(const GamePtr &that)
    {
        if (this != &that) {
            ID = that.ID;
        }
        return *this;
    }

    GamePtr &operator=(const T *that)
    {
        ID = -1;
        if (that != nullptr) {
            ID = Heap->ID(that);
        }
        return *this;
    }

    bool operator==(const T &that) const
    {
        return strcasecmp((*this).Get_Name(), that.Get_Name()) == 0;
    }
    
    bool operator!=(const T &that) const
    {
        return strcasecmp((*this).Get_Name(), that.Get_Name()) != 0;
    }

    bool Is_Valid() const { return Heap != nullptr && ID != -1; }
    bool Has_Valid_ID() const { return ID != -1; }

    bool Invalidate() { ID = -1; }

private:
    int32_t ID;
    static FixedIHeapClass *Heap;
};

template<class T1, class T2>
bool operator==(const GamePtr<T1> &a, const T2 *b)
{
    return static_cast<T1*>(a) == b;
}

template<class T1, class T2>
bool operator==(const T1 *a, const GamePtr<T2> &b)
{
    return static_cast<T2*>(b) == a;
}

template<class T1, class T2>
bool operator!=(const GamePtr<T1> &a, const T2 *b)
{
    return static_cast<T1*>(a) == b;
}

template<class T1, class T2>
bool operator!=(const T1 *a, const GamePtr<T2> &b)
{
    return static_cast<T2*>(b) == a;
}

#endif // GAMEPTR_H
