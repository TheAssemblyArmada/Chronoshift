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

#include "template.h"
#include "cell.h"
#include "globals.h"
#include "iomap.h"
#include "tileset.h"

#ifndef GAME_DLL
TFixedIHeapClass<TemplateClass> g_Templates;
#endif

/**
 * @brief
 *
 * 0x0056A1D8
 */
TemplateClass::TemplateClass(TemplateType type, cell_t cellnum) :
    ObjectClass(RTTI_TEMPLATE, g_Templates.ID(this)),
    m_Class(g_TemplateTypes.Ptr(type))
{
    if (cellnum != -1) {
        Unlimbo(Cell_To_Coord(cellnum));
    }
}

/**
 * @brief
 *
 * 0x0056A198
 */
void *TemplateClass::operator new(size_t size)
{
    TemplateClass *this_ptr = g_Templates.Alloc();
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = true;
    }
    return this_ptr;
}

/**
 * @brief
 *
 * 0x0056A1B8
 */
void TemplateClass::operator delete(void *ptr)
{
    TemplateClass *this_ptr = static_cast<TemplateClass *>(ptr);
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = false;
    }
    g_Templates.Free(this_ptr);
}

/**
 * @brief
 *
 * 0x004CDB90
 */
TemplateClass::~TemplateClass()
{
    if (g_GameActive) {
        Limbo();
    }
}

/**
 * @brief
 *
 * 0x00569F24
 */
BOOL TemplateClass::Mark(MarkType mark)
{
    DEBUG_ASSERT(m_IsActive);
    static bool _noup = false;

    void *image = Class_Of().Get_Image_Data();
    if (image == nullptr || !ObjectClass::Mark(mark)) {
        return false;
    }
    uint8_t *set = Get_Icon_Set_Map(image);
    for (int h = 0; m_Class->Get_Height() > h; ++h) {
        for (int w = 0; m_Class->Get_Width() > w; ++w) {
            cell_t cell = Cell_From_XY(w, h) + Coord_To_Cell(m_Coord);
            if (g_Map.In_Radar(cell)) {
                CellClass *cptr = &g_Map[cell];
                int icon = h * m_Class->Get_Width() + w;
                if (set[icon] != -1) {
                    if (mark == MARK_REMOVE && !_noup) {
                        if (cptr->Get_Template() == What_Type() && cptr->Get_Icon() == icon) {
                            cptr->Set_Template(TEMPLATE_NONE);
                            cptr->Set_Icon(0);
                        }
                    }
                    if (mark == MARK_PUT) {
                        if (What_Type()) {
                            cptr->Set_Template(What_Type());
                            cptr->Set_Icon(icon);
                        } else {
                            cptr->Set_Template(TEMPLATE_NONE);
                            cptr->Set_Icon(0);
                        }
                        cptr->Set_Smudge(SMUDGE_NONE);
                        cptr->Set_Smudge_Frame(0);
                        cptr->Set_Overlay(OVERLAY_NONE);
                        cptr->Set_Overlay_Frame(0);
                    }
                    cptr->Redraw_Objects();
                    cptr->Recalc_Attributes();
                }
            }
        }
    }
    if (mark == MARK_PUT) {
        _noup = true;
        delete this;
        _noup = false;
    }
    return true;
}
