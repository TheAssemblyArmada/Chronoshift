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
#include "drive.h"
#include "gametypes.h"
#include "globals.h"
#include "iomap.h"
#include "rules.h"
#include "scenario.h"
#include "target.h"
#include "unit.h"
#include "voc.h"

DriveClass::DriveClass(RTTIType type, int id, HousesType house) :
    FootClass(type, id, house),
    m_IsHarvesting(false),
    m_Teleported(false),
    m_Bit_4(false),
    m_Bit_8(false),
    m_field_145(),
    m_TeleportReturnLocation(0),
    m_SpeedAccum(0),
    m_TrackNumber(-1),
    m_TrackIndex(0)
{
}

DriveClass::DriveClass(const DriveClass &that) :
    FootClass(that),
    m_IsHarvesting(that.m_IsHarvesting),
    m_Teleported(that.m_Teleported),
    m_Bit_4(that.m_Bit_4),
    m_Bit_8(that.m_Bit_8),
    m_field_145(that.m_field_145),
    m_TeleportReturnLocation(that.m_TeleportReturnLocation),
    m_SpeedAccum(that.m_SpeedAccum),
    m_TrackNumber(that.m_TrackNumber),
    m_TrackIndex(that.m_TrackIndex)
{
}

DriveClass::DriveClass(const NoInitClass &noinit) :
    FootClass(noinit),
    m_field_145(noinit)
{
}

#ifdef CHRONOSHIFT_DEBUG
void DriveClass::Debug_Dump(MonoClass *mono) const
{
    FootClass::Debug_Dump(mono);
}
#endif

void DriveClass::AI()
{
#ifdef GAME_DLL
    void (*func)(DriveClass *) = reinterpret_cast<void (*)(DriveClass *)>(0x004B7CA8);
    func(this);
#else
    captainslog_dbgassert(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
#endif
}

BOOL DriveClass::Limbo()
{
/*#ifdef GAME_DLL
    BOOL (*func)(DriveClass *) = reinterpret_cast<BOOL (*)(DriveClass *)>(0x004B6488);
    return func(this);
#else*/
    if (!m_InLimbo) {
        Stop_Driver();
        m_TrackNumber = -1;
    }
    return FootClass::Limbo();
//#endif
}

void DriveClass::Scatter(coord_t coord, BOOL a2, BOOL a3)
{
#ifdef GAME_DLL
    void (*func)(DriveClass *, coord_t, BOOL, BOOL) = reinterpret_cast<void (*)(DriveClass *, coord_t, BOOL, BOOL)>(0x004B6304);
    func(this, coord, a2, a3);
#else
    captainslog_dbgassert(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
#endif
}

void DriveClass::Per_Cell_Process(PCPType pcp)
{
/*#ifdef GAME_DLL
    void(*func)(DriveClass *, PCPType) = reinterpret_cast<void(*)(DriveClass *, PCPType)>(0x004B6F40);
    func(this, pcp);
#else*/
    if ( pcp == 2 ) {
        if (As_Cell(m_NavCom) == Get_Cell()) {
            m_NavCom = 0;
            m_Paths[0] = FACING_NONE;
            m_Bit_4 = false;
        }
        Lay_Track();
    }
    FootClass::Per_Cell_Process(pcp);
//#endif
}

/**
 *
 *
 */
void DriveClass::Response_Select()
{
    static VocType _response[] = {
        VOC_VEHIC1, VOC_REPORT1, VOC_YESSIR1, VOC_AWAIT1
    };
    if (g_AllowVoice) {
        Sound_Effect(_response[g_Scen.Get_Random_Value(0, ARRAY_SIZE(_response))], fixed_t(1, 1), -(Get_Heap_ID() + 1));
    }
}

/**
 *
 *
 */
void DriveClass::Response_Move()
{
    static VocType _response[] = {
        VOC_ACKNO, VOC_AFFIRM1
    };
    if (g_AllowVoice) {
        Sound_Effect(_response[g_Scen.Get_Random_Value(0, ARRAY_SIZE(_response))], fixed_t(1, 1), -(Get_Heap_ID() + 1));
    }
}

/**
 *
 *
 */
void DriveClass::Response_Attack()
{
    static VocType _response[] = {
        VOC_AFFIRM1, VOC_ACKNO
    };
    if (g_AllowVoice) {
        Sound_Effect(_response[g_Scen.Get_Random_Value(0, ARRAY_SIZE(_response))], fixed_t(1, 1), -(Get_Heap_ID() + 1));
    }
}

void DriveClass::Assign_Destination(target_t dest)
{
#ifdef GAME_DLL
    void (*func)(DriveClass *, target_t) = reinterpret_cast<void (*)(DriveClass *, target_t)>(0x004B67C8);
    func(this, dest);
#else
    captainslog_dbgassert(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
#endif
}

/**
 *
 *
 */
BOOL DriveClass::Stop_Driver()
{
    if (m_HeadTo) {
        if (m_IsDown) {
            Mark(MARK_REMOVE);
        }
        Mark_Track(m_HeadTo, MARK_REMOVE);
        if (m_IsDown) {
            Mark(MARK_PUT);
        }
    }
    return FootClass::Stop_Driver();
}

void DriveClass::Fixup_Path(PathType *path)
{
#ifdef GAME_DLL
    void (*func)(DriveClass *, PathType *) = reinterpret_cast<void (*)(DriveClass *, PathType *)>(0x004B7F4C);
    func(this, path);
#else
    captainslog_dbgassert(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
#endif
}

void DriveClass::Overrun_Cell(cell_t cell, BOOL a2)
{
#ifdef GAME_DLL
    int (*func)(DriveClass *, cell_t, BOOL) = reinterpret_cast<int (*)(DriveClass *, cell_t, BOOL)>(0x004B8470);
    func(this, cell, a2);
#else
    captainslog_dbgassert(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
#endif
}

BOOL DriveClass::Ok_To_Move(DirType dir)
{
#ifdef GAME_DLL
    BOOL (*func)(DriveClass *, DirType) = reinterpret_cast<BOOL (*)(DriveClass *, DirType)>(0x004B83AC);
    return func(this, dir);
#else
    captainslog_dbgassert(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
    return false;
#endif
}

void DriveClass::Do_Turn(DirType dir)
{
#ifdef GAME_DLL
    void (*func)(DriveClass *, DirType) = reinterpret_cast<void (*)(DriveClass *, DirType)>(0x004B6514);
    func(this, dir);
#else
    captainslog_dbgassert(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
#endif
}

/**
 *
 *
 */
BOOL DriveClass::Teleport_To(cell_t cell)
{
    captainslog_assert(Valid_Cell(cell));

    if (g_Rule.Chrono_Kills_Cargo()) {
        Kill_Cargo(nullptr);
    }
    Stop_Driver();

    Force_Track(-1, 0);

    m_Facing.Set_Current(m_Facing.Get_Desired());

    Transmit_Message(RADIO_OVER_AND_OUT);

    Assign_Destination(0);
    Assign_Target(0);

    Assign_Mission(MISSION_NONE);
    Commence();

    Mark(MARK_REMOVE);

    UnitClass *uptr = reinterpret_cast<UnitClass *>(this);
    if (m_RTTI == RTTI_UNIT && uptr->Flag_Owner() != HOUSES_NONE) {
        // needs used housetype member verification
        HouseClass::As_Pointer(uptr->Flag_Owner())->Flag_Attach(Get_Cell());
    }
    if (Can_Enter_Cell(cell)) {
        // needs speed access verification
        cell = g_Map.Nearby_Location(cell, Techno_Class_Of().Get_Speed());
    }
    // set coord code needs verification
    m_Coord = Cell_To_Coord(cell);
    Mark(MARK_PUT);
    return true;
}

void DriveClass::Force_Track(int track, coord_t coord)
{
#ifdef GAME_DLL
    void (*func)(DriveClass *, int, coord_t) = reinterpret_cast<void (*)(DriveClass *, int, coord_t)>(0x004B669C);
    func(this, track, coord);
#else
    captainslog_dbgassert(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
#endif
}

coord_t DriveClass::Smooth_Turn(coord_t coord, DirType &dir)
{
#ifdef GAME_DLL
    coord_t (*func)(DriveClass *, coord_t, DirType &) =
        reinterpret_cast<coord_t (*)(DriveClass *, coord_t, DirType &)>(0x004B6748);
    return func(this, coord, dir);
#else
    captainslog_dbgassert(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
    return 0;
#endif
}

BOOL DriveClass::While_Moving()
{
#ifdef GAME_DLL
    BOOL (*func)(DriveClass *) = reinterpret_cast<BOOL (*)(DriveClass *)>(0x004B68B0);
    return func(this);
#else
    captainslog_dbgassert(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
    return false;
#endif
}

BOOL DriveClass::Start_Of_Move()
{
#ifdef GAME_DLL
    BOOL (*func)(DriveClass *) = reinterpret_cast<BOOL (*)(DriveClass *)>(0x004B6FA0);
    return func(this);
#else
    captainslog_dbgassert(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
    return false;
#endif
}

void DriveClass::Lay_Track()
{
    // TODO: Leftover from Dune2 for laying the visual track?
}

void DriveClass::Mark_Track(coord_t coord, MarkType mark)
{
#ifdef GAME_DLL
    void (*func)(DriveClass *, coord_t, MarkType) = reinterpret_cast<void (*)(DriveClass *, coord_t, MarkType)>(0x004B82AC);
    func(this, coord, mark);
#else
    captainslog_dbgassert(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
#endif
}
