////////////////////////////////////////////////////////////////////////////////
//                            --  REDALERT++ --                               //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: Redalert++
//
//          File:: STRAW.H
//
//        Author:: CCHyper
//
//  Contributors:: OmniBlade
//
//   Description:: Base fetching stream class.
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
#pragma once

#ifndef STRAW_H
#define STRAW_H

#include "always.h"
#ifndef RAPP_STANDALONE
#include "hooker.h"
#endif

enum StrawControl
{
    STRAW_DECODE = 1,
    STRAW_DECRYPT = 1,
    STRAW_UNCOMPRESS = 1,
    STRAW_ENCODE = 0,
    STRAW_ENCRYPT = 0,
    STRAW_COMPRESS = 0,
};

/*
enum Base64Straw::CodeControl {
  ENCODE = 0x0,
  DECODE = 0x1,
};

enum Base64Pipe::CodeControl {
  ENCODE = 0x0,
  DECODE = 0x1,
};
*/

class Straw
{
public:
    Straw() : m_chainTo(nullptr), m_chainFrom(nullptr) {}
    Straw(Straw const &that) : m_chainTo(that.m_chainTo), m_chainFrom(that.m_chainFrom) {}
    virtual ~Straw();

    Straw &operator=(Straw &that);

    virtual void Get_From(Straw *straw);
    virtual int Get(void *source, int length);

#ifndef RAPP_STANDALONE
    static void Hook_Get_From(Straw *ptr, Straw *straw);
    static int Hook_Get(Straw *ptr, void *source, int length);
    static void Hook_Me();
#endif

protected:
    Straw *m_chainTo;
    Straw *m_chainFrom;

};

inline Straw &Straw::operator=(Straw &that)
{
    if (this != &that) {
        m_chainTo = that.m_chainTo;
        m_chainFrom = that.m_chainFrom;
    }

    return *this;
}

#ifndef RAPP_STANDALONE
inline void Straw::Hook_Me()
{
    Hook_Function((void*)0x005C26D8, (void*)&Hook_Get);
    Hook_Function((void*)0x005C268C, (void*)&Hook_Get_From);
}
#endif

#endif // STRAW_H
