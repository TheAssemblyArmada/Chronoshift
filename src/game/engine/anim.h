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
    AnimClass(AnimType type, coord_t coord, unsigned char loop_delay = 0, unsigned char loop_count = 1);
    AnimClass(const AnimClass &that);
    AnimClass(const NoInitClass &noinit);
    virtual ~AnimClass();

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
    virtual const AnimTypeClass &Class_Of() const override { return *m_Class; }
    virtual void Detach(target_t target, int a2) override;
    virtual BOOL Render(BOOL force_render = false) override;
    virtual const int16_t *Occupy_List(BOOL a1 = false) const override;
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
    static void Do_Atom_Damage(HousesType house, cell_t cellnum);

    target_t Attached_To() const { return m_AttachedTo; }
    unsigned char Get_Loops() const { return m_Loops; }
    void Set_Loops(unsigned char loops) { m_Loops = loops; }
    void Make_Invisible() { m_Invisible = true; }
    BOOL Is_Invisible() const { return m_Invisible; }
    unsigned int Get_Loop_Delay() const { return m_LoopDelay; }

    AnimType What_Type() const { return m_Class->What_Type(); }

#ifdef GAME_DLL
    friend void Setup_Hooks();

    coord_t Wrap_Center_Coord()
    {
        return AnimClass::Center_Coord();
    }
    LayerType Wrap_In_Which_Layer()
    {
        return AnimClass::In_Which_Layer();
    }
    const int16_t *Wrap_Occupy_List(BOOL a1)
    {
        return AnimClass::Occupy_List(a1);
    }
    void Wrap_Draw_It(int x_pos, int y_pos, WindowNumberType window)
    {
        AnimClass::Draw_It(x_pos, y_pos, window);
    }
    coord_t Wrap_Sort_Y()
    {
        return AnimClass::Sort_Y();
    }
#endif

private:
    void Start();
    void Middle();

protected:
    StageClass m_LoopStage;
    GamePtr<AnimTypeClass> m_Class;
    target_t m_AttachedTo;
    HousesType m_Owner;
    unsigned char m_Loops;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_Bit1 : 1; // 1
    BOOL m_Bit2 : 1; // 2
    BOOL m_Invisible : 1; // 4
#else
    bool m_Bit1; // to remove / delete?
    bool m_Bit2; // skip process once?
    bool m_Invisible; // Is the anim invisible [and thus not rendered]?
#endif
    unsigned int m_LoopDelay;
    fixed_t m_field_4A;
};

#ifdef GAME_DLL
extern TFixedIHeapClass<AnimClass> &g_Anims;
#else
extern TFixedIHeapClass<AnimClass> g_Anims;
#endif

#endif // ANIM_H
