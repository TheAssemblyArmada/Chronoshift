/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Just a function to detach a target from everything else in game.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "tracker.h"

void Detach_This_From_All(int32_t target, int unk)
{
    // TODO Needs most of the top level object classes.
#ifndef RAPP_STANDALONE
    void(*func)(int32_t, int) = reinterpret_cast<void(*)(int32_t, int)>(0x0056C5E0);
    func(target, unk);
#endif
}