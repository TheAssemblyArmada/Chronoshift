/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base class for most in game type objects.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef ABSTRACTTYPE_H
#define ABSTRACTTYPE_H

#include "always.h"
#include "rtti.h"
#include "gametypes.h"
#include "language.h"
#include "stringex.h"

class NoInitClass;

class AbstractTypeClass
{
public:
    AbstractTypeClass(RTTIType type, int heap_id = -1, int ui_name = TXT_NULL, const char *name = nullptr);
    AbstractTypeClass(const AbstractTypeClass &that);
    AbstractTypeClass(const NoInitClass &noinit) {}
    ~AbstractTypeClass() {}

    AbstractTypeClass &operator=(const AbstractTypeClass &that);

    virtual coord_t Coord_Fixup(coord_t coord) const { return coord; }
    virtual int Full_Name() const;
    virtual int Get_Ownable() const { return OWNER_ALL; }

    BOOL Is_TechnoType() const;
    BOOL Is_FootType() const;
    int Get_Heap_ID() const { return m_HeapID; }
    RTTIType const What_Am_I() const { return m_RTTI; }
    const char *Get_Name() const { return m_Name; }

protected:
    RTTIType m_RTTI;
    int m_HeapID;
    char m_Name[24];
    int m_UIName;

#ifdef GAME_DLL
public:
    static void Hook_Me();

private:
    AbstractTypeClass *Hook_Ctor(RTTIType type, int heap_id, int ui_name, const char *name) { return new (this) AbstractTypeClass(type, heap_id, ui_name, name); }
    AbstractTypeClass *Hook_Ctor_NoInit(const NoInitClass &noinit) { return new (this) AbstractTypeClass(noinit); }
    AbstractTypeClass *Hook_Ctor_Copy(const AbstractTypeClass &that) { return new (this) AbstractTypeClass(that); }
    coord_t Hook_Coord_Fixup(coord_t coord) { return AbstractTypeClass::Coord_Fixup(coord); }
    int Hook_Full_Name() { return AbstractTypeClass::Full_Name(); }
    int Hook_Get_Ownable() { return AbstractTypeClass::Get_Ownable(); }
#endif
};

#ifdef GAME_DLL
#include "hooker.h"

inline void AbstractTypeClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x004055DC, *AbstractTypeClass::Hook_Ctor);
    Hook_Function(0x004CD990, *AbstractTypeClass::Hook_Ctor_NoInit);
    Hook_Function(0x00405480, *AbstractTypeClass::Hook_Ctor_Copy);
    Hook_Function(0x00405610, *AbstractTypeClass::Hook_Coord_Fixup);
    Hook_Function(0x0040561C, *AbstractTypeClass::Hook_Full_Name);
    Hook_Function(0x0040566C, *AbstractTypeClass::Hook_Get_Ownable);
#endif
}
#endif

inline AbstractTypeClass &AbstractTypeClass::operator=(const AbstractTypeClass &that)
{
    if (this != &that) {
        m_RTTI = that.m_RTTI;
        m_HeapID = that.m_HeapID;
        m_UIName = that.m_UIName;
        strlcpy(m_Name, that.m_Name, sizeof(m_Name));
    }

    return *this;
}

inline BOOL AbstractTypeClass::Is_TechnoType() const
{
    return m_RTTI == RTTI_BUILDINGTYPE || m_RTTI == RTTI_UNITTYPE || m_RTTI == RTTI_INFANTRYTYPE || m_RTTI == RTTI_VESSELTYPE
        || m_RTTI == RTTI_AIRCRAFTTYPE;
}

inline BOOL AbstractTypeClass::Is_FootType() const
{
    return m_RTTI == RTTI_UNITTYPE || m_RTTI == RTTI_INFANTRYTYPE || m_RTTI == RTTI_VESSELTYPE || m_RTTI == RTTI_AIRCRAFTTYPE;
}

#endif // ABSTRACTTYPE_H
