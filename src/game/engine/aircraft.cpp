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
#include "aircraft.h"
#include "infantry.h"
#include "scenario.h"

#ifndef GAME_DLL
TFixedIHeapClass<AircraftClass> g_Aircraft;
#endif

/**
 * @brief 
 * @address 0x00
 */
// TODO: Correct values for members.
AircraftClass::AircraftClass(AircraftType type, HousesType house) :
    FootClass(RTTI_AIRCRAFT, g_Aircraft.ID(this), house),
    m_Flyer(),
    m_Type(),
    m_field_14A(),
    m_field_14C(),
    m_Bit1(),
    m_Bit2(),
    m_Bit4(),
    m_Bit8(),
    m_Bit16(),
    m_field_154(),
    m_field_155(),
    m_field_15E()
{
}

/**
 * @brief 
 * @address 0x00
 */
AircraftClass::~AircraftClass()
{
}

/**
 * @brief 
 * @address 0x00
 */
void *AircraftClass::operator new(size_t size)
{
    return nullptr;
}

/**
 * @brief 
 * @address 0x00
 */
void AircraftClass::operator delete(void *ptr)
{
}

/**
 * @brief 
 * @address 0x00
 */
MoveType AircraftClass::Can_Enter_Cell(cell_t cellnum, FacingType facing) const
{
    return MoveType();
}

/**
 * @brief 
 * @address 0x00
 */
void AircraftClass::AI()
{
}

/**
 * @brief 
 * @address 0x00
 */
ActionType AircraftClass::What_Action(ObjectClass *object) const
{
    return ActionType();
}

/**
 * @brief 
 * @address 0x00
 */
ActionType AircraftClass::What_Action(cell_t cellnum) const
{
    return ActionType();
}

/**
 * @brief 
 * @address 0x00
 */
LayerType AircraftClass::In_Which_Layer() const
{
    return LayerType();
}

/**
 * @brief 
 * @address 0x00
 */
coord_t AircraftClass::Sort_Y() const
{
    return coord_t();
}

/**
 * @brief 
 * @address 0x00
 */
BOOL AircraftClass::Unlimbo(coord_t coord, DirType dir)
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
int AircraftClass::Exit_Object(TechnoClass *object)
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
const int16_t *AircraftClass::Overlap_List(BOOL a1) const
{
    return nullptr;
}

/**
 * @brief 
 * @address 0x00
 */
void AircraftClass::Draw_It(int x, int y, WindowNumberType window) const
{
}

/**
 * @brief 
 * @address 0x00
 */
void AircraftClass::Look(BOOL a1)
{
}

/**
 * @brief 
 * @address 0x00
 */
void AircraftClass::Active_Click_With(ActionType action, ObjectClass *object)
{
}

/**
 * @brief 
 * @address 0x00
 */
void AircraftClass::Active_Click_With(ActionType action, cell_t cellnum)
{
}

/**
 * @brief 
 * @address 0x00
 */
DamageResultType AircraftClass::Take_Damage(int &damage, int a2, WarheadType warhead, TechnoClass *object, BOOL a5)
{
    return DamageResultType();
}

/**
 * @brief 
 * @address 0x00
 */
void AircraftClass::Scatter(coord_t coord, int a2, BOOL a3)
{
}

/**
 * @brief 
 * @address 0x00
 */
void AircraftClass::Per_Cell_Process(PCPType pcp)
{
}

/**
 * @brief 
 * @address 0x00
 */
RadioMessageType AircraftClass::Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target)
{
    return RadioMessageType();
}

/**
 * @brief 
 * @address 0x00
 */
int AircraftClass::Mission_Attack()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
int AircraftClass::Mission_Guard()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
int AircraftClass::Mission_Guard_Area()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
int AircraftClass::Mission_Hunt()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
int AircraftClass::Mission_Move()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
int AircraftClass::Mission_Retreat()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
int AircraftClass::Mission_Unload()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
int AircraftClass::Mission_Enter()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
DirType AircraftClass::Turret_Facing() const
{
    return DirType();
}

/**
 * @brief 
 * @address 0x00
 */
DirType AircraftClass::Desired_Load_Dir(ObjectClass *object, cell_t &cellnum) const
{
    return DirType();
}

/**
 * @brief 
 * @address 0x00
 */
DirType AircraftClass::Fire_Direction() const
{
    return DirType();
}

/**
 * @brief 
 * @address 0x00
 */
int AircraftClass::Pip_Count() const
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
void AircraftClass::Response_Select()
{
}

/**
 * @brief 
 * @address 0x00
 */
void AircraftClass::Response_Move()
{
}

/**
 * @brief 
 * @address 0x00
 */
void AircraftClass::Response_Attack()
{
}

/**
 * @brief 
 * @address 0x00
 */
void AircraftClass::Player_Assign_Mission(MissionType mission, target_t target, target_t dest)
{
}

/**
 * @brief 
 * @address 0x00
 */
FireErrorType AircraftClass::Can_Fire(target_t target, WeaponSlotType weapon) const
{
    return FireErrorType();
}

/**
 * @brief 
 * @address 0x00
 */
BulletClass *AircraftClass::Fire_At(target_t target, WeaponSlotType weapon)
{
    return nullptr;
}

/**
 * @brief 
 * @address 0x00
 */
void AircraftClass::Assign_Destination(target_t dest)
{
}

/**
 * @brief 
 * @address 0x00
 */
void AircraftClass::Enter_Idle_Mode(BOOL a1)
{
}

/**
 * @brief 
 * @address 0x00
 */
void AircraftClass::Set_Speed(int speed)
{
}

/**
 * @brief 
 * @address 0x00
 */
DirType AircraftClass::Pose_Dir() const
{
    return DirType();
}

/**
 * @brief 
 * @address 0x00
 */
void AircraftClass::Init()
{
}

/**
 * @brief 
 * @address 0x00
 */
void AircraftClass::Read_INI(GameINIClass &ini)
{
}

/**
 * @brief 
 * @address 0x00
 */
void AircraftClass::Write_INI(GameINIClass &ini)
{
}

/**
 * @brief 
 * @address 0x00
 */
int AircraftClass::Shape_Number() const
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
void AircraftClass::Draw_Rotors(int x, int y, WindowNumberType window) const
{
}

/**
 * @brief 
 * @address 0x00
 */
int AircraftClass::Paradrop_Cargo()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
target_t AircraftClass::New_LZ(target_t target) const
{
    return target_t();
}

/**
 * @brief 
 * @address 0x00
 */
BOOL AircraftClass::Is_LZ_Clear(target_t target) const
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
int AircraftClass::Process_Fly_To(int a1, target_t a2)
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
BOOL AircraftClass::Process_Take_Off()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
BOOL AircraftClass::Process_Landing()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
target_t AircraftClass::Good_Fire_Location(target_t target) const
{
    return target_t();
}

/**
 * @brief 
 * @address 0x00
 */
BOOL AircraftClass::Cell_Seems_Ok(cell_t cell, int a2) const
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
target_t AircraftClass::Good_LZ() const
{
    return target_t();
}

/**
 * @brief 
 * @address 0x00
 */
BOOL AircraftClass::Landing_Takeoff_AI()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
BOOL AircraftClass::Edge_Of_World_AI()
{
    return 0;
}

/**
 * @brief 
 * @address 0x00
 */
void AircraftClass::Movement_AI()
{
}

/**
 * @brief 
 * @address 0x00
 */
void AircraftClass::Rotation_AI()
{
}

/**
 * @brief 
 * @address 0x00
 */
BOOL AircraftClass::Counts_As_Civ_Evac(ObjectClass *object)
{
    if (object == nullptr) {
        return false;
    }

    if (object->What_Am_I() != RTTI_INFANTRY) {
        return false;
    }

    InfantryClass *iobjptr = reinterpret_cast<InfantryClass *>(object);

    switch (iobjptr->What_Type()) {
        case INFANTRY_EINSTEIN:
        case INFANTRY_GENERAL:
        case INFANTRY_DELPHI:
        case INFANTRY_CHAN:
            return true;
        default:
            break;
    };

    if (Scen.Civ_Evac()) {
        if (iobjptr->What_Type() == INFANTRY_TANYA) {
            return true;
        }
    }

    if (!iobjptr->Class_Of().Is_Civilian()) {
        return false;
    }

    if (!iobjptr->Get_Technician()) {
        return true;

    }

    return false;
}
