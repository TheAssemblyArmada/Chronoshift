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
        RADAR_CURSOR_SIZE = 6,
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
    virtual void Refresh_Cells(cell_t cellnum, const int16_t *list) override;
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

    int Get_Radar_Button_X() const { return m_RadarButtonXPos; }
    int Get_Radar_Button_Y() const { return m_RadarButtonYPos; }
    int Get_Min_Radar_X() const { return m_MinRadarX; }
    int Get_Min_Radar_Y() const { return m_MinRadarY; }
    int Get_Max_Radar_Width() const { return m_MaxRadarWidth; }
    int Get_Max_Radar_Height() const { return m_MaxRadarHeight; }
    BOOL Radar_Active() const { return m_RadarActive; }
    BOOL Radar_Zoomed() const { return m_RadarZoomed; }
    BOOL Radar_Draw_Names() const { return m_RadarDrawNames; }
    BOOL Radar_Jammed() const { return m_RadarJammed; }

    void Set_Cursor_Frame(int frame) { m_RadarCursorFrame = frame; }
    void Set_Jammed(BOOL jammed) { m_RadarJammed = jammed; }

#ifdef GAME_DLL
    cell_t Hook_Click_Cell_Calc(int x, int y) { return RadarClass::Click_Cell_Calc(x, y); }
    BOOL Hook_Is_Zoomable() { return Is_Zoomable(); }
    BOOL Hook_Cell_On_Radar(cell_t cellnum) { return Cell_On_Radar(cellnum); }
    int Hook_Click_In_Radar(int &x, int &y, BOOL set_coords = false) { return RadarClass::Click_In_Radar(x, y, set_coords); }
#endif

protected:
    int m_RadarButtonXPos;
    int m_MinRadarX;
    int m_RadarButtonYPos;
    int m_MinRadarY;
    int m_RadarButtonWidth;
    int m_RadarButtonHeight;
    int m_MaxRadarWidth;
    int m_MaxRadarHeight;
    int m_field_CA8;
    int m_field_CAC;

#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_RadarToRedraw : 1; // 1
    BOOL m_RadarCursorRedraw : 1; // 2
    BOOL m_RadarExists : 1; // 4
    BOOL m_RadarActive : 1; // 8
    BOOL m_RadarActivating : 1; // 16
    BOOL m_RadarDeactivating : 1; // 32
    BOOL m_RadarJammed : 1; // 64
    BOOL m_RadarPulseActive : 1; // 128

    BOOL m_RadarZoomed : 1; // 1
    BOOL m_RadarDrawNames : 1; // 2
    BOOL m_RadarDrawSpiedInfo : 1; // 4
    BOOL m_RadarBit2_8 : 1; // 8
    BOOL m_RadarBit2_16 : 1; // 16
    BOOL m_RadarBit2_32 : 1; // 32
    BOOL m_RadarBit2_64 : 1; // 64
    BOOL m_RadarBit2_128 : 1; // 128
#else
    bool m_RadarToRedraw;
    bool m_RadarCursorRedraw;
    bool m_RadarExists;
    bool m_RadarActive;
    bool m_RadarActivating;
    bool m_RadarDeactivating;
    bool m_RadarJammed;
    bool m_RadarPulseActive;
    bool m_RadarZoomed;
    bool m_RadarDrawNames;
    bool m_RadarDrawSpiedInfo;
    bool m_RadarBit2_8;
    bool m_RadarBit2_16;
    bool m_RadarBit2_32;
    bool m_RadarBit2_64;
    bool m_RadarBit2_128;
#endif
    int m_RadarPulseFrame;
    int m_RadarCursorFrame; // bytes relating to region box focus in animation.
    int m_RadarAnimFrame;
    int m_MiniMapXOffset;
    int m_MiniMapYOffset;
    int m_MiniMapCellWidth;
    int m_MiniMapCellHeight;
    cell_t m_MiniMapCell;
#ifdef GAME_DLL
    char padding[2]; // needed for DLL builds because original has m_MiniMapCell a int, but Radar_Position returns it as a
                     // short.
#endif
    TRect<int> m_MiniMap;
    int m_MiniMapScale;
    HousesType m_SpiedHouse;
    int m_MiniMapCellCount;
    cell_t m_MiniMapCells[MINI_MAP_CELLS];
    static GraphicBufferClass s_IconStage;

#ifdef GAME_DLL
    static RTacticalClass &s_RadarButton;
    static void *&s_RadarAnim;
    static void *&s_RadarPulse;
    static void *&s_RadarFrame;
    static BOOL &s_FullRedraw;
    static GraphicBufferClass &s_TileStage;
#else
    static RTacticalClass s_RadarButton;
    static void *s_RadarAnim;
    static void *s_RadarPulse;
    static void *s_RadarFrame;
    static BOOL s_FullRedraw;
    static GraphicBufferClass s_TileStage;
#endif
};

#endif // RADAR_H
