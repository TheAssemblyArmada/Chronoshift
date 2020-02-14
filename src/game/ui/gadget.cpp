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
RemapControlType *GadgetClass::s_ColorScheme = &g_GreyScheme;
GadgetClass *GadgetClass::s_StuckOn = nullptr;
GadgetClass *GadgetClass::s_LastList = nullptr;
GadgetClass *GadgetClass::s_Focused = nullptr;
#endif

GadgetClass::GadgetClass(unsigned input_flag, BOOL sticky) : m_XPos(0), m_YPos(0), m_Width(0), m_Height(0), m_IsSticky(sticky) {}

GadgetClass::GadgetClass(int x, int y, int w, int h, unsigned input_flag, BOOL sticky) :
    m_XPos(x),
    m_YPos(y),
    m_Width(w),
    m_Height(h),
    m_ToRedraw(false),
    m_IsSticky(sticky),
    m_IsDisabled(false),
    m_InputFlag(input_flag)
{
    if (sticky) {
        m_InputFlag |= (MOUSE_LEFT_PRESS | MOUSE_LEFT_RLSE);
    }
}

GadgetClass::GadgetClass(GadgetClass &that) :
    LinkClass(that),
    m_XPos(that.m_XPos),
    m_YPos(that.m_YPos),
    m_Width(that.m_Width),
    m_Height(that.m_Height),
    m_ToRedraw(that.m_ToRedraw),
    m_IsSticky(that.m_IsSticky),
    m_IsDisabled(that.m_IsDisabled),
    m_InputFlag(that.m_InputFlag)
{
}

GadgetClass::~GadgetClass()
{
    if (Has_Focus()) {
        Clear_Focus();
    }

    if (this == s_StuckOn) {
        s_StuckOn = nullptr;
    }

    if (this == s_LastList) {
        s_LastList = nullptr;
    }

    if (this == s_Focused) {
        s_Focused = nullptr;
    }
}

GadgetClass &GadgetClass::operator=(GadgetClass &that)
{
    if (this != &that) {
        LinkClass::operator=(that);
        m_XPos = that.m_XPos;
        m_YPos = that.m_YPos;
        m_Width = that.m_Width;
        m_Height = that.m_Height;
        m_ToRedraw = that.m_ToRedraw;
        m_IsSticky = that.m_IsSticky;
        m_IsDisabled = that.m_IsDisabled;
        m_InputFlag = that.m_InputFlag;
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

    if (this != s_LastList) {
        s_LastList = this;
        s_StuckOn = nullptr;
        s_Focused = nullptr;
        not_last_list = true;
    }

    // captainslog_debug("GadgetClass::Input checking input buffer.");
    key_press = (KeyNumType)g_Keyboard->Check();
    if (key_press) {
        // captainslog_debug("GadgetClass::Input checked 0x%04X was in buffer, retrieving.", key_press);
        key_press = (KeyNumType)g_Keyboard->Get();
        // captainslog_debug("GadgetClass::Input retrieved 0x%04X from buffer.", key_press);
    }

    // checks for mouse keys?
    if ((key_press & 0xFF) != KN_LMOUSE && (key_press & 0xFF) != KN_RMOUSE) {
        mouse_x = g_Mouse->Get_Mouse_X();
        mouse_y = g_Mouse->Get_Mouse_Y();
    } else {
        mouse_y = g_Keyboard->Get_MouseQY();
        mouse_x = g_Keyboard->Get_MouseQX();
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
        if (g_Keyboard->Down(KN_LMOUSE)) {
            press_flags |= MOUSE_LEFT_HELD;
        } else {
            press_flags |= MOUSE_LEFT_UP;
        }

        // 0x1002
        if (g_Keyboard->Down(KN_RMOUSE)) {
            press_flags |= MOUSE_RIGHT_HELD;
        } else {
            press_flags |= MOUSE_RIGHT_UP;
        }
    }

    if (s_StuckOn != nullptr) {
        s_StuckOn->Draw_Me(false);
        GadgetClass *stuck = s_StuckOn;
        // captainslog_debug("Calling s_StuckOn->Clicked_On");
        s_StuckOn->Clicked_On(key_press, press_flags, mouse_x, mouse_y);

        if (s_StuckOn != nullptr) {
            s_StuckOn->Draw_Me(false);
        } else {
            stuck->Draw_Me(false);
        }

    } else {
        if (s_Focused != nullptr && (press_flags & KEYBOARD_INPUT) != 0) {
            s_Focused->Draw_Me(false);
            // captainslog_debug("Calling s_Focused->Clicked_On");
            s_Focused->Clicked_On(key_press, press_flags, mouse_x, mouse_y);

            if (s_Focused != nullptr) {
                s_Focused->Draw_Me(false);
            }

            if (key_press) {
                for (GadgetClass *entry = this; entry; entry = entry->Get_Next()) {
                    entry->Draw_Me(not_last_list);

                    if (!entry->m_IsDisabled && entry != s_Focused
                        && entry->Clicked_On(key_press, press_flags, mouse_x, mouse_y)) {
                        entry->Draw_Me(false);
                        return key_press;
                    }
                }
            }

        } else {
            for (GadgetClass *entry = this; entry; entry = entry->Get_Next()) {
                // captainslog_debug("Calling entry->Clicked_On");
                entry->Draw_Me(not_last_list);

                if (!entry->m_IsDisabled && entry->Clicked_On(key_press, press_flags, mouse_x, mouse_y)) {
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
    // captainslog_debug("Go Go Gadget Draw_All()!.");

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
    s_LastList = nullptr;
}

void GadgetClass::Disable()
{
    m_IsDisabled = true;
    m_ToRedraw = true;
    Clear_Focus();
}

void GadgetClass::Enable()
{
    m_IsDisabled = false;
    m_ToRedraw = true;
    Clear_Focus();
}

unsigned GadgetClass::Get_ID() const
{
    return 0;
}

void GadgetClass::Flag_To_Redraw()
{
    m_ToRedraw = true;
}

void GadgetClass::Peer_To_Peer(unsigned flags, KeyNumType &key, ControlClass &peer)
{
    // empty
}

void GadgetClass::Set_Focus()
{
    if (s_Focused != nullptr) {
        s_Focused->Flag_To_Redraw();
        s_Focused->Clear_Focus();
    }

    m_InputFlag |= KEYBOARD_INPUT;

    s_Focused = this;
}

void GadgetClass::Clear_Focus()
{
    if (this == s_Focused) {
        m_InputFlag &= ~KEYBOARD_INPUT;
        s_Focused = nullptr;
    }
}

BOOL GadgetClass::Has_Focus()
{
    return (this == s_Focused);
}

BOOL GadgetClass::Is_List_To_Redraw()
{
    for (GadgetClass *gadget = this; gadget != nullptr; gadget = gadget->Get_Next()) {
        if (gadget->m_ToRedraw) {
            return true;
        }
    }

    return false;
}

void GadgetClass::Set_Position(int x, int y)
{
    m_XPos = x;
    m_YPos = y;
}

void GadgetClass::Set_Size(int w, int h)
{
    m_Width = w;
    m_Height = h;
}

BOOL GadgetClass::Draw_Me(BOOL redraw)
{
    if (redraw || m_ToRedraw) {
        m_ToRedraw = false;

        return true;
    }

    return false;
}

void GadgetClass::Sticky_Process(unsigned flags)
{
    if (m_IsSticky && (flags & MOUSE_LEFT_PRESS)) {
        s_StuckOn = this;

        return;
    }

    if (this == s_StuckOn && (flags & MOUSE_LEFT_RLSE)) {
        m_ToRedraw = true;
        s_StuckOn = nullptr;

        return;
    }
}

BOOL GadgetClass::Action(unsigned flags, KeyNumType &key)
{
    if (flags != INPUT_NONE) {
        m_ToRedraw = true;
        Sticky_Process(flags);

        return true;
    }

    return false;
}

BOOL GadgetClass::Clicked_On(KeyNumType &key, unsigned flags, int x, int y)
{
    unsigned masked_flags = m_InputFlag & flags;

    if (this == s_StuckOn || (masked_flags & KEYBOARD_INPUT) != 0
        || ((masked_flags != INPUT_NONE) && (unsigned int)(x - m_XPos) < (unsigned int)m_Width
               && (unsigned int)(y - m_YPos) < (unsigned int)m_Height)) {
        return Action(masked_flags, key);
    }

    return false;
}
