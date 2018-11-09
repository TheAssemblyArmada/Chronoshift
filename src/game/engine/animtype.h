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
#pragma once

#ifndef ANIMTYPE_H
#define ANIMTYPE_H

#include "always.h"
#include "voc.h"
#include "fixed.h"
#include "heap.h"
#include "objecttype.h"
#include "gametypes.h"
#include "theater.h"

class AnimTypeClass : public ObjectTypeClass
{
public:
    AnimTypeClass(AnimType anim, const char *name, int a3, int mid_point, BOOL theater, BOOL normalized, BOOL a7,
        BOOL scorch, BOOL crater, BOOL sticky, BOOL surface, BOOL translucent, BOOL flamer, fixed damage, int rate,
        int start, int loop_start, int loop_end, int end, int loop_count, VocType report, AnimType next);
    AnimTypeClass(const AnimTypeClass &that);
    AnimTypeClass(const NoInitClass &noinit) : ObjectTypeClass(noinit) {}
    ~AnimTypeClass() {}

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    virtual BOOL Create_And_Place(cell_t cellnum, HousesType house = HOUSES_NONE) const override;
    virtual ObjectClass *Create_One_Of(HouseClass *house) const override;

    void Code_Pointers() {}
    void Decode_Pointers() {}

    static void Init_Heap();
    static void One_Time();
    static void Init(TheaterType theater);
    static AnimType From_Name(const char *name);
    static const char *Name_From(AnimType anim);
    static AnimTypeClass &As_Reference(AnimType anim);
    static AnimTypeClass *As_Pointer(AnimType anim);

    AnimType What_Type() const { return m_Type; }

    BOOL Is_Normalized() const { return m_Normalized; }
    BOOL Is_Surface() const { return m_Surface; }
    BOOL Is_Translucent() const { return m_Translucent; }
    BOOL Is_Theater() const { return m_Theater; }

private:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool m_Normalized : 1; // Should the animation speed be adjusted to appear at a consistent speed (def = false)?
            bool m_Surface : 1; // Is this animation at ground level (def = false)?
            bool m_Translucent : 1; // Is this animation translucent in appearence (def = false)?
            bool m_Bit_8 : 1; // AltPalette = Does it use an alternate drawing palette (def = false)?
            bool m_Flamer : 1; // Flamer = Does this animation leave flames after it is gone [e.g., napalm] (def = false)?
            bool m_Scorch : 1; // Does this animation scorch the ground [e.g., napalm does this] (def = false)?
            bool m_Crater : 1; // Does this form a crater [e.g., artillery does this] (def = false)?
            bool m_Sticky : 1; // Sticky = Animation sticks to unit in cell (def = false)? C&C uses this for BOAT
            bool m_Theater : 1; // Does it have theater specific imagery (def = false)?
        };
        int Bitfield;
    };
#else
    bool m_Normalized; // Should the animation speed be regulated/adjusted to appear at a consistent speed (def = false)?
    bool m_Surface; // Is this animation at ground level (def = false)?
    bool m_Translucent; // Is this animation translucent in appearence (def = false)?
    bool m_Bit_8; // AltPalette = Does it use an alternate drawing palette (def = false)?
    bool m_Flamer; // Flamer = Does this animation leave flames after it is gone [e.g., napalm] (def = false)?
    bool m_Scorch; // Does this animation scorch the ground [e.g., napalm does this] (def = false)?
    bool m_Crater; // Does this form a crater [e.g., artillery does this] (def = false)?
    bool m_Sticky; // Sticky = Animation sticks to unit in cell (def = false)? C&C uses this for BOAT
    bool m_Theater; // Does it have theater specific imagery (def = false)?
#endif
    AnimType m_Type;
    int m_field_13D; // coord offset?
    int m_MidPoint; // middle frame, for logic purposes    //DamageFrame? //LogicFrame?
    fixed m_Damage;
    int m_Rate; // Desired animation frames per minute (def = 900) default is 1 frame per logic tick.
    int m_Start; // Frame to start this animation from.
    int m_LoopStart; // Beginning frame of loop [if animation loops]
    int m_LoopEnd; // Last frame of loop [if animation loops] (def=last frame of animation)
    int m_End; // Frame to end this animation on. This is more like a count of frames rather than a frame number.
    int m_LoopCount; // Number of times this animation loops before ending (def = 0)? -1 == infinite?
    VocType m_Report; // Sound effect to play when this animation plays (def = VOC_NONE)
    AnimType m_Next; // Animation to spawn when this animation completes [fire uses this to get smaller over time] (def = ANIM_NONE)

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
