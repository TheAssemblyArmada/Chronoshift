/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief The game engines main routine to be called from the user entry point.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "gamemain.h"

void Game_Main(int argc, char **argv)
{
#ifdef GAME_DLL
    void (*func)(int, char **) = reinterpret_cast<void (*)(int, char **)>(0x004A5368);
    func(argc, argv);
#endif
}
