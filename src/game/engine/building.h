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

#ifndef BUILDING_H
#define BUILDING_H

#include "always.h"
#include "techno.h"
#include "buildingtype.h"
#include "factory.h"

class BuildingClass : public TechnoClass
{
    public:
    BuildingClass(RTTIType type, int id, HousesType house);
    BuildingClass(const BuildingClass &that);
    BuildingClass(const NoInitClass &noinit);
    virtual ~BuildingClass();

    // ObjectClass
    virtual BuildingTypeClass &Class_Of() const override { return *m_Type; }

    // TechnoClass
    virtual void Death_Announcement(TechnoClass *killer) const override;

    BuildingType What_Type() const { return m_Type->What_Type(); }

private:
    GamePtr<BuildingTypeClass> m_Type;
    GamePtr<FactoryClass> m_Factory;
    HousesType m_field_D5;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool m_Bit1 : 1; // 1
            bool m_Bit2 : 1; // 2
            bool m_Bit4 : 1; // 4
            bool m_Bit8 : 1; // 8
            bool m_Bit16 : 1; // 16
    };
        int m_Bitfield;
};
#else
    bool m_Bit1;
    bool m_Bit2;
    bool m_Bit4;
    bool m_Bit8;
    bool m_Bit16;
#endif
    TCountDownTimerClass<FrameTimerClass> m_C4FuseTimer;
    BStateType m_field_E3;
    BStateType m_field_E4;
    HousesType m_field_E5;
    target_t m_field_E6;
    int m_field_EA;
    target_t m_field_EE;
    TCountDownTimerClass<FrameTimerClass> m_field_F2;
};

#ifndef CHRONOSHIFT_STANDALONE
extern TFixedIHeapClass<BuildingClass> &g_Buildings;
#else
extern TFixedIHeapClass<BuildingClass> g_Buildings;
#endif

#endif // BUILDING_H
