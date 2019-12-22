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

#ifndef INFANTRY_H
#define INFANTRY_H

#include "always.h"
#include "cell.h"
#include "foot.h"
#include "infantrytype.h"

class InfantryClass : public FootClass
{
private:
    struct DoStruct {
        #ifndef CHRONOSHIFT_NO_BITFIELDS
        BOOL m_Bit1 : 1; // & 1
        BOOL m_Bit2 : 1; // & 2
        BOOL m_Bit4 : 1; // & 4
        #else
        bool m_Bit1;
        bool m_Bit2;
        bool m_Bit4;
        #endif
        unsigned char m_Rate;
    };
public:
    InfantryClass(InfantryType type, HousesType house);
    InfantryClass(const InfantryClass &that);
    InfantryClass(const NoInitClass &noinit);
    virtual ~InfantryClass();

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    // ObjectClass
    virtual void *Get_Image_Data() const final;
    virtual const InfantryTypeClass &Class_Of() const final { return *m_Class; }
    virtual int Full_Name() const final;
    virtual BOOL Limbo() final;
    virtual void Detach(target_t target, int a2) final;
    virtual BOOL Paradrop(coord_t coord) final;
    virtual void Draw_It(int x, int y, WindowNumberType window) const final;
    virtual void Active_Click_With(ActionType action, ObjectClass *object) final;
    virtual void Active_Click_With(ActionType action, cell_t cellnum) final;

    // MissionClass
    virtual int Mission_Attack() final;

    // TechnoClass
    virtual FireErrorType Can_Fire(target_t target, WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) const final;
    virtual void Assign_Target(target_t target) final;
    virtual BulletClass *Fire_At(target_t target, WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) final;
    virtual BOOL Is_Ready_To_Random_Animate() const final;
    virtual void Assign_Destination(target_t dest) final;

    // FootClass
    virtual BOOL Stop_Driver() final;

    // InfantryClass
    virtual BOOL Do_Action(DoType dotype, BOOL a1 = false);

    inline void Set_Occupy_Spot(coord_t coord) { Set_Occupy_Spot(Coord_To_Cell(coord), CellClass::Spot_Index(coord)); }
    inline void Clear_Occupy_Spot(coord_t coord) { Clear_Occupy_Spot(Coord_To_Cell(coord), CellClass::Spot_Index(coord)); }
    void Set_Occupy_Spot(cell_t cellnum, int spot_index);
    void Clear_Occupy_Spot(cell_t cellnum, int spot_index);

    BOOL Is_Technician() const { return m_Technician; }
    BOOL Get_Bit16() const { return m_Bit16; }

    InfantryType What_Type() const { return m_Class->What_Type(); }

private:
    int Shape_Number() const;
    BOOL Edge_Of_World_AI();

#ifdef GAME_DLL
    friend void Setup_Hooks();

public:
    void *Hook_Get_Image_Data()
    {
        return InfantryClass::Get_Image_Data();
    }
    int Hook_Full_Name()
    {
        return InfantryClass::Full_Name();
    }
    void Hook_Draw_It(int x, int y, WindowNumberType window)
    {
        InfantryClass::Draw_It(x, y, window);
    }
    void Hook_Active_Click_With(ActionType action, ObjectClass *object)
    {
        InfantryClass::Active_Click_With(action, object);
    }
    FireErrorType Hook_Can_Fire(target_t target, WeaponSlotType weapon)
    {
        return InfantryClass::Can_Fire(target, weapon);
    }
    BOOL Hook_Is_Ready_To_Random_Animate()
    {
        return InfantryClass::Is_Ready_To_Random_Animate();
    }
#endif

private:
    GamePtr<InfantryTypeClass> m_Class;
    DoType m_Doing;
    TCountDownTimerClass<FrameTimerClass> m_StokeTimer;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_Technician : 1; // 1
    BOOL m_Stoked : 1; // 2
    BOOL m_Prone : 1; // 4
    BOOL m_Bit8 : 1; // 8
    BOOL m_Bit16 : 1; // 16
#else
    bool m_Technician;
    bool m_Stoked;
    bool m_Prone;
    bool m_Bit8;
    bool m_Bit16;
#endif
    unsigned char m_Fear;

private:
    static DoStruct MasterDoControls[DO_COUNT];
};

#ifdef GAME_DLL
extern TFixedIHeapClass<InfantryClass> &g_Infantry;
#else
extern TFixedIHeapClass<InfantryClass> g_Infantry;
#endif

#endif // INFANTRY_H
