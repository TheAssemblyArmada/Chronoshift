/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling map loading and logic.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
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
#include "object.h"
#include "pipe.h"
#include "straw.h"
#include "vector.h"
#include "noinit.h"

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
    virtual void Code_Pointers() override {}
    virtual void Decode_Pointers() override {}

    CellClass &operator[](int index) { return Array[index]; }

    BOOL Place_Random_Crate();
    BOOL Remove_Crate(int16_t cellnum);
    int16_t Pick_Random_Location() const;
    BOOL In_Radar(int16_t cellnum) const;
    void Sight_From(int16_t cellnum, int radius, HouseClass *house, BOOL a4);
    void Shroud_From(int16_t cellnum, int radius);
    void Jam_From(int16_t cellnum, int radius, HouseClass *house);
    void UnJam_From(int16_t cellnum, int radius, HouseClass *house);
    void Place_Down(int16_t cellnum, ObjectClass *object);
    void Pick_Up(int16_t cellnum, ObjectClass *object);
    void Overlap_Down(int16_t cellnum, ObjectClass *object);
    void Overlap_Up(int16_t cellnum, ObjectClass *object);
    int Overpass();
    int32_t Cell_Region(int16_t cellnum);
    int Cell_Threat(int16_t cellnum, HousesType house);
    int Zone_Reset(int zones);
    int Zone_Span(int16_t cell, int zone, MZoneType mzone);
    int16_t Nearby_Location(int16_t cellnum, SpeedType speed, int zone, MZoneType mzone) const;
    BOOL Base_Region(int16_t cellnum, HousesType &house, ZoneType &zone) const;
    int Destroy_Bridge_At(int16_t cellnum);
    void Detach(int32_t target, int a2);
    int Intact_Bridge_Count() const;
    void Shroud_The_Map();
    int Write_Binary(Pipe &pipe);
    BOOL Read_Binary(Straw &straw);
    BOOL Validate();
    ObjectClass *Close_Object(uint32_t coord) const;

    int16_t Cell_Number(CellClass *cell) { return Array.ID(cell); }
    int Get_Map_Cell_Width() const {return MapCellWidth;}
    int Get_Map_Cell_Height() const {return MapCellHeight;}

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
    CrateClass Crates[256];
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
