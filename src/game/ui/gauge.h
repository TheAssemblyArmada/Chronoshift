/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class for representing gauge like UI elements.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef GAUGE_H
#define GAUGE_H

#include "always.h"
#include "controlc.h"

class GaugeClass : public ControlClass
{
public:
    GaugeClass(unsigned id, int x, int y, int w, int h);
    GaugeClass(GaugeClass &that);
    virtual ~GaugeClass() {}

    GaugeClass &operator=(GaugeClass &that);

    virtual BOOL Draw_Me(BOOL redraw) override;
    virtual BOOL Action(unsigned flags, KeyNumType &key) override;
    virtual BOOL Set_Maximum(int max);
    virtual BOOL Set_Value(int value);
    virtual int const Get_Value() const { return Value; }
    virtual void Use_Thumb(BOOL use_thumb);
    virtual int Thumb_Pixels() { return DefaultThumbSize; }
    virtual void Draw_Thumb();
    virtual int Pixel_To_Value(int pixel);
    virtual int Value_To_Pixel(int value);

    int const Get_Maximum() const { return Maximum; }

#ifdef GAME_DLL
static void Hook_Me();
#endif

protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL FillGauge : 1; // & 1
    BOOL UseThumb : 1; // & 2
    BOOL IsHorizontal : 1; // & 4
#else
    bool FillGauge;
    bool UseThumb;
    bool IsHorizontal;
#endif

    int Maximum;
    int Value;
    int LastPos;

    static const int DefaultThumbSize;
};

inline GaugeClass &GaugeClass::operator=(GaugeClass &that)
{
    if (this != &that) {
        ControlClass::operator=(that);
        FillGauge = that.FillGauge;
        UseThumb = that.UseThumb;
        IsHorizontal = that.IsHorizontal;
        Maximum = that.Maximum;
        Value = that.Value;
        LastPos = that.LastPos;
    }

    return *this;
}

#ifdef GAME_DLL
#include "hooker.h"

inline void GaugeClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x004C4E70, *GaugeClass::Draw_Me);
    Hook_Function(0x004C52D0, *GaugeClass::Action);
    Hook_Function(0x004C4D70, *GaugeClass::Set_Maximum);
    Hook_Function(0x004C4D90, *GaugeClass::Set_Value);
    //Hook_Function(0x004C6FB0, *GaugeClass::Get_Value);
    Hook_Function(0x004C6FC0, *GaugeClass::Use_Thumb);
    Hook_Function(0x004C6FF0, *GaugeClass::Thumb_Pixels);
    Hook_Function(0x004C5440, *GaugeClass::Draw_Thumb);
    Hook_Function(0x004C4DC8, *GaugeClass::Pixel_To_Value);
    Hook_Function(0x004C4E28, *GaugeClass::Value_To_Pixel);
#endif
}
#endif

#endif // GAUGE_H
