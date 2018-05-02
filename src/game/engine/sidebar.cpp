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

void *&SidebarClass::SidebarShape = Make_Global<void *>(0x00689F0C);
void **SidebarClass::StripClass::SpecialShapes = Make_Pointer<void *>(0x0068A46C);
void *SidebarClass::SidebarAddonShape = NULL;

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
    CameoCount(that.CameoCount)
{
    for (int index = 0; index < ARRAY_SIZE(Entries); ++index) {
        new (&Entries[index]) SelectButtonType;
    }
}
*/

SidebarClass::StripClass::StripClass(InitClass const &init) :

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

SidebarClass::StripClass::~StripClass() {}

// SuperWeaponClass..................
void SidebarClass::StripClass::One_Time(int column)
{
    // TODO, needs HouseClass, TabClass, HelpClass.
#ifndef RAPP_STANDALONE
    void (*func)(const StripClass *, int) = reinterpret_cast<void (*)(const StripClass *, int)>(0x0054DD54);
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
    DEBUG_ASSERT(this != nullptr);
    if (super != SPECIAL_NONE && super < SPECIAL_COUNT) {
        return SpecialShapes[super];
    }
    return nullptr;
}

void SidebarClass::StripClass::Init_Clear()
{
    field_21 = -1;
    CurrentRow = 0;
    field_2D = 0;
    CameoCount = 0;
    Strip_Boolean2 = false;
    Strip_Boolean4 = false;
    Strip_Boolean8 = false;

    // Reset all the cameo entries
    for (int i = 0; i < MAX_BUTTONS_PER_COLUMN; ++i) {
        Entries[i].ID = 0;
        Entries[i].Type = RTTI_NONE;
        Entries[i].Factory = 0xFFFFFFFF;
    }
}

void SidebarClass::StripClass::Init_IO(int column)
{
#ifndef RAPP_STANDALONE
    void (*func)(const StripClass *, int) = reinterpret_cast<void (*)(const StripClass *, int)>(0x0054DE54);
    func(this, column);
#endif
}

void SidebarClass::StripClass::Init_Theater(TheaterType theater)
{
#ifndef RAPP_STANDALONE
    void (*func)(const StripClass *, TheaterType) = reinterpret_cast<void (*)(const StripClass *, TheaterType)>(0x0054E008);
    func(this, theater);
#endif
    // Error! E374: col(16) object (or object pointer) required to access non-static data member
    // Reload_Sidebar();
    // DisplayClass::Init_Theater(theater);
    // Columns[COLUMN_LEFT]Init_Theater(theater);
    // Columns[COLUMN_RIGHT].Init_Theater(theater);
}

void SidebarClass::StripClass::Reload_LogoShapes()
{
    // TODO Needs HouseClass
#ifndef RAPP_STANDALONE
    void (*func)(const StripClass *) = reinterpret_cast<void (*)(const StripClass *)>(0x0054E094);
    func(this);
#endif
}

void SidebarClass::StripClass::Activate()
{
#ifndef RAPP_STANDALONE
    void (*func)(const StripClass *) = reinterpret_cast<void (*)(const StripClass *)>(0x0054E0C4);
    func(this);
#endif
}

void SidebarClass::StripClass::Deactivate()
{
#ifndef RAPP_STANDALONE
    void (*func)(const StripClass *) = reinterpret_cast<void (*)(const StripClass *)>(0x0054E184);
    func(this);
#endif
}

BOOL SidebarClass::StripClass::Add(RTTIType type, int id)
{
#ifndef RAPP_STANDALONE
    BOOL(*func)
    (const StripClass *, RTTIType, int) = reinterpret_cast<BOOL (*)(const StripClass *, RTTIType, int)>(0x0054E1F8);
    return func(this, type, id);
#endif
}

BOOL SidebarClass::StripClass::Scroll(BOOL reverse)
{
    if (reverse) {
        // We can't go below 0 for the current row, so check and if we are
        // trying to, just return false.
        if (CurrentRow == 0) {
            return false;
        }
        --RowStartIndex;
    } else {
        // We can't go beyond the count of cameos we have, so again return false
        // if we try.
        if (CurrentRow + ROW_COUNT >= CameoCount) {
            return false;
        }
        ++RowStartIndex;
    }
    return true;
}

void SidebarClass::StripClass::Flag_To_Redraw()
{
    DEBUG_ASSERT(this != nullptr);
    StripToRedraw = true;
    Map.Flag_To_Redraw();
}

BOOL SidebarClass::StripClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
    // TODO needs EventClass and FactoryClass
#ifndef RAPP_STANDALONE
    BOOL(*func)
    (const StripClass *, KeyNumType &, int, int) =
        reinterpret_cast<BOOL (*)(const StripClass *, KeyNumType &, int, int)>(0x0054E2E8);
    return func(this, key, mouse_x, mouse_y);
#endif
}

void SidebarClass::StripClass::Draw_It(BOOL force_redraw)
{
    // TODO Needs HouseClass and TechnoClass
#ifndef RAPP_STANDALONE
    void (*func)(const StripClass *, BOOL) = reinterpret_cast<void (*)(const StripClass *, BOOL)>(0x0054E6FC);
    func(this, force_redraw);
#endif
}

BOOL SidebarClass::StripClass::Recalc()
{
    // TODO Needs HouseClass, HouseTypeClass, and TechnoClass
#ifndef RAPP_STANDALONE
    BOOL (*func)(const StripClass *) = reinterpret_cast<BOOL (*)(const StripClass *)>(0x0054EB1C);
    return func(this);
#endif
}

BOOL SidebarClass::StripClass::Factory_Link(int unk1, RTTIType type, int unk3)
{
#ifndef RAPP_STANDALONE
    BOOL(*func)
    (const StripClass *, int, RTTIType, int) =
        reinterpret_cast<BOOL (*)(const StripClass *, int, RTTIType, int)>(0x0054F3B4);
    return func(this, unk1, type, unk3);
#endif
}

int SidebarClass::StripClass::Abandon_Production(int unk1)
{
    // TODO Needs FactoryClass
#ifndef RAPP_STANDALONE
    int (*func)(const StripClass *, int) = reinterpret_cast<int (*)(const StripClass *, int)>(0x0054F434);
    return func(this, unk1);
#endif
}

SidebarClass::SidebarClass() :
    PowerClass(),
    SidebarIsDrawn(false),
    SidebarToRedraw(true),
    SidebarBit4(false),
    SidebarBit8(false),
    SidebarBit16(false)
{
    InitClass init;
    for (int column = 0; column < ARRAY_SIZE(Columns); ++column) {
        new (&Columns[column]) StripClass(init);
    }
}

void SidebarClass::One_Time()
{
    PowerClass::One_Time();

    WindowList[WINDOW_SIDEBAR].X = 496;
    WindowList[WINDOW_SIDEBAR].Y = 180;
    WindowList[WINDOW_SIDEBAR].W = 160;
    WindowList[WINDOW_SIDEBAR].H = 192;

    for (int column = 0; column < ARRAY_SIZE(Columns); ++column) {
        Columns[column].One_Time(column);
    }

    if (SidebarShape != NULL) {
        delete SidebarShape;
    }

    SidebarShape = MixFileClass<CCFileClass>::Retrieve("sidebar.shp");
    DEBUG_ASSERT(SidebarShape != NULL);

    if (SidebarAddonShape != NULL) {
        delete SidebarAddonShape;
    }
    SidebarAddonShape = MixFileClass<CCFileClass>::Retrieve("addon.shp");
}

void SidebarClass::Init_Clear()
{
    PowerClass::Init_Clear();
    SidebarToRedraw = true;
    SidebarBit4 = false;
    SidebarBit8 = false;
    SidebarBit16 = false;
    for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
        Columns[column].Init_Clear();
    }
    Activate(0);
}

void SidebarClass::Init_IO()
{
#ifndef RAPP_STANDALONE
    void (*func)(const SidebarClass *) = reinterpret_cast<void (*)(const SidebarClass *)>(0x0054D144);
    func(this);
#endif
}

void SidebarClass::Init_Theater(TheaterType theater)
{
#ifndef RAPP_STANDALONE
    void (*func)(const SidebarClass *, TheaterType) =
        reinterpret_cast<void (*)(const SidebarClass *, TheaterType)>(0x0054D304);
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
    if (overlap_list != nullptr) {
        if (overlap_list[0] == LIST_START) {
            for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
                Columns[column].StripToRedraw = true;
            }
            Flag_To_Redraw();
        }
    }
    PowerClass::Refresh_Cells(cellnum, overlap_list);
}

void SidebarClass::Reload_Sidebar()
{
// TODO Needs HouseClass
#ifndef RAPP_STANDALONE
    void (*func)(const SidebarClass *) = reinterpret_cast<void (*)(const SidebarClass *)>(0x0054D340);
    func(this);
#else
    // in order of the houses.
    char *sidebarnames[HOUSES_COUNT] = { "SIDE?NA.SHP",
        "SIDE?NA.SHP",
        "SIDE?US.SHP",
        "SIDE?NA.SHP",
        "SIDE?US.SHP",
        "SIDE?NA.SHP",
        "SIDE?NA.SHP",
        "SIDE?NA.SHP",
        "SIDE?NA.SHP",
        "SIDE?US.SHP" };

    int side_index;

    if (PlayerPtr) {
        side_index = PlayerPtr->Side;
    } else {
        side_index = 0;
    }

    // char *shape_name = sidebarnames[side_index];

    // this basicly replaces the '?' in the filenames above with a number.
    SidebarShape = CCMixFileClass::Retrieve((char const *)(sidebarnames[side_index][4] = '1'));
    SidebarMiddleShape = CCMixFileClass::Retrieve((char const *)(sidebarnames[side_index][4] = '2'));
    SidebarBottomShape = CCMixFileClass::Retrieve((char const *)(sidebarnames[side_index][4] = '3'));

    // reload the side specific stip backgrounds.
    Strips[COLUMN_LEFT].Reload_LogoShapes();
    Strips[COLUMN_LEFT].Reload_LogoShapes();
#endif
}

ColumnType SidebarClass::Which_Column(RTTIType type)
{
    DEBUG_ASSERT(this != nullptr);
    switch (type) {
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

BOOL SidebarClass::Factory_Link(int unk1, RTTIType type, int unk3)
{
    DEBUG_ASSERT(this != nullptr);

    return Columns[Which_Column(type)].Factory_Link(unk1, type, unk3);
}

BOOL SidebarClass::Activate_Repair(int unk1)
{
#ifndef RAPP_STANDALONE
    BOOL (*func)(const SidebarClass *, int) = reinterpret_cast<BOOL (*)(const SidebarClass *, int)>(0x0054D464);
    return func(this, unk1);
#endif
}

BOOL SidebarClass::Activate_Upgrade(int unk1)
{
    DEBUG_ASSERT(this != nullptr);

    BOOL unk5 = SidebarToRedraw;

    if (unk1 == -1) {
        unk1 = (SidebarBit8 == 0);
    }

    if (unk1 == 1) {
        SidebarBit8 = true;
    } else {
        SidebarBit8 = false;
    }

    if (SidebarToRedraw != unk5) {
        Flag_To_Redraw();
        SidebarToRedraw = true;

        if (!SidebarBit8) {
            Set_Default_Mouse(MOUSE_POINTER);
        }
    }

    return unk5;
}

BOOL SidebarClass::Activate_Demolish(int unk1)
{
    DEBUG_ASSERT(this != nullptr);

    BOOL unk5 = SidebarToRedraw;

    if (unk1 == -1) {
        unk1 = (SidebarBit16 == 0);
    }

    if (unk1 == 1) {
        SidebarBit16 = true;
    } else {
        SidebarBit16 = false;
    }

    if (SidebarToRedraw != unk5) {
        Flag_To_Redraw();
        SidebarToRedraw = true;

        if (!SidebarBit16) {
            Set_Default_Mouse(MOUSE_POINTER);
        }
    }

    return unk5;
}

BOOL SidebarClass::Add(RTTIType item, int id)
{
    DEBUG_ASSERT(this != nullptr);

    // if ( InMapEditor ) {
    //    return false;
    //}

    // get Left or Right strip and add the "item", based on return value of Which_Column().
    if (Columns[Which_Column(item)].Add(item, id)) {
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
    BOOL(*func)
    (const SidebarClass *, BOOL, ColumnType) =
        reinterpret_cast<BOOL (*)(const SidebarClass *, BOOL, ColumnType)>(0x0054D684);
    return func(this, reverse, column);
#endif
}

void SidebarClass::Recalc()
{
    DEBUG_ASSERT(this != nullptr);

    BOOL unk1 = false;

    for (ColumnType column = COLUMN_FIRST; column < COLUMN_COUNT; ++column) {
        unk1 = Columns[column].Recalc();
        if (unk1) {
            break;
        }
    }

    if (unk1) {
        SidebarToRedraw = true;
        Flag_To_Redraw();
    }
}

BOOL SidebarClass::Activate(int mode)
{
#ifndef RAPP_STANDALONE
    BOOL (*func)(const SidebarClass *, int) = reinterpret_cast<BOOL (*)(const SidebarClass *, int)>(0x0054DA70);
    return func(this, mode);
#endif
}

int SidebarClass::Abandon_Production(RTTIType type, int unk2)
{
    DEBUG_ASSERT(this != nullptr);

    return Columns[Which_Column(type)].Abandon_Production(unk2);
}

void SidebarClass::Zoom_Mode_Control(ModeControl mode)
{
    // TODO, check all bools in this function, OmniBlade?
    // see pesudo in RA, not sure if i have the correct ones.

    if (!RadarActive) {
        if (Session.Game_To_Play() != GAME_CAMPAIGN) {
            Player_Names((RadarZoomed == false));
        }

    } else if (RadarDrawNames || Session.Game_To_Play() == GAME_CAMPAIGN) {
        if (RadarDrawNames || !Spy_Next_House()) {
            Zoom_Mode(Coord_To_Cell(DisplayPos));
        }

    } else if (!RadarPulseActive && !RadarZoomed) {
        Player_Names();

    } else {
        Player_Names(false);

        if (!Spy_Next_House()) {
            Zoom_Mode(Coord_To_Cell(DisplayPos));
        }
    }
}
