/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Pipe interface for writing Base64 encoded data.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef BASE64PIPE_H
#define BASE64PIPE_H

#include "always.h"
#include "pipe.h"

class Base64Pipe : public Pipe
{
public:
    Base64Pipe(PipeControl control = PIPE_ENCODE) : m_Mode(control), m_Counter(0) {}
    virtual ~Base64Pipe() {}

    virtual int Flush() override;
    virtual int Put(const void *source, int slen) override;

private:
    PipeControl m_Mode;
    int m_Counter;
    char m_CBuffer[4];
    char m_PBuffer[3];
};

#endif // BASE64PIPE_H
