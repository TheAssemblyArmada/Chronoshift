/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Straw that provides a stream of random bytes.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "rndstraw.h"

// Global instance of random straw to use as a source of random data.
#ifndef GAME_DLL
RandomStraw<RandomClass> g_CryptRandom;
#endif
