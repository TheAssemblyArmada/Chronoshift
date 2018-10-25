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
#include "ccfileclass.h"
#include "gameini.h"
#include "mixfile.h"

#ifndef CHRONOSHIFT_STANDALONE
void *&ObjectTypeClass::SelectShapes = Make_Global<void *>(0x006857A8);
void *&ObjectTypeClass::PipShapes = Make_Global<void *>(0x006857AC);
#else
void *ObjectTypeClass::SelectShapes = nullptr;
void *ObjectTypeClass::PipShapes = nullptr;
#endif

ObjectTypeClass::ObjectTypeClass(RTTIType type, int id, BOOL unk1, BOOL radar_invisible, BOOL selectable,
    BOOL legal_target, BOOL insignificant, BOOL is_immune, BOOL unk2, int uiname, const char *name) :
    AbstractTypeClass(type, id, uiname, name),
    Crushable(false),
    RadarInvisible(radar_invisible),
    Selectable(selectable),
    LegalTarget(legal_target),
    Insignificant(insignificant),
    Immune(is_immune),
    Bit64(unk1),
    Bit128(unk2),
    Armor(ARMOR_NONE),
    ImageData(nullptr),
    FrameDimensions(nullptr),
    RadarIconData(nullptr)
{
}

ObjectTypeClass::ObjectTypeClass(ObjectTypeClass const &that) :
    AbstractTypeClass(that),
    Crushable(that.Crushable),
    RadarInvisible(that.RadarInvisible),
    Selectable(that.Selectable),
    LegalTarget(that.LegalTarget),
    Insignificant(that.Insignificant),
    Immune(that.Immune),
    Bit64(that.Bit64),
    Bit128(that.Bit128),
    Armor(that.Armor),
    Strength(that.Strength),
    ImageData(that.ImageData),
    FrameDimensions(that.FrameDimensions),
    RadarIconData(that.RadarIconData)
{
    memcpy(ImageName, that.ImageName, sizeof(ImageName));
}

ObjectTypeClass::~ObjectTypeClass()
{
    ImageData = nullptr;
    RadarIconData = nullptr;
    
    if (FrameDimensions != nullptr) {
        delete[] FrameDimensions;
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
#ifndef CHRONOSHIFT_STANDALONE
    BuildingClass *(*func)(const ObjectTypeClass *, BOOL, BOOL, HousesType) =
        reinterpret_cast<BuildingClass *(*)(const ObjectTypeClass *, BOOL, BOOL, HousesType)>(0x0051E988);
    return func(this, a1, a2, house);
#else
    return nullptr;
#endif
}

 BOOL ObjectTypeClass::Read_INI(GameINIClass &ini)
 {
     if (ini.Find_Section(Get_Name())) {
         ini.Get_String(Get_Name(), "Image", "none", ImageName, sizeof(ImageName));
     }

     return false;
 }

/**
 * @brief Initialises the select and pip sprite pointers.
 */
void ObjectTypeClass::One_Time()
{
    SelectShapes = MixFileClass<CCFileClass>::Retrieve("select.shp");
    PipShapes = MixFileClass<CCFileClass>::Retrieve("pips.shp");
}

void *ObjectTypeClass::Get_Radar_Icon(void *shape, int frame, int frame_count, int size)
{
#ifndef CHRONOSHIFT_STANDALONE
    void *(*func)(void *, int, int, int) = reinterpret_cast<void *(*)(void *, int, int, int)>(0x004A9494);
    return func(shape, frame, frame_count, size);
#else
    // TODO
    return nullptr;
#endif
}
