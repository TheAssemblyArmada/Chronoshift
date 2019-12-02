/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Techno layer of the object class hierachy.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "techno.h"
#include "bullettype.h"
#include "coord.h"
#include "display.h"
#include "drawshape.h"
#include "foot.h"
#include "gameoptions.h"
#include "gbuffer.h"
#include "globals.h"
#include "house.h"
#include "infantrytype.h"
#include "iomap.h"
#include "keyboard.h"
#include "queue.h"
#include "remap.h"
#include "rules.h"
#include "scenario.h"
#include "special.h"
#include "target.h"
#include "weapontype.h"
#include <algorithm>

using std::min;

int const TechnoClass::BodyShape32[32] = {
    0, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1
};

/**
 *
 *
 * @address 0x00560DC8
 */
TechnoClass::TechnoClass(RTTIType type, int id, HousesType house) :
    RadioClass(type, house),
    m_Flasher(),
    m_AnimStage(),
    m_Cargo(),
    m_Door(),
    m_KillCount(0),
    m_IsUseless(false),
    m_IsTickedOff(false),
    m_Cloakable(false),
    m_IsPrimary(false),
    m_IsALoner(false),
    m_LockedOnMap(false),
    m_IsRecoiling(false),
    m_Tethered(false),
    m_PlayerOwned(false),
    m_PlayerAware(false),
    m_AIAware(false),
    m_Lemon(false),
    m_Bit2_16(true),
    m_ArmorMult(),
    m_FirepowerMult(),
    m_IdleActionTimer(),
    m_InvulnerabilityTimer(),
    m_Spied(0),
    m_Archive(0),
    m_OwnerHouse(HouseClass::As_Pointer(house)),
    m_CloakState(CLOAK_UNCLOAKED),
    m_CloakingStage(),
    m_CloakDelayTimer(),
    m_TarCom(0),
    m_SuspendedTarCom(0),
    m_Facing(),
    m_RearmTimer(),
    m_Ammo(-1),
    m_Price(0)
{
}

TechnoClass::TechnoClass(const TechnoClass &that) :
    RadioClass(that),
    m_Flasher(that.m_Flasher),
    m_AnimStage(that.m_AnimStage),
    m_Cargo(that.m_Cargo),
    m_Door(that.m_Door),
    m_KillCount(that.m_KillCount),
    m_IsUseless(that.m_IsUseless),
    m_IsTickedOff(that.m_IsTickedOff),
    m_Cloakable(that.m_Cloakable),
    m_IsPrimary(that.m_IsPrimary),
    m_IsALoner(that.m_IsALoner),
    m_LockedOnMap(that.m_LockedOnMap),
    m_IsRecoiling(that.m_IsRecoiling),
    m_Tethered(that.m_Tethered),
    m_PlayerOwned(that.m_PlayerOwned),
    m_PlayerAware(that.m_PlayerAware),
    m_AIAware(that.m_AIAware),
    m_Lemon(that.m_Lemon),
    m_Bit2_16(that.m_Bit2_16),
    m_ArmorMult(that.m_ArmorMult),
    m_FirepowerMult(that.m_FirepowerMult),
    m_IdleActionTimer(that.m_IdleActionTimer),
    m_InvulnerabilityTimer(that.m_InvulnerabilityTimer),
    m_Spied(that.m_Spied),
    m_Archive(that.m_Archive),
    m_OwnerHouse(that.m_OwnerHouse),
    m_CloakState(that.m_CloakState),
    m_CloakingStage(that.m_CloakingStage),
    m_CloakDelayTimer(that.m_CloakDelayTimer),
    m_TarCom(that.m_TarCom),
    m_SuspendedTarCom(that.m_SuspendedTarCom),
    m_Facing(that.m_Facing),
    m_RearmTimer(that.m_RearmTimer),
    m_Ammo(that.m_Ammo),
    m_Price(that.m_Price)
{
}

/**
 *
 *
 * @address 0x004CDAA0
 */
TechnoClass::TechnoClass(const NoInitClass &noinit) : 
    RadioClass(noinit),
    m_Flasher(noinit),
    m_AnimStage(noinit),
    m_Cargo(noinit),
    m_Door(noinit),
    m_IdleActionTimer(noinit),
    m_InvulnerabilityTimer(noinit),
    m_CloakingStage(noinit),
    m_CloakDelayTimer(noinit),
    m_Facing(noinit),
    m_RearmTimer(noinit)
{
}

#ifdef CHRONOSHIFT_DEBUG
void TechnoClass::Debug_Dump(MonoClass *mono) const
{
    RadioClass::Debug_Dump(mono);
}
#endif

/**
 *
 *
 * @address 0x00564358
 */
HousesType TechnoClass::Owner() const
{
    return m_OwnerHouse->What_Type();
}

void TechnoClass::AI()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x005643C8, void, TechnoClass *);
    func(this);
#endif
}

BOOL TechnoClass::Is_Player_Army() const
{
    return 0;
}

ActionType TechnoClass::What_Action(ObjectClass *object) const
{
    return ActionType();
}

ActionType TechnoClass::What_Action(cell_t cellnum) const
{
    return ActionType();
}

/**
 *
 *
 * @address 0x00568574
 */
int TechnoClass::Get_Ownable() const
{
    return Techno_Class_Of().Get_Ownable();
}

/**
 *
 *
 * @address 0x00566154
 */
BOOL TechnoClass::Can_Repair() const
{
    if (m_RTTI != RTTI_BUILDING) {
        return false;
    }
    if (!Techno_Class_Of().Is_Repairable()) {
        return false;
    }
    if (m_Health != Techno_Class_Of().Get_Strength()) {
        return true;
    }
    return false;
}

/**
 *
 *
 * @address 0x005660C0
 */
BOOL TechnoClass::Can_Player_Fire() const
{
    if(!m_OwnerHouse->Player_Has_Control()) {
        return false;
    }
    if (Is_Techno() && Is_Weapon_Equipped()) {
        return true;
    }
    return false;
}

/**
 *
 *
 * @address 0x00566088
 */
BOOL TechnoClass::Can_Player_Move() const
{
    return m_OwnerHouse->Player_Has_Control();
}

/**
 *
 *
 */
coord_t TechnoClass::Fire_Coord(WeaponSlotType weapon) const
{
    DirType dir = Turret_Facing();

    int x, y, z;
    Techno_Class_Of().Get_Fire_Offsets(weapon, x, y, z);
    z += m_Height;

    coord_t coord = Coord_Move(Center_Coord(), DIR_NORTH, z);

    if (m_Bit2_16) {
        coord = Coord_Move(coord, dir + 64, y);
    } else {
        coord = Coord_Move(coord, dir - 64, y);
    }

    return Coord_Move(coord, dir, x);
}

/**
 *
 *
 */
BOOL TechnoClass::Unlimbo(coord_t coord, DirType dir)
{
    if (!RadioClass::Unlimbo(coord, dir)) {
        return false;
    }
    m_Facing = dir;
    Enter_Idle_Mode(true);
    Commence();
    m_LockedOnMap = Map.In_Radar(Get_Cell());
    return true;
}

/**
 *
 *
 * @address 0x005673AC
 */
void TechnoClass::Detach(target_t target, int a2)
{
    ObjectClass::Detach(target, a2);
    if (SuspendedMission != MISSION_NONE && m_SuspendedTarCom == target) {
        SuspendedMission = MISSION_NONE;
        m_SuspendedTarCom = 0;
    }
    if (m_TarCom == target) {
        Assign_Target(0);
        Restore_Mission();
    }
    if (a2 && m_Radio != nullptr && m_Radio->As_Target() == target) {
        Transmit_Message(RADIO_OVER_AND_OUT);
    }
}

void TechnoClass::Record_The_Kill(TechnoClass *object)
{
}

/**
 *
 *
 * @address 0x00566EE8
 */
void TechnoClass::Do_Shimmer()
{
    //this function is quite different in TD

    Do_Uncloak();
}

/**
 *
 *
 * @address 0x005686B4
 */
int TechnoClass::Exit_Object(TechnoClass *object)
{
    return 0;
}

/**
 *
 *
 * @address 0x005618E4
 */
void TechnoClass::Draw_It(int x, int y, WindowNumberType window) const
{
    m_Door.Clear_To_Redraw();

    if (m_Selected || Special.Always_Show_Health()) {
        GraphicViewPortClass rect(g_logicPage->Get_Graphic_Buffer(),
            g_logicPage->Get_XPos() + WindowList[WINDOW_TACTICAL].X,
            g_logicPage->Get_YPos() + WindowList[WINDOW_TACTICAL].Y,
            WindowList[WINDOW_TACTICAL].W,
            WindowList[WINDOW_TACTICAL].H);

        if (m_RTTI == RTTI_INFANTRY) {
            y -= 6;
        }
        if (m_RTTI == RTTI_BUILDING
            && reinterpret_cast<const BuildingTypeClass &>(Class_Of()).What_Type() == BUILDING_BARR) {
            y -= 5;
        }

        int dim_w = 0;
        int dim_h = 0;
        Class_Of().Dimensions(dim_w, dim_h);

        // draw health bar
        if (m_Health > 0) {
            if (m_OwnerHouse->Is_Ally(g_PlayerPtr) || Rule.Show_Enemy_Health()) {
                fixed_t health = Health_Ratio();
                int bar_left = x - dim_w / 2;
                int bar_top = y - dim_h / 2;

                // fade background of health bar
                rect.Remap(bar_left + 1, bar_top + 1, dim_w - 1, 2, DisplayClass::FadingShade);

                // draw health bar bounding box
                rect.Draw_Rect(bar_left, bar_top, bar_left + dim_w - 1, bar_top + 3, 12);
                int health_w = std::clamp((dim_w - 2) * health, 1, dim_w - 2);
                ColorType healthcolor = COLOR_LTGREEN;
                if (health <= Rule.Condition_Yellow()) {
                    healthcolor = COLOR_YELLOW;
                }
                if (health <= Rule.Condition_Red()) {
                    healthcolor = COLOR_RED;
                }

                // draw health bar itself
                rect.Fill_Rect(bar_left + 1, bar_top + 1, bar_left + health_w, bar_top + 2, healthcolor);
            }
        }

        // draw selection box
        if (m_Selected) {
            int center_x = dim_w / 2;
            int center_y = dim_h / 2;
            int line_w = dim_w / 5;
            int line_h = dim_h / 5;

            int adj_y = 0;

            // if we are showing the health bar nudge the selection box a bit
            if (m_OwnerHouse->Is_Ally(g_PlayerPtr) || Rule.Show_Enemy_Health()) {
                adj_y = 4;
            }

            //this seems useless cause we did that already....
            if (m_RTTI == RTTI_VESSEL) {
                center_x = dim_w / 2;
            }

            // left center
            int left = x - center_x;
            // right center
            int right = x + center_x;
            // top center
            int top = y + adj_y - center_y;
            // bottom center
            int bottom = y + center_y;

            // left horizonal line
            int h_l = left + line_w;
            // right horizonal line
            int h_r = right - line_w;
            // top vertical line
            int v_t = top + line_h;
            // bottom vertical line
            int v_b = bottom - line_h;

            // draw top left
            rect.Draw_Line(left, top, h_l, top, COLOR_WHITE);
            rect.Draw_Line(left, top, left, v_t, COLOR_WHITE);

            // draw top right
            rect.Draw_Line(right, top, h_r, top, COLOR_WHITE);
            rect.Draw_Line(right, top, right, v_t, COLOR_WHITE);

            // draw bottom right
            rect.Draw_Line(right, bottom, h_r, bottom, COLOR_WHITE);
            rect.Draw_Line(right, bottom, right, v_b, COLOR_WHITE);

            // draw bottom left
            rect.Draw_Line(left, bottom, h_l, bottom, COLOR_WHITE);
            rect.Draw_Line(left, bottom, left, v_b, COLOR_WHITE);

            if (m_OwnerHouse->Is_Ally(g_PlayerPtr) || m_OwnerHouse->Spied_My_Radar(g_PlayerPtr)) {
                Draw_Pips((x - center_x) + 5, (y + center_y) - 3, window);
            }
        }
    }
}

/**
 *
 *
 * @address 0x00561444
 */
void TechnoClass::Hidden()
{
    if (m_PlayerAware && !m_OwnerHouse->Is_Human()) {
        m_PlayerAware = false;
    }
}

/**
 *
 *
 * @address 0x0056950C
 */
void TechnoClass::Look(BOOL a1)
{
    int sight = Techno_Class_Of().Get_Sight();
    if (sight > 0) {
        Map.Sight_From(Get_Cell(), sight, m_OwnerHouse, a1);
    }
}

/**
 *
 *
 * @address 0x00561480
 */
BOOL TechnoClass::Mark(MarkType mark)
{
    if (!ObjectClass::Mark(mark)) {
        return false;
    }
    if (m_Tethered) {
        Transmit_Message(RADIO_WE_BUMPED);
    }
    return true;
}

/**
 *
 *
 * @address 0x005643B4
 */
void TechnoClass::Clicked_As_Target(int flash_frames)
{
    m_Flasher.Flash(flash_frames, true);
}

/**
 *
 *
 * @address 0x00564C34
 */
BOOL TechnoClass::Select()
{
    if (!m_PlayerAware && (!m_OwnerHouse->Player_Has_Control() && !g_Debug_Unshroud)) {
        return false;
    }
    if (ObjectClass::Select()) {
        if (m_OwnerHouse->Player_Has_Control() && g_AllowVoice) {
            Response_Select();
        }
        return true;
    }
    return false;
}

BOOL TechnoClass::In_Range(coord_t a1, WeaponSlotType weapon) const
{
    return 0;
}

/**
 *
 *
 * @address 0x005661A0
 */
int TechnoClass::Weapon_Range(WeaponSlotType weapon) const
{
    if (weapon != WEAPON_SLOT_NONE && weapon < WEAPON_SLOT_COUNT) {
        const WeaponTypeClass *wtptr = Techno_Class_Of().Get_Weapon(weapon);

        if (wtptr != nullptr) {
            return wtptr->Get_Range();
        }
    }
    return 0;
}

DamageResultType TechnoClass::Take_Damage(int &damage, int a2, WarheadType warhead, TechnoClass *object, BOOL a5)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00566438, DamageResultType, TechnoClass *, int &, int, WarheadType, TechnoClass *, BOOL);
    return func(this, damage, a2, warhead, object, a5);
#else
    return DamageResultType();
#endif
}

int TechnoClass::Value() const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x0056756C, int, const TechnoClass *);
    return func(this);
#else
    return 0;
#endif
}

/**
 *
 *
 * @address 0x00561870
 */
void TechnoClass::Per_Cell_Process(PCPType pcp)
{
    if (pcp == PCP_2) {
        cell_t cell = Get_Cell();
        if (!m_LockedOnMap && Map.In_Radar(cell)) {
            m_LockedOnMap = true;
        }
        if (!m_PlayerAware) {
            if (Map[cell].Is_Revealed()) {
                Revealed(g_PlayerPtr);
            }
        }
    }
}

RadioMessageType TechnoClass::Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x005614C0, RadioMessageType, TechnoClass *, RadioClass *, RadioMessageType, target_t &);
    return func(this, radio, message, target);
#else
    return RadioMessageType();
#endif
}

/**
 *
 *
 */
BOOL TechnoClass::Revealed(HouseClass *house)
{
    if (house == g_PlayerPtr && m_PlayerAware) {
        return false;
    }

    if (house != g_PlayerPtr) {
        if (m_AIAware) {
            return false;
        }
        m_AIAware = true;
    }

    if (!RadioClass::Revealed(house)) {
        return false;
    }

    if (!house->Is_Human() && Mission == MISSION_AMBUSH) {
        Assign_Mission(MISSION_HUNT);
    }

    if (house == g_PlayerPtr) {
        m_PlayerAware = true;
        if (!m_PlayerOwned) {
            if (ScenarioInit == 0 && m_AttachedTrigger != nullptr) {
                m_AttachedTrigger->Spring(TEVENT_DISCOVERED_BY_PLAYER, this);
            }
            m_OwnerHouse->Set_Discovered(true);
            return true;
        }
        Look();
        return true;
    }
    m_AIAware = true;
    return true;
}

/**
 *
 *
 * @address 0x004F984C
 */
void TechnoClass::Code_Pointers()
{
    m_Cargo.Code_Pointers();
    RadioClass::Code_Pointers();
}

/**
 *
 *
 * @address 0x004F9868
 */
void TechnoClass::Decode_Pointers()
{
    m_Cargo.Decode_Pointers();
    RadioClass::Decode_Pointers();
}

/**
 *
 *
 * @address 0x005661E8
 */
void TechnoClass::Override_Mission(MissionType mission, int target1, int target2)
{
    m_SuspendedTarCom = m_TarCom;
    MissionClass::Override_Mission(mission);
    Assign_Target(target1);
}

/**
 *
 *
 * @address 0x0056621C
 */
BOOL TechnoClass::Restore_Mission()
{
    if(!MissionClass::Restore_Mission()){
        return false;
    }
    Assign_Target(m_SuspendedTarCom);
    return true;
}

/**
 *
 *
 * @address 0x00560C9C
 */
int TechnoClass::How_Many_Survivors() const
{
    return Techno_Class_Of().Is_Crewed() ? 1 : 0;
}

/**
 *
 *
 * @address 0x00423E00
 */
DirType TechnoClass::Turret_Facing() const
{
    return m_Facing.Get_Current();
}

BuildingClass *TechnoClass::Find_Docking_Bay(BuildingType building, int a2) const
{
    return nullptr;
}

/**
 *
 *
 * @address 0x00569220
 */
cell_t TechnoClass::Find_Exit_Cell(TechnoClass *object) const
{
    return Coord_To_Cell(Docking_Coord());
}

/**
 * Get the desired direction to face to load up.
 *
 * @address 0x005685C4
 */
DirType TechnoClass::Desired_Load_Dir(ObjectClass *object, cell_t &cellnum) const
{
    cellnum = 0;

    return DIR_NORTH;
}

/**
 * Get the direction we are currently able to fire in.
 *
 * @address 0x005685E0
 */
DirType TechnoClass::Fire_Direction() const
{
    return Turret_Facing();
}

/**
 *
 *
 */
InfantryType TechnoClass::Crew_Type() const
{
    if (!Techno_Class_Of().Is_Crewed()) {
        return INFANTRY_NONE;
    }
    if (m_OwnerHouse->Acts_Like() == HOUSES_NEUTRAL) {
        return (InfantryType)Scen.Get_Random_Value(INFANTRY_C1, INFANTRY_C9);
    }
    if (Techno_Class_Of().Get_Weapon(WEAPON_SLOT_PRIMARY) == nullptr && Scen.Get_Random_Value(0, 99) < 50){
        if (Scen.Get_Random_Value(0, 99) < 50) {
            return INFANTRY_C1;
        }
        return INFANTRY_C7;
    }
    return INFANTRY_E1;
}

/**
 *
 *
 * @address 0x00560D1C
 */
BOOL TechnoClass::Is_Allowed_To_Recloak() const
{
    return true;
}

/**
 *
 *
 * @address 0x00566134
 */
BOOL TechnoClass::Is_Weapon_Equipped() const
{
    return Techno_Class_Of().Get_Weapon(WEAPON_SLOT_PRIMARY) != nullptr;
}

/**
 *
 *
 */
int TechnoClass::Rearm_Delay(BOOL a1, WeaponSlotType weapon) const
{
    if (m_RTTI == RTTI_BUILDING && m_Ammo > 1) {
        return 1;
    }
    WeaponTypeClass *wptr = Techno_Class_Of().Get_Weapon(weapon);
    if (a1 && wptr != nullptr) {
        return m_OwnerHouse->Get_ROF_Multiplier() * wptr->Get_ROF();
    }
    return 3;
}

int TechnoClass::Refund_Amount() const
{
    return 0;
}

int TechnoClass::Threat_Range(int a1) const
{
    return 0;
}

/**
 *
 *
 * @address 0x005685F8
 */
void TechnoClass::Response_Select()
{
    //empty
}

/**
 *
 *
 * @address 0x00568600
 */
void TechnoClass::Response_Move()
{
    //empty
}

/**
 *
 *
 * @address 0x00568600
 */
void TechnoClass::Response_Attack()
{
    //empty
}

/**
 *
 *
 */
void TechnoClass::Player_Assign_Mission(MissionType mission, target_t target, target_t dest)
{
    if (g_AllowVoice) {
        if (mission == MISSION_ATTACK) {
            Response_Attack();
        } else {
            Response_Move();
        }
    }
    if (g_FormMove) {
        Queue_Mission_Formation(TargetClass(this), mission, target, dest, g_FormSpeed, g_FormMaxSpeed);
    } else {
        if (mission == MISSION_MOVE && g_keyboard->Down(Options.Get_KeyQueueMove1())
            || g_keyboard->Down(Options.Get_KeyQueueMove1())) {
            mission = MISSION_QMOVE;
        }
        Queue_Mission(TargetClass(this), mission, target, dest);
    }
}

/**
 *
 *
 * @address 0x00423E20
 */
int TechnoClass::Made_A_Kill()
{
    return ++m_KillCount;
}

/**
 *
 *
 */
BOOL TechnoClass::Target_Something_Nearby(ThreatType threat)
{
    threat = threat & (THREAT_2 | THREAT_1);
    if (Target_Legal(m_TarCom)) {
        if (threat & THREAT_1) {
            if (In_Range(What_Weapon_Should_I_Use(m_TarCom))) {
                Assign_Target(0);
            }
        }
    }
    if (!Target_Legal(m_TarCom)) {
        Assign_Target(Greatest_Threat(threat));
    }
    return Target_Legal(m_TarCom);
}

/**
 *
 *
 * @address 0x00564F3C
 */
void TechnoClass::Stun()
{
    Assign_Target(0);
    Assign_Destination(0);
    Transmit_Message(RADIO_OVER_AND_OUT);
    Detach_All(true);
    Unselect();
}

BOOL TechnoClass::In_Range(target_t target, WeaponSlotType weapon) const
{
    return 0;
}

BOOL TechnoClass::In_Range(ObjectClass *object, WeaponSlotType weapon) const
{
    return 0;
}

FireErrorType TechnoClass::Can_Fire(target_t target, WeaponSlotType weapon) const
{
    return FireErrorType();
}

target_t TechnoClass::Greatest_Threat(ThreatType threat)
{
    return target_t();
}

void TechnoClass::Assign_Target(target_t target)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00564F98, void, TechnoClass *, target_t);
    func(this, target);
#endif
}

BulletClass *TechnoClass::Fire_At(target_t target, WeaponSlotType weapon)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x005652F8, BulletClass *, TechnoClass *, target_t, WeaponSlotType);
    return func(this, target, weapon);
#else
    return nullptr;
#endif
}

BOOL TechnoClass::Captured(HouseClass *house)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00566290, BOOL, TechnoClass *, HouseClass *);
    return func(this, house);
#else
    return false;
#endif
}

BOOL TechnoClass::Electric_Zap(target_t target, BOOL a2, coord_t a3, uint8_t *a4)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x005650B0, BOOL, TechnoClass *, target_t, BOOL, coord_t, uint8_t *);
    return func(this, target, a2, a3, a4);
#else
    return false;
#endif
}

/**
 *
 *
 * @address 0x0056624C
 */
void TechnoClass::Renovate()
{
    Mark(MARK_REDRAW);
    m_Health = Techno_Class_Of().Get_Strength();
    if (What_Am_I() == RTTI_BUILDING) {
        Repair(0);
    }
}

/**
 *
 *
 * @address 0x0056733C
 */
uint8_t *TechnoClass::Remap_Table() const
{
    if (Techno_Class_Of().Is_Remapable()) {
        return (uint8_t *)m_OwnerHouse->Remap_Table(m_Flasher.Get_Flashed(), Techno_Class_Of().Get_Remap());
    }
    return ColorRemaps[REMAP_0].RemapPalette;
}

void TechnoClass::Draw_Pips(int x, int y, WindowNumberType window) const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00568710, void, const TechnoClass *, int, int, WindowNumberType);
    func(this, x, y, window);
#endif
}

void TechnoClass::Do_Uncloak()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00566D88, void, TechnoClass *);
    func(this);
#endif
}

void TechnoClass::Do_Cloak()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00566E34, void, TechnoClass *);
    func(this);
#endif
}

/**
 *
 *
 * @address 0x005686C0
 */
BOOL TechnoClass::Is_Ready_To_Random_Animate() const
{
    return m_IdleActionTimer.Expired();
}

/**
 *
 *
 * @address 0x00423E40
 */
BOOL TechnoClass::Random_Animate()
{
    return false;
}

/**
 *
 *
 * @address 0x00568700
 */
void TechnoClass::Assign_Destination(target_t dest)
{
    // empty
}

/**
 *
 *
 * @address 0x00568708
 */
void TechnoClass::Enter_Idle_Mode(BOOL a1)
{
    // empty
}

/**
 *
 *
 */
void TechnoClass::Set_Player_Owned()
{
    m_PlayerOwned = (m_OwnerHouse == g_PlayerPtr);
}

/**
 *
 *
 * @address 0x0056706C
 */
void TechnoClass::Techno_Draw_Object(
    const void *shape, int frame, int x, int y, WindowNumberType window, DirType dir, int scale) const
{
    DEBUG_ASSERT(m_IsActive);

    if (shape != nullptr) {

        VisualType visual = Visual_Character();

        void *remap_table = Remap_Table();
        void *shadow_table = DisplayClass::UnitShadow;

        const ObjectTypeClass &type = Class_Of();

        int frame_count = Get_Build_Frame_Count((void *)shape);

        if (type.Get_Image_Data() == shape && frame_count - 1 > frame) {
            type.Init_Frame_Dimensions(frame_count);
            type.Set_Frame_Dimensions((void *)shape, frame);
        }

        if (m_Height > 0) {
            shadow_table = DisplayClass::UnitShadowAir;
        }

        y -= Lepton_To_Pixel(Get_Height());

        if (m_RTTI == RTTI_INFANTRY) {
            if (!m_PlayerOwned) {
                if (reinterpret_cast<const InfantryTypeClass &>(Class_Of()).What_Type() == INFANTRY_SPY) {
                    remap_table = (void *)g_PlayerPtr->Remap_Table(false, REMAP_1);
                }
            }

            if (reinterpret_cast<const InfantryTypeClass &>(Class_Of()).Has_Alt_Remap()) {
                remap_table = (void *)reinterpret_cast<const InfantryTypeClass &>(Class_Of()).Alt_Remap_Table();
            }
        }

        if (!m_InvulnerabilityTimer.Expired()) {
            remap_table = DisplayClass::FadingRed;
        }

        ShapeFlags flags = SHAPE_WIN_REL | SHAPE_CENTER;

        // *Enhancement* In the original RA, all units were treated as vessels are here. Here they are treated separately and
        // all other units are treated as they were in C&C instead. This should give land units the C&C stealth tank effect
        // when cloaked.
        switch (What_Am_I()) {
            case RTTI_VESSEL:
                switch (visual) {
                    case VISUAL_NORMAL:
                        CC_Draw_Shape((void *)shape,
                            frame,
                            x,
                            y,
                            window,
                            flags | SHAPE_GHOST | SHAPE_FADING,
                            remap_table,
                            shadow_table,
                            dir,
                            scale);
                        break;

                    case VISUAL_INDISTINCT:
                    case VISUAL_DARKEN:
                        CC_Draw_Shape((void *)shape,
                            frame,
                            x,
                            y,
                            window,
                            flags | SHAPE_FADING,
                            remap_table,
                            DisplayClass::FadingShade,
                            dir,
                            scale);
                        break;

                    case VISUAL_SHADOWY:
                    case VISUAL_RIPPLE:
                        CC_Draw_Shape((void *)shape,
                            frame,
                            x,
                            y,
                            window,
                            flags | SHAPE_PREDATOR | SHAPE_FADING,
                            nullptr,
                            DisplayClass::FadingShade,
                            dir,
                            scale);
                        break;

                    case VISUAL_HIDDEN:
                        break;

                    default:
                        break;
                }

                break;
            default:
                if (visual != VISUAL_HIDDEN && visual != VISUAL_RIPPLE) {
                    if (visual == VISUAL_SHADOWY) {
                        CC_Draw_Shape((void *)shape,
                            frame,
                            x,
                            y,
                            window,
                            flags | SHAPE_FADING | SHAPE_PREDATOR,
                            nullptr,
                            DisplayClass::FadingShade,
                            dir,
                            scale);

                    } else {
                        CC_Draw_Shape((void *)shape,
                            frame,
                            x,
                            y,
                            window,
                            flags | SHAPE_GHOST | SHAPE_FADING,
                            remap_table,
                            shadow_table,
                            dir,
                            scale);
                    }

                    if (visual == VISUAL_DARKEN) {
                        CC_Draw_Shape((void *)shape,
                            frame,
                            x,
                            y,
                            window,
                            flags | SHAPE_FADING | SHAPE_PREDATOR,
                            remap_table,
                            DisplayClass::FadingShade,
                            dir,
                            scale);
                    }
                }

                // This draws for all except VISUAL_NORMAL and VISUAL_HIDDEN, provides predator effect over whatever is drawn
                // above. This is the major difference between C&C and RA in addition to the stages being more nuanced.
                if (visual != VISUAL_NORMAL && visual != VISUAL_HIDDEN) {
                    CC_Draw_Shape((void *)shape, frame, x, y, window, flags | SHAPE_PREDATOR, nullptr, nullptr, dir, scale);
                }

                break;
        }
    }
}

int TechnoClass::Anti_Air()
{
    if (!Is_Weapon_Equipped()) {
        return 0;
    }

    const TechnoTypeClass &tech_type = reinterpret_cast<const TechnoTypeClass &>(Class_Of());
    WeaponTypeClass *weapon = tech_type.Get_Weapon(WEAPON_SLOT_PRIMARY);

    if (!weapon->Get_Projectile()->Is_Anti_Air()) {
        return 0;
    }

    int effectiveness = weapon->Get_Damage() * weapon->Get_Warhead()->Get_Verses(ARMOR_LIGHT) * weapon->Get_Range();

    if (tech_type.Is_Two_Shooter()) {
        effectiveness *= 2;
    }

    return effectiveness / 50;
}

int TechnoClass::Anti_Armor()
{
    if (!Is_Weapon_Equipped()) {
        return 0;
    }

    const TechnoTypeClass &tech_type = reinterpret_cast<const TechnoTypeClass &>(Class_Of());
    WeaponTypeClass *weapon = tech_type.Get_Weapon(WEAPON_SLOT_PRIMARY);

    if (!weapon->Get_Projectile()->Is_Anti_Ground()) {
        return 0;
    }

    lepton_t range = min<lepton_t>(weapon->Get_Range(), 1024);
    int effectiveness = weapon->Get_Damage() * weapon->Get_Warhead()->Get_Verses(ARMOR_HEAVY) * range
        * weapon->Get_Warhead()->Get_Spread() / weapon->Get_ROF();

    if (tech_type.Is_Two_Shooter()) {
        effectiveness *= 2;
    }

    if (weapon->Get_Projectile()->Is_Inaccurate()) {
        effectiveness /= 2;
    }

    return effectiveness / 50;
}

int TechnoClass::Anti_Infantry()
{
    if (!Is_Weapon_Equipped()) {
        return 0;
    }

    const TechnoTypeClass &tech_type = reinterpret_cast<const TechnoTypeClass &>(Class_Of());
    WeaponTypeClass *weapon = tech_type.Get_Weapon(WEAPON_SLOT_PRIMARY);

    if (!weapon->Get_Projectile()->Is_Anti_Ground()) {
        return 0;
    }

    lepton_t range = min<lepton_t>(weapon->Get_Range(), 1024);
    int effectiveness = weapon->Get_Damage() * weapon->Get_Warhead()->Get_Verses(ARMOR_NONE) * range
        * weapon->Get_Warhead()->Get_Spread() / weapon->Get_ROF();

    if (tech_type.Is_Two_Shooter()) {
        effectiveness *= 2;
    }

    if (weapon->Get_Projectile()->Is_Inaccurate()) {
        effectiveness /= 2;
    }

    return effectiveness / 50;
}

/**
 *
 *
 */
BOOL TechnoClass::Can_Teleport_Here(cell_t cell) const
{
    if (m_RTTI == RTTI_INFANTRY){
        return false;
    }
    if (Map.In_Radar(cell)) {
        if (Map[cell].Get_Overlay() == OVERLAY_FPLS) {
            return false;
        }
        const TechnoTypeClass *ttptr = &Techno_Class_Of();
        return Map[cell].Is_Clear_To_Move(ttptr->Get_Speed(),
            true,
            true,
            -1,
            ttptr->Get_Speed() == SPEED_FLOAT ? MZONE_AMPHIBIOUS_DESTROYER : MZONE_NORMAL);
    }
    return false;
}

/**
 *
 *
 * @address 0x00566EFC
 */
VisualType TechnoClass::Visual_Character(BOOL flag) const
{
    DEBUG_ASSERT(m_IsActive);

    if (Techno_Class_Of().Is_Invisible() && m_PlayerOwned) {
        return VISUAL_NORMAL;
    }

    if (Techno_Class_Of().Is_Invisible() && !m_PlayerOwned && !g_InMapEditor) {
        return VISUAL_HIDDEN;
    }
    if (m_CloakState == CLOAK_UNCLOAKED || g_InMapEditor) {
        return VISUAL_NORMAL;
    }

    if (m_CloakState == CLOAK_CLOAKED) {
        if (!flag && m_PlayerOwned) {
            return VISUAL_SHADOWY;
        }
    } else {
        int32_t value = m_CloakingStage.Get_Stage();

        if (m_CloakState == CLOAK_UNCLOAKING) {
            value = 38 - m_CloakingStage.Get_Stage();
        }

        if (value <= 0) {
            return VISUAL_NORMAL;
        }

        int stage = fixed_t(value, 38) * 256;

        if (stage < 64) {
            return VISUAL_INDISTINCT;
        }

        if (stage < 128) {
            return VISUAL_DARKEN;
        }

        if (stage < 192) {
            return VISUAL_SHADOWY;
        }

        if (!flag && m_PlayerOwned) {
            return VISUAL_SHADOWY;
        }

        if (stage < 255) {
            return VISUAL_RIPPLE;
        }
    }

    return VISUAL_HIDDEN;
}

/**
 *
 *
 */
void TechnoClass::Kill_Cargo(TechnoClass *object)
{
    while (m_Cargo.Has_Cargo()) {
        FootClass *ptr = m_Cargo.Detach_Object();
        if (ptr != nullptr) {
            ptr->Record_The_Kill(object);
            delete ptr;
        }
    }
}

WeaponSlotType TechnoClass::What_Weapon_Should_I_Use(target_t target) const
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00560B14, WeaponSlotType, const TechnoClass *, target_t);
    return func(this, target);
#else
    return WeaponSlotType();
#endif
}

/**
 *
 *
 */
int TechnoClass::Combat_Damage(WeaponSlotType weapon) const
{
    const TechnoTypeClass *ttptr = &Techno_Class_Of();
    int weapons = 0;
    int damage = 0;
    WeaponTypeClass *wptr = nullptr;
    if (weapon == WEAPON_SLOT_PRIMARY || weapon == WEAPON_SLOT_NONE) {
        wptr = ttptr->Get_Weapon(WEAPON_SLOT_PRIMARY);
        if (wptr != nullptr) {
            damage = wptr->Get_Damage();
            ++weapons;
        }
    }
    if (weapon == WEAPON_SLOT_SECONDARY || weapon == WEAPON_SLOT_NONE) {
        wptr = ttptr->Get_Weapon(WEAPON_SLOT_SECONDARY);
        if (wptr != nullptr) {
            damage += wptr->Get_Damage();
            ++weapons;
        }
    }
    if (weapons > 1) {
        return damage / weapons;
    }
    return damage;
}

/**
 *
 *
 */
cell_t TechnoClass::Nearby_Location(TechnoClass *techno) const
{
    const TechnoTypeClass *ttptr = &Techno_Class_Of();
    SpeedType speed = ttptr->Get_Speed();
    if (speed == SPEED_WINGED) {
        speed = SPEED_TRACK;
    }
    cell_t cell = Coord_To_Cell(techno != nullptr ? techno->Center_Coord() : Center_Coord());
    MZoneType mzone = ttptr->Get_Movement_Zone();
    return Map.Nearby_Location(cell, speed, Map[cell].Get_Zone(mzone), mzone);
}
