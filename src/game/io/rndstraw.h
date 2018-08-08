/**
 * @file
 *
 * @Author CCHyper, OmniBlade
 *
 * @brief Straw that provides a stream of random bytes.
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

#ifndef RNDSTRAW_H
#define RNDSTRAW_H

#include "always.h"
#include "gamedebug.h"
#include "random.h"
#include "sha.h"
#include "straw.h"
#include <new> // for placement new

template<class RNG>
class RandomStraw : public Straw
{
public:
    RandomStraw();
    virtual ~RandomStraw();
    virtual int Get(void *buffer, int length) override;
    void Reset();
    int Seed_Bits_Needed() const;
    void Seed_Bit(int value);
    void Seed_Byte(int8_t value);
    void Seed_Word(int16_t value);
    void Seed_Double_Word(int32_t value);
    void Scramble_Seed();

private:
    int m_currentSeedBits;
    int m_currentRandoms;
    RNG m_randomArray[32];
};

template<class RNG>
RandomStraw<RNG>::RandomStraw() : m_currentSeedBits(0), m_currentRandoms(0)
{
    Reset();
}

template<class RNG>
RandomStraw<RNG>::~RandomStraw()
{
    Reset();
}

template<class RNG>
int RandomStraw<RNG>::Get(void *buffer, int length)
{
    DEBUG_ASSERT(buffer != nullptr);
    DEBUG_ASSERT(length > 0);

    uint8_t *buff = static_cast<uint8_t *>(buffer);

    if (buffer != nullptr && length > 0) {
        for (int i = 0; i < length; ++i) {
            *buff++ = m_randomArray[m_currentRandoms++]();
            m_currentRandoms &= ARRAY_SIZE(m_randomArray) - 1;
        }

        return length;
    }

    return Straw::Get(buffer, length);
}

template<class RNG>
void RandomStraw<RNG>::Reset()
{
    m_currentSeedBits = 0;
    m_currentRandoms = 0;

    for (int index = 0; index < ARRAY_SIZE(m_randomArray); ++index) {
        new (&m_randomArray[index]) RNG;
    }
}

template<class RNG>
int RandomStraw<RNG>::Seed_Bits_Needed() const
{
    if (m_currentSeedBits < 2048) { // 2048 bits, is this a RSA/SHA key size?
        return m_currentSeedBits - 2048;
    }

    return 0;
}

template<class RNG>
void RandomStraw<RNG>::Seed_Bit(int value)
{
    // Is the value a odd number? (1, 3, 5 etc.)
    if (value & 1) {
        *(reinterpret_cast<uint8_t *>(m_randomArray) + (m_currentSeedBits / 8 & 0xFF)) ^= 1 << (m_currentSeedBits & 7);
    }

    ++m_currentSeedBits;

    if (m_currentSeedBits == 2048) {
        Scramble_Seed();
    }
}

template<class RNG>
void RandomStraw<RNG>::Scramble_Seed()
{
    SHAEngine sha;

    int sz = 0;
    char buff[20];

    for (int i = 0; i < sizeof(m_randomArray); ++i) {
        char *arrayp = reinterpret_cast<char *>(m_randomArray);
        sha.Hash(m_randomArray, sizeof(m_randomArray));
        sha.Result(buff);

        if (sizeof(m_randomArray) - i <= ARRAY_SIZE(buff)) {
            sz = sizeof(m_randomArray) - i;
        } else {
            sz = ARRAY_SIZE(buff);
        }

        memmove(arrayp++, buff, sz);
    }
}

template<class RNG>
void RandomStraw<RNG>::Seed_Byte(int8_t value)
{
    int datasize = sizeof(int8_t) * 8;

    for (int index = 0; index < datasize; ++index) {
        Seed_Bit(value);
        value >>= 1;
    }
}

template<class RNG>
void RandomStraw<RNG>::Seed_Word(int16_t value)
{
    int datasize = sizeof(int16_t) * 8;

    for (int index = 0; index < datasize; ++index) {
        Seed_Bit(value);
        value >>= 1;
    }
}

template<class RNG>
void RandomStraw<RNG>::Seed_Double_Word(int32_t value)
{
    int datasize = sizeof(int32_t) * 8;

    for (int index = 0; index < datasize; ++index) {
        Seed_Bit(value);
        value >>= 1;
    }
}

// Global instance of random straw to use as a source of random data.
#ifndef CHRONOSHIFT_STANDALONE
extern RandomStraw<RandomClass> &g_cryptRandom;
#else
extern RandomStraw<RandomClass> g_cryptRandom;
#endif
// extern RandomStraw<Random2Class> CryptRandom2;

#endif // RNDSTRAW_H
