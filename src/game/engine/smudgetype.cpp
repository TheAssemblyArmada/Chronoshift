/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class containing information about terrain smudges.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "smudgetype.h"
#include "gamefile.h"
#include "drawshape.h"
#include "lists.h"
#include "mixfile.h"
#include <cstdio>

using std::snprintf;

#ifndef CHRONOSHIFT_STANDALONE
TFixedIHeapClass<SmudgeTypeClass> &SmudgeTypes = Make_Global<TFixedIHeapClass<SmudgeTypeClass> >(0x0065E068);
#else
TFixedIHeapClass<SmudgeTypeClass> SmudgeTypes;
#endif

const SmudgeTypeClass SmudgeCrater1(SMUDGE_CR1, "CR1", TXT_CRATER, 1, 1, false, true);
const SmudgeTypeClass SmudgeCrater2(SMUDGE_CR2, "CR2", TXT_CRATER, 1, 1, false, true);
const SmudgeTypeClass SmudgeCrater3(SMUDGE_CR3, "CR3", TXT_CRATER, 1, 1, false, true);
const SmudgeTypeClass SmudgeCrater4(SMUDGE_CR4, "CR4", TXT_CRATER, 1, 1, false, true);
const SmudgeTypeClass SmudgeCrater5(SMUDGE_CR5, "CR5", TXT_CRATER, 1, 1, false, true);
const SmudgeTypeClass SmudgeCrater6(SMUDGE_CR6, "CR6", TXT_CRATER, 1, 1, false, true);
const SmudgeTypeClass SmudgeScorch1(SMUDGE_SC1, "SC1", TXT_SCORCH, 1, 1, false, false);
const SmudgeTypeClass SmudgeScorch2(SMUDGE_SC2, "SC2", TXT_SCORCH, 1, 1, false, false);
const SmudgeTypeClass SmudgeScorch3(SMUDGE_SC3, "SC3", TXT_SCORCH, 1, 1, false, false);
const SmudgeTypeClass SmudgeScorch4(SMUDGE_SC4, "SC4", TXT_SCORCH, 1, 1, false, false);
const SmudgeTypeClass SmudgeScorch5(SMUDGE_SC5, "SC5", TXT_SCORCH, 1, 1, false, false);
const SmudgeTypeClass SmudgeScorch6(SMUDGE_SC6, "SC6", TXT_SCORCH, 1, 1, false, false);
const SmudgeTypeClass SmudgeBibx1(SMUDGE_BIB1, "BIB1", TXT_BIB, 4, 2, true, false);
const SmudgeTypeClass SmudgeBibx2(SMUDGE_BIB2, "BIB2", TXT_BIB, 3, 2, true, false);
const SmudgeTypeClass SmudgeBibx3(SMUDGE_BIB3, "BIB3", TXT_BIB, 2, 2, true, false);

SmudgeTypeClass::SmudgeTypeClass(
    SmudgeType smudge, const char *name, int uiname, int width, int height, BOOL bib, BOOL crater) :
    ObjectTypeClass(RTTI_SMUDGETYPE, smudge, true, true, false, false, true, true, false, uiname, name),
    Type(smudge),
    Width(width),
    Height(height),
    Crater(crater),
    Bib(bib)
{
}

SmudgeTypeClass::SmudgeTypeClass(SmudgeTypeClass const &that) :
    ObjectTypeClass(that),
    Type(that.Type),
    Width(that.Width),
    Height(that.Height),
    Bib(that.Bib),
    Crater(that.Crater)
{
}

void *SmudgeTypeClass::operator new(size_t size)
{
    DEBUG_ASSERT(size == sizeof(SmudgeTypeClass));
    return SmudgeTypes.Allocate();
}

void SmudgeTypeClass::operator delete(void *ptr)
{
    DEBUG_ASSERT(ptr != nullptr);
    SmudgeTypes.Free(ptr);
}

BOOL SmudgeTypeClass::Create_And_Place(int16_t cellnum, HousesType house) const
{
    // TODO requires SmudgeClass
#ifndef CHRONOSHIFT_STANDALONE
    BOOL(*func)(const SmudgeTypeClass*, int16_t, HousesType) = reinterpret_cast<BOOL(*)(const SmudgeTypeClass*, int16_t, HousesType)>(0x00549E50);
    return func(this, cellnum, house);
#elif 0
    DEBUG_ASSERT(this != nullptr);

    return new SmudgeClass(Type, Cell_To_Coord(cellnum), house) != nullptr;
#else
    return false;
#endif
}

ObjectClass *SmudgeTypeClass::Create_One_Of(HouseClass *house) const
{
    // TODO requires OverlayClass
#ifndef CHRONOSHIFT_STANDALONE
    ObjectClass *(*func)(const SmudgeTypeClass*, HouseClass*) = reinterpret_cast<ObjectClass *(*)(const SmudgeTypeClass*, HouseClass*)>(0x00549EB0);
    return func(this, house);
#elif 0
    DEBUG_ASSERT(this != nullptr);
    // DEBUG_ASSERT(house != nullptr);

    SmudgeClass *sptr = new SmudgeClass(Type);
    DEBUG_ASSERT(sptr != nullptr);
    return sptr;
#else
    return nullptr;
#endif
}

const int16_t *SmudgeTypeClass::Occupy_List(BOOL a1) const
{
    DEBUG_ASSERT(this != nullptr);

    static int16_t _occupy[32];

    int16_t *list = _occupy;
    for (int i = 0; i < Width; ++i) {
        for (int j = 0; j < Height; ++j) {
            *list++ += (i + 128);
        }
    }

    *list = LIST_END;

    return _occupy;
}

const int16_t *SmudgeTypeClass::Overlap_List() const
{
    DEBUG_ASSERT(this != nullptr);
    return Occupy_List(false);
}

void SmudgeTypeClass::Draw_It(int x, int y, int frame) const
{
    DEBUG_ASSERT(this != nullptr);

    if (Get_Image_Data() != nullptr) {
        g_isTheaterShape = true;
        CC_Draw_Shape(Get_Image_Data(), frame, x, y, WINDOW_TACTICAL, SHAPE_WIN_REL);
        g_isTheaterShape = false;
    }
}

SmudgeType SmudgeTypeClass::From_Name(const char *name)
{
    DEBUG_ASSERT(name != nullptr);

    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return SMUDGE_NONE;
    }

    if (name != nullptr) {
        for (SmudgeType smudge = SMUDGE_FIRST; smudge < SMUDGE_COUNT; ++smudge) {
            if (strcasecmp(name, Name_From(smudge)) == 0) {
                return smudge;
            }
        }
    }

    return SMUDGE_NONE;
}

const char *SmudgeTypeClass::Name_From(SmudgeType smudge)
{
    DEBUG_ASSERT(smudge != SMUDGE_NONE);
    DEBUG_ASSERT(smudge < SMUDGE_COUNT);

    return smudge != SMUDGE_NONE && smudge < SMUDGE_COUNT ? As_Reference(smudge).Get_Name() : "<none>";
}

void SmudgeTypeClass::Init_Heap()
{
    // Order of allocation MUST match order of SmudgeType enum.
    new SmudgeTypeClass(SmudgeCrater1);
    new SmudgeTypeClass(SmudgeCrater2);
    new SmudgeTypeClass(SmudgeCrater3);
    new SmudgeTypeClass(SmudgeCrater4);
    new SmudgeTypeClass(SmudgeCrater5);
    new SmudgeTypeClass(SmudgeCrater6);
    new SmudgeTypeClass(SmudgeScorch1);
    new SmudgeTypeClass(SmudgeScorch2);
    new SmudgeTypeClass(SmudgeScorch3);
    new SmudgeTypeClass(SmudgeScorch4);
    new SmudgeTypeClass(SmudgeScorch5);
    new SmudgeTypeClass(SmudgeScorch6);

    new SmudgeTypeClass(SmudgeBibx1);
    new SmudgeTypeClass(SmudgeBibx2);
    new SmudgeTypeClass(SmudgeBibx3);
}

void SmudgeTypeClass::Init(TheaterType theater)
{
    DEBUG_ASSERT(theater < THEATER_COUNT);
    DEBUG_ASSERT(theater != THEATER_NONE);

    char filename[256];

    if (theater != g_lastTheater) {
        for (SmudgeType i = SMUDGE_FIRST; i < SMUDGE_COUNT; ++i) {
            SmudgeTypeClass &smudge = As_Reference(i);
            const char *name = smudge.ImageName[0] != '\0' ? smudge.ImageName : smudge.Get_Name();
            snprintf(filename, sizeof(filename), "%s.%s", name, g_theaters[theater].ext);
            smudge.ImageData = MixFileClass<GameFileClass>::Retrieve(filename);
        }
    }
}

SmudgeTypeClass &SmudgeTypeClass::As_Reference(SmudgeType smudge)
{
    DEBUG_ASSERT(smudge != SMUDGE_NONE);
    DEBUG_ASSERT(smudge < SMUDGE_COUNT);
    DEBUG_ASSERT(&SmudgeTypes[smudge] != nullptr);

    return SmudgeTypes[smudge];
}

SmudgeTypeClass *SmudgeTypeClass::As_Pointer(SmudgeType smudge)
{
    DEBUG_ASSERT(smudge != SMUDGE_NONE);
    DEBUG_ASSERT(smudge < SMUDGE_COUNT);

    return (smudge < SMUDGE_COUNT) && (smudge != SMUDGE_NONE) ? &SmudgeTypes[smudge] : nullptr;
}
