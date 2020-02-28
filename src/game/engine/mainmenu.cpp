/**
 * @file
 *
 * @author CCHyper
 *
 * @brief Main menu.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "mainmenu.h"
#include "attract.h"
#include "callback.h"
#include "cd.h"
#include "dialog.h"
#include "expansion.h"
#include "gadget.h"
#include "gameevent.h"
#include "gameoptions.h"
#include "gbuffer.h"
#include "globals.h"
#include "init.h" // for Init_Random().
#include "iomap.h"
#include "keyboard.h"
#include "mouse.h"
#include "movie.h"
#include "msgbox.h"
#include "palette.h"
#include "pcx.h"
#include "queue.h"
#include "egos.h"
#include "rndstraw.h"
#include "scenario.h"
#include "session.h"
#include "special.h"
#include "surfacemonitor.h"
#include "textbtn.h"
#include "theme.h"
#include "trect.h"
#include "ttimer.h"
#include "txtlabel.h"
#include "version.h"
#include <captainslog.h>

#define LOGO_BTN_XPOS 18
#define LOGO_BTN_YPOS 20
#define LOGO_BTN_WIDTH 140 // 158
#define LOGO_BTN_HEIGHT 28

#define ANTS_BTN_XPOS 512
#define ANTS_BTN_YPOS 6
#define ANTS_BTN_WIDTH 102
#define ANTS_BTN_HEIGHT 102

int TitlePageXPos = -1;
int TitlePageYPos = -1;
int TitlePageWidth = -1;
int TitlePageHeight = -1;

// TODO: move the following functions.

void Choose_Side()
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(void) = reinterpret_cast<void (*)(void)>(0x004F8AFC);
    func();
#endif
}

GameEnum Select_MPlayer_Game()
{
#ifndef CHRONOSHIFT_STANDALONE
    GameEnum (*func)(void) = reinterpret_cast<GameEnum (*)(void)>(0x00503464);
    return func();
#else
    return GAME_NONE;
#endif
}

BOOL Com_Scenario_Dialog(BOOL a1)
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(void) = reinterpret_cast<BOOL (*)(void)>(0x00503464);
    return func();
#else
    return false;
#endif
}

BOOL Is_Mission_Counterstrike(const char *scen_name)
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(const char *) = reinterpret_cast<BOOL (*)(const char *)>(0x0054B790);
    return func(scen_name);
#else
    return false;
#endif
}

BOOL Expansion_Dialog(ExpansionType type)
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(ExpansionType) = reinterpret_cast<BOOL (*)(ExpansionType)>(0x004BE360);
    return func(type);
#else
    return false;
#endif
}

/**
 * @brief
 *
 * 0x00501568
 */
BOOL Coordinates_In_Region(int x_pos, int y_pos, int left, int top, int right, int bottom)
{
    return x_pos >= left && x_pos <= right && y_pos >= top && y_pos <= bottom;
}

/**
 * Loads a PCX file data into the provided viewport and palette.
 *
 * 0x005B3CD8
 */
void Load_Title_Screen(const char *filename, GraphicViewPortClass *vp, PaletteClass *pal)
{
    captainslog_debug("Load_Title_Screen(%s).\n", filename);

    GraphicBufferClass *buff = Read_PCX_File(filename, pal);

    if (buff == nullptr) {
        return;
    }

    buff->Blit(*vp);
    delete buff;
}

/**
 * Loads and displays the prolog page.
 *
 * 0x004F3E98
 */
void Load_Prolog_Page()
{
    g_Mouse->Hide_Mouse();

    Load_Title_Screen("prolog.pcx", &g_HidPage, &g_CCPalette);

    g_HidPage.Blit(g_SeenBuff);

    g_CCPalette.Set(15);
    g_Mouse->Show_Mouse();
}

void Load_Title_Page()
{
    g_Mouse->Hide_Mouse();

    Load_Title_Screen("title.pcx", &g_HidPage, &g_CCPalette);

    g_HidPage.Blit(g_SeenBuff);

    g_Mouse->Show_Mouse();
}

void Load_Standby_Page()
{
    Set_Logic_Page(g_LogicPage);

    g_Mouse->Hide_Mouse();

    Load_Title_Screen("title.pcx", &g_HidPage, &g_CCPalette);

    Fancy_Text_Print(TXT_PLEASE_STANDBY,
        TitlePageXPos + (TitlePageWidth / 2),
        TitlePageYPos + (TitlePageHeight / 2),
        &g_ColorRemaps[REMAP_10],
        COLOR_TBLACK,
        TPF_6PT_GRAD | TPF_SHADOW | TPF_CENTER);

    g_HidPage.Blit(g_SeenBuff);

    g_Mouse->Show_Mouse();
}

BOOL Select_Game(BOOL fade)
{
    BOOL process = true; // Flag for process loop.
    BOOL to_draw = true; // Flag to redraw gadgets.
    BOOL loaded_game = false; // Loaded a game from the Load Game menu?

    g_GameActive = true;
    g_GameFrame = 0;

    g_OutgoingEvents.Clear();
    g_ScheduledEvents.Clear();

    g_NewMaxAheadFrame1 = 0;
    g_NewMaxAheadFrame2 = 0;

    g_PlayerWins = false;
    g_PlayerLoses = false;
    g_PlayerAborts = false;

    g_SecretUnitsEnabled = false;
    g_NewUnitsEnabled = false;

    g_Debug_Unshroud = false;

    g_Session.Reset_Processing_Values();
    g_Session.Set_Desired_Frame_Rate(0);

    g_Session.Set_MPlayer_Obi_Wan(false);

    g_Scen.Global_Timer().Reset();
    g_Scen.Global_Timer().Stop();

    g_Scen.Set_Difficulties(DIFF_NORMAL, DIFF_NORMAL);

    g_Map.Reset_Pending_Object();
    g_Map.Set_Cursor_Shape(nullptr);
    g_Map.Set_Default_Mouse(MOUSE_POINTER);

    for (HousesType house = HousesType(0); house < HOUSES_MULTI_COUNT; ++house) {
        g_Session.MPlayer_Score_Info(house).m_field_10[g_Session.MPlayer_Current_Game()] = -1;
    }

    // Load recording values if we are in the playback mode.
    if (g_Session.Playback_Game()) {
        if (g_Session.Recording_File().Open(FM_READ)) {
            Load_Recording_Values(g_Session.Recording_File());
        } else {
            // We failed to open the recording file, reset the playback flag.
            g_Session.Set_Playback_Game(false);
        }
    }

    // By default, the attraction should not kick in if we can
    // not find the recording file, so set the default delay to zero.
    int attraction_delay = 0;

    if (g_Session.Attraction_Allowed() && g_Session.Recording_File().Is_Available()) {
        attraction_delay = DEFAULT_ATTRACTION_DELAY;
    }

    MenuButtonType menu_selection = g_Session.Game_To_Play() != GAME_CAMPAIGN ? MENU_MPLAYER_GAME : MENU_RESET;

    g_Theme.Queue_Song(THEME_CRUSH);

    // Ensure the mouse is shown before we draw the menu.
    g_Mouse->Show_Mouse();

    while (process) {
        // Draw the menu title page without any buttons.
        if (to_draw) {
            Load_Title_Page();

            g_Mouse->Hide_Mouse();

            g_GamePalette = g_CCPalette;
            g_CCPalette.Set();

            g_HidPage.Blit(g_SeenBuff);

            Set_Logic_Page(g_SeenBuff);

            g_Mouse->Show_Mouse();

            to_draw = false;
        }

        // If the menu selection has been reset (exit from a child dialog), re-init the main menu.
        if (menu_selection == MENU_RESET) {
            g_AntsEnabled = false;
            menu_selection = Main_Menu(attraction_delay);
        }

        Call_Back();

        switch (menu_selection) {
            // Continue the process loop as no button was clicked.
            default:
                continue;

            // When the menu is passed an attraction delay and the timer finishes, it will
            // will return NONE, so we handle playback of the recording file here.
            case MENU_NONE:
                if (g_Session.Attraction_Allowed()) {
                    if (g_Session.Recording_File().Is_Available()) {
                        g_Session.Set_Playback_Game(true);
                        if (g_Session.Recording_File().Open(FM_READ)) {
                            Load_Recording_Values(g_Session.Recording_File());
                            g_Theme.Fade_Out();
                            process = false;
                            continue;
                        }
                        g_Session.Set_Playback_Game(false);
                    }
                }
                menu_selection = MENU_RESET;
                continue;

            // We handle both expansions in this case.
            case MENU_COUNTERSTRIKE:
            case MENU_AFTERMATH:

                // If both expansions are not installed, this menu option is not accessable.
                if (!Is_Counterstrike_Installed() && !Is_Aftermath_Installed()) {
                    continue;
                }

                // TODO reset CarryoverClass?
                g_Scen.Set_Carry_Over_Money(fixed_t(0, 1));

                g_IsTanyaDead = false;
                g_SaveTanya = false;

                switch (menu_selection) {
                    default:
                        continue;
                    case MENU_COUNTERSTRIKE:
                        if (Force_CD_Available(DISK_COUNTERSTRIKE)) {
                            if (Expansion_Dialog(EXPANSION_COUNTERSTRIKE)) {
                                break;
                            }
                        }
                        menu_selection = MENU_RESET;
                        continue;
                    case MENU_AFTERMATH:
                        if (Force_CD_Available(DISK_AFTERMATH)) {
                            if (Expansion_Dialog(EXPANSION_AFTERMATH)) {
                                break;
                            }
                        }
                        menu_selection = MENU_RESET;
                        continue;
                };

                switch (Fetch_Difficulty_Dialog(true)) {
                    case DIFF_EASIEST:
                        g_Scen.Set_Difficulties(DIFF_NORMAL, DIFF_EASIEST);
                        break;
                    case DIFF_EASIER:
                        g_Scen.Set_Difficulties(DIFF_NORMAL, DIFF_EASIER);
                        break;
                    case DIFF_NORMAL:
                        g_Scen.Set_Difficulties(DIFF_EASIER, DIFF_EASIER);
                        break;
                    case DIFF_HARDER:
                        g_Scen.Set_Difficulties(DIFF_EASIEST, DIFF_EASIER);
                        break;
                    case DIFF_HARDEST:
                        g_Scen.Set_Difficulties(DIFF_EASIEST, DIFF_NORMAL);
                        break;
                    case DIFF_NONE: // DIFF_NONE is equal to "cancel" here.
                    default:
                        continue;
                };

                g_Theme.Stop_Playing_Song();
                g_Theme.Queue_Song(THEME_BIG_FOOT);

                g_Session.Set_Game_To_Play(GAME_CAMPAIGN);

                process = false;

                continue;

            case MENU_NEW_GAME:

                // TODO: reset CarryoverClass?
                g_Scen.Set_Carry_Over_Money(fixed_t(0, 1));

                g_IsTanyaDead = false;
                g_SaveTanya = false;

                g_BuildLevel = 10; // TODO: move to scenario init clear?

                g_Whom = HOUSES_GOODGUY;

                if (s_Special.Is_First_Run()) {
                    g_Scen.Set_Difficulties(DIFF_EASIER, DIFF_EASIER);

                    g_Theme.Stop_Playing_Song();

                    Choose_Side();

                    g_Mouse->Hide_Mouse();

                    switch (g_CurrentCD) {
                        case DISK_ALLIED:
                            g_Scen.Set_Scenario_Name("SCG01EA.INI");
                            break;
                        default: // There isn't any other disks, fallback to Soviet.
                        case DISK_SOVIET:
                            g_Scen.Set_Scenario_Name("SCU01EA.INI");
                            break;
                    };

                    g_Theme.Stop_Playing_Song();

                    Load_Title_Page();

                    g_Session.Set_Game_To_Play(GAME_CAMPAIGN);

                    process = false;

                } else {
                    switch (Fetch_Difficulty_Dialog()) {
                        case DIFF_EASIEST:
                            g_Scen.Set_Difficulties(DIFF_NORMAL, DIFF_EASIEST);
                            break;
                        case DIFF_EASIER:
                            g_Scen.Set_Difficulties(DIFF_NORMAL, DIFF_EASIER);
                            break;
                        case DIFF_NORMAL:
                            g_Scen.Set_Difficulties(DIFF_EASIER, DIFF_EASIER);
                            break;
                        case DIFF_HARDER:
                            g_Scen.Set_Difficulties(DIFF_EASIEST, DIFF_EASIER);
                            break;
                        case DIFF_HARDEST:
                            g_Scen.Set_Difficulties(DIFF_EASIEST, DIFF_NORMAL);
                            break;
                        case DIFF_NONE: // DIFF_NONE is equal to "cancel"
                        default:
                            continue;
                    };

                    // Set the first special ants mission if we have been flagged as starting this campaign.
                    if (g_AntsEnabled) {
                        g_Scen.Set_Scenario_Name("SCA01EA.INI");

                    } else {
                        MessageBoxClass msgbox;
                        int msgretval = msgbox.Process(TXT_CHOOSE_SIDE, TXT_ALLIES, TXT_CANCEL, TXT_SOVIET);

                        switch (msgretval) {
                            case ID_BUTTON_1: // Allies
                                g_Scen.Set_Scenario_Name("SCG01EA.INI");
                                break;

                            case ID_BUTTON_3: // Soviet
                                g_Scen.Set_Scenario_Name("SCU01EA.INI");
                                break;

                            case ID_BUTTON_2: // Cancel
                            default: // Jump back to the main menu.
                                menu_selection = MENU_RESET;
                                continue;
                        };
                    }

                    g_Theme.Stop_Playing_Song();

                    Load_Title_Page();

                    g_Session.Set_Game_To_Play(GAME_CAMPAIGN);

                    process = false;
                }

                continue;

            case MENU_LOAD_GAME:
                /*Loadg_OptionsClass loadopt;
                if (loadopt.Process()) {
                    loaded_game = true;
                    process = false;
                } else {
                    menu_selection = MENU_RESET;
                    to_draw = true;
                }*/
                menu_selection = MENU_RESET; // TEMP: Until Loadg_OptionsClass is complete.
                continue;

            case MENU_MPLAYER_GAME:
                /*if ( g_Wolapi != nullptr ) {
                    g_Session.Set_Game_To_Play(GAME_INTERNET);
                }*/

                switch (Select_MPlayer_Game()) {
                    case GAME_IPX:
                        captainslog_debug("Game type is IPX.\n");

                    case GAME_SKIRMISH:
                        captainslog_debug("Game type is SKIRMISH.\n");
                        if (Com_Scenario_Dialog(true)) {
                            g_AftermathMultiplayer =
                                Is_Aftermath_Installed() && !Is_Mission_Counterstrike(g_Scen.Scenario_Name());
                            // g_Session.IsOfficial = g_Session.MPlayerScenarios[g_Session.MPlayerLocalID].IsOfficial;
                        } else {
                            GameEnum game = Select_MPlayer_Game();
                            if (g_Session.Game_To_Play() != GAME_CAMPAIGN) {
                                menu_selection = MENU_RESET;
                                to_draw = true;
                            }
                        }

                    default: // Jump back to the main menu.
                        menu_selection = MENU_RESET;
                        continue;
                }

                /*if ( g_Wolapi != nullptr ) {
                    g_Session.Set_Game_To_Play(GAME_INTERNET);
                }*/
                continue;

            case MENU_MOVIES:
            //case MENU_SNEAK_PEAK:
                g_Theme.Stop_Playing_Song();
                g_Theme.Fade_Out();

                if (g_Debug_Flag) {
                    Play_Intro(true);
                } else {
                    // Movies_Dialog();
                    Play_SneakPeak();
                }

                continue;

            case MENU_MAP_EDITOR:
                // TODO: The following is test code. The game gets forced into
                //      a campaign scenario with map editor global to true.

                g_InMapEditor = true;

                g_Scen.Set_Scenario_Name("SCG01EA.INI");
                g_Session.Set_Game_To_Play(GAME_NONE);

                process = false;
                continue;

            case MENU_OPTIONS:
                continue;

            case MENU_EXIT:
                if (g_ConfirmExit) {
                    if (MessageBoxClass::Simple_YesNo_Message_Box(TXT_CONFIRM_QUIT) == ID_BUTTON_1) { // ID_BUTTON_1 is No.
                        menu_selection = MENU_RESET; // Jump back to the main menu.
                        continue;
                    }
                }
                g_Theme.Stop_Playing_Song();
                g_BlackPalette.Set(15, Call_Back);
                return false;
        };

    } // while (process)

    switch (g_Session.Game_To_Play()) {
        default:
            break;

        case GAME_INTERNET:
            /*if ( Wolapi == nullptr ) {
                captainslog_debug("g_Wolapi is NULL on internet game!");
            }*/

            if (!g_AftermathMultiplayer) {
                g_SecretUnitsEnabled = false;
                g_NewUnitsEnabled = false;
            }
            break;

        case GAME_1:
        case GAME_2:
        case GAME_IPX:
            if (!g_AftermathMultiplayer) {
                g_SecretUnitsEnabled = false;
                g_NewUnitsEnabled = false;
            }
            break;

        case GAME_6:
        case GAME_7:
        case GAME_CAMPAIGN:
        case GAME_SKIRMISH:
            break;
    }

    if (loaded_game || g_Session.Loading_Game()) {
        if (g_Session.Game_To_Play() != GAME_CAMPAIGN && g_Session.Game_To_Play() != GAME_SKIRMISH
            && g_Session.Game_To_Play() != GAME_6 && g_Session.Game_To_Play() != GAME_7 && !g_Session.Playback_Game()) {
            g_Session.Create_Connections();
        }

        // Reset the unit build penalty to the default value.
        if (g_Session.Game_To_Play() != GAME_INTERNET) {
            g_UnitBuildPenalty = 100;
        }

        return true;
    }

    g_Keyboard->Clear();

    Init_Random();

    // Save recording values if we are in the recording mode.
    if (g_Session.Record_Game()) {
        if (g_Session.Recording_File().Open(FM_WRITE)) {
            Save_Recording_Values(g_Session.Recording_File());
        } else {
            // We failed to open the recording file, reset the recording flag.
            g_Session.Set_Record_Game(false);
        }
    }

    g_Mouse->Show_Mouse();

    captainslog_debug("Attempting to start scenario.\n");

    bool success = Start_Scenario(g_Scen.Scenario_Name(), true);

    if (success) {
        captainslog_debug("Scenario started OK.\n");

        captainslog_debug("Initialising message system.\n");
        g_Session.Get_Messages().Init(g_Map.Tac_Offset_X(),
            g_Map.Tac_Offset_Y(),
            6,
            106,
            14,
            -1,
            -1,
            false,
            20,
            106,
            Lepton_To_Pixel(g_Map.Get_DisplayWidth()));

        if (!g_Session.Playback_Game()) {
            if (g_Session.Game_To_Play() != GAME_CAMPAIGN && g_Session.Game_To_Play() != GAME_SKIRMISH
                && g_Session.Game_To_Play() != GAME_6 && g_Session.Game_To_Play() != GAME_7) {
                g_Session.Create_Connections();
            }
        }

        if (g_Session.Game_To_Play() != GAME_INTERNET) {
            g_UnitBuildPenalty = 100;
        }

        Call_Back();

        g_Mouse->Hide_Mouse();

        g_BlackPalette.Set(15, Call_Back);

        // Clear buffer pages.
        g_VisiblePage.Clear();
        g_HidPage.Clear();

        g_Mouse->Show_Mouse();

        Set_Logic_Page(g_SeenBuff);

        // Activate the sidebar.
        g_Map.Activate(1);

        g_Map.Flag_To_Redraw();

        Call_Back();

        g_Map.Render();

        // TODO: Needs confirming, and requires IPXAddressClass.
        /*char buff1[4];
        char buff2[8];
        for ( int i = 0; i < g_Session.Players_List().Count(); ++i ) {
            g_Session.Players_List()[i].Get_Address(&buff1, &buff2);
        }*/

        return true;
    }

    return false;
}

MenuButtonType Main_Menu(int attraction_delay)
{
    GadgetClass *_menu_gadgets[MENU_COUNT];

    MenuButtonType _selection = MENU_NONE;
    MenuButtonType _topmost_button = MENU_NONE;
    MenuButtonType _current_selection = MENU_NONE;

    BOOL process = true; // Flag for process loop.
    BOOL to_draw = true; // Flag to redraw gadgets.

    // Fetch expansion availablity for CS and AM.
    BOOL cs_present = Is_Counterstrike_Installed();
    BOOL am_present = Is_Aftermath_Installed();

    // Set the title page rect.
    TitlePageXPos = g_VisiblePage.Get_XPos();
    TitlePageYPos = g_VisiblePage.Get_YPos();
    TitlePageWidth = 640;
    TitlePageHeight = 400;

    // Set the defualt values for all buttons.
    int btn_width = 236;
    int btn_height = 18;
    int btn_spacing = 2;
    int btn_xpos = TitlePageXPos + (TitlePageWidth / 2) - (btn_width / 2);
    int btn_top_ypos = TitlePageYPos + 168;

    // Calculate positions for half size buttons.
    int btn_half_horz_space = 2;
    int btn_half_width = (btn_width / 2) - btn_half_horz_space;
    int btn_half_left_xpos = btn_xpos;
    int btn_half_right_xpos = btn_xpos + (btn_width - btn_half_width);

    // Calculate text position for version string.
    int txt_x_offset = 180;
    int txt_y_offset = 40;
    int txt_left_xpos = TitlePageXPos + txt_x_offset;
    int txt_center_xpos = TitlePageXPos + (TitlePageWidth / 2);
    int txt_right_xpos = TitlePageXPos + TitlePageWidth - txt_x_offset;
    int txt_web_ypos = TitlePageYPos + TitlePageHeight - 41;
    int txt_ver_ypos = TitlePageYPos + TitlePageHeight - 58;

    // Calculate button spacing depending on what expansions are enabled.
    if (am_present && cs_present) {
        btn_spacing += 1;
    } else if (cs_present || am_present) {
        btn_spacing += 3;
        btn_top_ypos -= (btn_height + btn_spacing);
    } else {
        btn_spacing += 4;
        btn_top_ypos -= ((btn_height + btn_spacing) * 2);
    }

    TextButtonClass csbtn(
        BUTTON_CS, TXT_CS_MISSIONS, TPF_6PT_GRAD | TPF_CENTER | TPF_NOSHADOW, btn_xpos, btn_top_ypos, btn_width, btn_height);

    TextButtonClass ambtn(BUTTON_AM,
        TXT_AM_MISSIONS,
        TPF_6PT_GRAD | TPF_CENTER | TPF_NOSHADOW,
        btn_xpos,
        (csbtn.Get_YPos() + btn_height) + btn_spacing,
        btn_width,
        btn_height);

    TextButtonClass newgamebtn(BUTTON_NEW_GAME,
        TXT_START_NEW_GAME,
        TPF_6PT_GRAD | TPF_CENTER | TPF_NOSHADOW,
        btn_xpos,
        (ambtn.Get_YPos() + btn_height) + btn_spacing,
        btn_width,
        btn_height);

    TextButtonClass loadgamebtn(BUTTON_LOAD_GAME,
        TXT_LOAD_MISSION,
        TPF_6PT_GRAD | TPF_CENTER | TPF_NOSHADOW,
        btn_xpos,
        (newgamebtn.Get_YPos() + btn_height) + btn_spacing,
        btn_width,
        btn_height);

    TextButtonClass mpgamebtn(BUTTON_MPLAYER_GAME,
        TXT_MULTIPLAYER_GAME,
        TPF_6PT_GRAD | TPF_CENTER | TPF_NOSHADOW,
        btn_xpos,
        (loadgamebtn.Get_YPos() + btn_height) + btn_spacing,
        btn_width,
        btn_height);

    /*TextButtonClass sneakpeekbtn(BUTTON_SNEAK_PEAK,
        TXT_INTRO,
        TPF_6PT_GRAD | TPF_CENTER | TPF_NOSHADOW,
        btn_xpos,
        (mpgamebtn.Get_YPos() + btn_height) + btn_spacing,
        btn_width,
        btn_height);*/

    TextButtonClass movbtn(BUTTON_MOVIES,
        TXT_MOVIES,
        TPF_6PT_GRAD | TPF_CENTER | TPF_NOSHADOW,
        btn_half_left_xpos,
        (mpgamebtn.Get_YPos() + btn_height) + btn_spacing,
        btn_half_width,
        btn_height);

    TextButtonClass editorbtn(BUTTON_MAP_EDITOR,
        TXT_MAP_EDITOR,
        TPF_6PT_GRAD | TPF_CENTER | TPF_NOSHADOW,
        btn_half_right_xpos,
        (mpgamebtn.Get_YPos() + btn_height) + btn_spacing,
        btn_half_width,
        btn_height);

    TextButtonClass optbtn(BUTTON_OPTIONS,
        TXT_OPTIONS,
        TPF_6PT_GRAD | TPF_CENTER | TPF_NOSHADOW,
        btn_xpos,
        (editorbtn.Get_YPos() + btn_height) + btn_spacing,
        btn_width,
        btn_height);

    TextButtonClass exitbtn(BUTTON_EXIT,
        TXT_EXIT_GAME,
        TPF_6PT_GRAD | TPF_CENTER | TPF_NOSHADOW,
        btn_xpos,
        (optbtn.Get_YPos() + btn_height) + btn_spacing,
        btn_width,
        btn_height);

    /*TextLabelClass build_label(const_cast<char *>(g_Version.Version_Name()),
        txt_right_xpos,
        txt_ver_ypos,
        GadgetClass::Get_Color_Scheme(),
        TPF_EDITOR | TPF_NOSHADOW | TPF_RIGHT);*/

    TextLabelClass version_label(const_cast<char *>(g_Version.Version_String()),
        txt_left_xpos,
        txt_ver_ypos,
        GadgetClass::Get_Color_Scheme(),
        TPF_EDITOR | TPF_NOSHADOW | TPF_LEFT);

    // Button linking.
    TextButtonClass *activebtn = &newgamebtn;

    if (cs_present) {
        csbtn.Add_Tail(*activebtn);
    }

    if (am_present) {
        ambtn.Add_Tail(*activebtn);
    }

    loadgamebtn.Add_Tail(*activebtn);
    mpgamebtn.Add_Tail(*activebtn);
    // sneakpeekbtn.Add_Tail(*activebtn);
    movbtn.Add_Tail(*activebtn);
    editorbtn.Add_Tail(*activebtn);
    optbtn.Add_Tail(*activebtn);
    exitbtn.Add_Tail(*activebtn);

    // Text label linking.
    // build_label.Add_Tail(*activebtn);
    version_label.Add_Tail(*activebtn);

    // Store all the text buttons in the array, this is used for keyboard navigation.
    _menu_gadgets[MENU_COUNTERSTRIKE] = &csbtn;
    _menu_gadgets[MENU_AFTERMATH] = &ambtn;
    _menu_gadgets[MENU_NEW_GAME] = &newgamebtn;
    _menu_gadgets[MENU_LOAD_GAME] = &loadgamebtn;
    _menu_gadgets[MENU_MPLAYER_GAME] = &mpgamebtn;
    //_menu_gadgets[MENU_SNEAK_PEAK] = &sneakpeekbtn;
    _menu_gadgets[MENU_MOVIES] = &movbtn;
    _menu_gadgets[MENU_MAP_EDITOR] = &editorbtn;
    _menu_gadgets[MENU_OPTIONS] = &optbtn;
    _menu_gadgets[MENU_EXIT] = &exitbtn;

    // Turn on (select/highlight/enable) the top most button.
    if (cs_present) {
        reinterpret_cast<TextButtonClass *>(_menu_gadgets[MENU_COUNTERSTRIKE])->Turn_On();
        _topmost_button = MENU_COUNTERSTRIKE;
        _current_selection = MENU_COUNTERSTRIKE;
    } else if (am_present) {
        reinterpret_cast<TextButtonClass *>(_menu_gadgets[MENU_AFTERMATH])->Turn_On();
        _topmost_button = MENU_AFTERMATH;
        _current_selection = MENU_AFTERMATH;
    } else {
        reinterpret_cast<TextButtonClass *>(_menu_gadgets[MENU_NEW_GAME])->Turn_On();
        _topmost_button = MENU_NEW_GAME;
        _current_selection = MENU_NEW_GAME;
    }

    if (g_RequiredCD != DISK_ANY) {
        g_RequiredCD = DISK_CDCHECK;
        Force_CD_Available(DISK_CDCHECK);
    }

    Set_Logic_Page(g_SeenBuff);

    g_Keyboard->Clear();

    fixed_t oldvolume = g_Options.Get_Score_Volume();

    // Set the score volume for the main menu only.
    if (g_Options.Get_Score_Volume() <= fixed_t(0, 1)) {
        g_Options.Set_Score_Volume(fixed_t(1, 2));
    }

    int start_tick = g_TickCountTimer.Time();

    while (process) {
        // Play menu theme, this theme should loop.
        if (g_Theme.What_Is_Playing() == THEME_NONE) {
            g_Theme.Play_Song(THEME_INTRO);
        }

        if (attraction_delay > 0) {
            // check if the ticks passed as go past the attraction delay.
            if ((g_TickCountTimer.Time() - start_tick) > attraction_delay) {
                // Fade out the menu theme.
                g_Theme.Fade_Out();

                // Fade screen to black.
                g_BlackPalette.Set(15);

                _selection = MENU_NONE;

                process = false;
            }
        }

        if (g_AllSurfaces.Surfaces_Restored()) {
            g_AllSurfaces.Clear_Surfaces_Restored();
            to_draw = true;
        }

        Call_Back();

        if (to_draw) {
            Load_Title_Page();

            g_CCPalette.Set();

            Set_Logic_Page(g_HidPage);

            // Draw all linked menu buttons
            activebtn->Draw_All(true);

            g_Mouse->Hide_Mouse();

            g_HidPage.Blit(g_SeenBuff);

            g_Mouse->Show_Mouse();

            Set_Logic_Page(g_SeenBuff);

            to_draw = false;
        }

        KeyNumType input = activebtn->Input();

        if (input != KN_NONE) {
#if defined(PLATFORM_WINDOWS)
            struct _SYSTEMTIME SystemTime;
            GetSystemTime(&SystemTime);
            g_CryptRandom.Seed_Byte(SystemTime.wMilliseconds);
#else // !PLATFORM_WINDOWS
            struct tm *SystemTime;
            struct timespec MicroTime;
            MiscUtil::Clock_Get_Time(CLOCK_REALTIME, &MicroTime);
            SystemTime = localtime(&MicroTime.tv_sec);
            g_CryptRandom.Seed_Byte(MicroTime.tv_nsec / 1000);
#endif // PLATFORM_WINDOWS

            switch (input) {
                case GADGET_BUTTON(BUTTON_CS):
                    captainslog_debug("In Main_Menu(), BUTTON_CS clicked.\n");
                    _selection = MENU_COUNTERSTRIKE;
                    process = false;
                    break;

                case GADGET_BUTTON(BUTTON_AM):
                    captainslog_debug("In Main_Menu(), BUTTON_AM clicked.\n");
                    _selection = MENU_AFTERMATH;
                    process = false;
                    break;

                case GADGET_BUTTON(BUTTON_NEW_GAME):
                    captainslog_debug("In Main_Menu(), BUTTON_NEW_GAME clicked.\n");
                    _selection = MENU_NEW_GAME;
                    process = false;
                    break;

                case GADGET_BUTTON(BUTTON_LOAD_GAME):
                    captainslog_debug("In Main_Menu(), BUTTON_LOAD_GAME clicked.\n");
                    _selection = MENU_LOAD_GAME;
                    process = false;
                    break;

                case GADGET_BUTTON(BUTTON_MPLAYER_GAME):
                    captainslog_debug("In Main_Menu(), BUTTON_MP_GAME clicked.\n");
                    _selection = MENU_MPLAYER_GAME;
                    process = false;
                    break;

                    // NOTE: No longer needed as we merge this listing into Movies.
                    /*case GADGET_BUTTON(BUTTON_SNEAK_PEAK):
                        captainslog_debug("In Main_Menu(), BUTTON_SNEAK_PEAK clicked.\n");
                        _selection = MENU_SNEAK_PEAK;
                        process = false;
                        break;*/

                case GADGET_BUTTON(BUTTON_MOVIES):
                    captainslog_debug("In Main_Menu(), BUTTON_MOVIES clicked.\n");
                    _selection = MENU_MOVIES;
                    process = false;
                    break;

                case GADGET_BUTTON(BUTTON_MAP_EDITOR):
                    captainslog_debug("In Main_Menu(), BUTTON_MAP_EDITOR clicked.\n");
                    _selection = MENU_MAP_EDITOR;
                    process = false;
                    break;

                case GADGET_BUTTON(BUTTON_OPTIONS):
                    captainslog_debug("In Main_Menu(), BUTTON_OPTIONS clicked.\n");
                    _selection = MENU_OPTIONS;
                    process = false;
                    break;

                case GADGET_BUTTON(BUTTON_EXIT):
                    captainslog_debug("In Main_Menu(), BUTTON_EXIT clicked.\n");
                    _selection = MENU_EXIT;
                    process = false;
                    break;

                case KN_LMOUSE: {
                    if (Coordinates_In_Region(g_Keyboard->Get_MouseQX(),
                            g_Keyboard->Get_MouseQY(),
                            TitlePageXPos + LOGO_BTN_XPOS,
                            TitlePageYPos + LOGO_BTN_YPOS,
                            TitlePageXPos + LOGO_BTN_XPOS + LOGO_BTN_WIDTH,
                            TitlePageYPos + LOGO_BTN_YPOS + LOGO_BTN_HEIGHT)) {
                        // Fade screen to black.
                        g_BlackPalette.Set(15);

                        // Display the game credits.
                        Show_Who_Was_Responsible();

                        // Play the menu theme as we are returning back to the main menu loop;
                        g_Theme.Play_Song(THEME_INTRO);

                        // Flag the menu gadgets to draw.
                        to_draw = true;

                        break;
                    }

                    if (cs_present) {
                        if (g_Keyboard->Down(KN_LSHIFT) || g_Keyboard->Down(KN_RSHIFT)) {
                            if (Coordinates_In_Region(g_Keyboard->Get_MouseQX(),
                                    g_Keyboard->Get_MouseQY(),
                                    TitlePageXPos + ANTS_BTN_XPOS,
                                    TitlePageYPos + ANTS_BTN_YPOS,
                                    TitlePageXPos + ANTS_BTN_XPOS + ANTS_BTN_WIDTH,
                                    TitlePageYPos + ANTS_BTN_YPOS + ANTS_BTN_HEIGHT)) {
                                // Fade screen to black.
                                g_BlackPalette.Set(15);

                                // It Came From Red Alert!
                                g_AntsEnabled = true;

                                process = false;

                                _selection = MENU_AFTERMATH;

                                break;
                            }
                        }
                    }

                    break;
                }

                // Move up one button.
                case KN_UP: {
                    // Turn off the currently selected button.
                    TextButtonClass *txtbtn = reinterpret_cast<TextButtonClass *>(_menu_gadgets[_current_selection]);
                    txtbtn->Turn_Off();

                    --_current_selection;
                    if (_current_selection < _topmost_button) {
                        _current_selection = _topmost_button;
                    }

                    // Fetch the newly selected button and redraw it.
                    txtbtn = reinterpret_cast<TextButtonClass *>(_menu_gadgets[_current_selection]);
                    txtbtn->Turn_On();

                    break;
                }

                // Move down one button.
                case KN_DOWN: {
                    // Turn off the currently selected button.
                    TextButtonClass *txtbtn = reinterpret_cast<TextButtonClass *>(_menu_gadgets[_current_selection]);
                    txtbtn->Turn_Off();

                    ++_current_selection;
                    if (_current_selection >= MENU_COUNT) {
                        _current_selection = _topmost_button;
                    }

                    // Fetch the newly selected button and redraw it.
                    txtbtn = reinterpret_cast<TextButtonClass *>(_menu_gadgets[_current_selection]);
                    txtbtn->Turn_On();

                    break;
                }

                // Confirm selection.
                case KN_RETURN:
                //case KN_KEYPAD_RETURN:
                {
                    // Toggle the currently selected button and redraw it.
                    TextButtonClass *txtbtn = reinterpret_cast<TextButtonClass *>(_menu_gadgets[_current_selection]);
                    txtbtn->Set_Toggle_Bool1(true);
                    txtbtn->Draw_Me(true);

                    _selection = _current_selection;
                    process = false;
                    break;
                }

                // Exit game.
                case KN_ESC: {
                    _selection = MENU_EXIT;
                    process = false;
                    break;
                }

                default:
                    break;
            };
        }
    }

    // Restore the score volume to the default setting.
    g_Options.Set_Score_Volume(oldvolume);

    // Return the chosen button action.
    return _selection;
}
