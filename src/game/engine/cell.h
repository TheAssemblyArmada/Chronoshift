/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class containing information pertaining to a single map cell.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
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
    OCCUPANT_INFANTRY = INFANTRY_SPOT_TOP_LEFT
    | INFANTRY_SPOT_TOP_RIGHT
    | INFANTRY_SPOT_BOTTOM_LEFT
    | INFANTRY_SPOT_BOTTOM_RIGHT
    | INFANTRY_SPOT_CENTER,
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
        PLACEMENT_SOMETHING = 3,
    };
public:
    CellClass();
    CellClass(CellClass const &that) {}
    CellClass(NoInitClass const &noinit) {}
    ~CellClass() { OccupierPtr = nullptr; } //Null the pointer in memory, but object still exists

    BOOL operator==(CellClass const &that) const { return CellNumber == that.CellNumber; }
    BOOL operator!=(CellClass const &that) const { return CellNumber != that.CellNumber; }

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
    //SmudgeClass *Cell_Smudge() const;
    //OverlayClass *Cell_Overlay() const;
    uint32_t Cell_Coord() const;
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
    void Draw_It(int x, int y, BOOL unk_bool) const;
    void Concrete_Calc();
    void Wall_Update();
    int Reduce_Ore(int reduction);
    BOOL Reduce_Wall(int damage);
    BOOL Is_Clear_To_Move(SpeedType speed, BOOL ignore_crushable, BOOL ignore_destructable, int zone, MZoneType mzone) const;
    int Ore_Adjust(BOOL randomize);
    uint32_t Closest_Free_Spot(uint32_t coord, BOOL skip_occupied) const;
    BOOL Is_Spot_Free(int spotindex) const { return ((1 << spotindex) & OccupantBit) == 0; }

    int Cell_Number() const { return CellNumber; }
    int8_t Get_Zone(MZoneType mzone) const { return Zones[mzone]; }
    void Set_Zone(MZoneType mzone, int8_t zone) { Zones[mzone] = zone; }
    BOOL Get_Placement_Check() const { return PlacementCheck; }
    void Set_Placement_Check(BOOL check) { PlacementCheck = check; }
    BOOL Get_Bit4() const { return Bit4; }
    void Set_Bit4(BOOL bit) { Bit4 = bit; }
    BOOL Get_Bit8() const { return Bit8; }
    void Set_Bit8(BOOL bit) { Bit8 = bit; }
    BOOL Get_Bit128() { return Bit128; }
    void Set_Bit128(BOOL bit) { Bit128 = bit; }
    TemplateType Get_Template() const { return Template; }
    void Set_Template(TemplateType temp) { Template = temp; }
    uint8_t Get_Icon() const { return Icon; }
    void Set_Icon(uint8_t icon) { Icon = icon; }
    OverlayType Get_Overlay() const { return Overlay; }
    void Set_Overlay(OverlayType overlay) { Overlay = overlay; }
    int8_t Get_Overlay_Frame() const { return OverlayFrame; }
    void Set_Overlay_Frame(int8_t frame) { OverlayFrame = frame; }
    SmudgeType Get_Smudge() const { return Smudge; }
    void Set_Smudge(SmudgeType smudge) { Smudge = smudge; }
    HousesType Owner() const { return OwnerHouse; }
    ObjectClass *Get_Overlapper(int index) { return Overlapper[index]; }
    ObjectClass *Get_Occupier() const { return OccupierPtr; }
    LandType Get_Land() { return Land; }
    void Set_Land(LandType land) { Land = land; }

    static int Spot_Index(uint32_t coord);

private:
    int16_t CellNumber;

#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool Bit1 : 1; // 1
            bool PlacementCheck : 1; // 2
            bool Bit4 : 1; // 4 // Shrouded? Looks like it means partly visible, a neighbour is revealed.
            bool Bit8 : 1; // 8 //Revealed? //Occupied? //NotSeen? //#define	cf_Explored	0x08 //this means no shroud
            bool Bit16 : 1; // 16	//Could be HasWaypoint?  HasCellTag?
            bool Bit32 : 1; // 32    //MarkedOnRadar? IsWaypoint in C&C ? does the radar cursor cover this cell?
            bool HasFlag : 1; // 64
            bool Bit128 : 1; // 128 // HasFlag in C&C relates to Region here. Look like it marks a cell to advance shroud.
        };
        int Bitfield;
    };
#else
    bool Bit1; // 1
    bool PlacementCheck; // 2
    bool Bit4; // 4            //Shrouded?
    bool Bit8; // 8 //Revealed? //Occupied? //NotSeen? //#define	cf_Explored	0x08 //this means no shroud
    bool Bit16; // 16	//Could be HasWaypoint?  HasCellTag?
    bool Bit32; // 32    //MarkedOnRadar?  appears to be IsWaypoint in C&C     //does the radar cursor cover this cell?
    bool HasFlag; // 64
    bool Bit128; // HasFlag in C&C
#endif

    int8_t Zones[MZONE_COUNT]; // field_6
    uint16_t field_A;
    int CellTag; // CCPtr<TriggerClass> CellTag; // Needs TriggerClass
    TemplateType Template;
    uint8_t Icon;
    OverlayType Overlay;
    int8_t OverlayFrame;
    SmudgeType Smudge;
    int8_t SmudgeFrame;
    HousesType OwnerHouse;
    HousesType field_18;
    ObjectClass *OccupierPtr;
    ObjectClass *Overlapper[OVERLAPPER_COUNT];
    uint32_t OccupantBit;
    LandType Land;
    static const uint32_t StoppingCoordAbs[CELL_SPOT_COUNT];
};

inline BOOL CellClass::Contains_Ore() const
{
    return Overlay == OVERLAY_GOLD_01 || Overlay == OVERLAY_GOLD_02 || Overlay == OVERLAY_GOLD_03
        || Overlay == OVERLAY_GOLD_04;
}

#endif // CELL_H
