/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base class for UI elements, provides basic input handling.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "gadget.h"
#include "keyboard.h"
#include "mouse.h"
#include "remap.h"

#ifndef GAME_DLL
RemapControlType *GadgetClass::ColorScheme = &GreyScheme;
GadgetClass *GadgetClass::StuckOn = nullptr;
GadgetClass *GadgetClass::LastList = nullptr;
GadgetClass *GadgetClass::Focused = nullptr;
#endif

GadgetClass::GadgetClass(unsigned input_flag, BOOL sticky) : XPos(0), YPos(0), Width(0), Height(0), IsSticky(sticky) {}

GadgetClass::GadgetClass(int x, int y, int w, int h, unsigned input_flag, BOOL sticky) :
    XPos(x),
    YPos(y),
    Width(w),
    Height(h),
    ToRedraw(false),
    IsSticky(sticky),
    IsDisabled(false),
    InputFlag(input_flag)
{
    if (sticky) {
        InputFlag |= (MOUSE_LEFT_PRESS | MOUSE_LEFT_RLSE);
    }
}

GadgetClass::GadgetClass(GadgetClass &that) :
    LinkClass(that),
    XPos(that.XPos),
    YPos(that.YPos),
    Width(that.Width),
    Height(that.Height),
    ToRedraw(that.ToRedraw),
    IsSticky(that.IsSticky),
    IsDisabled(that.IsDisabled),
    InputFlag(that.InputFlag)
{
}

GadgetClass::~GadgetClass()
{
    if (Has_Focus()) {
        Clear_Focus();
    }

    if (this == StuckOn) {
        StuckOn = nullptr;
    }

    if (this == LastList) {
        LastList = nullptr;
    }

    if (this == Focused) {
        Focused = nullptr;
    }
}

GadgetClass &GadgetClass::operator=(GadgetClass &that)
{
    if (this != &that) {
        LinkClass::operator=(that);
        XPos = that.XPos;
        YPos = that.YPos;
        Width = that.Width;
        Height = that.Height;
        ToRedraw = that.ToRedraw;
        IsSticky = that.IsSticky;
        IsDisabled = that.IsDisabled;
        InputFlag = that.InputFlag;
    }

    return *this;
}

GadgetClass *GadgetClass::Get_Next() const
{
    return reinterpret_cast<GadgetClass *>(LinkClass::Get_Next());
}

GadgetClass *GadgetClass::Get_Prev() const
{
    return reinterpret_cast<GadgetClass *>(LinkClass::Get_Prev());
}

GadgetClass *GadgetClass::Remove()
{
    Clear_Focus();

    return reinterpret_cast<GadgetClass *>(LinkClass::Remove());
}

KeyNumType GadgetClass::Input()
{
    int mouse_y = 0;
    int mouse_x = 0;
    BOOL not_last_list = false;
    unsigned press_flags = INPUT_NONE;
    KeyNumType key_press = KN_NONE;

    if (this != LastList) {
        LastList = this;
        StuckOn = nullptr;
        Focused = nullptr;
        not_last_list = true;
    }

    // DEBUG_LOG("GadgetClass::Input checking input buffer.\n");
    key_press = (KeyNumType)g_keyboard->Check();
    if (key_press) {
        // DEBUG_LOG("GadgetClass::Input checked 0x%04X was in buffer, retrieving.\n", key_press);
        key_press = (KeyNumType)g_keyboard->Get();
        // DEBUG_LOG("GadgetClass::Input retrieved 0x%04X from buffer.\n", key_press);
    }

    // checks for mouse keys?
    if ((key_press & 0xFF) != KN_LMOUSE && (key_press & 0xFF) != KN_RMOUSE) {
        mouse_x = g_mouse->Get_Mouse_X();
        mouse_y = g_mouse->Get_Mouse_Y();
    } else {
        mouse_y = g_keyboard->Get_MouseQY();
        mouse_x = g_keyboard->Get_MouseQX();
    }

    if (key_press) {
        if (key_press == KN_LMOUSE) { // left mouse key?    //EDWIN: 0x1001
            press_flags |= MOUSE_LEFT_PRESS;
        }

        if (key_press == KN_RMOUSE) { // right mouse key?    //EDWIN: 0x1002
            press_flags |= MOUSE_RIGHT_PRESS;
        }

        if (key_press == (KN_LMOUSE | KN_RLSE_BIT)) {
            press_flags |= MOUSE_LEFT_RLSE;
        }

        if (key_press == (KN_RMOUSE | KN_RLSE_BIT)) {
            press_flags |= MOUSE_RIGHT_RLSE;
        }

        if (!press_flags) {
            press_flags |= KEYBOARD_INPUT;
        }

    } else {
        // 0x1001
        if (g_keyboard->Down(KN_LMOUSE)) {
            press_flags |= MOUSE_LEFT_HELD;
        } else {
            press_flags |= MOUSE_LEFT_UP;
        }

        // 0x1002
        if (g_keyboard->Down(KN_RMOUSE)) {
            press_flags |= MOUSE_RIGHT_HELD;
        } else {
            press_flags |= MOUSE_RIGHT_UP;
        }
    }

    if (StuckOn != nullptr) {
        StuckOn->Draw_Me(false);
        GadgetClass *stuck = StuckOn;
        // DEBUG_LOG("Calling StuckOn->Clicked_On\n");
        StuckOn->Clicked_On(key_press, press_flags, mouse_x, mouse_y);

        if (StuckOn != nullptr) {
            StuckOn->Draw_Me(false);
        } else {
            stuck->Draw_Me(false);
        }

    } else {
        if (Focused != nullptr && (press_flags & KEYBOARD_INPUT) != 0) {
            Focused->Draw_Me(false);
            // DEBUG_LOG("Calling Focused->Clicked_On\n");
            Focused->Clicked_On(key_press, press_flags, mouse_x, mouse_y);

            if (Focused != nullptr) {
                Focused->Draw_Me(false);
            }

            if (key_press) {
                for (GadgetClass *entry = this; entry; entry = entry->Get_Next()) {
                    entry->Draw_Me(not_last_list);

                    if (!entry->IsDisabled && entry != Focused
                        && entry->Clicked_On(key_press, press_flags, mouse_x, mouse_y)) {
                        entry->Draw_Me(false);
                        return key_press;
                    }
                }
            }

        } else {
            for (GadgetClass *entry = this; entry; entry = entry->Get_Next()) {
                // DEBUG_LOG("Calling entry->Clicked_On\n");
                entry->Draw_Me(not_last_list);

                if (!entry->IsDisabled && entry->Clicked_On(key_press, press_flags, mouse_x, mouse_y)) {
                    entry->Draw_Me(false);
                    return key_press;
                }
            }
        }
    }

    // Return the input key pressed.
    return key_press;
}

void GadgetClass::Draw_All(BOOL redraw)
{
    // DEBUG_LOG("Go Go Gadget Draw_All()!.\n");

    for (GadgetClass *gadget = this; gadget != nullptr; gadget = gadget->Get_Next()) {
        gadget->Draw_Me(redraw);
    }
}

void GadgetClass::Delete_List()
{
    GadgetClass *gadget;

    for (gadget = this; gadget->Get_Prev() != nullptr; gadget = gadget->Get_Prev())
        ;

    // Loop works best as while, not sure how the dtor call is supposed to work.
    while (gadget != nullptr) {
        GadgetClass *last = gadget;

        gadget->Clear_Focus();

        gadget = gadget->Get_Next();

        if (last != nullptr) {
            last->~GadgetClass();
        }
    }
}

ControlClass *GadgetClass::Extract_Gadget(unsigned id)
{
    if (id != 0) {
        for (GadgetClass *gadget = this; gadget != nullptr; gadget = gadget->Get_Next()) {
            if (gadget->Get_ID() == id) {
                return reinterpret_cast<ControlClass *>(gadget);
            }
        }
    }

    return nullptr;
}

void GadgetClass::Flag_List_To_Redraw()
{
    LastList = nullptr;
}

void GadgetClass::Disable()
{
    IsDisabled = true;
    ToRedraw = true;
    Clear_Focus();
}

void GadgetClass::Enable()
{
    IsDisabled = false;
    ToRedraw = true;
    Clear_Focus();
}

unsigned GadgetClass::Get_ID() const
{
    return 0;
}

void GadgetClass::Flag_To_Redraw()
{
    ToRedraw = true;
}

void GadgetClass::Peer_To_Peer(unsigned flags, KeyNumType &key, ControlClass &peer)
{
    // empty
}

void GadgetClass::Set_Focus()
{
    if (Focused != nullptr) {
        Focused->Flag_To_Redraw();
        Focused->Clear_Focus();
    }

    InputFlag |= KEYBOARD_INPUT;

    Focused = this;
}

void GadgetClass::Clear_Focus()
{
    if (this == Focused) {
        InputFlag &= ~KEYBOARD_INPUT;
        Focused = nullptr;
    }
}

BOOL GadgetClass::Has_Focus()
{
    return (this == Focused);
}

BOOL GadgetClass::Is_List_To_Redraw()
{
    for (GadgetClass *gadget = this; gadget != nullptr; gadget = gadget->Get_Next()) {
        if (gadget->ToRedraw) {
            return true;
        }
    }

    return false;
}

void GadgetClass::Set_Position(int x, int y)
{
    XPos = x;
    YPos = y;
}

void GadgetClass::Set_Size(int w, int h)
{
    Width = w;
    Height = h;
}

BOOL GadgetClass::Draw_Me(BOOL redraw)
{
    if (redraw || ToRedraw) {
        ToRedraw = false;

        return true;
    }

    return false;
}

void GadgetClass::Sticky_Process(unsigned flags)
{
    if (IsSticky && (flags & MOUSE_LEFT_PRESS)) {
        StuckOn = this;

        return;
    }

    if (this == StuckOn && (flags & MOUSE_LEFT_RLSE)) {
        ToRedraw = true;
        StuckOn = nullptr;

        return;
    }
}

BOOL GadgetClass::Action(unsigned flags, KeyNumType &key)
{
    if (flags != INPUT_NONE) {
        ToRedraw = true;
        Sticky_Process(flags);

        return true;
    }

    return false;
}

BOOL GadgetClass::Clicked_On(KeyNumType &key, unsigned flags, int x, int y)
{
    unsigned masked_flags = InputFlag & flags;

    if (this == StuckOn || (masked_flags & KEYBOARD_INPUT) != 0
        || ((masked_flags != INPUT_NONE) && (unsigned int)(x - XPos) < (unsigned int)Width
               && (unsigned int)(y - YPos) < (unsigned int)Height)) {
        return Action(masked_flags, key);
    }

    return false;
}
