/**
* @file
*
* @Author CCHyper, OmniBlade
*
* @brief RSA Encryption engine.
*
* @copyright Redalert++ is free software: you can redistribute it and/or
*            modify it under the terms of the GNU General Public License
*            as published by the Free Software Foundation, either version
*            2 of the License, or (at your option) any later version.
*
*            A full copy of the GNU General Public License can be found in
*            LICENSE
*/
#pragma once

#ifndef PK_H
#define PK_H

#include "always.h"
#include "int.h"

class Straw;

class PKey
{
public:
    PKey() : m_modulus(), m_exponent(), m_bitPrecision(0) {}
    PKey(const void *exponent, const void *modulus);
    PKey(PKey const &that) : m_modulus(that.m_modulus), m_exponent(that.m_exponent), m_bitPrecision(that.m_bitPrecision) {}
    ~PKey() {};

    PKey &operator=(PKey const &that);

    int Encode_Modulus(void *buffer) const;
    int Encode_Exponent(void *buffer) const;
    void Decode_Modulus(const void *buffer);
    void Decode_Exponent(const void *buffer);
    int Encrypt(void *source, int slen, void *dest) const;
    int Decrypt(void *source, int slen, void *dest) const;
    int Get_Count_Bits() const;
    int Get_Key_Length() const { return m_bitPrecision; }
    int Key_Bytes() const { return (m_bitPrecision - 1) / 8; }

    static void Generate(Straw &random, int bits, PKey &fastkey, PKey &slowkey);

private:
    Int<MAX_UNIT_PRECISION> m_modulus;
    Int<MAX_UNIT_PRECISION> m_exponent;
    int m_bitPrecision;
};

inline PKey &PKey::operator=(PKey const &that)
{
    if (this != &that) {
        memcpy(&m_modulus, &that.m_modulus, sizeof(m_modulus));
        memcpy(&m_exponent, &that.m_exponent, sizeof(m_exponent));
        m_bitPrecision = that.m_bitPrecision;
    }

    return *this;
}

// RSA keys
#ifndef RAPP_STANDALONE
extern PKey &g_publicKey;
#else
extern PKey g_publicKey;
#endif
extern PKey g_privateKey;

#endif // PK_H
