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
#include "heap.h"
#include <captainslog.h>

template<class T>
class GamePtr
{
public:
    GamePtr() : m_ID(-1) {}
    GamePtr(T *ptr) : m_ID(ptr != nullptr ? ptr->Get_Heap_ID() : -1) {}
    GamePtr(const GamePtr &that) : m_ID(that.m_ID) {}
    GamePtr(const NoInitClass &noinit) {}
    ~GamePtr() { m_ID = -1; }

    T *operator->() const
    {
        captainslog_assert(g_Heap != nullptr);
        captainslog_assert(m_ID < g_Heap->Length());
        return Is_Valid() ? Get_Raw_Pointer() : nullptr;
    }

    T &operator*() const
    {
        captainslog_assert(g_Heap != nullptr);
        captainslog_assert(m_ID < g_Heap->Length());
        return *Get_Raw_Pointer();
    }

    operator T *() const
    {
        captainslog_assert(g_Heap != nullptr);
        captainslog_assert(m_ID < g_Heap->Length());
        return Has_Valid_ID() ? Get_Raw_Pointer() : nullptr;
    }

    operator T &() const
    {
        captainslog_assert(g_Heap != nullptr);
        captainslog_assert(m_ID < g_Heap->Length());
        return *Get_Raw_Pointer();
    }

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
        // Original takes the objects word for it for the ID.
        m_ID = that != nullptr ? that->Get_Heap_ID() : -1;
        return *this;
    }
    
    bool operator==(const GamePtr &that) const { return m_ID == that.m_ID; }
    bool operator!=(const GamePtr &that) const { return m_ID != that.m_ID; }

#ifdef COMPILER_WATCOM // nullptr checks are against ((void *)0) in watcom.
    //friend bool operator==(const GamePtr &a, void *b) { return b == nullptr && !a.Has_Valid_ID(); }
    //friend bool operator!=(const GamePtr &a, void *b) { return !(a == b); }
    //friend bool operator==(void *a, const GamePtr &b) { return b == a; }
    //friend bool operator!=(void *a, const GamePtr &b) { return b != a; }
#else
    friend bool operator==(const GamePtr &a, const std::nullptr_t b) { return !a.Has_Valid_ID(); }
    friend bool operator!=(const GamePtr &a, const std::nullptr_t b) { return a.Has_Valid_ID(); }
    friend bool operator==(const std::nullptr_t a, const GamePtr &b) { return !b.Has_Valid_ID(); }
    friend bool operator!=(const std::nullptr_t a, const GamePtr &b) { return b.Has_Valid_ID(); }
#endif

    friend bool operator==(const GamePtr &a, T *b) { return a == GamePtr(b); }
    friend bool operator!=(const GamePtr &a, T *b) { return !(a == b); }
    friend bool operator==(T *a, const GamePtr &b) { return b == a; }
    friend bool operator!=(T *a, const GamePtr &b) { return b != a; }

    bool Name_Equal(const T &that) const { return strcasecmp(Get_Raw_Pointer()->Get_Name(), that.Get_Name()) == 0; }
    bool Name_Not_Equal(const T &that) const { return strcasecmp(Get_Raw_Pointer()->Get_Name(), that.Get_Name()) != 0; }

    void Set_ID(intptr_t id) { m_ID = id; }
    intptr_t Get_ID() const { return m_ID; }
    void Invalidate_ID() { m_ID = -1; }

    static void Set_Heap(const FixedIHeapClass *heap) { g_Heap = heap; }
    static void Invalidate_Heap() { g_Heap = nullptr; }

private:
    T *Get_Raw_Pointer() const { return (T *)(&(*g_Heap)[m_ID]); }
    const T *Get_Raw_Const_Pointer() const { return (T *)(&(*g_Heap)[m_ID]); }

    bool Is_Valid() const { return g_Heap != nullptr && m_ID != -1; }
    bool Has_Valid_ID() const { return m_ID != -1; }

private:
    intptr_t m_ID;
    static const FixedIHeapClass *g_Heap;
};

#endif // GAMEPTR_H
