/**
 * @file
 *
 * @author CCHyper
 *
 * @brief Class handling all objects that need per tick logic applying.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "logic.h"
#include "gamedebug.h"
#include "iomap.h"
//#include "voc.h"
//#include "trigger.h"
#include "house.h"
#include "scenario.h"
#include "session.h"
#include "factory.h"
//#include "tevent.h"
#include "object.h"
//#include "anim.h"
#include "target.h"

#ifndef CHRONOSHIFT_STANDALONE
LogicClass &Logic = Make_Global<LogicClass>(0x00668230);
DynamicVectorClass<TriggerClass *> &g_LogicTriggers = Make_Global<DynamicVectorClass<TriggerClass *> >(0x0067F270);
DynamicVectorClass<TriggerClass *> &g_MapTriggers = Make_Global<DynamicVectorClass<TriggerClass *> >(0x0067F254);
DynamicVectorClass<TriggerClass *> *g_HouseTriggers = Make_Pointer<DynamicVectorClass<TriggerClass *> >(0x0067F074);
#else
LogicClass Logic;
DynamicVectorClass<TriggerClass *> g_LogicTriggers;
DynamicVectorClass<TriggerClass *> g_MapTriggers;
DynamicVectorClass<TriggerClass *> g_HouseTriggers[HOUSES_COUNT];
#endif

LogicClass::~LogicClass()
{
    g_LogicTriggers.Clear();
}

void LogicClass::Init()
{
    DEBUG_LOG("LogicClass::Clear()\n");

    LayerClass::Init();
}

void LogicClass::Remove(ObjectClass *const object)
{
    LayerClass::Remove(object);
}

BOOL LogicClass::Submit(ObjectClass *object, BOOL sort)
{
    return LayerClass::Submit(object, sort);
}

/**
 * @brief AI processing entry function.
 */
void LogicClass::AI()
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(LogicClass *) = reinterpret_cast<void (*)(LogicClass *)>(0x004FDD70);
    return func(this);
#elif 0

    // TODO: This function requires a lot of classes.

    ++FramesPerSecond;

    Scen.Do_Fade_AI();

    for (LogicTriggerID = 0; LogicTriggerID < LogicTriggers.Count(); ++LogicTriggerID) {
        // DEBUG_LOG("LogicClass::AI() - LogicTriggerID is %d, trigger name is '%s'\n", LogicTriggerID,
        // LogicTriggers[LogicTriggerID]->ObjectType->Get_Name());

        TriggerClass *trigger = LogicTriggers[LogicTriggerID];
        DEBUG_ASSERT(trigger != nullptr);
        if (trigger != nullptr) {
            // DEBUG_LOG("LogicClass::AI() - LogicTrigger %d:'%s' is valid, about to check global status\n", LogicTriggerID,
            // LogicTriggers[LogicTriggerID]->ObjectType->Get_Name());

            if ((!Scen._GlobalsChanged || !trigger->Spring(TEVENT_GLOBAL_SET)) && !trigger->Spring(TEVENT_GLOBAL_CLEAR)

                && (!Scen._DestroyBridges || !trigger->Spring(TEVENT_ATTACHED_BRIDGE_DESTROYED))
                && !trigger->Spring(TEVENT_ELAPSED_TIME)) {
                if (Scen.GlobalTimer.Has_Expired()) {
                    trigger->Spring(TEVENT_TIMER_EXPIRED);
                }
            }
        }
    }

    if (Scen.GlobalTimer.Has_Expired()) {
        Scen.GlobalTimer.Stop();
        Map.Flag_To_Redraw();
    }

    Scen._DestroyBridges = false;
    Scen._GlobalsChanged = false;

    if (Special.Bools.ShroudGrows && Rule.ShroudRate > 0) {
        if (Scen.SomeScenarioTimer.Has_Expired()) {
            Scen.SomeScenarioTimer = (Rule.ShroudRate * DEF_TICKS_PER_MINUTE);

            Map.Encroach_Shadow();
        }
    }

    //
    //
    //
    // DEBUG_LOG("LogicClass::AI() - Teams.Count = %d\n", Teams.Count());
    for (int team = 0; team < Teams.Count(); ++team) {
        TeamClass *tptr = Teams[team];
        DEBUG_ASSERT(tptr != nullptr);
        if (tptr != nullptr) {
            if (tptr->Is_Active()) {
                // DEBUG_LOG("LogicClass::AI() - About to call AI() on team '%s'\n", tptr->Name());
                tptr->AI();
            }
        }
    }

    if (TimeQuake) {
        VocManager.Play_Locally(VOC_KABOOM15);

        Shake_The_Screen(8);
    }

    g_ChronalVortex.AI();

    for (int index = 0; index < Logic.Count(); ++index) {
        ObjectClass *objptr = Logic[index];
        DEBUG_ASSERT(objptr != nullptr);
        if (objptr != nullptr) {
            // DEBUG_LOG("LogicClass::AI() - About to call AI() on %s:%s\n", RTTIName[objptr->What_Am_I()].Name,
            // objptr->Name());

            objptr->AI();
        }

        if (objptr != nullptr) {
            if (TimeQuake) {
                DEBUG_LOG("LogicClass::AI() - Processing %s:%s for Time Quake damage!\n",
                    RTTIName[objptr->What_Am_I()].Name,
                    objptr->Name());

                if (objptr->Is_Active() && !objptr->In_Limbo() && objptr->Get_Health() > 0) {
                    if (TimeQuakeCenter != 0) {
                        if (Target_Distance(As_Target(objptr), TimeQuakeCenter) / 256 < Rule.MTankDistance) {
                            int quake_damage = 0;

                            switch (objptr->What_Am_I()) {
                                case RTTI_BUILDING:
                                    quake_damage = objptr->Class_Of().Get_Strength() * Rule.QuakeBuildingDamage;
                                    break;

                                case RTTI_INFANTRY:
                                    quake_damage = Rule.QuakeInfantryDamage;
                                    break;

                                case RTTI_VESSEL:
                                    quake_damage = objptr->Class_Of().Get_Strength() * Rule.QuakeVesselDamage;
                                    break;

                                case RTTI_UNIT:
                                    quake_damage = objptr->Class_Of().Get_Strength() * Rule.QuakeUnitDamage;
                                    break;

                                case RTTI_AIRCRAFT: {
                                    // this chunk needs checking, its to make sure aircraft that are
                                    // on the ground
                                    AircraftClass *aptr = dcast<AircraftClass *>(objptr);
                                    if (aptr != nullptr) {
                                        if (aptr->On_Ground()) {
                                            // todo, new aircraft key
                                            // quake_damage = objptr->Class_Of().Strength * Rule.QuakeAircraftDamage;
                                        }
                                    }
                                    break;
                                }

                                default:
                                    quake_damage = objptr->Class_Of().Get_Strength() * Rule.QuakeDamage;
                                    break;
                            };

                            if (quake_damage > 0) {
                                objptr->Clicked_As_Target(7);

                                AnimClass *anim = new AnimClass(ANIM_MINEEXP1, objptr->Center_Coord());
                                DEBUG_ASSERT(anim != nullptr);
                            }

                            objptr->Take_Damage(v36, 0, WARHEAD_AP, nullptr, 1); // TODO, check def args
                        }
                    }
                }
            }
        }

        if (objptr != Logic[index]) {
            --index;
        }
    }

    HouseClass::Recalc_Attributes();

    Map.Logic_AI();

    // DEBUG_LOG("LogicClass::AI() - Factories.Count = %d\n", Teams.Count());
    for (int factory = 0; factory < Factories.Count(); ++factory) {
        FactoryClass *fptr = Factories[factory];
        DEBUG_ASSERT(fptr != nullptr);
        if (fptr != nullptr) {
            if (fptr->Is_Active()) {
                DEBUG_LOG("LogicClass::AI() - About to call FactoryClass::AI() on factory '%d'\n", factory);
                fptr->AI();
            }
        }
    }

    if (Session.GameToPlay != GAME_CAMPAIGN) {
        for (HousesType mphouse = HOUSES_MULTI_FIRST; mphouse < HOUSES_MULTI_LAST; ++mphouse) {
            HouseClass *hptr = HouseClass::As_Pointer(mphouse);
            DEBUG_ASSERT(hptr != nullptr);

            if (hptr != nullptr) {
                if (hptr->Is_Active()) {
                    hptr->AI();
                }
            }
        }

    } else {
        for (HousesType house = HOUSES_FIRST; house < HOUSES_COUNT; ++house) {
            HouseClass *hptr = HouseClass::As_Pointer(house);
            DEBUG_ASSERT(hptr != nullptr);
            if (hptr != nullptr) {
                if (hptr->Is_Active()) {
                    hptr->AI();
                }
            }
        }
    }

    if (Session.GameToPlay != GAME_CAMPAIGN) {
        if (Scen.AutoSonarTimer <= 0) {
            if (AutoSonarPulse) {
                Map.Activate_Pulse();
                VocManager.Play_Locally(VOC_SONPULSE);
                AutoSonarPulse = false;
            }

            Scen.AutoSonarTimer = 600;
        }
    }
#endif
}

void LogicClass::Detach(target_t target, int a2)
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(LogicClass *, target_t, int) = reinterpret_cast<void (*)(LogicClass *, target_t, int)>(0x004FE2BC);
    return func(this, target, a2);
#elif 0

    // TODO: This function requires TriggerClass implimentation.

    if ((RTTIType)a2 == RTTI_TRIGGER) {
        for (int index = 0; index < LogicTriggers.Count(); ++index) {
            TriggerClass *ptr = LogicTriggers[index];
            DEBUG_ASSERT(ptr != nullptr);

            if (As_Trigger(target) == ptr) {
                LogicTriggers.Delete_Index(index);
            }
        }
    }
#endif
}
