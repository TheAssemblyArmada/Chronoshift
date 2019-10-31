/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base class for output stream classes.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef PIPE_H
#define PIPE_H

#include "always.h"

enum PipeControl
{
    PIPE_DECODE = 1,
    PIPE_DECRYPT = 1,
    PIPE_UNCOMPRESS = 1,
    PIPE_ENCODE = 0,
    PIPE_ENCRYPT = 0,
    PIPE_COMPRESS = 0
};

class Pipe
{
public:
    Pipe() : m_ChainTo(nullptr), m_ChainFrom(nullptr) {}
    Pipe(Pipe const &that) : m_ChainTo(that.m_ChainTo), m_ChainFrom(that.m_ChainFrom) {}
    virtual ~Pipe();

    Pipe &operator=(Pipe &that);

    virtual int Flush();
    virtual int End();
    virtual void Put_To(Pipe *pipe);
    virtual int Put(const void *source, int length);

protected:
    Pipe *m_ChainTo;
    Pipe *m_ChainFrom;
};

inline Pipe &Pipe::operator=(Pipe &that)
{
    if (this != &that) {
        m_ChainTo = that.m_ChainTo;
        m_ChainFrom = that.m_ChainFrom;
    }
    return *this;
}

#endif // PIPE_H
