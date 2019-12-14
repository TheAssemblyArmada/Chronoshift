/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Command line parser.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "cmdline.h"
#include "globals.h"
#include "special.h"
#include "language.h"
#include "session.h"
#include "scenario.h"
#include "gamedebug.h"

/**
 * Parses the command line for any arguments we act upon.
 *
 * 0x004F5880
 */
BOOL Parse_Command_Line(int argc, char **argv)
{
    // List of available command line options.
    const char *_help_text = {
        "Parameters:\n"
        "  -L              = <TODO>.\n"
        "                    (Syntax: -L<char>)\n"
        "                    (Options: E(nglish), F(rench), G(erman), I(talian), S(Spanish), J(Japanese)\n"
        "  -CD             = Set search path for data files.\n"
        "                    (Syntax: -CD<path>)\n"
        "  -STEALTH        = Hide multiplayer names (\"Boss mode\")\n"
        "  -MESSAGES       = Allow messages from outside this game.\n\n"
        "  -ATTRACT        = Enable attraction mode.\n"
        "  -WIN            = Run the game in windowed mode.\n"
        "  -EDITOR         = Launch the game directly into the scenario editor.\n"
        "  -SEED           = Pre-seed the random number generator.\n"
        "\Misc Parameters:\n"
        "FROMINSTALL       = Enables \"first run\" mode after installation."
        "PLAYTEST          = <TODO>.\n"
        
#if defined(CHRONOSHIFT_DEBUG)
        "\nDebug Parameters:\n"
        "  -NOINTRO        = Skip the intro movie(s).\n"
        "  -NOSOUND        = Disable all sound output.\n"
        "  -NOMOVIE        = Disable all movie playback.\n"
        "  -SUPERRECORD    = Super record mode.\n"
        "  -RECORD         = Record gameplay to attraction file (default RECORD.BIN).\n"
        "  -PLAY           = Play stored attraction file (default RECORD.BIN).\n"
        "  -CHECKMAP       = Enables map check validation.\n"
        "  -HANSOLO        = <TODO>.\n"
        "  -SCEN           = <TODO>.\n"
        "                    (Syntax: -SCEN<filename>)\n"
        "  -X              = <TODO>.\n"
        "                    (Syntax: -X<char>)\n"
        "                    (Options: H = "
        "                              I = "
        "                              M = Monochrome debug mode."
        "                              Q = Disable all sound output."
        "                              S = Super record mode."
        "                              V = Targeting flash visible for all."
        "                              X = Record mode."
        "                              Y = Playback mode."
#endif // CHRONOSHIFT_DEBUG

#if defined(CHRONOSHIFT_DEBUG) || ASSERT_LEVEL == ASSERTS_RELEASE
        "\nAdditional Parameters:\n"
        "  -IGNOREASSERTS  = Ignore all assertions.\n"
        "  -SILENTASSERTS  = Slience all assertions (disables the pop-up, but still logs to file).\n"
#endif
    };

    // The following are for documentation purposes only.
#if defined(CHRONOSHIFT_DEBUG)
    static const char *_play_codes[] = {
        "SONY", // 0xE0792D6D
        "?", // 0x90046ECF
        "FUNK", // 0xC3EE9A26
        "SLICK" // 0xED382178
    };

    static const char *_editor_codes[] = {
        "BUILD", // 0xA2C09326
        "MOBIUS", // 0x1F944BB3
        "CYCLONE", // 0xDE07154D
        "CARPET", // 0x0E07B213
        "EDITOR" // 0x16B170B1
    };

    static const char *_cheat_codes[] = {
        "JUPITER", // 0xA0E2AB53
        "NADE", // 0x00532693
        "PASSWORD", // 0x7DDFF824
        "CHEATER", // 0x2CB5CF01
        "BLUB", // 0xB5B63531
        "?", // 0xDFABC23A
        "NUKE", // 0x52B19A22
        "?", // 0xBE79088C
        "SPOON", // 0xB216AE7E
        "CARPET" // 0x0E07B213
    };
#endif

    s_Special.Init();

    g_Whom = HOUSES_GOODGUY;

    g_InMapEditor = false;

    g_DebugUnshroud = false;
    g_Debug_Playtest = false;
    //g_Debug_Skip_Intro = false;
    g_Debug_Flag = false;

    g_Language = LANGUAGE_ENGLISH;

    // Make sure we have more than one entry stored in argv as the first entry is the program path.
    if (argc < 2) {
        return true;
    }

    DEBUG_LOG("About to parse command line arguments.\n");

    // NOTE: We're starting on 1 because we don't need to know the path of the program, which is stored in argv[0].
    for (int index = 1; index < argc; ++index) {

        const char *arg = strupr(argv[index]);

        DEBUG_LOG("\tFound: '%s'\n", arg);

        if (!strcasecmp(arg, "/?") || !strcasecmp(arg, "-?") || !strcasecmp(arg, "-h") || !strcasecmp(arg, "/h")) {
#ifdef PLATFORM_WINDOWS
            MessageBoxA(nullptr, _help_text, "Chronoshift", MB_ICONINFORMATION);
#else
            printf("Chronoshift by The Assembly Armada\n");
            printf(_help_text);
#endif
            return false;
        }

        // Check for a custom language option.
        if (strstr(arg, "-L")) {

            const char lang = *((char *)&arg[strlen("-L")]);
            switch (toupper(lang)) {

                case 'E':
                default:
                    g_Language = LANGUAGE_ENGLISH;
                    break;

                case 'F':
                    g_Language = LANGUAGE_FRENCH;
                    break;

                case 'G':
                    g_Language = LANGUAGE_GERMAN;
                    break;

                case 'I':
                    g_Language = LANGUAGE_ITALIAN;
                    break;

                case 'S':
                    g_Language = LANGUAGE_SPANISH;
                    break;

                case 'J':
                    g_Language = LANGUAGE_JAPANESE;
                    break;

            };

            continue;

        }

        if (strstr(arg, "-CD")) {
            const char *path = &arg[strlen("-CD")];
            CDFileClass::Set_Search_Drives(path);
            continue;
        }

        if (strstr(arg, "-STEALTH")) {
            g_Session.Set_Net_Stealth(true);
            continue;
        }

        if (strstr(arg, "-MESSAGES")) {
            g_Session.Set_Net_Protect(true);
            continue;
        }

        if (strstr(arg, "-ATTRACT")) {
            g_Session.Set_Allow_Attraction(true);
            continue;
        }

        if (strstr(arg, "-WIN")) {
            //Windowed = true;
            continue;
        }

        if (strstr(arg, "-EDITOR")) {
            s_Special.Set_First_Run(false); // TODO: Confirm correct bit.
            g_InMapEditor = true;
            g_Debug_Unshroud = true;
            //g_Debug_Skip_Intro = true;
            continue;
        }

        if (strstr(arg, "-SEED")) {
            char *seed = (char *)&arg[strlen("-SEED")];
            g_CustomSeed = atoi(seed);
            continue;
        }

        if (strstr(arg, "FROMINSTALL")) {
            s_Special.Set_First_Run(true); // TODO: Confirm correct bit.
            continue;
        }

        if (strstr(arg, "PLAYTEST")) {
            g_Debug_Playtest = true;
            continue;
        }

#if defined(CHRONOSHIFT_DEBUG)
        if (strstr(arg, "-NOINTRO")) {
            s_Special.Set_First_Run(false); // TODO: Confirm correct bit.
            continue;
        }

        if (strstr(arg, "-NOSOUND")) {
            g_Debug_Quiet = true;
            continue;
        }

        if (strstr(arg, "-NOMOVIE")) {
            //g_Debug_No_Movie = true;
            continue;
        }

        if (strstr(arg, "-SUPERRECORD")) {
            g_Session.Set_Super_Record_Game(true);
            g_Session.Set_Playback_Game(false);
            continue;
        }

        if (strstr(arg, "-RECORD")) {
            g_Session.Set_Record_Game(true);
            g_Session.Set_Playback_Game(false);
            continue;
        }

        if (strstr(arg, "-PLAY")) {
            g_Session.Set_Playback_Game(true);
            g_Session.Set_Record_Game(false);
            continue;
        }

        if (strstr(arg, "-CHECKMAP")) {
            g_Debug_Check_Map = true;
            continue;
        }

        if (strstr(arg, "-HANSOLO")) {
            //g_Session.MPlayerSolo = true;
            continue;
        }

        // TODO: Debug feature WIP.
        /*if (strstr(arg, "-SCEN")) {
            char *scen_name = (char *)&arg[strlen("-SCEN")];
            strcpy(OverrideScenario, scen_name);
            OverrideScenario[strlen(scen_name)] = '\0';
            s_Special.Set_First_Run(false); // TODO: Confirm correct bit.
            Debug_Skip_Intro = true;
            continue;
        }*/

        // Check for a debug options.
        if (strstr(arg, "-X") != nullptr) {

            const char opt = *((char *)&arg[strlen("-X")]);
            switch (toupper(opt)) {

                default:
            #ifdef PLATFORM_WINDOWS
                    MessageBoxA(nullptr, "Invalid debug option switch.", "Chronoshift", MB_ICONINFORMATION);
            #else
                    printf("Invalid debug option switch.");
            #endif
                    break;

                case 'H': // Instant special weapon recharge.
                    break;

                case 'I': // Everything is invulnerable to damage / Disable damage.
                    break;

                case 'M': // Monochrome debug mode.
                    break;

                case 'Q': // Quiet mode.
                    g_Debug_Quiet = true;
                    break;

                case 'S': // Super record mode.
                    g_Session.Set_Super_Record_Game(true);
                    g_Session.Set_Playback_Game(false);
                    break;

                case 'V': // Targeting flash visible for all.
                    break;

                case 'X': // Record mode.
                    g_Session.Set_Record_Game(true);
                    g_Session.Set_Playback_Game(false);
                    break;

                case 'Y': // Playback mode.
                    g_Session.Set_Playback_Game(true);
                    g_Session.Set_Record_Game(false);
                    break;

            };

            continue;

        }
#endif // CHRONOSHIFT_DEBUG

#if defined(CHRONOSHIFT_DEBUG) || ASSERT_LEVEL == ASSERTS_RELEASE
        if (strstr(arg, "-IGNOREASSERTS")) {
            //captain_ignoreasserts(true);
            continue;
        }

        if (strstr(arg, "-SILENTASSERTS")) {
            //captain_allowpopups(false);
            continue;
        }
#endif

    }

    return true;
}
