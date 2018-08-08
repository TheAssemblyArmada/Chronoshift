/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Contains static data for anim objects.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef ANIMTYPE_H
#define ANIMTYPE_H

#include "always.h"
#include "audio.h"
#include "fixed.h"
#include "heap.h"
#include "objecttype.h"
#include "theater.h"

// If you want to add extra animations, you will also need to add a template class instance in animtype.cpp and add it to the
// Init_Heap function.
enum AnimType
{
    ANIM_NONE = -1,
    ANIM_FIRST = 0,
    ANIM_FBALL1 = 0, // "FBALL1"
    ANIM_FIREBALLFADE = 1, // "FB2
    ANIM_FRAG1 = 2, // "FRAG1"
    ANIM_VEH_HIT1 = 3, // "VEH-HIT1"
    ANIM_VEH_HIT2 = 4, // "VEH-HIT2"
    ANIM_VEH_HIT3 = 5, // "VEH-HIT3"
    ANIM_ART_EXP1 = 6, // "ART-EXP1"
    ANIM_NAPALM1 = 7, // "NAPALM1"
    ANIM_NAPALM2 = 8, // "NAPALM2"
    ANIM_NAPALM3 = 9, // "NAPALM3"
    ANIM_SMOKE_PUFF = 10, // "SMOKEY"
    ANIM_PIFF = 11, // "PIFF"			//small arms fire piff (single shot)
    ANIM_PIFFPIFF = 12, // "PIFFPIFF"		//small arms fire piff (multiple shots)
    ANIM_FIRE3 = 13, // "FIRE3"			//small flames
    ANIM_FIRE2 = 14, // "FIRE2"
    ANIM_FIRE1 = 15, // "FIRE1"
    ANIM_FIRE4 = 16, // "FIRE4"
    ANIM_MUZZLE_FLASH = 17, // "GUNFIRE"
    ANIM_SMOKE_M = 18, // "SMOKE_M"
    ANIM_BURN_SMALL = 19, // "BURN-S"
    ANIM_BURN_MED = 20, // "BURN-M"
    ANIM_BURN_BIG = 21, // "BURN-L"
    ANIM_ON_FIRE_SMALL = 22, // "BURN-S"
    ANIM_ON_FIRE_MED = 23, // "BURN-M"
    ANIM_ON_FIRE_BIG = 24, // "BURN-L"
    ANIM_SAM_N = 25, // "SAMFIRE"
    ANIM_SAM_NE = 26, // "SAMFIRE"
    ANIM_SAM_E = 27, // "SAMFIRE"
    ANIM_SAM_SE = 28, // "SAMFIRE"
    ANIM_SAM_S = 29, // "SAMFIRE"
    ANIM_SAM_SW = 30, // "SAMFIRE"
    ANIM_SAM_W = 31, // "SAMFIRE"
    ANIM_SAM_NW = 32, // "SAMFIRE"
    ANIM_GUN_N = 33, // "MINIGUN"
    ANIM_GUN_NE = 34, // "MINIGUN"
    ANIM_GUN_E = 35, // "MINIGUN"
    ANIM_GUN_SE = 36, // "MINIGUN"
    ANIM_GUN_S = 37, // "MINIGUN"
    ANIM_GUN_SW = 38, // "MINIGUN"
    ANIM_GUN_W = 39, // "MINIGUN"
    ANIM_GUN_NW = 40, // "MINIGUN"
    ANIM_LZ_SMOKE = 41, // "SMOKLAND"
    ANIM_CRATE_DEVIATOR = 42, // "DEVIATOR"
    ANIM_CRATE_DOLLAR = 43, // "DOLLAR"			//Money bonus
    ANIM_CRATE_EARTH = 44, // "EARTH"
    ANIM_CRATE_EMPULSE = 45, // "EMPULSE"
    ANIM_CRATE_INVUN = 46, // "INVUN"
    ANIM_CRATE_MINE = 47, // "MINE"
    ANIM_CRATE_RAPID = 48, // "RAPID"
    ANIM_CRATE_STEALTH = 49, // "STEALTH2"
    ANIM_CRATE_MISSILE = 50, // "MISSILE2"
    ANIM_MOVEFLASH = 51, // "MOVEFLSH"
    ANIM_OILFIELD_BURN = 52, // "FLMSPT"
    ANIM_ELECTRO = 53, // "ELECTRO"
    ANIM_PARACHUTE = 54, // "PARACH"
    ANIM_ELECTDOG = 55, // "ELECTDOG"
    ANIM_CORPSE1 = 56, // "CORPSE1"
    ANIM_CORPSE2 = 57, // "CORPSE2"
    ANIM_CORPSE3 = 58, // "CORPSE3"
    ANIM_SPUTDOOR = 59, // "SPUTDOOR"
    ANIM_ATOMSFX = 60, // "ATOMSFX"
    ANIM_CHRONOBOX = 61, // "CHRONBOX"
    ANIM_GPSBOX = 62, // "GPSBOX"
    ANIM_INVULBOX = 63, // "INVULBOX"
    ANIM_PARABOX = 64, // "PARABOX"
    ANIM_SONARBOX = 65, // "SONARBOX"
    ANIM_TWINKLE1 = 66, // "TWINKLE1"			//ore twinkle animation
    ANIM_TWINKLE2 = 67, // "TWINKLE2"			//ore twinkle animation
    ANIM_TWINKLE3 = 68, // "TWINKLE3"			//ore twinkle animation
    ANIM_FLAK = 69, // "FLAK"
    ANIM_WATER_EXP1 = 70, // "H2O_EXP1"
    ANIM_WATER_EXP2 = 71, // "H2O_EXP2"
    ANIM_WATER_EXP3 = 72, // "H2O_EXP3"
    ANIM_CRATE_ARMOR = 73, // "ARMOR"			//Armor bonus
    ANIM_CRATE_SPEED = 74, // "SPEED"
    ANIM_CRATE_FPOWER = 75, // "FPOWER"			//Firepower bonus crate animation
    ANIM_CRATE_TQUAKE = 76, // "TQUAKE"
    ANIM_PARABOMB = 77, // "PARABOMB"
    ANIM_MINEEXP1 = 78, // "VEH-HIT2"
    ANIM_ANTDEATH = 79, // "ANTDIE"
    // ANIM_WWCRATE = 80, // "WWCRATE" RA++
    ANIM_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(AnimType);

class AnimTypeClass : public ObjectTypeClass
{
public:
    AnimTypeClass(AnimType anim, char const *name, int a3, int mid_point, BOOL theater, BOOL normalized, BOOL a7,
        BOOL scorch, BOOL crater, BOOL sticky, BOOL surface, BOOL translucent, BOOL flamer, fixed damage, int rate,
        int start, int loop_start, int loop_end, int end, int loop_count, VocType report, AnimType next);
    AnimTypeClass(AnimTypeClass const &that);
    AnimTypeClass(NoInitClass const &noinit) : ObjectTypeClass(noinit) {}
    ~AnimTypeClass() {}

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

    static void Init_Heap();
    static void One_Time();
    static void Init(TheaterType theater);
    static AnimType From_Name(char const *name);
    static char const *Name_From(AnimType anim);
    static AnimTypeClass &As_Reference(AnimType anim);
    static AnimTypeClass *As_Pointer(AnimType anim);

private:
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool Normalized : 1; // Should the animation speed be adjusted to appear at a consistent speed (def = false)?
            bool Surface : 1; // Is this animation at ground level (def = false)?
            bool Translucent : 1; // Is this animation translucent in appearence (def = false)?
            bool ATBit1_8 : 1; // AltPalette = Does it use an alternate drawing palette (def = false)?
            bool Flamer : 1; // Flamer = Does this animation leave flames after it is gone [e.g., napalm] (def = false)?
            bool Scorch : 1; // Does this animation scorch the ground [e.g., napalm does this] (def = false)?
            bool Crater : 1; // Does this form a crater [e.g., artillery does this] (def = false)?
            bool Sticky : 1; // Sticky = Animation sticks to unit in cell (def = false)? C&C uses this for BOAT
            bool Theater : 1; // Does it have theater specific imagery (def = false)?
        };
        int Bitfield;
    };
#else
    bool Normalized; // Should the animation speed be regulated/adjusted to appear at a consistent speed (def = false)?
    bool Surface; // Is this animation at ground level (def = false)?
    bool Translucent; // Is this animation translucent in appearence (def = false)?
    bool ATBit1_8; // AltPalette = Does it use an alternate drawing palette (def = false)?
    bool Flamer; // Flamer = Does this animation leave flames after it is gone [e.g., napalm] (def = false)?
    bool Scorch; // Does this animation scorch the ground [e.g., napalm does this] (def = false)?
    bool Crater; // Does this form a crater [e.g., artillery does this] (def = false)?
    bool Sticky; // Sticky = Animation sticks to unit in cell (def = false)? C&C uses this for BOAT
    bool Theater; // Does it have theater specific imagery (def = false)?
#endif
    AnimType Type;
    int field_13D; // coord offset?
    int MidPoint; // middle frame, for logic purposes	//DamageFrame? //LogicFrame?
    fixed Damage;
    int Rate; // Desired animation frames per minute (def = 900) default is 1 frame per logic tick.
    int Start; // Frame to start this animation from.
    int LoopStart; // Beginning frame of loop [if animation loops]
    int LoopEnd; // Last frame of loop [if animation loops] (def=last frame of animation)
    int End; // Frame to end this animation on. This is more like a count of frames rather than a frame number.
    int LoopCount; // Number of times this animation loops before ending (def = 0)? -1 == infinite?
    VocType Report; // Sound effect to play when this animation plays (def = VOC_NONE)
    AnimType
        Next; // Animation to spawn when this animation completes [fire uses this to get smaller over time] (def = ANIM_NONE)
    /* From modenc
    The animation looping control code is pretty confusing. The loop control allows
    to set start/end frames for the first iteration of the loop, and start/end frames for all other iterations together.

    Start defines a 0-based index of the first frame to play on the first iteration.

    End defines the 0-based index of the last frame to play on the first iteration.

    LoopStart defines a 0-based index of the first frame for all following iterations.

    LoopEnd defines a 1-based index of the last frame for all but the first iterations. The first
    iteration will never play frames beyond this one, even if End is set to do so.

    LoopCount defines the total amount of iterations to play.
    */
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
extern TFixedIHeapClass<AnimTypeClass> &AnimTypes;
#else
extern TFixedIHeapClass<AnimTypeClass> AnimTypes;
#endif

#endif // ANIMTYPE_H
