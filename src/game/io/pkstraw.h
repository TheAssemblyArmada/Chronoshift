/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Straw that processes the data with RSA and Blowfish as it reads it.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef PKSTRAW_H
#define PKSTRAW_H

#include "always.h"
#include "blowstraw.h"
#include "pk.h"
#include "rndstraw.h"
#include "straw.h"

class PKStraw : public Straw
{
public:
    PKStraw(StrawControl mode, Straw &rstraw);
    virtual ~PKStraw() {};

    virtual int Get(void *buffer, int length) override;
    virtual void Get_From(Straw *straw) override;

    void Key(PKey *key);

    int Encrypted_Key_Length() const;
    int Plain_Key_Length() const;

private:
    BOOL m_changeKey;
    Straw &m_cryptRandom;
    BlowStraw m_blowFish;
    StrawControl m_mode;
    PKey *m_rsaKey;
    char m_cryptoBuffer[256];
    int m_encryptedKeyLength;
    int m_carryOver;
};

#endif // PKSTRAW_H
