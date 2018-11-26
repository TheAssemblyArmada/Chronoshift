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
#pragma once

#ifndef DRIVE_H
#define DRIVE_H

#include "always.h"
#include "foot.h"

class DriveClass : public FootClass
{
public:
    DriveClass(RTTIType type, int id, HousesType house);
    DriveClass(const DriveClass &that);
    DriveClass(const NoInitClass &noinit);
    virtual ~DriveClass() {}

    // AbstractClass
    virtual void AI() override;

    // ObjectClass
    virtual BOOL Limbo() override;
    void Scatter(coord_t coord, int a2, int a3) override;
    void Per_Cell_Process(PCPType pcp) override;

    // TechnoClass
    virtual void Response_Select() override;
    virtual void Response_Move() override;
    virtual void Response_Attack() override;

    // FootClass
    virtual void Assign_Destination(target_t dest) override;
    virtual BOOL Stop_Driver() override;
    virtual void Fixup_Path(PathType *path) override;

    // DriveClass
    virtual void Overrun_Cell(cell_t cell, int a2);
    virtual BOOL Ok_To_Move(DirType dir);

    void Do_Turn(DirType dir);
    BOOL Teleport_To(cell_t cell);
    void Force_Track(int a1, coord_t coord);
    coord_t Smooth_Turn(coord_t coord, DirType &dir);
    BOOL While_Moving();
    BOOL Start_Of_Move();
    void Lay_Track();
    void Mark_Track(coord_t coord, MarkType mark);

    unsigned int Speed_Accum() const { return m_SpeedAccum; }
    int Track_Number() const { return m_TrackNumber; }
    int Track_Index() const { return m_TrackIndex; }

protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool m_IsHarvesting : 1; // 1
            bool m_Teleported : 1; // 2
            bool m_Bit_4 : 1; // 4
            bool m_Bit_8 : 1; // 8
        };
        int m_Bitfield;
    };
#else
    bool m_IsHarvesting;
    bool m_Teleported;
    bool m_Bit_4;
    bool m_Bit_8;
#endif

    TCountDownTimerClass<FrameTimerClass> m_field_145;
    cell_t m_TeleportReturnLocation;
    unsigned int m_SpeedAccum;
    int m_TrackNumber;
    int m_TrackIndex;
};

#endif // DRIVE_H
