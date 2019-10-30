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
    virtual int const Get_Value() const { return m_Value; }
    virtual void Use_Thumb(BOOL use_thumb);
    virtual int Thumb_Pixels() { return DefaultThumbSize; }
    virtual void Draw_Thumb();
    virtual int Pixel_To_Value(int pixel);
    virtual int Value_To_Pixel(int value);

    int const Get_Maximum() const { return m_Maximum; }

protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_FillGauge : 1; // & 1
    BOOL m_UseThumb : 1; // & 2
    BOOL m_IsHorizontal : 1; // & 4
#else
    bool m_FillGauge;
    bool m_UseThumb;
    bool m_IsHorizontal;
#endif

    int m_Maximum;
    int m_Value;
    int m_LastPos;

    static const int DefaultThumbSize;
};

inline GaugeClass &GaugeClass::operator=(GaugeClass &that)
{
    if (this != &that) {
        ControlClass::operator=(that);
        m_FillGauge = that.m_FillGauge;
        m_UseThumb = that.m_UseThumb;
        m_IsHorizontal = that.m_IsHorizontal;
        m_Maximum = that.m_Maximum;
        m_Value = that.m_Value;
        m_LastPos = that.m_LastPos;
    }

    return *this;
}

#endif // GAUGE_H
