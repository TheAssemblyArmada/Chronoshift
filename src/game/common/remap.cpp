/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Structure containing color info for remaps.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "remap.h"

#ifndef CHRONOSHIFT_STANDALONE
RemapControlType *ColorRemaps = reinterpret_cast<RemapControlType *>(0x00666908);
RemapControlType &MetalScheme = Make_Global<RemapControlType>(0x00667528);
RemapControlType &GreyScheme = Make_Global<RemapControlType>(0x00667644);
RemapControlType &SidebarScheme = Make_Global<RemapControlType>(0x00684CC0);
#else
RemapControlType ColorRemaps[REMAP_COUNT];
RemapControlType MetalScheme;
RemapControlType GreyScheme;
RemapControlType SidebarScheme;
#endif
