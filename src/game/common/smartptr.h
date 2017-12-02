/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Thin wrapper around pointers.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef SMARTPTR_H
#define SMARTPTR_H

#include "always.h"
#include <cstddef> // for std::nullptr_t

class NoInitClass;

template<class T>
class SmartPtr
{
public:
    SmartPtr() : Pointer(nullptr) {}

    SmartPtr(T *realptr) : Pointer(realptr) {}

    SmartPtr(SmartPtr const &that) : Pointer(that.Pointer) {}

    SmartPtr(NoInitClass const &noinit) {}

    ~SmartPtr() { Release_Pointer(); }

    operator T *() const
    {
        // DEBUG_ASSERT_PRINT(Pointer != nullptr, "SmartPtr::operator*(): Pointer is NULL");
        return Pointer;
    }

    //
    operator T &() const
    {
        // DEBUG_ASSERT_PRINT(Pointer != nullptr, "SmartPtr::operator&(): Pointer is NULL");
        return Pointer;
    }

    //
    operator uintptr_t() const
    {
        // DEBUG_ASSERT_PRINT(Pointer != nullptr, "SmartPtr::uintptr(): Pointer is NULL");
        return (uintptr_t)Pointer;
    }

    //
    // Returns true if the smart ptr is not null.
    /*
    operator bool() const
    {
        //DEBUG_ASSERT_PRINT(Pointer != nullptr, "SmartPtr::uintptr(): Pointer is NULL");
        return Pointer != nullptr;
    }
    */

    // Returns true if the smart ptr is null.
    bool operator!() const
    {
        // DEBUG_ASSERT_PRINT(Pointer != nullptr, "SmartPtr::uintptr(): Pointer is NULL");
        return Pointer == nullptr;
    }

    T *operator->() const
    {
        // DEBUG_ASSERT_PRINT(Pointer != nullptr, "SmartPtr::operator->(): Pointer is NULL");
        return Pointer;
    }

    T &operator*() const
    {
        // DEBUG_ASSERT_PRINT(Pointer != nullptr, "SmartPtr::operator*(): Pointer is NULL");
        return *Pointer;
    }

    SmartPtr<T> operator++(int)
    {
        // DEBUG_ASSERT_PRINT(Pointer != nullptr, "SmartPtr::operator++(): Pointer is NULL");
        SmartPtr<T> temp = *this;
        ++Pointer;
        return temp;
    }

    SmartPtr<T> &operator++()
    {
        // DEBUG_ASSERT_PRINT(Pointer != nullptr, "SmartPtr::operator++(): Pointer is NULL");
        ++Pointer;
        return *this;
    }

    SmartPtr<T> operator--(int)
    {
        // DEBUG_ASSERT_PRINT(Pointer != nullptr, "SmartPtr::operator--(): Pointer is NULL");
        SmartPtr<T> temp = *this;
        --Pointer;
        return temp;
    }

    SmartPtr<T> &operator--()
    {
        // DEBUG_ASSERT_PRINT(Pointer != nullptr, "SmartPtr::operator--(): Pointer is NULL");
        --Pointer;
        return *this;
    }

    SmartPtr &operator=(SmartPtr const &that)
    {
        if (this != &that) {
            Pointer = that.Pointer;
        }

        return *this;
    }

    T *Get_Pointer() const
    {
        // DEBUG_ASSERT_PRINT(Pointer != nullptr, "SmartPtr::Get_Pointer(): Pointer is NULL");
        return Pointer;
    }

    // Release the pointer, returns the old.
    T *Release_Pointer()
    {
        T *old = Pointer;
        *((volatile T **)&Pointer) = nullptr;
        return old;
    }

private:
    T *Pointer;
};

// SmartPtr equality operators.
template<class T1, typename T2>
bool operator==(SmartPtr<T1> &left, T2 *right)
{
    return left.Get_Pointer() == right;
}

template<typename T1, class T2>
bool operator==(T1 *left, SmartPtr<T2> &right)
{
    return right.Get_Pointer() == left;
}

template<class T1, typename T2>
bool operator!=(SmartPtr<T1> &left, T2 *right)
{
    return left.Get_Pointer() != right;
}

template<typename T1, class T2>
bool operator!=(T1 *left, SmartPtr<T2> &right)
{
    return right.Get_Pointer() != left;
}

template<class T1, typename T2>
bool operator==(SmartPtr<T1> left, SmartPtr<T2> right)
{
    return left.Get_Pointer() == right.Get_Pointer();
}

template<class T1, typename T2>
bool operator!=(SmartPtr<T1> left, SmartPtr<T2> right)
{
    return left.Get_Pointer() != right.Get_Pointer();
}

#ifndef COMPILER_WATCOM
// "== nullptr"
template<class T>
bool operator==(SmartPtr<T> &left, std::nullptr_t right)
{
    return left.Get_Pointer() == right;
}

// "!= nullptr"
template<class T>
bool operator!=(SmartPtr<T> &left, std::nullptr_t right)
{
    return left.Get_Pointer() != right;
}
#endif

template<class T>
void Swap_Pointers(SmartPtr<T> &left, SmartPtr<T> &right)
{
    SmartPtr<T> tmp = left;
    left = right;
    right = tmp;
}

#endif // SMARTPTR_H
