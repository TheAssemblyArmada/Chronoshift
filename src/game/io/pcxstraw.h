/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Straw stream interface for reading PCX RLE compressed data.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef PCXSTRAW_H
#define PCXSTRAW_H

#include "always.h"
#include "straw.h"

#define RLE_DEFAULT_LINE_LENGTH 640

class PCXStraw : public Straw
{
public:
    PCXStraw(StrawControl mode, unsigned pitch = RLE_DEFAULT_LINE_LENGTH, unsigned width = RLE_DEFAULT_LINE_LENGTH);
    virtual ~PCXStraw();

    virtual int Get(void *buffer, int length) override;

private:
    StrawControl m_Mode;
    int m_LinePitch;
    int m_LineWidth;
    int m_Carryover;
    int m_Remaining;
    uint8_t *m_InBuffer;
    uint8_t *m_OutBuffer;
};

#endif // _RLESTRAW_H_
