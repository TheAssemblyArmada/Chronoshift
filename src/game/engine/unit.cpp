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
#include "unit.h"
#include "iomap.h"
#include "team.h"

#ifndef GAME_DLL
TFixedIHeapClass<UnitClass> g_Units;
#endif

/**
 * @brief 
 * @address 0x00
 */
// TODO: Correct values for members.
UnitClass::UnitClass(UnitType type, HousesType house) :
    DriveClass(RTTI_UNIT, g_Units.ID(this), house),
    m_Type(),
    m_FlagOwner(),
    m_IsDumping(),
    m_Gold(),
    m_Gems(),
    m_ToScatter(),
    m_BailCount(),
    m_GapGenCellTracker(),
    m_GapGenCenterCell(),
    m_V2RearmDelayTimer(),
    m_TurretFacing()
{
}

/**
 * @brief 
 * @address 0x00
 */
UnitClass::~UnitClass()
{
}

/**
 * @brief 
 * @address 0x00
 */
void *UnitClass::operator new(size_t size)
{
    UnitClass *this_ptr = g_Units.Alloc();
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = true;
    }
    return this_ptr;
}

/**
 * @brief 
 * @address 0x00
 */
void UnitClass::operator delete(void *ptr)
{
    UnitClass *this_ptr = static_cast<UnitClass *>(ptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = false;
    }
    g_Units.Free(this_ptr);
}

/**
 * @brief 
 * @address 0x00
 */
MoveType UnitClass::Can_Enter_Cell(cell_t cellnum, FacingType facing) const
{
    return MoveType();
}

/**
 * @brief 
 * @address 0x00
 */
void UnitClass::AI()
{
}

/**
 * @brief 
 * @address 0x00
 */
ActionType UnitClass::What_Action(ObjectClass *object) const
{
    return ActionType();
}

/**
 * @brief 
 * @address 0x00
 */
ActionType UnitClass::What_Action(cell_t cellnum) const
{
    return ActionType();
}

/**
 * @brief 
 * @address 0x00
 */
coord_t UnitClass::Sort_Y() const
{
    return coord_t();
}

/**
 * @brief 
 * @address 0x00
 */
BOOL UnitClass::Limbo()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
BOOL UnitClass::Unlimbo(coord_t coord, DirType dir)
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
const int16_t *UnitClass::Overlap_List(BOOL a1) const
{
    return nullptr;
}

/**
 * @brief 
 * @address 0x00
 */
void UnitClass::Draw_It(int x, int y, WindowNumberType window) const
{
}

/**
 * @brief 
 * @address 0x00
 */
void UnitClass::Active_Click_With(ActionType action, ObjectClass *object)
{
}

/**
 * @brief 
 * @address 0x00
 */
void UnitClass::Active_Click_With(ActionType action, cell_t cellnum)
{
}

/**
 * @brief 
 * @address 0x00
 */
DamageResultType UnitClass::Take_Damage(int &damage, int a2, WarheadType warhead, TechnoClass *object, BOOL a5)
{
    return DamageResultType();
}

/**
 * @brief 
 * @address 0x00
 */
void UnitClass::Scatter(coord_t coord, int a2, BOOL a3)
{
}

/**
 * @brief 
 * @address 0x00
 */
void UnitClass::Per_Cell_Process(PCPType pcp)
{
}

/**
 * @brief 
 * @address 0x00
 */
RadioMessageType UnitClass::Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target)
{
    return RadioMessageType();
}

/**
 * @brief 
 * @address 0x00
 */
int UnitClass::Mission_Guard()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
int UnitClass::Mission_Guard_Area()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
int UnitClass::Mission_Harvest()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
int UnitClass::Mission_Hunt()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
int UnitClass::Mission_Move()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
int UnitClass::Mission_Unload()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
int UnitClass::Mission_Repair()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
DirType UnitClass::Turret_Facing() const
{
    return DirType();
}

/**
 * @brief 
 * @address 0x00
 */
DirType UnitClass::Desired_Load_Dir(ObjectClass *object, cell_t &cellnum) const
{
    return DirType();
}

/**
 * @brief 
 * @address 0x00
 */
DirType UnitClass::Fire_Direction() const
{
    return DirType();
}

/**
 * @brief 
 * @address 0x00
 */
InfantryType UnitClass::Crew_Type() const
{
    return InfantryType();
}

/**
 * @brief 
 * @address 0x00
 */
fixed_t UnitClass::Ore_Load() const
{
    return fixed_t();
}

/**
 * @brief 
 * @address 0x00
 */
int UnitClass::Pip_Count() const
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
FireErrorType UnitClass::Can_Fire(target_t target, WeaponSlotType weapon) const
{
    return FireErrorType();
}

/**
 * @brief 
 * @address 0x00
 */
target_t UnitClass::Greatest_Threat(ThreatType threat)
{
    return target_t();
}

/**
 * @brief 
 * @address 0x00
 */
BulletClass *UnitClass::Fire_At(target_t target, WeaponSlotType weapon)
{
    return nullptr;
}

/**
 * @brief 
 * @address 0x00
 */
void UnitClass::Assign_Destination(target_t dest)
{
}

/**
 * @brief 
 * @address 0x00
 */
void UnitClass::Enter_Idle_Mode(BOOL a1)
{
}

/**
 * @brief 
 * @address 0x00
 */
BOOL UnitClass::Start_Driver(coord_t &dest)
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
BOOL UnitClass::Offload_Ore_Bail()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
void UnitClass::Approach_Target()
{
}

/**
 * @brief 
 * @address 0x00
 */
void UnitClass::Overrun_Cell(cell_t cell, int a2)
{
}

/**
 * @brief 
 * @address 0x00
 */
BOOL UnitClass::Ok_To_Move(DirType dir)
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
BOOL UnitClass::Flag_Attach(HousesType house)
{
    if (house == HOUSES_NONE || m_FlagOwner != HOUSES_NONE) {
        return false;
    }

    m_FlagOwner = house;
    Mark(MARK_REDRAW);

    return true;
}

/**
 * @brief 
 * @address 0x00
 */
BOOL UnitClass::Flag_Remove()
{
    if (m_FlagOwner == HOUSES_NONE) {
        return false;
    }

    m_FlagOwner = HOUSES_NONE;
    Mark(MARK_REDRAW);

    return true;
}

/**
 * @brief 
 * @address 0x00
 */
void UnitClass::Init()
{
    g_Units.Free_All();
}

/**
 * @brief 
 * @address 0x00
 */
void UnitClass::Read_INI(GameINIClass &ini)
{
}

/**
 * @brief 
 * @address 0x00
 */
void UnitClass::Write_INI(GameINIClass &ini)
{
}

/**
 * @brief 
 * @address 0x00
 */
void UnitClass::Rotation_AI()
{
}

/**
 * @brief 
 * @address 0x00
 */
BOOL UnitClass::Edge_Of_World_AI()
{
    if (Mission != MISSION_GUARD || Map.In_Radar(Coord_To_Cell(m_Coord)) || !m_LockedOnMap) {
        return false;
    }

    if (m_Team.Is_Valid()) {
        m_Team->Set_Bit2_4(true);
    }

    Stun();
    delete this;
}

/**
 * @brief 
 * @address 0x00
 */
void UnitClass::Reload_AI()
{
}

/**
 * @brief 
 * @address 0x00
 */
void UnitClass::Firing_AI()
{
}

/**
 * @brief 
 * @address 0x00
 */
BOOL UnitClass::Goto_Clear_Spot()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
BOOL UnitClass::Try_To_Deploy()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
int UnitClass::Shape_Number() const
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
BOOL UnitClass::Ore_Check(cell_t &cell, int a2, int a3)
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
BOOL UnitClass::Goto_Ore(int a1)
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
BOOL UnitClass::Harvesting()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
void UnitClass::Exit_Repair()
{
}

/**
 * @brief 
 * @address 0x00
 */
void UnitClass::Transport_Close_Door()
{
}

/**
 * @brief 
 * @address 0x00
 */
void UnitClass::Transport_Open_Door()
{
}

/**
 * @brief 
 * @address 0x00
 */
int UnitClass::Credit_Load() const
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
BOOL UnitClass::Should_Crush_It(TechnoClass* object) const
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
void UnitClass::Shroud_Regen()
{
}
