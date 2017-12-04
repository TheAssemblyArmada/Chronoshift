/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base class for most ingame object types.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef OBJECTTYPE_H
#define OBJECTTYPE_H

#include "always.h"
#include "abstract.h"
#include "armor.h"

class ObjectClass;
class BuildingClass;
class HouseClass;

class ObjectTypeClass : public AbstractTypeClass
{
public:
    ObjectTypeClass(RTTIType type, int id, BOOL animates, BOOL radar_invisible, BOOL selectable, BOOL legal_target,
        BOOL insignificant, BOOL is_immune, BOOL logical, int uiname, char const *name);
    ObjectTypeClass(ObjectTypeClass const &that);
    ObjectTypeClass(NoInitClass const &noinit) : AbstractTypeClass(noinit) {}
    ~ObjectTypeClass();

    virtual int Max_Pips() const { return 0; }
    virtual void Dimensions(int &w, int &h) const;
    virtual BOOL Create_And_Place(int16_t cellnum, HousesType house = HOUSES_NONE) const = 0;
    virtual int Cost_Of() const { return 0; }
    virtual int Time_To_Build() const { return 0; }
    virtual ObjectClass *Create_One_Of(HouseClass *house) const = 0;
    virtual int16_t *Occupy_List(BOOL a1 = false) const;
    virtual int16_t *Overlap_List() const;
    virtual BuildingClass *Who_Can_Build_Me(BOOL a1 = false, BOOL a2 = false, HousesType house = HOUSES_NONE) const;
    virtual void *Get_Cameo_Data() const { return nullptr; };

    BOOL Is_Selectable() { return Selectable; }
    BOOL Is_Immune() { return Immune; }
    BOOL Get_Bit64() { return Bit64; }
    BOOL Get_Bit128() { return Bit128; }
    int16_t Get_Strength() { return Strength; }
    void *Get_Image_Data() { return ImageData; }

    static void One_Time();

protected:
    char ImageName[256];
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool Crushable : 1; // & 1 Can it be crushed by a heavy tracked vehicle (def = false)?
            bool RadarInvisible : 1; // & 2 Is it invisible on radar maps (def = false)? unused in RA?
            bool Selectable : 1; // & 4 Can this object be selected by the player (def = true)?
            bool LegalTarget : 1; // & 8 Is this allowed to be a combat target (def = true)?
            bool Insignificant : 1; // & 16 Will this object not be announced when destroyed (def = false)?
            bool Immune : 1; // & 32 Is this object immune to damage (def = false)?
            bool Bit64 : 1; // & 64 Related to logical or animate?
            bool Bit128 : 1; // & 128 Related to logical or animate?
        };
        int Bitfield;
    };
#else
    bool Crushable; // Can it be crushed by a heavy tracked vehicle (def = false)?
    bool RadarInvisible; // Is it invisible on radar maps (def = false)? unused in RA?
    bool Selectable; // Can this object be selected by the player (def = true)?
    bool LegalTarget; // Is this allowed to be a combat target (def = true)?
    bool Insignificant; // Will this object not be announced when destroyed (def = false)?
    bool Immune; // Is this object immune to damage (def = false)?
    bool Bit64; // Related to logical or animate?
    bool Bit128; // Related to logical or animate?
#endif

    ArmorType Armor; // The armor type of this object [NONE, WOOD, LIGHT, HEAVY, CONCRETE].
    int16_t Strength;
    void *ImageData;
    int FrameDimensions;
    void *RadarIconData;

#ifndef RAPP_STANDALONE
#include "hooker.h"
    static void *&SelectShapes;
    static void *&PipShapes;
#else
    static void *SelectShapes;
    static void *PipShapes;
#endif
};

#endif // OBJECTTYPE_H
