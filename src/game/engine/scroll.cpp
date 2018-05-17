/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling scrolling the viewport.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */

#include "scroll.h"

ScrollClass::ScrollClass()
{}

void ScrollClass::Init_IO(void)
{
    HelpClass::Init_IO();
    //ScrollingCounter = 0; //help

}

void ScrollClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
#ifndef RAPP_STANDALONE
    DEFINE_CALL(func,0x00547088, void, (const ScrollClass *, KeyNumType &, int, int));
    func(this, key, mouse_x, mouse_y);
#endif
}

BOOL ScrollClass::Set_Autoscroll(int a1)
{
    BOOL old = Autoscroll;

    if ( a1 == -1 ) {
        Autoscroll = !Autoscroll;
    } else {
        Autoscroll = a1;
    }
    return old;
}