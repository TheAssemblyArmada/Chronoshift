/**
 * @file
 *
 * @author tomsons26
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

#include "sndctrl.h"
#include "callback.h"
#include "gameloop.h"
#include "gameoptions.h"
#include "gbuffer.h"
#include "mouse.h"
#include "musiclist.h"
#include "session.h"
#include "surfacemonitor.h"
#include "textbtn.h"
#include "theme.h"
#include <cstdio>

using std::snprintf;

/**
 * @brief
 *
 */
void SoundControlsClass::Adjust_Variables_For_Resolution()
{
    // TODO, take code from Sole, no code in RA
}

/**
 * @brief
 *
 * @address 0x00550290
 */
void SoundControlsClass::Process()
{
    static int _tabs[] = { 110, 144, 180 };
    Adjust_Variables_For_Resolution();

    // music list itself
    MusicListClass musiclist(0, 62, 162, 516, 146, TPF_6PT_GRAD | TPF_NOSHADOW);

    // buttons
    TextButtonClass okbtn(BUTTON_SOUND_OK, TXT_OK, TPF_CENTER | TPF_NOSHADOW | TPF_6PT_GRAD, 438, 310, 140);
    ShapeButtonClass stopbtn(BUTTON_SOUND_STOP, GameFileClass::Retrieve("btn-st.shp"), 62, 310);
    ShapeButtonClass playbtn(BUTTON_SOUND_PLAY, GameFileClass::Retrieve("btn-pl.shp"), 98, 310);
    TextButtonClass sufflebtn(BUTTON_SOUND_SUFFLE, TXT_OFF, TPF_CENTER | TPF_NOSHADOW | TPF_6PT_GRAD, 222, 310, 50);
    TextButtonClass repeatbtn(BUTTON_SOUND_REPEAT, TXT_OFF, TPF_CENTER | TPF_NOSHADOW | TPF_6PT_GRAD, 356, 310, 50);

    // volume sliders
    SliderClass scorevolsldr(SLIDER_SCORE_VOL, 322, 110, 216, 10, true);
    SliderClass soundvolsldr(SLIDER_SOUND_VOL, 322, 134, 216, 10, true);

    // dimensions are the same as the dialog itself, this is for capturing input?
    GadgetClass gadget(28, 54, 584, 292, MOUSE_LEFT_PRESS);

    // not sure what's this for and why it uses OK button id...
    ControlClass ctrl(
        BUTTON_SOUND_OK, 0, 0, g_SeenBuff.Get_Width(), g_SeenBuff.Get_Height(), MOUSE_LEFT_PRESS | MOUSE_RIGHT_PRESS);

    // set shuffle state
    sufflebtn.Set_Toggle_Disabled(true);
    if (g_Options.Get_Shuffle()) {
        sufflebtn.Turn_On();
    } else {
        sufflebtn.Turn_Off();
    }
    sufflebtn.Set_Text(sufflebtn.Get_Toggle_State() ? TXT_ON : TXT_OFF, false);

    // set repeat state
    repeatbtn.Set_Toggle_Disabled(true);
    if (g_Options.Get_Repeat()) {
        repeatbtn.Turn_On();
    } else {
        repeatbtn.Turn_Off();
    }
    repeatbtn.Set_Text(repeatbtn.Get_Toggle_State() ? TXT_ON : TXT_OFF, false);

    // set slider states
    scorevolsldr.Set_Maximum(255);
    scorevolsldr.Set_Thumb_Size(16);
    scorevolsldr.Set_Value(g_Options.Get_Score_Volume() * 256);
    soundvolsldr.Set_Maximum(255);
    soundvolsldr.Set_Thumb_Size(16);
    soundvolsldr.Set_Value(g_Options.Get_Sound_Volume() * 256);

    // set graphic page to draw to
    Set_Logic_Page(g_SeenBuff);

    // link all the gadgets
    GadgetClass *activegdt = &okbtn;
    musiclist.Add_Tail(*activegdt);
    stopbtn.Add_Tail(*activegdt);
    playbtn.Add_Tail(*activegdt);
    sufflebtn.Add_Tail(*activegdt);
    repeatbtn.Add_Tail(*activegdt);
    scorevolsldr.Add_Tail(*activegdt);
    soundvolsldr.Add_Tail(*activegdt);
    gadget.Add_Tail(*activegdt);
    ctrl.Add_Tail(*activegdt);

    // fill up the list entries
    for (ThemeType i = THEME_FIRST; i < g_Theme.Max_Themes(); ++i) {
        if (g_Theme.Is_Allowed(i)) {
            int length = g_Theme.Track_Length(i);
            // these strings are used while the list menu exists and are cleaned up at the end of the function
            char *str = new char[100];
            if (str != nullptr) {
                // TODO, this is how RA had it
                // this needs a better solution, packing the theme number into the string is quite the hack
                snprintf(str,
                    100,
                    "%cTrack %d\t%d:%02d\t%s",
                    i,
                    musiclist.Count() + 1,
                    length / 60,
                    length % 60,
                    g_Theme.Full_Name(i));
                musiclist.Add_Item(str);
            }
            if (g_Theme.What_Is_Playing() == i) {
                musiclist.Set_Selected_Index(musiclist.Count() - 1);
            }
        }
    }
    // set seperator tab locations
    musiclist.Set_Tabs(_tabs);

    BOOL process = true;
    BOOL to_draw = true;
    RemapControlType *scheme = GadgetClass::Get_Color_Scheme();
    while (process) {
        if (g_Session.Game_To_Play() == GAME_CAMPAIGN || g_Session.Game_To_Play() == GAME_SKIRMISH) {
            Call_Back();
        } else if (Main_Loop()) {
            process = false;
        }

        if (g_AllSurfaces.Surfaces_Restored()) {
            g_AllSurfaces.Clear_Surfaces_Restored();
            to_draw = true;
        }

        if (to_draw) {
            g_Mouse->Hide_Mouse();
            Dialog_Box(28, 54, 584, 292);
            Draw_Caption(42, 28, 54, 584);
            Fancy_Text_Print(TXT_MUSIC_VOLUME_COLON, 312, 106, scheme, 0, TPF_RIGHT | TPF_NOSHADOW | TPF_6PT_GRAD);
            Fancy_Text_Print(TXT_SOUND_VOLUME_COLON, 312, 130, scheme, 0, TPF_RIGHT | TPF_NOSHADOW | TPF_6PT_GRAD);
            Fancy_Text_Print(TXT_SUFFLE, 212, 312, scheme, 0, TPF_RIGHT | TPF_NOSHADOW | TPF_6PT_GRAD);
            Fancy_Text_Print(TXT_REPEAT, 346, 312, scheme, 0, TPF_RIGHT | TPF_NOSHADOW | TPF_6PT_GRAD);

            // Draw all linked gadgets
            activegdt->Draw_All();
            g_Mouse->Show_Mouse();
            to_draw = false;
        }

        // handle keyboard and button input
        KeyNumType input = activegdt->Input();
        if (input != KN_NONE) {
            switch (input) {
                case KN_ESC:
                    process = false;
                    break;
                case GADGET_INPUT_RENAME2(BUTTON_SOUND_STOP):
                    // are both needed?
                    g_Theme.Stop();
                    g_Theme.Stop_Playing_Song();
                    break;
                case KN_SPACE:
                case GADGET_INPUT_RENAME2(BUTTON_SOUND_PLAY):
                    // TODO, this is how RA had it
                    // This is a hack - first char of the string is the theme number
                    // Needs a better solution
                    g_Theme.Queue_Song((ThemeType)musiclist.Current_Item()[0]);
                    break;
                case GADGET_INPUT_RENAME2(BUTTON_SOUND_SUFFLE):
                    sufflebtn.Set_Text(sufflebtn.Get_Toggle_State() ? TXT_ON : TXT_OFF, false);
                    g_Options.Set_Shuffle(sufflebtn.Get_Toggle_State());
                    break;
                case GADGET_INPUT_RENAME2(BUTTON_SOUND_REPEAT):
                    repeatbtn.Set_Text(repeatbtn.Get_Toggle_State() ? TXT_ON : TXT_OFF, false);
                    g_Options.Set_Repeat(repeatbtn.Get_Toggle_State());
                    break;
                case GADGET_INPUT_RENAME2(BUTTON_SOUND_OK):
                    process = false;
                    break;
                case GADGET_INPUT_RENAME2(SLIDER_SCORE_VOL):
                    g_Options.Set_Score_Volume(fixed_t(scorevolsldr.Get_Value(), 256), true);
                    if (g_Session.Game_To_Play() != GAME_CAMPAIGN) {
                        g_Options.Set_MPlayer_Score_Volume(g_Options.Get_Score_Volume());
                    }
                    break;
                case GADGET_INPUT_RENAME2(SLIDER_SOUND_VOL):
                    g_Options.Set_Sound_Volume(fixed_t(soundvolsldr.Get_Value(), 256), true);
                    break;
                default:
                    break;
            }
        }
    }
    if (g_Options.Get_Score_Volume() == 0) {
        g_Theme.Stop();
    }
    // cleanup
    while (musiclist.Count()) {
        const char *item = musiclist.Get_Item(0);
        musiclist.Remove_Item(item);
        delete item;
    }
}
