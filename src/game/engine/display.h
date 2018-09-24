/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling tactical display (main action pane when in game).
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
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
#include "noinit.h"

class BufferClass;

enum ModeControl {
    MODE_CONTROL_TOGGLE = -1,
    MODE_CONTROL_OFF = 0,
    MODE_CONTROL_ON = 1
};

class DisplayClass : public MapClass
{
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

    virtual void Read_INI(CCINIClass &ini) override;
    void Write_INI(CCINIClass &ini); // Not virtual in RA.
    virtual BOOL Map_Cell(int16_t cellnum, HouseClass *house) override;
    virtual int16_t Click_Cell_Calc(int x, int y) const override;
    virtual void Help_Text(int str_id, int x = -1, int y = -1, int color = 14, BOOL no_wait = false) override {}
    virtual BOOL Scroll_Map(DirType dir, int &distance, BOOL redraw = true) override;
    virtual void Refresh_Cells(int16_t cellnum, int16_t *overlap_list) override;
    virtual void Set_View_Dimensions(int x, int y, int w = -1, int h = -1) override;
    virtual void Put_Place_Back(TechnoClass *obj) override {}
    virtual void Set_Tactical_Position(uint32_t location) override;
    virtual void Flag_Cell(int16_t cellnum) override;
    virtual void Mouse_Right_Press() override;
    virtual void Mouse_Left_Press(int mouse_x, int mouse_y) override;
    virtual void Mouse_Left_Up(int16_t cellnum, BOOL cell_shrouded = false, ObjectClass *object = nullptr,
        ActionType action = ACTION_NONE, BOOL mouse_in_radar = false) override;
    virtual void Mouse_Left_Held(int mouse_x, int mouse_y) override;
    virtual void Mouse_Left_Release(int16_t cellnum, int mouse_x, int mouse_y, ObjectClass *object = nullptr,
        ActionType action = ACTION_NONE, BOOL mouse_in_radar = false) override;

    uint32_t Pixel_To_Coord(int x, int y);
    void Set_Cursor_Shape(int16_t *list = nullptr);
    void Refresh_Band();
    void Cursor_Mark(int16_t cellnum, BOOL flag);
    void Get_Occupy_Dimensions(int &w, int &h, int16_t *list) const;
    const int16_t *Text_Overlap_List(const char *string, int x, int y) const;
    ObjectClass *Next_Object(ObjectClass *object) const;
    void Submit(ObjectClass *object, LayerType layer);
    void Remove(ObjectClass *object, LayerType layer);
    ObjectClass *Cell_Object(int16_t cellnum, int x, int y) const;
    void Select_These(uint32_t start, uint32_t finish);
    void Sell_Mode_Control(int mode);
    void Repair_Mode_Control(int mode);
    void Center_Map(uint32_t coord);
    int16_t Set_Cursor_Pos(int16_t cell);
    BOOL Passes_Proximity_Check(ObjectTypeClass *object, HousesType house, int16_t *list, int16_t cell) const;
    void Redraw_Icons();
    void Redraw_Shadow();
    BOOL In_View(int16_t cellnum) const;
    BOOL Coord_To_Pixel(uint32_t coord, int &x, int &y) const;
    uint32_t Pixel_To_Coord(int x, int y) const;
    int Cell_Shadow(int16_t cellnum) const;
    BOOL Push_Onto_TacMap(uint32_t &coord1, uint32_t &coord2);
    int16_t Calculated_Cell(SourceType source, int waypoint, int16_t cellnum, SpeedType speed, BOOL use_zone, MZoneType mzone) const;
    BOOL Good_Reinforcement_Cell(int16_t cell1, int16_t cell2, SpeedType speed, int zone, MZoneType mzone) const;
    uint32_t Closest_Free_Spot(uint32_t coord, BOOL skip_occupied) const;
    void All_To_Look(BOOL skip_buildings);
    void Constrained_Look(uint32_t coord, int constraint);
    void Encroach_Shadow();
    void Shroud_Cell(int16_t cellnum);

    static BOOL Is_Cell_Flagged(int16_t cellnum) { return CellRedraw[cellnum]; }

#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
    int16_t Hook_Click_Cell_Calc(int x, int y);
    const int16_t *Hook_Text_Overlap_List(char const *string, int x, int y);
#endif

private:
    // This only seems to be used by DisplayClass, so made it a static helper of this class.
    static int __cdecl Clip_Rect(int &x, int &y, int &w, int &h, int clip_w, int clip_h);
    static void Hook_Get_Occupy_Dimensions(DisplayClass* ptr, int &w, int &h, int16_t *list)
    {
        ptr->Get_Occupy_Dimensions(w, h, list);
    }

    static ObjectClass *Hook_Cell_Object(DisplayClass* ptr, int16_t cellnum, int x, int y)
    {
        return ptr->Cell_Object(cellnum, x, y);
    }
    
    static ObjectClass *Hook_Next_Object(DisplayClass* ptr, ObjectClass *object)
    {
        return ptr->Next_Object(object);
    }

    static BOOL Hook_In_View(DisplayClass* ptr, int16_t cellnum)
    {
        return ptr->In_View(cellnum);
    }
    
    static BOOL Hook_Coord_To_Pixel(DisplayClass* ptr, uint32_t coord, int &x, int &y)
    {
        return ptr->Coord_To_Pixel(coord, x, y);
    }

    static int Hook_Cell_Shadow(DisplayClass* ptr, int16_t cellnum)
    {
        return ptr->Cell_Shadow(cellnum);
    }

    static uint32_t Hook_Closest_Free_Spot(DisplayClass* ptr, uint32_t coord, BOOL skip_occupied)
    {
        return ptr->Closest_Free_Spot(coord, skip_occupied);
    }

    static int16_t Hook_Calculated_Cell(DisplayClass* ptr, SourceType source, int waypoint, int16_t cellnum, SpeedType speed, BOOL use_zone, MZoneType mzone)
    {
        return ptr->Calculated_Cell(source, waypoint, cellnum, speed, use_zone, mzone);
    }

    static BOOL Hook_Good_Reinforcement_Cell(DisplayClass* ptr, int16_t cell1, int16_t cell2, SpeedType speed, int zone, MZoneType mzone)
    {
        return ptr->Good_Reinforcement_Cell(cell1, cell2, speed, zone, mzone);
    }

protected:
    uint32_t DisplayPos; // Coord of top left of tactical display within the map.
    int16_t DisplayWidth; // Width of display in leptons.
    int16_t DisplayHeight; // Height of display in leptons.
    int16_t DisplayCursorStart;
    int16_t DisplayCursorEnd;
    int16_t *DisplayCursorOccupy;
    BOOL PassedProximityCheck;
    ObjectClass *PendingObjectPtr;
    ObjectTypeClass *PendingObjectTypePtr;
    HousesType PendingObjectHouse;
    int TacOffsetX;
    int TacOffsetY;
    int DisplayNewPos;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool DisplayToRedraw : 1; // 1
            bool DisplayRepairMode : 1; // 2
            bool DisplaySellMode : 1; // 4
            bool DisplayBit8 : 1;  // 8 This is Bit32 in SS. passes proximity check/can place?
            bool DisplayBit16 : 1; // 16
            bool RedrawShadow : 1; // 32 this is Bit128 in SS
        };
        int Bitfield;
    };
#else
    bool DisplayToRedraw;
    bool DisplayRepairMode;
    bool DisplaySellMode;
    bool DisplayBit8;  // This is Bit32 in SS. passes proximity check/can place? Could also be if mouse is pressed/dragging a band box?
    bool DisplayBit16;
    bool RedrawShadow; //this is Bit128 in SS
#endif
    SpecialWeaponType PendingSuper; // Current superweapon selected to fire.
    TRect<int> BandBox; // Dimensions of the selection band box.

public:
#ifndef CHRONOSHIFT_STANDALONE
    static LayerClass *Layers;
    static TacticalClass &TacticalButton;
    static char *FadingBrighten;
    static char *FadingShade;
    static char *FadingWayDark;
    static char *FadingLight;
    static char *FadingGreen;
    static char *FadingYellow;
    static char *FadingRed;
    static char *TranslucentTable;
    static char *WhiteTranslucentTable;
    static char *MouseTranslucentTable;
    static char *ShadowTrans;
    static char *UnitShadow;
    static char *UnitShadowAir;
    static char *SpecialGhost;
    static void *&TransIconset;
    static void *&ShadowShapes;
    static BufferClass *&TheaterBuffer;
    static BooleanVectorClass &CellRedraw;
#else
    static LayerClass Layers[LAYER_COUNT];
    static TacticalClass TacticalButton;
    static char FadingBrighten[256]; 
    static char FadingShade[256];
    static char FadingWayDark[256];
    static char FadingLight[256];
    static char FadingGreen[256];
    static char FadingYellow[256];
    static char FadingRed[256];
    static char TranslucentTable[14][256];
    static char WhiteTranslucentTable[2][256];
    static char MouseTranslucentTable[5][256];
    static char ShadowTrans[5][256];
    static char UnitShadow[2][256];
    static char UnitShadowAir[2][256];
    static char SpecialGhost[2][256];
    static void *TransIconset;
    static void *ShadowShapes;
    static BufferClass *TheaterBuffer;
    static BooleanVectorClass CellRedraw;
#endif
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
inline const int16_t *DisplayClass::Hook_Text_Overlap_List(char const *string, int x, int y)
{
    return DisplayClass::Text_Overlap_List(string, x, y);
}

inline int16_t DisplayClass::Hook_Click_Cell_Calc(int x, int y)
{
    return DisplayClass::Click_Cell_Calc(x, y);
}

inline void DisplayClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x004AEF7C, *DisplayClass::Init_Clear); 
    Hook_Function(0x004AEFF4, *DisplayClass::Init_IO); 
    Hook_Function(0x004AF02C, *DisplayClass::Init_Theater); 
    Hook_Function(0x004B0140, *DisplayClass::AI); 
    Hook_Function(0x004AEEF4, *DisplayClass::One_Time); 
    Hook_Function(0x004AF700, *DisplayClass::Set_Cursor_Shape); 
    Hook_Function(0x004B0278, *DisplayClass::Hook_Click_Cell_Calc); 
    Hook_Function(0x004B03B4, *DisplayClass::Scroll_Map); 
    Hook_Function(0x004B0628, *DisplayClass::Refresh_Cells); 
    Hook_Function(0x004AF4E0, *DisplayClass::Set_View_Dimensions); 
    Hook_Function(0x004B4860, *DisplayClass::Set_Tactical_Position); 
    Hook_Function(0x004B5908, *DisplayClass::Flag_Cell);  
    Hook_Function(0x004B35C4, *DisplayClass::Mouse_Right_Press); 
    Hook_Function(0x004B4608, *DisplayClass::Mouse_Left_Press); 
    Hook_Function(0x004B465C, *DisplayClass::Mouse_Left_Held);
    Hook_Function(0x004B2E84, *DisplayClass::Refresh_Band); 
    Hook_Function(0x004B006C, *DisplayClass::Cursor_Mark); 
    Hook_Function(0x004AFFA8, *DisplayClass::Hook_Get_Occupy_Dimensions); 
    Hook_Function(0x004AF2D8, *DisplayClass::Hook_Text_Overlap_List);
    Hook_Function(0x004B25A4, *DisplayClass::Hook_Next_Object); 
    Hook_Function(0x004B0214, *DisplayClass::Submit); 
    Hook_Function(0x004B0248, *DisplayClass::Remove); 
    Hook_Function(0x004B0C78, *DisplayClass::Hook_Cell_Object); 
    Hook_Function(0x004B4E20, *DisplayClass::Center_Map); 
    Hook_Function(0x004AFD40, *DisplayClass::Set_Cursor_Pos); 
    Hook_Function(0x004B1FD0, *DisplayClass::Redraw_Icons);
    Hook_Function(0x004B2178, *DisplayClass::Redraw_Shadow);
    Hook_Function(0x004B4CB8, *DisplayClass::Hook_In_View); 
    Hook_Function(0x004B0968, *DisplayClass::Hook_Coord_To_Pixel);
    Hook_Function(0x004B0698, *DisplayClass::Hook_Cell_Shadow);
    Hook_Function(0x004B0B10, *DisplayClass::Push_Onto_TacMap);
    Hook_Function(0x004B274C, *DisplayClass::Hook_Calculated_Cell); 
    Hook_Function(0x004B2B90, *DisplayClass::Hook_Good_Reinforcement_Cell); 
    Hook_Function(0x004B4D80, *DisplayClass::Hook_Closest_Free_Spot); 
    Hook_Function(0x004B4F44, *DisplayClass::Encroach_Shadow);
#endif
}
#endif

#endif // DISPLAY_H
