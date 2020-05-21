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

#ifndef GAME_DLL
GameOptionsClass g_Options;
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
    void (*process)(const GameOptionsClass *) = reinterpret_cast<void (*)(const GameOptionsClass *)>(0x004C9EE0);
    process(this);
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
    m_OptionsDialogWidth = 448;
    m_OptionsDialogHeight = 222;
    m_OptionsDialogCenterX = (g_SeenBuff.Get_Width() - m_OptionsDialogWidth) / 2;
    m_OptionsDialogCenterY = (g_SeenBuff.Get_Height() - m_OptionsDialogHeight) / 2;
    m_field_8C = 260;
    m_OptionsDialogButtonYSpacing = 18;
    m_field_94 = 10;
    m_OptionsDialogButtonOffsetFromTop = 42;
    m_field_9C = 144;
    m_field_A0 = 32;
    m_OptionsDialogBottomButtonOffsetTop = m_OptionsDialogHeight - 38;
}
