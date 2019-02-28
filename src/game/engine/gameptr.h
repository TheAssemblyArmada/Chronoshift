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

template<class T>
class GamePtr
{
public:
    GamePtr() : m_ID(-1) {}
    GamePtr(T *ptr) : m_ID(ptr != nullptr ? m_ID = ptr->Get_Heap_ID() : -1) {}
    GamePtr(const GamePtr &that) : m_ID(that.m_ID) {}
    GamePtr(const NoInitClass &noinit) {}
    ~GamePtr() { m_ID = -1; }

    T *operator->() const
    {
        DEBUG_ASSERT(g_Heap != nullptr);
        DEBUG_ASSERT(m_ID < g_Heap->Length());
        return Is_Valid() ? Get_Raw_Pointer() : nullptr;
    }

    T &operator*() const
    {
        DEBUG_ASSERT(g_Heap != nullptr);
        DEBUG_ASSERT(m_ID < g_Heap->Length());
        return *Get_Raw_Pointer();
    }

    operator T *() const
    {
        DEBUG_ASSERT(g_Heap != nullptr);
        DEBUG_ASSERT(m_ID < g_Heap->Length());
        return Has_Valid_ID() ? Get_Raw_Pointer() : nullptr;
    }

    operator T &() const
    {
        DEBUG_ASSERT(g_Heap != nullptr);
        DEBUG_ASSERT(m_ID < g_Heap->Length());
        return *Get_Raw_Pointer();
    }

    operator uintptr_t() const
    {
        DEBUG_ASSERT(g_Heap != nullptr);
        DEBUG_ASSERT(m_ID < g_Heap->Length());
        return Is_Valid() ? Get_Raw_Pointer() : nullptr;
    }

    operator bool() const { return Is_Valid(); }

    bool operator!() const { return !Is_Valid(); }

    GamePtr &operator=(const GamePtr &that)
    {
        if (this != &that) {
            m_ID = that.m_ID;
        }
        return *this;
    }

    GamePtr &operator=(const T *that)
    {
        m_ID = that != nullptr ? g_Heap->ID(that) : -1;
        return *this;
    }

    bool operator==(const T &that) const { return Get_Raw_Pointer() == &that; }

    bool operator!=(const T &that) const { return Get_Raw_Pointer() != &that; }

    bool Name_Equal(const T &that) const { return strcasecmp(Get_Raw_Pointer()->Get_Name(), that.Get_Name()) == 0; }

    bool Name_Not_Equal(const T &that) const { return strcasecmp(Get_Raw_Pointer()->Get_Name(), that.Get_Name()) != 0; }

    bool Is_Valid() const { return g_Heap != nullptr && m_ID != -1; }
    bool Has_Valid_ID() const { return m_ID != -1; }
    void Invalidate() { m_ID = -1; }

protected:
    T *Get_Raw_Pointer() const { return (T *)(&(*g_Heap)[m_ID]); }
    const T *Get_Raw_Const_Pointer() const { return (T *)(&(*g_Heap)[m_ID]); }

private:
    int32_t m_ID;
    static FixedIHeapClass *const g_Heap;
};

template<class T1, class T2>
bool operator==(const GamePtr<T1> &a, const T2 *b)
{
    return static_cast<T1 *>(a) == b;
}

template<class T1, class T2>
bool operator==(const T1 *a, const GamePtr<T2> &b)
{
    return static_cast<T2 *>(b) == a;
}

template<class T1, class T2>
bool operator!=(const GamePtr<T1> &a, const T2 *b)
{
    return static_cast<T1 *>(a) == b;
}

template<class T1, class T2>
bool operator!=(const T1 *a, const GamePtr<T2> &b)
{
    return static_cast<T2 *>(b) == a;
}

#endif // GAMEPTR_H
