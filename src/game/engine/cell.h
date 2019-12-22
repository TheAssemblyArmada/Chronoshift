/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class containing information pertaining to a single map cell.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef CELL_H
#define CELL_H

#include "always.h"
#include "gametypes.h"
#include "land.h"
#include "mzone.h"
#include "object.h"
#include "overlaytype.h"
#include "smudgetype.h"
#include "templatetype.h"

class TechnoClass;
class InfantryClass;
class UnitClass;
class BuildingClass;
class VesselClass;
class AircraftClass;
class TerrainClass;
class FootClass;
class Straw;
class Pipe;

#define OVERLAPPER_COUNT 6

enum CellOccupantEnum
{
    OCCUPANT_NONE = 0x0,
    INFANTRY_SPOT_NONE = 0x0,
    INFANTRY_SPOT_TOP_LEFT = 0x1,
    INFANTRY_SPOT_TOP_RIGHT = 0x2,
    INFANTRY_SPOT_BOTTOM_LEFT = 0x4,
    INFANTRY_SPOT_BOTTOM_RIGHT = 0x8,
    INFANTRY_SPOT_CENTER = 0x10,
    OCCUPANT_INFANTRY = INFANTRY_SPOT_TOP_LEFT | INFANTRY_SPOT_TOP_RIGHT | INFANTRY_SPOT_BOTTOM_LEFT
        | INFANTRY_SPOT_BOTTOM_RIGHT | INFANTRY_SPOT_CENTER,
    OCCUPANT_UNIT = 0x20, // unit, vessel or aircraft.
    OCCUPANT_TERRAIN = 0x40,
    OCCUPANT_BUILDING = 0x80,
    CELL_SPOT_COUNT = 5,
};

class CellClass
{
    enum
    {
        ORESTAGE_SPREADING = 6,
        ORESTAGE_FULLGROWN = 11,

        PLACEMENT_CLEAR = 0,
        PLACEMENT_YELLOW = 1,
        PLACEMENT_RED = 2,
        PLACEMENT_SELECT = 3,
    };

public:
    CellClass();
    CellClass(const CellClass &that) {}
    CellClass(const NoInitClass &noinit) {}
    ~CellClass() { m_OccupierPtr = nullptr; } // Null the pointer in memory, but object still exists

    BOOL operator==(const CellClass &that) const;
    BOOL operator!=(const CellClass &that) const { return !(m_CellNumber == that.m_CellNumber); }

    int Cell_Color(BOOL none = false) const;
    ObjectClass *Cell_Find_Object(RTTIType type) const;
    TechnoClass *Cell_Techno(int x = 0, int y = 0) const;
    ObjectClass *Cell_Object(int x = 0, int y = 0) const;
    BuildingClass *Cell_Building() const;
    InfantryClass *Cell_Infantry() const;
    UnitClass *Cell_Unit() const;
    VesselClass *Cell_Vessel() const;
    AircraftClass *Cell_Aircraft() const;
    TerrainClass *Cell_Terrain() const;
    // SmudgeClass *Cell_Smudge() const;
    // OverlayClass *Cell_Overlay() const;
    coord_t Cell_Coord() const;
    void Recalc_Attributes();
    BOOL Can_Ore_Grow() const;
    BOOL Can_Ore_Spread() const;
    BOOL Can_Ore_Germinate() const;
    BOOL Contains_Ore() const;
    BOOL Grow_Ore();
    BOOL Spread_Ore(BOOL force);
    CellClass &Adjacent_Cell(FacingType facing);
    BOOL Is_Bridge_Here() const;
    void Redraw_Objects(BOOL force = false);
    BOOL Is_Clear_To_Build(SpeedType speed) const;
    void Occupy_Down(ObjectClass *object);
    void Occupy_Up(ObjectClass *object);
    void Overlap_Down(ObjectClass *object);
    void Overlap_Up(ObjectClass *object);
    int Clear_Icon() const;
    void Draw_It(int x, int y, BOOL flag = false) const;
    void Concrete_Calc();
    void Wall_Update();
    int Reduce_Ore(int reduction);
    BOOL Reduce_Wall(int damage);
    BOOL Is_Clear_To_Move(SpeedType speed, BOOL ignore_crushable = false, BOOL ignore_destructable  = false, int zone = ZONE_NONE, MZoneType mzone = MZONE_NORMAL) const;
    int Ore_Adjust(BOOL randomize);
    coord_t Closest_Free_Spot(coord_t coord, BOOL skip_occupied) const;
    BOOL Is_Spot_Free(int spotindex) const { return ((1 << spotindex) & m_OccupantBit) == 0; }
    BOOL Goodie_Check(FootClass *foot);
    BOOL Should_Save() const;
    BOOL Load(Straw &straw);
    BOOL Save(Pipe &pipe) const;
    BOOL Has_Crater() const { return m_Smudge != SMUDGE_NONE && SmudgeTypeClass::As_Pointer(m_Smudge)->Is_Crater(); }
    BOOL Has_Bib() const { return m_Smudge != SMUDGE_NONE && SmudgeTypeClass::As_Pointer(m_Smudge)->Is_Bib(); }
    BOOL Has_Wall() const { return m_Overlay != OVERLAY_NONE && OverlayTypeClass::As_Pointer(m_Overlay)->Is_Wall(); }
    BOOL Has_High_Overlay() const { return m_Overlay != OVERLAY_NONE && OverlayTypeClass::As_Pointer(m_Overlay)->Is_High(); }
    void Clear_Occupants(CellOccupantEnum bit) { m_OccupantBit &= ~bit; }
    void Set_Occupants(CellOccupantEnum bit) { m_OccupantBit |= bit; }
    BOOL Check_Occupants(CellOccupantEnum bitmask) const { return m_OccupantBit & bitmask; }
    BOOL Is_Occuppied() const { return m_OccupantBit != OCCUPANT_NONE; }

    cell_t Cell_Number() const { return m_CellNumber; }
    int8_t Get_Zone(MZoneType mzone) const { return m_Zones[mzone]; }
    void Set_Zone(MZoneType mzone, int8_t zone) { m_Zones[mzone] = zone; }
    BOOL Get_Placement_Check() const { return m_PlacementCheck; }
    void Set_Placement_Check(BOOL check) { m_PlacementCheck = check; }
    BOOL Is_Visible() const { return m_Visible; }
    void Set_Visible(BOOL value) { m_Visible = value; }
    BOOL Is_Revealed() const { return m_Revealed; }
    void Set_Revealed(BOOL value) { m_Revealed = value; }
    BOOL Get_Bit1() const { return m_Bit1; }
    void Set_Bit1(BOOL value) { m_Bit1 = value; }
    BOOL Get_Bit16() const { return m_Bit16; }
    void Set_Bit16(BOOL value) { m_Bit16 = value; }
    BOOL Get_Bit32() const { return m_Bit32; }
    void Set_Bit32(BOOL value) { m_Bit32 = value; }
    BOOL Has_Flag() const { return m_HasFlag; }
    BOOL Get_Bit128() const { return m_Bit128; }
    void Set_Bit128(BOOL value) { m_Bit128 = value; }
    TemplateType Get_Template() const { return m_Template; }
    void Set_Template(TemplateType temp) { m_Template = temp; }
    uint8_t Get_Icon() const { return m_Icon; }
    void Set_Icon(uint8_t icon) { m_Icon = icon; }
    OverlayType Get_Overlay() const { return m_Overlay; }
    void Set_Overlay(OverlayType overlay) { m_Overlay = overlay; }
    int8_t Get_Overlay_Frame() const { return m_OverlayFrame; }
    void Set_Overlay_Frame(int8_t frame) { m_OverlayFrame = frame; }
    SmudgeType Get_Smudge() const { return m_Smudge; }
    void Set_Smudge(SmudgeType smudge) { m_Smudge = smudge; }
    uint8_t Get_Smudge_Frame() const { return m_SmudgeFrame; }
    void Set_Smudge_Frame(int8_t frame) { m_SmudgeFrame = frame; }
    HousesType Owner() const { return m_OwnerHouse; }
    void Set_Owner(HousesType owner) { m_OwnerHouse = owner; }
    ObjectClass *Get_Overlapper(int index) { return m_Overlapper[index]; }
    ObjectClass *Get_Occupier() const { return m_OccupierPtr; }
    LandType Get_Land() const { return m_Land; }
    void Set_Land(LandType land) { m_Land = land; }
    uint16_t Get_Field_A() const { return m_field_A; }
    void Set_Field_A(HousesType house) { m_field_A |= (1 << house); }
    void Clear_Field_A(HousesType house) { m_field_A &= ~(1 << house); }
    static int Spot_Index(coord_t coord);
    void Adjust_Threat(HousesType house, int threat);
    void Shimmer();
    void Incoming(coord_t coord, BOOL a2 = false, BOOL a3 = false);

    void Code_Pointers();
    void Decode_Pointers();

public:
    static cell_t CurrentSelectedCell;

#ifdef GAME_DLL
public:
    CellClass *Hook_Ctor() { return new (this) CellClass; }
    void Hook_Draw_It(int x, int y, BOOL unk_bool) { CellClass::Draw_It(x, y, unk_bool); }
    TechnoClass *Hook_Cell_Techno(int x, int y) { return CellClass::Cell_Techno(x, y); }
    ObjectClass *Hook_Cell_Find_Object(RTTIType type) { return CellClass::Cell_Find_Object(type); }
    ObjectClass *Hook_Cell_Object(int x, int y) { return CellClass::Cell_Object(x, y); }
    BOOL Hook_Is_Clear_To_Move(SpeedType speed, BOOL ignore_crushable, BOOL ignore_destructable, int zone, MZoneType mzone)
    {
        return CellClass::Is_Clear_To_Move(speed, ignore_crushable, ignore_destructable, zone, mzone);
    }
#endif

private:
    cell_t m_CellNumber;

#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_Bit1 : 1; // 1
    BOOL m_PlacementCheck : 1; // 2
    BOOL m_Visible : 1; // 4 // Is this cell at least partly visible due ot being next to a revealed or visible cell.
    BOOL m_Revealed : 1; // 8 // Is this cell fully revealed and thus has no shroud at all.
    BOOL m_Bit16 : 1; // 16    //Could be HasWaypoint?  HasCellTag?
    BOOL m_Bit32 : 1; // 32    //MarkedOnRadar? IsWaypoint in C&C ? does the radar cursor cover this cell?
    BOOL m_HasFlag : 1; // 64
    BOOL m_Bit128 : 1; // 128 // HasFlag in C&C relates to Region here. Look like it marks a cell to advance shroud.
#else
    bool m_Bit1;
    bool m_PlacementCheck;
    bool m_Visible; // Is this cell at least partly visible due ot being next to a revealed or visible cell.
    bool m_Revealed; // Is this cell fully revealed and thus has no shroud at all.
    bool m_Bit16; // Could be HasWaypoint?  HasCellTag?
    bool m_Bit32; // MarkedOnRadar?  appears to be IsWaypoint in C&C     //does the radar cursor cover this cell?
    bool m_HasFlag;
    bool m_Bit128; // HasFlag in C&C
#endif

    int8_t m_Zones[MZONE_COUNT]; // field_6
    uint16_t m_field_A;
    int m_CellTag; // GamePtr<TriggerClass> CellTag; // Needs TriggerClass
    TemplateType m_Template;
    uint8_t m_Icon;
    OverlayType m_Overlay;
    int8_t m_OverlayFrame;
    SmudgeType m_Smudge;
    int8_t m_SmudgeFrame;
    HousesType m_OwnerHouse;
    HousesType m_field_18;
    ObjectClass *m_OccupierPtr;
    ObjectClass *m_Overlapper[OVERLAPPER_COUNT];
    uint32_t m_OccupantBit;
    LandType m_Land;
    static const coord_t s_StoppingCoordAbs[CELL_SPOT_COUNT];
};

inline BOOL CellClass::Contains_Ore() const
{
    return m_Overlay == OVERLAY_GOLD_01 || m_Overlay == OVERLAY_GOLD_02 || m_Overlay == OVERLAY_GOLD_03
        || m_Overlay == OVERLAY_GOLD_04;
}

inline BOOL CellClass::operator==(CellClass const &that) const
{
    return m_CellNumber == that.m_CellNumber && m_Bit1 == that.m_Bit1 && m_PlacementCheck == that.m_PlacementCheck
        && m_Visible == that.m_Visible && m_Revealed == that.m_Revealed && m_Bit16 == that.m_Bit16 && m_Bit32 == that.m_Bit32
        && m_HasFlag == that.m_HasFlag && m_Bit128 == that.m_Bit128 && m_field_A == that.m_field_A
        && m_CellTag == that.m_CellTag && m_Template == that.m_Template && m_Icon == that.m_Icon
        && m_Overlay == that.m_Overlay && m_OverlayFrame == that.m_OverlayFrame && m_Smudge == that.m_Smudge
        && m_SmudgeFrame == that.m_SmudgeFrame && m_OwnerHouse == that.m_OwnerHouse && m_field_18 == that.m_field_18
        && m_OccupierPtr == that.m_OccupierPtr && m_OccupantBit == that.m_OccupantBit && m_Land == that.m_Land
        && memcmp(m_Zones, that.m_Zones, sizeof(m_Zones)) == 0
        && memcmp(m_Overlapper, that.m_Overlapper, sizeof(m_Overlapper)) == 0;
}

#endif // CELL_H
