/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Base of the IOMap hierachy responsible for handling message list drawing.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
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
#include "action.h"
#include "facing.h"
#include "gametypes.h"
#include "keyboard.h"
#include "theater.h"
#include "noinit.h"

enum RedrawEnum
{
    REDRAW_0 = 0,
    REDRAW_FORCE = 1,
    REDRAW_2 = 2
};

enum MouseType
{
    MOUSE_POINTER,
    MOUSE_SCROLL_N,
    MOUSE_SCROLL_NE,
    MOUSE_SCROLL_E,
    MOUSE_SCROLL_SE,
    MOUSE_SCROLL_S,
    MOUSE_SCROLL_SW,
    MOUSE_SCROLL_W,
    MOUSE_SCROLL_NW,
    MOUSE_CANT_SCROLL_N,
    MOUSE_CANT_SCROLL_NE,
    MOUSE_CANT_SCROLL_E,
    MOUSE_CANT_SCROLL_SE,
    MOUSE_CANT_SCROLL_S,
    MOUSE_CANT_SCROLL_SW,
    MOUSE_CANT_SCROLL_W,
    MOUSE_CANT_SCROLL_NW,
    MOUSE_CANT_MOVE,
    MOUSE_MOVE, // guessed name
    MOUSE_ENTER,
    MOUSE_DEPLOY,
    MOUSE_SELECT,
    MOUSE_ATTACK,
    MOUSE_SELL,
    MOUSE_SELL_UNIT,
    MOUSE_REPAIR,
    MOUSE_CANT_REPAIR,
    MOUSE_CANT_SELL,
    MOUSE_RADAR, // radar/minimap cursor?
    MOUSE_NUKE,
    MOUSE_AIR_STRIKE,
    MOUSE_SABOTAGE,
    MOUSE_GUARD_AREA,
    MOUSE_HEAL,
    MOUSE_DAMAGE,
    MOUSE_GREPAIR,
    MOUSE_ATTACK2,
    MOUSE_CANT_DEPLOY,
    MOUSE_CANT_ENTER,
    MOUSE_CANT_GREPAIR,
    MOUSE_CHRONOSPHERE, // WarpIn
    MOUSE_CHRONOWARP, // WarpOut

    MOUSE_EDITOR_BRUSH, // Special case! As we are merging the map editor and the game, and EDWIN
                        // has its own mouse shape, we need to make sure that g_InMapEditor is checked
                        // for this case and we switch the mouse shape file to EDMOUSE.SHP.
                        // In EDMOUSE.SHP, the brush has the same frame number as MOUSE_CANT_DEPLOY.

    MOUSE_COUNT,

    MOUSE_NONE = -1,
    MOUSE_FIRST = MOUSE_POINTER,
};

class GadgetClass;
class NoInitClass;
class GameINIClass;
class GraphicViewPortClass;
class HouseClass;
class ObjectClass;
class TechnoClass;
class Pipe;
class Straw;

class GameScreenClass
{
public:
    GameScreenClass();
    GameScreenClass(const NoInitClass &noinit) {}
    ~GameScreenClass();

    // Base virtual methods.
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
    virtual void Set_Default_Mouse(MouseType mouse, BOOL in_radar = false) = 0;
    virtual BOOL Override_Mouse_Shape(MouseType mouse, BOOL in_radar = false) = 0;
    virtual void Revert_Mouse_Shape() = 0;
    virtual void Mouse_Small(BOOL use_small_frame) = 0;
    virtual GraphicViewPortClass *Shadow_Address() { return ShadowPage; }

    // Additions for MapClass
    virtual void Alloc_Cells() = 0;
    virtual void Free_Cells() = 0;
    virtual void Init_Cells() = 0;
    virtual void Detach(ObjectClass *object) = 0;
    virtual void Logic_AI() = 0;
    virtual void Set_Map_Dimensions(int x, int y, int w, int h) = 0;
    virtual void Code_Pointers() = 0;
    virtual void Decode_Pointers() = 0;

    // Additions for DisplayClass
    virtual void Read_INI(GameINIClass &ini) = 0;
    // virtual void Write_INI(GameINIClass &ini) = 0; // Not virtual in RA.
    virtual BOOL Map_Cell(cell_t cellnum, HouseClass *house) = 0;
    virtual cell_t Click_Cell_Calc(int x, int y) const = 0;
    virtual void Help_Text(int str_id, int x = -1, int y = -1, int color = 14, BOOL no_wait = false) = 0;
    virtual MouseType Get_Mouse_Shape() const = 0;
    virtual BOOL Scroll_Map(DirType dir, int &distance, BOOL redraw = true) = 0;
    virtual void Refresh_Cells(cell_t cellnum, const int16_t *list) = 0;
    virtual void Set_View_Dimensions(int x, int y, int w = -1, int h = -1) = 0;
    virtual void Put_Place_Back(TechnoClass *) = 0;
    virtual void Set_Tactical_Position(coord_t location) = 0;
    virtual void Flag_Cell(cell_t cellnum) = 0;
    virtual void Mouse_Right_Press() = 0;
    virtual void Mouse_Left_Press(int mouse_x, int mouse_y) = 0;
    virtual void Mouse_Left_Up(cell_t cellnum, BOOL cell_shrouded = false, ObjectClass *object = nullptr, ActionType action = ACTION_NONE, BOOL mouse_in_radar = false) = 0;
    virtual void Mouse_Left_Held(int mouse_x, int mouse_y) = 0;
    virtual void Mouse_Left_Release(cell_t cellnum, int mouse_x, int mouse_y, ObjectClass *object = nullptr, ActionType action = ACTION_NONE, BOOL mouse_in_radar = false) = 0;

    // Additions for RadarClass
    virtual BOOL Jam_Cell(cell_t cellnum, HouseClass *house) = 0;
    virtual BOOL UnJam_Cell(cell_t cellnum, HouseClass *house) = 0;

    // Additions for MouseClass
    virtual BOOL Load(Straw &straw) = 0;
    virtual BOOL Save(Pipe &pipe) const = 0;

protected:
    int RedrawFlag;

private:
#ifdef GAME_DLL
    static GadgetClass *&Buttons;
    static GraphicViewPortClass *&ShadowPage;
#else
    static GadgetClass *Buttons;
    static GraphicViewPortClass *ShadowPage;
#endif
};

#endif // GSCREEN_H
