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
#include "smartptr.h"
#include "vector.h"
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
    ObjectClass(ObjectClass const &that);
    ObjectClass(NoInitClass const &noinit) : AbstractClass(noinit) {}
    virtual ~ObjectClass();

    virtual const char *Name() const override;
    virtual uint32_t Center_Coord() const override { return Coord; }
    virtual uint32_t Target_Coord() const override;
    virtual void AI() override;
    virtual BOOL Is_Player_Army() const { return false; }
    virtual void *Get_Image_Data() const { return Class_Of().Get_Image_Data(); }
    virtual ActionType What_Action(ObjectClass *object) const { return ACTION_NONE; }
    virtual ActionType What_Action(int16_t cellnum) const { return ACTION_NONE; }
    virtual LayerType In_Which_Layer() const { return Height > 171 ? LAYER_TOP : LAYER_GROUND; }
    virtual int Get_Ownable() const { return OWNER_ALL; }
    virtual ObjectTypeClass &Class_Of() const = 0;
    virtual int Full_Name() const { return Class_Of().Full_Name(); }
    virtual BOOL Can_Repair() const { return false; }
    virtual BOOL Can_Demolish() const { return false; }
    virtual BOOL Can_Player_Fire() const { return false; }
    virtual BOOL Can_Player_Move() const { return false; }
    virtual uint32_t Docking_Coord() const { return Center_Coord(); }
    virtual uint32_t Render_Coord() const { return Center_Coord(); }
    virtual uint32_t Sort_Y() const { return Coord; }
    virtual uint32_t Fire_Coord(int weapon = WEAPON_SLOT_PRIMARY) const { return Coord; }
    virtual uint32_t Exit_Coord() const { return Center_Coord(); }
    virtual BOOL Limbo();
    virtual BOOL Unlimbo(uint32_t coord, DirType dir = DIR_NORTH);
    virtual void Detach(int32_t target, int a2);
    virtual void Detach_All(int a1 = 1);
    virtual void Record_The_Kill(TechnoClass *object = nullptr) {}
    virtual BOOL Paradrop(uint32_t coord);
    virtual void Do_Shimmer() {}
    virtual int Exit_Object(TechnoClass *object) { return 0; }
    virtual BOOL Render(BOOL force_render = false);
    virtual const int16_t *Occupy_List(BOOL a1 = false) const;
    virtual const int16_t *Overlap_List(BOOL a1 = false) const;
    virtual fixed Health_Ratio() const;
    virtual void Draw_It(int x, int y, WindowNumberType window) const = 0;
    virtual void Hidden() {}
    virtual void Look(BOOL a1 = false) {}
    virtual BOOL Mark(MarkType mark);
    virtual void Mark_For_Redraw();
    virtual void Active_Click_With(ActionType action, ObjectClass *object) {}
    virtual void Active_Click_With(ActionType action, int16_t cellnum) {}
    virtual void Clicked_As_Target(int a1) {}
    virtual BOOL Select();
    virtual void Unselect();
    virtual BOOL In_Range(uint32_t a1, int weapon = 0) const { return 0; }
    virtual int Weapon_Range(int weapon = WEAPON_SLOT_PRIMARY) const { return 0; }
    virtual DamageResultType Take_Damage(int &damage, int a2, WarheadType warhead, TechnoClass *object = nullptr, BOOL a5 = false);
    virtual void Scatter(uint32_t coord = 0, int a2 = 0, BOOL a3 = false) {}
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

    SmartPtr<ObjectClass> &Get_Next() { return Next; }
    BOOL In_Limbo() const { return InLimbo; }

    static BOOL Sort_Y_Less_Than(ObjectClass *object1, ObjectClass *object2);
    static BOOL Sort_Y_Greater_Than(ObjectClass *object1, ObjectClass *object2);

protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool IsDown : 1; // & 1
            bool ToDamage : 1; // & 2
            bool ToDisplay : 1; // & 4
            bool InLimbo : 1; // & 8 Is this object in limbo [state of nothing, thus not processed per frame tick]?
            bool Selected : 1; // & 16 Has this object been selected by the player?
            bool AnimAttached : 1; // & 32
            bool IsFalling : 1; // & 64 Is this object falling from a height [as a paradropped object]?
            bool OBit1_128 : 1; // & 128
        };
        int Bitfield;
    };
#else
    bool IsDown;
    bool ToDamage;
    bool ToDisplay;
    bool InLimbo; // Is this object in limbo [state of nothing, thus not processed per frame tick]?
    bool Selected; // Has this object been selected by the player?
    bool AnimAttached;
    bool IsFalling; // Is this object falling from a height [as a paradropped object]?
    bool OBit1_128;
#endif
    int FallingHeight;
    SmartPtr<ObjectClass> Next;
    int AttachedTrigger; // GamePtr<TriggerClass> once triggerclass is implemented.
    int16_t Health;
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

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
extern DynamicVectorClass<ObjectClass*> &CurrentObjects;
#else
extern DynamicVectorClass<ObjectClass*> CurrentObjects;
#endif

#endif // OBJECT_H
