/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief RSA Encryption engine.
 *
 * @copyright RedAlert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "pk.h"
#include "straw.h"

#include <cstdlib>

using std::rand;

// RSA keys
#ifndef CHRONOSHIFT_STANDALONE
PKey &g_publicKey = *reinterpret_cast<PKey *>(0x00665F68);
#else
PKey g_publicKey;
#endif
PKey g_privateKey;

PKey::PKey(const void *exponent, const void *modulus) : m_modulus(), m_exponent(), m_bitPrecision(0)
{
    DEBUG_ASSERT(exponent != nullptr);
    DEBUG_ASSERT(modulus != nullptr);

    Decode_Modulus(modulus);
    Decode_Exponent(exponent);
}

int PKey::Get_Count_Bits() const
{
    return MPMath::Count_Bits(m_modulus, MAX_UNIT_PRECISION) - 1;
}

int PKey::Encode_Modulus(void *buffer) const
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

int PKey::Encode_Exponent(void *buffer) const
{
    if (buffer != nullptr) {
        return m_exponent.DER_Encode(static_cast<uint8_t *>(buffer));
    }

    return 0;
}

void PKey::Decode_Exponent(const void *exponent)
{
    m_exponent.DER_Decode(static_cast<const uint8_t *>(exponent));
}

Int<MAX_UNIT_PRECISION> PKey::Generate_Prime(Straw &random, int bits)
{
    Int<MAX_UNIT_PRECISION> min_val = Int<MAX_UNIT_PRECISION>(1) << bits - 2;
    Int<MAX_UNIT_PRECISION> max_val = (Int<MAX_UNIT_PRECISION>(1) << bits - 1) - 1;
    Int<MAX_UNIT_PRECISION> prime;
    Int<MAX_UNIT_PRECISION> ret_val;

    do {
        prime.Randomize(random, min_val, max_val);
        ret_val = prime * 2 + 1;
        RemainderTable<Int<MAX_UNIT_PRECISION> > prime_table(prime);
        RemainderTable<Int<MAX_UNIT_PRECISION> > ret_table(ret_val);

        while (prime_table.Has_Zero() || ret_table.Has_Zero() || !prime.Is_Prime() || !ret_val.Is_Prime()) {
            prime += Int<MAX_UNIT_PRECISION>(2);
            ret_val += Int<MAX_UNIT_PRECISION>(4);

            if (prime > max_val) {
                break;
            }

            prime_table.Increment(2);
            ret_table.Increment(4);
        }
    } while (prime > max_val);

    return ret_val;
}

void PKey::Generate(Straw &random, int bits, PKey &fastkey, PKey &slowkey)
{
    uint8_t before[256];
    uint8_t after[256];

    // Generate a key pair and test encryption round trips on random data. If not, generate again.
    do {
        // Key generation
        Int<MAX_UNIT_PRECISION> multiplicand = Generate_Prime(random, bits);
        Int<MAX_UNIT_PRECISION> multiplier = Generate_Prime(random, bits);
        Int<MAX_UNIT_PRECISION> e = Int<MAX_UNIT_PRECISION>(Public_Exponent());
        Int<MAX_UNIT_PRECISION> product = multiplicand * multiplier;
        fastkey.m_exponent = e;
        fastkey.m_modulus = product;
        fastkey.m_bitPrecision = product.Bit_Count() - 1;
        slowkey.m_exponent = e.Inverse((multiplicand - 1) * (multiplier - 1));
        slowkey.m_modulus = product;
        slowkey.m_bitPrecision = product.Bit_Count() - 1;

        // Generate some random data to test the keys on.
        for (int i = 0; i < fastkey.Clear_Text_Block_Size(); ++i) {
            before[i] = rand();
        }

        // Test round tripping data.
        fastkey.Encrypt(before, fastkey.Clear_Text_Block_Size(), after);
        slowkey.Decrypt(after, slowkey.Cipher_Text_Block_Size(), after);
    } while (memcmp(before, after, fastkey.Clear_Text_Block_Size()) != 0);
}

int PKey::Encrypt(void *src, int size, void *dst) const
{
    int total = 0;
    int max_bytes = Key_Bytes();

    uint8_t *srcptr = static_cast<uint8_t *>(src);
    uint8_t *dstptr = static_cast<uint8_t *>(dst);

    while (size > 0) {
        // Stage data to a buffer.
        Int<MAX_UNIT_PRECISION> data;
        memmove(data, srcptr, size > max_bytes ? max_bytes : size);

        // Treat data as LE for PK encryption/decryption purposes
        data.Host_To_LE();
        Int<MAX_UNIT_PRECISION> result = data.Exponential_Modulation(m_exponent, m_modulus);
        result.LE_To_Host();

        memmove(dstptr, result, max_bytes + 1);

        size -= max_bytes;
        srcptr += max_bytes;
        dstptr += max_bytes + 1;
        total += max_bytes + 1;
    }

    return total;
}

int PKey::Decrypt(void *src, int size, void *dst) const
{
    int total = 0;
    int max_bytes = Key_Bytes();

    uint8_t *srcptr = static_cast<uint8_t *>(src);
    uint8_t *dstptr = static_cast<uint8_t *>(dst);

    while (size >= max_bytes + 1) {
        // Stage data to a buffer.
        Int<MAX_UNIT_PRECISION> data;
        memmove(data, srcptr, max_bytes + 1);

        // Treat data as LE for PK encryption/decryption purposes
        data.Host_To_LE();
        Int<MAX_UNIT_PRECISION> result = data.Exponential_Modulation(m_exponent, m_modulus);
        result.LE_To_Host();

        memmove(dstptr, result, max_bytes);

        size -= max_bytes + 1;
        srcptr += max_bytes + 1;
        dstptr += max_bytes;
        total += max_bytes;
    }

    return total;
}
