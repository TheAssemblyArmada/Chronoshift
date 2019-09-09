/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief GUI for settings options in OptionsClass.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef GAMEOPTIONS_H
#define GAMEOPTIONS_H

#include "always.h"
#include "options.h"

class GameOptionsClass : public OptionsClass
{
public:
    void Process();
    void Adjust_Vars_For_Resolution();

private:
    int OptionsDialogWidth;
    int OptionsDialogHeight;
    int OptionsDialogCenterX;
    int OptionsDialogCenterY;
    int field_8C;
    int OptionsDialogButtonYSpacing;
    int field_94;
    int OptionsDialogButtonOffsetFromTop;
    int field_9C;
    int field_A0;
    int OptionsDialogBottomButtonOffsetTop;
};

#ifdef GAME_DLL
#include "hooker.h"

extern GameOptionsClass &Options;
#else
extern GameOptionsClass Options;
#endif

#endif // GAMEOPTIONS_H
