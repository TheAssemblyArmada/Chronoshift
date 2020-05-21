/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 * @author tomsons26
 *
 * @brief Engine initialisation functions.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef INIT_H
#define INIT_H

#include "always.h"

class GraphicViewPortClass;
class PaletteClass;

void Init_Expansion_Files();
void Init_Bootstrap_Mixfiles();
void Init_Secondary_Mixfiles();
void Reinit_Secondary_Mixfiles();
void Init_Bulk_Data();
void Init_One_Time_Systems();
void Init_Keys();
void Init_Fonts();
void Init_Random();
void Init_Color_Remaps();
void Init_Mouse();
void Bootstrap();
void Init_CDROM_Access();
void Load_Title_Screen(const char *filename, GraphicViewPortClass *vp, PaletteClass *pal);
void Load_Prolog_Page();
void Init_Heaps();
void Anim_Init();
BOOL Init_Game(int argc, char **argv);

#endif // INIT_H
