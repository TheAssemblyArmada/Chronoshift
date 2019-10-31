/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Straw stream class implementation reading data from a memory buffer.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef BUFFSTRAW_H
#define BUFFSTRAW_H

#include "always.h"
#include "buffer.h"
#include "straw.h"

class BufferStraw : public Straw
{
public:
    BufferStraw(void *buffer, int length) : m_Buffer((uint8_t *)buffer, length), m_Index(0) {}
    virtual ~BufferStraw() {}

    virtual int Get(void *buffer, int length) override;

    bool Is_Valid() { return m_Buffer.Is_Valid(); }

private:
    BufferClass m_Buffer;
    int m_Index;
};

#endif // BUFFSTRAW_H
