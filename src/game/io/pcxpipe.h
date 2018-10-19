/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Pipe stream interface for writing PCX RLE compressed data.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef PCXPIPE_H
#define PCXPIPE_H

#include "always.h"
#include "pipe.h"

#define RLE_DEFAULT_SIZE 640

class PCXPipe : public Pipe
{
public:
    PCXPipe(PipeControl mode, int size = RLE_DEFAULT_SIZE);
    virtual ~PCXPipe();
    virtual int Flush() override;
    virtual int Put(const void *buffer, int length) override;

private:
    PipeControl Mode;
    int LineLength;
    int Carryover;
    int Remaining;
    uint8_t *InBuffer;
    uint8_t *OutBuffer;
};

#endif // PCXPIPE_H
