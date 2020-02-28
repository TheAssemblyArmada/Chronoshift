#pragma once

#ifndef ATTRACT_H
#define ATTRACT_H

#include "always.h"

class FileClass;

void Do_Record_Playback();

BOOL Save_Recording_Values(FileClass &file);
BOOL Load_Recording_Values(FileClass &file);

#endif // ATTRACT_H
