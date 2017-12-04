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

            if (Class_Of().Get_Bit128()) {
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
#ifndef RAPP_STANDALONE
    BOOL (*func)(ObjectClass *) = reinterpret_cast<BOOL (*)(ObjectClass *)>(0x0051DDE8);
    return func(this);
#elif 0
    if (GameActive && !InLimbo) {
        Unselect();
        Detach_All();
        Mark(MARK_REMOVE);
        Map.Remove(this, In_Which_Layer());

        if (Class_Of().OT_Bit64) {
            Logic.Remove(this);
        }

        Hidden();
        InLimbo = true;

        return true;
    }

    return false;
#else
    return 0;
#endif
}

BOOL ObjectClass::Unlimbo(uint32_t coord, DirType dir)
{
#ifndef RAPP_STANDALONE
    BOOL (*func)
    (ObjectClass *, uint32_t, DirType) = reinterpret_cast<BOOL (*)(ObjectClass *, uint32_t, DirType)>(0x0051DE9C);
    return func(this, coord, dir);
#elif 0 // Needs IOMapClass and Logic
    int16_t cell = Coord_To_Cell(coord);

    if (GameActive && InLimbo && !IsDown && (ScenarioInit || Can_Enter_Cell(cell) == MOVE_OK)) {
        InLimbo = false;
        ToDisplay = false;
        uint32_t tmp = Class_Of().Coord_Fixup(coord);
        Coord = tmp;

        if (Mark(MARK_PUT)) {
            if (IsActive) {
                if (In_Which_Layer() != LAYER_NONE) {
                    Map.Submit(this, In_Which_Layer());
                }

                if (Class_Of().Get_Bit64()) {
                    Logic.Submit(this);
                }
            }

            return true;
        }
    }

    return false;
#else
    return 0;
#endif
}

void ObjectClass::Detach(int32_t target, int a2)
{
#ifndef RAPP_STANDALONE
    void(*func)(ObjectClass *, int32_t, int) = reinterpret_cast<void(*)(ObjectClass *, int32_t, int)>(0x0051DF74);
    func(this, target, a2);
#elif 0
    DEBUG_ASSERT(IsActive);

    if (AttachedTrigger != nullptr && Target_Get_RTTI(target) == RTTI_TRIGGER) {
        if (AttachedTrigger->As_Target() == target) {
            Attach_Trigger(nullptr);
        }
    }
#else
    return 0;
#endif
}

void ObjectClass::Detach_All(int a1)
{
#ifndef RAPP_STANDALONE
    void(*func)(ObjectClass *, int) = reinterpret_cast<void(*)(ObjectClass *, int)>(0x0051DFDC);
    func(this, a1);
#elif 0
    if (a1 || Owner() != PlayerPtr->What_Type()) {
        Unselect();
    }

    Detach_This_From_All(As_Target(this), a1);
#else
    return 0;
#endif
}

BOOL ObjectClass::Paradrop(uint32_t coord)
{
#ifndef RAPP_STANDALONE
    BOOL(*func)(ObjectClass *, uint32_t) = reinterpret_cast<BOOL(*)(ObjectClass *, uint32_t)>(0x0051E5C0);
    return func(this, coord);
#elif 0 // Needs AnimClass and Coord_Move
    DEBUG_ASSERT(IsActive);

    FallingHeight = 256;
    IsFalling = true;

    if (Unlimbo(coord, DIR_SOUTH)) {
        AnimClass *anim = nullptr;
        uint32_t coord = Coord_Move(Center_Coord(), DIR_NORTH, Height + 48);

        if (RTTI == RTTI_BULLET) {
            anim = new AnimClass(ANIM_PARABOMB, coord, 0, 1, true);	//args needs checking
        } else {
            anim = new AnimClass(ANIM_PARACHUTE, coord, 0, 1, true);	//args needs checking
        }

        anim->Attach_To(this);
    }

    return false;
#else
    return 0;
#endif
}

BOOL ObjectClass::Render(BOOL force_render)
{
#ifndef RAPP_STANDALONE
    BOOL(*func)(ObjectClass *, BOOL) = reinterpret_cast<BOOL(*)(ObjectClass *, BOOL)>(0x0051DD34);
    return func(this, force_render);
#elif 0 // Needs IOMap DisplayClass
    DEBUG_ASSERT(IsActive);

    if ((g_inMapEditor || DebugUnshroud || (force_render || ToDisplay)) && IsDown && !InLimbo) {
        ToDisplay = false;
        int x_pos;
        int y_pos;

        if (Map.Coord_To_Pixel(Coord_To_Cell(Render_Coord()), x_pos, y_pos)) {
            Draw_It(x_pos, y_pos, WINDOW_TACTICAL);

            return true;
        }
    }

    return false;
#else
    return 0;
#endif
}

int16_t *ObjectClass::Occupy_List(BOOL a1) const
{
    return Class_Of().Occupy_List();
}

int16_t *ObjectClass::Overlap_List(BOOL a1) const
{
    return Class_Of().Overlap_List();
}

fixed ObjectClass::Health_Ratio() const
{
    return fixed(Health, Class_Of().Get_Strength());
}

BOOL ObjectClass::Mark(MarkType mark)
{
#ifndef RAPP_STANDALONE
    BOOL(*func)(ObjectClass *, MarkType) = reinterpret_cast<BOOL(*)(ObjectClass *, MarkType)>(0x0051E368);
    return func(this, mark);
#elif 0 // Needs IOMap DisplayClass
    if (!IsActive || InLimbo) {
        return false;
    }

    if (mark == MARK_REDRAW || mark == MARK_3) {
        if ((!ToDisplay || mark == MARK_3) && IsDown) {
            Mark_For_Redraw();
            return true;
        }

        return false;
    }

    if (mark == MARK_5 && IsDown) {
        if (Class_Of().Get_Bit128()) {
            Map.Overlap_Up(Coord_To_Cell(Coord), this);
        }

        Mark_For_Redraw();

        return true;
    }

    if (mark == MARK_4 && IsDown) {
        if (Class_Of().Get_Bit128()) {
            Map.Overlap_Down(Coord_To_Cell(Coord), this);
        }

        Mark_For_Redraw();

        return true;
    }

    if (!g_inMapEditor) {
        int risk = 0;
        HousesType house = HOUSES_NONE;

        int16_t cellnum = NullCell;

        bool is_techno = false

        if (Is_Techno()) {
            risk = reinterpret_cast<TechnoClass *>(this)->Risk();
            house = reinterpret_cast<TechnoClass *>(this)->Owner();
            is_techno = true;
            cellnum = Coord_To_Cell(Get_Coord());
        }

        if (mark == MARK_PUT && !IsDown) {
            if (is_techno && Session.GameToPlay == GAME_CAMPAIGN && In_Which_Layer() == LAYER_GROUND) {
                Map[cellnum].Adjust_Threat(house, risk);
            }

            IsDown = true;
            Mark_For_Redraw();

            return true;
        }

        if (mark != MARK_REMOVE || !IsDown) {
            return false;
        }

        if (is_techno && Session.GameToPlay == GAME_CAMPAIGN && In_Which_Layer() == LAYER_GROUND) {
            Map[cellnum].Adjust_Threat(house, risk);
        }
    }

    IsDown = false;

    return true;
#else
    return 0;
#endif
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
