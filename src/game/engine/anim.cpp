/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Anim layer of the object class hierachy.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "anim.h"
#include "target.h"

#ifndef GAME_DLL
TFixedIHeapClass<AnimClass> g_Anims;
#endif

AnimClass::AnimClass(AnimType type, coord_t coord, unsigned char loop_delay, unsigned char loop_count/*, BOOL invisible*/) :
    ObjectClass(RTTI_ANIM, g_Anims.ID(this)),
    m_LoopStage(),
    m_Type(AnimTypeClass::As_Pointer(type)),
    m_AttachedTo(0),
    m_Owner(HOUSES_NONE),
    m_Loops(loop_count),
    m_Bit1(false),
    m_Bit2(true),
    m_Invisible(false /*invisible*/),
    m_LoopDelay(loop_delay),
    m_field_4A()
{
}

AnimClass::AnimClass(const AnimClass &that) :
    m_LoopStage(that.m_LoopStage),
    m_Type(that.m_Type),
    m_AttachedTo(that.m_AttachedTo),
    m_Owner(that.m_Owner),
    m_Loops(that.m_Loops),
    m_Bit1(that.m_Bit1),
    m_Bit2(that.m_Bit2),
    m_Invisible(that.m_Invisible),
    m_LoopDelay(that.m_LoopDelay),
    m_field_4A(that.m_field_4A)
{
}

AnimClass::AnimClass(const NoInitClass &noinit) :
    ObjectClass(noinit),
    m_LoopStage(noinit)
{
}

void *AnimClass::operator new(size_t size)
{
    AnimClass *this_ptr = g_Anims.Alloc();
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = true;
    }
    return this_ptr;
}

void AnimClass::operator delete(void *ptr)
{
    AnimClass *this_ptr = static_cast<AnimClass *>(ptr);
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = false;
    }
    g_Anims.Free(this_ptr);
}

void AnimClass::Init()
{
    g_Anims.Free_All();
}

coord_t AnimClass::Center_Coord() const
{
#ifdef GAME_DLL
    // TODO
    return 0;
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return 0;
#endif
}

void AnimClass::AI()
{
#ifdef GAME_DLL
    void (*func)(AnimClass *) = reinterpret_cast<void (*)(AnimClass *)>(0x00424B94);
    func(this);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
#endif
}

LayerType AnimClass::In_Which_Layer() const
{
    // Infantry corpses need to be rendered below everything else.
    if (What_Type() == ANIM_CORPSE1 || What_Type() == ANIM_CORPSE2 || What_Type() == ANIM_CORPSE3) {
        return LAYER_SURFACE;
    }

    if (m_Type->Is_Surface() || Target_Legal(m_AttachedTo)) {
        return LAYER_GROUND;
    }

    // All other anim types default to AIR.
    return LAYER_AIR;
}

void AnimClass::Detach(target_t target, int a2)
{
#ifdef GAME_DLL
    void (*func)(AnimClass *, target_t, int) = reinterpret_cast<void (*)(AnimClass *, target_t, int)>(0x00425A98);
    func(this, target, a2);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
#endif
}

BOOL AnimClass::Render(BOOL force_render)
{
#ifdef GAME_DLL
    BOOL (*func)(AnimClass *, BOOL) = reinterpret_cast<BOOL (*)(AnimClass *, BOOL)>(0x0042410C);
    return func(this, force_render);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return false;
#endif
}

const int16_t *AnimClass::Occupy_List(BOOL a1) const
{
#ifdef GAME_DLL
    int16_t *(*func)(const AnimClass *, BOOL) = reinterpret_cast<int16_t *(*)(const AnimClass *, BOOL)>(0x004245E8);
    return func(this, a1);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return nullptr;
#endif
}

/*const int16_t *AnimClass::Overlap_List(BOOL a1) const
{
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return nullptr;
}*/

void AnimClass::Draw_It(int x_pos, int y_pos, WindowNumberType window) const
{
#ifdef GAME_DLL
    void (*func)(const AnimClass *, int, int, WindowNumberType) =
        reinterpret_cast<void (*)(const AnimClass *, int, int, WindowNumberType)>(0x0042415C);
    func(this, x_pos, y_pos, window);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
#endif
}

BOOL AnimClass::Mark(MarkType mark)
{
#ifdef GAME_DLL
    BOOL (*func)(AnimClass *, MarkType) = reinterpret_cast<BOOL (*)(AnimClass *, MarkType)>(0x004242D0);
    return func(this, mark);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return false;
#endif
}

coord_t AnimClass::Sort_Y() const
{
#ifdef GAME_DLL
    coord_t (*func)(const AnimClass *) = reinterpret_cast<coord_t (*)(const AnimClass *)>(0x00423F64);
    return func(this);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return 0;
#endif
}

void AnimClass::Code_Pointers()
{
    ObjectClass::Code_Pointers();
}

void AnimClass::Decode_Pointers()
{
    ObjectClass::Decode_Pointers();
}

BOOL AnimClass::Can_Place_Here(coord_t coord) const
{
#ifdef GAME_DLL
    BOOL (*func)(const AnimClass *, coord_t) = reinterpret_cast<BOOL (*)(const AnimClass *, coord_t)>(0x00425C50);
    return func(this, coord);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return false;
#endif
}

const int16_t *AnimClass::Anim_Overlap_List() const
{
#ifdef GAME_DLL
    int16_t *(*func)(const AnimClass *) = reinterpret_cast<int16_t *(*)(const AnimClass *)>(0x00424318);
    return func(this);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return nullptr;
#endif
}

void AnimClass::Attach_To(ObjectClass *object)
{
#ifdef GAME_DLL
    void (*func)(AnimClass *, ObjectClass *) = reinterpret_cast<void (*)(AnimClass *, ObjectClass *)>(0x0042554C);
    func(this, object);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
#endif
}

coord_t AnimClass::Adjust_Coord(coord_t coord)
{
#ifdef GAME_DLL
    // TODO
    return 0;
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return 0;
#endif
}

void AnimClass::Do_Atom_Damage(HousesType house, cell_t cell)
{
#ifdef GAME_DLL
    void (*func)(AnimClass *, HousesType, cell_t) = reinterpret_cast<void (*)(AnimClass *, HousesType, cell_t)>(0x00425AE0);
    func(this, house, cell);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
#endif
}

void AnimClass::Start()
{
#ifdef GAME_DLL
    void (*func)(AnimClass *) = reinterpret_cast<void (*)(AnimClass *)>(0x004256B8);
    func(this);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
#endif
}

void AnimClass::Middle()
{
#ifdef GAME_DLL
    void (*func)(AnimClass *) = reinterpret_cast<void (*)(AnimClass *)>(0x00425748);
    func(this);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
#endif
}
