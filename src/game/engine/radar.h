/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling radar (minimap when in game).
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef RADAR_H
#define RADAR_H

#include "always.h"
#include "display.h"
#include "gbuffer.h"

enum RadarStateType
{
    RADAR_M1 = -1, // Toggle active and inactive
    RADAR_0 = 0, // Turn radar off
    RADAR_1 = 1, // Turn radar on
    RADAR_2 = 2, // Radar off no side bar.
    RADAR_3 = 3, // Radar on no side bar
    RADAR_4 = 4, // Remove radar
};

enum RadarClickEnum
{
    RADAR_CLICK_M1 = -1,
    RADAR_CLICK_0 = 0,
    RADAR_CLICK_1 = 1
};

class RadarClass : public DisplayClass
{
    enum
    {
        MINI_MAP_CELLS = 400,
    };

    // Internal gadget class for handling input to the radar area of the game screen.
    class RTacticalClass : public GadgetClass
    {
    public:
        RTacticalClass();
        RTacticalClass(RTacticalClass &that) : GadgetClass(that) {}
        virtual ~RTacticalClass() {}

        virtual BOOL Action(unsigned flags, KeyNumType &key) override;
    };

public:
    RadarClass();
    RadarClass(const NoInitClass &noinit) : DisplayClass(noinit) {}

    virtual void One_Time() override;
    virtual void Init_Clear() override;
    virtual void AI(KeyNumType &key, int mouse_x, int mouse_y) override;
    virtual void Draw_It(BOOL force_redraw) override;
    virtual void Set_Map_Dimensions(int x, int y, int w, int h) override;
    virtual BOOL Map_Cell(cell_t cellnum, HouseClass *house) override;
    virtual cell_t Click_Cell_Calc(int x, int y) const override;
    virtual void Refresh_Cells(cell_t cellnum, int16_t *overlap_list) override;
    virtual void Set_Tactical_Position(coord_t location) override;
    virtual void Flag_Cell(cell_t cellnum) override;
    virtual BOOL Jam_Cell(cell_t cellnum, HouseClass *house) override;
    virtual BOOL UnJam_Cell(cell_t cellnum, HouseClass *house) override;

    BOOL Is_Radar_Jammed();
    BOOL Is_Radar_Active();
    BOOL Is_Radar_Existing();
    void Activate_Pulse();
    void Radar_Pixel(cell_t cellnum);
    void Render_Terrain(cell_t cellnum, int x, int y, int scale);
    void Render_Infantry(cell_t cellnum, int x, int y, int scale);
    void Render_Overlay(cell_t cellnum, int x, int y, int scale);
    void Zoom_Mode(cell_t cellnum);
    BOOL Is_Zoomable() const;
    int Click_In_Radar(int &x, int &y, BOOL set_coords = false) const;
    BOOL Cell_On_Radar(cell_t cellnum);
    void Set_Radar_Position(cell_t cellnum);
    cell_t Radar_Position();
    void Cell_XY_To_Radar_Pixel(int cell_x, int cell_y, int &x, int &y);
    void Radar_Anim();
    void Radar_Cursor(BOOL redraw);
    void Player_Names(BOOL draw = true);
    BOOL Draw_House_Info();
    void Draw_Names();
    void Mark_Radar(int left, int top, int right, int bottom, BOOL mark, int size);
    void Cursor_Cell(cell_t cellnum, BOOL mark);
    void Plot_Radar_Pixel(cell_t cellnum);
    int Radar_Activate(int mode = RADAR_M1);
    BOOL Spy_Next_House();

#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
    cell_t Hook_Click_Cell_Calc(int x, int y) { return RadarClass::Click_Cell_Calc(x, y); }
    BOOL Hook_Is_Zoomable() { return Is_Zoomable(); }
    BOOL Hook_Cell_On_Radar(cell_t cellnum) { return Cell_On_Radar(cellnum); }
    int Hook_Click_In_Radar(int &x, int &y, BOOL set_coords = false) { return RadarClass::Click_In_Radar(x, y, set_coords); }

#endif

protected:
    int RadarButtonXPos;
    int MinRadarX;
    int RadarButtonYPos;
    int MinRadarY;
    int RadarButtonWidth;
    int RadarButtonHeight;
    int MaxRadarWidth;
    int MaxRadarHeight;
    int field_CA8;
    int field_CAC;

#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL RadarToRedraw : 1; // 1
    BOOL RadarCursorRedraw : 1; // 2
    BOOL RadarExists : 1; // 4
    BOOL RadarActive : 1; // 8
    BOOL RadarActivating : 1; // 16
    BOOL RadarDeactivating : 1; // 32
    BOOL RadarJammed : 1; // 64
    BOOL RadarPulseActive : 1; // 128

    BOOL RadarZoomed : 1; // 1
    BOOL RadarDrawNames : 1; // 2
    BOOL RadarDrawSpiedInfo : 1; // 4
    BOOL RadarBit2_8 : 1; // 8
    BOOL RadarBit2_16 : 1; // 16
    BOOL RadarBit2_32 : 1; // 32
    BOOL RadarBit2_64 : 1; // 64
    BOOL RadarBit2_128 : 1; // 128
#else
    bool RadarToRedraw;
    bool RadarCursorRedraw;
    bool RadarExists;
    bool RadarActive;
    bool RadarActivating;
    bool RadarDeactivating;
    bool RadarJammed;
    bool RadarPulseActive;
    bool RadarZoomed;
    bool RadarDrawNames;
    bool RadarDrawSpiedInfo;
    bool RadarBit2_8;
    bool RadarBit2_16;
    bool RadarBit2_32;
    bool RadarBit2_64;
    bool RadarBit2_128;
#endif
    int RadarPulseFrame;
    int RadarCursorFrame; // bytes relating to region box focus in animation.
    int RadarAnimFrame;
    int MiniMapXOffset;
    int MiniMapYOffset;
    int MiniMapCellWidth;
    int MiniMapCellHeight;
    cell_t MiniMapCell;
    char padding[2]; // needed cause someone made MiniMapCell a int but Radar_Position returns it, its a short
    TRect<int> MiniMap;
    int MiniMapScale;
    HousesType SpiedHouse;
    int MiniMapCellCount;
    cell_t MiniMapCells[MINI_MAP_CELLS];
    static GraphicBufferClass IconStage;

#ifndef CHRONOSHIFT_STANDALONE
    static RTacticalClass &RadarButton;
    static void *&RadarAnim;
    static void *&RadarPulse;
    static void *&RadarFrame;
    static BOOL &FullRedraw;
    static GraphicBufferClass &TileStage;
#else
    static RTacticalClass RadarButton;
    static void *RadarAnim;
    static void *RadarPulse;
    static void *RadarFrame;
    static BOOL FullRedraw;
    static GraphicBufferClass TileStage;
#endif
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
inline void RadarClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x0052D668, *RadarClass::One_Time); // seems to work
    Hook_Function(0x0052D718, *RadarClass::Init_Clear); // seems to work
    Hook_Function(0x00532010, *RadarClass::Set_Map_Dimensions); // seems to work
    Hook_Function(0x0052FEAC, *RadarClass::Map_Cell); // seems to work
    Hook_Function(0x0052FE28, *RadarClass::Hook_Click_Cell_Calc); // seems to work
    Hook_Function(0x00531474, *RadarClass::Refresh_Cells); // seems to work
    Hook_Function(0x00532070, *RadarClass::Set_Tactical_Position); // seems to work
    Hook_Function(0x005329EC, *RadarClass::Flag_Cell); // seems to work
    Hook_Function(0x00532938, *RadarClass::Activate_Pulse); // seems to work
    Hook_Function(0x0052F460, *RadarClass::Hook_Is_Zoomable); // seems to work
    Hook_Function(0x00532098, *RadarClass::Hook_Cell_On_Radar); // seems to work
    Hook_Function(0x00532000, *RadarClass::Radar_Position); // seems to work
    Hook_Function(0x005301B4, *RadarClass::Cell_XY_To_Radar_Pixel); // seems to work
    Hook_Function(0x0053210C, *RadarClass::Player_Names); // seems to work
    Hook_Function(0x0052FF40, *RadarClass::Mark_Radar); // seems to work
    Hook_Function(0x0052FED8, *RadarClass::Cursor_Cell); // seems to work
    Hook_Function(0x0052EF8C, *RadarClass::Render_Overlay); // works
    Hook_Function(0x00530300, *RadarClass::Radar_Cursor); // works
    Hook_Function(0x0052F294, *RadarClass::Zoom_Mode); // 
    Hook_Function(0x0052FCC0, *RadarClass::Radar_Pixel); // 
    Hook_Function(0x0052FD3C, *RadarClass::Hook_Click_In_Radar); //
    Hook_Function(0x0052D790, *RadarClass::Radar_Activate); //
#endif
}
#endif

#endif // RADAR_H
