////////////////////////////////////////////////////////////////////////////////
//                            --  REDALERT++ --                               //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: Redalert++
//
//          File:: PIPE.H
//
//        Author:: CCHyper
//
//  Contributors:: OmniBlade
//
//   Description:: Base putting stream class.
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

#ifndef PIPE_H
#define PIPE_H

#include "always.h"

enum PipeControl
{
    PIPE_DECODE = 0,
    PIPE_DECRYPT = 0,
    PIPE_UNCOMPRESS = 0,
    PIPE_ENCODE = 1,
    PIPE_ENCRYPT = 1,
    PIPE_COMPRESS = 1
};

class Pipe
{
public:
    Pipe() : m_chainTo(nullptr), m_chainFrom(nullptr) {}
    Pipe(Pipe const &that) : m_chainTo(that.m_chainTo), m_chainFrom(that.m_chainFrom) {}
    virtual ~Pipe();

    Pipe &operator=(Pipe &that);

    virtual int Flush();
    virtual int End();
    virtual void Put_To(Pipe *pipe);
    virtual int Put(void const *source, int length);

protected:
    Pipe *m_chainTo;
    Pipe *m_chainFrom;
};

inline Pipe &Pipe::operator=(Pipe &that)
{
    if (this != &that) {
        m_chainTo = that.m_chainTo;
        m_chainFrom = that.m_chainFrom;
    }
    return *this;
}

#endif // PIPE_H
