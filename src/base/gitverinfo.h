/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Globals for accessing git version information from the build system.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef GITVERINFO_H
#define GITVERINFO_H

#include <time.h>

extern char g_GitBranch[];
extern char g_GitSHA1[];
extern char g_GitShortSHA1[];
extern char g_GitCommitTime[];
extern char g_GitCommitDate[];
extern char g_GitCommitAuthor[];
extern unsigned g_GitCommitCount;
extern time_t g_GitCommitTimeStamp;
extern unsigned g_MajorVersion;
extern unsigned g_MinorVersion;

#endif // GITVERINFO_H
