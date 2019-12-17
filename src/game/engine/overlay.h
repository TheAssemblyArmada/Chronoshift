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

#ifndef OVERLAY_H
#define OVERLAY_H

#include "always.h"
#include "object.h"
#include "overlaytype.h"

class OverlayClass : public ObjectClass
{
public:
    OverlayClass(OverlayType type, cell_t cell = -1, HousesType house = HOUSES_NONE);
    OverlayClass(const OverlayClass &that);
    OverlayClass(const NoInitClass &noinit);
    virtual ~OverlayClass();

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    // ObjectClass
    virtual const OverlayTypeClass &Class_Of() const final { return *m_Class; }
    virtual void Draw_It(int x, int y, WindowNumberType window) const final;
    virtual BOOL Mark(MarkType mark) final;

    static void Read_INI(GameINIClass &ini);
    static void Write_INI(GameINIClass &ini);

    OverlayType What_Type() const { return m_Class->What_Type(); }

    static void Init();

private:
    GamePtr<OverlayTypeClass> m_Class;

    static HousesType s_ToOwn;
};

#ifdef GAME_DLL
extern TFixedIHeapClass<OverlayClass> &g_Overlays;
#else
extern TFixedIHeapClass<OverlayClass> g_Overlays;
#endif

#endif //OVERLAY_H
