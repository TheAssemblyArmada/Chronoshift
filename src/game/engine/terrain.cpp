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
#include "terrain.h"
#include "target.h"
#include "anim.h"
#include "cell.h"
#include "drawshape.h"
#include "globals.h"
#include "iomap.h"
#include "lists.h"
#include "rules.h"

#ifndef GAME_DLL
TFixedIHeapClass<TerrainClass> g_Terrains;
#endif

/**
 * @brief
 *
 * @address 0x0056A3FC
 */
TerrainClass::TerrainClass(TerrainType type, cell_t cellnum) :
    ObjectClass(RTTI_TERRAIN, g_Terrains.ID(this)),
    m_AnimStage(),
    m_Type(g_TerrainTypes.Ptr(type)),
    m_OnFire(false),
    m_Crumbling(false)
{
    Set_Health(m_Type->Get_Strength());
    if (cellnum != -1) {
        if (!Unlimbo(Cell_To_Coord(cellnum))) {
            delete this;
        }
    }
    m_AnimStage.Set_Delay(0);
}

/**
 * @brief
 *
 * @address 0x0056A270
 */
TerrainClass::~TerrainClass()
{
    if (GameActive) {
        Limbo();
    }
}

/**
 * @brief
 *
 * @address 0x0056A3BC
 */
void *TerrainClass::operator new(size_t size)
{
    TerrainClass *this_ptr = g_Terrains.Alloc();
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = true;
    }
    return this_ptr;
}

/**
 * @brief
 *
 * @address 0x0056A3DC
 */
void TerrainClass::operator delete(void *ptr)
{
    TerrainClass *this_ptr = static_cast<TerrainClass *>(ptr);
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = false;
    }
    g_Terrains.Free(this_ptr);
}

/**
 * @brief
 *
 * @address 0x0056AB98
 */
coord_t TerrainClass::Center_Coord() const
{
    DEBUG_ASSERT(m_IsActive);
    return Coord_Add(m_Coord, m_Type->Get_UnkCoord());
}

/**
 * @brief
 *
 * @address 0x0056AC98
 */
coord_t TerrainClass::Target_Coord() const
{
    return Coord_Add(Coord_From_Lepton_XY(0, -m_Height), Sort_Y());
}

/**
 * @brief
 *
 * @address 0x0056A668
 */
MoveType TerrainClass::Can_Enter_Cell(cell_t cellnum, FacingType facing) const
{
    if (cellnum >= MAP_MAX_AREA) {
        return MOVE_NO;
    }
    for (const int16_t *list = Occupy_List(); *list != LIST_END; ++list) {
        if (m_Type->Is_Waterbound()) {
            if (!Map[cellnum + *list].Is_Clear_To_Build(SPEED_FLOAT)) {
                return MOVE_NO;
            }
        } else {
            if (!Map[cellnum + *list].Is_Clear_To_Build(SPEED_TRACK)) {
                return MOVE_NO;
            }
        }
    }
    return MOVE_OK;
}

/**
 * @brief
 *
 * @address 0x0056A91C
 */
void TerrainClass::AI()
{
    ObjectClass::AI();
    if (What_Type() == TERRAIN_MINE && !(g_GameFrame % (900 * Rule.Ore_Growth_Rate()))) {
        Map[As_Cell(As_Target())].Spread_Ore(true);
    }
    if (m_AnimStage.Stage_Changed()) {
        Mark(MARK_REMOVE);
        if (m_Crumbling) {
            if (m_AnimStage.Get_Stage() == Get_Build_Frame_Count(Get_Image_Data()) - 1) {
                delete this;
                Map.Zone_Reset((1 | 2 | 4)); // TODO, implement enum for zone flags.
            }
        }
    }
}

/**
 * @brief
 *
 * @address 0x004CDCA0
 */
coord_t TerrainClass::Sort_Y() const
{
    DEBUG_ASSERT(m_IsActive);
    return Coord_Add(m_Coord, m_Type->Get_UnkCoord());
}

/**
 * @brief
 *
 * @address 0x0056AB58
 */
BOOL TerrainClass::Limbo()
{
    if (!m_InLimbo) {
        Map[Coord_To_Cell(m_Coord)].Clear_Occupant_Bit(OCCUPANT_TERRAIN);
    }
    return ObjectClass::Limbo();
}

/**
 * @brief
 *
 * @address 0x0056AAB4
 */
BOOL TerrainClass::Unlimbo(coord_t coord, DirType dir)
{
    if (Theater_Allowed()) {
        return ObjectClass::Unlimbo(coord, dir);
    }
    return false;
}

/**
 * @brief
 *
 * @address 0x0056A5AC
 */
void TerrainClass::Draw_It(int x, int y, WindowNumberType window) const
{
    void *shape = Get_Image_Data();
    if (shape != nullptr) {
        int frame = 0;
        if (m_Crumbling) {
            frame = m_OnFire + m_AnimStage.Get_Stage();
        } else if (m_Health < 2) {
            frame = 1;
        }
        ShapeFlags flags = SHAPE_NORMAL;
        if (m_Selected && g_InMapEditor) {
            flags |= SHAPE_FADING;
        }
        // edwin has a if case for team flags here
        g_isTheaterShape = true;
        CC_Draw_Shape(shape,
            frame,
            x,
            y,
            window,
            flags | (SHAPE_GHOST | SHAPE_VIEWPORT_REL),
            DisplayClass::FadingLight,
            DisplayClass::UnitShadow);
        g_isTheaterShape = false;
    }
}

/**
 * @brief
 *
 * @address 0x0056A510
 */
BOOL TerrainClass::Mark(MarkType mark)
{
    if (!ObjectClass::Mark(mark)) {
        return false;
    }
    cell_t cellnum = Coord_To_Cell(m_Coord);
    if (mark == MARK_REMOVE) {
        Map.Pick_Up(cellnum, this);
        return true;
    }
    if (mark == MARK_PUT) {
        Map.Place_Down(cellnum, this);
        return true;
    }
    Map.Refresh_Cells(cellnum, Overlap_List(true));
    Map.Refresh_Cells(cellnum, Occupy_List(false));
    return true;
}

/**
 * @brief
 *
 * @address 0x0057AAE8
 */
DamageResultType TerrainClass::Take_Damage(int &damage, int a2, WarheadType warhead, TechnoClass *object, BOOL a5)
{
    if (m_OnFire && warhead != WARHEAD_FIRE) {
        return DAMAGE_UNAFFECTED;
    }
    if (warhead == WARHEAD_SA) {
        return DAMAGE_UNAFFECTED;
    }
    if (m_Type->Is_Immune()) {
        return DAMAGE_UNAFFECTED;
    }
    DamageResultType ret = ObjectClass::Take_Damage(damage, a2, warhead, object, a5);
    if (damage > 0 && warhead == WARHEAD_FIRE) {
        Catch_Fire();
    }
    if (ret == DAMAGE_DEAD) {
        Detach_All(true);
        if (m_OnFire) {
            Shorten_Attached_Anims();
        } else {
            Start_To_Crumble();
        }
    }
    return ret;
}

/**
 * @brief
 *
 * @address 0x0056A734
 */
BOOL TerrainClass::Catch_Fire()
{
    if (m_OnFire || m_Crumbling || m_Type->Get_Armor() != ARMOR_NONE) {
        return false;
    }
    AnimClass *aptr = new AnimClass(ANIM_BURN_BIG, Coord_Add(Sort_Y(), Coord_From_Lepton_XY(0, -80)));
    if (aptr != nullptr) {
        aptr->Attach_To(this);
    }
    aptr = new AnimClass(ANIM_BURN_MED, Coord_Add(Sort_Y(), Coord_From_Lepton_XY(0, -224)), 15);
    if (aptr != nullptr) {
        aptr->Attach_To(this);
    }
    m_OnFire = true;
    return true;
}

/**
 * @brief
 *
 * @address 0x0056A854
 */
void TerrainClass::Fire_Out()
{
    if (m_OnFire) {
        m_OnFire = false;
        if (!m_Crumbling && m_Health == 0) {
            Detach_All();
            Mark(MARK_REMOVE);
            Start_To_Crumble();
            AnimClass *aptr = new AnimClass(ANIM_SMOKE_M, Center_Coord());
            DEBUG_ASSERT(aptr != nullptr);
        }
    }
}

/**
 * @brief
 *
 * @address 0x0056AB08
 */
void TerrainClass::Start_To_Crumble()
{
    if (!m_Crumbling) {
        m_Crumbling = true;
        m_AnimStage.Set_Stage(0);
        m_AnimStage.Set_Delay(2);
    }
}

/**
 * @brief
 *
 * @address 0x0056ABF0
 */
uint8_t *const TerrainClass::Radar_Icon(short cellnum) const
{
    struct IconStruct
    {
        uint8_t X;
        uint8_t Y;
    };
    IconStruct *icon = (IconStruct *)m_Type->Get_Radar_Icon_Data();
    uint8_t *icondata = (uint8_t *)&icon[1];

    cell_t cell = Coord_To_Cell(m_Coord);

    uint8_t x = Cell_Get_X(cell) - Cell_Get_X(cellnum);
    uint8_t y = Cell_Get_Y(cell) - Cell_Get_Y(cellnum);
    // see OverlayTypeClass::Radar_Icon for explaination whats going on
    if (x < icon->X && y < icon->Y) {
        return icondata + 9 * (icon->X + x * y);
    }
    return nullptr;
}
