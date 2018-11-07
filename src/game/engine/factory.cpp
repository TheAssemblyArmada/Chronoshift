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

#ifndef CHRONOSHIFT_STANDALONE
TFixedIHeapClass<FactoryClass> &g_Factories = Make_Global<TFixedIHeapClass<FactoryClass> >(0x0065D948);
#else
TFixedIHeapClass<FactoryClass> g_Factories;
#endif

#define MAX_CLOCK_STAGES 54

FactoryClass::FactoryClass() :
    ProductionTime(),
    RTTI(RTTI_FACTORY),
    HeapID(g_Factories.ID(this)),
    IsActive(false),
    IsSuspended(false),
    IsDifferent(false),
    Balance(0),
    OriginalBalance(0),
    Object(nullptr),
    SpecialItem(-1),
    Owner(nullptr)
{
}

FactoryClass::~FactoryClass()
{
    if (GameActive) {
        Abandon();
    }
}

FactoryClass::FactoryClass(const FactoryClass &that) {}

void *FactoryClass::operator new(size_t size)
{
    FactoryClass *this_ptr = g_Factories.Alloc();
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->IsActive = true;
    }
    return this_ptr;
}

void FactoryClass::operator delete(void *ptr)
{
    FactoryClass *this_ptr = static_cast<FactoryClass *>(ptr);
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->IsActive = false;
    }
    g_Factories.Free(this_ptr);
}

void FactoryClass::Init()
{
    g_Factories.Free_All();
}

void FactoryClass::AI()
{
    if (!IsSuspended && (Object != nullptr || SpecialItem != -1)) {
        if (!Has_Completed()) {
            if (ProductionTime.Stage_Changed()) {
                IsDifferent = true;
                int tick_cost = Min(Cost_Per_Tick(), Balance);
                if (Owner->Available_Money() >= tick_cost) {
                    Owner->Spend_Money(tick_cost);
                    Balance -= tick_cost;
                } else {
                    ProductionTime.Set_Stage(ProductionTime.Get_Stage() - 1);
                }
            #ifdef CHRONOSHIFT_DEBUG
                if ( g_Debug_Instant_Build ) {
                    ProductionTime.Set_Stage(MAX_CLOCK_STAGES);
                }
            #endif
                if (ProductionTime.Get_Stage() == MAX_CLOCK_STAGES) {
                    IsSuspended = true;
                    ProductionTime.Set_Delay(0);
                    Owner->Spend_Money(Balance);
                    Balance = 0;
                }
            }
        }
    }
}

BOOL FactoryClass::Has_Changed()
{
    bool prev = IsDifferent;
    IsDifferent = false;
    return prev;
}

BOOL FactoryClass::Set(TechnoTypeClass &objecttype, HouseClass &house)
{
    Abandon();

    IsDifferent = true;
    IsSuspended = true;

    Object = reinterpret_cast<TechnoClass *>(objecttype.Create_One_Of(&house));

    Balance = 0;

    if (Object != nullptr) {
        int cost = objecttype.Cost_Of();
        Balance = cost;
        Object->Set_Price(cost);
        Owner = Object->Get_Owner_House();
    }

    ProductionTime.Set_Stage(0);
    ProductionTime.Set_Delay(0);

    return Object != nullptr;
}

BOOL FactoryClass::Set(int &special, HouseClass &house)
{
    Abandon();

    IsDifferent = true;
    IsSuspended = true;

    SpecialItem = special;
    Owner = &house;
    Balance = 0;

    ProductionTime.Set_Stage(0);
    ProductionTime.Set_Delay(0);

    return SpecialItem != -1;
}

void FactoryClass::Set(TechnoClass &object)
{
    Abandon();

    IsDifferent = true;
    IsSuspended = true;

    Object = &object;
    Owner = Object->Get_Owner_House();
    Balance = 0;

    ProductionTime.Set_Stage(0);
    ProductionTime.Set_Delay(0);
}

BOOL FactoryClass::Suspend()
{
    if (!IsSuspended) {
        IsSuspended = true;
        ProductionTime.Set_Delay(0);
        return true;
    }
    return false;
}

BOOL FactoryClass::Start()
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(FactoryClass *) = reinterpret_cast<BOOL (*)(FactoryClass *)>(0x004BEFCC);
    return func(this);
#else
    return 0; // TODO
#endif
}

BOOL FactoryClass::Abandon()
{
    if (Object != nullptr) {
        DEBUG_LOG("Abandoning production of %s\n", Object->Name());

        if (Owner != nullptr) {
            Owner->Refund_Money(Object->Class_Of().Cost_Of() - Balance);
        }

        Balance = 0;

        IsSuspended = true;
        IsDifferent = true;

        ProductionTime.Set_Stage(0);
        ProductionTime.Set_Delay(0);

        ++ScenarioInit;

        if (SpecialItem != -1) {
            SpecialItem = -1;
        }

        if (Object != nullptr) {
            delete Object;
            Object = nullptr;
        }

        --ScenarioInit;

        return true;
    }

    return false;
}

BOOL FactoryClass::Has_Completed() const
{
    return (Object != nullptr || SpecialItem != -1) && ProductionTime.Get_Stage() == MAX_CLOCK_STAGES;
}

int FactoryClass::Cost_Per_Tick() const
{
    int cost_per_tick = 0;

    if (Object == nullptr) {
        return cost_per_tick;
    }

    int v2 = MAX_CLOCK_STAGES - ProductionTime.Get_Stage();
    if (v2 > 0) {
        cost_per_tick = Balance / v2;
    } else {
        cost_per_tick = Balance;
    }

    return cost_per_tick;
}

BOOL FactoryClass::Completed()
{
    if (Object != nullptr && ProductionTime.Get_Stage() == MAX_CLOCK_STAGES) {
        Object = nullptr;
        IsSuspended = true;
        IsDifferent = true;
        ProductionTime.Set_Stage(0);
        ProductionTime.Set_Delay(0);
        return true;
    }
    if (SpecialItem != -1 && ProductionTime.Get_Stage() == MAX_CLOCK_STAGES) {
        SpecialItem = -1;
        IsSuspended = true;
        IsDifferent = true;
        ProductionTime.Set_Stage(0);
        ProductionTime.Set_Delay(0);
        return true;
    }
    return false;
}

void FactoryClass::Code_Pointers()
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(FactoryClass *) = reinterpret_cast<void (*)(FactoryClass *)>(0x004F94E8);
    return func(this);
#else
    // TODO: Requires HouseClass.
    if (Object != nullptr) {
        Object = (TechnoClass *)As_Target(Object);
    }
    if (Owner != nullptr) {
        Owner = Owner->Get_Type();
    }
#endif
}

void FactoryClass::Decode_Pointers()
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(FactoryClass *) = reinterpret_cast<void (*)(FactoryClass *)>(0x004F9550);
    return func(this);
#else
    // TODO: Requires HouseClass.
    if (Target_Legal(Object)) {
        Object = As_Techno(Object);
    }
    if (Owner != HOUSES_NONE) {
        Owner = HouseClass::As_Pointer(Owner);
    }
#endif
}
