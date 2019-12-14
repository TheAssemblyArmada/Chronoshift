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
    int m_OptionsDialogWidth;
    int m_OptionsDialogHeight;
    int m_OptionsDialogCenterX;
    int m_OptionsDialogCenterY;
    int m_field_8C;
    int m_OptionsDialogButtonYSpacing;
    int m_field_94;
    int m_OptionsDialogButtonOffsetFromTop;
    int m_field_9C;
    int m_field_A0;
    int m_OptionsDialogBottomButtonOffsetTop;
};

#ifdef GAME_DLL
#include "hooker.h"

extern GameOptionsClass &g_Options;
#else
extern GameOptionsClass g_Options;
#endif

#endif // GAMEOPTIONS_H
