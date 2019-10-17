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
    Base64Straw(StrawControl mode = STRAW_DECODE) : m_mode(mode), m_counter(0) {}
    virtual ~Base64Straw() {}

    virtual int Get(void *dest, int slen) override;

private:
    StrawControl m_mode;
    int m_counter;
    char m_cBuffer[4];
    char m_pBuffer[3];
};

#endif // B64STRAW_H
