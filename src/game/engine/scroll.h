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
#pragma once

#ifndef SCROLL_H
#define SCROLL_H

#include "always.h"
#include "help.h"

class ScrollClass : public HelpClass
{
public:
    ScrollClass();

    virtual void Init_IO() override;
    virtual void AI(KeyNumType &key, int mouse_x, int mouse_y) override;

    BOOL Set_Autoscroll(int a1);

#ifndef RAPP_STANDALONE
    static void Hook_Me();
#endif

protected:
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool Autoscroll : 1; // 1
        };
        int Bitfield;
    };
#else
    bool Autoscroll;
#endif
    int field_165D;		//0x165D		//is this a rate of sorts?

private:
    static TCountDownTimerClass<SystemTimerClass> ScrollingCounter;

};

#ifndef RAPP_STANALONE
#include "hooker.h"
inline void ScrollClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    // Hook_Function(0x, *ScrollClass::Init_Clear);
#endif
}
#endif

#endif // SCROLL_H
