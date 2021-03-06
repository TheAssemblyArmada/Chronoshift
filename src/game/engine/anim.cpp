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
#include "cell.h"
#include "drawshape.h"
#include "gameoptions.h"
#include "house.h"
#include "iomap.h"
#include "lists.h"
#include "rules.h"
#include "target.h"
#include "smudge.h"

#ifndef GAME_DLL
TFixedIHeapClass<AnimClass> g_Anims;
#endif

/**
 * @brief
 *
 * @address 0x00424648
 */
AnimClass::AnimClass(AnimType type, coord_t coord, unsigned char loop_delay, unsigned char loop_count) :
    ObjectClass(RTTI_ANIM, g_Anims.ID(this)),
    m_LoopStage(),
    m_Class(g_AnimTypes.Ptr(type)),
    m_AttachedTo(0),
    m_Owner(HOUSES_NONE),
    m_Loops(loop_count),
    m_Bit1(false),
    m_Bit2(true),
    m_Invisible(false),
    m_LoopDelay(loop_delay),
    m_field_4A(0)
{
    if (m_Class->Get_End() == -1) {
        // seems like this if once had Theater specific art access
        // g_IsTheaterShape = m_Class->Is_Theater();
        m_Class->Set_End(Get_Build_Frame_Count(m_Class->Get_Image_Data()));
        // g_IsTheaterShape = false;
    }
    if (m_Class->Get_Loop_End() == -1) {
        m_Class->Set_End(m_Class->Get_End());
    }
    if (m_Class->Is_Normalized()) {
        m_LoopStage.Set_Delay(g_Options.Normalize_Delay(m_Class->Get_Rate()));
    } else {
        m_LoopStage.Set_Delay(m_Class->Get_Rate());
    }
    m_LoopStage.Set_Stage(0);
    if (m_Class->Is_Surface()) {
        Set_Height(256);
    }
    Unlimbo(Adjust_Coord(coord));
    if (What_Type() == ANIM_LZ_SMOKE) {
        g_Map.Sight_From(Coord_To_Cell(coord), g_Rule.Drop_Zone_Radius(), g_PlayerPtr, false);
    }
    // something here has been badly removed, this is how the code is in edwin and RA
    m_Loops = m_Class->Get_Loop_Count() * std::max<uint8_t>(loop_count, 1);
    m_Loops = std::max<uint8_t>(m_Loops, 1); // thats what it is...
    //
    if (m_LoopDelay == 0) {
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

AnimClass::~AnimClass()
{
    if (g_GameActive) {
        ObjectClass *optr = As_Object(m_AttachedTo);
        if (Target_Legal(m_AttachedTo) && optr != nullptr) {
            g_Map.Remove(this, In_Which_Layer());

            int i;
            for (i = 0; i < g_Anims.Count(); ++i) {
                AnimClass *aptr = &g_Anims[i];
                if (aptr != this && aptr->Attached_To() == m_AttachedTo) {
                    break;
                }
            }
            if (i == g_Anims.Count()) {
                optr->Fire_Out();
                optr->Mark(MARK_5);
                optr->Set_AnimAttached(false);
                optr->Mark(MARK_4);
            }

            m_AttachedTo = 0;
            m_Coord = Coord_Add(m_Coord, optr->Center_Coord());
        }
        Limbo();
    }
    m_AttachedTo = 0;
    m_Class = nullptr;
}

/**
 * @brief
 *
 * @address 0x00424608
 */
void *AnimClass::operator new(size_t size)
{
    AnimClass *this_ptr = g_Anims.Alloc();
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
    captainslog_assert(this_ptr != nullptr);
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
    captainslog_dbgassert(false, "Unimplemented function called!\n");
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
void AnimClass::Detach(target_t target, BOOL a2)
{
    if (target == m_AttachedTo && a2) {
        g_Map.Remove(this, In_Which_Layer());
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
    if (g_Map[Center_Cell()].Is_Revealed()) {
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
        g_IsTheaterShape = m_Class->Is_Theater();
        void *shape = Get_Image_Data();
        if (shape != nullptr) {
            uint8_t *trans_table = nullptr;
            int frame = m_Class->Get_Start() + m_LoopStage.Get_Stage();
            if (m_Class->Is_Bit_8()) {
                trans_table = DisplayClass::s_WhiteTranslucentTable[0];
            }
            if (trans_table == nullptr && m_Class->Is_Translucent()) {
                trans_table = DisplayClass::s_TranslucentTable[0];
            }
            if (What_Type() == ANIM_ATOMSFX) {
                trans_table = DisplayClass::s_UnitShadow[0];
            }
            ShapeFlags flags = SHAPE_CENTER | SHAPE_VIEWPORT_REL;
            if (trans_table != nullptr) {
                flags |= SHAPE_GHOST;
            }
            CC_Draw_Shape(shape, frame, x_pos, y_pos, window, flags, nullptr, trans_table);
        }
        g_IsTheaterShape = false;
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
    g_Map.SidebarClass::Refresh_Cells(Center_Cell(), Anim_Overlap_List());
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
    captainslog_dbgassert(false, "Unimplemented function called!\n");
    return nullptr;
#endif
}

/**
 *
 *
 */
void AnimClass::Attach_To(ObjectClass *object)
{
    if (object != nullptr) {
        object->Mark(MARK_5);
        object->Set_AnimAttached(true);
        object->Mark(MARK_4);

        g_Map.Remove(this, In_Which_Layer());
        m_AttachedTo = object->As_Target();
        g_Map.Submit(this, In_Which_Layer());

        m_Coord = Coord_Subtract(m_Coord, object->Target_Coord());
    }
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
    captainslog_dbgassert(false, "Unimplemented function called!\n");
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

/**
 *
 *
 */
void AnimClass::Middle()
{
    cell_t cellnum = Get_Cell();
    CellClass &cell = g_Map[cellnum];

    if (What_Type() == ANIM_ATOMSFX) {
        AnimClass::Do_Atom_Damage(m_Owner, cellnum);
    }

    if (Class_Of().Scorches_Ground()) {
        SmudgeClass *sptr = new SmudgeClass((SmudgeType)g_Scen.Get_Random_Value(SMUDGE_SC1, SMUDGE_SC6), Center_Coord());
        captainslog_assert(sptr != nullptr);

    }

    if (Class_Of().Forms_Crater()) {
        cell.Reduce_Ore(6);
        SmudgeClass *sptr = new SmudgeClass(SMUDGE_CR1, Center_Coord());
        captainslog_assert(sptr != nullptr);
    }

    // aptr asserts are commented out cause this chunk causes common heap overruns

    switch (What_Type()) {
        case ANIM_NAPALM1:
        case ANIM_NAPALM2:
        case ANIM_NAPALM3: {
            coord_t coord = g_Map.Closest_Free_Spot(Coord_Scatter(Center_Coord(), 64, false), true);
            AnimClass *aptr = new AnimClass(ANIM_FIRE3, coord, 0, g_Scen.Get_Random_Value(1, 2));
            //captainslog_assert(aptr != nullptr);

            if (g_Scen.Check_Random_Chance(50)) {
                coord_t coord = g_Map.Closest_Free_Spot(Coord_Scatter(Center_Coord(), 160, false), true);
                AnimClass *aptr = new AnimClass(ANIM_FIRE3, coord, 0, g_Scen.Get_Random_Value(1, 2));
                //captainslog_assert(aptr != nullptr);
            }

            if (g_Scen.Check_Random_Chance(50)) {
                coord_t coord = g_Map.Closest_Free_Spot(Coord_Scatter(Center_Coord(), 112, false), true);
                AnimClass *aptr = new AnimClass(ANIM_FIRE2, coord, 0, g_Scen.Get_Random_Value(1, 2));
                //captainslog_assert(aptr != nullptr);
            }

            break;
        }
        case ANIM_FIRE2:
        case ANIM_FIRE1: {
            AnimClass *aptr = new AnimClass(ANIM_FIRE3, Center_Coord(), 0, g_Scen.Get_Random_Value(1, 2));
            //captainslog_assert(aptr != nullptr);

            if (aptr != nullptr) {
                if (Target_Legal(m_AttachedTo)) {
                    aptr->Attach_To(As_Object(m_AttachedTo));
                }
            }
            break;
        }
        default:
            break;
    }
}
