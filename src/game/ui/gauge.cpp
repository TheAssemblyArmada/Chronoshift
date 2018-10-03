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
#include "gauge.h"
#include "dialog.h"
#include "fixed.h"
#include "minmax.h"
#include "mouse.h"
#include "remap.h"

const int GaugeClass::DefaultThumbSize = 4;

GaugeClass::GaugeClass(unsigned id, int x, int y, int w, int h) :
    ControlClass(id, x, y, w, h, MOUSE_LEFT_PRESS | MOUSE_LEFT_HELD | MOUSE_LEFT_RLSE, true)
{
    Set_Maximum(255);
    Set_Value(0);

    UseThumb = true;
    IsHorizontal = w > h;
    FillGauge = true;
    LastPos = 0;
}

GaugeClass::GaugeClass(GaugeClass &that) :
    ControlClass(that),
    FillGauge(that.FillGauge),
    UseThumb(that.UseThumb),
    IsHorizontal(that.IsHorizontal),
    Maximum(that.Maximum),
    Value(that.Value),
    LastPos(that.LastPos)
{
}

BOOL GaugeClass::Draw_Me(BOOL redraw)
{
    if (ControlClass::Draw_Me(redraw)) {
        if (g_logicPage == &g_seenBuff) {
            g_mouse->Conditional_Hide_Mouse(XPos, YPos, XPos + Width, YPos + Height);
        }

        Draw_Box(XPos, YPos, Width, Height, BOX_STYLE_0, true);

        if (FillGauge) {
            int pval = Value_To_Pixel(Value);

            if (IsHorizontal) {
                if (pval >= XPos + 1) {
                    g_logicPage->Fill_Rect(
                        XPos + 1, YPos + 1, pval, YPos + Height - 2, GadgetClass::ColorScheme->WindowPalette[5]);
                }
            } else {
                if (pval >= YPos + 1) {
                    g_logicPage->Fill_Rect(
                        XPos + 1, YPos + 1, XPos + Width - 2, pval, GadgetClass::ColorScheme->WindowPalette[5]);
                }
            }
        }

        if (UseThumb) {
            Draw_Thumb();
        }

        if (g_logicPage == &g_seenBuff) {
            g_mouse->Conditional_Show_Mouse();
        }

        return true;
    }

    return false;
}

BOOL GaugeClass::Action(unsigned flags, KeyNumType &key)
{
    if (!UseThumb) {
        return false;
    }

    Sticky_Process(flags);

    if (flags & MOUSE_LEFT_PRESS || flags & MOUSE_LEFT_HELD && this == GadgetClass::StuckOn) {
        int valpixel;
        int mousepos;

        if (flags & MOUSE_LEFT_PRESS) {
            int pixelval = Value_To_Pixel(Value);
            mousepos = IsHorizontal ? g_mouse->Get_Mouse_X() : g_mouse->Get_Mouse_Y();

            if (mousepos <= pixelval) {
                LastPos = 0;
            } else {
                mousepos -= pixelval;
                if (mousepos >= Thumb_Pixels()) {
                    LastPos = mousepos;
                } else {
                    LastPos = 0;
                }
            }

            mousepos = IsHorizontal ? g_mouse->Get_Mouse_X() : g_mouse->Get_Mouse_Y();
            valpixel = Pixel_To_Value(mousepos - LastPos);

            for (int i = valpixel; i < Value;) {
                if (LastPos <= 0) {
                    break;
                }

                --LastPos;
                mousepos = IsHorizontal ? g_mouse->Get_Mouse_X() : g_mouse->Get_Mouse_Y();
                i = Pixel_To_Value(mousepos - LastPos);
            }
        }

        mousepos = IsHorizontal ? g_mouse->Get_Mouse_X() : g_mouse->Get_Mouse_Y();
        valpixel = Pixel_To_Value(mousepos - LastPos);

        if (!Set_Value(valpixel)) {
            ControlClass::Action(INPUT_NONE, key);
            return true;
        }
    } else {
        flags &= ~MOUSE_LEFT_HELD;
    }

    return ControlClass::Action(flags, key);
}

BOOL GaugeClass::Set_Maximum(int max)
{
    if (max != Maximum) {
        Maximum = max;
        Flag_To_Redraw();

        return true;
    }

    return false;
}

BOOL GaugeClass::Set_Value(int value)
{
    value = Clamp(value, 0, Maximum);

    if (value != Value) {
        Value = value;
        Flag_To_Redraw();

        return true;
    }

    return false;
}

void GaugeClass::Use_Thumb(BOOL use_thumb)
{
    UseThumb = use_thumb;
}

void GaugeClass::Draw_Thumb()
{
    int start_pixel = Value_To_Pixel(Value);

    if (start_pixel + 4 > Value_To_Pixel(Maximum)) {
        start_pixel = Value_To_Pixel(Maximum) - 2;
    }

    start_pixel = Max(XPos, start_pixel);

    if (IsHorizontal) {
        Draw_Box(start_pixel, YPos, Thumb_Pixels(), Height, BOX_STYLE_1, true);
    } else {
        Draw_Box(XPos, start_pixel, Width, Thumb_Pixels(), BOX_STYLE_1, true);
    }
}

int GaugeClass::Pixel_To_Value(int pixel)
{
    int offset;
    int max;

    if (IsHorizontal) {
        offset = pixel - (XPos + 1);
        max = Width - 2;
    } else {
        offset = pixel - (YPos + 1);
        max = Height - 2;
    }

    int value = Clamp(offset, 0, max);
    fixed fval(value, max);

    return Maximum * fval;
}

int GaugeClass::Value_To_Pixel(int value)
{
    int offset;
    int max;

    if (IsHorizontal) {
        max = Width - 2;
        offset = XPos;
    } else {
        max = Height - 2;
        offset = YPos;
    }

    fixed result(value, Maximum);

    return offset + (max * result);
}
