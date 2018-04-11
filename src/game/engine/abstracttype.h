/**
* @file
*
* @author CCHyper
* @author OmniBlade
*
* @brief Base class for most in game objects.
*
* @copyright RedAlert++ is free software: you can redistribute it and/or
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
    virtual int Get_Ownable() const { return 0xFFFFF; } // This is a bit for each of the normal houses, pluse 8 multiplayer.

    AbstractTypeClass &operator=(AbstractTypeClass &that);

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


#endif // ABSTRACTTYPE_H
