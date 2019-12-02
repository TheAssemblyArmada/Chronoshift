/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling tactical display (main action pane when in game).
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef DISPLAY_H
#define DISPLAY_H

#include "always.h"
#include "gadget.h"
#include "map.h"
#include "source.h"
#include "trect.h"

class BufferClass;

enum ModeControl
{
    MODE_CONTROL_TOGGLE = -1,
    MODE_CONTROL_OFF = 0,
    MODE_CONTROL_ON = 1
};

class DisplayClass : public MapClass
{
protected:
    // Internal gadget class for handling input to the tactical area of the game screen.
    class TacticalClass : public GadgetClass
    {
    public:
        TacticalClass();
        TacticalClass(TacticalClass &that) : GadgetClass(that) {}
        virtual ~TacticalClass() {}

        virtual BOOL Action(unsigned flags, KeyNumType &key) override;
    };

public:
    DisplayClass();
    DisplayClass(const NoInitClass &noinit) : MapClass(noinit) {}

    virtual void One_Time() override;
    virtual void Init_Clear() override;
    virtual void Init_IO() override;
    virtual void Init_Theater(TheaterType theater) override;
    virtual void AI(KeyNumType &key, int mouse_x, int mouse_y) override;
    virtual void Draw_It(BOOL force_redraw) override;
    virtual void Code_Pointers() override;
    virtual void Decode_Pointers() override;

    virtual void Read_INI(GameINIClass &ini) override;
    virtual BOOL Map_Cell(cell_t cellnum, HouseClass *house) override;
    virtual cell_t Click_Cell_Calc(int x, int y) const override;
    virtual void Help_Text(int str_id, int x = -1, int y = -1, int color = 14, BOOL no_wait = false) override {}
    virtual BOOL Scroll_Map(DirType dir, int &distance, BOOL redraw = true) override;
    virtual void Refresh_Cells(cell_t cellnum, const int16_t *list) override;
    virtual void Set_View_Dimensions(int x, int y, int w = -1, int h = -1) override;
    virtual void Put_Place_Back(TechnoClass *obj) override {}
    virtual void Set_Tactical_Position(coord_t location) override;
    virtual void Flag_Cell(cell_t cellnum) override;
    virtual void Mouse_Right_Press() override;
    virtual void Mouse_Left_Press(int mouse_x, int mouse_y) override;
    virtual void Mouse_Left_Up(cell_t cellnum, BOOL cell_shrouded = false, ObjectClass *object = nullptr,
        ActionType action = ACTION_NONE, BOOL mouse_in_radar = false) override;
    virtual void Mouse_Left_Held(int mouse_x, int mouse_y) override;
    virtual void Mouse_Left_Release(cell_t cellnum, int mouse_x, int mouse_y, ObjectClass *object = nullptr,
        ActionType action = ACTION_NONE, BOOL mouse_in_radar = false) override;
    virtual void Write_INI(GameINIClass &ini) override;

    //void Write_INI(GameINIClass &ini); // Not virtual in RA.
    coord_t Pixel_To_Coord(int x, int y);
    void Set_Cursor_Shape(int16_t *list = nullptr);
    void Refresh_Band();
    void Cursor_Mark(cell_t cellnum, BOOL flag);
    void Get_Occupy_Dimensions(int &w, int &h, int16_t *list) const;
    const int16_t *Text_Overlap_List(const char *string, int x, int y) const;
    ObjectClass *Prev_Object(ObjectClass *object) const;
    ObjectClass *Next_Object(ObjectClass *object) const;
    void Submit(ObjectClass *object, LayerType layer);
    void Remove(ObjectClass *object, LayerType layer);
    ObjectClass *Cell_Object(cell_t cellnum, int x, int y) const;
    void Select_These(coord_t start, coord_t finish);
    void Sell_Mode_Control(int mode);
    void Repair_Mode_Control(int mode);
    void Center_Map(coord_t coord = 0);
    cell_t Set_Cursor_Pos(cell_t cell);
    BOOL Passes_Proximity_Check(ObjectTypeClass *object, HousesType house, int16_t *list, cell_t cell) const;
    void Redraw_Icons();
    void Redraw_Shadow();
    BOOL In_View(cell_t cellnum) const;
    BOOL Coord_To_Pixel(coord_t coord, int &x, int &y) const;
    coord_t Pixel_To_Coord(int x, int y) const;
    int Cell_Shadow(cell_t cellnum) const;
    BOOL Push_Onto_TacMap(coord_t &coord1, coord_t &coord2);
    cell_t Calculated_Cell(
        SourceType source, int waypoint, cell_t cellnum, SpeedType speed, BOOL use_zone, MZoneType mzone) const;
    BOOL Good_Reinforcement_Cell(cell_t cell1, cell_t cell2, SpeedType speed, int zone, MZoneType mzone) const;
    coord_t Closest_Free_Spot(coord_t coord, BOOL skip_occupied) const;
    void All_To_Look(BOOL skip_buildings);
    void Constrained_Look(coord_t coord, int constraint);
    void Encroach_Shadow();
    void Shroud_Cell(cell_t cellnum);

    void All_Layers_To_Redraw();
    void Flag_All_Cells_To_Redraw();
    static BOOL Is_Cell_Flagged(cell_t cellnum) { return CellRedraw.Is_True(cellnum); }

    lepton_t Get_DisplayWidth() const { return m_DisplayWidth; }
    lepton_t Get_DisplayHeight() const { return m_DisplayHeight; }
    cell_t Get_Cursor_Start() const { return m_DisplayCursorStart; }
    cell_t Get_Cursor_End() const { return m_DisplayCursorEnd; }
    BOOL Passed_Proximity_Check() const { return m_PassedProximityCheck; }
    ObjectClass *Pending_Object() const { return m_PendingObjectPtr; }
    ObjectTypeClass *Pending_ObjectType() const { return m_PendingObjectTypePtr; }
    HousesType Pending_Object_Owner() const { return m_PendingObjectOwner; }
    SpecialWeaponType Pending_Super() const { return m_PendingSuper; }
    void Set_Pending_Super(SpecialWeaponType type) { m_PendingSuper = type; }
    void Reset_Pending_Object();
    int Tac_Offset_X() const { return m_TacOffsetX; }
    int Tac_Offset_Y() const { return m_TacOffsetY; }
    coord_t New_Pos() const { return m_DisplayNewPos; }
    void Display_To_Redraw() { m_DisplayToRedraw = true; }
    BOOL To_Redraw() const { return m_DisplayToRedraw; }
    BOOL Repair_Mode() const { return m_DisplayRepairMode; }
    BOOL Sell_Mode() const { return m_DisplaySellMode; }
    BOOL Bit_8() const { return m_DisplayBit8; }

private:
    // This only seems to be used by DisplayClass, so made it a static helper of this class.
    static int __cdecl Clip_Rect(int &x, int &y, int &w, int &h, int clip_w, int clip_h);
    void Check_Proximity(ObjectTypeClass *object, HousesType house, cell_t cell, int &passes, bool &outside_radar) const;
    BOOL Is_Spot_Free(coord_t coord) const;

 protected:
    coord_t m_DisplayPos; // Coord of top left of tactical display within the map.
    lepton_t m_DisplayWidth; // Width of display in leptons.
    lepton_t m_DisplayHeight; // Height of display in leptons.
    cell_t m_DisplayCursorStart;
    cell_t m_DisplayCursorEnd;
    int16_t *m_DisplayCursorOccupy;
    BOOL m_PassedProximityCheck;
    ObjectClass *m_PendingObjectPtr;
    ObjectTypeClass *m_PendingObjectTypePtr;
    HousesType m_PendingObjectOwner;
    int m_TacOffsetX;
    int m_TacOffsetY;
    coord_t m_DisplayNewPos;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_DisplayToRedraw : 1; // 1
    BOOL m_DisplayRepairMode : 1; // 2
    BOOL m_DisplaySellMode : 1; // 4
    BOOL m_DisplayBit8 : 1; // 8 This is Bit32 in SS. passes proximity check/can place?
    BOOL m_DisplayBit16 : 1; // 16
    BOOL m_RedrawShadow : 1; // 32 this is Bit128 in SS
#else
    bool m_DisplayToRedraw;
    bool m_DisplayRepairMode;
    bool m_DisplaySellMode;
    bool m_DisplayBit8; // This is Bit32 in SS. passes proximity check/can place? Could also be if mouse is pressed/dragging a
                      // band box?
    bool m_DisplayBit16;
    bool m_RedrawShadow; // this is Bit128 in SS
#endif
    SpecialWeaponType m_PendingSuper; // Current superweapon selected to fire.
    TRect<int> m_BandBox; // Dimensions of the selection band box.

public:
#ifdef GAME_DLL
    static LayerClass *Layers;
    static TacticalClass &TacticalButton;
    static ARRAY_DEC(uint8_t, FadingBrighten, 256);
    static ARRAY_DEC(uint8_t, FadingShade, 256);
    static ARRAY_DEC(uint8_t, FadingWayDark, 256);
    static ARRAY_DEC(uint8_t, FadingLight, 256);
    static ARRAY_DEC(uint8_t, FadingGreen, 256);
    static ARRAY_DEC(uint8_t, FadingYellow, 256);
    static ARRAY_DEC(uint8_t, FadingRed, 256);
    static ARRAY2D_DEC(uint8_t, TranslucentTable, 14, 256);
    static ARRAY2D_DEC(uint8_t, WhiteTranslucentTable, 2, 256);
    static ARRAY2D_DEC(uint8_t, MouseTranslucentTable, 5, 256);
    static ARRAY2D_DEC(uint8_t, ShadowTrans, 5, 256);
    static ARRAY2D_DEC(uint8_t, UnitShadow, 2, 256);
    static ARRAY2D_DEC(uint8_t, UnitShadowAir, 2, 256);
    static ARRAY2D_DEC(uint8_t, SpecialGhost, 2, 256);
    static void *&TransIconset;
    static void *&ShadowShapes;
    static BufferClass *&TheaterBuffer;
    static BooleanVectorClass &CellRedraw;
#else
    static LayerClass Layers[LAYER_COUNT];
    static TacticalClass TacticalButton;
    static uint8_t FadingBrighten[256];
    static uint8_t FadingShade[256];
    static uint8_t FadingWayDark[256];
    static uint8_t FadingLight[256];
    static uint8_t FadingGreen[256];
    static uint8_t FadingYellow[256];
    static uint8_t FadingRed[256];
    static uint8_t TranslucentTable[14][256];
    static uint8_t WhiteTranslucentTable[2][256];
    static uint8_t MouseTranslucentTable[5][256];
    static uint8_t ShadowTrans[5][256];
    static uint8_t UnitShadow[2][256];
    static uint8_t UnitShadowAir[2][256];
    static uint8_t SpecialGhost[2][256];
    static void *TransIconset;
    static void *ShadowShapes;
    static BufferClass *TheaterBuffer;
    static BooleanVectorClass CellRedraw;
#endif
    static char FadingWhite[256];

#ifdef GAME_DLL
    cell_t Hook_Click_Cell_Calc(int x, int y) { return DisplayClass::Click_Cell_Calc(x, y); }
    const int16_t *Hook_Text_Overlap_List(const char *string, int x, int y)
    {
        return DisplayClass::Text_Overlap_List(string, x, y);
    }

    void Hook_Get_Occupy_Dimensions(int &w, int &h, int16_t *list) { DisplayClass::Get_Occupy_Dimensions(w, h, list); }

    ObjectClass *Hook_Cell_Object(cell_t cellnum, int x, int y) { return DisplayClass::Cell_Object(cellnum, x, y); }

    ObjectClass *Hook_Next_Object(ObjectClass *object) { return DisplayClass::Next_Object(object); }

    BOOL Hook_In_View(cell_t cellnum) { return DisplayClass::In_View(cellnum); }

    BOOL Hook_Coord_To_Pixel(coord_t coord, int &x, int &y) { return DisplayClass::Coord_To_Pixel(coord, x, y); }

    int Hook_Cell_Shadow(cell_t cellnum) { return DisplayClass::Cell_Shadow(cellnum); }

    coord_t Hook_Closest_Free_Spot(coord_t coord, BOOL skip_occupied)
    {
        return DisplayClass::Closest_Free_Spot(coord, skip_occupied);
    }

    cell_t Hook_Calculated_Cell(
        SourceType source, int waypoint, cell_t cellnum, SpeedType speed, BOOL use_zone, MZoneType mzone)
    {
        return DisplayClass::Calculated_Cell(source, waypoint, cellnum, speed, use_zone, mzone);
    }

    BOOL Hook_Good_Reinforcement_Cell(cell_t cell1, cell_t cell2, SpeedType speed, int zone, MZoneType mzone)
    {
        return DisplayClass::Good_Reinforcement_Cell(cell1, cell2, speed, zone, mzone);
    }
#endif
};

#endif // DISPLAY_H
