/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Functions for handling the "attract" demo mode.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "attract.h"
#include "gameoptions.h"
#include "globals.h"
#include "special.h"
#include "scenario.h"
#include "iomap.h"
#include "target.h"
#include "session.h"
//#include "formation.h"

void Do_Record_Playback()
{
    // TODO: This doesn't work right in the original game and needs some formation stuff to complete here.
    if (g_Session.Record_Game()) {
        // When super record is enabled, a single record file can contain multiple games. so we
        // seek to the end of the file and continue writing onto the previous session.
        if (g_Session.Super_Record_Game()) {
            g_Session.Recording_File().Open(FM_READ_WRITE);
            g_Session.Recording_File().Seek(0, FS_SEEK_END);
        }

        // Write the current tactical position.
        coord_t new_pos = g_Map.New_Pos();
        g_Session.Recording_File().Write(&new_pos, sizeof(new_pos));

        // Write the total number of selected objects.
        int currobj_cnt = CurrentObjects.Count();
        g_Session.Recording_File().Write(&currobj_cnt, sizeof(currobj_cnt));

        // Write a total checksum of all the currently selected objects.
        target_t total_obj_target = 0;
        for (int index = 0; index < currobj_cnt; ++index) {
            total_obj_target += As_Target(CurrentObjects[index]);
        }
        g_Session.Recording_File().Write(&total_obj_target, sizeof(total_obj_target));

        // Write target lookup values for each of the selected objects.
        for (int index = 0; index < currobj_cnt; ++index) {
            target_t target = As_Target(CurrentObjects[index]);
            g_Session.Recording_File().Write(&target, sizeof(target));
        }

        g_Session.Recording_File().Write(&g_TeamEvent, 4);
        g_Session.Recording_File().Write(&g_TeamNumber, 4);
        //g_Session.Recording_File().Write(&g_TeamMaxSpeed, 4);
        //g_Session.Recording_File().Write(&g_TeamSpeed, 4);
        g_Session.Recording_File().Write(&g_FormationEvent, 4);
        g_Session.Recording_File().Write(&g_FormMove, 4);
        g_Session.Recording_File().Write(&g_FormSpeed, 4);
        g_Session.Recording_File().Write(&g_FormMaxSpeed, 4);

        g_TeamEvent = TEAM_EVENT_NONE;
        //g_TeamNumber = TEAM_NUMBER_NONE;
        //g_FormationEvent = FORMATION_EVENT_NONE;

        if (g_Session.Super_Record_Game()) {
            g_Session.Recording_File().Close();
        }
    }

    if (g_Session.Playback_Game()) {
        // Read and assign the new tactical position.
        coord_t tact_pos = 0;
        if (g_Session.Recording_File().Read(&tact_pos, sizeof(tact_pos)) == sizeof(tact_pos) && tact_pos != g_Map.New_Pos()) {
            g_Map.Set_Tactical_Position(tact_pos);
        }

        // Read and update the current selected objects.
        int32_t currobj_cnt = 0;
        if (g_Session.Recording_File().Read(&currobj_cnt, sizeof(currobj_cnt)) == sizeof(currobj_cnt)) {
            target_t calc_total_obj_target = 0;

            for (int index = 0; index < currobj_cnt; ++index) {
                calc_total_obj_target += As_Target(CurrentObjects[index]);
            }

            // Read the saved total target checksum.
            target_t total_obj_target_checksum = 0;
            g_Session.Recording_File().Read(&total_obj_target_checksum, 4);

            if (total_obj_target_checksum != calc_total_obj_target) {
                Unselect_All();
            }

            g_AllowVoice = true;

            // Read out the select object targets and select them they match.
            target_t obj_target = 0;

            for (int index = 0; index < currobj_cnt; ++index) {
                if (g_Session.Recording_File().Read(&obj_target, sizeof(obj_target)) == sizeof(obj_target)) {
                    ObjectClass *obj = As_Object(obj_target);

                    if (obj != nullptr) {
                        if (total_obj_target_checksum != calc_total_obj_target) {
                            obj->Select();
                            g_AllowVoice = false;
                        }
                    }
                }
            }

            g_AllowVoice = true;
        }

        g_Session.Recording_File().Read(&g_TeamEvent, sizeof(g_TeamEvent));
        g_Session.Recording_File().Read(&g_TeamNumber, sizeof(g_TeamNumber));
        //g_Session.Recording_File().Read(&g_TeamMaxSpeed, 4); // Saves this as a DWORD.
        //g_Session.Recording_File().Read(&g_TeamSpeed, 4); // Saves this as a DWORD.

        if (g_TeamEvent != TEAM_EVENT_0) {
            //Handle_Team(g_TeamNumber, g_TeamEvent);
        }

        g_Session.Recording_File().Read(&g_FormationEvent, 4); // Saves this as a DWORD.

        //if (g_FormationEvent != FORMATION_EVENT_0) {
        //    FormationManager.Toggle();
        //}

        g_Session.Recording_File().Read(&g_FormMove, sizeof(g_FormMove)); // Saves this as a DWORD.
        g_Session.Recording_File().Read(&g_FormSpeed, sizeof(g_FormSpeed)); // Saves this as a DWORD.
        g_Session.Recording_File().Read(&g_FormMaxSpeed, sizeof(g_FormMaxSpeed)); // Saves this as a DWORD.

        // Force a full render of the game viewport.
        g_Map.Render();
    }
}

BOOL Save_Recording_Values(GameFileClass &recfile)
{
    // TODO: revist this code when overhauling Save/Load system.
    if (g_Session.Save(recfile)) {
        recfile.Write(&g_BuildLevel, sizeof(g_BuildLevel)); // Saves this as a DWORD.
        recfile.Write(&g_Debug_Unshroud, sizeof(g_Debug_Unshroud)); // Saves this as a DWORD.
        //recfile.Write(&IPX.unk01, sizeof(IPX.unk01));
        recfile.Write(&g_Seed, sizeof(g_Seed)); // Saves this as a DWORD.
        uint32_t index = g_Scen.Get_Scenario_Index();
        recfile.Write(&index, sizeof(index));
        recfile.Write(g_Scen.Scenario_Name(), ScenarioClass::SCENARIO_NAME_LEN);
        uint8_t tmp8 = g_Whom;
        recfile.Write(&tmp8, sizeof(tmp8)); // Saves as a byte in original
        uint32_t tmp32 = s_Special.Pack();
        recfile.Write(&tmp32, sizeof(tmp32));
        recfile.Write(&g_Options, sizeof(g_Options));

        captainslog_info("Saved recording values for scenario : %s\n", g_Scen.Scenario_Name());

        if (g_Session.Super_Record_Game()) {
            g_Session.Recording_File().Close();
        }

        return true;
    }

    return false;
}

BOOL Load_Recording_Values(GameFileClass &recfile)
{
    // TODO: revist this code when overhauling Save/Load system.
    if (g_Session.Load(recfile)) {
        recfile.Read(&g_BuildLevel, sizeof(g_BuildLevel)); // Saves this as a DWORD.
        recfile.Read(&g_Debug_Unshroud, /*sizeof(g_Debug_Unshroud)*/ 4); // Saves this as a DWORD.
        // recfile.Read(&IPX.unk01, sizeof(IPX.unk01));
        recfile.Read(&g_Seed, sizeof(g_Seed)); // Saves this as a DWORD.
        uint32_t index;
        recfile.Read(&index, sizeof(index));
        g_Scen.Set_Scenario_Index(index);
        char name_buff[ScenarioClass::SCENARIO_NAME_LEN];
        recfile.Read(name_buff, sizeof(name_buff));
        g_Scen.Set_Scenario_Name(name_buff);
        uint8_t tmp8;
        recfile.Read(&tmp8, sizeof(tmp8)); // Saves this as a BYTE.
        g_Whom = HousesType(tmp8);
        uint32_t tmp32;
        recfile.Read(&tmp32, sizeof(tmp32));
        s_Special.Unpack(tmp32);
        recfile.Read(&g_Options, sizeof(g_Options));

        captainslog_debug("Loaded recording values for scenario : %s\n", g_Scen.Scenario_Name());

        return true;
    }

    return false;
}
