/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base class for UI elements, provides basic input handling.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef GADGET_H
#define GADGET_H

#include "keyboard.h"
#include "link.h"
#include "trect.h"

class ControlClass;
struct RemapControlType;

enum GadgetInput
{
    INPUT_NONE = 0x0,
    MOUSE_LEFT_PRESS = 0x1,
    MOUSE_LEFT_HELD = 0x2,
    MOUSE_LEFT_RLSE = 0x4,
    MOUSE_LEFT_UP = 0x8,
    MOUSE_RIGHT_PRESS = 0x10,
    MOUSE_RIGHT_HELD = 0x20,
    MOUSE_RIGHT_RLSE = 0x40, // this allows Action() to be called at one place
    MOUSE_RIGHT_UP = 0x80,
    KEYBOARD_INPUT = 0x100, // Respond to none mouse event?
};

DEFINE_ENUMERATION_BITWISE_OPERATORS(GadgetInput);

class GadgetClass : public LinkClass
{
public:
    GadgetClass() {}
    GadgetClass(unsigned input_flag, BOOL sticky = false);
    GadgetClass(int x, int y, int w, int h, unsigned input_flag, BOOL sticky = false);
    GadgetClass(GadgetClass &that);
    virtual ~GadgetClass();

    virtual GadgetClass *const Get_Next() const;
    virtual GadgetClass *const Get_Prev() const;
    virtual GadgetClass *Remove();
    virtual KeyNumType Input();
    virtual void Draw_All(BOOL redraw);
    virtual void Delete_List();
    virtual ControlClass *Extract_Gadget(unsigned id);
    virtual void Flag_List_To_Redraw();
    virtual void Disable();
    virtual void Enable();
    virtual unsigned Get_ID() const;
    virtual void Flag_To_Redraw();
    virtual void Peer_To_Peer(unsigned flags, KeyNumType &key, ControlClass &peer);
    virtual void Set_Focus();
    virtual void Clear_Focus();
    virtual BOOL Has_Focus();
    virtual BOOL Is_List_To_Redraw();
    virtual BOOL Is_To_Redraw() { return ToRedraw; }
    virtual void Set_Position(int x, int y);
    virtual void Set_Size(int w, int h);
    virtual BOOL Draw_Me(BOOL redraw);
    virtual void Sticky_Process(unsigned flags);
    virtual BOOL Action(unsigned flags, KeyNumType &key);
    virtual BOOL Clicked_On(KeyNumType &a1, unsigned flags, int x, int y);

    GadgetClass &operator=(GadgetClass &that);

    static RemapControlType *Get_Color_Scheme() { return ColorScheme; }
    static void Set_Color_Scheme(RemapControlType *remap) { ColorScheme = remap; }

protected:
    static RemapControlType *ColorScheme;
    static GadgetClass *StuckOn;
    static GadgetClass *LastList;
    static GadgetClass *Focused;

protected:
    int XPos;
    int YPos;
    int Width;
    int Height;

#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool ToRedraw : 1; // & 1
            bool IsSticky : 1; // & 2
            bool IsDisabled : 1; // & 4
        };
        int m_gadgetFlags;
    };
#else
    bool ToRedraw;
    bool IsSticky;
    bool IsDisabled;
#endif

    unsigned InputFlag;
};

#endif // GADGET_H
