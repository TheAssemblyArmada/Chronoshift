/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Pipe interface for writing LCW encoded data.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef LCWPIPE_H
#define LCWPIPE_H

#include "always.h"
#include "pipe.h"

#define LCW_DEFAULT_SIZE 8192

class LCWPipe : public Pipe
{
public:
    LCWPipe(PipeControl mode = PIPE_COMPRESS, int size = LCW_DEFAULT_SIZE);
    virtual ~LCWPipe();

    virtual int Flush() override;
    virtual int Put(const void *buffer, int length) override;

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

#endif // LCWPIPE_H
