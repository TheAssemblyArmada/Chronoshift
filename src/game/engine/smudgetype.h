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
#pragma once

#ifndef SMUDGETYPE_H
#define SMUDGETYPE_H

#include "always.h"
#include "heap.h"
#include "objecttype.h"
#include "theater.h"

enum SmudgeType
{
    SMUDGE_CR1,
    SMUDGE_CR2,
    SMUDGE_CR3,
    SMUDGE_CR4,
    SMUDGE_CR5,
    SMUDGE_CR6,
    SMUDGE_SC1,
    SMUDGE_SC2,
    SMUDGE_SC3,
    SMUDGE_SC4,
    SMUDGE_SC5,
    SMUDGE_SC6,
    SMUDGE_BIB1,
    SMUDGE_BIB2,
    SMUDGE_BIB3,

    SMUDGE_COUNT,

    SMUDGE_NONE = -1,
    SMUDGE_FIRST = SMUDGE_CR1
};

DEFINE_ENUMERATION_OPERATORS(SmudgeType);

class SmudgeTypeClass : public ObjectTypeClass
{
public:
    SmudgeTypeClass(SmudgeType smudge, const char *name, int uiname, int width = 1, int height = 1, BOOL crater = false, BOOL burn = false);
    SmudgeTypeClass(const SmudgeTypeClass &that);
    SmudgeTypeClass(const NoInitClass &noinit) : ObjectTypeClass(noinit) {}
    ~SmudgeTypeClass() {}

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    void Code_Pointers() {}
    void Decode_Pointers() {}

    virtual BOOL Create_And_Place(cell_t cellnum, HousesType house = HOUSES_NONE) const override;
    virtual ObjectClass *Create_One_Of(HouseClass *house) const override;
    virtual const int16_t *Occupy_List(BOOL a1 = false) const override;
    virtual const int16_t *Overlap_List() const override;
    virtual void Draw_It(int x, int y, int frame) const;

    BOOL Is_Crater() { return m_Crater; }
    BOOL Is_Bib() { return m_Bib; }

    static void Init_Heap();
    static void One_Time() {}
    static void Init(TheaterType theater);
    static SmudgeType From_Name(const char *name);
    static const char *Name_From(SmudgeType smudge);
    static SmudgeTypeClass &As_Reference(SmudgeType smudge);
    static SmudgeTypeClass *As_Pointer(SmudgeType smudge);

    static void Prep_For_Add();

private:
    SmudgeType m_Type;
    int m_Width;// Width in cells
    int m_Height; // Height in cells
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_Crater : 1; // 1 Crater smudges
    BOOL m_Bib : 1; //2 Burn and Bib smudges
#else
    bool m_Crater; // Crater smudges
    bool m_Bib; // Burn and Bib smudges
#endif
};

#ifdef GAME_DLL
extern TFixedIHeapClass<SmudgeTypeClass> &g_SmudgeTypes;
#else
extern TFixedIHeapClass<SmudgeTypeClass> g_SmudgeTypes;
#endif

#endif // SMUDGETYPE_H