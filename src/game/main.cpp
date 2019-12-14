/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Entry point and associated low level init code.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "main.h"
#include "alloc.h"
#include "audio.h"
#include "cdfile.h"
#include "cmdline.h"
#include "cpudetect.h"
#include "dialog.h"
#include "eventhandler.h"
#include "focus.h"
#include "gamedebug.h"
#include "gamemain.h"
#include "gameoptions.h"
#include "gbuffer.h"
#include "getcd.h"
#include "gitverinfo.h"
#include "globals.h"
#include "ini.h"
#include "initvideo.h"
#include "keyboard.h"
#include "mouse.h"
#include "ostimer.h"
#include "session.h"
#include "shape.h"
#include "special.h"
#include "startup.h"
#include "surfacemonitor.h"
#include "version.h"

#ifdef PLATFORM_WINDOWS
#include <shellapi.h>

#if !defined GAME_DLL && defined COMPILER_MSVC
// This pragma makes msvc use the normal main function for GUI applications.
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif
#endif

#ifdef PLATFORM_UNIX
#include <libgen.h>
#include <unistd.h>
#endif

#ifdef PLATFORM_WINDOWS
#include <cstdlib>

using std::atexit;

// Global so we can ensure the argument list is freed at exit.
static char **g_argv;

void Free_Argv()
{
    LocalFree(g_argv);
}

// Taken from https://github.com/thpatch/win32_utf8/blob/master/src/shell32_dll.c
// Get the command line as UTF-8 as it would be on other platforms.
char **CommandLineToArgvU(LPCWSTR lpCmdLine, int *pNumArgs)
{
    int cmd_line_pos; // Array "index" of the actual command line string
    // int lpCmdLine_len = wcslen(lpCmdLine) + 1;
    int lpCmdLine_len = WideCharToMultiByte(CP_UTF8, 0, lpCmdLine, -1, nullptr, 0, nullptr, nullptr) + 1;
    char **argv_u;

    wchar_t **argv_w = CommandLineToArgvW(lpCmdLine, pNumArgs);

    if (!argv_w) {
        return nullptr;
    }

    cmd_line_pos = *pNumArgs + 1;

    // argv is indeed terminated with an additional sentinel NULL pointer.
    argv_u = (char **)LocalAlloc(LMEM_FIXED, cmd_line_pos * sizeof(char *) + lpCmdLine_len);

    if (argv_u) {
        int i;
        char *cur_arg_u = (char *)&argv_u[cmd_line_pos];

        for (i = 0; i < *pNumArgs; i++) {
            size_t cur_arg_u_len;
            argv_u[i] = cur_arg_u;
            int conv_len = WideCharToMultiByte(CP_UTF8, 0, argv_w[i], -1, cur_arg_u, lpCmdLine_len, nullptr, nullptr);

            cur_arg_u_len = argv_w[i] != nullptr ? conv_len : conv_len + 1;
            cur_arg_u += cur_arg_u_len;
            lpCmdLine_len -= cur_arg_u_len;
        }

        argv_u[i] = nullptr;

        if (g_argv != nullptr) {
            LocalFree(g_argv);
        }

        g_argv = argv_u;
        atexit(Free_Argv);
    }

    LocalFree(argv_w);

    return argv_u;
}
#endif

/**
 * @brief Sets the working directory, some code currently expects data to be in this directory.
 */
void Set_Working_Directory()
{
#if defined(PLATFORM_WINDOWS)
    char path[MAX_PATH];

    GetModuleFileNameA(GetModuleHandleA(nullptr), path, sizeof(path));

    for (char *i = &path[strlen(path)]; i != path; --i) {
        if (*i == '\\' || *i == '/') {
            *i = '\0';
            break;
        }
    }

    SetCurrentDirectoryA(path);

#elif defined(PLATFORM_LINUX) // posix otherwise, really just linux currently
    // TODO /proc/curproc/file for FreeBSD /proc/self/path/a.out Solaris
    const char *link = "/proc/self/exe";
    struct stat sb;

    if (lstat(link, &sb) == -1) {
        DEBUG_LOG("lstat() error setting working directory.\n");
        exit(EXIT_FAILURE);
    }

    char *path = new char[sb.st_size + 1];
    ssize_t ret = readlink(link, path, PATH_MAX);

    if (ret < 0) {
        DEBUG_LOG("readlink() error setting working directory.\n");
        exit(EXIT_FAILURE);
    }

    if (ret > sb.st_size) {
        DEBUG_LOG("symlink increased in size between lstat() and readlink()\n");
        exit(EXIT_FAILURE);
    }

    path[sb.st_size] = '\0';

    if(chdir(dirname(path)) != 0) {
        DEBUG_LOG("Failed to set working directory with chdir().\n");
        exit(EXIT_FAILURE);
    }

    delete[] path;
#elif defined(PLATFORM_OSX) // osx otherwise
    char path[PATH_MAX];
    int size = PATH_MAX;
    _NSGetExecutablePath(path, &size);
    chdir(dirname(path));

#else //
#error Platform not supported for Set_Working_Directory()!
#endif // PLATFORM_WINDOWS
}

/**
 * @brief Creates the main window for the game.
 */
void Create_Main_Window(void *hInstance, int nCmdShow, int width, int height)
{
#ifdef PLATFORM_WINDOWS
    WNDCLASSA WndClass;
    HINSTANCE app_hinstance = GetModuleHandleA(nullptr);
    STARTUPINFOA sinfo;
    int show_cmd;

    sinfo.dwFlags = 0;
    GetStartupInfoA(&sinfo);

    if (sinfo.dwFlags & STARTF_USESHOWWINDOW) {
        show_cmd = sinfo.wShowWindow;
    } else {
        show_cmd = SW_SHOWDEFAULT;
    }

    WndClass.style = CS_VREDRAW | CS_HREDRAW;
    WndClass.lpfnWndProc = WindowProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = app_hinstance;
    WndClass.hIcon = LoadIconA(app_hinstance, (LPCSTR)1);
    WndClass.hCursor = 0;
    WndClass.hbrBackground = 0;
    WndClass.lpszMenuName = "Chronoshift";
    WndClass.lpszClassName = "Chronoshift";

    RegisterClassA(&WndClass);

#if defined(CHRONOSHIFT_DEBUG)
    const char *buildtype = "Debug";
#else
    const char *buildtype = "Release";
#endif

    char window_name[256];
    snprintf(window_name,
        sizeof(window_name),
        "Chronoshift [%s] - Branch:%s ID:%s Date:%s",
        buildtype,
        g_gitBranch,
        g_gitShortSHA1,
        g_gitCommitDate);

    HWND app_hwnd = CreateWindowExA(WS_EX_TOPMOST,
        "Chronoshift",
        window_name,
        WS_POPUP,
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
        0,
        0,
        0,
        0,
        app_hinstance,
        0);

    ShowWindow(app_hwnd, show_cmd);
    UpdateWindow(app_hwnd);
    SetFocus(app_hwnd);
    
    g_MainWindow = app_hwnd;
    g_CCFocusMessage = RegisterWindowMessageA("CHRONOSHIFT_GOT_FOCUS");
    g_AudioFocusLoss = Focus_Loss;
    g_MiscFocusLoss = Focus_Loss;
    g_MiscFocusRestore = Focus_Restore;
    g_GBufferFocusLoss = Focus_Loss;
#endif
}

/**
 * @brief Entry point for the game engine.
 */
#ifdef GAME_DLL
int not_main(int argc, char **argv)
#else
int main(int argc, char **argv)
#endif
{
    // Windows main can't take arguments as UTF8, so we need to overwrite them with the correct content.
#ifdef PLATFORM_WINDOWS
    argv = CommandLineToArgvU(GetCommandLineW(), &argc);
#endif

    DEBUG_INIT(DEBUG_LOG_TO_FILE);

    // Make pretty log header for debug logging builds.
    DEBUG_LOG("================================================================================\n\n");
    DEBUG_LOG("Chronoshift Version: %s\n", g_Version.Version_Name());
    DEBUG_LOG("Build date: %s\n", g_gitCommitDate);
    DEBUG_LOG("Build branch: %s\n", g_gitBranch);
    DEBUG_LOG("Build commit: %s\n", g_gitShortSHA1);
    // DEBUG_LOG("Processor: %s\n", CPUDetectClass::Get_Processor_String());
    // DEBUG_LOG("Physical Memory: %llu MiB.\n", CPUDetectClass::Get_Total_Physical_Memory() / (1024 * 1024 + 1));
    DEBUG_LOG(CPUDetectClass::Get_Processor_Log());
    DEBUG_LOG("================================================================================\n");

    // Commented out, we dont need to worry about memory these days...
    /*if (Ram_Free() < 7000000) {
        DEBUG_LOG("Insufficient free memory, only have %d KB free.\n", Ram_Free() / 1024);
        return 255;
    }*/

#ifdef GAME_DLL
    // Remove this once Send_Statistics_Packet is implemented as its only used there.
    g_ProgramInstance = GetModuleHandleA(nullptr);
#endif

    Set_Working_Directory();

    // Original checked for and tried to delete some wolapi reg keys here and wolsetup.exe if it existed.

    // Original tried to delete conquer.eng if it existed here.

    if (!Parse_Command_Line(argc, argv)) {
        return 0;
    }

    g_PlatformTimer = new PlatformTimerClass(60, false);

    unsigned ticks = g_PlatformTimer->Get_System_Tick_Count();
    PlatformTimerClass::Sleep(1000); // Sleep for 1 second to test the timer.

    if (ticks == g_PlatformTimer->Get_System_Tick_Count()) {
        DEBUG_LOG(
            "Error - Timer system failed to initialise due to system instability. You may need to restart the operating "
            "system.");

        return 255;
    }

    g_Keyboard = new KeyboardClass;
    
    Check_Use_Compressed_Shapes();

    // Check if we have reasonable enough space for saves and such.
    if (Disk_Space_Available() < 0x800000) {
        DEBUG_LOG("Disk space is critically low during init.\n");

        // TODO, original pops up windows message box informing of low space and asking to continue.
    }

    // TODO: Search for this in the user home folder, then in game folder as fallback.
    RawFileClass settingsfile("chronoshift.ini");
    INIClass settingsini;

    // If we was unable to find the settings file, copy them from the original
    // Red Alert's settings file and create a new Chronoshift settings file.
    if (!settingsfile.Is_Available()) {

        // Create blank file.
        settingsfile.Create();

        if (!settingsfile.Is_Available()) {
            DEBUG_LOG("Unable to create Chronoshift game settings file!\n");
            g_Keyboard->Get();
            delete g_PlatformTimer;
            g_PlatformTimer = nullptr;
            return 0;
        }

        // Copy settings from original Red Alert file. This also handles the
        // case where the game requires 'redalert.ini' to exist, otherwise
        // it will not run (user had to run SETUP.EXE first).
        RawFileClass redalertfile("redalert.ini");
        INIClass redalertini;
        if (!redalertfile.Is_Available()) {
            DEBUG_LOG("Red Alert settings ini not found, continuing with default settings.\n");
            settingsini.Put_Int("Sound", "Card", 0);
            settingsini.Put_Int("Sound", "Port", 0x3F8, INIINTEGER_AS_HEX);
            settingsini.Put_Int("Sound", "IRQ", 4);
            settingsini.Put_Int("Sound", "DMA", -1);
            settingsini.Put_Bool("Options", "HardwareFills", false);
            settingsini.Put_Bool("Options", "VideoBackBuffer", true);
            settingsini.Put_Int("Options", "Resolution", 0);

            // Save the settings out to file.
            settingsini.Save(settingsfile);
        } else {
            // Copy the existing settings to the file.
            redalertini.Save(settingsfile);
        }
    }

    settingsini.Load(settingsfile);

    Read_Startup_Options(&settingsfile);
    
    // NOTE: Original passed in screen dimentions etc, but we handle these
    // inside Create_Main_Window() itself now.
    Create_Main_Window(nullptr, 0, 0, 0);

    //TODO when Audio_Init is reimplemented, remove need for HWND parameter.
#ifdef PLATFORM_WINDOWS
    g_SoundOn = Audio_Init(g_MainWindow, 16, 0, 22050, 0);
#else
    g_SoundOn = Audio_Init(nullptr, 16, 0, 22050, 0);
#endif

    if (!Init_Video()) {
        delete g_PlatformTimer;
        g_PlatformTimer = nullptr;

        return 255;
    }

    g_Options.Adjust_Vars_For_Resolution();
    
    // TODO set memory error handler here.
    
    g_WindowList[WINDOW_0].W = g_SeenBuff.Get_Width();
    g_WindowList[WINDOW_0].H = g_SeenBuff.Get_Height();
    g_WindowList[WINDOW_5].W = g_SeenBuff.Get_Width();
    g_WindowList[WINDOW_5].H = g_SeenBuff.Get_Height();
    
    // TODO Won't need both of these once standalone.
    g_Mouse = g_WWMouse = new MouseClass(&g_SeenBuff, 48, 48);
    g_MouseInstalled = true;
    
    int cd_drive = g_CDList.Reset_And_Get_CD_Drive();
    CDFileClass::Set_CD_Drive(cd_drive);
    // CDFileClass::Set_CD_Drive(g_CDList.Reset_And_Get_CD_Drive());

    if (!s_Special.Is_First_Run() && g_Session.Game_To_Play() != GAME_6 && g_Session.Game_To_Play() != GAME_7) {
        s_Special.Set_First_Run(settingsini.Get_Bool("Intro", "PlayIntro", true));
    }

    if (s_Special.Is_First_Run()) {
        g_BreakoutAllowed = true;
        settingsini.Put_Bool("Intro", "PlayIntro", false);
        settingsini.Save(settingsfile);
    }

    g_SlowPalette = settingsini.Get_Bool("Options", "SlowPalette", true);
    
    // TODO set Memory_Error_Exit handler here.
    
    Game_Main(argc, argv);

    // Start tidying up before quit.
    g_VisiblePage.Clear();
    g_HiddenPage.Clear();
    
    // TODO set Memory_Error_Exit handler here.
    
    g_ReadyToQuit = 1;

#ifdef PLATFORM_WINDOWS
    PostMessageA(g_MainWindow, WM_DESTROY, 0, 0);
#endif

    // Keep calling the event handler loop until everything in the queue has been processed.
    do {
        g_Keyboard->Check();
    } while (g_ReadyToQuit == 1);

    return 0;
}

#ifdef GAME_DLL
/**
 * @brief Wrapper for main to hook original entry point
 */
int __stdcall Main_Func(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Windows code will replace the arguments to main anyway so it doesn't matter what we do here.
    int ret = not_main(0, nullptr);

    return ret;
}
#endif
