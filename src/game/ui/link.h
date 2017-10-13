/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Base class for UI elements linking them together for processing.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef LINK_H
#define LINK_H

#include "always.h"

class LinkClass
{
public:
    LinkClass() : Next(nullptr), Prev(nullptr) {}
    LinkClass(LinkClass &that) : Next(nullptr), Prev(nullptr) { Add(that); }
    virtual ~LinkClass() { Remove(); }

    virtual LinkClass *Get_Next() const { return Next; }
    virtual LinkClass *Get_Prev() const { return Prev; }
    virtual LinkClass &Add(LinkClass &that);
    virtual LinkClass &Add_Tail(LinkClass &that);
    virtual LinkClass &Add_Head(LinkClass &that);
    virtual LinkClass &Head_Of_List();
    virtual LinkClass &Tail_Of_List();
    virtual void Zap();
    virtual LinkClass *Remove();

    LinkClass &operator=(LinkClass &that);
    void Unlink();
    int Count();

protected:
    LinkClass *Next;
    LinkClass *Prev;
};


inline LinkClass &LinkClass::operator=(LinkClass &that)
{
    if (this != &that) {
        Remove();
        Add(that);
    }

    return *this;
}

inline int LinkClass::Count()
{
    int count = 0;

    for (LinkClass *next = Get_Next(); next != nullptr; next = next->Get_Next()) {
        ++count;
    }

    return count;
}

#endif // LINK_H
