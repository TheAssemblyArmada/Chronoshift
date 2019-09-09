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
#ifdef GAME_DLL
RandomStraw<RandomClass> &g_cryptRandom = *reinterpret_cast<RandomStraw<RandomClass> *>(0x00667764);
#else
RandomStraw<RandomClass> g_cryptRandom;
#endif
// RandomStraw<Random2Class> CryptRandom2;
// RandomStraw<Random3Class> CryptRandom3;
// RandomStraw<Random4Class> CryptRandom4;    //Random4 is not designed for cryptography.
