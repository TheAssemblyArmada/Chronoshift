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
    friend void Setup_Hooks();
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

#endif // SLIDER_H
