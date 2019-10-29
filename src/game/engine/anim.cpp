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
#include "drawshape.h"
#include "gameoptions.h"
#include "house.h"
#include "iomap.h"
#include "lists.h"
#include "rules.h"
#include "target.h"


#ifndef GAME_DLL
TFixedIHeapClass<AnimClass> g_Anims;
#endif

/**
 * @brief
 *
 * @address 0x00424648
 */
AnimClass::AnimClass(AnimType type, coord_t coord, unsigned char loop_delay, unsigned char loop_count /*, BOOL invisible*/) :
    ObjectClass(RTTI_ANIM, g_Anims.ID(this)),
    m_LoopStage(),
    m_Class(g_AnimTypes.Ptr(type)),
    m_AttachedTo(0),
    m_Owner(HOUSES_NONE),
    m_Loops(loop_count),
    m_Bit1(false),
    m_Bit2(true),
    m_Invisible(false /*invisible*/),
    m_LoopDelay(loop_delay),
    m_field_4A()
{
    if (m_Class->Get_End() == -1) {
        // seems like this if once had Theater specific art access
        // g_isTheaterShape = m_Class->Is_Theater();
        m_Class->Set_End(Get_Build_Frame_Count(m_Class->Get_Image_Data()));
        // g_isTheaterShape = false;
    }
    if (m_Class->Get_Loop_End() == -1) {
        m_Class->Set_End(m_Class->Get_End());
    }
    if (m_Class->Is_Normalized()) {
        m_LoopStage.Set_Delay(Options.Normalize_Delay(m_Class->Get_Rate()));
    } else {
        m_LoopStage.Set_Delay(m_Class->Get_Rate());
    }
    m_LoopStage.Set_Stage(0);
    if (m_Class->Is_Surface()) {
        Set_Height(256);
    }
    Unlimbo(Adjust_Coord(coord));
    if (What_Type() == ANIM_LZ_SMOKE) {
        Map.Sight_From(Coord_To_Cell(coord), Rule.Drop_Zone_Radius(), g_PlayerPtr, false);
    }
    // something here has been badly removed, this is how the code is in edwin and RA
    m_Loops = m_Class->Get_Loop_Count() * std::max<uint8_t>(loop_count, 1);
    m_Loops = std::max<uint8_t>(m_Loops, 1); // thats what it is...
    //
    if (m_LoopDelay > 0) {
        Start();
    }
}

AnimClass::AnimClass(const AnimClass &that) :
    m_LoopStage(that.m_LoopStage),
    m_Class(that.m_Class),
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

/**
 * @brief
 *
 * @address 0x00424608
 */
void *AnimClass::operator new(size_t size)
{
    AnimClass *this_ptr = g_Anims.Alloc();
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = true;
    }
    return this_ptr;
}

/**
 * @brief
 *
 * @address 0x00424628
 */
void AnimClass::operator delete(void *ptr)
{
    AnimClass *this_ptr = static_cast<AnimClass *>(ptr);
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = false;
    }
    g_Anims.Free(this_ptr);
}

/**
 * @brief
 *
 * @address 0x004245F4
 */
void AnimClass::Init()
{
    g_Anims.Free_All();
}

/**
 * @brief
 *
 * @address 0x004240C0
 */
coord_t AnimClass::Center_Coord() const
{
    if (Target_Legal(m_AttachedTo)) {
        ObjectClass *objptr = As_Object(m_AttachedTo);
        if (objptr != nullptr) {
            return Coord_Add(m_Coord, objptr->Target_Coord());
        }
    }
    return m_Coord;
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

/**
 * @brief
 *
 * @address 0x00425610
 */
LayerType AnimClass::In_Which_Layer() const
{
    // Infantry corpses need to be rendered below everything else.
    if (What_Type() == ANIM_CORPSE1 || What_Type() == ANIM_CORPSE2 || What_Type() == ANIM_CORPSE3) {
        return LAYER_SURFACE;
    }

    if (Target_Legal(m_AttachedTo) || m_Class->Is_Surface()) {
        return LAYER_GROUND;
    }

    // All other anim types default to AIR.
    return LAYER_AIR;
}

/**
 * @brief
 *
 * @address 0x00425A98
 */
void AnimClass::Detach(target_t target, int a2)
{
    // TS doesn't check a2, YR checks target != 0 in place of a2...
    if (target == m_AttachedTo && a2) {
        Map.Remove(this, In_Which_Layer());
        m_AttachedTo = 0;
        m_Bit1 = true;
        Mark(MARK_REMOVE);
    }
}

/**
 * @brief
 *
 * @address 0x0042410C
 */
BOOL AnimClass::Render(BOOL force_render)
{
    if (m_LoopDelay) {
        return false;
    }
    if (Map[Center_Cell()].Is_Revealed()) {
        m_ToDisplay = true;
    }

    return ObjectClass::Render(force_render);
}

/**
 * @brief
 *
 * @address 0x004245E8
 */
const int16_t *AnimClass::Occupy_List(BOOL a1) const
{
    static const int16_t _simple[] = { 0, LIST_END };
    return _simple;
}

/**
 * @brief
 *
 * @address 0x0042415C
 */
void AnimClass::Draw_It(int x_pos, int y_pos, WindowNumberType window) const
{
    if (!m_Invisible) {
        g_isTheaterShape = m_Class->Is_Theater();
        void *shape = Get_Image_Data();
        if (shape != nullptr) {
            uint8_t *trans_table = nullptr;
            int frame = m_Class->Get_Start() + m_LoopStage.Get_Stage();
            if (m_Class->Is_Bit_8()) {
                trans_table = DisplayClass::WhiteTranslucentTable[0];
            }
            if (trans_table == nullptr && m_Class->Is_Translucent()) {
                trans_table = DisplayClass::TranslucentTable[0];
            }
            if (What_Type() == ANIM_ATOMSFX) {
                trans_table = DisplayClass::UnitShadow[0];
            }
            ShapeFlags flags = SHAPE_CENTER | SHAPE_VIEWPORT_REL;
            if (trans_table != nullptr) {
                flags |= SHAPE_GHOST;
            }
            CC_Draw_Shape(shape, frame, x_pos, y_pos, window, flags, nullptr, trans_table);
        }
        g_isTheaterShape = false;
    }
}

/**
 * @brief
 *
 * @address 0x004242D0
 */
BOOL AnimClass::Mark(MarkType mark)
{
    if (!ObjectClass::Mark(mark)) {
        return false;
    }
    Map.SidebarClass::Refresh_Cells(Center_Cell(), Anim_Overlap_List());
    return true;
}

/**
 * @brief
 *
 * @address 0x00423F64
 */
coord_t AnimClass::Sort_Y() const
{
    if (m_AttachedTo) {
        // TODO, can 0x10000 be represented in pixels or leptons?
        return Coord_Add(As_Object(m_AttachedTo)->Sort_Y(), 0x10000);
    }
    if (What_Type() == ANIM_MOVEFLASH) {
        return Coord_Add(Center_Coord(), Coord_From_Pixel_XY(0, -24));
    }
    if (m_Class->Is_Surface() || What_Type() == ANIM_LZ_SMOKE) {
        return Coord_Add(Center_Coord(), Coord_From_Pixel_XY(0, 14));
    }
    return m_Coord;
}

void AnimClass::Code_Pointers()
{
    ObjectClass::Code_Pointers();
}

void AnimClass::Decode_Pointers()
{
    ObjectClass::Decode_Pointers();
}

/**
 * @brief
 *
 * @address 0x00425C50
 */
BOOL AnimClass::Can_Place_Here(coord_t coord) const
{
    return true;
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

/**
 * @brief
 *
 * @address inlined
 */
coord_t AnimClass::Adjust_Coord(coord_t coord)
{
    // In C&C this function adjusted the coord for Nod Temple silo door.
    return coord;
}

void AnimClass::Do_Atom_Damage(HousesType house, cell_t cell)
{
#ifdef GAME_DLL
    void (*func)(HousesType, cell_t) = reinterpret_cast<void (*)(HousesType, cell_t)>(0x00425AE0);
    func(house, cell);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
#endif
}

/**
 * @brief
 *
 * @address 0x004256B8
 */
void AnimClass::Start()
{
    Mark(MARK_REDRAW);
    Sound_Effect(m_Class->Get_Report(), m_Coord);
    if (m_Class->Get_MidPoint() == 0) {
        Middle();
    }
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
