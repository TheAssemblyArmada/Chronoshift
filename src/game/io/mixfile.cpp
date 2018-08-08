/**
 * @file
 *
 * @Author CCHyper, OmniBlade
 *
 * @brief  Handles the "Mix" file archives used by Westwood games.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "mixfile.h"
#include "ccfileclass.h"

#ifndef CHRONOSHIFT_STANDALONE
template<> List<MixFileClass<CCFileClass> *> &MixFileClass<CCFileClass>::s_mixList =
    *reinterpret_cast<List<MixFileClass<CCFileClass> *> *>(0x006ECE30);
#else
template<> List<MixFileClass<CCFileClass> *> MixFileClass<CCFileClass>::s_mixList;
#endif

// Explicit template instantiation.
template class MixFileClass<CCFileClass>;