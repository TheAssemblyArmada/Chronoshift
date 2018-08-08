////////////////////////////////////////////////////////////////////////////////
//                            --  REDALERT++ --                               //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: Redalert++
//
//          File:: SHASTRAW.H
//
//        Author:: CCHyper
//
//  Contributors:: OmniBlade
//
//   Description:: Stream implementation that hashes data with SHA1 as it
//                 passes through.
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

#ifndef SHASTRAW_H
#define SHASTRAW_H

#include "always.h"
#include "sha.h"
#include "straw.h"
#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
#endif

class SHAStraw : public Straw
{
public:
    SHAStraw() : m_sha1() {}
    virtual ~SHAStraw() {}

    virtual int Get(void *buffer, int length) override;

    int Result(void *data);

#ifndef CHRONOSHIFT_STANDALONE
    static int Hook_Result(SHAStraw *ptr, void *data);
    static int Hook_Get(SHAStraw *ptr, void *source, int length);
    static void Hook_Me();
#endif

protected:
    SHAEngine m_sha1; // A instance of the SHA-1 Engine class.

};

#ifndef CHRONOSHIFT_STANDALONE
inline void SHAStraw::Hook_Me()
{
    Hook_Function(0x005D5B04, &Hook_Result);
    Hook_Function(0x005D5AD0, &Hook_Get);
}
#endif

#endif // SHASTRAW_H
