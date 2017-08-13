////////////////////////////////////////////////////////////////////////////////
//                            --  REDALERT++ --                               //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: Redalert++
//
//          File:: SHA.H
//
//        Author:: CCHyper
//
//  Contributors:: OmniBlade
//
//   Description:: SHA1 Hashing engine.
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

#ifndef SHA_H
#define SHA_H

#include "always.h"

#define SHA_DIGEST_LENGTH 20
#define SHA_BLOCK_LENGTH 64

class SHAEngine
{
public:
    struct SHADigest
    {
        uint32_t H0;
        uint32_t H1;
        uint32_t H2;
        uint32_t H3;
        uint32_t H4;
    };

    SHAEngine();
    ~SHAEngine();

    void Hash(const void *input, int length);
    int Result(void *output);
    int Print_Result(char *output);

private:
    void Process_Partial(const void *&data, int &length);
    void Process_Block(const void *data, SHADigest &digest) const;
    void Print(const void *buffer, char *stringbuff);

private:
    BOOL m_computed;
    SHADigest m_finalDigest;
    SHADigest m_digest;
    uint32_t m_processedBytes;
    uint32_t m_unprocessedBytes;
    char m_messageBlock[SHA_BLOCK_LENGTH]; // this is also knows as 'bytes' in other implimentions
};

#endif // SHA_H
