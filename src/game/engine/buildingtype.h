/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class holding static info on building objects.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef BUILDINGTYPE_H
#define BUILDINGTYPE_H

#include "always.h"
#include "facing.h"
#include "heap.h"
#include "smudgetype.h"
#include "technotype.h"
#include "theater.h"

enum BuildingType
{
    BUILDING_NONE = -1,
    BUILDING_FIRST = 0,

    // The first 32 buildings here can be stored in an unsigned 32bit bit field. For example, in HouseClass to determine
    // which structures the house has. To decide which bit is set, the formula is (1 << value)
    BUILDING_ATEK = 0,
    BUILDING_IRON_CURTAIN = 1,
    BUILDING_WEAP = 2,
    BUILDING_PDOX = 3,
    BUILDING_PBOX = 4,
    BUILDING_HBOX = 5,
    BUILDING_DOME = 6,
    BUILDING_GAP_GENERATOR = 7,
    BUILDING_GUN = 8,
    BUILDING_AGUN = 9,
    BUILDING_FTUR = 10,
    BUILDING_FACT = 11,
    BUILDING_PROC = 12,
    BUILDING_SILO = 13,
    BUILDING_HELIPAD = 14,
    BUILDING_SAM = 15,
    BUILDING_AIRFIELD = 16,
    BUILDING_POWR = 17,
    BUILDING_APWR = 18,
    BUILDING_STEK = 19,
    BUILDING_HOSPITAL = 20,
    BUILDING_BARR = 21,
    BUILDING_TENT = 22,
    BUILDING_KENN = 23,
    BUILDING_FIX = 24,
    BUILDING_BIO = 25,
    BUILDING_MISS = 26,
    BUILDING_SYRD = 27,
    BUILDING_SPEN = 28,
    BUILDING_MSLO = 29,
    BUILDING_FCOM = 30,
    BUILDING_TESLA_COIL = 31,

    // For buildings beyond this to count in the check for what exists, the bitfield will need extending.
    BUILDING_VALID_COUNT = 32,
    
    // Fakes
    BUILDING_WEAF = 32,
    BUILDING_FACF = 33,
    BUILDING_SYRF = 34,
    BUILDING_SPEF = 35,
    BUILDING_DOMF = 36,

    // Walls
    BUILDING_SBAG = 37,
    BUILDING_CYCL = 38,
    BUILDING_BRIK = 39,
    BUILDING_BARB = 40,
    BUILDING_WOOD = 41,
    BUILDING_FENC = 42,

    // Mines
    BUILDING_MINV = 43,
    BUILDING_MINP = 44,

    // Villages
    BUILDING_V01 = 45,
    BUILDING_V02 = 46,
    BUILDING_V03 = 47,
    BUILDING_V04 = 48,
    BUILDING_V05 = 49,
    BUILDING_V06 = 50,
    BUILDING_V07 = 51,
    BUILDING_V08 = 52,
    BUILDING_V09 = 53,
    BUILDING_V10 = 54,
    BUILDING_V11 = 55,
    BUILDING_V12 = 56,
    BUILDING_V13 = 57,
    BUILDING_V14 = 58,
    BUILDING_V15 = 59,
    BUILDING_V16 = 60,
    BUILDING_V17 = 61,
    BUILDING_V18 = 62,
    BUILDING_V19 = 63,
    BUILDING_V20 = 64,
    BUILDING_V21 = 65,
    BUILDING_V22 = 66,
    BUILDING_V23 = 67,
    BUILDING_V24 = 68,
    BUILDING_V25 = 69,
    BUILDING_V26 = 70,
    BUILDING_V27 = 71,
    BUILDING_V28 = 72,
    BUILDING_V29 = 73,
    BUILDING_V30 = 74,
    BUILDING_V31 = 75,
    BUILDING_V32 = 76,
    BUILDING_V33 = 77,
    BUILDING_V34 = 78,
    BUILDING_V35 = 79,
    BUILDING_V36 = 80,
    BUILDING_V37 = 81,

    // Barrels
    BUILDING_BARREL = 82,
    BUILDING_BRL3 = 83,

    // Ants
    BUILDING_QUEE = 84,
    BUILDING_LAR1 = 85,
    BUILDING_LAR2 = 86,

    BUILDING_COUNT = 87,
    BUILDING_NOEXP_COUNT = 84
};

DEFINE_ENUMERATION_OPERATORS(BuildingType);

enum BSizeType
{
    BSIZE_NONE = -1,
    BSIZE_FIRST = 0,
    BSIZE_1BY1 = 0,
    BSIZE_2BY1 = 1,
    BSIZE_1BY2 = 2,
    BSIZE_2BY2 = 3,
    BSIZE_2BY3 = 4,
    BSIZE_3BY2 = 5,
    BSIZE_3BY3 = 6,
    BSIZE_4BY2 = 7,
    BSIZE_5BY5 = 8,
    BSIZE_COUNT
};

enum BStateType
{
    BSTATE_NONE = -1,
    BSTATE_FIRST = 0,
    BSTATE_0 = 0, // this could be ActiveAnimOne            //CONSTRUCTION
    BSTATE_1 = 1, // i think this is Idle or ActiveAnimTwo
    BSTATE_2 = 2, // attacking?                //ATTACKING
    BSTATE_3 = 3,
    BSTATE_4 = 4,
    BSTATE_5 = 5,
    BSTATE_COUNT
};

DEFINE_ENUMERATION_OPERATORS(BStateType);

class BuildingTypeClass : public TechnoTypeClass
{
    struct AnimControlType
    {
        int m_Start; 
        int m_FrameCount;
        int m_Rate;
    };

public:
    BuildingTypeClass(BuildingType type, int uiname, const char *name, FacingType facing, coord_t exit_coord,
        RemapType altremap, int primaryf, int primaryl, int primaryh, BOOL fake, BOOL unk1, BOOL nominal, BOOL wall,
        BOOL unk2, BOOL radar_invisible, BOOL selectable, BOOL legal_target, BOOL insignificant, BOOL theater, BOOL turret,
        BOOL remapable, RTTIType factory, DirType dir, BSizeType size, const int16_t *exit_list, const int16_t *occupy_list,
        const int16_t *overlap_list);
    BuildingTypeClass(const BuildingTypeClass &that);
    BuildingTypeClass(const NoInitClass &noinit) : TechnoTypeClass(noinit) {}
    ~BuildingTypeClass() {}

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    // AbstractTypeClass virtuals.
    virtual coord_t Coord_Fixup(coord_t coord) const override;
    virtual int Full_Name() const override;
    // ObjectTypeClass virtuals
    virtual int Max_Pips() const override;
    virtual void Dimensions(int &w, int &h) const override;
    virtual BOOL Create_And_Place(cell_t cellnum, HousesType house = HOUSES_NONE) const override;
    virtual int Cost_Of() const override;
    virtual ObjectClass *Create_One_Of(HouseClass *house) const override;
    virtual const int16_t *Occupy_List(BOOL recalc = false) const override;
    virtual const int16_t *Overlap_List() const override;
    // TechnoTypeClass virtuals
    virtual int Raw_Cost() const override;
    virtual BOOL Read_INI(GameINIClass &ini) override;
    virtual void *Get_Buildup_Data() const { return m_BuildupData; }

    int Width() const;
    int Height(BOOL check_bib = false) const;
    BOOL Bib_And_Offset(SmudgeType &smudge, int16_t &cellnum) const;

    void Code_Pointers() {}
    void Decode_Pointers() {}

    BuildingType What_Type() const { return m_Type; }
    RTTIType Factory_Type() const { return m_FactoryType; }
    BOOL Is_Fake() const { return m_Fake; }
    BOOL Is_Capturable() const { return m_Capturable; }
    BOOL Base_Normal() const { return m_BaseNormal; }
    BOOL Unsellable() const { return m_Unsellable; }
    BOOL Is_Wall() const { return m_Wall; }
    int Adjacency() const { return m_Adjacency; }
    void Init_Anim(BStateType bstate, int start_frame, int frame_count, int delay);

    int Power_Output() const { return m_Power; }
    int Power_Drain() const { return m_Drain; }
    int Storage_Capacity() const { return m_Storage; }

    static void One_Time();
    static BuildingTypeClass &As_Reference(BuildingType type);
    static BuildingTypeClass *As_Pointer(BuildingType type);
    static BuildingType From_Name(const char *name);
    static void Init(TheaterType theater);
    static void Init_Heap();
    static const char *Name_From(BuildingType type) { return As_Reference(type).m_Name; }

    static void Prep_For_Add();
    
private:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_BaseNormal : 1; // & 1 Considered for building adjacency checks (def = true)?
    BOOL m_Fake : 1; // & 2 Is this a fake structure (def = false)?
    BOOL m_Bib : 1; // & 4 Does the building have a bib built in (def = false)?
    BOOL m_Wall : 1; // & 8 Is this a wall type structure [special rules apply] (def = false)?
    BOOL m_SimpleDamage : 1; // & 16 Does building have simple damage imagery (def = true)?
    BOOL m_Capturable : 1; // & 32 Can this building be infiltrated by a spy/thief/engineer (def = false)?
    BOOL m_Normalized : 1; // & 64 Is nimation speed adjusted for a consistent speed (def = false)?
    BOOL m_Powered : 1; // & 128 Does it require power to function (def = false)?
    BOOL m_Unsellable : 1; // & 1 Cannot sell this building (even if it can be built) (def = false)?
#else
    bool m_BaseNormal; // Considered for building adjacency checks (def = true)?
    bool m_Fake; // Is this a fake structure (def = false)?
    bool m_Bib; // Does the building have a bib built in (def = false)?
    bool m_Wall; // Is this a wall type structure [special rules apply] (def = false)?
    bool m_SimpleDamage; // Does building have simple damage imagery (def = true)?
    bool m_Capturable; // Can this building be infiltrated by a spy/thief/engineer (def = false)?
    bool m_Normalized;  // Is nimation speed adjusted for a consistent speed (def = false)?
    bool m_Powered;// Does it require power to function (def = false)?
    bool m_Unsellable; // Cannot sell this building (even if it can be built) (def = false)?
#endif
    FacingType m_Facing;
    int m_Adjacency;
    RTTIType m_FactoryType; // Type of object this building can produce.
    coord_t m_ExitCoord;
    const int16_t *m_ExitList;
    BuildingType m_Type;
    DirType m_StartFacing;
    int m_Storage; // Number of bails stored
    int m_Power;
    int m_Drain;
    BSizeType m_BuildingSize; // The size/foundation this building occupies (def = BSIZE_NONE).
    AnimControlType m_Anims[BSTATE_COUNT];
    const int16_t *m_OccupyList;
    const int16_t *m_OverlapList;
    void *m_BuildupData;

public:
#ifdef GAME_DLL
    static void *&g_WarFactoryOverlay;
    static void *&g_LightningShapes;
#else
    static void *g_WarFactoryOverlay;
    static void *g_LightningShapes;
#endif
};

#ifdef GAME_DLL
extern TFixedIHeapClass<BuildingTypeClass> &g_BuildingTypes;
#else
extern TFixedIHeapClass<BuildingTypeClass> g_BuildingTypes;
#endif

#endif // BUILDINGTYPE_H
