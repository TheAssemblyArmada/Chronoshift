/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class for representing gauge like UI elements.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
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

protected:
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool FillGauge : 1; // & 1
            bool UseThumb : 1; // & 2
            bool IsHorizontal : 1; // & 4
        };
        int m_gaugeFlags;
    };
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

#endif // GAUGE_H
