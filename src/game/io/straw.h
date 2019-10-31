/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base interface for fetching a stream of data.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef STRAW_H
#define STRAW_H

#include "always.h"

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
    Straw() : m_ChainTo(nullptr), m_ChainFrom(nullptr) {}
    Straw(Straw const &that) : m_ChainTo(that.m_ChainTo), m_ChainFrom(that.m_ChainFrom) {}
    virtual ~Straw();

    Straw &operator=(Straw &that);

    virtual void Get_From(Straw *straw);
    virtual int Get(void *source, int length);

protected:
    Straw *m_ChainTo;
    Straw *m_ChainFrom;

};

inline Straw &Straw::operator=(Straw &that)
{
    if (this != &that) {
        m_ChainTo = that.m_ChainTo;
        m_ChainFrom = that.m_ChainFrom;
    }

    return *this;
}

#endif // STRAW_H
