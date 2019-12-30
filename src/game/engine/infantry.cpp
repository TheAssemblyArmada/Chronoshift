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
#include "infantry.h"
#include "building.h"
#include "iomap.h"
#include "rules.h"
#include "scenario.h"

#ifndef GAME_DLL
TFixedIHeapClass<InfantryClass> g_Infantry;
#endif

// clang-format off
InfantryClass::DoStruct InfantryClass::MasterDoControls[DO_COUNT] = {
#ifndef CHRONOSHIFT_NO_BITFIELDS
    { 1, 0 },
    { 1, 0 },
    { 1, 0 },
    { 1 | 2 | 4, 2 },
    { 1, 1 },
    { 2, 2 },
    { 1 | 2 | 4, 2 },
    { 0, 3 },
    { 1, 1 },
    { 1, 2 },
    { 1, 2 },
    { 0, 2 },
    { 0, 2 },
    { 0, 2 },
    { 0, 2 },
    { 0, 2 },
    { 0, 2 },
    { 0, 2 },
    { 0, 2 },
    { 0, 2 },
    { 0, 2 }
#else
    { true,  false, false, 0 }, // DO_READY
    { true,  false, false, 0 }, // DO_GUARD
    { true,  false, false, 0 }, // DO_PRONE
    { true,  true,  true,  2 }, // DO_WALK
    { true,  false, false, 1 }, // DO_FIRE_UP
    { false, true,  false, 2 }, // DO_DOWN
    { true,  true,  true,  2 }, // DO_CRAWL
    { false, false, false, 3 }, // DO_UP
    { true,  false, false, 1 }, // DO_FIRE_PRONE
    { true,  false, false, 2 }, // DO_IDLE1
    { true,  false, false, 2 }, // DO_IDLE2
    { false, false, false, 2 }, // DO_DIE1
    { false, false, false, 2 }, // DO_DIE2
    { false, false, false, 2 }, // DO_DIE3
    { false, false, false, 2 }, // DO_DIE4
    { false, false, false, 2 }, // DO_DIE5
    { false, false, false, 2 }, // DO_16
    { false, false, false, 2 }, // DO_17
    { false, false, false, 2 }, // DO_18
    { false, false, false, 2 }, // DO_19
    { false, false, false, 2 }  // DO_20
#endif
};
// clang-format on

InfantryClass::InfantryClass(InfantryType type, HousesType house) :
    FootClass(RTTI_INFANTRY, g_Infantry.ID(this), house),
    m_Doing(DO_NONE),
    m_StokeTimer(0),
    m_Technician(false),
    m_Stoked(false),
    m_Prone(false),
    m_Bit8(false),
    m_Bit16(false),
    m_Fear(0)
{
    m_OwnerHouse->Tracking_Add(this);

    m_Cloakable = Class_Of().Is_Cloakable();
    m_Bit2_16 = !Class_Of().Is_Two_Shooter();
    m_Health = Class_Of().Get_Strength();
    m_Ammo = Class_Of().Get_Ammo();
}

InfantryClass::InfantryClass(const InfantryClass &that) :
    FootClass(that),
    m_Class(that.m_Class),
    m_Doing(that.m_Doing),
    m_StokeTimer(that.m_StokeTimer),
    m_Technician(that.m_Technician),
    m_Stoked(that.m_Stoked),
    m_Prone(that.m_Prone),
    m_Bit8(that.m_Bit8),
    m_Bit16(that.m_Bit16),
    m_Fear(that.m_Fear)
{
}

InfantryClass::InfantryClass(const NoInitClass &noinit) :
    FootClass(noinit)
{
}

InfantryClass::~InfantryClass()
{
    if (g_GameActive) {
        if (m_Team != nullptr) {
            m_Team->Remove(this);
            m_Team = nullptr;
        }
        m_OwnerHouse->Tracking_Remove(this);
        Limbo();
    }
    m_Class = nullptr;
    m_Doing = DO_NONE;
}

void *InfantryClass::operator new(size_t size)
{
    InfantryClass *this_ptr = g_Infantry.Alloc();
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = true;
    }
    return this_ptr;
}

void InfantryClass::operator delete(void *ptr)
{
    InfantryClass *this_ptr = static_cast<InfantryClass *>(ptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = false;
    }
    g_Infantry.Free(this_ptr);
}

/**
 *
 *
 */
void *InfantryClass::Get_Image_Data() const
{
    if (What_Type() == INFANTRY_SPY && !m_PlayerOwned) {
        return InfantryTypeClass::As_Reference(INFANTRY_E1).Get_Image_Data();
    }

    return FootClass::Get_Image_Data();
}

/**
 *
 *
 */
BOOL InfantryClass::Paradrop(coord_t coord)
{
    if (!FootClass::Paradrop(coord)) {
        return false;
    }

    Assign_Mission(m_OwnerHouse->Is_Human() ? MISSION_GUARD : MISSION_HUNT);
    return true;
}

/**
 *
 *
 */
void InfantryClass::Draw_It(int x, int y, WindowNumberType window) const
{
    void *image = Get_Image_Data();
    if (image != nullptr) {
        Techno_Draw_Object(image, Shape_Number(), x - 2, y + 4, window);
        FootClass::Draw_It(x - 2, y + 4, window);
    }
}

/**
 *
 *
 */
void InfantryClass::Active_Click_With(ActionType action, cell_t cellnum)
{
    FootClass::Active_Click_With(action, cellnum);
}

/**
 *
 *
 */
void InfantryClass::Active_Click_With(ActionType action, ObjectClass *object)
{
    action = What_Action(object);
    switch (action) {
        case ACTION_CAPTURE:
        case ACTION_DAMAGE:
        case ACTION_GREPAIR:
            action = ACTION_CAPTURE;
            break;
        case ACTION_HEAL:
            action = ACTION_ATTACK;
            break;
        default:
            break;
    }

    FootClass::Active_Click_With(action, object);
}

/**
 *
 *
 */
int InfantryClass::Full_Name() const
{
    if (m_Technician) {
        return TXT_TECHNICIAN;
    }

    if (What_Type() == INFANTRY_SPY && !m_OwnerHouse->Player_Has_Control()){
        return InfantryTypeClass::As_Reference(INFANTRY_E1).Full_Name();
    }

    return Class_Of().Full_Name();
}

/**
 *
 *
 */
BOOL InfantryClass::Limbo()
{
    if (!m_InLimbo) {
        Stop_Driver();
        Clear_Occupy_Spot(m_Coord);
    }

    return FootClass::Limbo();
}

/**
 *
 *
 */
void InfantryClass::Detach(target_t target, int a2)
{
    if (m_TarCom == target) {
        Mark(MARK_5);
        m_Firing = false;
        Mark(MARK_4);
    }
    FootClass::Detach(target, a2);
}

/**
 *
 *
 */
int InfantryClass::Mission_Attack()
{
    if (Class_Of().Has_C4() && As_Building(m_TarCom) != nullptr) {
        Assign_Destination(m_TarCom);
        Assign_Mission(MISSION_SABOTAGE);
        return 1;
    }

    if (Class_Of().Is_Infiltrator() && As_Building(m_TarCom) != nullptr) {
        Assign_Destination(m_TarCom);
        Assign_Mission(MISSION_CAPTURE);
        return 1;
    }
    return FootClass::Mission_Attack();
}

/**
 *
 *
 */
FireErrorType InfantryClass::Can_Fire(target_t target, WeaponSlotType weapon) const
{
    TechnoClass *tptr = As_Techno(target);

    if (Combat_Damage() < 0 && (tptr == nullptr || tptr->Health_Ratio() >= g_Rule.Condition_Green())) {
        return FIRE_ILLEGAL;
    }

    if (m_Moving || Target_Legal(m_NavCom) && m_Doing != DO_NONE && !MasterDoControls[m_Doing].m_Bit1) {
        return FIRE_MOVING;
    }

    return FootClass::Can_Fire(target, weapon);
}

/**
 *
 *
 */
void InfantryClass::Assign_Target(target_t target)
{
    m_Paths[0] = FACING_NONE;

    ObjectClass *optr = As_Object(target);
    if (Class_Of().Is_Canine() && optr != nullptr && optr->What_Am_I() != RTTI_INFANTRY) {
        target = 0;
    }

    TechnoClass::Assign_Target(target);

    if (!Target_Legal(m_NavCom) && Class_Of().Is_Infiltrator() && !Is_Weapon_Equipped()) {
        BuildingClass *bptr = As_Building(target);
        if (bptr != nullptr) {
            if (bptr->Class_Of().Is_Capturable()){
                Assign_Destination(target);
            }
        }

    }
}

/**
 *
 *
 */
BulletClass *InfantryClass::Fire_At(target_t target, WeaponSlotType weapon)
{
    Mark(MARK_5);
    m_Firing = false;
    Mark(MARK_4);

    BulletClass *bullet = FootClass::Fire_At(target, weapon);
    if (bullet == nullptr || m_InLimbo) {
        return nullptr;
    }

    if (Class_Of().Is_Fraidycat() && m_Ammo == 0) {
        m_Fear = 255;
        if (m_Mission == MISSION_ATTACK || m_Mission == MISSION_HUNT) {
            Assign_Mission(MISSION_GUARD);
        }
    }
    return bullet;
}

/**
 *
 *
 */
BOOL InfantryClass::Is_Ready_To_Random_Animate() const
{
    if (!FootClass::Is_Ready_To_Random_Animate()) {
        return false;
    }
    if (m_Height > 0) {
        return false;
    }
    if (m_Moving) {
        return false;
    }
    if (m_Bit1_4) {
        return false;
    }
    if (m_Firing) {
        return false;
    }
    return m_Doing == DO_GUARD || m_Doing == DO_READY;
}

/**
 *
 *
 */
void InfantryClass::Assign_Destination(target_t dest)
{
    if (m_Moving && !m_InFormation && Target_Legal(dest)) {
        if (g_Map[Get_Cell()].Is_Clear_To_Move(Class_Of().Get_Speed(), true)) {
            Stop_Driver();
        }
    }

    if (m_OwnerHouse->Is_Human() && Target_Legal(dest) && m_NavCom == dest && m_Prone) {
        if (!Class_Of().Is_Fraidycat() && !Class_Of().Is_Canine()) {
            Do_Action(DO_UP);
        }
    }

    TechnoClass *tptr = As_Techno(dest);
    if (tptr != nullptr && (m_Mission == MISSION_ENTER || m_MissionQueue == MISSION_ENTER) && !Radio_Valid()) {
        if (tptr->Radio_Valid()) {
            m_Archive = dest;
        } else if (Transmit_Message(RADIO_HELLO, tptr) == RADIO_ROGER) {
            if (Transmit_Message(RADIO_TRYING_TO_LOAD) == RADIO_ROGER) {
                return;
            }
            Transmit_Message(RADIO_OVER_AND_OUT);
        }
    } else {
        m_Paths[0] = FACING_NONE;
    }
    FootClass::Assign_Destination(dest);
}

/**
 *
 *
 */
BOOL InfantryClass::Stop_Driver()
{
    if (m_HeadTo) {
        Clear_Occupy_Spot(m_HeadTo);
    }
    Set_Occupy_Spot(m_Coord);

    Do_Action((!Class_Of().Is_Canine() && m_Prone) ? DO_PRONE : DO_READY);

    m_Bit8 = Can_Enter_Cell(Get_Cell()) != MOVE_OK;

    return FootClass::Stop_Driver();
}

BOOL InfantryClass::Do_Action(DoType dotype, BOOL a1)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004EE980, BOOL, InfantryClass *, DoType, BOOL);
    return func(this, dotype, a1);
#else
    return false;
#endif
}

int InfantryClass::Shape_Number() const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004EC224, int, const InfantryClass *);
    return func(this);
#else
    return 0;
    //TODO recheck this as im not sure its correct
    /*
    static int HumanShape[32] = {
        0, 0, 7, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 5, 5, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0
    };

    DoType doing = m_Doing;
    if (doing == DO_NONE) {
        doing = DO_READY;
    }
    const DoInfoStruct &doinfo = Class_Of().Fetch_DoInfo(doing);

    int frames = (doinfo.m_NumFrames <= 1 ? 1 : doinfo.m_NumFrames);
    int number = m_AnimStage.Get_Stage() % frames;
    if (doinfo.m_FacingMultiplier > 0) {
        number += (doinfo.m_FacingMultiplier * HumanShape[FacingClass::s_Facing32[m_Facing.Get_Current()]]);
    }
    return number + doinfo.m_StartingFrame;
    */
#endif
}

/**
 *
 *
 */
void InfantryClass::Set_Occupy_Spot(cell_t cellnum, int spot_index)
{
    DEBUG_ASSERT(spot_index < CELL_SPOT_COUNT);

    CellClass &cell = g_Map[cellnum];
    cell.Set_Occupants(CellOccupantEnum(1 << spot_index));
    cell.Set_Owner(Owner());
}

/**
 *
 *
 */
void InfantryClass::Clear_Occupy_Spot(cell_t cellnum, int spot_index)
{
    DEBUG_ASSERT(spot_index < CELL_SPOT_COUNT);

    CellClass &cell = g_Map[cellnum];
    cell.Clear_Occupants(CellOccupantEnum(1 << spot_index));

    // If the cell isn't occupied by any infantry the cell isn't owned anymore
    if (!cell.Check_Occupants(OCCUPANT_INFANTRY)) {
        cell.Set_Owner(HOUSES_NONE);
    }
}

/**
 *
 *
 */
BOOL InfantryClass::Edge_Of_World_AI()
{
    if (m_Team != nullptr && m_LockedOnMap) {
        m_Team->Set_Bit2_4(true);
    }
    if (m_Team == nullptr && m_Mission == MISSION_GUARD && !g_Map.In_Radar(Get_Cell())) {
        Stun();
        delete this;
        return true;
    }
    return false;
}
