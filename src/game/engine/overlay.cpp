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
#include "overlay.h"
#include "cell.h"
#include "iomap.h"
#include "globals.h"

#ifndef GAME_DLL
TFixedIHeapClass<OverlayClass> g_Overlays;
#endif

HousesType OverlayClass::s_ToOwn = HOUSES_NONE;

OverlayClass::OverlayClass(OverlayType type, cell_t cell, HousesType house) :
    ObjectClass(RTTI_OVERLAY, g_Overlays.ID(this)),
    m_Class(g_OverlayTypes.Ptr(type))
{
    if (cell != -1) {
        s_ToOwn = house;
        Unlimbo(Cell_To_Coord(cell));
        s_ToOwn = HOUSES_NONE;
    }
}

OverlayClass::OverlayClass(const OverlayClass &that) :
    ObjectClass(that),
    m_Class(that.m_Class)
{
}

OverlayClass::OverlayClass(const NoInitClass &noinit) :
    ObjectClass(noinit)
{
}

OverlayClass::~OverlayClass()
{
    if (g_GameActive) {
        OverlayClass::Limbo();
    }
    m_Class = nullptr;
}

void *OverlayClass::operator new(size_t size)
{
    OverlayClass *this_ptr = g_Overlays.Alloc();
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = true;
    }
    return this_ptr;
}

void OverlayClass::operator delete(void *ptr)
{
    OverlayClass *this_ptr = static_cast<OverlayClass *>(ptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = false;
    }
    g_Overlays.Free(this_ptr);
}

/**
 *
 *
 */
void OverlayClass::Draw_It(int x, int y, WindowNumberType window) const
{
    // empty
}

/**
 *
 *
 */
BOOL OverlayClass::Mark(MarkType mark)
{
    if (!ObjectClass::Mark(mark) || mark != MARK_PUT) {
        return false;
    }

    CellClass *cell = &g_Map[Get_Cell()];
    const OverlayTypeClass *type = &Class_Of();

    if (type->Is_Wall()) {
        if (!cell->Is_Clear_To_Build(SPEED_TRACK)) {
            delete this;
            return false;
        }

        cell->Set_Overlay(type->What_Type());
        cell->Set_Overlay_Frame(0);
        cell->Redraw_Objects();
        cell->Wall_Update();

        g_Map.Zone_Reset(type->Is_Crushable() ? MZONE_CRUSHER : MZONE_NORMAL);

        if (s_ToOwn != HOUSES_NONE) {
            cell->Set_Owner(s_ToOwn);
        }
    } else {
        bool cell_clear = false;
        if (g_ScenarioInit == 0) {
            if (type->What_Type() == OVERLAY_WATER_WOOD_CRATE) {
                cell_clear = cell->Is_Clear_To_Move(SPEED_FLOAT);
            } else {
                if (type->What_Type() == OVERLAY_STEEL_CRATE || type->What_Type() == OVERLAY_WOOD_CRATE) {
                    cell_clear = cell->Is_Clear_To_Move(SPEED_TRACK);
                } else {
                    cell_clear = cell->Is_Clear_To_Move(SPEED_TRACK, true, true);
                }
            }
        } else {
            cell_clear = true;
        }

        if ((g_ScenarioInit || cell->Get_Overlay() == OVERLAY_NONE) && cell_clear) {
            cell->Set_Overlay(type->What_Type());
            cell->Set_Overlay_Frame(0);
            cell->Redraw_Objects();
            if (type->Get_Land() == LAND_ORE) {
                cell->Set_Overlay_Frame(1);
                cell->Ore_Adjust(false);
            }
        }
    }

    cell->Recalc_Attributes();
    g_Map.Overlap_Up(Get_Cell(), this);
    m_IsDown = false;
    m_InLimbo = true;
    delete this;
    return true;
}

void OverlayClass::Read_INI(GameINIClass &ini)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00527014, void, GameINIClass &);
    func(ini);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
#endif
}

void OverlayClass::Write_INI(GameINIClass &ini)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0052736C, void, GameINIClass &);
    func(ini);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
#endif
}

/**
 *
 *
 */
void OverlayClass::Init()
{
    g_Overlays.Free_All();
}
