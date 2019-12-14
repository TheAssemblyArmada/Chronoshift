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
#include "building.h"
#include "display.h"
#include "drawshape.h"
#include "globals.h"
#include "house.h"
#include "iomap.h"
#include "session.h"
#include "team.h"
#include "target.h"
#include "queue.h"

#ifndef GAME_DLL
TFixedIHeapClass<AircraftClass> g_Aircraft;
#endif

AircraftClass::AircraftClass(AircraftType type, HousesType house) :
    FootClass(RTTI_AIRCRAFT, g_Aircraft.ID(this), house)
{
}

AircraftClass::AircraftClass(const AircraftClass &that) :
    FootClass(that)
{
}

AircraftClass::AircraftClass(const NoInitClass &noinit) :
    FootClass(noinit)
{
}

AircraftClass::~AircraftClass()
{
}

/**
 * @brief
 *
 * @address 0x00421918
 */
MoveType AircraftClass::Can_Enter_Cell(cell_t cellnum, FacingType facing) const
{
    if (!Map.In_Radar(cellnum)) {
        return MOVE_NO;
    }
    CellClass &cell = Map[cellnum];
    TechnoClass *optr = reinterpret_cast<TechnoClass *>(cell.Get_Occupier());
    if (optr != nullptr && optr->Is_Techno()) {
        if (!optr->Get_Owner_House()->Is_Ally(m_OwnerHouse)) {
            if (optr->Cloak_State() == 2) {
                if (Session.Game_To_Play() == GAME_CAMPAIGN || !m_PlayerOwned || cell.Is_Visible()) {
                    return MOVE_OK;
                }
                return MOVE_NO;
            }
            if (optr->What_Am_I() == RTTI_BUILDING) {
                BuildingClass *bptr = reinterpret_cast<BuildingClass *>(optr);
                if (bptr->Class_Of().Is_Invisible()) {
                    if (Session.Game_To_Play() == GAME_CAMPAIGN || !m_PlayerOwned || cell.Is_Visible()) {
                        return MOVE_OK;
                    }
                    return MOVE_NO;
                }
            }
        }
    }
    if (!cell.Is_Clear_To_Move(SPEED_TRACK)) {
        return MOVE_NO;
    }
    if (Session.Game_To_Play() == GAME_CAMPAIGN || !m_PlayerOwned || cell.Is_Visible()) {
        return MOVE_OK;
    }
    return MOVE_NO;
}

/**
 * @brief
 *
 * @address 0x0041E0E8
 */
void AircraftClass::AI()
{
    if (!m_IsLanding && !m_IsTakingOff) {
        Commence();
    }
    FootClass::AI();
    if (m_IsActive) {
        if (!m_IsLanding && !m_IsTakingOff) {
            Commence();
        }
        Rotation_AI();
        Movement_AI();
        if (In_Which_Layer() != LAYER_GROUND) {
            Mark(MARK_REDRAW);
        }
        if (m_PlayerOwned) {
            if (Class_Of().Get_Sight() > 0) {
                if (m_LookDelayTimer.Expired()) {
                    Look();
                    m_LookDelayTimer = 15;
                }
            }
        }
        if (!Landing_Takeoff_AI()) {
            if (Map.In_View(Get_Cell())) {
                Map.Flag_To_Redraw();
                Map.Display_To_Redraw();
            }
            Edge_Of_World_AI();
        }
    }
}

ActionType AircraftClass::What_Action(ObjectClass *object) const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004202F0, ActionType, const AircraftClass *, ObjectClass *);
    return func(this, object);
#else
    return ACTION_NONE;
#endif
}

/**
 *
 *
 */
ActionType AircraftClass::What_Action(cell_t cellnum) const
{
    ActionType action = FootClass::What_Action(cellnum);
    if (action == ACTION_MOVE) {
        return ACTION_NO_MOVE;
    }
    if (action == ACTION_ATTACK) {
        if (Class_Of().Get_Weapon(WEAPON_SLOT_PRIMARY) == nullptr) {
            return ACTION_NONE;
        }
    }
    if (Class_Of().Is_Airplane() && action == ACTION_MOVE) {
        return ACTION_NO_MOVE;
    }
    return action;
}

LayerType AircraftClass::In_Which_Layer() const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00423BEC, LayerType, const AircraftClass *);
    return func(this);
#else
    return LAYER_NONE;
#endif
}

/**
 * @brief
 *
 * @address 0x0041EA98
 */
coord_t AircraftClass::Sort_Y() const
{
    // TODO figure out if we can represent the hex val as a from pixels or coord conversion
    return m_Coord + 0x800000;
}

BOOL AircraftClass::Unlimbo(coord_t coord, DirType dir)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041CEF0, BOOL, AircraftClass *, coord_t, DirType);
    return func(this, coord, dir);
#else
    return false;
#endif
}

int AircraftClass::Exit_Object(TechnoClass *object)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041EC50, int, const AircraftClass *, ObjectClass *);
    return func(this, object);
#else
    return 0;
#endif
}

const int16_t *AircraftClass::Overlap_List(BOOL a1) const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041E238, const int16_t *, const AircraftClass *, BOOL);
    return func(this, a1);
#else
    return nullptr;
#endif
}

/**
 * @brief
 *
 * @address 0x0041D25C
 */
void AircraftClass::Draw_It(int x, int y, WindowNumberType window) const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041D25C, void, const AircraftClass *, int, int, WindowNumberType);
    func(this, x, y, window);
#else
    static int _jitter[] = { 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, -1, -1, -1, 0 };

    void *image = Get_Image_Data();
    if (image != nullptr) {
        int frame = Shape_Number();
        DirType rot = DIR_NONE;
        if (Class_Of().Get_ROT_Count() == 16) {
            rot = (DirType)FacingClass::Rotation16[m_BDir.Get_Current()];
        }

        int jitter = 0;
        if (m_Height == 256 && m_FlyControl.Get_Speed() < 3) {
            jitter = _jitter[g_GameFrame % 16];
        }

        if (Visual_Character() <= VISUAL_DARKEN) {
            CC_Draw_Shape(image,
                frame,
                x + 1,
                y + 2,
                window,
                SHAPE_PREDATOR | SHAPE_FADING | SHAPE_CENTER | SHAPE_VIEWPORT_REL,
                DisplayClass::FadingShade);
        }
        Techno_Draw_Object(image, frame, x, jitter + y, window, rot);

        if (m_Class->Has_Rotors()) {
            Draw_Rotors(x, jitter + y, window);
        }

        // TODO, verify Lepton_To_Pixel is the right function to call
        int height = Lepton_To_Pixel(m_Height);
        // TODO, verify that the y arg is correct as it has optimized math
        TechnoClass::Draw_It(x, y - height, window);
    }
#endif
}

/**
 * @brief
 *
 * @address 0x00423C38
 */
void AircraftClass::Look(BOOL a1)
{
    int sight = Class_Of().Get_Sight();
    if (m_Height == 0) {
        sight = 1;
    }
    if (sight > 0) {
        Map.Sight_From(Get_Cell(), sight, m_OwnerHouse, a1);
    }
}

/**
 *
 *
 * @address 0x00420208
 */
void AircraftClass::Active_Click_With(ActionType action, ObjectClass *object)
{
    action = What_Action(object);
    switch (action) {
        case ACTION_ENTER:
            Player_Assign_Mission(MISSION_ENTER, 0, object->As_Target());
            break;
        case ACTION_SELF:
            Player_Assign_Mission(MISSION_UNLOAD);
            break;
        case ACTION_NO_MOVE:
            return;
        default:
            break;
    }
    FootClass::Active_Click_With(action, object);
}

/**
 * @brief
 *
 * @address 0x00420294
 */
void AircraftClass::Active_Click_With(ActionType action, cell_t cellnum)
{
    FootClass::Active_Click_With(action, cellnum);
}

DamageResultType AircraftClass::Take_Damage(int &damage, int a2, WarheadType warhead, TechnoClass *object, BOOL a5)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041F01C, DamageResultType, AircraftClass *, int &, int, WarheadType, TechnoClass *, BOOL);
    return func(this, damage, a2, warhead, object, a5);
#else
    return DAMAGE_NONE;
#endif
}

void AircraftClass::Scatter(coord_t coord, int a2, BOOL a3)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00422980, void, AircraftClass *, coord_t, int, BOOL);
    func(this, coord, a2, a3);
#endif
}

/**
 * @brief
 *
 * @address 0x00423B50
 */
void AircraftClass::Per_Cell_Process(PCPType pcp)
{
    FootClass::Per_Cell_Process(pcp);
}

RadioMessageType AircraftClass::Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00420F5C, RadioMessageType, AircraftClass *, RadioClass *, RadioMessageType, target_t &);
    return func(this, radio, message, target);
#else
    return RADIO_NONE;
#endif
}

int AircraftClass::Mission_Attack()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004206DC, int, AircraftClass *);
    return func(this);
#else
    return 0;
#endif
}

int AircraftClass::Mission_Guard()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004229F8, int, AircraftClass *);
    return func(this);
#else
    return 0;
#endif
}

int AircraftClass::Mission_Guard_Area()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00422FC8, int, AircraftClass *);
    return func(this);
#else
    return 0;
#endif
}

int AircraftClass::Mission_Hunt()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041D85C, int, AircraftClass *);
    return func(this);
#else
    return 0;
#endif
}

int AircraftClass::Mission_Move()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041F1F4, int, AircraftClass *);
    return func(this);
#else
    return 0;
#endif
}

int AircraftClass::Mission_Retreat()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041EAD0, int, AircraftClass *);
    return func(this);
#else
    return 0;
#endif
}

int AircraftClass::Mission_Unload()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041E588, int, AircraftClass *);
    return func(this);
#else
    return 0;
#endif
}

int AircraftClass::Mission_Enter()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00421E00, int, AircraftClass *);
    return func(this);
#else
    return 0;
#endif
}

/**
 * @brief
 *
 * @address 0x00423E60
 */
DirType AircraftClass::Turret_Facing() const
{
    return m_BDir.Get_Current();
}

/**
 * @brief
 *
 * @address 0x00421464
 */
DirType AircraftClass::Desired_Load_Dir(ObjectClass *object, cell_t &cellnum) const
{
    // this seems quickly modified to return always north so not sure if there's any point in this code
    cell_t cell = Get_Cell();
    for (int i = 0; i < 4; ++i) {
        cellnum = Cell_Get_Adjacent(cell, SS_41B73C(FACING_SOUTH, i));
        if (Map.In_Radar(cellnum)) {
            if (object->Get_Cell() == cellnum || Map[cellnum].Is_Clear_To_Move(SPEED_FOOT)) {
                return DIR_NORTH;
            }
        }

        cellnum = cellnum = Cell_Get_Adjacent(cell, SS_41B710(FACING_SOUTH, i));
        if (Map.In_Radar(cellnum)) {
            if (object->Get_Cell() == cellnum || Map[cellnum].Is_Clear_To_Move(SPEED_FOOT)) {
                return DIR_NORTH;
            }
        }
    }
    return DIR_NORTH;
}

/**
 * @brief
 *
 * @address 0x00422780
 */
DirType AircraftClass::Fire_Direction() const
{
    return m_BDir.Get_Current();
}

/**
 * @brief
 *
 * @address 0x00421D04
 */
int AircraftClass::Pip_Count() const
{
    int count = 0;
    if (Class_Of().Max_Passengers() > 0) {
        return m_Cargo.Cargo_Count();
    }
    if (m_Ammo > 0) {
        fixed_t ammo(m_Ammo, Class_Of().Get_Ammo());
        count = ammo * Class_Of().Max_Pips();
        if (!count) {
            count = 1;
        }
    }
    return count;
}

/**
 * @brief
 *
 * @address 0x00423154
 */
void AircraftClass::Response_Select()
{
    static VocType _response[] = {
        VOC_VEHIC1,
        VOC_REPORT1,
        VOC_YESSIR1,
        VOC_AWAIT1
    };

    VocType voc = _response[g_nonCriticalRandom(0, ARRAY_SIZE(_response) - 1)];
    if (g_AllowVoice) {
        Sound_Effect(voc, fixed_t(1, 1), -(Get_Heap_ID() + 1));
    }
}

/**
 * @brief
 *
 * @address 0x004230FC
 */
void AircraftClass::Response_Move()
{
    static VocType _response[] = {
        VOC_ACKNO,
        VOC_AFFIRM1,
    };

    VocType voc = _response[g_nonCriticalRandom(0, ARRAY_SIZE(_response) - 1)];
    if (g_AllowVoice) {
        Sound_Effect(voc, fixed_t(1, 1), -(Get_Heap_ID() + 1));
    }
}

/**
 * @brief
 *
 * @address 0x004230A4
 */
void AircraftClass::Response_Attack()
{
    static VocType _response[] = {
        VOC_AFFIRM1,
        VOC_ACKNO,
    };

    VocType voc = _response[g_nonCriticalRandom(0, ARRAY_SIZE(_response) - 1)];
    if (g_AllowVoice) {
        Sound_Effect(voc, fixed_t(1, 1), -(Get_Heap_ID() + 1));
    }
}

/**
 *
 *
 * @address 0x004202A8
 */
void AircraftClass::Player_Assign_Mission(MissionType mission, target_t target, target_t dest)
{
    if (g_AllowVoice) {
        if (mission == MISSION_ATTACK) {
            Response_Attack();
        } else {
            Response_Move();
        }
    }
    Queue_Mission(TargetClass(this), mission, target, dest);
}

FireErrorType AircraftClass::Can_Fire(target_t target, WeaponSlotType weapon) const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004231AC, FireErrorType, const AircraftClass *, target_t, WeaponSlotType);
    return func(this, target, weapon);
#else
    return FIRE_NONE;
#endif
}

BulletClass *AircraftClass::Fire_At(target_t target, WeaponSlotType weapon)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041EE90, BulletClass *, AircraftClass *, target_t, WeaponSlotType);
    return func(this, target, weapon);
#else
    return nullptr;
#endif
}

/**
 * @brief
 *
 * @address 0x00423B60
 */
void AircraftClass::Assign_Destination(target_t dest)
{
    if (dest != m_NavCom) {
        if (dest != 0) {
            // TODO, investigate, doing dest != m_NavCom again is useless...
            if (Class_Of().Is_Airplane() && (m_IsLanding || m_NavCom != 0 && dest != m_NavCom)) {
                Process_Take_Off();
                m_Status = STATUS_0;
            }
        }
        FootClass::Assign_Destination(dest);
    }
}

void AircraftClass::Enter_Idle_Mode(BOOL a1)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041FA08, void, AircraftClass *, BOOL);
    func(this, a1);
#endif
}

/**
 * @brief
 *
 * @address 0x0042263C
 */
void AircraftClass::Set_Speed(int speed)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0042263C, void, AircraftClass *, int);
    func(this, speed);
#else
    FootClass::Set_Speed(speed);
    // TODO, check if math correct, it uses a lot of fixed values
    int mphval = m_OwnerHouse->Get_Airspeed_Multiplier() * (Class_Of().Get_MPH() * m_SpeedMult);
    m_FlyControl.Fly_Speed(speed, (MPHType)std::min(mphval, 255));
#endif
}

int AircraftClass::Shape_Number() const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041D160, int, const AircraftClass *);
    return func(this);
#else
    return 0;
#endif
}

void AircraftClass::Draw_Rotors(int x, int y, WindowNumberType window) const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0041D3F4, void, const AircraftClass *, int, int, WindowNumberType);
    func(this, x, y, window);
#endif
}

void AircraftClass::Rotation_AI()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00423A2C, void, AircraftClass *);
    func(this);
#endif
}

/**
 * @brief
 *
 * @address 0x00423958
 */
void AircraftClass::Movement_AI()
{
    if (m_NavCom != 0 && m_Mission == MISSION_GUARD && m_MissionQueue == MISSION_NONE) {
        Assign_Mission(MISSION_MOVE);
    }
    if (m_Speed > 0) {
        if (In_Which_Layer() == LAYER_GROUND) {
            Mark(MARK_REMOVE);
            m_FlyControl.Physics(m_Coord, Get_Facing().Get_Current());
            Mark(MARK_PUT);
        } else {
            Mark(MARK_3);
            if (m_FlyControl.Physics(m_Coord, Get_Facing().Get_Current())) {
                Mark(MARK_3);
            }
        }
    }
}

BOOL AircraftClass::Landing_Takeoff_AI()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00423498, BOOL, AircraftClass *);
    return func(this);
#else
    return false;
#endif
}

/**
 * @brief
 *
 * @address 0x0042381C
 */
BOOL AircraftClass::Edge_Of_World_AI()
{
    if (Map.In_Radar(Get_Cell())) {
        m_LockedOnMap = true;
        return false;
    }
    if (m_Mission != MISSION_RETREAT) {
        return false;
    }
    while (m_Cargo.Attached_Object()) {
        FootClass *obj = reinterpret_cast<FootClass *>(m_Cargo.Detach_Object());

        if (obj->Counts_As_Civ_Evac()) {
            obj->Get_Owner_House()->Set_Civilians_Evacuated(true);
        }

        if (obj->Get_Team()) {
            obj->Get_Team()->Set_Bit2_4(true);
        }
        if (obj != nullptr) {
            delete obj;
        }
    }

    Stun();
    delete this;
    return true;
}

BOOL AircraftClass::Process_Take_Off()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00421580, BOOL, AircraftClass *);
    return func(this);
#else
    return false;
#endif
}

/**
 *
 *
 * @address 0x0041E9F4
 */
BOOL AircraftClass::Is_LZ_Clear(target_t lzone)
{
/*
    if (lzone == 0) {
        return false;
    }
    cell_t cell = As_Cell(lzone);
    if (!Map.In_Radar(cell)) {
        return false;
    }
    ObjectClass *optr = Map[cell].Cell_Object();
    if (optr != nullptr && optr != this) {
        if (m_Radio == nullptr || m_Radio != optr) {
            return false;
        }
    } else if (!Map[cell].Is_Clear_To_Move(SPEED_TRACK)) {
        return false;
    }
    return true;
*/
    if (!Target_Legal(lzone)) {
        return false;
    }
    cell_t cell = As_Cell(lzone);
    if (!Map.In_Radar(cell)) {
        return false;
    }
    ObjectClass *optr = Map[cell].Cell_Object();
    if (optr != nullptr) {
        if (optr == this) {
            return true;
        }
        return Radio_Valid() && m_Radio == optr;
    }
    return Map[cell].Is_Clear_To_Move(SPEED_TRACK);
}

/**
 *
 *
 */
DirType AircraftClass::Pose_Dir() const
{
    if (What_Type() == AIRCRAFT_TRANSPORT) {
        return DIR_NORTH;
    }
    if (Class_Of().Is_Airplane()){
        return DIR_EAST;
    }
    return DIR_NORTH_EAST;
}
