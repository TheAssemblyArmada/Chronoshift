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
#include "gameoptions.h"
#include "gbuffer.h"

#ifdef GAME_DLL
GameOptionsClass &Options = Make_Global<GameOptionsClass>(0x00668188);
#else
GameOptionsClass Options;
#endif

/**
 * @brief Performs processing for this object per frame.
 *
 * 0x004C9EE0
 */
void GameOptionsClass::Process()
{
    // TODO
#ifdef GAME_DLL
    void (*process)() = reinterpret_cast<void (*)()>(0x004C9EE0);
    process();
#endif
}

/**
 * @brief Adjusts the values of variables based on current resolution.
 *
 * 0x004CAC6C
 */
void GameOptionsClass::Adjust_Vars_For_Resolution()
{
    // TODO work out what these are all used for if anything.
    OptionsDialogWidth = 448;
    OptionsDialogHeight = 222;
    OptionsDialogCenterX = (g_seenBuff.Get_Width() - OptionsDialogWidth) / 2;
    OptionsDialogCenterY = (g_seenBuff.Get_Height() - OptionsDialogHeight) / 2;
    field_8C = 260;
    OptionsDialogButtonYSpacing = 18;
    field_94 = 10;
    OptionsDialogButtonOffsetFromTop = 42;
    field_9C = 144;
    field_A0 = 32;
    OptionsDialogBottomButtonOffsetTop = OptionsDialogHeight - 38;
}
