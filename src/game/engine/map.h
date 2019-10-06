/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling map loading and logic.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef MAP_H
#define MAP_H

#include "always.h"
#include "cell.h"
#include "crate.h"
#include "gscreen.h"
#include "vector.h"

class Pipe;
class Straw;
class ObjectClass;

class MapClass : public GameScreenClass
{
    enum
    {
        MAP_MAX_CRATES = 256,
    };
public:
    MapClass();
    MapClass(const NoInitClass &noinit) : GameScreenClass(noinit), Array(noinit) {}

    virtual void One_Time() override;
    virtual void Init_Clear() override;
    virtual void Alloc_Cells() override;
    virtual void Free_Cells() override;
    virtual void Init_Cells() override;
    virtual void Detach(ObjectClass *object) override {}
    virtual void Logic_AI() override;
    virtual void Set_Map_Dimensions(int x, int y, int w, int h) override;
    virtual void Code_Pointers() override;
    virtual void Decode_Pointers() override;

    CellClass &operator[](int index) { return Array[index]; }

    BOOL Place_Random_Crate();
    BOOL Place_Random_Crate_At_Cell(cell_t cellnum);
    BOOL Remove_Crate(cell_t cellnum);
    cell_t Pick_Random_Location() const;
    BOOL In_Radar(cell_t cellnum) const;
    cell_t Clamp_To_Radar(cell_t cellnum) const;
    void Sight_From(cell_t cellnum, int radius, HouseClass *house, BOOL a4);
    void Shroud_From(cell_t cellnum, int radius);
    void Jam_From(cell_t cellnum, int radius, HouseClass *house);
    void UnJam_From(cell_t cellnum, int radius, HouseClass *house);
    void Place_Down(cell_t cellnum, ObjectClass *object);
    void Pick_Up(cell_t cellnum, ObjectClass *object);
    void Overlap_Down(cell_t cellnum, ObjectClass *object);
    void Overlap_Up(cell_t cellnum, ObjectClass *object);
    int Overpass();
    int32_t Cell_Region(cell_t cellnum);
    int Cell_Threat(cell_t cellnum, HousesType house);
    int Zone_Reset(int zones);
    int Zone_Span(cell_t cell, int zone, MZoneType mzone);
    cell_t Nearby_Location(cell_t cellnum, SpeedType speed, int zone, MZoneType mzone) const;
    BOOL Base_Region(cell_t cellnum, HousesType &house, ZoneType &zone) const;
    int Destroy_Bridge_At(cell_t cellnum);
    void Detach(int32_t target, int a2);
    int Intact_Bridge_Count() const;
    void Shroud_The_Map();
    int Write_Binary(Pipe &pipe);
    BOOL Read_Binary(Straw &straw);
    BOOL Validate();
    ObjectClass *Close_Object(coord_t coord) const;

    cell_t Cell_Number(CellClass *cell) { return Array.ID(cell); }
    int Get_Map_Cell_X() const {return MapCellX; }
    int Get_Map_Cell_Y() const {return MapCellY; }
    int Get_Map_Cell_Width() const {return MapCellWidth; }
    int Get_Map_Cell_Height() const {return MapCellHeight; }

#ifdef GAME_DLL
    cell_t MapClass::Hook_Pick_Random_Location() { return Pick_Random_Location(); }
    BOOL MapClass::Hook_In_Radar(cell_t cellnum) { return In_Radar(cellnum); }

    cell_t MapClass::Hook_Nearby_Location(cell_t cellnum, SpeedType speed, int zone, MZoneType mzone)
    {
        return Nearby_Location(cellnum, speed, zone, mzone);
    }

    int MapClass::Hook_Intact_Bridge_Count() { return Intact_Bridge_Count(); }
    ObjectClass *Hook_Close_Object(coord_t coord) { return Close_Object(coord); }
    BOOL Hook_Base_Region(cell_t cellnum, HousesType &house, ZoneType &zone) { return Base_Region(cellnum, house, zone); }
#endif

protected:
    int MapCellX;
    int MapCellY;
    int MapCellWidth;
    int MapCellHeight;
    int TotalValue;
    VectorClass<CellClass> Array;
    int XSize;
    int YSize;
    int TotalSize;
    CrateClass Crates[MAP_MAX_CRATES];
    int16_t OreGrowth[64];
    int OreGrowthCount;
    int OreGrowthExcess;
    int16_t OreSpread[64];
    int OreSpreadCount;
    int OreSpreadExcess;
    int16_t OreLogicPos;

private:
    static const int RadiusOffset[];
    static const int RadiusCount[];
};

#endif // MAP_H
