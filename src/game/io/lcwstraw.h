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
    StrawControl m_mode;
    int m_carryOver;
    uint8_t *m_inBuffer;
    uint8_t *m_outBuffer;
    int m_blockSize;
    int m_compressedBlockSize;
    int16_t m_compressedBytes;
    int16_t m_uncompressedBytes;
};

#endif // LCWSTRAW_H
