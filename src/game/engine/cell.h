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

class CellClass
{
public:
    CellClass() {} // TODO
    CellClass(CellClass const &that) {}
    CellClass(NoInitClass const &noinit) {}
    ~CellClass() {}

    BOOL operator==(CellClass const &that) const { return CellNumber == that.CellNumber; }
    BOOL operator!=(CellClass const &that) const { return CellNumber != that.CellNumber; }

    int Cell_Color(BOOL a1 = false) const;
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

private:
    int16_t CellNumber;

#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool Bit1 : 1; // 1
            bool Bit2 : 1; // 2
            bool Bit4 : 1; // 4            //Shrouded?
            bool Bit8 : 1; // 8 //Revealed? //Occupied? //NotSeen? //#define	cf_Explored	0x08 //this means no shroud
            bool Bit16 : 1; // 16	//Could be HasWaypoint?  HasCellTag?
            bool Bit32 : 1; // 32    //MarkedOnRadar? IsWaypoint in C&C ? does the radar cursor cover this cell?
            bool HasFlag : 1; // 64
            bool Bit128 : 1; // 128 // HasFlag in C&C
        };
        int Bitfield;
    };
#else
    bool Bit1; // 1
    bool Bit2; // 2
    bool Bit4; // 4            //Shrouded?
    bool Bit8; // 8 //Revealed? //Occupied? //NotSeen? //#define	cf_Explored	0x08 //this means no shroud
    bool Bit16; // 16	//Could be HasWaypoint?  HasCellTag?
    bool Bit32; // 32    //MarkedOnRadar?  appears to be IsWaypoint in C&C     //does the radar cursor cover this cell?
    bool HasFlag; // 64
    bool Bit128; // HasFlag in C&C
#endif

    char field_6[MZONE_COUNT];
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
    ObjectClass *Overlapper[6];
    uint32_t OccupantBit;
    LandType Land;
};

#endif // CELL_H
