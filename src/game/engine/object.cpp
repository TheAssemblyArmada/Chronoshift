/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base class for most ingame object instances.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "object.h"
#include "coord.h"
#include "infantry.h"
#include "iomap.h"
#include "rules.h"
#include "logic.h"
#include "techno.h"
#include "house.h"
#include "globals.h"
#include "session.h"
#include "anim.h"
#include "special.h"
#include "tracker.h"
#include "foot.h"
#include "target.h"
#include <algorithm>

#ifndef GAME_DLL
DynamicVectorClass<ObjectClass*> CurrentObjects;
#endif

ObjectClass::ObjectClass() :
    m_IsDown(false),
    m_ToDamage(false),
    m_ToDisplay(false),
    m_InLimbo(true),
    m_Selected(false),
    m_AnimAttached(false),
    m_IsFalling(false),
    m_OBit1_128(false),
    m_FallingHeight(0),
    m_Next(nullptr),
    m_AttachedTrigger(nullptr),
    m_Health(255)
{
}

/**
 * @brief
 *
 * @address 0x0051D750
 */
ObjectClass::ObjectClass(RTTIType type, int id) :
    AbstractClass(type, id),
    m_IsDown(false),
    m_ToDamage(false),
    m_ToDisplay(false),
    m_InLimbo(true),
    m_Selected(false),
    m_AnimAttached(false),
    m_IsFalling(false),
    m_OBit1_128(false),
    m_FallingHeight(0),
    m_Next(nullptr),
    m_AttachedTrigger(nullptr),
    m_Health(255)
{
}

ObjectClass::ObjectClass(ObjectClass const &that) :
    AbstractClass(that),
    m_IsDown(that.m_IsDown),
    m_ToDamage(that.m_ToDamage),
    m_ToDisplay(that.m_ToDisplay),
    m_InLimbo(that.m_InLimbo),
    m_Selected(that.m_Selected),
    m_AnimAttached(that.m_AnimAttached),
    m_IsFalling(that.m_IsFalling),
    m_OBit1_128(that.m_OBit1_128),
    m_FallingHeight(that.m_FallingHeight),
    m_Next(that.m_Next),
    m_AttachedTrigger(that.m_AttachedTrigger),
    m_Health(that.m_Health)
{
}

/**
 * @brief
 *
 * @address 0x00423D30
 */
ObjectClass::~ObjectClass()
{
    m_Next = nullptr;
}

#ifdef CHRONOSHIFT_DEBUG
void ObjectClass::Debug_Dump(MonoClass *mono) const
{
    AbstractClass::Debug_Dump(mono);
}
#endif

/**
 * @brief
 *
 * @address 0x0051D7CC
 */
const char *ObjectClass::Name() const
{
    return Class_Of().Get_Name();
}

/**
 * @brief
 *
 * @address 0x0051D988
 */
coord_t ObjectClass::Target_Coord() const
{
    coord_t coord = Center_Coord();

    return Coord_From_Lepton_XY(Coord_Lepton_X(coord), Coord_Lepton_Y(coord) - Get_Height());
}

/**
 *
 *
 */
void ObjectClass::AI()
{
    AbstractClass::AI();

    if (m_IsFalling) {
        LayerType layer = In_Which_Layer();

        m_Height += m_FallingHeight;

        if (m_Height <= 0) {
            m_Height = 0;
            m_IsFalling = false;
            Per_Cell_Process(PCP_2);
            Shorten_Attached_Anims();
        }

        if (m_AnimAttached) {
            --m_FallingHeight;
            m_FallingHeight = std::max(m_FallingHeight, -3);
        } else {
            m_FallingHeight -= g_Rule.Get_Gravity();
            m_FallingHeight = std::max(m_FallingHeight, -100);
        }

        if (layer != In_Which_Layer()) {
            g_Map.Remove(this, layer);
            g_Map.Submit(this, In_Which_Layer());

            if (Class_Of().Get_Bit128()) {
                if (In_Which_Layer() == LAYER_GROUND) {
                    g_Map.Place_Down(Center_Cell(), this);
                } else {
                    g_Map.Pick_Up(Center_Cell(), this);
                }
            }
        }
    }
}

/**
 * @brief
 *
 * @address 0x0051E5C0
 */
BOOL ObjectClass::Limbo()
{
    if (!g_GameActive || m_InLimbo) {
        return false;
    }
    Unselect();
    Detach_All();
    Mark(MARK_REMOVE);
    g_Map.Remove(this, In_Which_Layer());

    if (Class_Of().Get_Bit64()) {
        g_Logic.Remove(this);
    }

    Hidden();
    m_InLimbo = true;
    m_ToDisplay = false;
    return true;
}

BOOL ObjectClass::Unlimbo(coord_t coord, DirType dir)
{
#ifdef GAME_DLL
    BOOL (*func)
    (ObjectClass *, coord_t, DirType) = reinterpret_cast<BOOL (*)(ObjectClass *, coord_t, DirType)>(0x0051DE9C);
    return func(this, coord, dir);
#else
    // Needs IOMapClass and g_Logic
    cell_t cell = Coord_To_Cell(coord);

    if (g_GameActive && m_InLimbo && !m_IsDown && (g_ScenarioInit || Can_Enter_Cell(cell) == MOVE_OK)) {

        m_InLimbo = false;
        m_ToDisplay = false;

        Set_Coord(Class_Of().Coord_Fixup(coord));

        if (Mark(MARK_PUT)) {
            if (m_IsActive) {
                if (In_Which_Layer() != LAYER_NONE) {
                    g_Map.Submit(this, In_Which_Layer());
                }

                if (Class_Of().Get_Bit64()) {
                    g_Logic.Submit(this);
                }
            }

            return true;
        }
    }

    return false;
#endif
}

/**
 * @brief
 *
 * @address 0x0051DDC0
 */
void ObjectClass::Detach(target_t target, BOOL a2)
{
    if (m_AttachedTrigger != nullptr && Target_Get_RTTI(target) == RTTI_TRIGGER) {
        if (m_AttachedTrigger->As_Target() == target) {
            Attach_Trigger(nullptr);
        }
    }
}

/**
 * @brief
 *
 * @address 0x0051DFDC
 */
void ObjectClass::Detach_All(BOOL a1)
{
    if (a1 || Owner() != g_PlayerPtr->What_Type()) {
        Unselect();
    }

    Detach_This_From_All(As_Target(), a1);
}

/**
 * @brief
 *
 * @address 0x0051E5C0
 */
BOOL ObjectClass::Paradrop(coord_t coord)
{
    m_Height = 256;
    m_IsFalling = true;

    if (!Unlimbo(coord, DIR_SOUTH)) {
        return false;
    }
    coord_t anim_coord = Coord_Move(Center_Coord(), DIR_NORTH, m_Height + 48);
    AnimClass *anim = new AnimClass((What_Am_I() == RTTI_BULLET ? ANIM_PARABOMB : ANIM_PARACHUTE), anim_coord);
    if (anim != nullptr) {
        anim->Attach_To(this);
    }
    return true;
}

BOOL ObjectClass::Render(BOOL force_render)
{
/*#ifdef GAME_DLL
    BOOL(*func)(ObjectClass *, BOOL) = reinterpret_cast<BOOL(*)(ObjectClass *, BOOL)>(0x0051DD34);
    return func(this, force_render);
#else*/
    captainslog_assert(m_IsActive);

    coord_t render_coord = Render_Coord();

    if (!g_InMapEditor && !g_DebugUnshroud) {

        if (!force_render && !m_ToDisplay) {
            return false;
        }

        if (!m_IsDown || m_InLimbo) {
            return false;
        }

    }

    m_ToDisplay = false;

    int render_x_pos = 0;
    int render_y_pos = 0;

    if (g_Map.Coord_To_Pixel(render_coord, render_x_pos, render_y_pos)) {

        Draw_It(render_x_pos, render_y_pos, WINDOW_TACTICAL);

        //if (/*g_Debug_Head_To*/) {

            if (m_Selected) {

                if (Is_Foot()) {

                    FootClass *foot_thisptr = reinterpret_cast<FootClass *>(this);

                    if (foot_thisptr->Head_To()) {

                        int my_x_pos = 0;
                        int my_y_pos = 0;

                        int headto_x_pos = 0;
                        int headto_y_pos = 0;

                        int pixel_size = 3;

                        // Draw object coord pixel.
                        g_Map.Coord_To_Pixel(foot_thisptr->m_Coord, my_x_pos, my_y_pos);
                        my_y_pos += 16;
                        g_LogicPage->Put_Fat_Pixel(my_x_pos - 1, my_y_pos - 1, pixel_size, COLOR_YELLOW);

                        // Draw HeadTo coord pixel.
                        g_Map.Coord_To_Pixel(foot_thisptr->Head_To(), headto_x_pos, headto_y_pos);
                        headto_y_pos += 16;
                        g_LogicPage->Put_Fat_Pixel(headto_x_pos - 1, headto_y_pos - 1, pixel_size, COLOR_YELLOW);

                        // Draw connection line.
                        g_LogicPage->Draw_Line(my_x_pos, my_y_pos, headto_x_pos, headto_y_pos, COLOR_YELLOW);

                        //g_Map.Flag_To_Redraw(true);

                    }

                }

            }

        //}

        //if (/*s_Special.Bit3_1 || g_Debug_Head_To*/) {

            if (m_Selected) {

                if (Is_Foot()) {

                    FootClass *foot_thisptr = reinterpret_cast<FootClass *>(this);

                    if (foot_thisptr->Head_To()) {

                        if (foot_thisptr->Get_Path_Facing(0) != FACING_NONE) {

                            /*int path_x_pos = 0;
                            int path_y_pos = 0;

                            int path2_x_pos = 0;
                            int path2_y_pos = 0;

                            int pixel_size = 3;

                            for (int index = 0; index < ARRAY_SIZE(foot_thisptr->m_Paths); ++index) {

                                if (foot_thisptr->m_Paths[index] == FACING_NONE) {
                                    break;
                                }

                                cell_t adjcell = Coord_To_Cell(foot_thisptr->Head_To()) + AdjacentCell[foot_thisptr->m_Paths[index]];
                                coord_t coord = Cell_To_Coord(adjcell);

                                if (g_Map.Coord_To_Pixel(coord, path2_x_pos, path2_y_pos) )  {

                                    path2_y_pos += 16;

                                    Fancy_Text_Print()
                                    g_LogicPage->Draw_Line(path_x_pos, path_y_pos, path2_x_pos, path2_y_pos, COLOR_YELLOW);
                                    g_LogicPage->Put_Fat_Pixel(path2_x_pos - 2, path2_y_pos - 2, pixel_size, COLOR_GREEN);

                                }

                                path_x_pos = path2_x_pos;
                                path_y_pos = path2_y_pos;

                            }

                            g_Map.Coord_To_Pixel(foot_thisptr->m_Coord, path_x_pos, path_y_pos);

                            path_y_pos += 16;

                            g_LogicPage->Put_Fat_Pixel(path_x_pos - 1, path_y_pos - 1, pixel_size, COLOR_PINK);*/

                            //g_Map.Flag_To_Redraw(true);

                        }

                    }

                }

            }

        //}

        //if (/*g_Debug_Nav_Com*/) {

            if (m_Selected) {

                if (Is_Foot()) {

                    FootClass *foot_thisptr = reinterpret_cast<FootClass *>(this);

                    if (Target_Legal(foot_thisptr->Nav_Com())) {

                        int my_x_pos = 0;
                        int my_y_pos = 0;

                        int navcom_x_pos = 0;
                        int navcom_y_pos = 0;

                        int pixel_size = 3;

                        // Draw object coord pixel.
                        g_Map.Coord_To_Pixel(foot_thisptr->m_Coord, my_x_pos, my_y_pos);
                        my_y_pos += 16;
                        g_LogicPage->Put_Fat_Pixel(my_x_pos - 1, my_y_pos - 1, pixel_size, COLOR_BLUE);

                        // Draw HeadTo coord pixel.
                        g_Map.Coord_To_Pixel(As_Coord(foot_thisptr->Nav_Com()), navcom_x_pos, navcom_y_pos);
                        navcom_y_pos += 16;
                        g_LogicPage->Put_Fat_Pixel(navcom_x_pos - 1, navcom_y_pos - 1, pixel_size, COLOR_BLUE);

                        // Draw connection line.
                        g_LogicPage->Draw_Line(my_x_pos, my_y_pos, navcom_x_pos, navcom_y_pos, COLOR_BLUE);

                        //g_Map.Flag_To_Redraw(true);

                    }

                }

            }

        //}

        //if (/*g_Debug_Tar_Com*/) {

            if (m_Selected) {

                if (Is_Foot()) {

                    FootClass *foot_thisptr = reinterpret_cast<FootClass *>(this);

                    if (Target_Legal(foot_thisptr->Get_TarCom())) {

                        int my_x_pos = 0;
                        int my_y_pos = 0;

                        int tarcom_x_pos = 0;
                        int tarcom_y_pos = 0;

                        int pixel_size = 3;

                        // Draw object coord pixel.
                        g_Map.Coord_To_Pixel(foot_thisptr->m_Coord, my_x_pos, my_y_pos);
                        my_y_pos += 16;
                        g_LogicPage->Put_Fat_Pixel(my_x_pos - 1, my_y_pos - 1, pixel_size, COLOR_RED);

                        // Draw HeadTo coord pixel.
                        g_Map.Coord_To_Pixel(As_Coord(foot_thisptr->Get_TarCom()), tarcom_x_pos, tarcom_y_pos);
                        tarcom_y_pos += 16;
                        g_LogicPage->Put_Fat_Pixel(tarcom_x_pos - 1, tarcom_y_pos - 1, pixel_size, COLOR_RED);

                        // Draw connection line.
                        g_LogicPage->Draw_Line(my_x_pos, my_y_pos, tarcom_x_pos, tarcom_y_pos, COLOR_RED);

                        //g_Map.Flag_To_Redraw(true);

                    }

                }

            }

        //}

        return true;

    }

    return false;
//#endif
}

/**
 * @brief
 *
 * @address 0x0051E71C
 */
const int16_t *ObjectClass::Occupy_List(BOOL a1) const
{
    return Class_Of().Occupy_List();
}

/**
 * @brief
 *
 * @address 0x0051E734
 */
const int16_t *ObjectClass::Overlap_List(BOOL a1) const
{
    return Class_Of().Overlap_List();
}

/**
 * @brief
 *
 * @address 0x0051E77C
 */
fixed_t ObjectClass::Health_Ratio() const
{
    return fixed_t(m_Health, Class_Of().Get_Strength());
}

BOOL ObjectClass::Mark(MarkType mark)
{
#ifdef GAME_DLL
    BOOL(*func)(ObjectClass *, MarkType) = reinterpret_cast<BOOL(*)(ObjectClass *, MarkType)>(0x0051E368);
    return func(this, mark);
#elif 0
    // Needs IOMap, DisplayClass
    if (!m_IsActive || m_InLimbo) {
        return false;
    }

    if (mark == MARK_REDRAW || mark == MARK_3) {
        if ((!m_ToDisplay || mark == MARK_3) && m_IsDown) {
            Mark_For_Redraw();
            return true;
        }

        return false;
    }

    if (mark == MARK_5 && m_IsDown) {
        if (Class_Of().Get_Bit128()) {
            g_Map.Overlap_Up(Coord_To_Cell(Get_Coord()), this);
        }

        Mark_For_Redraw();

        return true;
    }

    if (mark == MARK_4 && m_IsDown) {
        if (Class_Of().Get_Bit128()) {
            g_Map.Overlap_Down(Coord_To_Cell(Get_Coord()), this);
        }

        Mark_For_Redraw();

        return true;
    }

    if (!g_InMapEditor) {
        int risk = 0;
        HousesType house = HOUSES_NONE;

        cell_t cellnum = 0;

        bool is_techno = false;

        if (Is_Techno()) {
            risk = reinterpret_cast<TechnoClass *>(this)->Risk();
            house = reinterpret_cast<TechnoClass *>(this)->Owner();
            is_techno = true;
            cellnum = Coord_To_Cell(Get_Coord());
        }

        if (mark == MARK_PUT && !m_IsDown) {
            if (is_techno && g_Session.Game_To_Play() == GAME_CAMPAIGN && In_Which_Layer() == LAYER_GROUND) {
                g_Map[cellnum].Adjust_Threat(house, risk);
            }

            m_IsDown = true;
            Mark_For_Redraw();

            return true;
        }

        if (mark != MARK_REMOVE || !m_IsDown) {
            return false;
        }

        if (is_techno && g_Session.Game_To_Play() == GAME_CAMPAIGN && In_Which_Layer() == LAYER_GROUND) {
            g_Map[cellnum].Adjust_Threat(house, risk);
        }
    }

    m_IsDown = false;

    return true;
#else
    return false;
#endif
}

/**
 * @brief
 *
 * @address 0x0051DDC0
 */
void ObjectClass::Mark_For_Redraw()
{
    if (!m_ToDisplay) {
        m_ToDisplay = true;
        g_Map.Flag_To_Redraw();
    }
}

BOOL ObjectClass::Select()
{
#ifdef GAME_DLL
    BOOL(*func)(ObjectClass *) = reinterpret_cast<BOOL(*)(ObjectClass *)>(0x0051DBB0);
    return func(this);
#elif 0
    // TODO Needs TechnoClass, HouseClass, DisplayClass
    captainslog_assert(m_IsActive);

    if ((g_InMapEditor || !m_Selected) && Class_Of().Is_Selectable()) {
        if (!g_InMapEditor && Can_Player_Move() && Is_Techno() && reinterpret_cast<TechnoClass *>(this)->m_IsALoner) {
            return false;
        }

        if (Get_Height() <= 0 || (RTTI != RTTI_UNIT && RTTI != RTTI_VESSEL && RTTI != RTTI_INFANTRY)) {
            if (g_Map.PendingObjTypePtr == nullptr) {
                if (CurrentObjects.Count() > 0) {
                    HouseClass *objowner = HouseClass::As_Pointer(Owner());
                    HouseClass *current = HouseClass::As_Pointer(CurrentObjects.Fetch_Head()->Owner());

                    // Check if this objects player control matches the current
                    // selected player control or if the current selected isn't
                    // player controlled.
                    if ((current->PlayerControl != objowner->PlayerControl) || !current->PlayerControl) {
                        Unselect_All();
                    }
                }

                TechnoClass *tptr = reinterpret_cast<TechnoClass *>(this);

                if ((tptr != nullptr && tptr->Class_Of().m_IsLeader)) {
                    CurrentObjects.Add_Head(tptr);
                } else {
                    CurrentObjects.Add(this);
                }

                if (In_Which_Layer() == LAYER_GROUND) {
                    Mark(MARK_5);
                }

                m_Selected = true;

                if (In_Which_Layer() == LAYER_GROUND) {
                    Mark(MARK_4);
                }

                return true;
            }
        }
    }

    return false;
#else
    return false;
#endif
}

/**
 * @brief
 *
 * @address 0x0051DB48
 */
void ObjectClass::Unselect()
{
    // If selected, process unselection.
    if (m_Selected) {
        // Remove this object from the currently selected objects array.
        CurrentObjects.Delete(this);

        if (In_Which_Layer() == LAYER_GROUND) {
            Mark(MARK_5);
        }

        m_Selected = false;

        if (In_Which_Layer() == LAYER_GROUND) {
            Mark(MARK_4);
        }
    }
}

DamageResultType ObjectClass::Take_Damage(int &damage, int a2, WarheadType warhead, TechnoClass *object, BOOL a5)
{
#ifdef GAME_DLL
    DamageResultType (*func)(ObjectClass *, int &, int, WarheadType, TechnoClass *, int) =
        reinterpret_cast<DamageResultType (*)(ObjectClass *, int &, int, WarheadType, TechnoClass *, int)>(0x0051E07C);
    return func(this, damage, a2, warhead, object, a5);
#elif 0
    // Needs IOMap DisplayClass, Damage
    DamageResultType result = DAMAGE_UNAFFECTED;

    int health = m_Health;

    if (health <= 0 || !damage || (!a5 && Class_Of().Is_Immune())) {
        return DAMAGE_UNAFFECTED;
    }

    int strength = Class_Of().Get_Strength();

    if (!a5) {
        damage = Modify_Damage(damage, warhead, Class_Of().Get_Armor(), a2); // needs confirming

        if (object != nullptr) {
            if (object->RTTI == RTTI_INFANTRY) {
                InfantryClass *iptr = reinterpret_cast<InfantryClass *>(object);
                
                if (iptr->Class_Of().IsCanine) {
                    if (As_Target(this) == object->TarCom) {
                        damage = m_Health;
                    } else {
                        damage = 0;
                    }
                }
            }
        }
    }

    if (damage <= 0) {
        return DAMAGE_UNAFFECTED;
    }

    if (damage > 0) {
        result = DAMAGE_UNDAMAGED;

        if (health <= damage) {
            damage = health;
        } else if ((strength / 2 <= health) && (health - damage < strength / 2)) {
            result = DAMAGE_YELLOW;
        }

        m_Health -= damage;

        if (health == damage) {
            if (object != nullptr) {
                Record_The_Kill(object);

                if (Is_Techno()) {
                    TechnoClass *tptr = reinterpret_cast<TechnoClass *>(object);
                    
                    if (tptr != nullptr) {
                        ObjectClass *v99 = As_Object(tptr->OwnerHouse->field_531);
                        captainslog_assert(v99 != nullptr);
                        if (v99 != nullptr) {
                            if (this != v99) {
                                tptr->OwnerHouse->field_531 = 0;
                            }
                        }
                    }
                }

                Detach_All();
            }

        } else if (m_Health == 1) {
            result = DAMAGE_RED;
        }

        if (object != nullptr) {
            if (m_AttachedTrigger != nullptr && result != DAMAGE_DEAD) {
                m_AttachedTrigger->Spring(TEVENT_ATTACKED, this);
            }
        }

        if (result != DAMAGE_UNAFFECTED && m_Selected) {
            Mark(MARK_REDRAW);
        }
        
        return result;
    }

    // TODO, check why not vessel?
    if (What_Am_I() == RTTI_INFANTRY || What_Am_I() == RTTI_UNIT || What_Am_I() == RTTI_AIRCRAFT) {
        Clicked_As_Target(7);

        m_Health -= damage;

        if (m_Health > strength) {
            m_Health = strength;
        }
    }

    return DAMAGE_UNAFFECTED;
#else
    return DAMAGE_UNAFFECTED;
#endif
}

/**
 * @brief
 *
 * @address 0x0051E748
 */
BuildingClass *ObjectClass::Who_Can_Build_Me(BOOL a1, BOOL a2) const
{
    return Class_Of().Who_Can_Build_Me(a1, a2, Owner());
}

/**
 * @brief
 *
 * @address 0x0051E054
 */
RadioMessageType ObjectClass::Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target)
{
    switch (message) {
        case RADIO_WE_BUMPED:
            Mark(MARK_REDRAW);
            return RADIO_ROGER;
        default:
            break;
    };

    return RADIO_STATIC;
}

/**
 * @brief
 *
 * @address 0x0051E5AC
 */
BOOL ObjectClass::Revealed(HouseClass *house)
{
    return house != nullptr;
}

/**
 * @brief
 *
 * @address 0x004F98E0
 */
void ObjectClass::Code_Pointers()
{
    if (m_Next != nullptr) {
        m_Next = reinterpret_cast<ObjectClass *>(m_Next->As_Target());
    }
    // base class does not provide a Code_Pointers function to call here.
}

/**
 * @brief
 *
 * @address 0x004F9924
 */
void ObjectClass::Decode_Pointers()
{
    if (m_Next != nullptr) {
        m_Next = As_Object((uintptr_t)m_Next);
    }
    // base class does not provide a Decode_Pointers function to call here.
}

/**
 * @brief
 *
 * @address 0x0051DABC
 */
void ObjectClass::Move(FacingType facing)
{
    Mark(MARK_REMOVE);

    coord_t adj = Coord_Get_Adjacent(Get_Coord(), facing);

    if (Can_Enter_Cell(Coord_To_Cell(adj)) == MOVE_OK) {
        m_Coord = adj;
    }

    Mark(MARK_PUT);
}

/**
 * @brief
 *
 * @address 0x004A9414
 */
void Unselect_All()
{
    while (CurrentObjects.Count() > 0) {
        CurrentObjects.Fetch_Head()->Unselect();
    }
}

/**
 * @brief
 *
 * @address 0x00423F20
 */
void ObjectClass::Shorten_Attached_Anims()
{
    for (int i = 0; i < g_Anims.Count(); ++i) {
        AnimClass *aptr = &g_Anims[i];
        if (aptr != nullptr) {
            if (As_Object(aptr->Attached_To()) == this) {
                aptr->Set_Loops(0);
            }
        }
    }
}

/**
 * @brief
 *
 * @address 0x0041CB90
 */
BOOL ObjectClass::Counts_As_Civ_Evac()
{
    if (What_Am_I() != RTTI_INFANTRY) {
        return false;
    }

    InfantryClass *ithis = reinterpret_cast<InfantryClass *>(this);

    if (ithis->What_Type() == INFANTRY_EINSTEIN) {
        return true;
    }
    if (ithis->What_Type() == INFANTRY_GENERAL) {
        return true;
    }
    if (ithis->What_Type() == INFANTRY_DELPHI) {
        return true;
    }
    if (ithis->What_Type() == INFANTRY_CHAN) {
        return true;
    }

    //auto evac Tanya
    if (g_Scen.Evacuate_Civilians()) {
        if (ithis->What_Type() == INFANTRY_TANYA) {
            return true;
        }
    }
    if (!ithis->Class_Of().Is_Civilian()) {
        return false;
    }
    if (ithis->Is_Technician()) {
        return false;
    }
    return true;
}

/**
 * @brief
 *
 * @address 0x0051E690
 */
BOOL ObjectClass::Attach_Trigger(TriggerClass *trigger)
{
    if (m_AttachedTrigger != nullptr) {
        m_AttachedTrigger->Release_Ref();
        m_AttachedTrigger = nullptr;
    }
    if (trigger != nullptr) {
        m_AttachedTrigger = trigger;
        m_AttachedTrigger->Add_Ref();
        return true;
    }
    return false;
}
