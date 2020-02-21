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
#pragma once

#ifndef OBJECT_H
#define OBJECT_H

#include "always.h"
#include "abstract.h"
#include "action.h"
#include "dialog.h"
#include "fixed.h"
#include "gametypes.h"
#include "layer.h"
#include "objecttype.h"
#include "rtti.h"
#include "vector.h"
#include "trigger.h"
#include "warheadtype.h"

class TechnoClass;
class BuildingClass;
class HouseClass;
class RadioClass;

class ObjectClass : public AbstractClass
{
public:
    ObjectClass();
    ObjectClass(RTTIType type, int id);
    ObjectClass(const ObjectClass &that);
    ObjectClass(const NoInitClass &noinit) : AbstractClass(noinit) {}
    virtual ~ObjectClass();

#ifdef CHRONOSHIFT_DEBUG
#ifdef CHRONOSHIFT_STANDALONE
    virtual void Debug_Dump(MonoClass *mono) const override;
#else
    void Debug_Dump(MonoClass *mono) const;
#endif
#endif

    virtual const char *Name() const override;
    virtual coord_t Center_Coord() const override { return Get_Coord(); }
    virtual coord_t Target_Coord() const override;
    virtual void AI() override;
    virtual BOOL Is_Players_Army() const { return false; }
    virtual void *Get_Image_Data() const { return Class_Of().Get_Image_Data(); }
    virtual ActionType What_Action(ObjectClass *object) const { return ACTION_NONE; }
    virtual ActionType What_Action(cell_t cellnum) const { return ACTION_NONE; }
    virtual LayerType In_Which_Layer() const { return Get_Height() >= 171 ? LAYER_TOP : LAYER_GROUND; } // What display layer am i located in (for drawing purposes)?
    virtual int Get_Ownable() const { return OWNER_ALL; }
    virtual const ObjectTypeClass &Class_Of() const = 0;
    virtual int Full_Name() const { return Class_Of().Full_Name(); }
    virtual BOOL Can_Repair() const { return false; }
    virtual BOOL Can_Demolish() const { return false; }
    virtual BOOL Can_Player_Fire() const { return false; }
    virtual BOOL Can_Player_Move() const { return false; }
    virtual coord_t Docking_Coord() const { return Center_Coord(); }
    virtual coord_t Render_Coord() const { return Center_Coord(); }
    virtual coord_t Sort_Y() const { return Get_Coord(); }
    virtual coord_t Fire_Coord(WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) const { return Get_Coord(); }
    virtual coord_t Exit_Coord() const { return Center_Coord(); }
    virtual BOOL Limbo(); // Take the object from the game world and place into limbo.
    virtual BOOL Unlimbo(coord_t coord, DirType dir = DIR_NORTH); // Object is appearing in the game world.
    virtual void Detach(target_t target, BOOL a2 = true);
    virtual void Detach_All(BOOL a1 = true);
    virtual void Record_The_Kill(TechnoClass *object = nullptr) {}
    virtual BOOL Paradrop(coord_t coord);
    virtual void Do_Shimmer() {}
    virtual int Exit_Object(TechnoClass *object) { return 0; }
    virtual BOOL Render(BOOL force_render = false);
    virtual const int16_t *Occupy_List(BOOL a1 = false) const;
    virtual const int16_t *Overlap_List(BOOL a1 = false) const;
    virtual fixed_t Health_Ratio() const;
    virtual void Draw_It(int x, int y, WindowNumberType window) const = 0;
    virtual void Hidden() {}
    virtual void Look(BOOL a1 = false) {}
    virtual BOOL Mark(MarkType mark);
    virtual void Mark_For_Redraw();
    virtual void Active_Click_With(ActionType action, ObjectClass *object) {}
    virtual void Active_Click_With(ActionType action, cell_t cellnum) {}
    virtual void Clicked_As_Target(int a1) {}
    virtual BOOL Select();
    virtual void Unselect();
    virtual BOOL In_Range(coord_t coord, WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) const { return 0; }
    virtual int Weapon_Range(WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) const { return 0; }
    virtual DamageResultType Take_Damage(int &damage, int a2, WarheadType warhead, TechnoClass *object = nullptr, BOOL a5 = false);
    virtual void Scatter(coord_t coord = 0, BOOL a2 = false, BOOL a3 = false) {}
    virtual BOOL Catch_Fire() { return 0; }
    virtual void Fire_Out() {}
    virtual int Value() const { return 0; }
    virtual MissionType Get_Mission() const { return MISSION_NONE; }
    virtual void Per_Cell_Process(PCPType pcp) {}
    virtual BuildingClass *Who_Can_Build_Me(BOOL a1 = false, BOOL a2 = false) const;
    virtual RadioMessageType Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target);
    virtual BOOL Revealed(HouseClass *house);
    virtual void Repair(int a1 = -1) {}
    virtual void Sell_Back(int a1) {}
    virtual void Code_Pointers();
    virtual void Decode_Pointers();
    virtual void Move(FacingType facing);

    ObjectClass *Get_Next() { return m_Next; }
    void Set_Next(ObjectClass *next) { m_Next = next; }
    const GamePtr<TriggerClass> &Get_Attached_Trigger() const { return m_AttachedTrigger; }
    void Invalidate_Attached_Trigger() { m_AttachedTrigger = nullptr; }
    BOOL In_Limbo() const { return m_InLimbo; }
    BOOL Selected() const { return m_Selected; }
    int16_t Get_Health() const { return m_Health; }
    void Set_Health(int16_t health) { m_Health = health; }

    void Set_ToDisplay(bool state) { m_ToDisplay = state; }
    void Set_AnimAttached(bool state) { m_AnimAttached = state; }

    static BOOL Sort_Y_Less_Than(ObjectClass *object1, ObjectClass *object2);
    static BOOL Sort_Y_Greater_Than(ObjectClass *object1, ObjectClass *object2);

    BOOL Attach_Trigger(TriggerClass *trigger);
    // These functions were global, but only ObjectClass derived classes use them.
    void Shorten_Attached_Anims();
    BOOL Counts_As_Civ_Evac();

protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_IsDown : 1; // 1
    BOOL m_ToDamage : 1; // 2
    BOOL m_ToDisplay : 1; // 4
    BOOL m_InLimbo : 1; // 8
    BOOL m_Selected : 1; // 16
    BOOL m_AnimAttached : 1; // 32
    BOOL m_IsFalling : 1; // 64
    BOOL m_OBit1_128 : 1; // 128
#else
    bool m_IsDown;
    bool m_ToDamage;
    bool m_ToDisplay;
    bool m_InLimbo; // Is this object in limbo [state of nothing, thus not processed per frame tick]?
    bool m_Selected; // Has this object been selected by the player?
    bool m_AnimAttached;
    bool m_IsFalling; // Is this object falling from a height [as a paradropped object]?
    bool m_OBit1_128;
#endif
    int m_FallingHeight;
    ObjectClass *m_Next;
    GamePtr<TriggerClass> m_AttachedTrigger;
    int16_t m_Health;
};

void Unselect_All();

//EDWIN 0x0044021C
inline BOOL ObjectClass::Sort_Y_Less_Than(ObjectClass *object1, ObjectClass *object2)
{
    return object1->Sort_Y() < object2->Sort_Y();
}
 
//EDWIN 0x004401BC
inline BOOL ObjectClass::Sort_Y_Greater_Than(ObjectClass *object1, ObjectClass *object2)
{
    return object1->Sort_Y() > object2->Sort_Y();
}

#ifdef GAME_DLL
extern DynamicVectorClass<ObjectClass*> &CurrentObjects;
#else
extern DynamicVectorClass<ObjectClass*> CurrentObjects;
#endif

#endif // OBJECT_H
