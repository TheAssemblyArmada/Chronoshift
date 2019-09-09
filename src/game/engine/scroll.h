/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling scrolling the viewport.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
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

enum ScrollMode
{
    SCROLLMODE_TOGGLE = -1,
    SCROLLMODE_MANUAL = 0,
    SCROLLMODE_AUTO = 1,
};

class ScrollClass : public HelpClass
{
public:
    ScrollClass();
    ScrollClass(const NoInitClass &noinit) : HelpClass(noinit) {}

    virtual void Init_IO() override;
    virtual void AI(KeyNumType &key, int mouse_x, int mouse_y) override;

    BOOL Set_Autoscroll(int mode);

#ifdef GAME_DLL
    static void Hook_Me();
#endif

protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL Autoscroll : 1; // 1
#else
    bool Autoscroll;
#endif
    int ScrollUnkInt; // is this a rate of sorts?

private:
#ifdef GAME_DLL
    static TCountDownTimerClass<SystemTimerClass> &ScrollingCounter;
#else
    static TCountDownTimerClass<SystemTimerClass> ScrollingCounter;
#endif
};

#ifdef GAME_DLL
#include "hooker.h"
inline void ScrollClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x004F92E0, *ScrollClass::Init_IO);
    Hook_Function(0x00547088, *ScrollClass::AI);
    Hook_Function(0x00547464, *ScrollClass::Set_Autoscroll);
#endif
}
#endif

#endif // SCROLL_H
