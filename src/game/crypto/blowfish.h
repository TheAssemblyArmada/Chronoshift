/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Blowfish encryption engine.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#ifndef BLOWFISH_H
#define BLOWFISH_H

#include "always.h"

#define BF_ENCRYPT 1
#define BF_DECRYPT 0
#define BF_BLOCKSIZE 8 // Size of encryption block.
#define BF_ROUNDS 16
#define BF_DEF_KEY_LENGTH 16
#define BF_MIN_KEY_LENGTH 4
#define BF_MAX_KEY_LENGTH 56 // 448 bits, Maximum key length allowed.

class BlowfishEngine
{
public:
    BlowfishEngine(void *key = nullptr, int length = 0);
    ~BlowfishEngine();

    void Submit_Key(void *key, int length);
    int Encrypt(void *plaintext, int length, void *cyphertext);
    int Decrypt(void *cyphertext, int length, void *plaintext);

private:
    void Process_Block(void *input, void *output, uint32_t *pbox);
    void Sub_Key_Encrypt(uint32_t *input, uint32_t *output);

private:
    BOOL m_hasKey;
    uint32_t m_pBoxEnc[BF_ROUNDS + 2];
    uint32_t m_pBoxDec[BF_ROUNDS + 2];
    uint32_t m_sBox[4 * 256];

    static const uint32_t Blowfish_p_init[BF_ROUNDS + 2];
    static const uint32_t Blowfish_s_init[4][256];
};

#endif // BLOWFISH_H
