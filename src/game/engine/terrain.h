/**
 * @file
 *
 * @author tomsons26
 *
 * @brief
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */

#pragma once

#ifndef TERRAIN_H
#define TERRAIN_H

#include "always.h"
#include "dialog.h"
#include "facing.h"
#include "scenario.h"
#include "techno.h"
#include "terraintype.h"
#include "warheadtype.h"

class TerrainClass : public ObjectClass
{
    friend void Setup_Hooks();

public:
    TerrainClass(TerrainType type, cell_t cellnum = -1);
    TerrainClass(const TerrainClass &that);
    TerrainClass(const NoInitClass &noinit) : ObjectClass(noinit) {}
    virtual ~TerrainClass();

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    // AbstractClass
    virtual coord_t Center_Coord() const override;
    virtual coord_t Target_Coord() const override;
    virtual MoveType Can_Enter_Cell(cell_t cellnum, FacingType facing = FACING_NONE) const override;
    virtual void AI() override;

    // ObjectClass
    virtual const TerrainTypeClass &Class_Of() const override { return *m_Class; }
    virtual coord_t Render_Coord() const override { return m_Coord; }
    virtual coord_t Sort_Y() const override;
    virtual BOOL Limbo() override;
    virtual BOOL Unlimbo(coord_t coord, DirType dir = DIR_NORTH) override;
    virtual void Draw_It(int x, int y, WindowNumberType window) const override;
    virtual BOOL Mark(MarkType mark) override;
    virtual void Clicked_As_Target(int a1) override {}
    virtual DamageResultType Take_Damage(
        int &damage, int a2, WarheadType warhead, TechnoClass *object = nullptr, BOOL a5 = false) override;
    virtual BOOL Catch_Fire() override;
    virtual void Fire_Out() override;

    TerrainType What_Type() const { return m_Class->Get_Type(); }
    uint8_t *const Radar_Icon(short cellnum) const;

private:
    bool Theater_Allowed() const { return ((1 << g_Scen.Get_Theater()) & m_Class->Get_Theater()); }
    void Start_To_Crumble();

private:
    StageClass m_AnimStage;
    GamePtr<TerrainTypeClass> m_Class;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_OnFire : 1; // & 1
    BOOL m_Crumbling : 1; // & 2
#else
    bool m_OnFire; // is this terrain object on fire
    bool m_Crumbling; // is this terrain object crumbling
#endif

#ifdef GAME_DLL
private:
    coord_t Hook_Center_Coord() { return TerrainClass::Center_Coord(); }
    coord_t Hook_Target_Coord() { return TerrainClass::Target_Coord(); }
    MoveType Hook_Can_Enter_Cell(cell_t cellnum, FacingType facing) { return TerrainClass::Can_Enter_Cell(cellnum, facing); }
    coord_t Hook_Sort_Y() { return TerrainClass::Sort_Y(); }
    void Hook_Draw_It(int x, int y, WindowNumberType window) { TerrainClass::Draw_It(x, y, window); }
    uint8_t *const Hook_Radar_Icon(short cellnum) { return TerrainClass::Radar_Icon(cellnum); }
#endif
};

#ifdef GAME_DLL
extern TFixedIHeapClass<TerrainClass> &g_Terrains;
#else
extern TFixedIHeapClass<TerrainClass> g_Terrains;
#endif

#endif // TERRAIN_H
