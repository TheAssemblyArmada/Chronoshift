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
#include "pk.h"
#include "straw.h"

// RSA keys
#ifndef RAPP_STANDALONE
PKey &g_publicKey = *reinterpret_cast<PKey*>(0x00665F68);
#else
PKey g_publicKey(nullptr, nullptr);
#endif
PKey g_privateKey(nullptr, nullptr);

PKey::PKey(const void *exponent, const void *modulus)
{
    // init modulus and exponent
    MPMath::Init(m_modulus, 0, MAX_UNIT_PRECISION);
    MPMath::Init(m_exponent, 0, MAX_UNIT_PRECISION);

    // decode modulus and exponent?
    if (modulus != nullptr) {
        m_modulus.DER_Decode(static_cast<const uint8_t *>(modulus));
    }

    if (exponent != nullptr) {
        m_exponent.DER_Decode(static_cast<const uint8_t *>(modulus));
    }

    // Key length is the size of the Modulus because that determines the max
    // size of the data that can be encrypted per round.
    m_bitPrecision = Get_Count_Bits();
}

int PKey::Get_Count_Bits() const
{
    return MPMath::Count_Bits(m_modulus, MAX_UNIT_PRECISION) - 1;
}

int const PKey::Encode_Modulus(void *buffer) const
{
    if (buffer != nullptr) {
        return m_modulus.DER_Encode(static_cast<uint8_t *>(buffer));
    }

    return 0;
}

void PKey::Decode_Modulus(const void *modulus)
{
    m_modulus.DER_Decode(static_cast<const uint8_t *>(modulus));
    m_bitPrecision = Get_Count_Bits();
}

int const PKey::Encode_Exponent(void *buffer) const
{
    if (buffer != nullptr) {
        return m_exponent.DER_Encode(static_cast<uint8_t *>(buffer));
    }

    return 0;
}

void PKey::Decode_Exponent(const void *exponent)
{
    m_exponent.DER_Decode(static_cast<const uint8_t *>(exponent));
    m_bitPrecision = Get_Count_Bits();
}

void PKey::Generate(Straw &random, int bits, PKey &fastkey, PKey &slowkey)
{
    // The original game has functions to generate new encrytion keys, but we don't support that here.
    // It is possible support will be added later.
}

int const PKey::Encrypt(void *src, int size, void *dst) const
{
    mp_digit_u tmp2[64];
    mp_digit_u tmp3[64];

    int total = 0;
    int max_bytes = Key_Bytes();

    uint8_t *srcptr = static_cast<uint8_t *>(src);
    uint8_t *dstptr = static_cast<uint8_t *>(dst);

    while (size > 0) {
        // Stage data to a buffer.
        MPMath::Init(tmp3, 0, MAX_UNIT_PRECISION);
        memmove(tmp3, srcptr, size > max_bytes ? max_bytes : size);

        // Treat data as LE for PK encryption/decryption purposes
        MPMath::Move_LETOH(tmp3, tmp3, MAX_UNIT_PRECISION);
        MPMath::Init(tmp2, 0, MAX_UNIT_PRECISION);
        MPMath::Exponent_Mod(tmp2, tmp3, m_exponent, m_modulus, MAX_UNIT_PRECISION);
        MPMath::Move_HTOLE(tmp2, tmp2, MAX_UNIT_PRECISION);

        memmove(dstptr, tmp2, max_bytes + 1);

        size -= max_bytes;
        srcptr += max_bytes;
        dstptr += max_bytes + 1;
        total += max_bytes + 1;
    }

    return total;
}

int const PKey::Decrypt(void *src, int size, void *dst) const
{
    mp_digit_u tmp2[64];
    mp_digit_u tmp3[64];

    int total = 0;
    int max_bytes = Key_Bytes();

    uint8_t *srcptr = static_cast<uint8_t *>(src);
    uint8_t *dstptr = static_cast<uint8_t *>(dst);

    while (size >= max_bytes + 1) {
        // Stage data to a buffer.
        MPMath::Init(tmp3, 0, MAX_UNIT_PRECISION);
        memmove(tmp3, srcptr, max_bytes + 1);

        // Treat data as LE for PK encryption/decryption purposes
        MPMath::Move_LETOH(tmp3, tmp3, MAX_UNIT_PRECISION);
        MPMath::Init(tmp2, 0, MAX_UNIT_PRECISION);
        MPMath::Exponent_Mod(tmp2, tmp3, m_exponent, m_modulus, MAX_UNIT_PRECISION);
        MPMath::Move_HTOLE(tmp2, tmp2, MAX_UNIT_PRECISION);

        memmove(dstptr, tmp2, max_bytes);

        size -= max_bytes + 1;
        srcptr += max_bytes + 1;
        dstptr += max_bytes;
        total += max_bytes;
    }

    return total;
}
