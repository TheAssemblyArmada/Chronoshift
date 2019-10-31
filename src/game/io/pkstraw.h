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
#include "rndstraw.h"
#include "straw.h"

class PKey;

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
    BOOL m_ChangeKey;
    Straw &m_CryptRandom;
    BlowStraw m_BlowFish;
    StrawControl m_Mode;
    PKey *m_RSAKey;
    char m_CryptoBuffer[256];
    int m_EncryptedKeyLength;
    int m_Carryover;
};

#endif // PKSTRAW_H
