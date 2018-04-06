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
#include "iomap.h"
#include "minmax.h"

#ifndef RAPP_STANDALONE
DynamicVectorClass<ObjectClass*> &CurrentObjects = Make_Global<DynamicVectorClass<ObjectClass*> >(0x006677F8);
#else
DynamicVectorClass<ObjectClass*> CurrentObjects;
#endif

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
    if (damage || Owner() != PlayerPtr->What_Type()) {
        Unselect();
    }

    Detach_This_From_All(As_Target(this), damage);
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

const int16_t *ObjectClass::Occupy_List(BOOL a1) const
{
    return Class_Of().Occupy_List();
}

const int16_t *ObjectClass::Overlap_List(BOOL a1) const
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

void ObjectClass::Mark_For_Redraw()
{
    if (!ToDisplay) {
        ToDisplay = true;
        Map.Flag_To_Redraw();
    }
}

BOOL ObjectClass::Select()
{
#ifndef RAPP_STANDALONE
    BOOL(*func)(ObjectClass *) = reinterpret_cast<BOOL(*)(ObjectClass *)>(0x0051DBB0);
    return func(this);
#elif 0 // TODO Needs TechnoClass HouseClass DisplayClass
    DEBUG_ASSERT(IsActive);

    if ((g_inMapEditor || !Selected) && Class_Of().Is_Selectable()) {
        if (!g_inMapEditor && Can_Player_Move() && Is_Techno() && reinterpret_cast<TechnoClass *>(this)->IsALoaner) {
            return false;
        }

        if (Get_Height() <= 0 || (RTTI != RTTI_UNIT && RTTI != RTTI_VESSEL && RTTI != RTTI_INFANTRY)) {
            if (Map.PendingObjTypePtr == nullptr) {
                if (CurrentObjects.Count() > 0) {
                    HouseClass *objowner = HouseClass::As_Pointer(Owner());
                    HouseClass *current = HouseClass::As_Pointer(CurrentObjects[0]->Owner());

                    // Check if this objects player control matches the current
                    // selected player control or if the current selected isn't
                    // player controlled.
                    if ((current->PlayerControl != objowner->PlayerControl) || !current->PlayerControl) {
                        Unselect_All();
                    }
                }

                TechnoClass *tptr = reinterpret_cast<TechnoClass *>(this);

                if ((tptr != nullptr && tptr->Class_Of().IsLeader)) {
                    CurrentObjects.Add_Head(tptr);
                } else {
                    CurrentObjects.Add(this);
                }

                if (In_Which_Layer() == LAYER_GROUND) {
                    Mark(MARK_5);
                }

                Selected = true;

                if (In_Which_Layer() == LAYER_GROUND) {
                    Mark(MARK_4);
                }

                return true;
            }
        }
    }

    return false;
#else
    return 0;
#endif
}

void ObjectClass::Unselect()
{
    // If selected, process unselection.
    if (Selected) {
        // Remove this object from the currently selected objects array.
        CurrentObjects.Delete(this);

        if (In_Which_Layer() == LAYER_GROUND) {
            Mark(MARK_5);
        }

        if (In_Which_Layer() == LAYER_GROUND) {
            Mark(MARK_4);
        }

        // Set this object to unselected now we have performed the necessary tasks.
        Selected = false;
    }
}

DamageResultType ObjectClass::Take_Damage(int &damage, int a2, WarheadType warhead, TechnoClass *object, BOOL a5)
{
#ifndef RAPP_STANDALONE
    DamageResultType (*func)(ObjectClass *, int &, int, WarheadType, TechnoClass *, int) =
        reinterpret_cast<DamageResultType (*)(ObjectClass *, int &, int, WarheadType, TechnoClass *, int)>(0x0051E07C);
    return func(this, damage, a2, warhead, object, a5);
#elif 0 // Needs IOMap DisplayClass
    DamageResultType result = DAMAGE_UNAFFECTED;

    int health = Health;

    if (health <= 0 || !damage || (!a5 && Class_Of().Is_Immune())) {
        return DAMAGE_UNAFFECTED;
    }

    int strength = Class_Of().Strength;

    if (!a5) {
        damage = Modify_Damage(damage, warhead, Class_Of().Armor, a2); // needs confirming

        if (object != nullptr) {
            if (object->RTTI == RTTI_INFANTRY) {
                InfantryClass *iptr = reinterpret_cast<InfantryClass *>(object);
                
                if (iptr->Class_Of().IsCanine) {
                    if (As_Target(this) == object->TarCom) {
                        damage = Health;
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

        Health -= damage;

        if (health == damage) {
            if (object != nullptr) {
                Record_The_Kill(object);

                if (Is_Techno()) {
                    TechnoClass *tptr = reiterpret_cast<TechnoClass *>(object);
                    
                    if (tptr != nullptr) {
                        ObjectClass *v99 = As_Object(tptr->OwnerHouse->field_531);
                        DEBUG_ASSERT(v99 != nullptr);
                        if (v99 != nullptr) {
                            if (this != v99) {
                                tptr->OwnerHouse->field_531 = NullTarget;
                            }
                        }
                    }
                }

                Detach_All(1);
            }

        } else if (Health == 1) {
            result = DAMAGE_RED;
        }

        if (object != nullptr) {
            if (AttachedTrigger != nullptr && result != DAMAGE_DEAD) {
                AttachedTrigger->Spring(TEVENT_ATTACKED, this);
            }
        }

        if (result != DAMAGE_UNAFFECTED && Selected) {
            Mark(MARK_REDRAW);
        }
        
        return result;
    }

    // TODO, check why not vessel?
    if (RTTI == RTTI_INFANTRY || RTTI == RTTI_UNIT || RTTI == RTTI_AIRCRAFT) {
        Clicked_As_Target(7);

        Health -= damage;

        if (Health > strength) {
            Health = strength;
        }
    }

    return DAMAGE_UNAFFECTED;
#else
    return DAMAGE_UNAFFECTED;
#endif
}

BuildingClass *ObjectClass::Who_Can_Build_Me(BOOL a1, BOOL a2) const
{
    return Class_Of().Who_Can_Build_Me(a1, a2, Owner());
}

RadioMessageType ObjectClass::Receive_Message(RadioClass *radio, RadioMessageType msg, int32_t &a3)
{
    switch (msg) {
        case RADIO_13:
            Mark(MARK_REDRAW);

            return RADIO_ROGER;
        default:
            break;
    };

    return RADIO_STATIC;
}

BOOL ObjectClass::Revealed(HouseClass *house)
{
    return house != nullptr;
}

void ObjectClass::Code_Pointers()
{
#ifndef RAPP_STANDALONE
    void(*func)(ObjectClass *) = reinterpret_cast<void(*)(ObjectClass *)>(0x004F98E0);
    func(this);
#elif 0
    if (Next != nullptr) {
        Next = (ObjectClass *)As_Target((ObjectClass *)Next);
    }

    AbstractClass::Code_Pointers();
#endif
}

void ObjectClass::Decode_Pointers()
{
#ifndef RAPP_STANDALONE
    void(*func)(ObjectClass *) = reinterpret_cast<void(*)(ObjectClass *)>(0x004F9924);
    func(this);
#elif 0
    if (Next != nullptr) {
        Next = As_Object((uintptr_t)Next);
    }

    AbstractClass::Code_Pointers();
#endif
}

void ObjectClass::Move(FacingType facing)
{
    Mark(MARK_REMOVE);

    uint32_t adj = Coord_Get_Adjacent(Coord, facing);

    if (!Can_Enter_Cell(Coord_To_Cell(adj))) {
        Coord = adj;
    }

    Mark(MARK_PUT);
}
