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
    PipeControl m_mode;
    int m_dataInBuffer;
    uint8_t *m_inBuffer;
    uint8_t *m_outBuffer;
    int m_blockSize;
    int m_maxBlockSize;
    int16_t m_compressedBytes;
    int16_t m_uncompressedBytes;
};

#endif // LZOPIPE_H
