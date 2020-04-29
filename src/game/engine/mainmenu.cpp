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
#include "egos.h"
#include "expansion.h"
#include "gadget.h"
#include "gameevent.h"
#include "gameoptions.h"
#include "gbuffer.h"
#include "globals.h"
#include "init.h" // for Init_Random().
#include "iomap.h"
#include "ipx.h"
#include "keyboard.h"
#include "loaddialog.h"
#include "mouse.h"
#include "movie.h"
#include "msgbox.h"
#include "multimission.h"
#include "palette.h"
#include "pcx.h"
#include "queue.h"
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
    BOOL (*func)(void) = reinterpret_cast<BOOL (*)(void)>(0x0051289C);
    return func();
#else
    return false;
#endif
}

BOOL Init_Network()
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(void) = reinterpret_cast<BOOL (*)(void)>(0x005063C8);
    return func();
#else
    return false;
#endif
}
BOOL Remote_Connect()
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(void) = reinterpret_cast<BOOL (*)(void)>(0x00506840);
    return func();
#else
    return false;
#endif
}

BOOL Is_Mission_Counterstrike(const char *scen_name)
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(const char *) = reinterpret_cast<BOOL (*)(const char *)>(0x0054B8B4);
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
    captainslog_debug("Load_Title_Screen(%s).", filename);

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

    // Make sure mouse is flagged to draw.
    g_Mouse->Show_Mouse();

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
    g_Session.Set_Desired_Frame_Rate(30);
    g_Session.Set_MPlayer_Obi_Wan(false);
    g_Scen.Global_Timer().Reset();
    g_Scen.Global_Timer().Stop();
    g_Scen.Set_Difficulties(DIFF_NORMAL, DIFF_NORMAL);
    g_Map.Reset_Pending_Object();
    g_Map.Set_Cursor_Shape(nullptr);
    
    for (HousesType house = HousesType(0); house < HOUSES_MULTI_COUNT; ++house) {
        g_Session.MPlayer_Score_Info(house).m_Score[g_Session.MPlayer_Current_Game()] = -1;
    }

    g_Map.Set_Default_Mouse(MOUSE_POINTER);

    // By default, the attraction should not kick in if we can
    // not find the recording file, so set the default delay to zero.
    int attraction_delay = 0;

    if (g_Session.Attraction_Allowed() && g_Session.Recording_File().Is_Available()) {
        attraction_delay = DEFAULT_ATTRACTION_DELAY;
    }

    MenuButtonType menu_selection = g_Session.Game_To_Play() != GAME_CAMPAIGN ? MENU_MPLAYER_GAME : MENU_RESET;
    g_Theme.Queue_Song(THEME_CRUSH);

    // Load recording values if we are in the playback mode.
    if (g_Session.Playback_Game()) {
        if (g_Session.Recording_File().Open(FM_READ)) {
            Load_Recording_Values(g_Session.Recording_File());
            g_Theme.Queue_Song(THEME_STOP);
        } else {
            // We failed to open the recording file, reset the playback flag.
            g_Session.Set_Playback_Game(false);
        }
    }

    while (process) {
        // Draw the menu title page without any buttons.
        if (to_draw) {
            g_Mouse->Hide_Mouse();
            Load_Title_Page();
            g_GamePalette = g_CCPalette;
            g_HidPage.Blit(g_SeenBuff);
            g_CCPalette.Set();
            Set_Logic_Page(g_SeenBuff);
            g_Mouse->Show_Mouse();
            to_draw = false;
        }

        if (s_Special.Is_First_Run()) {
            menu_selection = MENU_NEW_GAME;
        }

        // If the menu selection has been reset (exit from a child dialog), re-init the main menu.
        if (menu_selection == MENU_RESET) { // was 8 in original?
            g_AntsEnabled = false;
            menu_selection = Main_Menu(attraction_delay);
        }

        Call_Back();

        switch (menu_selection) {
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
            case MENU_AFTERMATH: {
                // If both expansions are not installed, this menu option is not accessable.
                if (!Is_Counterstrike_Installed() && !Is_Aftermath_Installed()) {
                    break;
                }

                // TODO: reset CarryoverClass?
                g_Scen.Set_Carry_Over_Money(fixed_t(0, 1));
                g_IsTanyaDead = false;
                g_SaveTanya = false;

                if (menu_selection == MENU_COUNTERSTRIKE) {
                    if (!Force_CD_Available(DISK_COUNTERSTRIKE) || !Expansion_Dialog(EXPANSION_COUNTERSTRIKE)) {
                        menu_selection = MENU_RESET;
                        break;
                    }
                } else { // Aftermath
                    if (!Force_CD_Available(DISK_AFTERMATH) || !Expansion_Dialog(EXPANSION_AFTERMATH)) {
                        menu_selection = MENU_RESET;
                        break;
                    }
                }

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
                        menu_selection = MENU_RESET;
                        continue;
                };

                g_Theme.Stop_Playing_Song();
                g_Theme.Queue_Song(THEME_BIG_FOOT);
                g_Session.Set_Game_To_Play(GAME_CAMPAIGN);
                process = false;

                break;
            }
            case MENU_NEW_GAME:
                if (s_Special.Is_First_Run()) {
                    g_Scen.Set_Difficulties(DIFF_NORMAL, DIFF_NORMAL);
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
                }
                
                // TODO: reset CarryoverClass?
                g_Scen.Set_Carry_Over_Money(fixed_t(0, 1));
                g_IsTanyaDead = false;
                g_SaveTanya = false;
                g_BuildLevel = 10; // TODO: move to scenario init clear?
                g_Whom = HOUSES_GOODGUY;

                if (s_Special.Is_First_Run()) {
                    g_Theme.Stop_Playing_Song();
                    Choose_Side();
                    g_Mouse->Hide_Mouse();

                    if (g_CurrentCD == DISK_ALLIED) {
                        g_Scen.Set_Scenario_Name("SCG01EA.INI");
                    } else {
                        // Any other disk ends up being soviet?
                        g_Scen.Set_Scenario_Name("SCU01EA.INI");
                    }
                } else {
                    // Set the first special ants mission if we have been flagged as starting this campaign.
                    if (g_AntsEnabled) {
                        g_Scen.Set_Scenario_Name("SCA01EA.INI");
                    } else {
                        MessageBoxClass msgbox;

                        switch (msgbox.Process(TXT_CHOOSE_SIDE, TXT_ALLIES, TXT_CANCEL, TXT_SOVIET)) {
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
                }

                g_Session.Set_Game_To_Play(GAME_CAMPAIGN);
                process = false;
                break;

            case MENU_LOAD_GAME: {
                LoadOptionsClass loadopt(LoadOptionsClass::MODE_LOAD);

                if (loadopt.Process()) {
                    g_Theme.Queue_Song(THEME_BIG_FOOT);
                    loaded_game = true;
                    process = false;
                } else {
                    menu_selection = MENU_RESET;
                    to_draw = true;
                }

                break;
            }
            case MENU_MPLAYER_GAME:
                // This switch decides what to do when entering the MP menu. It tries to return to where it was if possible.
                switch (g_Session.Game_To_Play()) {
                    case GAME_CAMPAIGN: // This option is used when first entering the MP menu.
                        g_Session.Set_Game_To_Play(Select_MPlayer_Game());

                        // If we cancelled and we still have GAME_CAMPAIGN, reset menu.
                        if (g_Session.Game_To_Play() == GAME_CAMPAIGN) {
                            menu_selection = MENU_RESET;
                            to_draw = true;
                        }

                        // Otherwise we trust Select_MPlayer_Game set our game up correctly and we are good to go.
                        break;
                    case GAME_MODEM:
                    case GAME_NULL_MODEM: // Fallthrough, modem and null modem here.
                        // TODO: Do we really want to support these as options?
                        g_Session.Set_Game_To_Play(GAME_CAMPAIGN);
                        menu_selection = MENU_RESET;
                        to_draw = true;
                        break;
                    case GAME_INTERNET:
                        // TODO: Can't link WOL cross platform, implement CNCNet support?
                        g_Session.Set_Game_To_Play(GAME_CAMPAIGN);
                        menu_selection = MENU_RESET;
                        to_draw = true;
                        break;
                    case GAME_SKIRMISH:
                        if (!Com_Scenario_Dialog(true)) {
                            g_Session.Set_Game_To_Play(Select_MPlayer_Game());

                            // If we cancelled and we still have GAME_CAMPAIGN, reset menu.
                            if (g_Session.Game_To_Play() == GAME_CAMPAIGN) {
                                menu_selection = MENU_RESET;
                                to_draw = true;
                            }
                        } else {
                            g_AftermathMultiplayer =
                                Is_Aftermath_Installed() && !Is_Mission_Counterstrike(g_Scen.Scenario_Name());
                            g_Session.Set_Scenario_Is_Official(g_Session.Get_Local_Scenario()->Official());
                        }

                        break;
                    default:
                        break;
                }

                switch (g_Session.Game_To_Play()) {
                    // Looks like these are supposed to do the same as Skirmish here and just go on to get in game, but not supported yet.
                    //case GAME_MODEM:
                    //case GAME_NULL_MODEM:
                    //case GAME_INTERNET:
                    case GAME_SKIRMISH:
                        g_Theme.Stop_Playing_Song();
                        process = false;
                        g_Options.Set_Score_Volume(g_Options.Get_Multiplayer_Score_Volume());
                        break;
                    case GAME_LAN:
                        if (g_PacketTransport != nullptr) {
                            delete g_PacketTransport;
                        }

                        g_PacketTransport = new IPXInterfaceClass;

                        if (g_Session.Game_To_Play() == GAME_LAN && Init_Network() && Remote_Connect()) {
                            g_Theme.Stop_Playing_Song();
                            process = false;
                            g_Options.Set_Score_Volume(g_Options.Get_Multiplayer_Score_Volume());
                        } else {
                            g_Session.Set_Game_To_Play(GAME_CAMPAIGN);
                            menu_selection = MENU_RESET;
                            to_draw = true;
                            delete g_PacketTransport;
                            g_PacketTransport = nullptr;
                        }

                        break;
                    default:
                        break;
                }
                
                break;
            case MENU_MOVIES:
                g_Theme.Stop_Playing_Song();

                if (g_Debug_Flag) {
                    Play_Intro(true);
                } else {
                    // Movies_Dialog();
                    g_Mouse->Hide_Mouse();
                    g_VisiblePage.Clear();
                    g_Mouse->Show_Mouse();
                    Play_Movie(MOVIE_PROLOG);
                    g_Keyboard->Clear();
                    Play_Movie(MOVIE_SIZZLE);
                    g_Keyboard->Clear();
                    Play_Movie(MOVIE_SIZZLE2);
                    g_Keyboard->Clear();
                }

                menu_selection = MENU_RESET;
                g_Theme.Queue_Song(THEME_CRUSH);
                to_draw = true;
                break;

            case MENU_MAP_EDITOR:
                // TODO: Implement
                menu_selection = MENU_RESET;
                break;

            case MENU_OPTIONS:
                // TODO: Implement
                menu_selection = MENU_RESET;
                break;

            case MENU_EXIT:
                if (g_ConfirmExit) {
                    if (MessageBoxClass::Simple_YesNo_Message_Box(TXT_CONFIRM_QUIT) == ID_BUTTON_1) { // ID_BUTTON_1 is No.
                        menu_selection = MENU_RESET; // Jump back to the main menu.
                        continue;
                    }
                }
                g_Theme.Stop_Playing_Song();
                g_BlackPalette.Set(30);
                return false;

            // Continue the process loop as no button was clicked.
            default:
                break;
        };

    } // while (process)
    
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

    switch (g_Session.Game_To_Play()) {
        case GAME_INTERNET:
            /*if ( Wolapi == nullptr ) {
                captainslog_debug("g_Wolapi is NULL on internet game!");
            }*/

            if (!g_AftermathMultiplayer) {
                g_SecretUnitsEnabled = false;
                g_NewUnitsEnabled = false;
            }
            break;

        case GAME_MODEM:
        case GAME_NULL_MODEM:
        case GAME_LAN:
        case GAME_SKIRMISH:
            if (!g_AftermathMultiplayer) {
                g_SecretUnitsEnabled = false;
                g_NewUnitsEnabled = false;
            } else {
                g_NewUnitsEnabled = true;
            }
            break;
        default:
            break;

    }

    if (!loaded_game && !g_Session.Loading_Game()) {
        g_Mouse->Hide_Mouse();

        if (menu_selection != MENU_NEW_GAME) {
            g_BlackPalette.Set(15, Call_Back);
            g_HiddenPage.Clear();
            g_VisiblePage.Clear();
        }

        g_Mouse->Show_Mouse();

        captainslog_debug("Attempting to start scenario.");
        if (!Start_Scenario(g_Scen.Scenario_Name(), true)) {
            return false;
        }
    }

    captainslog_debug("Scenario started OK.");
    captainslog_debug("Initialising message system.");
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

    if (g_Session.Game_To_Play() != GAME_CAMPAIGN && g_Session.Game_To_Play() != GAME_SKIRMISH
        && g_Session.Game_To_Play() != GAME_TEN && g_Session.Game_To_Play() != GAME_MPATH && !g_Session.Playback_Game()) {
        g_Session.Create_Connections();
    }

    // Reset the unit build penalty to the default value.
    if (g_Session.Game_To_Play() != GAME_INTERNET) {
        g_UnitBuildPenalty = 100;
    }

    Call_Back();
    g_Mouse->Hide_Mouse();
    g_BlackPalette.Set(15, Call_Back);

    // Clear buffer pages.
    g_HiddenPage.Clear();
    g_VisiblePage.Clear();
    g_Mouse->Show_Mouse();
    Set_Logic_Page(g_SeenBuff);

    // Activate the sidebar.
    g_Map.Activate(1);
    g_Map.Flag_To_Redraw();
    Call_Back();
    g_Map.Render();

    return true;
}

MenuButtonType Main_Menu(int attraction_delay)
{
    GadgetClass *menu_gadgets[MENU_COUNT];
    MenuButtonType selection = MENU_NONE;
    MenuButtonType topmost_button = MENU_NONE;
    MenuButtonType current_selection = MENU_NONE;
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
    const int btn_width = 236;
    const int btn_height = 18;
    int btn_spacing = 2;
    int btn_xpos = TitlePageXPos + (TitlePageWidth / 2) - (btn_width / 2);
    int btn_ypos = TitlePageYPos + 156;

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
    int txt_right_xpos = 392; // TitlePageXPos + TitlePageWidth - txt_x_offset;
    int txt_web_ypos = TitlePageYPos + TitlePageHeight - 41;
    int txt_ver_ypos = TitlePageYPos + TitlePageHeight - 52;

    // Calculate button spacing depending on what expansions are enabled.
    if (am_present && cs_present) {
        btn_spacing += 1;
    } else if (cs_present || am_present) {
        btn_spacing += 3;
        btn_ypos -= (btn_height + btn_spacing);
    } else {
        btn_spacing += 4;
        btn_ypos -= ((btn_height + btn_spacing) * 2);
    }

    TextButtonClass csbtn(
        BUTTON_CS, TXT_CS_MISSIONS, TPF_6PT_GRAD | TPF_CENTER | TPF_NOSHADOW, btn_xpos, btn_ypos, btn_width, btn_height);
    btn_ypos += btn_height + btn_spacing;

    TextButtonClass ambtn(
        BUTTON_AM, TXT_AM_MISSIONS, TPF_6PT_GRAD | TPF_CENTER | TPF_NOSHADOW, btn_xpos, btn_ypos, btn_width, btn_height);
    btn_ypos += btn_height + btn_spacing;

    TextButtonClass newgamebtn(BUTTON_NEW_GAME,
        TXT_START_NEW_GAME,
        TPF_6PT_GRAD | TPF_CENTER | TPF_NOSHADOW,
        btn_xpos,
        btn_ypos,
        btn_width,
        btn_height);
    btn_ypos += btn_height + btn_spacing;

    TextButtonClass loadgamebtn(BUTTON_LOAD_GAME,
        TXT_LOAD_MISSION,
        TPF_6PT_GRAD | TPF_CENTER | TPF_NOSHADOW,
        btn_xpos,
        btn_ypos,
        btn_width,
        btn_height);
    btn_ypos += btn_height + btn_spacing;

    TextButtonClass mpgamebtn(BUTTON_MPLAYER_GAME,
        TXT_MULTIPLAYER_GAME,
        TPF_6PT_GRAD | TPF_CENTER | TPF_NOSHADOW,
        btn_xpos,
        btn_ypos,
        btn_width,
        btn_height);
    btn_ypos += btn_height + btn_spacing;

    TextButtonClass movbtn(BUTTON_MOVIES,
        TXT_MOVIES,
        TPF_6PT_GRAD | TPF_CENTER | TPF_NOSHADOW,
        btn_half_left_xpos,
        btn_ypos,
        btn_width,
        btn_height);
    btn_ypos += btn_height + btn_spacing;

    TextButtonClass editorbtn(BUTTON_MAP_EDITOR,
        TXT_MAP_EDITOR,
        TPF_6PT_GRAD | TPF_CENTER | TPF_NOSHADOW,
        btn_xpos,
        btn_ypos,
        btn_width,
        btn_height);
    btn_ypos += btn_height + btn_spacing;

    TextButtonClass optbtn(
        BUTTON_OPTIONS, TXT_OPTIONS, TPF_6PT_GRAD | TPF_CENTER | TPF_NOSHADOW, btn_xpos, btn_ypos, btn_width, btn_height);
    btn_ypos += btn_height + btn_spacing;

    TextButtonClass exitbtn(
        BUTTON_EXIT, TXT_EXIT_GAME, TPF_6PT_GRAD | TPF_CENTER | TPF_NOSHADOW, btn_xpos, btn_ypos, btn_width, btn_height);

    TextLabelClass version_label(const_cast<char *>(g_Version.Version_String()),
        txt_right_xpos,
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
    movbtn.Add_Tail(*activebtn);
    editorbtn.Add_Tail(*activebtn);
    optbtn.Add_Tail(*activebtn);
    exitbtn.Add_Tail(*activebtn);

    // Text label linking.
    version_label.Add_Tail(*activebtn);

    // Store all the text buttons in the array, this is used for keyboard navigation.
    menu_gadgets[MENU_COUNTERSTRIKE] = &csbtn;
    menu_gadgets[MENU_AFTERMATH] = &ambtn;
    menu_gadgets[MENU_NEW_GAME] = &newgamebtn;
    menu_gadgets[MENU_LOAD_GAME] = &loadgamebtn;
    menu_gadgets[MENU_MPLAYER_GAME] = &mpgamebtn;
    menu_gadgets[MENU_MOVIES] = &movbtn;
    menu_gadgets[MENU_MAP_EDITOR] = &editorbtn;
    menu_gadgets[MENU_OPTIONS] = &optbtn;
    menu_gadgets[MENU_EXIT] = &exitbtn;

    // Turn on (select/highlight/enable) the top most button.
    if (cs_present) {
        reinterpret_cast<TextButtonClass *>(menu_gadgets[MENU_COUNTERSTRIKE])->Turn_On();
        topmost_button = MENU_COUNTERSTRIKE;
        current_selection = MENU_COUNTERSTRIKE;
    } else if (am_present) {
        reinterpret_cast<TextButtonClass *>(menu_gadgets[MENU_AFTERMATH])->Turn_On();
        topmost_button = MENU_AFTERMATH;
        current_selection = MENU_AFTERMATH;
    } else {
        reinterpret_cast<TextButtonClass *>(menu_gadgets[MENU_NEW_GAME])->Turn_On();
        topmost_button = MENU_NEW_GAME;
        current_selection = MENU_NEW_GAME;
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
            if ((g_TickCountTimer.Time() - start_tick) > (unsigned)attraction_delay) {
                // Fade out the menu theme.
                g_Theme.Fade_Out();

                // Fade screen to black.
                g_BlackPalette.Set(15);
                selection = MENU_NONE;
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
            struct _SYSTEMTIME sys_time;
            GetSystemTime(&sys_time);
            g_CryptRandom.Seed_Byte(sys_time.wMilliseconds);
#elif defined HAVE_SYS_TIME_H
            struct tm *sys_time;
            struct timeval curr_time;
            gettimeofday(&curr_time, nullptr);
            sys_time = localtime(&curr_time.tv_sec);
            g_CryptRandom.Seed_Byte(curr_time.tv_usec / 1000);
#endif // PLATFORM_WINDOWS

            switch (input) {
                case GADGET_BUTTON(BUTTON_CS):
                    captainslog_debug("In Main_Menu(), BUTTON_CS clicked.");
                    selection = MENU_COUNTERSTRIKE;
                    process = false;
                    break;

                case GADGET_BUTTON(BUTTON_AM):
                    captainslog_debug("In Main_Menu(), BUTTON_AM clicked.");
                    selection = MENU_AFTERMATH;
                    process = false;
                    break;

                case GADGET_BUTTON(BUTTON_NEW_GAME):
                    captainslog_debug("In Main_Menu(), BUTTON_NEW_GAME clicked.");
                    selection = MENU_NEW_GAME;
                    process = false;
                    break;

                case GADGET_BUTTON(BUTTON_LOAD_GAME):
                    captainslog_debug("In Main_Menu(), BUTTON_LOAD_GAME clicked.");
                    selection = MENU_LOAD_GAME;
                    process = false;
                    break;

                case GADGET_BUTTON(BUTTON_MPLAYER_GAME):
                    captainslog_debug("In Main_Menu(), BUTTON_MP_GAME clicked.");
                    selection = MENU_MPLAYER_GAME;
                    process = false;
                    break;

                case GADGET_BUTTON(BUTTON_MOVIES):
                    captainslog_debug("In Main_Menu(), BUTTON_MOVIES clicked.");
                    selection = MENU_MOVIES;
                    process = false;
                    break;

                case GADGET_BUTTON(BUTTON_MAP_EDITOR):
                    captainslog_debug("In Main_Menu(), BUTTON_MAP_EDITOR clicked.");
                    selection = MENU_MAP_EDITOR;
                    process = false;
                    break;

                case GADGET_BUTTON(BUTTON_OPTIONS):
                    captainslog_debug("In Main_Menu(), BUTTON_OPTIONS clicked.");
                    selection = MENU_OPTIONS;
                    process = false;
                    break;

                case GADGET_BUTTON(BUTTON_EXIT):
                    captainslog_debug("In Main_Menu(), BUTTON_EXIT clicked.");
                    selection = MENU_EXIT;
                    process = false;
                    break;

                case KN_LMOUSE: {
                    if (Coordinates_In_Region(g_Keyboard->Get_MouseQX(),
                            g_Keyboard->Get_MouseQY(),
                            TitlePageXPos + LOGO_BTN_XPOS,
                            TitlePageYPos + LOGO_BTN_YPOS,
                            TitlePageXPos + LOGO_BTN_XPOS + LOGO_BTN_WIDTH,
                            TitlePageYPos + LOGO_BTN_YPOS + LOGO_BTN_HEIGHT)) {
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
                                // It Came From Red Alert!
                                g_AntsEnabled = true;
                                process = false;
                                selection = MENU_NEW_GAME;

                                break;
                            }
                        }
                    }

                    break;
                }

                // Move up one button.
                case KN_UP: {
                    // Turn off the currently selected button.
                    TextButtonClass *txtbtn = reinterpret_cast<TextButtonClass *>(menu_gadgets[current_selection]);
                    txtbtn->Turn_Off();
                    txtbtn->Flag_To_Redraw();
                    --current_selection;

                    if (current_selection < topmost_button) {
                        current_selection = MENU_EXIT;
                    }

                    // Fetch the newly selected button and redraw it.
                    txtbtn = reinterpret_cast<TextButtonClass *>(menu_gadgets[current_selection]);
                    txtbtn->Turn_On();
                    txtbtn->Flag_To_Redraw();

                    break;
                }

                // Move down one button.
                case KN_DOWN: {
                    // Turn off the currently selected button.
                    TextButtonClass *txtbtn = reinterpret_cast<TextButtonClass *>(menu_gadgets[current_selection]);
                    txtbtn->Turn_Off();
                    txtbtn->Flag_To_Redraw();
                    ++current_selection;

                    if (current_selection >= MENU_COUNT) {
                        current_selection = topmost_button;
                    }

                    // Fetch the newly selected button and redraw it.
                    txtbtn = reinterpret_cast<TextButtonClass *>(menu_gadgets[current_selection]);
                    txtbtn->Turn_On();
                    txtbtn->Flag_To_Redraw();

                    break;
                }

                // Confirm selection.
                case KN_RETURN: {
                    // Toggle the currently selected button and redraw it.
                    TextButtonClass *txtbtn = reinterpret_cast<TextButtonClass *>(menu_gadgets[current_selection]);
                    txtbtn->Set_Toggle_Bool1(true);
                    txtbtn->Draw_Me(true);
                    selection = current_selection;
                    process = false;
                    break;
                }

                default:
                    break;
            };
        }

        // BUGFIX: Stops high CPU load in this menu.
        PlatformTimerClass::Sleep(1);
    }

    // Restore the score volume to the default setting.
    g_Options.Set_Score_Volume(oldvolume);

    // Return the chosen button action.
    return selection;
}
