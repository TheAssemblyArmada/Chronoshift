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
//#include "callback.h"
#include "ccini.h"
#include "controlc.h"
#include "dialog.h"
#include "language.h"
#include "mouse.h"
//#include "rules.h"
#include "slider.h"
#include "stringex.h"
#include "textbtn.h"
#include "vector.h"

void Difficulty_Get(CCINIClass &ini, DifficultyClass &diff, const char *section)
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
    // TODO Requires RuleClass and CallBack
#if 0
    DynamicVectorClass<ControlClass *> DialogGadgets(3);
    char strbuff[512];
    bool process = true; // loop while true
    bool to_draw = true;

    strlcpy(strbuff, Fetch_String(TXT_SET_DIFFICULTY), sizeof(strbuff));

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

    TextButtonClass okbtn(
        1, TXT_OK, TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW , 470, 244, 60);
    //TextButtonClass cancelbtn(
    //    BUTTON_CANCEL, TXT_OK, TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW , 470, 244, 60);
    SliderClass diffsli(2, 110, 222, 420, 16, true);

    if (Rule.FineDiffControl) {
        diffsli.Set_Maximum(DIFF_COUNT);
        diffsli.Set_Value((diffsli.Get_Maximum() / 2) - 1); // set initial value.	//2
    } else {
        diffsli.Set_Maximum(3);
        diffsli.Set_Value((diffsli.Get_Maximum() / 2) - 1); // set initial value.
    }

    diffsli.Add(okbtn);
    //diffsli.Add(cancelbtn);

    // Button linking
    GadgetClass *active_gadget = &diffsli;

    // Store all the gadgets in an array.
    DialogGadgets.Add(&okbtn);
    //DialogGadgets.Add(&cancelbtn);
    DialogGadgets.Add(&diffsli);

    Set_Logic_Page(&g_seenBuff);

    while (process) {
        if (to_draw) {
            g_mouse->Hide_Mouse();

            Dialog_Box(200, 200, 500, 160);

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

            g_mouse->Show_Mouse();
        }

        Application_Callback();

        if (AllSurfaces.field_0) {
            AllSurfaces.field_0 = false;
            to_draw = true;
        } else {
            KeyNumType input = diffsli.Input();

            if (input == KN_RETURN || input == 0x80001) {
                process = false;
            }

            //if (input == KN_ESC || input == GADGET_INPUT_RENAME(BUTTON_CANCEL)) {
            //    process = false;
            //    return DIFF_NONE;
            //}
        } 
    }

    return (Rule.FineDiffControl == false) + 1) * diffsli.Get_Value();
#else
    return DIFF_EASIEST;
#endif
}
