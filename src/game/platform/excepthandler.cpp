/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Functions for exception handling and log outputting.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */

#include "language.h"
#include "rawfile.h"
#include "stackdump.h"
#include "startup.h"
#include "stdio.h"

#if defined COMPILER_WATCOM || defined COMPILER_MSVC

#ifdef COMPILER_MSVC
#include <windows.h>
#include <winver.h>
#endif

RawFileClass g_ExceptLogFile;

void Except_Log_Init()
{
    char dirbuf[PATH_MAX];
    char curbuf[PATH_MAX];
    char prevbuf[PATH_MAX];

#ifdef PLATFORM_WINDOWS
    char *tmp = getenv("USERPROFILE");

    if (tmp != NULL) {
        strcpy(dirbuf, tmp);
        strcat(dirbuf, "\\Documents\\Chronoshift");
        CreateDirectoryA(dirbuf, NULL);
        strcat(dirbuf, "\\");
    } else {
        GetModuleFileNameA(0, dirbuf, sizeof(dirbuf));

        // Get the path to the executable minus the actual filename.
        for (char *i = &dirbuf[strlen(dirbuf)]; i >= dirbuf && (*i != '\\' || *i != '/'); --i) {
            *i = '\0';
        }
    }
#else

#endif
    const char *prefix = "";
    strcpy(prevbuf, dirbuf);
    strcat(prevbuf, prefix);
    strcat(prevbuf, "exceptPrev.txt");
    strcpy(curbuf, dirbuf);
    strcat(curbuf, prefix);
    strcat(curbuf, "except.txt");
    remove(prevbuf);
    rename(curbuf, prevbuf);
    g_ExceptLogFile.Set_Name(curbuf);
}

// msvc standard exception handler
LONG __cdecl Exception_Handler(unsigned int u, struct _EXCEPTION_POINTERS *e_info)
{
    Except_Log_Init();
    Dump_Exception_Info(u, e_info);
    g_ExceptLogFile.Write(g_ExceptionFileBuffer, strlen(g_ExceptionFileBuffer));
    g_ExceptLogFile.Close();

    const char *caption = Text_String(TXT_CHRONOSHIFT);

    MessageBoxA(NULL,
        "The application encountered an internal error\nand is unable to continue normally.\n\n"
        "Please show the except.txt to a developer\n",
        caption != nullptr ? caption : "Application",
        MB_OK | MB_ICONERROR);

    Emergency_Exit(1);

    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

#ifdef COMPILER_WATCOM
// wrapper for watcoms own format
int __cdecl Watcom_Exception_Handler(EXCEPTION_RECORD *ex, void *establisher_frame, CONTEXT *context, void *dispatch_context)
{
    EXCEPTION_POINTERS rec;
    rec.ExceptionRecord = ex;
    rec.ContextRecord = context;

    return Exception_Handler(0, &rec);
}
#endif
