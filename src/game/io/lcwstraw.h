/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Straw interface for reading LCW encoded data.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef LCWSTRAW_H
#define LCWSTRAW_H

#include "always.h"
#include "straw.h"

#define LCW_DEFAULT_SIZE 8192

class LCWStraw : public Straw
{
public:
    LCWStraw(StrawControl mode = STRAW_DECODE, int size = LCW_DEFAULT_SIZE);
    virtual ~LCWStraw();

    virtual int Get(void *buffer, int length) override;

private:
    StrawControl m_Mode;
    int m_Carryover;
    uint8_t *m_InBuffer;
    uint8_t *m_OutBuffer;
    int m_BlockSize;
    int m_CompressedBlockSize;
    int16_t m_CompressedBytes;
    int16_t m_UncompressedBytes;
};

#endif // LCWSTRAW_H
