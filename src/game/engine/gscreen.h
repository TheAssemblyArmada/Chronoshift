/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Base of the IOMap hierachy responsible for handling message list drawing.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef GSCREEN_H
#define GSCREEN_H

#include "always.h"
#include "gadget.h"
#include "keyboard.h"
#include "mouse.h"
#include "noinit.h"
#include "theater.h"

enum RedrawEnum
{
    REDRAW_0 = 0,
    REDRAW_FORCE_MAYBE = 1,
    REDRAW_2 = 2
};

enum MouseType
{
    MOUSE_NONE = -1,
    MOUSE_POINTER = 0,
    MOUSE_SCROLL_N = 1,
    MOUSE_SCROLL_NE = 2,
    MOUSE_SCROLL_E = 3,
    MOUSE_SCROLL_SE = 4,
    MOUSE_SCROLL_S = 5,
    MOUSE_SCROLL_SW = 6,
    MOUSE_SCROLL_W = 7,
    MOUSE_SCROLL_NW = 8,
    MOUSE_CANT_SCROLL_N = 9,
    MOUSE_CANT_SCROLL_NE = 10,
    MOUSE_CANT_SCROLL_E = 11,
    MOUSE_CANT_SCROLL_SE = 12,
    MOUSE_CANT_SCROLL_S = 13,
    MOUSE_CANT_SCROLL_SW = 14,
    MOUSE_CANT_SCROLL_W = 15,
    MOUSE_CANT_SCROLL_NW = 16,
    MOUSE_CANT_MOVE = 17,
    MOUSE_MOVE = 18, // guessed name
    MOUSE_ENTER = 19,
    MOUSE_DEPLOY = 20,
    MOUSE_SELECT = 21,
    MOUSE_ATTACK = 22,
    MOUSE_SELL = 23,
    MOUSE_SELL_UNIT = 24,
    MOUSE_REPAIR = 25,
    MOUSE_CANT_REPAIR = 26,
    MOUSE_CANT_SELL = 27,
    MOUSE_RADAR = 28, // radar/minimap cursor?
    MOUSE_NUKE = 29,
    MOUSE_AIR_STRIKE = 30,
    MOUSE_SABOTAGE = 31,
    MOUSE_GUARD_AREA = 32,
    MOUSE_HEAL = 33,
    MOUSE_DAMAGE = 34,
    MOUSE_GREPAIR = 35,
    MOUSE_ATTACK2 = 36,
    MOUSE_CANT_DEPLOY = 37,
    MOUSE_CANT_ENTER = 38,
    MOUSE_CANT_GREPAIR = 39,
    MOUSE_CHRONOSPHERE = 40, // WarpIn
    MOUSE_CHRONO2 = 41, // WarpOut	//ACTION_CHRONOWARP
    MOUSE_EDITOR_BRUSH = 42, // the frame id of this is the same as "NoDeploy", but this is to only be used.
                             // when Debug_Map_Editor is true, as it used EDMOUSE.
    MOUSE_LAST = 42,
    MOUSE_COUNT = 43
};

class GraphicViewPortClass;

class GameScreenClass
{
public:
    GameScreenClass();
    GameScreenClass(NoInitClass &init) {}
    ~GameScreenClass();

    virtual void One_Time();
    virtual void Init(TheaterType theater);
    virtual void Init_Clear();
    virtual void Init_IO();
    virtual void Init_Theater(TheaterType theater) {}
    virtual void Input(KeyNumType &key, int &mouse_x, int &mouse_y);
    virtual void AI(KeyNumType &key, int mouse_x, int mouse_y);
    virtual void Add_A_Button(GadgetClass &gadget);
    virtual void Remove_A_Button(GadgetClass &gadget);
    virtual void Flag_To_Redraw(BOOL redraw = false);
    virtual void Render();
    virtual void Draw_It(BOOL force_redraw);
    virtual void Blit_Display();
    virtual void Set_Default_Mouse(MouseType mouse, BOOL a2 = false) = 0;
    virtual BOOL Override_Mouse_Shape(MouseType mouse, BOOL a2 = false) = 0;
    virtual void Revert_Mouse_Shape() = 0;
    virtual void Mouse_Small(BOOL a1) = 0;
    virtual GraphicViewPortClass *Shadow_Address() { return ShadowPage; }

protected:
    int RedrawFlag;

private:
#ifndef RAPP_STANDALONE
    static GadgetClass *&Buttons;
    static GraphicViewPortClass *&ShadowPage;
#else
    static GadgetClass *Buttons;
    static GraphicViewPortClass *ShadowPage;
#endif
};

#endif // GSCREEN_H
