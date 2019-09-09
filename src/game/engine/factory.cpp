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
#include "factory.h"
#include "gamedebug.h"
#include "globals.h"
#include "target.h"
#include "techno.h"
#include "technotype.h"
#include "house.h"
#include <algorithm>

#ifdef GAME_DLL
TFixedIHeapClass<FactoryClass> &g_Factories = Make_Global<TFixedIHeapClass<FactoryClass> >(0x0065D948);
#else
TFixedIHeapClass<FactoryClass> g_Factories;
#endif

#define MAX_CLOCK_STAGES 54

FactoryClass::FactoryClass() :
    m_ProductionTime(),
    m_RTTI(RTTI_FACTORY),
    m_HeapID(g_Factories.ID(this)),
    m_IsActive(false),
    m_IsSuspended(false),
    m_IsDifferent(false),
    m_Balance(0),
    m_OriginalBalance(0),
    m_Object(nullptr),
    m_SpecialItem(-1),
    m_Owner(nullptr)
{
}

FactoryClass::~FactoryClass()
{
    if (GameActive) {
        Abandon();
    }
}

FactoryClass::FactoryClass(const FactoryClass &that)
{
}

void *FactoryClass::operator new(size_t size)
{
    FactoryClass *this_ptr = g_Factories.Alloc();
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = true;
    }
    return this_ptr;
}

void FactoryClass::operator delete(void *ptr)
{
    FactoryClass *this_ptr = static_cast<FactoryClass *>(ptr);
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = false;
    }
    g_Factories.Free(this_ptr);
}

void FactoryClass::Init()
{
    g_Factories.Free_All();
}

void FactoryClass::AI()
{
    if (!m_IsSuspended && (m_Object != nullptr || m_SpecialItem != -1)) {
        if (!Has_Completed()) {
            if (m_ProductionTime.Stage_Changed()) {
                m_IsDifferent = true;
                unsigned int tick_cost = std::min(Cost_Per_Tick(), m_Balance);
                if (m_Owner->Available_Money() >= tick_cost) {
                    m_Owner->Spend_Money(tick_cost);
                    m_Balance -= tick_cost;
                } else {
                    m_ProductionTime.Set_Stage(m_ProductionTime.Get_Stage() - 1);
                }
            #ifdef CHRONOSHIFT_DEBUG
                if (g_Debug_InstantBuild) {
                    m_ProductionTime.Set_Stage(MAX_CLOCK_STAGES);
                }
            #endif
                if (m_ProductionTime.Get_Stage() == MAX_CLOCK_STAGES) {
                    m_IsSuspended = true;
                    m_ProductionTime.Set_Delay(0);
                    m_Owner->Spend_Money(m_Balance);
                    m_Balance = 0;
                }
            }
        }
    }
}

BOOL FactoryClass::Has_Changed()
{
    bool prev = m_IsDifferent;
    m_IsDifferent = false;
    return prev;
}

BOOL FactoryClass::Set(TechnoTypeClass &objecttype, HouseClass &house)
{
#ifdef GAME_DLL
    int (*func)(FactoryClass *, TechnoTypeClass &, HouseClass &) = reinterpret_cast<int (*)(FactoryClass *, TechnoTypeClass &, HouseClass &)>(0x004BEE30);
    return func(this, objecttype, house);
#else
    Abandon();

    m_IsDifferent = true;
    m_IsSuspended = true;

    m_ProductionTime.Set_Stage(0);
    m_ProductionTime.Set_Delay(0);

    m_Balance = 0;

    m_Object = objecttype.Techno_Create_One_Of(&house);

    if (!house.Is_Human()) {
        if (m_Object != nullptr) {
            if (m_Object->What_Am_I() == RTTI_BUILDING) {
                //TODO: Requires BuildingClass.
                // reinterpret_cast<BuildingClass *>(m_Object)->Set_To_Rebuild(True);
            }
        }
    }

    if (m_Object != nullptr) {
        m_Owner = m_Object->Get_Owner_House();
        int cost = objecttype.Cost_Of() * m_Owner->Cost_Multiplier();
        m_Balance = cost;
        m_Object->Set_Price(cost);
    }

    return m_Object != nullptr;
#endif
}

BOOL FactoryClass::Set(int &special, HouseClass &house)
{
    Abandon();

    m_IsDifferent = true;
    m_IsSuspended = true;

    m_SpecialItem = special;
    m_Owner = &house;
    m_Balance = 0;

    m_ProductionTime.Set_Stage(0);
    m_ProductionTime.Set_Delay(0);

    return m_SpecialItem != -1;
}

void FactoryClass::Set(TechnoClass &object)
{
    Abandon();

    m_IsDifferent = true;
    m_IsSuspended = true;

    m_Object = &object;
    m_Owner = m_Object->Get_Owner_House();
    m_Balance = 0;

    m_ProductionTime.Set_Stage(0);
    m_ProductionTime.Set_Delay(0);
}

BOOL FactoryClass::Suspend()
{
    if (!m_IsSuspended) {
        m_IsSuspended = true;
        m_ProductionTime.Set_Delay(0);
        return true;
    }
    return false;
}

BOOL FactoryClass::Start()
{
#ifdef GAME_DLL
    BOOL (*func)(FactoryClass *) = reinterpret_cast<BOOL (*)(FactoryClass *)>(0x004BEFCC);
    return func(this);
#else
    return 0; // TODO
#endif
}

BOOL FactoryClass::Abandon()
{
    if (m_Object != nullptr) {
        DEBUG_LOG("Abandoning production of %s\n", m_Object->Name());

        if (m_Owner != nullptr) {
            m_Owner->Refund_Money(m_Object->Class_Of().Cost_Of() - m_Balance);
        }

        m_Balance = 0;

        m_IsSuspended = true;
        m_IsDifferent = true;

        m_ProductionTime.Set_Stage(0);
        m_ProductionTime.Set_Delay(0);

        ++ScenarioInit;

        if (m_SpecialItem != -1) {
            m_SpecialItem = -1;
        }

        if (m_Object != nullptr) {
            delete m_Object;
            m_Object = nullptr;
        }

        --ScenarioInit;

        return true;
    }

    return false;
}

BOOL FactoryClass::Has_Completed() const
{
    return (m_Object != nullptr || m_SpecialItem != -1) && m_ProductionTime.Get_Stage() == MAX_CLOCK_STAGES;
}

unsigned int FactoryClass::Cost_Per_Tick() const
{
    int cost_per_tick = 0;

    if (m_Object != nullptr) {
        int v2 = MAX_CLOCK_STAGES - m_ProductionTime.Get_Stage();
        if (v2 > 0) {
            cost_per_tick = m_Balance / v2;
        } else {
            cost_per_tick = m_Balance;
        }
    }
    return cost_per_tick;
}

BOOL FactoryClass::Completed()
{
    if (m_Object != nullptr && m_ProductionTime.Get_Stage() == MAX_CLOCK_STAGES) {
        m_Object = nullptr;
        m_IsSuspended = true;
        m_IsDifferent = true;
        m_ProductionTime.Set_Stage(0);
        m_ProductionTime.Set_Delay(0);
        return true;
    }
    if (m_SpecialItem != -1 && m_ProductionTime.Get_Stage() == MAX_CLOCK_STAGES) {
        m_SpecialItem = -1;
        m_IsSuspended = true;
        m_IsDifferent = true;
        m_ProductionTime.Set_Stage(0);
        m_ProductionTime.Set_Delay(0);
        return true;
    }
    return false;
}

void FactoryClass::Code_Pointers()
{
//#ifdef GAME_DLL
    void (*func)(FactoryClass *) = reinterpret_cast<void (*)(FactoryClass *)>(0x004F94E8);
    return func(this);
/*#else
    // TODO: Requires HouseClass.
    if (m_Object != nullptr) {
        m_Object = (TechnoClass *)As_Target(m_Object);
    }
    if (m_Owner != nullptr) {
        m_Owner = m_Owner->Get_Type();
    }
#endif*/
}

void FactoryClass::Decode_Pointers()
{
//#ifdef GAME_DLL
    void (*func)(FactoryClass *) = reinterpret_cast<void (*)(FactoryClass *)>(0x004F9550);
    return func(this);
/*#else
    // TODO: Requires HouseClass.
    if (Target_Legal(m_Object)) {
        m_Object = As_Techno(m_Object);
    }
    if (m_Owner != HOUSES_NONE) {
        m_Owner = HouseClass::As_Pointer(m_Owner);
    }
#endif*/
}
