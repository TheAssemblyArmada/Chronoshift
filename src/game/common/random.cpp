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
#include "random.h"

#ifdef GAME_DLL
RandomClass &g_nonCriticalRandom = *reinterpret_cast<RandomClass*>(0x00667760);
#else
RandomClass g_nonCriticalRandom;
#endif

const int RandomClass::s_multiplier = 0x41C64E6D; // K multiplier value.
const int RandomClass::s_adder = 0x00003039; // K additive value.

int RandomClass::operator()()
{
    m_seed = (m_seed * s_multiplier) + s_adder;

    return ((m_seed >> THROW_AWAY_BITS) & MAXIMUM);
}

int RandomClass::operator()(int minval, int maxval)
{
    // Test for shortcut case where the range is null and thus
    // the number to return is actually implicit from the
    // parameters.
    if (minval == maxval) {
        return minval;
    }

    // Ensure that the min and max range values are in proper order.
    if (minval > maxval) {
        int temp = minval;
        minval = maxval;
        maxval = temp;
    }

    // Find the highest bit that fits within the magnitude of the
    // range of random numbers desired. Notice that the scan is
    // limited to the range of significant bits returned by the
    // random number algorithm.
    int magnitude = maxval - minval;
    int highbit = SIGNIFICANT_BITS - 1;

    while ((magnitude & (1 << highbit)) == 0 && highbit > 0) {
        --highbit;
    }

    //
    // Create a full bit mask pattern that has all bits set that just
    // barely covers the magnitude of the number range desired.
    //
    int mask = ~((~0L) << (highbit + 1));

    // Keep picking random numbers until it fits within the magnitude desired.
    // With a  good random number generator, it will have to perform this loop
    // an average of one and a half times.
    int pick = magnitude + 1;

    while (pick > magnitude) {
        pick = (*this)() & mask;
    }

    // Finally, bias the random number pick to the start of the range requested.
    return pick + minval;
}
