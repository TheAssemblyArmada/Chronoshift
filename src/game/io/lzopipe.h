/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Pipe interface for writing LZO encoded data.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef LZOPIPE_H
#define LZOPIPE_H

#include "always.h"
#include "pipe.h"

class LZOPipe : public Pipe
{
public:
    LZOPipe(PipeControl mode, int size = 0);
    virtual ~LZOPipe();

    virtual int Flush();
    virtual int Put(const void *source, int length);

private:
    PipeControl m_Mode;
    int m_DataInBuffer;
    uint8_t *m_InBuffer;
    uint8_t *m_OutBuffer;
    int m_BlockSize;
    int m_MaxBlockSize;
    int16_t m_CompressedBytes;
    int16_t m_UncompressedBytes;
};

#endif // LZOPIPE_H
