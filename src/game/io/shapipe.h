/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Output stream implementation that hashes data with SHA1 as it passes it through.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef SHAPIPE_H
#define SHAPIPE_H

#include "always.h"
#include "pipe.h"
#include "sha.h"

class SHAPipe : public Pipe
{
public:
    SHAPipe() : Pipe(), m_sha1() {}
    virtual ~SHAPipe() {}

    virtual int Put(const void *buffer, int length) override;
    int const Result(void *data) { return m_sha1.Result(data); }

protected:
    SHAEngine m_sha1;

};

#endif // SHAPIPE_H
