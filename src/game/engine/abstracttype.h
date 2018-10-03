/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base class for most in game objects.
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
#include "language.h"
#include "noinit.h"
#include "rtti.h"
#include "gametypes.h"
#include "stringex.h"

class AbstractTypeClass
{
public:
    AbstractTypeClass(RTTIType type, int id = -1, int uiname = TXT_NULL, const char *name = nullptr);
    AbstractTypeClass(AbstractTypeClass const &that);
    AbstractTypeClass(NoInitClass const &noinit) {}
    ~AbstractTypeClass() {}

    virtual uint32_t Coord_Fixup(uint32_t coord) const { return coord; }
    virtual int Full_Name() const;
    virtual int Get_Ownable() const { return OWNER_ALL; }

    AbstractTypeClass &operator=(AbstractTypeClass &that);

    BOOL Is_TechnoType() const;
    BOOL Is_FootType() const;
    int Get_Heap_ID() const { return HeapID; }
    RTTIType const What_Am_I() const { return RTTI; }
    const char *Get_Name() const { return Name; }

protected:
    RTTIType RTTI;
    int HeapID;
    char Name[24];
    int UIName;
};

inline AbstractTypeClass &AbstractTypeClass::operator=(AbstractTypeClass &that)
{
    if (this != &that) {
        RTTI = that.RTTI;
        HeapID = that.HeapID;
        UIName = that.UIName;
        strlcpy(Name, that.Name, sizeof(Name));
    }

    return *this;
}

inline BOOL AbstractTypeClass::Is_TechnoType() const
{
    return RTTI == RTTI_BUILDINGTYPE || RTTI == RTTI_UNITTYPE || RTTI == RTTI_INFANTRYTYPE || RTTI == RTTI_VESSELTYPE
        || RTTI == RTTI_AIRCRAFTTYPE;
}

inline BOOL AbstractTypeClass::Is_FootType() const
{
    return RTTI == RTTI_UNITTYPE || RTTI == RTTI_INFANTRYTYPE || RTTI == RTTI_VESSELTYPE || RTTI == RTTI_AIRCRAFTTYPE;
}

#endif // ABSTRACTTYPE_H
