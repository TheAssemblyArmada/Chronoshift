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
#include "remap.h"
#include "trect.h"

class ControlClass;

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

    virtual GadgetClass *Get_Next() const override;
    virtual GadgetClass *Get_Prev() const override;
    virtual GadgetClass *Remove() override;
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
    virtual BOOL Draw_Me(BOOL redraw);
    virtual void Sticky_Process(unsigned flags);
    virtual BOOL Action(unsigned flags, KeyNumType &key);
    virtual BOOL Clicked_On(KeyNumType &a1, unsigned flags, int x, int y);

    GadgetClass &operator=(GadgetClass &that);

    void Set_Size(int w, int h);
    void Set_XPos(int x) { XPos = x; }
    void Set_YPos(int y) { YPos = y; }
    void Set_Width(int w) { Width = w; }
    void Set_Height(int h) { Height = h; }
    int Get_XPos() { return XPos; }
    int Get_YPos() { return YPos; }
    int Get_Width() { return Width; }
    int Get_Height() { return Height; }

    static RemapControlType *Get_Color_Scheme() { return ColorScheme; }
    static void Set_Color_Scheme(RemapControlType *remap) { ColorScheme = remap; }

#ifndef RAPP_STANDALONE
    static void Hook_Me();
#endif

protected:
#ifndef RAPP_STANDALONE
    static RemapControlType *&ColorScheme;
    static GadgetClass *&StuckOn;
    static GadgetClass *&LastList;
    static GadgetClass *&Focused;
#else
    static RemapControlType *ColorScheme;
    static GadgetClass *StuckOn;
    static GadgetClass *LastList;
    static GadgetClass *Focused;
#endif

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

#ifndef RAPP_STANDALONE
#include "hooker.h"

inline void GadgetClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x004C3E40, *GadgetClass::Remove);
    Hook_Function(0x004C3F38, *GadgetClass::Input);
    Hook_Function(0x004C3F08, *GadgetClass::Draw_All);
    Hook_Function(0x004C3E74, *GadgetClass::Delete_List);
    Hook_Function(0x004C4160, *GadgetClass::Extract_Gadget);
    Hook_Function(0x004A2810, *GadgetClass::Flag_List_To_Redraw);
    Hook_Function(0x004C3E2C, *GadgetClass::Disable);
    Hook_Function(0x004C3E08, *GadgetClass::Enable);
    //Hook_Function(0x004B5950, *GadgetClass::Get_ID);
    Hook_Function(0x004C4198, *GadgetClass::Flag_To_Redraw);
    Hook_Function(0x004AC370, *GadgetClass::Peer_To_Peer);
    Hook_Function(0x004C41D4, *GadgetClass::Set_Focus);
    Hook_Function(0x004C4210, *GadgetClass::Clear_Focus);
    Hook_Function(0x004C423C, *GadgetClass::Has_Focus);
    Hook_Function(0x004C4254, *GadgetClass::Is_List_To_Redraw);
    Hook_Function(0x004A2830, *GadgetClass::Is_To_Redraw);
    Hook_Function(0x004C4280, *GadgetClass::Set_Position);
    Hook_Function(0x004C3EE8, *GadgetClass::Draw_Me);
    Hook_Function(0x004C41A4, *GadgetClass::Sticky_Process); //issue
    Hook_Function(0x004C3EC8, *GadgetClass::Action);
    Hook_Function(0x004C3DB4, *GadgetClass::Clicked_On); //issue

#endif
}
#endif

#endif // GADGET_H
