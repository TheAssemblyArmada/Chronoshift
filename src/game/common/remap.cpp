/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Structure containing color info for remaps.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "remap.h"

#ifndef GAME_DLL
RemapControlType g_ColorRemaps[REMAP_COUNT];
RemapControlType g_MetalScheme;
RemapControlType g_GreyScheme;
RemapControlType g_SidebarScheme;
#endif
