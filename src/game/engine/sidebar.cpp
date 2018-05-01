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
#include "sidebar.h"
#include "ccfileclass.h"
#include "iomap.h"
#include "lists.h"
#include "coord.h"
#include "mixfile.h"
#include "session.h"

SidebarClass::SBGadgetClass::SBGadgetClass() : GadgetClass(496, 154, 143, 244, MOUSE_LEFT_UP, false) {}

BOOL SidebarClass::SBGadgetClass::Action(unsigned flags, KeyNumType &key)
{
    // TODO, needs HelpClass, MouseClass, TechnoClass.
#ifndef RAPP_STANDALONE
    BOOL(*func)
    (const SBGadgetClass *, unsigned, KeyNumType &) =
        reinterpret_cast<BOOL (*)(const SBGadgetClass *, unsigned, KeyNumType &)>(0x0054F380);
    return func(this, flags, key);
#else
    return false;
#endif
}

SidebarClass::StripClass::SelectClass::SelectClass() :
    ControlClass(CONTROL_SB_SELECT_BUTTON, 0, 0, 62, 48, MOUSE_LEFT_PRESS | MOUSE_LEFT_UP | MOUSE_RIGHT_PRESS,
        false), // maybe CAMEO_WIDTH instead of 62? see above
    Owner(nullptr),
    Row(0)
{
}

BOOL SidebarClass::StripClass::SelectClass::Action(unsigned flags, KeyNumType &key)
{
    // TODO, needs HelpClass, MouseClass, TechnoClass, EventClass.
#ifndef RAPP_STANDALONE
    BOOL(*func)
    (const SelectClass *, unsigned, KeyNumType &) =
        reinterpret_cast<BOOL (*)(const SelectClass *, unsigned, KeyNumType &)>(0x0054ECD8);
    return func(this, flags, key);
#else
    return false;
#endif
}

void SidebarClass::StripClass::SelectClass::Set_Owner(StripClass &strip, int row)
{
    Owner = &strip;
    Row = row;

    // copy the x pos from the strip
    XPos = strip.XPos;

    // calculate my position from the row index and strip y_pos
    YPos = (48 * row) + strip.YPos;
}

SidebarClass::StripClass::StripClass() :
    ProgressTimer(),

    // TODO, one of thse is -1
    XPos(0),
    YPos(0),
    WhichColumn(),
    StripToRedraw(true),
    Strip_Boolean2(false),
    Strip_Boolean4(false),
    Strip_Boolean8(false),
    Strip_Boolean16(false),
    Strip_Boolean32(false),
    field_21(-1),
    CurrentRow(0),
    RowStartIndex(0),
    field_2D(0),
    field_31(0),
    CameoCount(0)
{
}

/*
Both can't init Entries
SidebarClass::StripClass::StripClass(StripClass const &that) :
    ProgressTimer(that.ProgressTimer),
    XPos(that.XPos),
    YPos(that.YPos),
    WhichColumn(that.WhichColumn),
    StripToRedraw(that.StripToRedraw),
    Strip_Boolean2(that.Strip_Boolean2),
    Strip_Boolean4(that.Strip_Boolean4),
    Strip_Boolean8(that.Strip_Boolean8),
    Strip_Boolean16(that.Strip_Boolean16),
    Strip_Boolean32(that.Strip_Boolean32),
    field_21(that.field_21),
    CurrentRow(that.CurrentRow),
    RowStartIndex(that.RowStartIndex),
    field_2D(that.field_2D),
    field_31(that.field_31),
    CameoCount(that.CameoCount),
    Entries(that.Entries)
{
    memcpy(Entries, that.Entries, sizeof(Entries));
}

SidebarClass::StripClass::StripClass(InitClass const &init) :

    //TODO, one of thse is -1
    XPos(0),
    YPos(0),
    WhichColumn(),
    StripToRedraw(true),
    Strip_Boolean2(false),
    Strip_Boolean4(false),
    Strip_Boolean8(false),
    Strip_Boolean16(false),
    Strip_Boolean32(false),
    field_21(-1),
    CurrentRow(0),
    RowStartIndex(0),
    field_2D(0),
    field_31(0),
    CameoCount(0),
    Entries()
{
    for ( int index = 0; index < ARRAY_SIZE(Entries); ++index ) {
        new(&Entries[index]) SelectButtonType;
    }

}
*/

SidebarClass::StripClass::~StripClass() {}

// SuperWeaponClass..................
void SidebarClass::StripClass::One_Time(int column)
{
    // TODO, needs HouseClass, TabClass, HelpClass.
#ifndef RAPP_STANDALONE
    void(*func) (const StripClass *, int) = reinterpret_cast<void (*)(const StripClass *, int)>(0x0054DD54);
    func(this, column);
#else
    DEBUG_ASSERT(this != nullptr);

    char icon_fname[16];
    // ClockShapes = MixFileClass<CCFileClass>::Retrieve("CLOCK.SHP");

    // for ( SpecialWeaponType super = SPECIAL_FIRST; super < SPECIAL_COUNT; ++super ) {
    //    snprintf(icon_fname, sizeof(icon_fname), "%.4sICON.SHP", SuperWeaponClass::Get_Special_Icon(super));
    //    SpecialShapes[super] = MixFileClass<CCFileClass>::Retrieve(icon_fname);
    //}
#endif
}

void *SidebarClass::StripClass::Get_Special_Cameo(SpecialWeaponType super)
{
#ifndef RAPP_STANDALONE
    void *(*func) (const StripClass *, SpecialWeaponType) = reinterpret_cast<void * (*)(const StripClass *, SpecialWeaponType)>(0x0054DDE0);
    return func(this, super);
#else
    DEBUG_ASSERT(this != nullptr);
    if (super != SPECIAL_NONE && super < SPECIAL_COUNT) {
        //return SpecialShapes[super];
    }
    return nullptr;
#endif
}

//how much is MaxCameoPerColumn?
void SidebarClass::StripClass::Init_Clear()
{
    DEBUG_ASSERT(this != nullptr);

    field_21 = -1;
    CurrentRow = 0;
    field_2D = 0;
    CameoCount = 0;
    Strip_Boolean2 = false;
    Strip_Boolean4 = false;
    Strip_Boolean8 = false;

    // Reset all the cameo entries
    //for (int index = 0; index < MaxCameoPerColumn; ++index) {
    //    Entries[index].ID = 0;
    //    Entries[index].Type = RTTI_NONE;
    //    Entries[index].Factory = -1;
    //}
}

void SidebarClass::StripClass::Init_IO(int column)
{
#ifndef RAPP_STANDALONE
    void(*func) (const StripClass *, int) = reinterpret_cast<void (*)(const StripClass *, int)>(0x0054DE54);
    func(this, column);
#endif
}

void SidebarClass::StripClass::Init_Theater(TheaterType theater)
{
#ifndef RAPP_STANDALONE
    void(*func) (const StripClass *, TheaterType) = reinterpret_cast<void (*)(const StripClass *, TheaterType)>(0x0054E008);
    func(this, theater);
#endif
}

void SidebarClass::StripClass::Reload_LogoShapes()
{
#ifndef RAPP_STANDALONE
    void(*func) (const StripClass *) = reinterpret_cast<void (*)(const StripClass *)>(0x0054E094);
    func(this);
#endif
}

void SidebarClass::StripClass::Activate()
{
#ifndef RAPP_STANDALONE
    void(*func) (const StripClass *) = reinterpret_cast<void (*)(const StripClass *)>(0x0054E0C4);
    func(this);
#endif
}

void SidebarClass::StripClass::Deactivate()
{
#ifndef RAPP_STANDALONE
    void(*func) (const StripClass *) = reinterpret_cast<void (*)(const StripClass *)>(0x0054E184);
    func(this);
#endif
}

bool SidebarClass::StripClass::Add(RTTIType type, int id)
{
#ifndef RAPP_STANDALONE
    bool(*func) (const StripClass *, RTTIType, int) = reinterpret_cast<bool (*)(const StripClass *, RTTIType, int)>(0x0054E1F8);
    return func(this, type, id);
#endif
}

bool SidebarClass::StripClass::Scroll(bool reverse)
{
#ifndef RAPP_STANDALONE
    bool(*func) (const StripClass *, bool) = reinterpret_cast<bool (*)(const StripClass *, bool)>(0x0054E290);
    return func(this, reverse);
#endif
}

void SidebarClass::StripClass::Flag_To_Redraw()
{
    DEBUG_ASSERT(this != nullptr);
    StripToRedraw = true;
    Map.Flag_To_Redraw();
}

bool SidebarClass::StripClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
#ifndef RAPP_STANDALONE
    bool (*func)(const StripClass *, KeyNumType &, int, int) =
        reinterpret_cast<bool (*)(const StripClass *, KeyNumType &, int, int)>(0x0054E2E8);
    return func(this, key, mouse_x, mouse_y);
#endif
}

void SidebarClass::StripClass::Draw_It(bool force_redraw)
{
#ifndef RAPP_STANDALONE
    void(*func) (const StripClass *, bool) = reinterpret_cast<void (*)(const StripClass *, bool)>(0x0054E6FC);
    func(this, force_redraw);
#endif
}

bool SidebarClass::StripClass::Recalc()
{
#ifndef RAPP_STANDALONE
    bool(*func) (const StripClass *) = reinterpret_cast<bool (*)(const StripClass *)>(0x0054EB1C);
    return func(this);
#endif
}

bool SidebarClass::StripClass::Factory_Link(int a1, RTTIType type, int a3)
{
#ifndef RAPP_STANDALONE
    bool(*func) (const StripClass *, int, RTTIType, int) = reinterpret_cast<bool (*)(const StripClass *, int, RTTIType, int)>(0x0054F3B4);
    return func(this, a1, type, a3);
#endif
}

int SidebarClass::StripClass::Abandon_Production(int a1)
{
#ifndef RAPP_STANDALONE
    int(*func) (const StripClass *, int) = reinterpret_cast<int (*)(const StripClass *, int)>(0x0054F434);
    return func(this, a1);
#endif
}

//eh what?
SidebarClass::SidebarClass(void) :
    PowerClass(),
    SidebarIsDrawn(false),
    SidebarToRedraw(true),
    SidebarBit4(false),
    SidebarBit8(false),
    SidebarBit16(false)
{
    InitClass init;

    //
    // 
    //
    //for ( int column = 0; column < ARRAY_SIZE(Columns); ++column ) {
    //    chrono_placement_new(&Columns[column]) StripClass(init);
    //}
}
/*
SidebarClass::SidebarClass(SidebarClass const &that) :
    PowerClass(that),
    SidebarIsDrawn(that.SidebarIsDrawn),
    SidebarToRedraw(that.SidebarToRedraw),
    SidebarBit4(that.SidebarBit4),
    SidebarBit8(that.SidebarBit8),
    SidebarBit16(that.SidebarBit16),
    Columns()
{
}

SidebarClass::SidebarClass(NoInitClass const &noinit) :
    PowerClass(noinit),
    Columns()
{
}
*/

void SidebarClass::One_Time()
{
#ifndef RAPP_STANDALONE
    void(*func) (const SidebarClass *) = reinterpret_cast<void (*)(const SidebarClass *)>(0x0054D07C);
    func(this);
#endif
}

void SidebarClass::Init_Clear()
{
#ifndef RAPP_STANDALONE
    void(*func) (const SidebarClass *) = reinterpret_cast<void (*)(const SidebarClass *)>(0x0054D0F8);
    func(this);
#endif
}

void SidebarClass::Init_IO()
{
#ifndef RAPP_STANDALONE
    void(*func) (const SidebarClass *) = reinterpret_cast<void (*)(const SidebarClass *)>(0x0054D144);
    func(this);
#endif
}

void SidebarClass::Init_Theater(TheaterType theater)
{
#ifndef RAPP_STANDALONE
    void(*func) (const SidebarClass *, TheaterType) = reinterpret_cast<void (*)(const SidebarClass *, TheaterType)>(0x0054D304);
    func(this, theater);
#endif
}

void SidebarClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
#ifndef RAPP_STANDALONE
    void (*func)(const SidebarClass *, KeyNumType &, int, int) =
        reinterpret_cast<void (*)(const SidebarClass *, KeyNumType &, int, int)>(0x0054D8F0);
    func(this, key, mouse_x, mouse_y);
#endif
}

void SidebarClass::Draw_It(BOOL force_redraw)
{
#ifndef RAPP_STANDALONE
    void (*func)(const SidebarClass *, BOOL) = reinterpret_cast<void (*)(const SidebarClass *, BOOL)>(0x0054D724);
    func(this, force_redraw);
#endif
}

void SidebarClass::Refresh_Cells(int16_t cellnum, int16_t *overlap_list)
{
    DEBUG_ASSERT(this != nullptr);
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);
    if ( overlap_list != nullptr ) {
        if ( overlap_list[0] == LIST_START ) {
            for ( ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column ) {
                Columns[column].StripToRedraw = true;
            }
            Flag_To_Redraw();
        }
    }
    PowerClass::Refresh_Cells(cellnum, overlap_list);
}

void SidebarClass::Reload_Sidebar(void)
{
#ifndef RAPP_STANDALONE
    void (*func)(const SidebarClass *) = reinterpret_cast<void (*)(const SidebarClass *)>(0x0054D340);
    func(this);
#endif
}

ColumnType SidebarClass::Which_Column(RTTIType type)
{
    DEBUG_ASSERT(this != nullptr);
    switch ( type ) {

        case RTTI_BUILDING:
        case RTTI_BUILDINGTYPE:
            return COLUMN_LEFT;

        case RTTI_AIRCRAFT:
        case RTTI_AIRCRAFTTYPE:
        case RTTI_INFANTRY:
        case RTTI_INFANTRYTYPE:
        case RTTI_UNIT:
        case RTTI_UNITTYPE:
        case RTTI_VESSEL:
        case RTTI_VESSELTYPE:
            return COLUMN_RIGHT;

        default:
            DEBUG_ASSERT_PRINT(0, "Unhandled RTTIType in Which_Column() returning COLUMN_NONE, We should never reach here!");
            return COLUMN_NONE;
    }
}


BOOL SidebarClass::Factory_Link(int a1, RTTIType type, int a3)
{
    DEBUG_ASSERT(this != nullptr);

    return Columns[Which_Column(type)].Factory_Link(a1, type, a3);
}


BOOL SidebarClass::Activate_Repair(int a1)
{
#ifndef RAPP_STANDALONE
    BOOL (*func)(const SidebarClass *, int) = reinterpret_cast<BOOL (*)(const SidebarClass *, int)>(0x0054D464);
    return func(this, a1);
#endif
}

BOOL SidebarClass::Activate_Upgrade(int a1)
{
    DEBUG_ASSERT(this != nullptr);

    bool v5 = SidebarToRedraw;
    
    if ( a1 == -1 ) {
        a1 = (SidebarBit8 == 0);
    }
    
    if ( a1 == 1 ) {
        SidebarBit8 = true;
    } else {
        SidebarBit8 = false;
    }
    
    if ( SidebarToRedraw != v5 ) {
        Flag_To_Redraw();
        SidebarToRedraw = true;
        
        if ( !SidebarBit8 ) {
            Set_Default_Mouse(MOUSE_POINTER);
        }
    }
    
    return v5;
}


BOOL SidebarClass::Activate_Demolish(int a1)
{
    DEBUG_ASSERT(this != nullptr);

    bool v5 = SidebarToRedraw;
    
    if ( a1 == -1 ) {
        a1 = (SidebarBit16 == 0);
    }
    
    if ( a1 == 1 ) {
        SidebarBit16 = true;
    } else {
        SidebarBit16 = false;
    }
    
    if ( SidebarToRedraw != v5 ) {
        Flag_To_Redraw();
        SidebarToRedraw = true;
        
        if ( !SidebarBit16 ) {
            Set_Default_Mouse(MOUSE_POINTER);
        }
    }
    
    return v5;
}

BOOL SidebarClass::Add(RTTIType item, int id)
{
    DEBUG_ASSERT(this != nullptr);

    //if ( InMapEditor ) {
    //    return false;
    //}

    //get Left or Right strip and add the "item", based on return value of Which_Column().
    if ( Columns[Which_Column(item)].Add(item, id) ) {

        Activate(1);

        SidebarToRedraw = true;

        Flag_To_Redraw();
    
        return true;
    }

    return false;

}

BOOL SidebarClass::Scroll(BOOL reverse, ColumnType column)
{
#ifndef RAPP_STANDALONE
    BOOL (*func)(const SidebarClass *, BOOL, ColumnType) = reinterpret_cast<BOOL (*)(const SidebarClass *, BOOL, ColumnType)>(0x0054D684);
    return func(this, reverse, column);
#endif
}

void SidebarClass::Recalc(void)
{
    DEBUG_ASSERT(this != nullptr);

    bool v1 = false;

    for ( ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column ) {
        v1 = Columns[column].Recalc();
        if ( v1 ) {
            break;
        }
    }

    if ( v1 ) {
        SidebarToRedraw = true;
        Flag_To_Redraw();
    }

}

BOOL SidebarClass::Activate(int mode)
{
#ifndef RAPP_STANDALONE
    bool (*func)(const SidebarClass *, int) = reinterpret_cast<bool (*)(const SidebarClass *, int)>(0x0054DA70);
    return func(this, mode);
#endif
}

int SidebarClass::Abandon_Production(RTTIType type, int a2)
{
    DEBUG_ASSERT(this != nullptr);

    return Columns[Which_Column(type)].Abandon_Production(a2);
}

void SidebarClass::Zoom_Mode_Control(ModeControl mode)
{

    //TODO, check all bools in this function, OmniBlade?
    //see pesudo in RA, not sure if i have the correct ones.

    if ( !RadarActive ) {

        if ( Session.Game_To_Play() != GAME_CAMPAIGN ) {
            Player_Names((RadarZoomed == false));
        }


    } else if ( RadarDrawNames || Session.Game_To_Play() == GAME_CAMPAIGN ) {

        if ( RadarDrawNames || !Spy_Next_House() ) {
            Zoom_Mode(Coord_To_Cell(DisplayPos));
        }

    } else if ( !RadarPulseActive && !RadarZoomed ) {

        Player_Names();

    } else {

        Player_Names(false);

        if ( !Spy_Next_House() ) {
            Zoom_Mode(Coord_To_Cell(DisplayPos));
        }

    }
}
