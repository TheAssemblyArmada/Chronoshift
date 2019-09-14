/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Pathfinding layer of game unit class hierachy.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef FOOT_H
#define FOOT_H

#include "always.h"
#include "fixed.h"
#include "techno.h"

class TeamClass;

struct PathType
{
    cell_t StartCell;
    int32_t Score; // How costly is it to traverse this route?
    int32_t Length; // How many moves is this route?
    FacingType *Moves; // Array of directions, moves to make each cell.
    uint32_t *Overlap; // Flags of which cells have been checked?
    cell_t PreviousCell; // Possible name, not 100%
    cell_t UnravelCheckpoint;
};

class FootClass : public TechnoClass
{
    enum
    {
        NAV_LENGTH = 10, // Length of our nav list.
        PATH_LENGTH = 12, // Length of path we store to act on.
        GEN_PATH_LENGTH = 200, // Length of path we actually generate.
    };

public:
    FootClass(RTTIType type, int id, HousesType house);
    FootClass(const FootClass &that);
    FootClass(const NoInitClass &noinit);
    virtual ~FootClass();

    // AbstractClass
    virtual MoveType Can_Enter_Cell(cell_t cellnum, FacingType facing = FACING_NONE) const override;
    virtual void AI() override;

    // ObjectClass
    virtual BOOL Can_Demolish() const override;
    virtual coord_t Sort_Y() const override;
    virtual BOOL Unlimbo(coord_t coord, DirType dir = DIR_NORTH) override;
    virtual void Detach(target_t target, int a2) override;
    virtual void Detach_All(int a1 = 1) override;
    virtual BOOL Mark(MarkType mark) override;
    virtual void Active_Click_With(ActionType action, ObjectClass *object) override;
    virtual void Active_Click_With(ActionType action, cell_t cellnum) override;
    virtual DamageResultType Take_Damage(
        int &damage, int a2, WarheadType warhead, TechnoClass *object = nullptr, BOOL a5 = false) override;
    virtual void Per_Cell_Process(PCPType pcp) override;
    virtual RadioMessageType Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target) override;
    virtual void Sell_Back(int a1) override;
    virtual void Code_Pointers() override;
    virtual void Decode_Pointers() override;

    // MissionClass
    virtual int Mission_Attack() override;
    virtual int Mission_Capture() override;
    virtual int Mission_Guard() override;
    virtual int Mission_Guard_Area() override;
    virtual int Mission_Hunt() override;
    virtual int Mission_Move() override;
    virtual int Mission_Retreat() override;
    virtual int Mission_Enter() override;
    virtual void Override_Mission(MissionType mission, int target1 = -1, int target2 = -1) override;
    virtual BOOL Restore_Mission() override;

    // TechnoClass
    virtual void Stun() override;
    virtual void Death_Announcement(TechnoClass *killer) const override;
    virtual target_t Greatest_Threat(ThreatType threat) override;
    virtual void Assign_Destination(target_t dest) override;

    // FootClass
    virtual coord_t Likely_Coord();
    virtual BOOL Start_Driver(coord_t &dest);
    virtual BOOL Stop_Driver();
    virtual BOOL Offload_Ore_Bail();
    virtual void Approach_Target();
    virtual void Fixup_Path(PathType *path);
    virtual void Set_Speed(int speed) { m_Speed = speed; }

    PathType *Find_Path(cell_t dest, FacingType *facing, int length, MoveType move);
    BOOL Basic_Path();
    BOOL Unravel_Loop(PathType *path, cell_t &cell, FacingType &facing, int x1, int y1, int x2, int y2, MoveType move);
    BOOL Register_Cell(PathType *path, cell_t cell, FacingType facing, int cost, MoveType move);
    BOOL Follow_Edge(cell_t start, cell_t destination, PathType *path, FacingType chirality, FacingType facing, int threat,
        int threat_state, int length, MoveType move);
    int Optimize_Moves(PathType *path, MoveType move);
    cell_t Safety_Point(cell_t start_cell, cell_t end_cell, int start, int end);
    int Passable_Cell(cell_t cell, FacingType facing, int threat, MoveType move);

    coord_t Head_To() const { return m_HeadTo; }
    FacingType Get_Path_Facing(int index) const { return m_Paths[index]; }
    target_t Nav_Com() const { return m_NavCom; }

    static int Point_Relative_To_Line(int px, int py, int sx, int sy, int ex, int ey);
    PathType *Find_Path_Wrapper(cell_t dest, FacingType *buffer, int length, MoveType move);

#ifdef GAME_DLL
    BOOL Hook_Can_Demolish() { return FootClass::Can_Demolish(); }
    coord_t Hook_Sort_Y() { return FootClass::Sort_Y(); }
#endif

protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_Bit1_1 : 1; // 1
    BOOL m_Initiated : 1; // 2
    BOOL m_Bit1_4 : 1; // 4
    BOOL m_ToLook : 1; // 8
    BOOL m_Deploying : 1; // 16
    BOOL m_Firing : 1; // 32
    BOOL m_Rotating : 1; // 64
    BOOL m_Moving : 1; // 128
    BOOL m_Bit2_1 : 1; // 1
    BOOL m_InFormation : 1; // 2
    BOOL m_Bit2_4 : 1; // 4
    BOOL m_ToScatter : 1; // 8
    BOOL m_Bit2_16 : 1; // 16
#else
    bool m_Bit1_1;
    bool m_Initiated;
    bool m_Bit1_4;
    bool m_ToLook;
    bool m_Deploying;
    bool m_Firing;
    bool m_Rotating;
    bool m_Moving;
    bool m_Bit2_1;
    bool m_InFormation;
    bool m_Bit2_4;
    bool m_ToScatter;
    bool m_Bit2_16;
#endif

    int32_t m_Speed;
    fixed_t m_SpeedMult;
    int32_t m_FormXCoefficient;
    int32_t m_FormYCoefficient;
    target_t m_NavCom;
    target_t m_SuspendedNavCom;
    target_t m_NavList[NAV_LENGTH];
    GamePtr<TeamClass> m_Team;
    TeamNumberType m_field_113;
    FootClass *m_field_114; // Next team member?
    FacingType m_Paths[PATH_LENGTH];
    MoveType m_PathMove; // The move type the current path was generated with.
    TCountDownTimerClass<FrameTimerClass> m_PathDelay;
    uint32_t m_field_12E;
    TCountDownTimerClass<FrameTimerClass> m_BaseDefenseDelay;
    SpeedType m_TeamSpeed;
    MPHType m_TeamMaxSpeed;
    coord_t m_HeadTo;
};

#endif // FOOT_H
