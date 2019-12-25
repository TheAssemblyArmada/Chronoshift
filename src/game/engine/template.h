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

#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "always.h"
#include "object.h"
#include "templatetype.h"

class TemplateClass : public ObjectClass
{
public:
    TemplateClass(TemplateType type, cell_t cellnum = -1);
    TemplateClass(const TemplateClass &that);
    TemplateClass(const NoInitClass &noinit) : ObjectClass(noinit) {}
    virtual ~TemplateClass();

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    virtual const TemplateTypeClass &Class_Of() const override { return *m_Class; }
    virtual void Draw_It(int x, int y, WindowNumberType window) const override {}
    virtual BOOL Mark(MarkType mark) override;

    TemplateType What_Type() const { return m_Class->Get_Type(); }

private:
    GamePtr<TemplateTypeClass> m_Class;
};

#ifdef GAME_DLL
extern TFixedIHeapClass<TemplateClass> &g_Templates;
#else
extern TFixedIHeapClass<TemplateClass> g_Templates;
#endif

#endif // TEMPLATE_H
