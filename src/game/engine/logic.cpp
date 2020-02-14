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
#include <captainslog.h>

#ifndef GAME_DLL
LogicClass g_Logic;
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
    captainslog_debug("LogicClass::Clear()");

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
#ifdef GAME_DLL
    void (*func)(LogicClass *) = reinterpret_cast<void (*)(LogicClass *)>(0x004FDD70);
    return func(this);
#elif 0

    // TODO: This function requires a lot of classes.

    ++FramesPerSecond;

    g_Scen.Do_Fade_AI();

    for (LogicTriggerID = 0; LogicTriggerID < LogicTriggers.Count(); ++LogicTriggerID) {
        // captainslog_debug("LogicClass::AI() - LogicTriggerID is %d, trigger name is '%s'", LogicTriggerID,
        // LogicTriggers[LogicTriggerID]->ObjectType->Get_Name());

        TriggerClass *trigger = LogicTriggers[LogicTriggerID];
        captainslog_assert(trigger != nullptr);
        if (trigger != nullptr) {
            // captainslog_debug("LogicClass::AI() - LogicTrigger %d:'%s' is valid, about to check global status", LogicTriggerID,
            // LogicTriggers[LogicTriggerID]->ObjectType->Get_Name());

            if ((!g_Scen.m_GlobalsChanged || !trigger->Spring(TEVENT_GLOBAL_SET)) && !trigger->Spring(TEVENT_GLOBAL_CLEAR)

                && (!g_Scen.m_DestroyBridges || !trigger->Spring(TEVENT_ATTACHED_BRIDGE_DESTROYED))
                && !trigger->Spring(TEVENT_ELAPSED_TIME)) {
                if (g_Scen.m_GlobalTimer.Has_Expired()) {
                    trigger->Spring(TEVENT_TIMER_EXPIRED);
                }
            }
        }
    }

    if (g_Scen.m_GlobalTimer.Has_Expired()) {
        g_Scen.m_GlobalTimer.Stop();
        g_Map.Flag_To_Redraw();
    }

    g_Scen.m_DestroyBridges = false;
    g_Scen.m_GlobalsChanged = false;

    if (s_Special.Bools.ShroudGrows && g_Rule.ShroudRate > 0) {
        if (g_Scen.SomeScenarioTimer.Has_Expired()) {
            g_Scen.SomeScenarioTimer = (g_Rule.ShroudRate * DEF_TICKS_PER_MINUTE);

            g_Map.Encroach_Shadow();
        }
    }

    // captainslog_debug("LogicClass::AI() - Teams.Count = %d", Teams.Count());
    for (int team = 0; team < Teams.Count(); ++team) {
        TeamClass *tptr = Teams[team];
        captainslog_assert(tptr != nullptr);
        if (tptr != nullptr) {
            if (tptr->Is_Active()) {
                // captainslog_debug("LogicClass::AI() - About to call AI() on team '%s'", tptr->Name());
                tptr->AI();
            }
        }
    }

    if (TimeQuake) {
        VocManager.Play_Locally(VOC_KABOOM15);

        Shake_The_Screen(8);
    }

    g_ChronalVortex.AI();

    for (int index = 0; index < g_Logic.Count(); ++index) {
        ObjectClass *objptr = g_Logic[index];
        captainslog_assert(objptr != nullptr);
        if (objptr != nullptr) {
            // captainslog_debug("LogicClass::AI() - About to call AI() on %s:%s", RTTIName[objptr->What_Am_I()].Name,
            // objptr->Name());

            objptr->AI();
        }

        if (objptr != nullptr) {
            if (TimeQuake) {
                captainslog_debug("LogicClass::AI() - Processing %s:%s for Time Quake damage!",
                    g_RTTIName[objptr->What_Am_I()].Name,
                    objptr->Name());

                if (objptr->Is_Active() && !objptr->In_Limbo() && objptr->Get_Health() > 0) {
                    if (TimeQuakeCenter != 0) {
                        if (Target_Distance(As_Target(objptr), TimeQuakeCenter) / 256 < g_Rule.MTankDistance) {
                            int quake_damage = 0;

                            switch (objptr->What_Am_I()) {
                                case RTTI_BUILDING:
                                    quake_damage = objptr->Class_Of().Get_Strength() * g_Rule.QuakeBuildingDamage;
                                    break;

                                case RTTI_INFANTRY:
                                    quake_damage = g_Rule.QuakeInfantryDamage;
                                    break;

                                case RTTI_VESSEL:
                                    quake_damage = objptr->Class_Of().Get_Strength() * g_Rule.QuakeVesselDamage;
                                    break;

                                case RTTI_UNIT:
                                    quake_damage = objptr->Class_Of().Get_Strength() * g_Rule.QuakeUnitDamage;
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
                                    quake_damage = objptr->Class_Of().Get_Strength() * g_Rule.QuakeDamage;
                                    break;
                            };

                            if (quake_damage > 0) {
                                objptr->Clicked_As_Target(7);

                                AnimClass *anim = new AnimClass(ANIM_MINEEXP1, objptr->Center_Coord());
                                captainslog_assert(anim != nullptr);
                            }

                            objptr->Take_Damage(v36, 0, WARHEAD_AP, nullptr, 1); // TODO, check def args
                        }
                    }
                }
            }
        }

        if (objptr != g_Logic[index]) {
            --index;
        }
    }

    HouseClass::Recalc_Attributes();

    g_Map.Logic_AI();

    // captainslog_debug("LogicClass::AI() - Factories.Count = %d", Teams.Count());
    for (int factory = 0; factory < Factories.Count(); ++factory) {
        FactoryClass *fptr = Factories[factory];
        captainslog_assert(fptr != nullptr);
        if (fptr != nullptr) {
            if (fptr->Is_Active()) {
                captainslog_debug("LogicClass::AI() - About to call FactoryClass::AI() on factory '%d'", factory);
                fptr->AI();
            }
        }
    }

    if (g_Session.GameToPlay != GAME_CAMPAIGN) {
        for (HousesType mphouse = HOUSES_MULTI_FIRST; mphouse < HOUSES_MULTI_LAST; ++mphouse) {
            HouseClass *hptr = HouseClass::As_Pointer(mphouse);
            captainslog_assert(hptr != nullptr);

            if (hptr != nullptr) {
                if (hptr->Is_Active()) {
                    hptr->AI();
                }
            }
        }

    } else {
        for (HousesType house = HOUSES_FIRST; house < HOUSES_COUNT; ++house) {
            HouseClass *hptr = HouseClass::As_Pointer(house);
            captainslog_assert(hptr != nullptr);
            if (hptr != nullptr) {
                if (hptr->Is_Active()) {
                    hptr->AI();
                }
            }
        }
    }

    if (g_Session.GameToPlay != GAME_CAMPAIGN) {
        if (g_Scen.AutoSonarTimer <= 0) {
            if (AutoSonarPulse) {
                g_Map.Activate_Pulse();
                VocManager.Play_Locally(VOC_SONPULSE);
                AutoSonarPulse = false;
            }

            g_Scen.AutoSonarTimer = 600;
        }
    }
#endif
}

void LogicClass::Detach(target_t target, int a2)
{
#ifdef GAME_DLL
    void (*func)(LogicClass *, target_t, int) = reinterpret_cast<void (*)(LogicClass *, target_t, int)>(0x004FE2BC);
    return func(this, target, a2);
#elif 0

    // TODO: This function requires TriggerClass implimentation.

    if ((RTTIType)a2 == RTTI_TRIGGER) {
        for (int index = 0; index < LogicTriggers.Count(); ++index) {
            TriggerClass *ptr = LogicTriggers[index];
            captainslog_assert(ptr != nullptr);

            if (As_Trigger(target) == ptr) {
                LogicTriggers.Delete_Index(index);
            }
        }
    }
#endif
}
