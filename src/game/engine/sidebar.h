/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling sidebar.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef SIDEBAR_H
#define SIDEBAR_H

#include "always.h"
#include "controlc.h"
#include "noinit.h"
#include "power.h"
#include "shapebtn.h"
#include "textbtn.h"
#include "stage.h"

enum ColumnType
{
    COLUMN_NONE = -1,
    COLUMN_FIRST = 0,
    COLUMN_LEFT = 0,
    COLUMN_RIGHT,
    COLUMN_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(ColumnType);

enum SidebarStateType
{
    SIDEBAR_TOGGLE = -1,
    SIDEBAR_ACTIVATE = 0,
    SIDEBAR_DEACTIVATE = 1
};

#define MAX_BUTTONS_PER_COLUMN 75
//#define ROW_COUNT 4

class SidebarClass : public PowerClass
{
    // Internal gadget class for handling input to the radar area of the game screen.
    class SBGadgetClass : public GadgetClass
    {
    public:
        SBGadgetClass();
        SBGadgetClass(SBGadgetClass &that) : GadgetClass(that) {}
        virtual ~SBGadgetClass() {}

        virtual BOOL Action(unsigned flags, KeyNumType &key) override;
    };

    class StripClass
    {
        friend class SidebarClass;

        enum
        {
            ROW_COUNT = 4,
        };

    public:
        class SelectClass : public ControlClass
        {
        public:
            SelectClass();

            virtual ~SelectClass() {}
            virtual BOOL Action(unsigned flags, KeyNumType &key) override;

            void Set_Owner(StripClass &strip, int row);

        protected:
            StripClass *Owner;
            int Row;
        };

    public:
        StripClass();
        StripClass(InitClass const &init);
        ~StripClass() {}

        void One_Time(int column);
        void *Get_Special_Cameo(SpecialWeaponType super);
        void Init_Clear();
        void Init_IO(int column);
        void Init_Theater(TheaterType theater);
        void Reload_LogoShapes();
        void Activate();
        void Deactivate();
        BOOL Add(RTTIType type, int id);
        BOOL Scroll(BOOL reverse);
        void Flag_To_Redraw();
        BOOL AI(KeyNumType &key, int mouse_x, int mouse_y);
        void Draw_It(BOOL force_redraw = false);
        BOOL Recalc();
        BOOL Factory_Link(int factory_id, RTTIType type, int id);
        int Abandon_Production(int unk1);

    private:
#ifndef RAPP_STANDALONE
        static void *&LogoShapes;
        static void *&ClockShapes;
        static void **SpecialShapes;
        // this is an array of 2x4.
        // (1) left strip, cameo 1,2,3,4
        // (2) right strip, cameo 1,2,3,4
        // based on column count and row count, this will grow dynamicly.
        static SelectClass *SelectButton;

        // these are the strip scrolling buttons, 2 pairs of up and down.
        // based on the column count, buttons will be allocated dynamicly.
        static ShapeButtonClass *UpButton;
        static ShapeButtonClass *DownButton;
        static char *ClockTranslucentTable;
#else
        static void *LogoShapes;
        static void *ClockShapes;
        static void *SpecialShapes[SPECIAL_COUNT];
        // this is an array of 2x4.
        // (1) left strip, cameo 1,2,3,4
        // (2) right strip, cameo 1,2,3,4
        // based on column count and row count, this will grow dynamicly.
        static SelectClass SelectButton[COLUMN_COUNT][ROW_COUNT];

        // these are the strip scrolling buttons, 2 pairs of up and down.
        // based on the column count, buttons will be allocated dynamicly.
        static ShapeButtonClass UpButton[COLUMN_COUNT];
        static ShapeButtonClass DownButton[COLUMN_COUNT];
        static char ClockTranslucentTable[256][2];
#endif
    private:
        struct SelectButtonType
        {
            SelectButtonType() : ID(0), Type(RTTI_NONE), Factory(-1) {}
            ~SelectButtonType() {}
            int ID;
            RTTIType Type;
            int Factory; // ID for a factory instance that is constructing this.
        };

    protected:
        StageClass ProgressTimer; // 0x0
        int XPos; // 0x11 -658
        int YPos; // 0x15 -654
        int WhichColumn; // 0x19 -650
#ifndef RAPP_NO_BITFIELDS
        // Union/Struct required to get correct packing when compiler packing set to 1.
        union
        {
            struct
            {
                // bitfield 0x1D
                bool StripToRedraw : 1; // 1
                bool Strip_Boolean2 : 1; // 2
                bool Strip_Boolean4 : 1; // 4	//related to some direction?
                bool Strip_Boolean8 : 1; // 8	//Flags if the icons are scrolling?
                bool Strip_Boolean16 : 1; // 16
                bool Strip_Boolean32 : 1; // 32
            };
            int Bitfield;
        };
#else
        // bitfield 0x1D
        bool StripToRedraw; // 1
        bool Strip_Boolean2; // 2
        bool Strip_Boolean4; // 4	//related to some direction?
        bool Strip_Boolean8; // 8	//Flags if the icons are scrolling?
        bool Strip_Boolean16; // 16
        bool Strip_Boolean32; // 32
#endif

        int field_21; // 0x21		// Icon with build clock active?
        int CurrentRow; // 0x25
        int RowStartIndex; // 0x29		// scroll position, which row is topmost visible
        int field_2D; // 0x2D
        int field_31; // 0x31
        int CameoCount; // 0x35
        SelectButtonType Entries[MAX_BUTTONS_PER_COLUMN]; // 0x39	75 camoes per column
    };

public:
    SidebarClass();
    SidebarClass(NoInitClass &noinit) : PowerClass(noinit) {}

    virtual void One_Time() override;
    virtual void Init_Clear() override;
    virtual void Init_IO() override;
    virtual void Init_Theater(TheaterType theater) override;
    virtual void AI(KeyNumType &key, int mouse_x, int mouse_y) override;
    virtual void Draw_It(BOOL force_redraw) override;
    virtual void Refresh_Cells(int16_t cellnum, int16_t *overlap_list) override;

    void Reload_Sidebar();
    int Which_Column(RTTIType type);
    BOOL Factory_Link(int factory_id, RTTIType type, int id);
    BOOL Activate_Repair(int state);
    BOOL Activate_Upgrade(int state);
    BOOL Activate_Demolish(int state);
    BOOL Add(RTTIType type, int id);
    BOOL Scroll(BOOL reverse, int column);
    void Recalc();
    BOOL Activate(int mode = SIDEBAR_TOGGLE);
    int Abandon_Production(RTTIType, int);
    void Zoom_Mode_Control(ModeControl mode);

    BOOL Is_Sidebar_Drawn() const { return SidebarIsDrawn; }
    void Disable_Zoom_Button() { ZoomButton.Disable(); }
    void Enable_Zoom_Button() { ZoomButton.Enable(); }

#ifndef RAPP_STANDALONE
    static void Hook_Me();
#endif

protected:
    StripClass Columns[COLUMN_COUNT];
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool SidebarIsDrawn : 1; // 1 when set to false, the sidebar is not drawn (for sidebar TAB hidding in C&C).
            bool SidebarToRedraw : 1; // 2 buttons to redraw?
            bool SidebarBit4 : 1; // 4 repair active
            bool SidebarBit8 : 1; // 8 upgrade active
            bool SidebarBit16 : 1; // 16 demolish active
        };
        int Bitfield;
    };
#else
    // bitfield 0x15F6
    bool SidebarIsDrawn; // when set to false, the sidebar is not drawn.(related to the sidebar TAB hidding in C&C)
    bool SidebarToRedraw; // buttons to redraw?
    bool SidebarBit4; // repair active
    bool SidebarBit8; // upgrade active
    bool SidebarBit16; // demolish active
#endif

#ifndef RAPP_STANDALONE
    static SBGadgetClass &Background;
    static ShapeButtonClass &RepairButton;
    static ShapeButtonClass &SellButton;
    static ShapeButtonClass &ZoomButton;

    static void *&SidebarShape;
    static void *&SidebarMiddleShape;
    static void *&SidebarBottomShape;
#else
    static SBGadgetClass Background;
    static ShapeButtonClass RepairButton;
    static ShapeButtonClass SellButton;
    static ShapeButtonClass ZoomButton;

    static void *SidebarShape;
    static void *SidebarMiddleShape;
    static void *SidebarBottomShape;
#endif
    static void *SidebarAddonShape; // RAPP addition
    static TextButtonClass Shore;
    static TextButtonClass River;
    static TextButtonClass Road;
    static TextButtonClass Ridges;
    static TextButtonClass Trees;
    static TextButtonClass Debris;
    static ShapeButtonClass Terrain;
    static ShapeButtonClass Ore;
    static ShapeButtonClass Gems;
    static ShapeButtonClass Water;
    static ShapeButtonClass Left;
    static ShapeButtonClass Right;
    static ShapeButtonClass Maximize;
    static ShapeButtonClass StartPosition;
    static ControlClass SelectedObject;
};

#ifndef RAPP_STANALONE
#include "hooker.h"
inline void SidebarClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x0054ECA8, *SidebarClass::StripClass::SelectClass::Set_Owner); // seems to work
    Hook_Function(0x0054DDE0, *SidebarClass::StripClass::Get_Special_Cameo); // seems to work
    Hook_Function(0x0054DE00, *SidebarClass::StripClass::Init_Clear); // seems to work
    Hook_Function(0x0054DE54, *SidebarClass::StripClass::Init_IO); //works
    Hook_Function(0x0054E008, *SidebarClass::StripClass::Init_Theater); //works
    Hook_Function(0x0054E0C4, *SidebarClass::StripClass::Activate); // seems to work
    Hook_Function(0x0054E184, *SidebarClass::StripClass::Deactivate); // seems to work
    Hook_Function(0x0054E1F8, *SidebarClass::StripClass::Add); // seems to work
    Hook_Function(0x0054E290, *SidebarClass::StripClass::Scroll); // seems to work
    Hook_Function(0x0054E2CC, *SidebarClass::StripClass::Flag_To_Redraw); // seems to work
    Hook_Function(0x0054F3B4, *SidebarClass::StripClass::Factory_Link); //works
    Hook_Function(0x0054D07C,  *SidebarClass::One_Time); // seems to work
    Hook_Function(0x0054D0F8, *SidebarClass::Init_Clear); //works 
    Hook_Function(0x0054D144, *SidebarClass::Init_IO); //works 
    Hook_Function(0x0054D304, *SidebarClass::Init_Theater); //works
    Hook_Function(0x0054D724, *SidebarClass::Draw_It); //Powerbar needed shows even when power structure not in play.
    Hook_Function(0x0054D404, *SidebarClass::Refresh_Cells); //works
    Hook_Function(0x0054D3B0, *SidebarClass::Which_Column); // works
    Hook_Function(0x0054D61C, *SidebarClass::Add); // seems to work
    Hook_Function(0x0054D684, *SidebarClass::Scroll); //breaks scrolling both strips at same time
    Hook_Function(0x0054DA2C, *SidebarClass::Recalc); // seems to work
    Hook_Function(0x0054DA70, *SidebarClass::Activate); //works
    Hook_Function(0x0054F408, *SidebarClass::Abandon_Production); // seems to work
    Hook_Function(0x0054F4B8, *SidebarClass::Zoom_Mode_Control); //works
#endif
}
#endif

#endif // SIDEBAR_H
