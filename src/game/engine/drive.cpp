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
#include "scenario.h"
#include "globals.h"
#include "target.h"
#include "gametypes.h"
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

void DriveClass::AI()
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(DriveClass *) = reinterpret_cast<void (*)(DriveClass *)>(0x004B7CA8);
    func(this);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
#endif
}

BOOL DriveClass::Limbo()
{
/*#ifndef CHRONOSHIFT_STANDALONE
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

void DriveClass::Scatter(coord_t coord, int a2, int a3)
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(DriveClass *, coord_t, int, int) = reinterpret_cast<void (*)(DriveClass *, coord_t, int, int)>(0x004B6304);
    func(this, coord, a2, a3);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
#endif
}

void DriveClass::Per_Cell_Process(PCPType pcp)
{
/*#ifndef CHRONOSHIFT_STANDALONE
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

void DriveClass::Response_Select()
{
/*#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(DriveClass *) = reinterpret_cast<void (*)(DriveClass *)>(0x00423154);
    func(this);
#else*/
    static VocType _response[] = {
        VOC_VEHIC1, VOC_REPORT1, VOC_YESSIR1/*, VOC_YESSIR1, VOC_YESSIR1*/, VOC_AWAIT1
    };
    if (g_AllowVoice) {
        Sound_Effect(_response[Scen.Get_Random_Value(0, ARRAY_SIZE(_response))], fixed_t::_1_1, -(Get_Heap_ID() + 1));
    }
//#endif
}

void DriveClass::Response_Move()
{
/*#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(DriveClass *) = reinterpret_cast<void (*)(DriveClass *)>(0x004230FC);
    func(this);
#else*/
    static VocType _response[] = {
        VOC_ACKNO, VOC_AFFIRM1
    };
    if (g_AllowVoice) {
        Sound_Effect(_response[Scen.Get_Random_Value(0, ARRAY_SIZE(_response))], fixed_t::_1_1, -(Get_Heap_ID() + 1));
    }
//#endif
}

void DriveClass::Response_Attack()
{
    /*#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(DriveClass *) = reinterpret_cast<void (*)(DriveClass *)>(0x004230A4);
    func(this);
#else*/
    static VocType _response[] = {
        VOC_AFFIRM1, VOC_ACKNO
    };
    if (g_AllowVoice) {
        Sound_Effect(_response[Scen.Get_Random_Value(0, ARRAY_SIZE(_response))], fixed_t::_1_1, -(Get_Heap_ID() + 1));
    }
//#endif
}

void DriveClass::Assign_Destination(target_t dest)
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(DriveClass *, target_t) = reinterpret_cast<void (*)(DriveClass *, target_t)>(0x004B67C8);
    func(this, dest);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
#endif
}

BOOL DriveClass::Stop_Driver()
{
/*#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(DriveClass *) = reinterpret_cast<BOOL (*)(DriveClass *)>(0x004B64B8);
    return func(this);
#else*/
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
//#endif
}

void DriveClass::Fixup_Path(PathType *path)
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(DriveClass *, PathType *) = reinterpret_cast<void (*)(DriveClass *, PathType *)>(0x004B7F4C);
    func(this, path);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
#endif
}

void DriveClass::Overrun_Cell(cell_t cell, int a2)
{
#ifndef CHRONOSHIFT_STANDALONE
    int (*func)(DriveClass *, cell_t, int) = reinterpret_cast<int (*)(DriveClass *, cell_t, int)>(0x004B8470);
    func(this, cell, a2);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
#endif
}

BOOL DriveClass::Ok_To_Move(DirType dir)
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(DriveClass *, DirType) = reinterpret_cast<BOOL (*)(DriveClass *, DirType)>(0x004B83AC);
    return func(this, dir);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
    return false;
#endif
}

void DriveClass::Do_Turn(DirType dir)
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(DriveClass *, DirType) = reinterpret_cast<void (*)(DriveClass *, DirType)>(0x004B6514);
    func(this, dir);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
#endif
}

BOOL DriveClass::Teleport_To(cell_t cell)
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(DriveClass *, short) = reinterpret_cast<BOOL (*)(DriveClass *, cell_t)>(0x004B653C);
    return func(this, cell);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
    return false;
#endif
}

void DriveClass::Force_Track(int track, coord_t coord)
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(DriveClass *, int, coord_t) = reinterpret_cast<void (*)(DriveClass *, int, coord_t)>(0x004B669C);
    func(this, track, coord);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
#endif
}

coord_t DriveClass::Smooth_Turn(coord_t coord, DirType &dir)
{
#ifndef CHRONOSHIFT_STANDALONE
    coord_t (*func)(DriveClass *, coord_t, DirType &) =
        reinterpret_cast<coord_t (*)(DriveClass *, coord_t, DirType &)>(0x004B6748);
    return func(this, coord, dir);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
    return 0;
#endif
}

BOOL DriveClass::While_Moving()
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(DriveClass *) = reinterpret_cast<BOOL (*)(DriveClass *)>(0x004B68B0);
    return func(this);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
    return false;
#endif
}

BOOL DriveClass::Start_Of_Move()
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(DriveClass *) = reinterpret_cast<BOOL (*)(DriveClass *)>(0x004B6FA0);
    return func(this);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
    return false;
#endif
}

void DriveClass::Lay_Track()
{
    // TODO: Leftover from Dune2 for laying the visual track?
}

void DriveClass::Mark_Track(coord_t coord, MarkType mark)
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(DriveClass *, coord_t, MarkType) = reinterpret_cast<void (*)(DriveClass *, coord_t, MarkType)>(0x004B82AC);
    func(this, coord, mark);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function '%s' called!\n", __FUNCTION__);
#endif
}
