/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Straw interface for reading Base64 encoded data.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef B64STRAW_H
#define B64STRAW_H

#include "always.h"
#include "straw.h"

class Base64Straw : public Straw
{
public:
    Base64Straw(StrawControl mode = STRAW_DECODE) : m_Mode(mode), m_Counter(0) {}
    virtual ~Base64Straw() {}

    virtual int Get(void *dest, int slen) override;

private:
    StrawControl m_Mode;
    int m_Counter;
    char m_CBuffer[4];
    char m_PBuffer[3];
};

#endif // B64STRAW_H
