/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Struct for handling differences between the difficulty levels.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "difficulty.h"
#include "callback.h"
#include "gameini.h"
#include "gbuffer.h"
#include "controlc.h"
#include "dialog.h"
#include "language.h"
#include "mouse.h"
#include "ostimer.h"
#include "rules.h"
#include "slider.h"
#include "surfacemonitor.h"
#include "textbtn.h"
#include "vector.h"

void Difficulty_Get(GameINIClass &ini, DifficultyClass &diff, const char *section)
{
    if (ini.Find_Section(section) != nullptr) {
        diff.Firepower = ini.Get_Fixed(section, "FirePower", "1.0");
        diff.Groundspeed = ini.Get_Fixed(section, "Groundspeed", "1.0");
        diff.Airspeed = ini.Get_Fixed(section, "Airspeed", "1.0");
        diff.Armor = ini.Get_Fixed(section, "Armor", "1.0");
        diff.ROF = ini.Get_Fixed(section, "ROF", "1.0");
        diff.Cost = ini.Get_Fixed(section, "Cost", "1.0");
        diff.RepairDelay = ini.Get_Fixed(section, "RepairDelay", "0.02");
        diff.BuildDelay = ini.Get_Fixed(section, "BuildDelay", "0.03");
        diff.BuildTime = ini.Get_Fixed(section, "BuildTime", "1.0");
        diff.BuildSlowdown = ini.Get_Bool(section, "BuildSlowdown", false);
        diff.DestroyWalls = ini.Get_Bool(section, "DestroyWalls", true);
        diff.ContentScan = ini.Get_Bool(section, "ContentScan", false);
    }
}

int Fetch_Difficulty_Dialog(BOOL one_time_mission)
{
    char strbuff[512];
    bool process = true; // loop while true
    bool to_draw = true;

    strlcpy(strbuff, Text_String(TXT_SET_DIFFICULTY), sizeof(strbuff));

    // trucante string at "." if arg is true (is this one off mission?)
    if (one_time_mission) {
        int index = 0;

        if (strbuff[0] != '\0') {
            for (index = 0; strbuff[index + 1] != '\0'; ++index) {
                if (strbuff[index] == '.') {
                    strbuff[index + 1] = '\0';
                    break;
                }
            }
        }
    }

    // empty call sets the font spacing stuff?
    Fancy_Text_Print(nullptr, 0, 0, nullptr, COLOR_TBLACK, TPF_6PT_GRAD | TPF_NOSHADOW);
    int str_w = 0;
    int str_h = 0;
    Format_Window_String(strbuff, 380, str_w, str_h);

    TextButtonClass okbtn(1, TXT_OK, TPF_CENTER | TPF_NOSHADOW | TPF_6PT_GRAD, 470, 244, 60);
    SliderClass diffsli(2, 110, 222, 420, 16, true);

    if (g_Rule.Fine_Diff_Control()) {
        diffsli.Set_Maximum(DIFF_COUNT);
        diffsli.Set_Value(2); // set initial value.    //2
    } else {
        diffsli.Set_Maximum(3);
        diffsli.Set_Value(1); // set initial value.
    }

    diffsli.Add(okbtn);

    // Button linking
    GadgetClass *active_gadget = &okbtn;
    Set_Logic_Page(&g_SeenBuff);

    while (process) {
        if (to_draw) {
            g_Mouse->Hide_Mouse();

            Dialog_Box(70, 120, 500, 160);
            to_draw = false;
            Fancy_Text_Print(
                strbuff, 110, 150, GadgetClass::Get_Color_Scheme(), COLOR_TBLACK, TPF_6PT_GRAD | TPF_NOSHADOW | TPF_LEFT);

            Fancy_Text_Print(TXT_HARD,
                diffsli.Get_Width() + diffsli.Get_XPos(),
                diffsli.Get_YPos() - 18,
                GadgetClass::Get_Color_Scheme(),
                COLOR_TBLACK,
                TPF_6PT_GRAD | TPF_SHADOW | TPF_RIGHT);
            Fancy_Text_Print(TXT_NORMAL,
                diffsli.Get_XPos() + diffsli.Get_Width() / 2,
                diffsli.Get_YPos() - 18,
                GadgetClass::Get_Color_Scheme(),
                COLOR_TBLACK,
                TPF_6PT_GRAD | TPF_SHADOW | TPF_CENTER);
            Fancy_Text_Print(TXT_EASY,
                diffsli.Get_XPos(),
                diffsli.Get_YPos() - 18,
                GadgetClass::Get_Color_Scheme(),
                COLOR_TBLACK,
                TPF_6PT_GRAD | TPF_SHADOW | TPF_LEFT);

            if (active_gadget != nullptr) {
                active_gadget->Draw_All(true);
            }

            g_Mouse->Show_Mouse();
        }

        Call_Back();

        if (g_AllSurfaces.Surfaces_Restored()) {
            g_AllSurfaces.Clear_Surfaces_Restored();
            to_draw = true;
        } else {
            KeyNumType input = active_gadget->Input();

            if (input == KN_RETURN || input == GADGET_BUTTON(1)) {
                process = false;
            }
        }

        // BUGFIX: Stops high CPU load in this menu.
        PlatformTimerClass::Sleep(1);
    }

    return ((g_Rule.Fine_Diff_Control() == false) + 1) * diffsli.Get_Value();
}
