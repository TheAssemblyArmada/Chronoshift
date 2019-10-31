/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Pipe stream class implementation writing data to a memory buffer.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef BUFFPIPE_H
#define BUFFPIPE_H

#include "always.h"
#include "buffer.h"
#include "pipe.h"

class BufferPipe : public Pipe
{
public:
    BufferPipe(void *buffer, int length) : m_Buffer(buffer, length), m_Index(0) {}
    virtual ~BufferPipe() {}

    virtual int Put(const void *buffer, int length) override;

    bool Is_Valid() { return m_Buffer.Is_Valid(); }

private:
    BufferClass m_Buffer;
    int m_Index;
};

#endif // BUFFPIPE_H
