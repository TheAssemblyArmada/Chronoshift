/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Straw interface for reading LZO encoded data.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef LZOSTRAW_H
#define LZOSTRAW_H

#include "always.h"
#include "straw.h"

class LZOStraw : public Straw
{
public:
    LZOStraw(StrawControl mode, int size = 0);
    virtual ~LZOStraw();

    virtual int Get(void *buffer, int length) override;

#ifndef RAPP_STANDALONE
    static void Hook_Me();
#endif

private:
    StrawControl m_mode;
    int m_carryover;
    uint8_t *m_inBuffer;
    uint8_t *m_outBuffer;
    int m_blockSize;
    int m_compressedBlockSize;
    int16_t m_compressedBytes;
    int16_t m_uncompressedBytes;
};

#ifndef RAPP_STANDALONE
#include "hooker.h"

inline void LZOStraw::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x005D5BF4, *LZOStraw::Get);
#endif
}
#endif

#endif // LZOSTRAW_H
