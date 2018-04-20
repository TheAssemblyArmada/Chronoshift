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
#include "trect.h"

class BufferClass;

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
    virtual void Help_Text(int str_id, int x = -1, int y = -1, BOOL on_wait = false) override {}
    virtual BOOL Scroll_Map(DirType dir, int &distance, BOOL redraw = true) override;
    virtual void Refresh_Cells(int16_t cellnum, int16_t *overlap_list) override;
    virtual void Set_View_Dimensions(int x, int y, int w = -1, int h = -1) override;
    virtual void Put_Place_Back(TechnoClass *obj) override {}
    virtual void Set_Tactical_Position(uint32_t location) override;
    virtual void Flag_Cell(int16_t cellnum) override;
    virtual void Mouse_Right_Press(int mouse_x, int mouse_y) override;
    virtual void Mouse_Left_Press(int mouse_x, int mouse_y) override;
    virtual void Mouse_Left_Up(int16_t cellnum, BOOL cell_shrouded = false, ObjectClass *object = nullptr,
        ActionType action = ACTION_NONE, BOOL mouse_in_radar = false) override;
    virtual void Mouse_Left_Held(int mouse_x, int mouse_y) override;
    virtual void Mouse_Left_Release(int16_t cellnum, int mouse_x, int mouse_y, ObjectClass *object = nullptr,
        ActionType action = ACTION_NONE, BOOL mouse_in_radar = false) override;

    void One_Time();
    uint32_t Pixel_To_Coord(int x, int y);
    void Set_Cursor_Shape(int16_t *list = nullptr);
    void Refresh_Band();
    void Cursor_Mark(int16_t cellnum, BOOL flag);
    void Get_Occupy_Dimensions(int &w, int &h, int16_t *list) const;
    const int16_t *Text_Overlap_List(const char *string, int x, int y) const;

#ifndef RAPP_STANDALONE
    static void Hook_Me();
    int16_t Hook_Click_Cell_Calc(int x, int y);
    const int16_t *Hook_Text_Overlap_List(char const *string, int x, int y);
#endif

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
#ifndef RAPP_NO_BITFIELDS
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

#ifndef RAPP_STANDALONE
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

#ifndef RAPP_STANALONE
#include "hooker.h"
inline const int16_t *DisplayClass::Hook_Text_Overlap_List(char const *string, int x, int y)
{
    return DisplayClass::Text_Overlap_List(string, x, y);
}

inline void DisplayClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x004AEF7C, *DisplayClass::Init_Clear); //seems to work
    Hook_Function(0x004AEFF4, *DisplayClass::Init_IO); //seems to work
    Hook_Function(0x004AF02C, *DisplayClass::Init_Theater); // invokes emergency exit
    Hook_Function(0x004B0140, *DisplayClass::AI); //seems to work
    Hook_Function(0x004B0278, *DisplayClass::Hook_Click_Cell_Calc); //crashes
    Hook_Function(0x004B03B4, *DisplayClass::Scroll_Map); //doesn't let me scroll
    Hook_Function(0x004B0628, *DisplayClass::Refresh_Cells); //seems to work
    Hook_Function(0x004AF4E0, *DisplayClass::Set_View_Dimensions); //crashes on TacticalButton issue with init order?
    Hook_Function(0x004B4860, *DisplayClass::Set_Tactical_Position); //seems to work
    Hook_Function(0x004B5908, *DisplayClass::Flag_Cell);  //seems to work
    Hook_Function(0x004B35C4, *DisplayClass::Mouse_Right_Press); //seems to work
    Hook_Function(0x004B4608, *DisplayClass::Mouse_Left_Press); //seems to work
    Hook_Function(0x004B465C, *DisplayClass::Mouse_Left_Held); //crashes on Refresh_Band call
    Hook_Function(0x004B2694, *DisplayClass::Pixel_To_Coord); //seems to work
    Hook_Function(0x004B2E84, *DisplayClass::Refresh_Band); //crashes
    Hook_Function(0x004AF2D8, *DisplayClass::Hook_Text_Overlap_List);
    //Hook_Function(0x005CC697, Confine_Rect); //seems to work
    //Hook_Function(0x005CC890, Change_Window); //seems to work
#endif
}
#endif

#endif // DISPLAY_H
