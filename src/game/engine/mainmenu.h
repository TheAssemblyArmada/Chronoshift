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
#pragma once

#ifndef MAINMENU_H
#define MAINMENU_H

#include "always.h"

enum MenuButtonType
{
    MENU_COUNTERSTRIKE,
    MENU_AFTERMATH,
    MENU_NEW_GAME,
    MENU_LOAD_GAME,
    MENU_MPLAYER_GAME,
    //MENU_SNEAK_PEAK,
    MENU_MOVIES,
    MENU_MAP_EDITOR,
    MENU_OPTIONS,
    MENU_EXIT,
    
    MENU_COUNT,

    MENU_RESET = -2, // Special flag to indicate a full reset of the main menu.
    MENU_NONE = -1,
};

DEFINE_ENUMERATION_OPERATORS(MenuButtonType);

enum {
    DEFAULT_ATTRACTION_DELAY = 3600
};

class GraphicViewPortClass;
class PaletteClass;

void Load_Title_Page();
void Load_Standby_Page();
void Load_Prolog_Page();
void Load_Title_Screen(const char *filename, GraphicViewPortClass *vp, PaletteClass *pal);

BOOL Select_Game(BOOL fade = false);
MenuButtonType Main_Menu(int attract_delay = DEFAULT_ATTRACTION_DELAY);

#endif // MAINMENU_H
