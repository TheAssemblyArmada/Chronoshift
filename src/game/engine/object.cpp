/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base class for most ingame object instances.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "object.h"
#include "coord.h"
#include "minmax.h"

ObjectClass::ObjectClass() :
    IsDown(false),
    ToDamage(false),
    ToDisplay(false),
    InLimbo(true),
    Selected(false),
    AnimAttached(false),
    IsFalling(false),
    OBit1_128(false),
    FallingHeight(0),
    Next(nullptr),
    AttachedTrigger(-1),
    // AttachedTrigger(nullptr), // should be this when TriggerClass implemented.
    Health(255)
{
}

ObjectClass::ObjectClass(RTTIType type, int id) :
    AbstractClass(type, id),
    IsDown(false),
    ToDamage(false),
    ToDisplay(false),
    InLimbo(true),
    Selected(false),
    AnimAttached(false),
    IsFalling(false),
    OBit1_128(false),
    FallingHeight(0),
    Next(nullptr),
    AttachedTrigger(-1),
    // AttachedTrigger(nullptr), // should be this when TriggerClass implemented.
    Health(255)
{
}

ObjectClass::ObjectClass(ObjectClass const &that) :
    AbstractClass(that),
    IsDown(that.IsDown),
    ToDamage(that.ToDamage),
    ToDisplay(that.ToDisplay),
    InLimbo(that.InLimbo),
    Selected(that.Selected),
    AnimAttached(that.AnimAttached),
    IsFalling(that.IsFalling),
    OBit1_128(that.OBit1_128),
    FallingHeight(that.FallingHeight),
    Health(that.Health),
    Next(that.Next),
    AttachedTrigger(that.AttachedTrigger)
{
}

ObjectClass::~ObjectClass()
{
    Next = nullptr;
}

const char *ObjectClass::Name() const
{
    return Class_Of().Get_Name();
}

uint32_t ObjectClass::Target_Coord() const
{
    uint32_t coord = Center_Coord();

    return Coord_From_Lepton_XY(Coord_Lepton_X(coord), Coord_Lepton_Y(coord) - Height);
}

void ObjectClass::AI()
{
#ifndef RAPP_STANDALONE
    void (*func)(ObjectClass *) = reinterpret_cast<void (*)(ObjectClass *)>(0x0051D7F0);
    func(this);
#elif 0
    DEBUG_ASSERT(IsActive);

    AbstractClass::AI();

    if (IsFalling) {
        LayerType layer = In_Which_Layer();

        Height = FallingHeight + Height;

        if (Height <= 0) {
            Height = 0;
            IsFalling = false;
            Per_Cell_Process(PCP_2);
            Shorten_Attached_Anims();
        }

        if (AnimAttached) {
            --FallingHeight;
            FallingHeight = Max(-3, FallingHeight);
        } else {
            FallingHeight = FallingHeight - Rule.Gravity;
            FallingHeight = Max(-100, FallingHeight);
        }

        if (layer != In_Which_Layer()) {
            Map.Remove(this, layer);
            Map.Submit(this, In_Which_Layer());

            if (Class_Of().Get_Logical()) {
                if (In_Which_Layer() == LAYER_SURFACE) {
                    Map.Place_Down(Coord_To_Cell(Center_Coord()), this);
                } else {
                    Map.Pick_Up(Coord_To_Cell(Center_Coord()), this);
                }
            }
        }
    }
#endif
}

int ObjectClass::Get_Ownable() const
{
    return OWNER_SPAIN | OWNER_GREECE | OWNER_USSR | OWNER_ENGLAND | OWNER_UKRAINE | OWNER_GERMANY | OWNER_FRANCE
        | OWNER_TURKEY | OWNER_GOODGUY | OWNER_BADGUY | OWNER_NEUTRAL | OWNER_SPECIAL | OWNER_MP1 | OWNER_MP2 | OWNER_MP3
        | OWNER_MP4 | OWNER_MP5 | OWNER_MP6 | OWNER_MP7 | OWNER_MP8;
}

BOOL ObjectClass::Limbo()
{
    return 0;
}

BOOL ObjectClass::Unlimbo(uint32_t coord, DirType dir)
{
    return 0;
}

void ObjectClass::Detach(int32_t target, int a2) {}

void ObjectClass::Detach_All(int a1) {}

void ObjectClass::Record_The_Kill(TechnoClass *object) {}

BOOL ObjectClass::Paradrop(uint32_t coord)
{
    return 0;
}

void ObjectClass::Do_Shimmer() {}

int ObjectClass::Exit_Object(TechnoClass *object)
{
    return 0;
}

BOOL ObjectClass::Render(BOOL force_render)
{
    return 0;
}

int16_t *ObjectClass::Occupy_List(BOOL a1) const
{
    return nullptr;
}

int16_t *ObjectClass::Overlap_List(BOOL a1) const
{
    return nullptr;
}

fixed ObjectClass::Health_Ratio() const
{
    return fixed();
}

void ObjectClass::Hidden() {}

void ObjectClass::Look(BOOL a1) {}

BOOL ObjectClass::Mark(MarkType mark)
{
    return 0;
}

void ObjectClass::Mark_For_Redraw() {}

void ObjectClass::Active_Click_With(ActionType action, ObjectClass *object) {}

void ObjectClass::Active_Click_With(ActionType action, int16_t cellnum) {}

void ObjectClass::Clicked_As_Target(int a1) {}

BOOL ObjectClass::Select()
{
    return 0;
}

void ObjectClass::Unselect() {}

BOOL ObjectClass::In_Range(uint32_t a1, int weapon) const
{
    return 0;
}

int ObjectClass::Weapon_Range(int weapon) const
{
    return 0;
}

DamageResultType ObjectClass::Take_Damage(int &a1, int a2, WarheadType warhead, TechnoClass *object, int a5)
{
    return DamageResultType();
}

void ObjectClass::Scatter(uint32_t coord, int a2, BOOL a3) {}

BOOL ObjectClass::Catch_Fire()
{
    return 0;
}

void ObjectClass::Fire_Out() {}

int ObjectClass::Value() const
{
    return 0;
}

MissionType ObjectClass::Get_Mission() const
{
    return MissionType();
}

void ObjectClass::Per_Cell_Process(PCPType pcp) {}

BuildingClass *ObjectClass::Who_Can_Build_Me(BOOL a1, BOOL a2) const
{
    return nullptr;
}

RadioMessageType ObjectClass::Receive_Message(RadioClass *radio, RadioMessageType msg, int32_t &a3)
{
    return RadioMessageType();
}

BOOL ObjectClass::Revealed(HouseClass *house)
{
    return 0;
}

void ObjectClass::Repair(int a1) {}

void ObjectClass::Sell_Back(int a1) {}

void ObjectClass::Code_Pointers() {}

void ObjectClass::Decode_Pointers() {}

void ObjectClass::Move(FacingType facing) {}
