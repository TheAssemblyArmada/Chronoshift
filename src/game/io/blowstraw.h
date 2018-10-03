/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Straw stream class implementation reading data processed with the blowfish algorithm.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef BLOWSTRAW_H
#define BLOWSTRAW_H

#include "blowfish.h"
#include "straw.h"

class BlowStraw : public Straw
{
public:
    BlowStraw(StrawControl mode = STRAW_DECRYPT) : m_blowFish(nullptr), m_carryOver(0), m_mode(mode) {}
    virtual ~BlowStraw();

    virtual int Get(void *buffer, int length) override;

    void Key(void *key, int key_size);

private:
    BlowfishEngine *m_blowFish;
    char m_currentBlock[BF_BLOCKSIZE];
    int m_carryOver;
    StrawControl m_mode;
};

#endif // BLOWSTRAW_H
