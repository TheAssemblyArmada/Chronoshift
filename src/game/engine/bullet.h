/**
 * @file
 *
 * @author CCHyper
 *
 * @brief
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef BULLET_H
#define BULLET_H

#include "always.h"
#include "bullettype.h"
#include "fly.h"
#include "fuse.h"
#include "object.h"

class TechnoClass;

class BulletClass : public ObjectClass
{
public:
    BulletClass(BulletType bullet, target_t target, TechnoClass *payback, int speed, WarheadType warhead, int health);
    BulletClass(const BulletClass &that);
    BulletClass(const NoInitClass &noinit) : ObjectClass(noinit), m_Flyer(noinit), m_Fuse(noinit), m_Facing(noinit) {}
    virtual ~BulletClass();

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    // ObjectClass.
    virtual coord_t Target_Coord() const final;
    virtual void AI() final;
    virtual LayerType In_Which_Layer() const final;
    virtual BulletTypeClass const &Class_Of() const final { return *m_Class; }
    virtual coord_t Sort_Y() const final;
    virtual BOOL Unlimbo(coord_t coord, DirType dir = DIR_NORTH) final;
    virtual void Detach(target_t target, BOOL a2 = false) final;
    virtual const int16_t *Occupy_List(BOOL a1 = false) const final;

#ifdef CHRONOSHIFT_STANDALONE
    virtual const int16_t *Overlap_List(BOOL a1 = false) const final { return Occupy_List(); }
#endif

    virtual void Draw_It(int x, int y, WindowNumberType window) const final;
    virtual BOOL Mark(MarkType mark) final;
    virtual void Code_Pointers() final;
    virtual void Decode_Pointers() final;

    // BulletClass.
    virtual void Assign_Target(target_t new_target) final { m_Target = new_target; }

    // TODO: This is an error in the original code, should be overriding ObjectClass Overlap_List correctly.
    // Remove this version and unwrap version above when standalone.
#ifndef CHRONOSHIFT_STANDALONE
    virtual const int16_t *Overlap_List() const { return Occupy_List(); }
#endif

    BOOL Homes_In() const { return Class_Of().Rate_Of_Turn() > 0; } // Does this bullet home in on its target?

    BulletType What_Type() const { return m_Class->What_Type(); }

    static void Init();

private:
    int Shape_Number() const;
    void Do_Map_Reveal();
    BOOL Is_Forced_To_Explode(coord_t &at) const;
    void Bullet_Explodes(BOOL a1 = false);

private:
    FlyClass m_Flyer;
    FuseClass m_Fuse;
    GamePtr<BulletTypeClass> m_Class;
    TechnoClass *m_Payback; // the object that fired us.
    FacingClass m_Facing;

#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_BBit1 : 1; // 1
    BOOL m_ToSpawnTrailer : 1; // 2
    BOOL m_BBit4 : 1; // 4
#else
    bool m_BBit1;
    bool m_ToSpawnTrailer; // am i flagged to spawn a trailer animation?
    bool m_BBit4;
#endif
    target_t m_Target;
    int32_t m_Speed;
    WarheadType m_Warhead;

#ifdef GAME_DLL
public:
    friend void Setup_Hooks();

    BulletClass *Hook_Ctor(BulletType bullet, target_t target, TechnoClass *payback, int speed, WarheadType warhead, int health) { return new (this) BulletClass(bullet, target, payback, speed, warhead, health); }
    void Hook_Dtor() { BulletClass::~BulletClass(); }
    const int16_t *Hook_Occupy_List() { return BulletClass::Occupy_List(); }
    void Hook_Draw_It(int x, int y, WindowNumberType window) { BulletClass::Draw_It(x, y, window); }
    coord_t Hook_Target_Coord() { return BulletClass::Target_Coord(); }
    coord_t Hook_Sort_Y() { return BulletClass::Sort_Y(); }
    LayerType Hook_In_Which_Layer() { return BulletClass::In_Which_Layer(); }
    const int16_t *Hook_Overlap_List() { return BulletClass::Overlap_List(); }
#endif
};

#ifdef GAME_DLL
extern TFixedIHeapClass<BulletClass> &g_Bullets;
#else
extern TFixedIHeapClass<BulletClass> g_Bullets;
#endif

#endif // BULLET_H
