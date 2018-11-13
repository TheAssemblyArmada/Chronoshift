/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Techno layer of the object class hierachy.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef ANIM_H
#define ANIM_H

#include "always.h"
#include "object.h"
#include "stage.h"
#include "gameptr.h"
#include "animtype.h"

class AnimClass : public ObjectClass
{
public:
    AnimClass(AnimType type, coord_t coord, unsigned char loop_delay = 0, unsigned char loop_count = 1/*, BOOL invisible = false*/);
    AnimClass(const AnimClass &that);
    AnimClass(const NoInitClass &noinit);
    virtual ~AnimClass() {}

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    static void Init();

    // AbstractClass
    virtual coord_t Center_Coord() const override;
    virtual void AI() override;

    // ObjectClass
    virtual LayerType In_Which_Layer() const override;
    virtual const AnimTypeClass &Class_Of() const override { return *m_Type; }
    virtual void Detach(target_t target, int a2) override;
    virtual BOOL Render(BOOL force_render = false) override;
    virtual const int16_t *Occupy_List(BOOL a1 = false) const override;
    //virtual const int16_t *Overlap_List(BOOL a1 = false) const override; // Move "Anim_Overlap_List" function below to this function.
    virtual void Draw_It(int x_pos, int y_pos, WindowNumberType window) const override;
    virtual BOOL Mark(MarkType mark) override;
    virtual coord_t Sort_Y() const override;
    virtual void Code_Pointers() override;
    virtual void Decode_Pointers() override;

    // AnimClass
    virtual BOOL Can_Place_Here(coord_t coord) const;
    virtual const int16_t *Anim_Overlap_List() const; // Was "Overlap_List", renamed for now due to conflict.

    void Attach_To(ObjectClass *object);
    coord_t Adjust_Coord(coord_t coord);
    void Do_Atom_Damage(HousesType house, cell_t cell);

    unsigned char Get_Loops() const { return m_Loops; }
    void Make_Invisible() { m_Invisible = true; }
    BOOL Is_Invisible() const { return m_Invisible; }
    unsigned int Get_Loop_Delay() const { return m_LoopDelay; }

    AnimType What_Type() const { return m_Type->What_Type(); }

private:
    void Start();
    void Middle();

protected:
    StageClass m_LoopStage;
    GamePtr<AnimTypeClass> m_Type;
    target_t m_AttachedTo;
    HousesType m_Owner;
    unsigned char m_Loops;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool m_Bit1 : 1; // 1
            bool m_Bit2 : 1; // 2
            bool m_Invisible : 1; // 4
        };
        int m_Bitfield;
    };
#else
    bool m_Bit1; // to remove / delete?
    bool m_Bit2; // skip process once?
    bool m_Invisible; // Is the anim invisible [and thus not rendered]?
#endif
    unsigned int m_LoopDelay;
    fixed_t m_field_4A;

public:
#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
    static void Hook_Me()
    {
    #ifdef COMPILER_WATCOM
    #endif
    }
#endif

};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
extern TFixedIHeapClass<AnimClass> &g_Anims;
#else
extern TFixedIHeapClass<AnimClass> g_Anims;
#endif

#endif // ANIM_H
