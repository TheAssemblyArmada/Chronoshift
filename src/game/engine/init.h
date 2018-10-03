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

void Init_Expansion_Files();
void Init_Bootstrap_Mixfiles();
void Init_Secondary_Mixfiles();
void Reinit_Secondary_Mixfiles();
//void Init_Bulk_Data();
void Init_Keys();
void Init_Fonts();
void Init_Random();
void Init_Color_Remaps();

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"

namespace Init
{

inline void Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x004F5EC4, Init_Random);
    Hook_Function(0x004F6240, Init_Color_Remaps);
    Hook_Function(0x004F781C, Init_Expansion_Files);
    Hook_Function(0x004F7948, Init_Fonts);
    Hook_Function(0x004F7DF0, Init_Bootstrap_Mixfiles);
    Hook_Function(0x004F8018, Init_Secondary_Mixfiles);
    Hook_Function(0x004F8664, Init_Keys);
#endif
}

}
#endif

#endif // INIT_H
