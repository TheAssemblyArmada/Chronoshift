/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling sidebar.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
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
#include "power.h"
#include "shapebtn.h"
#include "stage.h"

enum ColumnType
{
    COLUMN_LEFT,
    COLUMN_RIGHT,

    COLUMN_COUNT,

    COLUMN_NONE = -1,
    COLUMN_FIRST = COLUMN_LEFT
};

DEFINE_ENUMERATION_OPERATORS(ColumnType);

enum SidebarStateType
{
    SIDEBAR_TOGGLE = -1,
    SIDEBAR_DEACTIVATE = 0,
    SIDEBAR_ACTIVATE = 1
};

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

    public:
        enum
        {
            MAX_BUTTONS_PER_COLUMN = 75,
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
            StripClass *m_Owner;
            int m_Row;
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

        void Set_Position(int x, int y)
        {
            m_XPos = x;
            m_YPos = y;
        }

    private:
#ifdef GAME_DLL
        static void *&s_LogoShapes;
        static void *&s_ClockShapes;
        static ARRAY_DEC(void *, s_SpecialShapes, SPECIAL_COUNT);
        // this is an array of 2x4.
        // (1) left strip, cameo 1,2,3,4
        // (2) right strip, cameo 1,2,3,4
        // based on column count and row count, this will grow dynamicly.
        static ARRAY2D_DEC(SelectClass, s_SelectButton, COLUMN_COUNT, ROW_COUNT);

        // these are the strip scrolling buttons, 2 pairs of up and down.
        // based on the column count, buttons will be allocated dynamicly.
        static ARRAY_DEC(ShapeButtonClass, s_UpButton, COLUMN_COUNT);
        static ARRAY_DEC(ShapeButtonClass, s_DownButton, COLUMN_COUNT);
        static ARRAY2D_DEC(char, s_ClockTranslucentTable, 2, 256);
#else
        static void *s_LogoShapes;
        static void *s_ClockShapes;
        static void *s_SpecialShapes[SPECIAL_COUNT];
        // this is an array of 2x4.
        // (1) left strip, cameo 1,2,3,4
        // (2) right strip, cameo 1,2,3,4
        // based on column count and row count, this will grow dynamicly.
        static SelectClass s_SelectButton[COLUMN_COUNT][ROW_COUNT];

        // these are the strip scrolling buttons, 2 pairs of up and down.
        // based on the column count, buttons will be allocated dynamicly.
        static ShapeButtonClass s_UpButton[COLUMN_COUNT];
        static ShapeButtonClass s_DownButton[COLUMN_COUNT];
        static char s_ClockTranslucentTable[256][2];
#endif
    private:
        struct SelectButtonType
        {
            int ID;
            RTTIType Type;
            int Factory; // ID for a factory instance that is constructing this item.
        };

    protected:
        StageClass m_ProgressTimer; // 0x0
        int m_XPos; // 0x11 -658
        int m_YPos; // 0x15 -654
        int m_WhichColumn; // 0x19 -650
#ifndef CHRONOSHIFT_NO_BITFIELDS
        BOOL m_StripToRedraw : 1; // 1
        BOOL m_Strip_Boolean2 : 1; // 2
        BOOL m_Strip_Boolean4 : 1; // 4    // related to some direction?
        BOOL m_Strip_Boolean8 : 1; // 8    // Flags if the icons are scrolling?
        BOOL m_Strip_Boolean16 : 1; // 16
        BOOL m_Strip_Boolean32 : 1; // 32
#else
        // bitfield 0x1D
        bool m_StripToRedraw; // 1
        bool m_Strip_Boolean2; // 2
        bool m_Strip_Boolean4; // 4    // related to some direction?
        bool m_Strip_Boolean8; // 8    // Flags if the icons are scrolling?
        bool m_Strip_Boolean16; // 16
        bool m_Strip_Boolean32; // 32
#endif

        int m_field_21; // Icon with build clock active?
        int m_CurrentRow;
        int m_RowStartIndex; //  scroll position, which row is topmost visible
        int m_field_2D;
        int m_field_31;
        int m_CameoCount;
        SelectButtonType m_Entries[MAX_BUTTONS_PER_COLUMN];
    };

public:
    SidebarClass();
    SidebarClass(const NoInitClass &noinit) : PowerClass(noinit) {}

    virtual void One_Time() override;
    virtual void Init_Clear() override;
    virtual void Init_IO() override;
    virtual void Init_Theater(TheaterType theater) override;
    virtual void AI(KeyNumType &key, int mouse_x, int mouse_y) override;
    virtual void Draw_It(BOOL force_redraw) override;
    virtual void Refresh_Cells(cell_t cellnum, const int16_t *list) override;

    void Reload_Sidebar();
    int Which_Column(RTTIType type);
    BOOL Factory_Link(int factory_id, RTTIType type, int id);
    BOOL Activate_Repair(int state);
    BOOL Activate_Upgrade(int state);
    BOOL Activate_Demolish(int state);
    BOOL Add(RTTIType type, int id);
    BOOL Scroll(BOOL reverse, int column = COLUMN_NONE);
    void Recalc();
    BOOL Activate(int mode = SIDEBAR_TOGGLE);
    int Abandon_Production(RTTIType, int);
    void Zoom_Mode_Control();

    BOOL Is_Sidebar_Drawn() const { return m_SidebarIsDrawn; }
    void Disable_Zoom_Button() { s_ZoomButton.Disable(); }
    void Enable_Zoom_Button() { s_ZoomButton.Enable(); }
    void Flag_Strip_Redraw(ColumnType strip) { m_Columns[strip].Flag_To_Redraw(); }

protected:
    StripClass m_Columns[COLUMN_COUNT];
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_SidebarIsDrawn : 1; // 1
    BOOL m_SidebarToRedraw : 1; // 2
    BOOL m_SidebarBit4 : 1; // 4
    BOOL m_SidebarBit8 : 1; // 8
    BOOL m_SidebarBit16 : 1; // 16
#else
    // bitfield 0x15F6
    bool m_SidebarIsDrawn; // when set to false, the sidebar is not drawn.(related to the sidebar TAB hidding in C&C)
    bool m_SidebarToRedraw; // buttons to redraw?
    bool m_SidebarBit4; // repair active
    bool m_SidebarBit8; // upgrade active
    bool m_SidebarBit16; // demolish active
#endif

#ifdef GAME_DLL
    static SBGadgetClass &s_Background;
    static ShapeButtonClass &s_RepairButton;
    static ShapeButtonClass &s_SellButton;
    static ShapeButtonClass &s_ZoomButton;

    static void *&s_SidebarShape;
    static void *&s_SidebarMiddleShape;
    static void *&s_SidebarBottomShape;
#else
    static SBGadgetClass s_Background;
    static ShapeButtonClass s_RepairButton;
    static ShapeButtonClass s_SellButton;
    static ShapeButtonClass s_ZoomButton;

    static void *s_SidebarShape;
    static void *s_SidebarMiddleShape;
    static void *s_SidebarBottomShape;
#endif
    static void *s_SidebarAddonShape; // NOTE: Chronoshift addition.
};

#endif // SIDEBAR_H
