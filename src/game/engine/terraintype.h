/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class containing information about terrain object behaviour.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef TERRAINTYPE_H
#define TERRAINTYPE_H

#include "always.h"
#include "heap.h"
#include "objecttype.h"
#include "theater.h"

enum TerrainType {
    TERRAIN_NONE = -1,
    TERRAIN_FIRST = 0,
    TERRAIN_TREE1 = 0,
    TERRAIN_TREE2 = 1,
    TERRAIN_TREE3 = 2,
    TERRAIN_TREE5 = 3,
    TERRAIN_TREE6 = 4,
    TERRAIN_TREE7 = 5,
    TERRAIN_TREE8 = 6,
    TERRAIN_TREE10 = 7,
    TERRAIN_TREE11 = 8,
    TERRAIN_TREE12 = 9,
    TERRAIN_TREE13 = 10,
    TERRAIN_TREE14 = 11,
    TERRAIN_TREE15 = 12,
    TERRAIN_TREE16 = 13,
    TERRAIN_TREE17 = 14,
    TERRAIN_TREECLUMP1 = 15,
    TERRAIN_TREECLUMP2 = 16,
    TERRAIN_TREECLUMP3 = 17,
    TERRAIN_TREECLUMP4 = 18,
    TERRAIN_TREECLUMP5 = 19,
    TERRAIN_ICE01 = 20,
    TERRAIN_ICE02 = 21,
    TERRAIN_ICE03 = 22,
    TERRAIN_ICE04 = 23,
    TERRAIN_ICE05 = 24,
    TERRAIN_BOXES01 = 25,
    TERRAIN_BOXES02 = 26,
    TERRAIN_BOXES03 = 27,
    TERRAIN_BOXES04 = 28,
    TERRAIN_BOXES05 = 29,
    TERRAIN_BOXES06 = 30,
    TERRAIN_BOXES07 = 31,
    TERRAIN_BOXES08 = 32,
    TERRAIN_BOXES09 = 33,
    TERRAIN_MINE = 34,
    TERRAIN_FLAGFLY = 35,
    TERRAIN_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(TerrainType);

class HouseClass;

class TerrainTypeClass : public ObjectTypeClass
{
public:
    TerrainTypeClass(TerrainType type, int theater, uint32_t a3, BOOL immune, BOOL waterbound, const char *name, int uiname, const int16_t *occupy_list, const int16_t *overlap_list);
    TerrainTypeClass(TerrainTypeClass const &that);
    TerrainTypeClass(NoInitClass const &noinit) : ObjectTypeClass(noinit) {}
    ~TerrainTypeClass();

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    virtual uint32_t Coord_Fixup(uint32_t coord) const override;
    virtual BOOL Create_And_Place(int16_t cellnum, HousesType house = HOUSES_NONE) const override;
    virtual ObjectClass *Create_One_Of(HouseClass *house) const override;
    virtual const int16_t *Occupy_List(BOOL a1 = false) const override;
    virtual const int16_t *Overlap_List() const override;

    void Code_Pointers() {}
    void Decode_Pointers() {}

    static void Init_Heap();
    static void One_Time() {}
    static void Init(TheaterType theater);
    static TerrainType From_Name(const char *name);
    static const char *Name_From(TerrainType terrain);
    static TerrainTypeClass &As_Reference(TerrainType terrain);
    static TerrainTypeClass *As_Pointer(TerrainType terrain);

public:
    TerrainType Type; 
    uint32_t UnkInt; // Object bounds perhaps, packed coord?
    uint32_t Theater; // Bitfield of which theaters this terrain is allowed to appear in.
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool WaterBound : 1; // Is the terrain only allowed on the water (def = false)?
        };
        int Bitfield;
    };
#else
    bool WaterBound; // Is the terrain only allowed on the water (def = false)?
#endif
    const int16_t *OccupyList;
    const int16_t *OverlapList;
};

#ifndef RAPP_STANDALONE
#include "hooker.h"
extern TFixedIHeapClass<TerrainTypeClass> &TerrainTypes;
#else
extern TFixedIHeapClass<TerrainTypeClass> TerrainTypes;
#endif

#endif // TERRAINTYPE_H
