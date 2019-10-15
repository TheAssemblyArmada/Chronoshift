/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class containing information about terrain object behaviour.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
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

enum TerrainType
{
    TERRAIN_TREE1,
    TERRAIN_TREE2,
    TERRAIN_TREE3,
    TERRAIN_TREE5,
    TERRAIN_TREE6,
    TERRAIN_TREE7,
    TERRAIN_TREE8,
    TERRAIN_TREE10,
    TERRAIN_TREE11,
    TERRAIN_TREE12,
    TERRAIN_TREE13,
    TERRAIN_TREE14,
    TERRAIN_TREE15,
    TERRAIN_TREE16,
    TERRAIN_TREE17,
    TERRAIN_TREECLUMP1,
    TERRAIN_TREECLUMP2,
    TERRAIN_TREECLUMP3,
    TERRAIN_TREECLUMP4,
    TERRAIN_TREECLUMP5,
    TERRAIN_ICE01,
    TERRAIN_ICE02,
    TERRAIN_ICE03,
    TERRAIN_ICE04,
    TERRAIN_ICE05,
    TERRAIN_BOXES01,
    TERRAIN_BOXES02,
    TERRAIN_BOXES03,
    TERRAIN_BOXES04,
    TERRAIN_BOXES05,
    TERRAIN_BOXES06,
    TERRAIN_BOXES07,
    TERRAIN_BOXES08,
    TERRAIN_BOXES09,
    TERRAIN_MINE,
    //TERRAIN_FLAGFLY, // TOOD: EDWIN addition, will be required for map editor.

    TERRAIN_COUNT,

    TERRAIN_NONE = -1,
    TERRAIN_FIRST = TERRAIN_TREE1
};

DEFINE_ENUMERATION_OPERATORS(TerrainType);

class HouseClass;

class TerrainTypeClass : public ObjectTypeClass
{
public:
    TerrainTypeClass(TerrainType type, int theater, uint32_t a3, BOOL immune, BOOL waterbound, const char *name, int uiname, const int16_t *occupy_list, const int16_t *overlap_list);
    TerrainTypeClass(const TerrainTypeClass &that);
    TerrainTypeClass(const NoInitClass &noinit) : ObjectTypeClass(noinit) {}
    ~TerrainTypeClass();

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    virtual coord_t Coord_Fixup(coord_t coord) const override;
    virtual BOOL Create_And_Place(cell_t cellnum, HousesType house = HOUSES_NONE) const override;
    virtual ObjectClass *Create_One_Of(HouseClass *house) const override;
    virtual const int16_t *Occupy_List(BOOL a1 = false) const override;
    virtual const int16_t *Overlap_List() const override;

    void Code_Pointers() {}
    void Decode_Pointers() {}

    uint32_t Get_Theater() const { return Theater; }
    TerrainType Get_Type() const { return Type; }
    coord_t Get_UnkCoord() const { return UnkCoord; }
    bool Is_Waterbound() const { return WaterBound; }

    static void Init_Heap();
    static void One_Time() {}
    static void Init(TheaterType theater);
    static TerrainType From_Name(const char *name);
    static const char *Name_From(TerrainType terrain);
    static TerrainTypeClass &As_Reference(TerrainType terrain);
    static TerrainTypeClass *As_Pointer(TerrainType terrain);

    static void Prep_For_Add();

public:
    TerrainType Type; 
    coord_t UnkCoord; // Object bounds perhaps
    uint32_t Theater; // Bitfield of which theaters this terrain is allowed to appear in.
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL WaterBound : 1; // & 1
#else
    bool WaterBound; // Is the terrain only allowed on the water (def = false)?
#endif
    const int16_t *OccupyList;
    const int16_t *OverlapList;
};

#ifdef GAME_DLL
#include "hooker.h"
extern TFixedIHeapClass<TerrainTypeClass> &g_TerrainTypes;
#else
extern TFixedIHeapClass<TerrainTypeClass> g_TerrainTypes;
#endif

#endif // TERRAINTYPE_H
