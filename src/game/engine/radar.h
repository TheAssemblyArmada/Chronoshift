/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling radar (minimap when in game).
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
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
#include "noinit.h"

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
    RadarClass(NoInitClass &noinit) : DisplayClass(noinit) {}

    virtual void One_Time() override;
    virtual void Init_Clear() override;
    virtual void AI(KeyNumType &key, int mouse_x, int mouse_y) override;
    virtual void Draw_It(BOOL force_redraw) override;
    virtual void Set_Map_Dimensions(int x, int y, int w, int h) override;
    virtual BOOL Map_Cell(int16_t cellnum, HouseClass *house) override;
    virtual int16_t Click_Cell_Calc(int x, int y) const override;
    virtual void Refresh_Cells(int16_t cellnum, int16_t *overlap_list) override;
    virtual void Set_Tactical_Position(uint32_t location) override;
    virtual void Flag_Cell(int16_t cellnum) override;
    virtual BOOL Jam_Cell(int16_t cellnum, HouseClass *house) override;
    virtual BOOL UnJam_Cell(int16_t cellnum, HouseClass *house) override;

    BOOL Is_Radar_Jammed();
    BOOL Is_Radar_Active();
    BOOL Is_Radar_Existing();
    void Activate_Pulse();
    void Radar_Pixel(int16_t cellnum);
    void Render_Terrain(int16_t cellnum, int x, int y, int scale);
    void Render_Infantry(int16_t cellnum, int x, int y, int scale);
    void Render_Overlay(int16_t cellnum, int x, int y, int scale);
    void Zoom_Mode(int16_t cellnum);
    BOOL const Is_Zoomable() const;
    int const Click_In_Radar(int &x, int &y, BOOL set_coords = false) const;
    BOOL const Cell_On_Radar(int16_t cellnum);
    void Set_Radar_Position(int16_t cellnum);
    int16_t Radar_Position();
    void Cell_XY_To_Radar_Pixel(int cell_x, int cell_y, int &x, int &y);
    void Radar_Anim();
    void Radar_Cursor(BOOL redraw);
    void Player_Names(BOOL draw = true);
    BOOL Draw_House_Info();
    void Draw_Names();
    void Mark_Radar(int left, int top, int right, int bottom, BOOL mark, int size);
    void Cursor_Cell(int16_t cellnum, BOOL mark);
    void Plot_Radar_Pixel(int16_t cellnum);
    int Radar_Activate(int mode = RADAR_M1);
    BOOL Spy_Next_House();

#ifndef RAPP_STANDALONE
    static void Hook_Me();
    int16_t Hook_Click_Cell_Calc(int x, int y) { return RadarClass::Click_Cell_Calc(x, y); }
    static BOOL Hook_Is_Zoomable(RadarClass *ptr) { return ptr->Is_Zoomable(); }
    static BOOL Hook_Cell_On_Radar(RadarClass *ptr, int16_t cellnum) { return ptr->Cell_On_Radar(cellnum); }
#endif

protected:
    int RadarButtonXPos; // 0xC88						//radar frame left?
    int MinRadarX; // 0xC8C MinimapX			//radar map offset left?
    int RadarButtonYPos; // 0xC90						//radar frame top?
    int MinRadarY; // 0xC94 MinimapY			//radar map offset top?
    int RadarButtonHeight; // 0xC98
    int RadarButtonWidth; // 0xC9C
    int MaxRadarWidth; // 0xCA0 MaxRadarWidth?
    int MaxRadarHeight; // 0xCA4 MaxRadarHeight?
    int field_CA8; // 0xCA8
    int field_CAC; // 0xCAC

#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            // bitfield 0xCB0
            bool RadarToRedraw : 1; // 1
            bool RadarCursorRedraw : 1; // 2
            bool RadarExists : 1; // 4 Low power?
            bool RadarActive : 1; // enabling from power off?
            bool RadarActivating : 1; // 16 //RadarActivating?
            bool RadarDeactivating : 1; // 32 //RadarDeactivating?
            bool RadarJammed : 1; // 64
            bool RadarPulseActive : 1; // 128 this is also set when the radar is being jammed? see HouseClass::AI
            // bitfield 0xCB1
            bool RadarZoomed : 1; // 1 Zoomed?
            bool RadarDrawNames : 1; // 2
            bool RadarDrawSpiedInfo : 1; // 4 tomsons said this does some type of redraw?
            bool RadarBit2_8 : 1; // 8
            bool RadarBit2_16 : 1; // 16
            bool RadarBit2_32 : 1; // 32
            bool RadarBit2_64 : 1; // 64
            bool RadarBit2_128 : 1; // 128
        };
        int Bitfield;
    };
#else
    // bitfield 0xCB0
    bool RadarToRedraw; // 1
    bool RadarCursorRedraw; // 2
    bool RadarExists; // 4 Low power?
    bool RadarActive; // enabling from power off?
    bool RadarActivating; // 16 //RadarActivating?
    bool RadarDeactivating; // 32 //RadarDeactivating?
    bool RadarJammed; // 64
    bool RadarPulseActive; // 128 this is also set when the radar is being jammed? see HouseClass::AI
    // bitfield 0xCB1
    bool RadarZoomed; // 1 Zoomed?
    bool RadarDrawNames; // 2
    bool RadarDrawSpiedInfo; // 4 tomsons said this does some type of redraw?
    bool RadarBit2_8; //
    bool RadarBit2_16; // 16
    bool RadarBit2_32; // 32
    bool RadarBit2_64; // 64
    bool RadarBit2_128; // 128
#endif
    int RadarPulseFrame; // 0xCB4
    int RadarCursorFrame; // bytes relating to region box focus in animation.
    int RadarAnimFrame; // 0xCBC
    int MiniMapXOffset; // TODO, convert to rect
    int MiniMapYOffset;
    int MiniMapCellWidth;
    int MiniMapCellHeight;
    int16_t MiniMapCell;
    char padding[2]; // needed cause someone made MiniMapCell a int but Radar_Position returns it, its a short
    TRect<int> MiniMap; // 0xCD4
    int MiniMapScale; // 0xCE4 scale?
    HousesType SpiedHouse; // HousesType
    int MiniMapCellCount;
    int16_t MiniMapCells[400];

#ifndef RAPP_STANDALONE
    static RTacticalClass &RadarButton;
    static void *&RadarAnim;
    static void *&RadarPulse;
    static void *&RadarFrame;
    static BOOL &FullRedraw;
    static GraphicBufferClass &IconStage;
    static GraphicBufferClass &TileStage;
#else
    static TacticalClass RadarButton;
    static void *RadarAnim;
    static void *RadarPulse;
    static void *RadarFrame;
    static bool FullRedraw;
    static GraphicBufferClass IconStage;
    static GraphicBufferClass TileStage;
#endif
};

#ifndef RAPP_STANALONE
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
#endif
}
#endif

#endif // RADAR_H
