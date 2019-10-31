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
#include "objecttype.h"
#include "gameini.h"
#include "gamefile.h"
#include "mixfile.h"

#ifndef GAME_DLL
void *ObjectTypeClass::SelectShapes = nullptr;
void *ObjectTypeClass::PipShapes = nullptr;
#endif

ObjectTypeClass::ObjectTypeClass(RTTIType type, int id, BOOL unk1, BOOL radar_invisible, BOOL selectable,
    BOOL legal_target, BOOL insignificant, BOOL is_immune, BOOL unk2, int ui_name, const char *name) :
    AbstractTypeClass(type, id, ui_name, name),
    m_Crushable(false),
    m_RadarInvisible(radar_invisible),
    m_Selectable(selectable),
    m_LegalTarget(legal_target),
    m_Insignificant(insignificant),
    m_Immune(is_immune),
    m_Bit64(unk1),
    m_Bit128(unk2),
    m_Armor(ARMOR_NONE),
    m_Strength(0),
    m_ImageData(nullptr),
    m_FrameDimensions(nullptr),
    m_RadarIconData(nullptr)
{
}

ObjectTypeClass::ObjectTypeClass(const ObjectTypeClass &that) :
    AbstractTypeClass(that),
    m_Crushable(that.m_Crushable),
    m_RadarInvisible(that.m_RadarInvisible),
    m_Selectable(that.m_Selectable),
    m_LegalTarget(that.m_LegalTarget),
    m_Insignificant(that.m_Insignificant),
    m_Immune(that.m_Immune),
    m_Bit64(that.m_Bit64),
    m_Bit128(that.m_Bit128),
    m_Armor(that.m_Armor),
    m_Strength(that.m_Strength),
    m_ImageData(that.m_ImageData),
    m_FrameDimensions(that.m_FrameDimensions),
    m_RadarIconData(that.m_RadarIconData)
{
    memcpy(m_ImageName, that.m_ImageName, sizeof(m_ImageName));
}

ObjectTypeClass::~ObjectTypeClass()
{
    m_ImageData = nullptr;
    m_RadarIconData = nullptr;
    
    if (m_FrameDimensions != nullptr) {
        delete[] m_FrameDimensions;
    }
}

/**
 * @brief Gets the dimensions of the object.
 */
void ObjectTypeClass::Dimensions(int &w, int &h) const
{
    w = 10;
    h = 10;
}

/**
 * @brief Gets an occupy list for determining cell occupation.
 */
const int16_t *ObjectTypeClass::Occupy_List(BOOL a1) const
{
    static const int16_t _list[] = { 0, INT16_MAX };

    return _list;
}

/**
 * @brief Gets an overlap list for overlapped blitting and refresh calcs.
 */
const int16_t *ObjectTypeClass::Overlap_List() const
{
    static const int16_t _list[] = { 0, INT16_MAX };

    return _list;
}

/**
 * @brief Gets a pointer to a BuildingClass that builds this object.
 */
BuildingClass *ObjectTypeClass::Who_Can_Build_Me(BOOL a1, BOOL a2, HousesType house) const
{
    // TODO Requires BuildingClass, BuildingTypeClass, HouseClass
#ifdef GAME_DLL
    BuildingClass *(*func)(const ObjectTypeClass *, BOOL, BOOL, HousesType) =
        reinterpret_cast<BuildingClass *(*)(const ObjectTypeClass *, BOOL, BOOL, HousesType)>(0x0051E988);
    return func(this, a1, a2, house);
#else
    return nullptr;
#endif
}

 BOOL ObjectTypeClass::Read_INI(GameINIClass &ini)
 {
     if (ini.Find_Section(m_Name)) {
         ini.Get_String(m_Name, "Image", "none", m_ImageName, sizeof(m_ImageName));

         return true;
     }

     return false;
 }

/**
 * @brief Initialises the select and pip sprite pointers.
 */
void ObjectTypeClass::One_Time()
{
    SelectShapes = GameFileClass::Retrieve("select.shp");
    PipShapes = GameFileClass::Retrieve("pips.shp");
}

void *ObjectTypeClass::Get_Radar_Icon(void *shape, int frame, int frame_count, int size)
{
#ifdef GAME_DLL
    void *(*func)(void *, int, int, int) = reinterpret_cast<void *(*)(void *, int, int, int)>(0x004A9494);
    return func(shape, frame, frame_count, size);
#else
    // TODO
    return nullptr;
#endif
}
