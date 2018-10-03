/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Random number generator.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef RANDOM_H
#define RANDOM_H

#include "always.h"

/**
 * @brief Provides a linear congruential random number generator with 15 bits of significance.
 */
class RandomClass
{
public:
    enum
    {
        SIGNIFICANT_BITS = 15 // Random number bit significance.
    };

public:
    RandomClass(int seed = 0) : m_seed(seed) {}

    int operator()();
    int operator()(int minval, int maxval);

    uint32_t Get_Seed() { return m_seed; }

private:
    enum
    {
        THROW_AWAY_BITS = 10, // Low bits to throw away.
        MAXIMUM = 32767
    };

    uint32_t m_seed;
    static const int s_multiplier;
    static const int s_adder;
};

#ifndef CHRONOSHIFT_STANDALONE
extern RandomClass &g_nonCriticalRandom;
#else
extern RandomClass g_nonCriticalRandom;
#endif

#endif // RANDOM_H
