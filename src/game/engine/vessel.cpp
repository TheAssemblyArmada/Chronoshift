/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
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
#include "vessel.h"
#include "house.h"
#include "iomap.h"
#include "gameini.h"
#include "coord.h"
#include "globals.h"
#include "rules.h"
#include <cstdio>

#define TRANSPORT_DOOR_DELAY 5
#define TRANSPORT_DOOR_STAGES 6

#ifndef CHRONOSHIFT_STANDALONE
TFixedIHeapClass<VesselClass> &g_Vessels = Make_Global<TFixedIHeapClass<VesselClass> >(0x0065DC8C);
#else
TFixedIHeapClass<VesselClass> g_Vessels;
#endif

VesselClass::VesselClass(VesselType type, int id, HousesType house) :
    DriveClass(RTTI_VESSEL, g_Vessels.ID(this), house),
    m_Type(g_VesselTypes.Ptr(type)),
    m_ToSelfRepair(false),
    m_Repairing(false),
    m_TransportDoorTimer(),
    m_SubmergeTimer(),
    m_SecondaryTurretFacing()
{
    if (m_OwnerHouse != nullptr) {
        //Set_Player_Owned(); // TODO: Where is this line from?
        m_OwnerHouse->Tracking_Add(this);
    }

    if (What_Type() != VESSEL_NONE) {
        m_Bit2_16 = !(Class_Of().Is_Two_Shooter() == true);
        m_Cloakable = Class_Of().Is_Cloakable();
        m_Ammo = Class_Of().Get_Ammo();
        m_Health = Class_Of().Get_Strength();
    }
}

VesselClass::VesselClass(const VesselClass &that) :
    DriveClass(that),
    m_Type(that.m_Type),
    m_ToSelfRepair(that.m_ToSelfRepair),
    m_Repairing(that.m_Repairing),
    m_TransportDoorTimer(that.m_TransportDoorTimer),
    m_SubmergeTimer(that.m_SubmergeTimer),
    m_SecondaryTurretFacing(that.m_SecondaryTurretFacing)
{
}

VesselClass::VesselClass(const NoInitClass &noinit) :
    DriveClass(noinit),
    m_TransportDoorTimer(noinit),
    m_SubmergeTimer(noinit),
    m_SecondaryTurretFacing(noinit)
{
}

VesselClass::~VesselClass()
{
    m_Type = nullptr;
    if (GameActive) {
        if (m_OwnerHouse != nullptr) {
            m_OwnerHouse->Tracking_Remove(this);
        }
    }
}

void *VesselClass::operator new(size_t size)
{
    VesselClass *this_ptr = g_Vessels.Alloc();
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = true;
    }
    return this_ptr;
}

void VesselClass::operator delete(void *ptr)
{
    VesselClass *this_ptr = static_cast<VesselClass *>(ptr);
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = false;
    }
    g_Vessels.Free(this_ptr);
}

MoveType VesselClass::Can_Enter_Cell(cell_t cellnum, FacingType facing) const
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    MoveType const (*func)(const VesselClass *, cell_t, FacingType) = reinterpret_cast<MoveType const (*)(const VesselClass *, cell_t, FacingType)>(0x00589ECC);
    return func(this, cellnum, facing);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return MOVE_NONE;
#endif
}

void VesselClass::AI()
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(VesselClass *) = reinterpret_cast<void (*)(VesselClass *)>(0x0058A634);
    return func(this);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    
    /*if (Not_On_A_Mission()) {
        Enter_Idle_Mode();
    }

    if (Get_Mission() == MISSION_HUNT && !Is_Weapon_Equipped()) {
        Assign_Mission(MISSION_RETREAT);
    }

    if (!m_Moving && m_Door.Is_Closed()) {
        Commence();
    }

    if (Map.In_View(Center_Cell())) {
        if (Visually_Unclear()) {
            Mark(MARK_REDRAW);
        }
    }

    if (What_Type() == VESSEL_CARRIER) {
        if (m_Cargo.Cargo_Count()) {
            if (m_field_145.Expired()) {
                // TODO
                for ( ObjectClass *cargoptr = m_Cargo.Attached_Object(); cargoptr != nullptr; cargoptr = cargoptr->Get_Next().Get_Pointer() ) {
                    target_t tmptarget = 0;
                    cargoptr->Receive_Message(this, RADIO_RELOAD, tmptarget);
                }
            }
        }
    }

    DriveClass::AI();

    if (m_IsActive) {

        Rotation_AI();
        Combat_AI();

        if (!Edge_Of_World_AI()) {

            if (Class_Of().Max_Passengers() > 0 && !m_Door.Is_Closed() && Get_Mission() != MISSION_UNLOAD
                && Transmit_Message(RADIO_NEED_A_LIFT) != RADIO_ROGER) {
                if (m_TransportDoorTimer.Expired()) {
                    Transport_Close_Door();
                }
            }

            if (!m_Moving && m_Door.Is_Closed()) {
                Commence();
            }

            Repair_AI();
        }

    }*/
#endif
}

ActionType VesselClass::What_Action(ObjectClass *object) const
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    ActionType (*func)(const VesselClass *, ObjectClass *) = reinterpret_cast<ActionType (*)(const VesselClass *, ObjectClass *)>(0x0058AAA0);
    return func(this, object);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return ACTION_NONE;
#endif
}

ActionType VesselClass::What_Action(cell_t cellnum) const
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    ActionType (*func)(const VesselClass *, cell_t) = reinterpret_cast<ActionType (*)(const VesselClass *, cell_t)>(0x0058AAA0);
    return func(this, cellnum);
#else
    ActionType action = DriveClass::What_Action(cellnum);

    if (action != ACTION_NO_MOVE || Map[cellnum].Get_Land() != LAND_BEACH) {
        if (action == ACTION_NO_MOVE) {
            if (Is_Weapon_Equipped()) {
                if (Class_Of().Get_Weapon(WEAPON_SLOT_PRIMARY)->Get_Projectile()->Get_UnderWater()
                    && Map[cellnum].Is_Bridge_Here()) {
                    return ACTION_ATTACK;
                }
            }
        }

        return action;
    }

    return ACTION_MOVE;
#endif
}

coord_t VesselClass::Fire_Coord(WeaponSlotType weapon) const
{
    DEBUG_ASSERT(m_IsActive);

    switch (What_Type())
    {
        case VESSEL_CRUISER:
        {
            DirType dir = (m_Bit2_16 ? m_Facing.Get_Current() + -128 : m_Facing.Get_Current());
            coord_t coord = Coord_Move(Coord_Move(Center_Coord(), dir, 256), DIR_NORTH, 48);
            return Coord_Move(coord, Turret_Facing(), 64);
        }
        case VESSEL_PT_BOAT:
        {
            coord_t coord = Coord_Move(Coord_Move(Center_Coord(), m_Facing.Get_Current(), 128), DIR_NORTH, 32);
            return Coord_Move(coord, Turret_Facing(), 16);
        }
        default:
            return DriveClass::Fire_Coord(weapon);
    };
}

const int16_t *VesselClass::Overlap_List(BOOL a1) const
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    const int16_t *(*func)(const VesselClass *, BOOL) = reinterpret_cast<const int16_t *(*)(const VesselClass *, BOOL)>(0x0058A494);
    return func(this, a1);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");

    /*DEBUG_ASSERT(this != nullptr);
    DEBUG_ASSERT(m_IsActive);

    if (Get_Height() <= 0 && Class_Of().FrameDimensions != nullptr && a1) {
        TRect<int> tmprect(0, 0, 0, 0);

        int frame = Shape_Number();

        Class_Of().Set_Frame_Dimensions(Get_Image_Data(), frame);

        tmprect = Class_Of().FrameDimensions[frame];

        if (m_Selected) {
            tmprect = tmprect.Union(TRect<int>(-32, -32, 64, 64));
        }

        return Coord_Spillage_List(Get_Coord(), tmprect, 1);
    }

    return Coord_Spillage_List(Get_Coord(), 56) + 1; // todo, check this +1, assembly says +2?*/
    return nullptr;
#endif
}

void VesselClass::Draw_It(int x, int y, WindowNumberType window) const
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(const VesselClass *, int, int, WindowNumberType) = reinterpret_cast<void (*)(const VesselClass *, int, int, WindowNumberType)>(0x0058A0FC);
    return func(this, x, y, window);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
#endif
}

void VesselClass::Active_Click_With(ActionType action, ObjectClass *object)
{
    DEBUG_ASSERT(m_IsActive);

    if (What_Action(object) == ACTION_ENTER) {
        DriveClass::Active_Click_With(ACTION_MOVE, object);
    } else {
        DriveClass::Active_Click_With(action, object);
    }
}

void VesselClass::Active_Click_With(ActionType action, cell_t cellnum)
{
    DEBUG_ASSERT(m_IsActive);

    DriveClass::Active_Click_With(action, cellnum);
}

DamageResultType VesselClass::Take_Damage(int &damage, int a2, WarheadType warhead, TechnoClass *object, BOOL a5)
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    DamageResultType (*func)(VesselClass *, int &, int, WarheadType, TechnoClass *, int) = reinterpret_cast<DamageResultType (*)(VesselClass *, int &, int, WarheadType, TechnoClass *, int)>(0x0058AD50);
    return func(this, damage, a2, warhead, object, a5);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return DAMAGE_NONE;
#endif
}

void VesselClass::Per_Cell_Process(PCPType pcp)
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(VesselClass *, PCPType) = reinterpret_cast<void (*)(VesselClass *, PCPType)>(0x0058A90C);
    return func(this, pcp);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
#endif
}

RadioMessageType VesselClass::Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target)
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    RadioMessageType (*func)(VesselClass *, RadioClass *, RadioMessageType, target_t &) = reinterpret_cast<RadioMessageType (*)(VesselClass *, RadioClass *, RadioMessageType, target_t &)>(0x0058BA50);
    return func(this, radio, message, target);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return RADIO_NONE;
#endif
}

int VesselClass::Mission_Retreat()
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    int (*func)(VesselClass *) = reinterpret_cast<int (*)(VesselClass *)>(0x0058C748);
    return func(this);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return 0;
#endif
}

int VesselClass::Mission_Unload()
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    int (*func)(VesselClass *) = reinterpret_cast<int (*)(VesselClass *)>(0x0058C2A0);
    return func(this);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return 0;
#endif
}

DirType VesselClass::Turret_Facing() const
{
    DEBUG_ASSERT(m_IsActive);

    return (Class_Of().Is_Turret_Equipped() ? m_SecondaryTurretFacing.Get_Current() : m_Facing.Get_Current());
}

DirType VesselClass::Desired_Load_Dir(ObjectClass *object, cell_t &cellnum) const
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    DirType const (*func)(const VesselClass *, ObjectClass *, cell_t &) = reinterpret_cast<DirType const (*)(const VesselClass *, ObjectClass *, cell_t &)>(0x0058C074);
    return func(this, object, cellnum);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return DIR_NONE;
#endif
}

BOOL VesselClass::Is_Allowed_To_Recloak() const
{
    DEBUG_ASSERT(m_IsActive);

    return m_SubmergeTimer.Expired();
}

FireErrorType VesselClass::Can_Fire(target_t target, WeaponSlotType weapon) const
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    FireErrorType (*func)(const VesselClass *, target_t, WeaponSlotType) = reinterpret_cast<FireErrorType (*)(const VesselClass *, target_t, WeaponSlotType)>(0x0058AFF8);
    return func(this, target, weapon);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return FIRE_NONE;
#endif
}

target_t VesselClass::Greatest_Threat(ThreatType threat)
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    target_t (*func)(VesselClass *, ThreatType) = reinterpret_cast<target_t (*)(VesselClass *, ThreatType)>(0x0058B6F8);
    return func(this, threat);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return 0;
#endif
}

BulletClass *VesselClass::Fire_At(target_t target, WeaponSlotType weapon)
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    BulletClass *(*func)(VesselClass *, target_t, WeaponSlotType) = reinterpret_cast<BulletClass * (*)(VesselClass *, target_t, WeaponSlotType)>(0x0058D498);
    return func(this, target, weapon);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
    return nullptr;
#endif
}

void VesselClass::Assign_Destination(target_t dest)
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(VesselClass *, target_t) = reinterpret_cast<void (*)(VesselClass *, target_t)>(0x0058C654);
    return func(this, dest);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
#endif
}

void VesselClass::Enter_Idle_Mode(BOOL a1)
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(VesselClass *, BOOL) = reinterpret_cast<void (*)(VesselClass *, BOOL)>(0x0058B87C);
    return func(this, a1);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
#endif
}

BOOL VesselClass::Start_Driver(coord_t &dest)
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(VesselClass *, coord_t &) = reinterpret_cast<BOOL (*)(VesselClass *, coord_t &)>(0x0058CC9C);
    return func(this, dest);
#else
    if (DriveClass::Start_Driver(dest) && m_IsActive) {
        Mark_Track(dest, MARK_PUT);
        return true;
    }
    return false;
#endif
}

int VesselClass::Shape_Number() const
{
    int frame = 0;

    switch (What_Type()) {
        case VESSEL_TRANSPORT: // Fall through.
        case VESSEL_CARRIER:
            frame = 0;
            break;
        default:
            frame = TechnoClass::BodyShape32[FacingClass::Facing16[m_Facing.Get_Current()] * 2] / 2;
            break;
    };

    if (!m_Door.Is_Closed()) {
        frame = m_Door.Door_Stage();
    }

    return frame;
}

void VesselClass::Rotation_AI()
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(VesselClass *) = reinterpret_cast<void (*)(VesselClass *)>(0x0058CEE4);
    return func(this);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
#endif
}

void VesselClass::Combat_AI()
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(VesselClass *) = reinterpret_cast<void (*)(VesselClass *)>(0x0058D094);
    return func(this);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
#endif
}

void VesselClass::Repair_AI()
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(VesselClass *) = reinterpret_cast<void (*)(VesselClass *)>(0x0058D300);
    return func(this);
#else
    if (m_ToSelfRepair) {
        if (!(g_GameFrame % (Rule.Get_Repair_Rate() * 900))) {
            Mark(MARK_REDRAW);

            int cost = Class_Of().Repair_Cost();

            if (m_OwnerHouse->Available_Money() >= cost) {
                m_OwnerHouse->Spend_Money(cost);

                m_Health += Class_Of().Repair_Step();

                if (m_Health >= Class_Of().Get_Strength()) {
                    m_Health = Class_Of().Get_Strength();

                    m_Repairing = false;
                    m_ToSelfRepair = false;

                    if (m_PlayerOwned) {
                        Speak(VOX_UNIT_REPAIRED);
                    }
                }
            }
        }
    }
#endif
}

BOOL VesselClass::Edge_Of_World_AI()
{
    DEBUG_ASSERT(m_IsActive);

#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(VesselClass *) = reinterpret_cast<BOOL (*)(VesselClass *)>(0x0058D14C);
    return func(this);
#else
    if (m_Moving || Map.In_Radar(Get_Cell()) || !m_LockedOnMap) {
        return false;
    }

    // TODO: Requires TeamClass.
    /*if (m_Team != nullptr) {
        m_Team->Bit2_4 = true;
    }*/

    Stun();

    delete this;

    return true;
#endif
}

void VesselClass::Transport_Open_Door()
{
    DEBUG_ASSERT(m_IsActive);

    if (!m_Moving && !m_Rotating) {
        m_Door.Open_Door(TRANSPORT_DOOR_DELAY, TRANSPORT_DOOR_STAGES);
    }
}

void VesselClass::Transport_Close_Door()
{
    DEBUG_ASSERT(m_IsActive);

    m_Door.Close_Door(TRANSPORT_DOOR_DELAY, TRANSPORT_DOOR_STAGES);
}

void VesselClass::Read_INI(GameINIClass &ini)
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(GameINIClass &) = reinterpret_cast<void (*)(GameINIClass &)>(0x0058C95C);
    return func(ini);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called!\n");
#endif
}

void VesselClass::Write_INI(GameINIClass &ini)
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(GameINIClass &) = reinterpret_cast<void (*)(GameINIClass &)>(0x0058CC18);
    return func(ini);
#else
    char entrybuff[12];
    char valuebuff[GameINIClass::MAX_LINE_LENGTH];
    ini.Clear("SHIPS");
    for (int index = 0; index < g_Vessels.Count(); ++index) {
        VesselClass *thisptr = g_Vessels.Ptr(index);
        if (thisptr != nullptr) {
            if (thisptr->Is_Active() && thisptr->In_Limbo()) {
                snprintf(entrybuff, sizeof(entrybuff), "%d", index);
                // Format.
                // [HousesType],[VesselType],[Health],[CellNum],[DirType],[MissionType],[Trigger]
                snprintf(valuebuff,
                         sizeof(valuebuff),
                         "%s,%s,%d,%u,%d,%s,%s",
                         thisptr->m_OwnerHouse->Get_Name(),
                         thisptr->Name(),
                         thisptr->Health_Ratio() * 256,
                         thisptr->Get_Cell(),
                         thisptr->m_Facing.Get_Current(),
                         MissionClass::Name_From(thisptr->Get_Mission()),
                         ""//(thisptr->m_AttachedTrigger->Name()) // TODO: Requires TriggerClass.
                         );
                ini.Put_String("SHIPS", entrybuff, valuebuff);
            }
        }
    }
#endif
}

void VesselClass::Init()
{
    g_Vessels.Free_All();
}
