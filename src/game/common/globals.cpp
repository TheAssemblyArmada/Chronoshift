/**
 * @file
 *
 * @Author CCHyper, OmniBlade
 *
 * @brief Global variables that are used amongst many classes.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "globals.h"

#ifndef RAPP_STANDALONE
int &g_frame = Make_Global<int>(0x006680C4);
BOOL &g_gameInFocus = Make_Global<int>(0x00665F64);
#else
int g_frame;
BOOL g_gameInFocus;
#endif