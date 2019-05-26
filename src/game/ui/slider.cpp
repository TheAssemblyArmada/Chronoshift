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
#include "slider.h"
#include "gamefile.h"
#include "dialog.h"
#include "fixed.h"
#include "mixfile.h"
#include "mouse.h"
#include "shapebtn.h"

SliderClass::SliderClass(unsigned id, int x, int y, int w, int h, BOOL no_buttons) :
    GaugeClass(id, x, y, w, h),
    ButtonPlus(nullptr),
    ButtonMinus(nullptr),
    NoButtons(no_buttons)
{
    if (!no_buttons) {
        ButtonPlus = new ShapeButtonClass(id, GameFileClass::Retrieve("BTN-PLUS.SHP"), XPos + Width + 2, YPos);

        if (ButtonPlus) {
            ButtonPlus->Make_Peer(*this);
            ButtonPlus->Add(*this);
            ButtonPlus->Flag_To_Redraw();
        }

        ButtonMinus = new ShapeButtonClass(id, GameFileClass::Retrieve("BTN-MINS.SHP"), XPos - 6, YPos);

        if (ButtonMinus) {
            ButtonMinus->Make_Peer(*this);
            ButtonMinus->Add(*this);
            ButtonMinus->Flag_To_Redraw();
        }
    }

    Set_Thumb_Size(1);
    Recalc_Thumb();

    FillGauge = false;
}

SliderClass::SliderClass(SliderClass &that) :
    GaugeClass(that),
    ButtonPlus(that.ButtonPlus),
    ButtonMinus(that.ButtonMinus),
    NoButtons(that.NoButtons),
    BumpSize(that.BumpSize),
    ThumbnailPixels(that.ThumbnailPixels),
    ThumbnailSize(that.ThumbnailSize)
{
}

SliderClass::~SliderClass()
{
    if (ButtonPlus != nullptr) {
        delete ButtonPlus;
        ButtonPlus = nullptr;
    }

    if (ButtonMinus != nullptr) {
        delete ButtonMinus;
        ButtonMinus = nullptr;
    }
}

void SliderClass::Peer_To_Peer(unsigned flags, KeyNumType &key, ControlClass &peer)
{
    if (flags & MOUSE_LEFT_RLSE) {
        if (&peer == ButtonPlus) {
            Step(false);
        }

        if (&peer == ButtonMinus) {
            Step(true);
        }
    }
}

BOOL SliderClass::Draw_Me(BOOL redraw)
{
    if (NoButtons && ControlClass::Draw_Me(redraw)) {
        if (&g_seenBuff == g_logicPage) {
            g_mouse->Conditional_Hide_Mouse(XPos, YPos, Width + XPos, Height + YPos);
        }

        Draw_Box(XPos, YPos, Width, Height, BOX_STYLE_0, true);
        Draw_Thumb();

        if (&g_seenBuff == g_logicPage) {
            g_mouse->Conditional_Show_Mouse();
        }

        return true;
    }

    return GaugeClass::Draw_Me(redraw);
}

BOOL SliderClass::Action(unsigned flags, KeyNumType &key)
{
    if (flags & MOUSE_LEFT_PRESS) {
        int mouse_pos;
        int gadget_pos;

        if (IsHorizontal) {
            mouse_pos = g_mouse->Get_Mouse_X();
            gadget_pos = XPos;
        } else {
            mouse_pos = g_mouse->Get_Mouse_Y();
            gadget_pos = YPos;
        }

        gadget_pos += ThumbnailSize + 1;

        if (mouse_pos >= gadget_pos) {
            if (mouse_pos <= ThumbnailPixels + gadget_pos) {
                GaugeClass::Action(flags, key);
                key = KN_NONE;

                return true;
            } else {
                Bump(false); // bump down
                GaugeClass::Action(0, key);
                key = KN_NONE;

                return true;
            }
        } else {
            Bump(true); // bump up
            GaugeClass::Action(0, key);
            key = KN_NONE;

            return true;
        }
    }

    return GaugeClass::Action(flags, key);
}

BOOL SliderClass::Set_Maximum(int max)
{
    if (GaugeClass::Set_Maximum(max)) {
        Recalc_Thumb();

        return true;
    }

    return false;
}

BOOL SliderClass::Set_Value(int value)
{
    if (GaugeClass::Set_Value(Min(value, Maximum - BumpSize))) {
        Recalc_Thumb();

        return true;
    }

    return false;
}

void SliderClass::Draw_Thumb()
{
    if (IsHorizontal) {
        Draw_Box(ThumbnailSize + XPos, YPos, ThumbnailPixels, Height, BOX_STYLE_1, true);
    } else {
        Draw_Box(XPos, ThumbnailSize + YPos, Width, ThumbnailPixels, BOX_STYLE_1, true);
    }
}

void SliderClass::Set_Thumb_Size(int size)
{
    BumpSize = Clamp(size, 1, Maximum);
    Recalc_Thumb();
}

BOOL SliderClass::Bump(BOOL bump_up)
{
    if (bump_up) {
        return Set_Value(Value - BumpSize);
    }

    return Set_Value(Value + BumpSize);
}

BOOL SliderClass::Step(BOOL step_up)
{
    if (step_up) {
        return Set_Value(Value - 1);
    }

    return Set_Value(Value + 1);
}

void SliderClass::Recalc_Thumb()
{
    int length = IsHorizontal ? Width : Height;
    ThumbnailPixels = Max(4, length * fixed(BumpSize, Maximum));
    ThumbnailSize = Min(length - ThumbnailPixels, length * fixed(Value, Maximum));
}
