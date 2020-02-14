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
#include "smudge.h"
#include "cell.h"
#include "gameini.h"
#include "globals.h"
#include "iomap.h"

using std::snprintf;
using std::strtok;
using std::atoi;

#ifndef GAME_DLL
TFixedIHeapClass<SmudgeClass> g_Smudges;
#endif

HousesType SmudgeClass::s_ToOwn = HOUSES_NONE;

/**
 *
 *
 */
SmudgeClass::SmudgeClass(SmudgeType type, coord_t coord, HousesType house) :
    ObjectClass(RTTI_SMUDGE, g_Smudges.ID(this)),
    m_Class(g_SmudgeTypes.Ptr(type))
{
    if (coord != -1) {
        s_ToOwn = house;
        if (Unlimbo(coord)) {
            s_ToOwn = HOUSES_NONE;
        } else {
            delete this;
        }
    }
}

/**
 *
 *
 */
SmudgeClass::SmudgeClass(const SmudgeClass &that) :
    ObjectClass(that),
    m_Class(that.m_Class)
{
}

/**
 *
 *
 */
SmudgeClass::SmudgeClass(const NoInitClass &noinit) :
    ObjectClass(noinit)
{
}

/**
 *
 *
 */
SmudgeClass::~SmudgeClass()
{
    if (g_GameActive) {
        Limbo();
    }
    m_Class = nullptr;
}

/**
 *
 *
 */
void *SmudgeClass::operator new(size_t size)
{
    SmudgeClass *this_ptr = g_Smudges.Alloc();
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = true;
    }
    return this_ptr;
}

/**
 *
 *
 */
void SmudgeClass::operator delete(void *ptr)
{
    SmudgeClass *this_ptr = static_cast<SmudgeClass *>(ptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = false;
    }
    g_Smudges.Free(this_ptr);
}

/**
 *
 *
 */
void SmudgeClass::Draw_It(int x, int y, WindowNumberType window) const
{
    // empty
}

/**
 *
 *
 */
BOOL SmudgeClass::Mark(MarkType mark)
{
    if (!ObjectClass::Mark(mark) || mark != MARK_PUT) {
        return false;
    }
    cell_t cell = Get_Cell();
    const SmudgeTypeClass &type = Class_Of();

    for (int i = 0; i < type.Get_Width(); ++i) {
        for (int j = 0; j < type.Get_Height(); ++j) {
            cell_t calc_cell = (j * CELL_MAX) + i + cell;
            if (g_Map.In_Radar(calc_cell)) {
                CellClass *cptr = &g_Map[calc_cell];
                captainslog_assert(cptr != nullptr);

                if (type.Is_Bib()) {
                    cptr->Set_Smudge(type.What_Type());
                    cptr->Set_Smudge_Frame(type.Get_Width() * j + i);
                    cptr->Set_Owner(s_ToOwn);
                } else if (cptr->Is_Clear_To_Move(SPEED_TRACK, true, true, true)) {
                    if (type.Is_Crater() && cptr->Has_Crater()) {
                        cptr->Set_Smudge_Frame(std::min<uint8_t>(cptr->Get_Smudge_Frame() + 1, 4));
                    }
                    if (cptr->Get_Smudge() == SMUDGE_NONE) {
                        SmudgeType smudge = type.Is_Crater() ? (SmudgeType)cptr->Spot_Index(m_Coord) : type.What_Type();
                        cptr->Set_Smudge(smudge);
                        cptr->Set_Smudge_Frame(0);
                    }
                }
                cptr->Redraw_Objects();
            }
        }
    }
    g_Map.Overlap_Up(Get_Cell(), this);
    m_IsDown = false;
    m_InLimbo = true;
    delete this;
    return true;
}

/**
 *
 *
 */
void SmudgeClass::Disown(cell_t cell)
{
    const SmudgeTypeClass &type = Class_Of();

    if (type.Is_Bib()) {
        for (int i = 0; i < type.Get_Width(); ++i) {
            for (int j = 0; j < type.Get_Height(); ++j) {
                cell_t calc_cell = (j * CELL_MAX) + i + cell;
                CellClass *cptr = &g_Map[calc_cell];
                captainslog_assert(cptr != nullptr);

                if (cptr->Get_Overlay() == OVERLAY_NONE || !cptr->Has_Wall()) {
                    cptr->Set_Smudge(SMUDGE_NONE);
                    cptr->Set_Smudge_Frame(0);
                    if (!cptr->Has_Flag()) {
                        cptr->Set_Owner(HOUSES_NONE);
                    }
                    cptr->Redraw_Objects();
                }
            }
        }
    }
}

/**
 *
 *
 */
void SmudgeClass::Read_INI(GameINIClass &ini)
{
    char valuebuf[128];

    for (int i = 0; i < ini.Entry_Count("SMUDGE"); ++i) {
        ini.Get_String("SMUDGE", ini.Get_Entry("SMUDGE", i), nullptr, valuebuf, sizeof(valuebuf));
        SmudgeType smudge = SmudgeTypeClass::From_Name(strtok(valuebuf, ","));
        if (smudge != SMUDGE_NONE) {
            char *cell_str = strtok(nullptr, ",");
            if (cell_str != nullptr) {
                cell_t cellnum = atoi(cell_str);

                int frame = 0;
                char *frame_str = strtok(nullptr, ",");
                if (frame_str != nullptr) {
                    frame = atoi(frame_str);
                }

                new SmudgeClass(smudge, Cell_To_Coord(cellnum));
                CellClass &cell = g_Map[cellnum];
                if (cell.Get_Smudge() == smudge && frame > 0) {
                    cell.Set_Smudge_Frame(frame);
                }
            }
        }
    }
}

/**
 *
 *
 */
void SmudgeClass::Write_INI(GameINIClass &ini)
{
    char num[32];
    char entry[128];

    ini.Clear("SMUDGE");
    for (int i = 0; i < MAP_MAX_AREA; ++i) {
        CellClass *cptr = &g_Map[i];
        captainslog_assert(cptr != nullptr);

        if (cptr->Get_Smudge() != SMUDGE_NONE) {
            SmudgeTypeClass *stptr = SmudgeTypeClass::As_Pointer(cptr->Get_Smudge());
            if (stptr != nullptr && !stptr->Is_Bib()) {
                snprintf(num, sizeof(num), "%d", i);
                snprintf(entry, sizeof(entry), "%s,%d,%d", stptr->Get_Name(), i, cptr->Get_Smudge_Frame());
                ini.Put_String("SMUDGE", num, entry);
            }
        }
    }
}

/**
 *
 *
 */
void SmudgeClass::Init()
{
    g_Smudges.Free_All();
}
