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
    SMUDGE_NONE = -1,

    SMUDGE_FIRST = 0,

    SMUDGE_CR1 = 0,
    SMUDGE_CR2 = 1,
    SMUDGE_CR3 = 2,
    SMUDGE_CR4 = 3,
    SMUDGE_CR5 = 4,
    SMUDGE_CR6 = 5,

    SMUDGE_SC1 = 6,
    SMUDGE_SC2 = 7,
    SMUDGE_SC3 = 8,
    SMUDGE_SC4 = 9,
    SMUDGE_SC5 = 10,
    SMUDGE_SC6 = 11,

    SMUDGE_BIB1 = 12,
    SMUDGE_BIB2 = 13,
    SMUDGE_BIB3 = 14,

    SMUDGE_LAST = 14,

    SMUDGE_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(SmudgeType);

class SmudgeTypeClass : public ObjectTypeClass
{
public:
    SmudgeTypeClass(SmudgeType smudge, const char *name, int uiname, int width = 1, int height = 1, BOOL crater = false, BOOL burn = false);
    SmudgeTypeClass(SmudgeTypeClass const &that);
    SmudgeTypeClass(NoInitClass const &noinit) : ObjectTypeClass(noinit) {}
    ~SmudgeTypeClass() {}

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    void Code_Pointers() {}
    void Decode_Pointers() {}

    virtual BOOL Create_And_Place(int16_t cellnum, HousesType house = HOUSES_NONE) const override;
    virtual ObjectClass *Create_One_Of(HouseClass *house) const override;
    virtual const int16_t *Occupy_List(BOOL a1 = false) const override;
    virtual const int16_t *Overlap_List() const override;
    virtual void Draw_It(int x, int y, int frame) const;

    BOOL Is_Crater() { return Crater; }
    BOOL Is_Bib() { return Bib; }

    static SmudgeType From_Name(const char *name);
    static const char *Name_From(SmudgeType smudge);
    static void Init_Heap();
    static void One_Time() {}
    static void Init(TheaterType theater);
    static SmudgeTypeClass &As_Reference(SmudgeType smudge);
    static SmudgeTypeClass *As_Pointer(SmudgeType smudge);

private:
    SmudgeType Type;
    int Width;// Width in cells
    int Height; // Height in cells
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool Crater : 1; // 1 Crater smudges
            bool Bib : 1; //2 Burn and Bib smudges
        };
        int Bitfield;
    };
#else
    bool Crater; // Crater smudges
    bool Bib; // Burn and Bib smudges
#endif
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
extern TFixedIHeapClass<SmudgeTypeClass> &SmudgeTypes;
#else
extern TFixedIHeapClass<SmudgeTypeClass> SmudgeTypes;
#endif

#endif // SMUDGETYPE_H