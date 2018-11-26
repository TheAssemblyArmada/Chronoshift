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
#include "gamekeyboard.h"
#include "gameoptions.h"
#include "gamedebug.h"
#include "globals.h"
#include "display.h"
#include "iomap.h"
#include "house.h"
#include "queue.h"
#include "gameevent.h"
#include "queue.h"
#include "techno.h"
//#include "vortex.h"
#include "building.h"
#include "unit.h"
#include "theme.h"
#include "coord.h"
#include "rules.h"
#include "audio.h"
#include "scenario.h"
#include "msglist.h"
#include "session.h"
#include "palette.h"
#include "pcx.h"

static BOOL g_Debug_Keys = false;

void Handle_Team(int a1, int a2)
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(int, int) = reinterpret_cast<void (*)(int, int)>(0x004A9FAC);
    func(a1, a2);
#endif
}

void Handle_View(int view, int a2)
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(int, int) = reinterpret_cast<void (*)(int, int)>(0x004AAA98);
    func(view, a2);
#else
    /*if ( view < VIEW_COUNT ) {
        if ( a2 ) {
            Scen.Set_View(view, Coord_To_Cell(Map.Get_DisplayPos()) + (8 * 128)); // 1034
        } else {
            Map.Set_Tactical_Position(Cell_To_Coord(Scen.Get_View(view)) & 0xFF00FF00);
            Map.Flag_To_Redraw(true);
        }
    }*/
#endif
}

void Toggle_Formation()
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)() = reinterpret_cast<void (*)()>(0x004A6300);
    func();
#endif
}

void Message_Input(KeyNumType &key)
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(KeyNumType &) = reinterpret_cast<void (*)(KeyNumType &)>(0x004A6BB0);
    func(key);
#else
    //TODO
#endif
}

#if defined(CHRONOSHIFT_DEBUG)
void Debug_Keyboard_Process(KeyNumType &key)
{
    if (key != KN_NONE) {
        
        KeyNumType justkey = (KeyNumType)(key & ~(KEY_SHIFT_BIT | KEY_CTRL_BIT | KEY_ALT_BIT | KEY_VK_BIT));
        KeyNumType keynum = (KeyNumType)(key & ~(KEY_VK_BIT));

        int modifierkey = 0;

        if (key & KEY_SHIFT_BIT) {
            modifierkey = 1;
        }
        if (key & KEY_CTRL_BIT) {
            modifierkey = 2;
        }
        if (key & KEY_ALT_BIT) {
            modifierkey = 3;
        }

        if (justkey == Options.Get_DebugKeyToggleVortex()) {
            static bool _vortex_active = false;
            //TODO: Requires ChronalVortexClass.
            if (!_vortex_active) {
                coord_t coord = Map.Pixel_To_Coord(g_wwmouse->Get_Mouse_X(), g_wwmouse->Get_Mouse_Y());
                if (coord) {
                    //ChronalVortex.Appear(coord);
                    //ChronalVortex.Set_Target(nullptr);
                    Sound_Effect(VOC_TESLA1, coord);
                    _vortex_active = true;
                }
            } else {
                //ChronalVortex.Set_Target(nullptr);
                //ChronalVortex.Disappear();
                _vortex_active = false;
            }
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyForceRedraw()) {
            Map.Flag_All_Cells_To_Redraw();
            Map.All_Layers_To_Redraw();
            Map.Flag_To_Redraw(true);
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyRandomCrate()) {
            Map.Place_Random_Crate();
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyRandomCrateAtMouse()) {
            coord_t coord = Map.Pixel_To_Coord(g_wwmouse->Get_Mouse_X(), g_wwmouse->Get_Mouse_Y());
            cell_t cell = Coord_To_Cell(coord);
            Map.Place_Random_Crate_At_Cell(cell);
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyFreeMoney()) {
            static int _free_money = 2500;
            g_PlayerPtr->Refund_Money(_free_money);
            Session.Get_Messages().Add_Simple_Message("Debug", "Player has been rewarded free money!", PLAYER_COLOR_LIGHT_BLUE);
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyForceWin()) {
            g_PlayerPtr->Flag_To_Win();
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyForceLose()) {
            g_PlayerPtr->Flag_To_Lose();
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyForceDie()) {
            g_PlayerPtr->Flag_To_Die();
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyBailOut()) {
            //Emergency_Exit(-1);
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyCellIcon()) {
            g_Debug_Icon = !g_Debug_Icon;
            Map.Flag_To_Redraw(true);
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyUnshroud()) {
            g_Debug_Unshroud = !g_Debug_Unshroud;
            Map.Flag_To_Redraw(true);
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeySightRange()) {
            g_Debug_SightRange = !g_Debug_SightRange;
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyGuardRange()) {
            g_Debug_GuardRange = !g_Debug_GuardRange;
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyWeaponRange()) {
            g_Debug_WeaponRange = !g_Debug_WeaponRange;
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyAttackFriendlies()) {
            g_Debug_AttackFriendlies = !g_Debug_AttackFriendlies;
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyNavList()) {
            g_Debug_NavList = !g_Debug_NavList;
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyFindPath()) {
            g_Debug_Find_Path = !g_Debug_Find_Path;
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyToggleMono()) {
            g_MonoEnabled = !g_MonoEnabled;
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyPrevMonoPage()) {
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyNextMonoPage()) {
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeySpawnMCV()) {
            coord_t coord = Map.Pixel_To_Coord(g_wwmouse->Get_Mouse_X(), g_wwmouse->Get_Mouse_Y());
            if ( coord ) {
                //TODO: Requires UnitClass.
                /*UnitClass *uptr = new UnitClass(UNIT_MCV, g_PlayerPtr->What_Type());
                DEBUG_ASSERT(uptr != nullptr);
                if ( uptr != nullptr ) {
                    uptr->Unlimbo(Map.Pixel_To_Coord(g_wwmouse->Get_Mouse_X(), g_wwmouse->Get_Mouse_Y()));
                }*/
            }
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeySpawnHarvester()) {
            coord_t coord = Map.Pixel_To_Coord(g_wwmouse->Get_Mouse_X(), g_wwmouse->Get_Mouse_Y());
            if ( coord ) {
                //TODO: Requires UnitClass.
                /*UnitClass *uptr = new UnitClass(UNIT_HARVESTER, g_PlayerPtr->What_Type());
                DEBUG_ASSERT(uptr != nullptr);
                if ( uptr != nullptr ) {
                    uptr->Unlimbo(Map.Pixel_To_Coord(g_wwmouse->Get_Mouse_X(), g_wwmouse->Get_Mouse_Y()));
                }*/
            }
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeySpawnHeli()) {
            coord_t coord = Map.Pixel_To_Coord(g_wwmouse->Get_Mouse_X(), g_wwmouse->Get_Mouse_Y());
            if ( coord ) {
                //TODO: Requires AircraftClass.
                /*AircraftClass *aptr = new AircraftClass(AIRCRAFT_HELI, g_PlayerPtr->What_Type());
                DEBUG_ASSERT(aptr != nullptr);
                if ( aptr != nullptr ) {
                    aptr->Set_Height(0);
                    aptr->Unlimbo(coord);
                }*/
            }
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeySpawnHind()) {
            coord_t coord = Map.Pixel_To_Coord(g_wwmouse->Get_Mouse_X(), g_wwmouse->Get_Mouse_Y());
            if (coord) {
                //TODO: Requires AircraftClass.
                /*AircraftClass *aptr = new AircraftClass(AIRCRAFT_HIND, g_PlayerPtr->What_Type());
                DEBUG_ASSERT(aptr != nullptr);
                if (aptr != nullptr) {
                    aptr->Set_Height(0);
                    aptr->Unlimbo(coord);
                }*/
            }
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeySpawnTransport()) {
            coord_t coord = Map.Pixel_To_Coord(g_wwmouse->Get_Mouse_X(), g_wwmouse->Get_Mouse_Y());
            if ( coord ) {
                //TODO: Requires AircraftClass.
                /*AircraftClass *aptr = new AircraftClass(AIRCRAFT_TRANSPORT, g_PlayerPtr->What_Type());
                DEBUG_ASSERT(aptr != nullptr);
                if ( aptr != nullptr ) {
                    aptr->Set_Height(0);
                    aptr->Unlimbo(coord);
                }*/
            }
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyAtomExplosion()) {
            coord_t coord = Map.Pixel_To_Coord(g_wwmouse->Get_Mouse_X(), g_wwmouse->Get_Mouse_Y());
            if ( coord ) {
                //TODO: Requires AnimClass, Explosion_Damage.
                /*AnimClass *aptr = new AnimClass(ANIM_ATOMSFX, coord);
                DEBUG_ASSERT(aptr != nullptr);
                if ( aptr != nullptr ) {
                    Explosion_Damage(coord, Rule.AtomDamage, nullptr, WARHEAD_HE);
                }*/
            }
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeySuperExplosion()) {
            coord_t coord = Map.Pixel_To_Coord(g_wwmouse->Get_Mouse_X(), g_wwmouse->Get_Mouse_Y());
            if ( coord ) {
                //TODO: Requires AnimClass, Explosion_Damage.
                /*AnimClass *aptr = new AnimClass(ANIM_ATOMSFX, coord);
                DEBUG_ASSERT(aptr != nullptr);
                if ( aptr != nullptr ) {
                    Explosion_Damage(coord, Rule.AtomDamage, nullptr, WARHEAD_HE);
                }*/
            }
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyExplosion()) {
            coord_t coord = Map.Pixel_To_Coord(g_wwmouse->Get_Mouse_X(), g_wwmouse->Get_Mouse_Y());
            if ( coord ) {
                //TODO: Requires AnimClass, Explosion_Damage.
                /*AnimClass *aptr = new AnimClass(ANIM_ART_EXP1, coord);
                DEBUG_ASSERT(aptr != nullptr);
                if ( aptr != nullptr ) {
                    Explosion_Damage(coord, 250, nullptr, WARHEAD_HE);
                }*/
            }
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyDeleteObject()) {
            if ( CurrentObjects.Count() > 0 ) {
                for (int index = 0; index < CurrentObjects.Count(); ++index) {
                    ObjectClass *objptr = CurrentObjects[index];
                    DEBUG_ASSERT(objptr != nullptr);
                    if (objptr != nullptr) {
                        if (objptr->Is_Active() && !objptr->In_Limbo()) {
                            delete objptr;
                            Session.Get_Messages().Add_Simple_Message("Debug", "Object deleted!", PLAYER_COLOR_LIGHT_BLUE);
                        }
                    }
                }
            }
            Map.Recalc();
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyScreenshot()) {
            char filename[256];
            static GraphicBufferClass temp_page(g_seenBuff.Get_Width(), g_seenBuff.Get_Height(), nullptr, g_seenBuff.Get_Width() * g_seenBuff.Get_Height());
            g_seenBuff.Blit(temp_page, g_seenBuff.Get_XPos(), g_seenBuff.Get_YPos(), g_seenBuff.Get_XPos(), g_seenBuff.Get_YPos(), g_seenBuff.Get_Width(), g_seenBuff.Get_Height());
            for (int index = 0; index < 9999; ++index ) {
                snprintf(filename, sizeof(filename), "scrn%04d.pcx", index);
                if (!GameFileClass::File_Available(filename)) {
                    break;
                }
            }
            RawFileClass scrnfile(filename);
            Write_PCX_File(scrnfile, temp_page, GamePalette);
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "Screenshot Saved! (%s in your user home directory))", filename);
            Session.Get_Messages().Add_Simple_Message("System", buffer, PLAYER_COLOR_LIGHT_BLUE);
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyMotionCapture()) {
            g_Debug_MotionCapture = true;
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyThemeNext()) {
            Theme.Play_Next();
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyThemePrev()) {
            Theme.Play_Prev();
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeySpecialSonarPulse()) {
            g_PlayerPtr->Special_Weapons(SPECIAL_SONAR_PULSE).Enable(true);
            g_PlayerPtr->Special_Weapons(SPECIAL_SONAR_PULSE).Forced_Charge();
            Map.Add(RTTI_SPECIAL, SPECIAL_SONAR_PULSE);
            //Map.Right_Strip_Flag_To_Redraw();
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeySpecialAtomBomb()) {
            g_PlayerPtr->Special_Weapons(SPECIAL_ATOM_BOMB).Enable(true);
            g_PlayerPtr->Special_Weapons(SPECIAL_ATOM_BOMB).Forced_Charge();
            Map.Add(RTTI_SPECIAL, SPECIAL_ATOM_BOMB);
            //Map.Right_Strip_Flag_To_Redraw();
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeySpecialWarpSphere()) {
            g_PlayerPtr->Special_Weapons(SPECIAL_WARP_SPHERE).Enable(true);
            g_PlayerPtr->Special_Weapons(SPECIAL_WARP_SPHERE).Forced_Charge();
            Map.Add(RTTI_SPECIAL, SPECIAL_WARP_SPHERE);
            //Map.Right_Strip_Flag_To_Redraw();
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeySpecialParaBomb()) {
            g_PlayerPtr->Special_Weapons(SPECIAL_PARA_BOMB).Enable(true);
            g_PlayerPtr->Special_Weapons(SPECIAL_PARA_BOMB).Forced_Charge();
            Map.Add(RTTI_SPECIAL, SPECIAL_PARA_BOMB);
            //Map.Right_Strip_Flag_To_Redraw();
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeySpecialParaInfantry()) {
            g_PlayerPtr->Special_Weapons(SPECIAL_PARA_INFANTRY).Enable(true);
            g_PlayerPtr->Special_Weapons(SPECIAL_PARA_INFANTRY).Forced_Charge();
            Map.Add(RTTI_SPECIAL, SPECIAL_PARA_INFANTRY);
            //Map.Right_Strip_Flag_To_Redraw();
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeySpecialSpyPlane()) {
            g_PlayerPtr->Special_Weapons(SPECIAL_SPY_PLANE).Enable(true);
            g_PlayerPtr->Special_Weapons(SPECIAL_SPY_PLANE).Forced_Charge();
            Map.Add(RTTI_SPECIAL, SPECIAL_SPY_PLANE);
            //Map.Right_Strip_Flag_To_Redraw();
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeySpecialIronCurtain()) {
            g_PlayerPtr->Special_Weapons(SPECIAL_IRON_CURTAIN).Enable(true);
            g_PlayerPtr->Special_Weapons(SPECIAL_IRON_CURTAIN).Forced_Charge();
            Map.Add(RTTI_SPECIAL, SPECIAL_IRON_CURTAIN);
            //Map.Right_Strip_Flag_To_Redraw();
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeySpecialGPS()) {
            g_PlayerPtr->Special_Weapons(SPECIAL_GPS).Enable(true);
            g_PlayerPtr->Special_Weapons(SPECIAL_GPS).Forced_Charge();
            Map.Add(RTTI_SPECIAL, SPECIAL_GPS);
            //Map.Right_Strip_Flag_To_Redraw();
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyInstantBuild()) {
            g_Debug_InstantBuild = !g_Debug_InstantBuild;
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyBuildCheat()) {
            g_Debug_BuildCheat = !g_Debug_BuildCheat;
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyAIControl()) {
            g_Debug_AIControl = !g_Debug_AIControl;
            static int _player_iq = g_PlayerPtr->Get_Current_IQ();
            static DiffType _player_diff = g_PlayerPtr->Get_AI_Difficulty();
            if (g_PlayerPtr->Is_Human() && g_PlayerPtr->Player_Has_Control()) {
                // Let AI assume control.
                g_PlayerPtr->Set_Human(false);
                g_PlayerPtr->Set_Player_Control(false);
                g_PlayerPtr->Set_Production(true);
                g_PlayerPtr->Set_Autocreate(true);

                g_PlayerPtr->Set_IQ_Level(Rule.IQ_Controls().MaxIQLevels);
                g_PlayerPtr->Set_AI_Difficulty(DIFF_HARDEST);

                Session.Get_Messages().Add_Simple_Message("Debug", "AI has taken control of the player!", PLAYER_COLOR_LIGHT_BLUE);
            } else {
                // Re-assume control.
                g_PlayerPtr->Set_Human(true);
                g_PlayerPtr->Set_Player_Control(true);
                g_PlayerPtr->Set_Production(false);
                g_PlayerPtr->Set_Autocreate(false);

                g_PlayerPtr->Set_IQ_Level(_player_iq);
                g_PlayerPtr->Set_AI_Difficulty(_player_diff);

                Session.Get_Messages().Add_Simple_Message("Debug", "Player has now re-assumed control!", PLAYER_COLOR_LIGHT_BLUE);
            }
            Map.Recalc();
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyStealObject()) {
            if ( CurrentObjects.Count() > 0 ) {
                for (size_t index = 0; index < CurrentObjects.Count(); ++index) {
                    TechnoClass *objptr = reinterpret_cast<TechnoClass *>(CurrentObjects[index]);
                    DEBUG_ASSERT(objptr != nullptr);
                    if (objptr != nullptr) {
                        if (objptr->Is_Active() && !objptr->In_Limbo()) {
                            if (!objptr->Get_Owner_House()->Is_Player()) {
                                objptr->Captured(g_PlayerPtr);
                                Session.Get_Messages().Add_Simple_Message("Debug", "Captured object!", PLAYER_COLOR_LIGHT_BLUE);
                            }
                        }
                    }
                }
            }
            Map.Recalc();
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyToggleDamage()) {
            g_Debug_Damage = !g_Debug_Damage;
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyToggleCloakable()) {
            if (CurrentObjects.Count() > 0) {
                for (size_t index = 0; index < CurrentObjects.Count(); ++index) {
                    TechnoClass *objptr = reinterpret_cast<TechnoClass *>(CurrentObjects[index]);
                    DEBUG_ASSERT(objptr != nullptr);
                    if (objptr != nullptr) {
                        if (objptr->Is_Active() && !objptr->In_Limbo()) {
                            objptr->Set_Cloakable(!objptr->Is_Cloakable());
                        }
                    }
                }
            }
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeyApplyDamage()) {
            if (CurrentObjects.Count() > 0) {
                for (size_t index = 0; index < CurrentObjects.Count(); ++index) {
                    TechnoClass *objptr = reinterpret_cast<TechnoClass *>(CurrentObjects[index]);
                    DEBUG_ASSERT(objptr != nullptr);
                    if (objptr != nullptr) {
                        if (objptr->Is_Active() && !objptr->In_Limbo()) {
                            int damage = 50;
                            objptr->Take_Damage(damage, 0, WARHEAD_SA);
                        }
                    }
                }
            }
            key = KN_NONE;
        }

        // TODO: Update radar.
        // TODO: Find out why extended area is black.
        if (justkey == Options.Get_DebugKeyToggleFullMap()) {
            static int _mapcellx = -1;
            static int _mapcelly = -1;
            static int _mapcellwidth = -1;
            static int _mapcellheight = -1;
            static bool _toggle = false;
            if (_toggle == false) {
                _mapcellx = Map.Get_Map_Cell_X();
                _mapcelly = Map.Get_Map_Cell_Y();
                _mapcellwidth = Map.Get_Map_Cell_Width();
                _mapcellheight = Map.Get_Map_Cell_Height();
                Map.Set_Map_Dimensions(1, 1, 126, 126);
                _toggle = true;
            } else {
                Map.Set_Map_Dimensions(_mapcellx, _mapcelly, _mapcellwidth, _mapcellheight);
                _mapcellx = -1;
                _mapcelly = -1;
                _mapcellwidth = -1;
                _mapcellheight = -1;
                _toggle = false;
            }
            Map.Flag_All_Cells_To_Redraw();
            Map.Flag_To_Redraw(true);
            key = KN_NONE;
        }

        if (justkey == Options.Get_DebugKeySpecialDialog()) {
            if (Session.Game_To_Play() == GAME_CAMPAIGN) {
                //Special_Dialog();
            } else {
                //g_SpecialDialog = SPECIAL_DLG_SPECIAL;
                key = KN_NONE;
            }
        }

    }

}
#endif

void Keyboard_Process(KeyNumType &key)
{
    // Distance to scroll on scroll commands. 256 == 1 cell.
    static int _scroll_dist = 256;

    if (key != KN_NONE) {

        Message_Input(key);

        ObjectClass *objptr = nullptr;
        ObjectClass *curobj = nullptr;

        KeyNumType justkey = (KeyNumType)(key & ~(KEY_SHIFT_BIT | KEY_CTRL_BIT | KEY_ALT_BIT | KEY_VK_BIT));
        KeyNumType keynum = (KeyNumType)(key & ~(KEY_VK_BIT));

        int modifierkey = 0;

        if (key & KEY_SHIFT_BIT) {
            modifierkey = 1;
        }
        if (key & KEY_CTRL_BIT) {
            modifierkey = 2;
        }
        if (key & KEY_ALT_BIT) {
            modifierkey = 3;
        }

        if (justkey == Options.Get_KeyNext()) {
            if (modifierkey == 0) {
                if (CurrentObjects.Count() > 0) {
                    curobj = CurrentObjects.Fetch_Head();
                } else {
                    curobj = nullptr;
                }
                objptr = Map.Next_Object(curobj);
            } else {
                if (CurrentObjects.Count() > 0) {
                    curobj = CurrentObjects.Fetch_Head();
                } else {
                    curobj = nullptr;
                }
                objptr = Map.Prev_Object(curobj);
            }
            DEBUG_ASSERT(objptr != nullptr);
            if (objptr != nullptr) {
                DEBUG_LOG("Keyboard_Process() - Jumping to next object %s.\n", objptr->Name());
                Unselect_All();
                objptr->Select();
                Map.Center_Map();
                Map.Flag_To_Redraw(true);
            }
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyPrevious()) {
            if (modifierkey == 0) {
                if (CurrentObjects.Count() > 0) {
                    curobj = CurrentObjects.Fetch_Head();
                } else {
                    curobj = nullptr;
                }
                objptr = Map.Prev_Object(curobj);
            } else {
                if (CurrentObjects.Count() > 0) {
                    curobj = CurrentObjects.Fetch_Head();
                } else {
                    curobj = nullptr;
                }
                objptr = Map.Next_Object(curobj);
            }
            DEBUG_ASSERT(objptr != nullptr);
            if (objptr != nullptr) {
                DEBUG_LOG("Keyboard_Process() - Jumping to prev object %s.\n", objptr->Name());
                Unselect_All();
                objptr->Select();
                Map.Center_Map();
                Map.Flag_To_Redraw(true);
            }
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyStop()) {
            for (int index = 0; index < CurrentObjects.Count(); ++index) {
                objptr = CurrentObjects[index];
                DEBUG_ASSERT(objptr != nullptr);
                if (objptr != nullptr) {
                    if (objptr->Can_Player_Move() && objptr->What_Am_I() != RTTI_BUILDING) {
                        GameEventClass ev(GameEventClass::EVENT_IDLE, TargetClass(objptr));
                        OutgoingEvents.Add(ev);
                    }
                }
            }
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyGuard()) {
            for (int index = 0; index < CurrentObjects.Count(); ++index) {
                objptr = CurrentObjects[index];
                DEBUG_ASSERT(objptr != nullptr);
                if (objptr != nullptr) {
                    if (objptr->Can_Player_Move() && objptr->Can_Player_Fire()) {
                        GameEventClass ev(TargetClass(objptr), MISSION_AREA_GUARD, TargetClass(0), TargetClass(0));
                        OutgoingEvents.Add(ev);
                    }
                }
            }
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyScatter()) {
            for (int index = 0; index < CurrentObjects.Count(); ++index) {
                objptr = CurrentObjects[index];
                DEBUG_ASSERT(objptr != nullptr);
                if (objptr != nullptr) {
                    if (objptr->Can_Player_Move()) {
                        GameEventClass ev(GameEventClass::EVENT_SCATTER, TargetClass(objptr));
                        OutgoingEvents.Add(ev);
                    }
                }
            }
            key = KN_NONE;
        }

        if ((justkey == Options.Get_KeyHome1() || key == Options.Get_KeyHome2())) {
            if (CurrentObjects.Count() > 0) {
                Map.Center_Map();
                Map.Flag_To_Redraw(true);
                key = KN_NONE;
            } else {
                key = Options.Get_KeyBase();
            }
        }

        if (justkey == Options.Get_KeyBase()) {

            bool selected = false;
            coord_t object_coord = 0;

            Unselect_All();

            // If we have a construction yard, center in on that.
            if (!selected && g_PlayerPtr->Currently_Owned_Building_Count() > 0) {
                for (int index = 0; index < g_Buildings.Count(); ++index) {
                    BuildingClass *bptr = &g_Buildings[index];
                    DEBUG_ASSERT(bptr != nullptr);
                    if (bptr != nullptr) {
                        if (!bptr->In_Limbo() && bptr->Get_Owner_House()->Is_Player()) {
                            if (bptr->What_Type() == BUILDING_FACT) {
                                Unselect_All();
                                if (bptr->Is_Primary()) {
                                    bptr->Select();
                                    selected = true;
                                    object_coord = bptr->Center_Coord();
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            // Or, if we have a mcv, center in on that instead.
            if (!selected && g_PlayerPtr->Currently_Owned_Unit_Count() > 0) {
                for (int index = 0; index < g_Units.Count(); ++index) {
                    UnitClass *uptr = &g_Units[index];
                    DEBUG_ASSERT(uptr != nullptr);
                    if (uptr != nullptr) {
                        if (!uptr->In_Limbo() && uptr->Get_Owner_House()->Is_Player()) {
                            if (uptr->What_Type() == UNIT_MCV) {
                                Unselect_All();
                                uptr->Select();
                                selected = true;
                                object_coord = uptr->Center_Coord();
                                break;
                            }
                        }
                    }
                }
            }

            coord_t center_coord = (selected ? object_coord : g_PlayerPtr->Base_Center());
            if (center_coord) {
                Map.Center_Map(center_coord);
            }

            Map.Flag_To_Redraw(true);

            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyFormation()) {
            Toggle_Formation();
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyAlliance()) {
            if ( (g_Debug_Flag || Session.Game_To_Play() != GAME_CAMPAIGN) && CurrentObjects.Count() > 0 && !g_PlayerPtr->Is_Defeated() ) {
                ObjectClass *objptr = CurrentObjects.Fetch_Head();
                DEBUG_ASSERT(objptr != nullptr);
                if ( objptr != nullptr ) {
                    if ( objptr->Owner() == g_PlayerPtr->What_Type() ) {
                        GameEventClass ev(GameEventClass::EVENT_ALLY, objptr->Owner());
                        OutgoingEvents.Add(ev);
                    }
                }
            }
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeySelectView()) {
            // Value of 0 is top left of the screen.
            Map.Select_These(0, Coord_From_Lepton_XY(Map.Get_DisplayWidth(), Map.Get_DisplayHeight()));
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyRepairToggle()) {
            Map.Repair_Mode_Control(MODE_CONTROL_TOGGLE);
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeySellToggle()) {
            Map.Sell_Mode_Control(MODE_CONTROL_TOGGLE);
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyMapToggle()) {
            Map.Zoom_Mode_Control();
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeySidebarUp()) {
            Map.Scroll(true);
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeySidebarDown()) {
            Map.Scroll(false);
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyOption1() || key == Options.Get_KeyOption2()) {
            Map.Help_Text(TXT_NULL);
            Queue_Options();
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyScrollLeft()) {
            Map.Scroll_Map(DIR_WEST, _scroll_dist);
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyScrollRight()) {
            Map.Scroll_Map(DIR_EAST, _scroll_dist);
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyScrollUp()) {
            Map.Scroll_Map(DIR_NORTH, _scroll_dist);
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyScrollDown()) {
            Map.Scroll_Map(DIR_SOUTH, _scroll_dist);
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyTeam1() || justkey == '1') {
            Handle_Team(0, modifierkey);
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyTeam2() || justkey == '2') {
            Handle_Team(1, modifierkey);
            key = KN_NONE;
        }

        if ((justkey == Options.Get_KeyTeam3() || justkey == '3')) {
            Handle_Team(2, modifierkey);
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyTeam4() || justkey == '4') {
            Handle_Team(3, modifierkey);
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyTeam5() || justkey == '5') {
            Handle_Team(4, modifierkey);
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyTeam6() || justkey == '6') {
            Handle_Team(5, modifierkey);
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyTeam7() || justkey == '7') {
            Handle_Team(6, modifierkey);
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyTeam8() || justkey == '8') {
            Handle_Team(7, modifierkey);
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyTeam9() || justkey == '9') {
            Handle_Team(8, modifierkey);
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyTeam10() || justkey == '0') {
            Handle_Team(9, modifierkey);
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyBookmark1()) {
            Handle_View(0, modifierkey);
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyBookmark2()) {
            Handle_View(1, modifierkey);
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyBookmark3()) {
            Handle_View(2, modifierkey);
            key = KN_NONE;
        }

        if (justkey == Options.Get_KeyBookmark4()) {
            Handle_View(3, modifierkey);
            key = KN_NONE;
        }

    #if defined(CHRONOSHIFT_DEBUG)
        if (Session.Game_To_Play() == GAME_CAMPAIGN || Session.Game_To_Play() == GAME_SKIRMISH) {
            if (justkey == Options.Get_DebugKeyToggleDebug()) {
                g_Debug_Keys = !g_Debug_Keys;
                DEBUG_LOG("Debug hotkeys %s!", (g_Debug_Keys ? "enabled" : "disabled"));
                if (g_Debug_Keys) {
                    Session.Get_Messages().Add_Simple_Message("System", "Debug hotkeys enabled!", PLAYER_COLOR_LIGHT_BLUE);
                }
            }
            if (g_Debug_Keys || g_Debug_Flag) {
                Debug_Keyboard_Process(key);
            }
        }
    #endif

    }

}
