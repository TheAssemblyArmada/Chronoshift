/**
 * @file
 *
 * @Author CCHyper, OmniBlade
 *
 * @brief Wrapper class for platform drawing engine.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "gbuffer.h"

#ifndef RAPP_STANDALONE
 //BOOL &GraphicViewPortClass::AllowHardwareBlitFills;
 //BOOL &GraphicViewPortClass::AllowStretchBlits;
int &GraphicViewPortClass::ScreenWidth = Make_Global<int>(0x006016B0);
int &GraphicViewPortClass::ScreenHeight = Make_Global<int>(0x006016B4);
#else
BOOL GraphicViewPortClass::AllowHardwareBlitFills;
BOOL GraphicViewPortClass::AllowStretchBlits;
int GraphicViewPortClass::ScreenWidth = 640;
int GraphicViewPortClass::ScreenHeight = 400;
#endif
