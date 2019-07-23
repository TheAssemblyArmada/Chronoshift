/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class for representing slider elements such as scroll bars.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef SLIDER_H
#define SLIDER_H

#include "always.h"
#include "gauge.h"
#include "globals.h"
#include "shapebtn.h"

class SliderClass : public GaugeClass
{
public:
    SliderClass(unsigned id, int x, int y, int w, int h, BOOL no_buttons);
    SliderClass(SliderClass &that);
    virtual ~SliderClass();

    SliderClass &operator=(SliderClass &that);

    virtual void Peer_To_Peer(unsigned flags, KeyNumType &key, ControlClass &peer) override;
    virtual BOOL Draw_Me(BOOL redraw) override;
    virtual BOOL Action(unsigned flags, KeyNumType &key) override;
    virtual BOOL Set_Maximum(int max) override;
    virtual BOOL Set_Value(int value) override;
    virtual int Thumb_Pixels() override { return ThumbnailPixels; }
    virtual void Draw_Thumb() override;
    virtual void Set_Thumb_Size(int size);
    virtual BOOL Bump(BOOL bump_up);
    virtual BOOL Step(BOOL step_up);

#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
#endif

protected:
    void Recalc_Thumb();

protected:
    ShapeButtonClass *ButtonPlus;
    ShapeButtonClass *ButtonMinus;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL NoButtons : 1; // & 1
#else
    bool NoButtons; // 1
#endif
    int BumpSize;
    int ThumbnailPixels;
    int ThumbnailSize;
};

inline SliderClass &SliderClass::operator=(SliderClass &that)
{
    if (this != &that) {
        GaugeClass::operator=(that);
        ButtonPlus = that.ButtonPlus;
        ButtonMinus = that.ButtonPlus;
        NoButtons = that.NoButtons;
        BumpSize = that.BumpSize;
        ThumbnailPixels = that.ThumbnailPixels;
        ThumbnailSize = that.ThumbnailSize;
        ButtonPlus->Make_Peer(*this);
        ButtonMinus->Make_Peer(*this);
    }

    return *this;
}

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"

inline void SliderClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x004FCD70, *SliderClass::Thumb_Pixels);
    Hook_Function(0x0054F8D0, *SliderClass::Set_Maximum);
    Hook_Function(0x0054F8F4, *SliderClass::Set_Thumb_Size);
    Hook_Function(0x0054F92C, *SliderClass::Set_Value);
    Hook_Function(0x0054F95C, *SliderClass::Recalc_Thumb);
    Hook_Function(0x0054F9E8, *SliderClass::Action);
    Hook_Function(0x0054FA9C, *SliderClass::Bump);
    Hook_Function(0x0054FACC, *SliderClass::Step);
    Hook_Function(0x0054FAF0, *SliderClass::Draw_Thumb);
    Hook_Function(0x0054FB3C, *SliderClass::Draw_Me);
    Hook_Function(0x0054FBCC, *SliderClass::Peer_To_Peer);
#endif
}
#endif

#endif // SLIDER_H
