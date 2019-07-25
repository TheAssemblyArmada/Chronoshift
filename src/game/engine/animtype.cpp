/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Contains static data for anim objects.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "animtype.h"
#include "animdata.h"
#include "gamefile.h"
#include "coord.h"
#include "mixfile.h"
#include "voc.h"
#include <cstdio>

using std::snprintf;

#ifndef CHRONOSHIFT_STANDALONE
TFixedIHeapClass<AnimTypeClass> &g_AnimTypes = Make_Global<TFixedIHeapClass<AnimTypeClass> >(0x0065DEA0);
#else
TFixedIHeapClass<AnimTypeClass> g_AnimTypes;
#endif

/**
 * 0x00407388
 */
AnimTypeClass::AnimTypeClass(AnimType anim, const char *name, int a3, int mid_point, BOOL theater,
    BOOL normalized, BOOL a7, BOOL scorch, BOOL crater, BOOL sticky, BOOL surface,
    BOOL translucent, BOOL flamer, fixed_t damage, int rate, int start, int loop_start,
    int loop_end, int end, int loop_count, VocType report, AnimType next) :
    ObjectTypeClass(RTTI_ANIMTYPE, 0, 0, 0, 0, 0, 0, 0, 0, TXT_NULL, name),
    m_Normalized(normalized),
    m_Surface(surface),
    m_Translucent(translucent),
    m_Bit_8(a7),
    m_Flamer(flamer),
    m_Scorch(scorch),
    m_Crater(crater),
    m_Sticky(sticky),
    m_Theater(theater),
    m_Type(anim),
    m_field_13D(a3),
    m_MidPoint(mid_point),
    m_Damage(damage),
    m_Rate(rate),
    m_Start(start),
    m_LoopStart(loop_start),
    m_LoopEnd(loop_end),
    m_End(end),
    m_LoopCount(loop_count),
    m_Report(report),
    m_Next(next)
{
}

/**
 * 0x0041C750
 */
AnimTypeClass::AnimTypeClass(const AnimTypeClass &that) :
    ObjectTypeClass(that),
    m_Normalized(that.m_Normalized),
    m_Surface(that.m_Surface),
    m_Translucent(that.m_Translucent),
    m_Bit_8(that.m_Bit_8),
    m_Flamer(that.m_Flamer),
    m_Scorch(that.m_Scorch),
    m_Crater(that.m_Crater),
    m_Sticky(that.m_Sticky),
    m_Theater(that.m_Theater),
    m_Type(that.m_Type),
    m_field_13D(that.m_field_13D),
    m_MidPoint(that.m_MidPoint),
    m_Damage(that.m_Damage),
    m_Rate(that.m_Rate),
    m_Start(that.m_Start),
    m_LoopStart(that.m_LoopStart),
    m_LoopEnd(that.m_LoopEnd),
    m_End(that.m_End),
    m_LoopCount(that.m_LoopCount),
    m_Report(that.m_Report),
    m_Next(that.m_Next)
{
}

/**
 * 0x00407564
 */
void *AnimTypeClass::operator new(size_t size)
{
    return g_AnimTypes.Allocate();
}

/**
 * 0x00407578
 */
void AnimTypeClass::operator delete(void *ptr)
{
    g_AnimTypes.Free(ptr);
}

/**
 * @brief 
 *
 * 0x0041C730
 */
BOOL AnimTypeClass::Create_And_Place(cell_t cellnum, HousesType house) const
{
#if 0
    coord_t coord = Cell_To_Coord(cellnum);
    AnimClass *aptr = new AnimClass(Type, coord);
    DEBUG_ASSERT(aptr != nullptr);

    return aptr != nullptr;
#else 
    return false;
#endif
}

/**
 * @brief 
 *
 * 0x0041C740
 */
ObjectClass *AnimTypeClass::Create_One_Of(HouseClass *house) const
{
#if 0
    AnimClass *aptr = new AnimClass(Type, 0);
    DEBUG_ASSERT(aptr != nullptr);
    return aptr;
#else
    return nullptr;
#endif
}

/**
 * @brief Initialises the memory heap for AnimTypeClass objects.
 * @warning Order of initialisation is important so enum matches position in the heap.
 *
 * 0x00407590
 */
void AnimTypeClass::Init_Heap()
{
    // The order of heap initialisation MUST match the AnimType enum in animtype.h
    new AnimTypeClass(AnimFBall1);
    new AnimTypeClass(AnimFireBallFade);
    new AnimTypeClass(AnimFrag1);
    new AnimTypeClass(AnimVehHit1);
    new AnimTypeClass(AnimVehHit2);
    new AnimTypeClass(AnimVehHit3);
    new AnimTypeClass(AnimArtExp1);
    new AnimTypeClass(AnimNapalm1);
    new AnimTypeClass(AnimNapalm2);
    new AnimTypeClass(AnimNapalm3);
    new AnimTypeClass(AnimSmokePuff);
    new AnimTypeClass(AnimPiff);
    new AnimTypeClass(AnimPiffPiff);
    new AnimTypeClass(AnimFire3);
    new AnimTypeClass(AnimFire2);
    new AnimTypeClass(AnimFire1);
    new AnimTypeClass(AnimFire4);
    new AnimTypeClass(AnimGunfire);
    new AnimTypeClass(AnimSmokeM);
    new AnimTypeClass(AnimBurnSmall);
    new AnimTypeClass(AnimBurnMed);
    new AnimTypeClass(AnimBurnBig);
    new AnimTypeClass(AnimOnFireSmall);
    new AnimTypeClass(AnimOnFireMed);
    new AnimTypeClass(AnimOnFireBig);
    new AnimTypeClass(AnimSAMN);
    new AnimTypeClass(AnimSAMNE);
    new AnimTypeClass(AnimSAME);
    new AnimTypeClass(AnimSAMSE);
    new AnimTypeClass(AnimSAMS);
    new AnimTypeClass(AnimSAMSW);
    new AnimTypeClass(AnimSAMW);
    new AnimTypeClass(AnimSAMNW);
    new AnimTypeClass(AnimGUNN);
    new AnimTypeClass(AnimGUNNE);
    new AnimTypeClass(AnimGUNE);
    new AnimTypeClass(AnimGUNSE);
    new AnimTypeClass(AnimGUNS);
    new AnimTypeClass(AnimGUNSW);
    new AnimTypeClass(AnimGUNW);
    new AnimTypeClass(AnimGUNNW);
    new AnimTypeClass(AnimLZSmoke);
    new AnimTypeClass(AnimCrateDeviator);
    new AnimTypeClass(AnimCrateDollar);
    new AnimTypeClass(AnimCrateEarth);
    new AnimTypeClass(AnimCrateEmpulse);
    new AnimTypeClass(AnimCrateInvun);
    new AnimTypeClass(AnimCrateMine);
    new AnimTypeClass(AnimCrateRapid);
    new AnimTypeClass(AnimCrateStealth);
    new AnimTypeClass(AnimCrateMissile);
    new AnimTypeClass(AnimMoveFlash);
    new AnimTypeClass(AnimOilFieldBurn);
    new AnimTypeClass(AnimElectricDie);
    new AnimTypeClass(AnimParachute);
    new AnimTypeClass(AnimDogElectricDie);
    new AnimTypeClass(AnimCorpse1);
    new AnimTypeClass(AnimCorpse2);
    new AnimTypeClass(AnimCorpse3);
    new AnimTypeClass(AnimSputDoor);
    new AnimTypeClass(AnimAtomBomb);
    new AnimTypeClass(AnimChronoBox);
    new AnimTypeClass(AnimGPSBox);
    new AnimTypeClass(AnimInvulBox);
    new AnimTypeClass(AnimParaBox);
    new AnimTypeClass(AnimSonarBox);
    new AnimTypeClass(AnimTwinkle1);
    new AnimTypeClass(AnimTwinkle2);
    new AnimTypeClass(AnimTwinkle3);
    new AnimTypeClass(AnimFlak);
    new AnimTypeClass(AnimWaterExp1);
    new AnimTypeClass(AnimWaterExp2);
    new AnimTypeClass(AnimWaterExp3);
    new AnimTypeClass(AnimCrateArmor);
    new AnimTypeClass(AnimCrateSpeed);
    new AnimTypeClass(AnimCrateFPower);
    new AnimTypeClass(AnimCrateTQuake);
    new AnimTypeClass(AnimParaBomb);
    new AnimTypeClass(AnimMineExp1);
    new AnimTypeClass(AnimAntDeath);
}

/**
 * @brief One time initialisation of data.
 *
 * 0x0041C5E4
 */
void AnimTypeClass::One_Time()
{
    char filename[512];

    for (AnimType type = ANIM_FIRST; type < ANIM_COUNT; ++type) {
        AnimTypeClass *aptr = As_Pointer(type);
        const char *name = aptr->ImageName[0] != '\0' ? aptr->ImageName : aptr->Get_Name();
        snprintf(filename, sizeof(filename), "%s.shp", name);
        aptr->ImageData = GameFileClass::Retrieve(filename);

    }
}

/**
 * @brief Initialisation of theater specific data for current theater.
 *
 * 0x0041C5E4
 */
void AnimTypeClass::Init(TheaterType theater)
{
    DEBUG_ASSERT(theater < THEATER_COUNT);
    DEBUG_ASSERT(theater != THEATER_NONE);

    char filename[512];

    if (theater != g_lastTheater) {
        for (AnimType type = ANIM_FIRST; type < ANIM_COUNT; ++type) {
            AnimTypeClass *atptr = As_Pointer(type);
            const char *name = atptr->ImageName[0] != '\0' ? atptr->ImageName : atptr->Get_Name();
            // TODO: Change the theater info to lower case and standardise on that?
            const char *ext = atptr->m_Theater ? g_theaters[theater].ext : "shp";
            snprintf(filename, sizeof(filename), "%s.%s", name, ext);
            atptr->ImageData = GameFileClass::Retrieve(filename);
        }

    }
}

/**
 * @brief Gets appropriate enum value from provided string.
 */
AnimType AnimTypeClass::From_Name(const char *name)
{
    DEBUG_ASSERT(name != nullptr);

    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return ANIM_NONE;
    }

    if (name != nullptr) {
        for (AnimType type = ANIM_FIRST; type < ANIM_COUNT; ++type) {
            if (strcasecmp(name, Name_From(type)) == 0) {
                return type;
            }
        }
    }

    return ANIM_NONE;
}

/**
 * @brief Gets appropriate enum value from provided string.
 */
const char *AnimTypeClass::Name_From(AnimType type)
{
    return type != ANIM_NONE && type < ANIM_COUNT ? As_Reference(type).Get_Name() : "<none>";
}

/**
 * @brief Gets a reference to an AnimTypeClass object from an AnimType enum value.
 *
 * 0x0041C714
 */
AnimTypeClass &AnimTypeClass::As_Reference(AnimType type)
{
    AnimTypeClass *ptr = &g_AnimTypes[type];
    DEBUG_ASSERT(ptr != nullptr);
    return *ptr;
}

/**
 * @brief Gets a pointer to an AnimTypeClass object from an AnimType enum value.
 */
AnimTypeClass *AnimTypeClass::As_Pointer(AnimType type)
{
    return type != ANIM_NONE && type < ANIM_COUNT ? &g_AnimTypes[type] : nullptr;
}
