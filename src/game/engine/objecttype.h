/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base class for most ingame object types.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
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
#include "abstracttype.h"
#include "armor.h"
#include "gametypes.h"
#include "shape.h"
#include "trect.h"

class ObjectClass;
class BuildingClass;
class HouseClass;
class GameINIClass;

class ObjectTypeClass : public AbstractTypeClass
{
public:
    ObjectTypeClass(RTTIType type, int id, BOOL animates, BOOL radar_invisible, BOOL selectable, BOOL legal_target,
        BOOL insignificant, BOOL is_immune, BOOL logical, int ui_name, const char *name);
    ObjectTypeClass(const ObjectTypeClass &that);
    ObjectTypeClass(const NoInitClass &noinit) : AbstractTypeClass(noinit) {}
    ~ObjectTypeClass();

    virtual int Max_Pips() const { return 0; }
    virtual void Dimensions(int &w, int &h) const;
    virtual BOOL Create_And_Place(cell_t cellnum, HousesType house = HOUSES_NONE) const = 0;
    virtual int Cost_Of() const { return 0; }
    virtual int Time_To_Build() const { return 0; }
    virtual ObjectClass *Create_One_Of(HouseClass *house) const = 0;
    virtual const int16_t *Occupy_List(BOOL a1 = false) const;
    virtual const int16_t *Overlap_List() const;
    virtual BuildingClass *Who_Can_Build_Me(BOOL a1 = false, BOOL a2 = false, HousesType house = HOUSES_NONE) const;
    virtual void *Get_Cameo_Data() const { return nullptr; };

    BOOL Read_INI(GameINIClass &ini);

    BOOL Is_Crushable() const { return m_Crushable; }
    BOOL Is_Selectable() const { return m_Selectable; }
    BOOL Is_Immune() const { return m_Immune; }
    BOOL Get_Bit64() const { return m_Bit64; }
    BOOL Get_Bit128() const { return m_Bit128; }
    int16_t Get_Strength() const { return m_Strength; }
    ArmorType Get_Armor() const { return m_Armor; }
    void *Get_Image_Data() const { return m_ImageData; }
    void *Get_Radar_Icon_Data() const { return m_RadarIconData; }
    const char *Get_Image_Name() const { return m_ImageName[0] != '\0' ? m_ImageName : m_Name; }

    void Init_Frame_Dimensions(int frames) const;
    TRect<int> *Get_Frame_Dimensions() { return m_FrameDimensions; }
    void Set_Frame_Dimensions(void *shape, int frame) const;

    static void One_Time();

protected:
    static void *Get_Radar_Icon(void *shape, int frame, int frame_count, int size);

protected:
    char m_ImageName[256];
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_Crushable : 1; // & 1
    BOOL m_RadarInvisible : 1; // & 2
    BOOL m_Selectable : 1; // & 4
    BOOL m_LegalTarget : 1; // & 8
    BOOL m_Insignificant : 1; // & 16
    BOOL m_Immune : 1; // & 32
    BOOL m_Bit64 : 1; // & 64
    BOOL m_Bit128 : 1; // & 128
#else
    bool m_Crushable; // Can it be crushed by a heavy tracked vehicle?
    bool m_RadarInvisible; // Is it invisible on radar maps?
    bool m_Selectable; // Can this object be selected by the player?
    bool m_LegalTarget; // Is this allowed to be a combat target?
    bool m_Insignificant; // Will this object not be announced when destroyed?
    bool m_Immune; // Is this object immune to damage?
    bool m_Bit64; // TODO: Logical or HasLogic? If true, object gets added to and removed from the logic layer.
    bool m_Bit128; // TODO: If true, object can be placed down and picked up from the tactical map.
#endif

    ArmorType m_Armor; // The armor type of this object, see ArmorType enum.
    int16_t m_Strength;
    void *m_ImageData;
    mutable TRect<int> *m_FrameDimensions; // Mutable as only lazy caches information, not changeable state.
    void *m_RadarIconData;

#ifdef GAME_DLL
    static void *&SelectShapes;
    static void *&PipShapes;
#else
    static void *SelectShapes;
    static void *PipShapes;
#endif
};

inline void ObjectTypeClass::Init_Frame_Dimensions(int frames) const
{
    if (m_FrameDimensions != nullptr) {
        m_FrameDimensions = new TRect<int>[frames];
    }
}

inline void ObjectTypeClass::Set_Frame_Dimensions(void *shape, int frame) const
{
    if (m_FrameDimensions != nullptr && !m_FrameDimensions[frame].Is_Valid()) {
        m_FrameDimensions[frame] = Shape_Dimensions(shape, frame);
    }
}

#endif // OBJECTTYPE_H
