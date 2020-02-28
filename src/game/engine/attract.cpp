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
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)() = reinterpret_cast<void (*)()>(0x004AB140);
    func();
#else
    if (Session.Record_Game()) {

        // When super record is enabled, a single record file can contain multiple games. so we
        // seek to the end of the file and continue writing onto the previous session.
        if (Session.Super_Record_Game()) {
            Session.Recording_File().Open(FM_READ_WRITE);
            Session.Recording_File().Seek(0, FS_SEEK_END);
        }

        // Write the current tactical position.
        coord_t new_pos = Map.New_Pos();
        Session.Recording_File().Write(&new_pos, sizeof(new_pos));

        // Write the total number of selected objects.
        int currobj_cnt = CurrentObjects.Count();
        Session.Recording_File().Write(&currobj_cnt, sizeof(currobj_cnt));

        // Write a total checksum of all the currently selected objects.
        target_t total_obj_target = 0;
        for (int index = 0; index < currobj_cnt; ++index) {
            total_obj_target += As_Target(CurrentObjects[index]);
        }
        Session.Recording_File().Write(&total_obj_target, sizeof(total_obj_target));

        // Write target lookup values for each of the selected objects.
        for (int index = 0; index < currobj_cnt; ++index) {
            target_t target = As_Target(CurrentObjects[index]);
            Session.Recording_File().Write(&target, sizeof(target));
        }

        Session.Recording_File().Write(&g_TeamEvent, 4);
        Session.Recording_File().Write(&g_TeamNumber, 4);
        Session.Recording_File().Write(&g_TeamMaxSpeed, 4);
        Session.Recording_File().Write(&g_TeamSpeed, 4);
        Session.Recording_File().Write(&g_FormationEvent, 4);
        Session.Recording_File().Write(&g_FormMove, 4);
        Session.Recording_File().Write(&g_FormSpeed, 4);
        Session.Recording_File().Write(&g_FormMaxSpeed, 4);

        g_TeamEvent = TEAM_EVENT_NONE;
        g_TeamNumber = TEAM_NUMBER_NONE;

        g_FormationEvent = FORMATION_EVENT_NONE;

        if (Session.Super_Record_Game()) {
            Session.Recording_File().Close();
        }
    }

    if (Session.Playback_Game()) {

        // Read and assign the new tactical position.
        coord_t tact_pos = 0;
        if (Session.Recording_File().Read(&tact_pos, sizeof(tact_pos)) == sizeof(tact_pos) && tact_pos != Map.NewPos) {
            Map.Set_Tactical_Position(tact_pos);
        }

        // Read and update the current selected objects.
        int32_t currobj_cnt = 0;
        if (Session.Recording_File().Read(&currobj_cnt, sizeof(currobj_cnt)) == sizeof(currobj_cnt)) {
            // Calculate the
            target_t calc_total_obj_target = 0;
            for (int index = 0; index < currobj_cnt; ++index) {
                calc_total_obj_target += As_Target(CurrentObjects[index]);
            }

            // Read the saved total target checksum.
            target_t total_obj_target_checksum = 0;
            Session.Recording_File().Read(&total_obj_target_checksum, 4);

            //
            if (total_obj_target_checksum != calc_total_obj_target) {
                Unselect_All();
            }

            g_AllowVoice = true;

            // Read out the select object targets and select them they match.
            target_t obj_target = 0;
            for (int index = 0; index < currobj_cnt; ++index) {
                if (Session.Recording_File().Read(&obj_target, sizeof(target_t)) == sizeof(target_t)) {
                    ObjectClass *obj = As_Object(obj_target);
                    if (obj != nullptr) {
                        if (total_obj_target_checksum != total_obj_target) {
                            obj->Select();
                            g_AllowVoice = false;
                        }
                    }
                }
            }

            g_AllowVoice = true;
        }

        Session.Recording_File().Read(&g_TeamEvent, 4); // Saves this as a DWORD.
        Session.Recording_File().Read(&g_TeamNumber, 4); // Saves this as a DWORD.
        Session.Recording_File().Read(&g_TeamMaxSpeed, 4); // Saves this as a DWORD.
        Session.Recording_File().Read(&g_TeamSpeed, 4); // Saves this as a DWORD.

        if (g_TeamEvent != TEAM_EVENT_0) {
            Handle_Team(g_TeamNumber, g_TeamEvent);
        }

        Session.Recording_File().Read(&g_FormationEvent, 4); // Saves this as a DWORD.

        if (g_FormationEvent != FORMATION_EVENT_0) {
            FormationManager.Toggle();
        }

        Session.Recording_File().Read(&g_FormMove, 4); // Saves this as a DWORD.
        Session.Recording_File().Read(&g_FormSpeed, 4); // Saves this as a DWORD.
        Session.Recording_File().Read(&g_FormMaxSpeed, 4); // Saves this as a DWORD.

        // Force a full render of the game viewport.
        Map.Render();
    }
#endif
}

BOOL Save_Recording_Values(FileClass &recfile)
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(FileClass &) = reinterpret_cast<BOOL (*)(FileClass &)>(0x004F8718);
    return func(recfile);
#else
    if (Session.Save(recfile)) {
        recfile.Write(&g_BuildLevel, sizeof(g_BuildLevel)); // Saves this as a DWORD.
        recfile.Write(&g_Debug_Unshroud, /*sizeof(g_Debug_Unshroud)*/ 4); // Saves this as a DWORD.
        // recfile.Write(&IPX.unk01, sizeof(IPX.unk01));
        recfile.Write(&g_RandomSeed, sizeof(g_RandomSeed)); // Saves this as a DWORD.
        recfile.Write(&Scen.ScenarioName, sizeof(Scen.ScenarioName));
        recfile.Write(&g_Whom, /*sizeof(g_Whom)*/ 4); // Saves this as a DWORD.
        recfile.Write(&Special, sizeof(Special));
        recfile.Write(&Options, sizeof(Options));

        DEBUG_LOG("Saved recording values for scenario : %s\n", Scen.Scenario_Name());

        if (Session.Super_Record_Game()) {
            Session.Recording_File().Close();
        }

        return true;
    }
    return false;
#endif
}

BOOL Load_Recording_Values(FileClass &recfile)
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(FileClass &) = reinterpret_cast<BOOL (*)(FileClass &)>(0x004F87C4);
    return func(recfile);
#else
    if (Session.Load(recfile)) {
        recfile.Read(&g_BuildLevel, sizeof(g_BuildLevel)); // Saves this as a DWORD.
        recfile.Read(&g_Debug_Unshroud, /*sizeof(g_Debug_Unshroud)*/ 4); // Saves this as a DWORD.
        // recfile.Read(&IPX.unk01, sizeof(IPX.unk01));
        recfile.Read(&g_RandomSeed, sizeof(g_RandomSeed)); // Saves this as a DWORD.
        recfile.Read(&Scen.ScenarioName, sizeof(Scen.ScenarioName));
        recfile.Read(&g_Whom, /*sizeof(g_Whom)*/ 4); // Saves this as a DWORD.
        recfile.Read(&Special, sizeof(Special));
        recfile.Read(&Options, sizeof(Options));

        DEBUG_LOG("Loaded recording values for scenario : %s\n", Scen.Scenario_Name());

        return true;
    }
    return false;
#endif
}
