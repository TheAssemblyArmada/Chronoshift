/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class for representing a list of "checkable" items the represent BOOLean states.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "checklist.h"
#include "gbuffer.h"
#include <stdio.h>

CheckListClass::CheckListClass(
    unsigned id, int x, int y, int w, int h, TextPrintType style, void *up_btn_shape, void *down_btn_shape) :
    ListClass(id, x, y, w, h, style, up_btn_shape, down_btn_shape),
    IgnoreInput(0)
{
}

CheckListClass::CheckListClass(CheckListClass &that) : ListClass(that), IgnoreInput(that.IgnoreInput) {}

CheckListClass::~CheckListClass() {}

int CheckListClass::Add_Item(const char *string)
{
    DEBUG_ASSERT(string != nullptr);

    CheckListItemClass *checkitem = new CheckListItemClass(string, false);

    // Bit of casting tomfoolery here to store the listitem pointer in ListClass' Entries vector. Relies on pointers to
    // structs being the same as pointers to strings size wise which it is on all platforms we care about.
    return ListClass::Add_Item((char *)checkitem);
}

int CheckListClass::Add_Item(int string_index)
{
    return ListClass::Add_Item(string_index);
}

const char *CheckListClass::Current_Item() const
{
    CheckListItemClass *checkitem = (CheckListItemClass *)ListClass::Current_Item();

    if (checkitem) {
        return checkitem->ItemString;
    }

    return nullptr;
}

const char *CheckListClass::Get_Item(int index) const
{
    CheckListItemClass *checkitem = (CheckListItemClass *)ListClass::Get_Item(index);

    if (checkitem) {
        return checkitem->ItemString;
    }

    return nullptr;
}

void CheckListClass::Remove_Item(int string_index)
{
    ListClass::Remove_Item(string_index);
}

void CheckListClass::Remove_Item(const char *string)
{
    for (int i = 0; i < Count(); ++i) {
        CheckListItemClass *checkitem = (CheckListItemClass *)ListClass::Get_Item(i);

        if (checkitem != nullptr && strcasecmp(checkitem->ItemString, string) == 0) {
            ListClass::Remove_Item(i);
            delete checkitem;

            return;
        }
    }
}

void CheckListClass::Set_Selected_Index(const char *string)
{
    for (int i = 0; i < Count(); ++i) {
        CheckListItemClass *checkitem = (CheckListItemClass *)ListClass::Get_Item(i);

        if (checkitem != nullptr && strcasecmp(checkitem->ItemString, string) == 0) {
            Set_Selected_Index(i);

            return;
        }
    }
}

void CheckListClass::Check_Item(int index, BOOL check_state)
{
    CheckListItemClass *checkitem = (CheckListItemClass *)ListClass::Get_Item(index);
    
    if (checkitem) {
        if (check_state != checkitem->IsChecked) {
            checkitem->IsChecked = check_state;
            Flag_To_Redraw();
        }
    }
}

BOOL CheckListClass::Is_Checked(int index) const
{
    CheckListItemClass *checkitem = (CheckListItemClass *)ListClass::Get_Item(index);

    if (checkitem) {
        return checkitem->IsChecked;
    }

    return false;
}

BOOL CheckListClass::Action(unsigned flags, KeyNumType &key)
{
    if (IgnoreInput) {
        return false;
    }

    BOOL retval = ListClass::Action(flags, key);

    if (flags & MOUSE_LEFT_PRESS) {
        BOOL state = CheckListClass::Is_Checked(CurrentIndex);
        CheckListClass::Check_Item(CurrentIndex, state == 0);
    }

    return retval;
}

void CheckListClass::Draw_Entry(int index, int x, int y, int x_max, BOOL redraw)
{
    char stringbuff[100];
    TextPrintType style;

    if (index < Count()) {
        CheckListItemClass *checkitem = (CheckListItemClass *)ListClass::Get_Item(index);

        if (checkitem) {
            memset(stringbuff, 0, sizeof(stringbuff));

            if (checkitem->IsChecked) {
                stringbuff[0] = '\x03'; // This is a tick in some WW fonts
            } else {
                stringbuff[0] = ' '; // A space
            }

            stringbuff[1] = ' '; // Another space
            snprintf(&stringbuff[2], sizeof(stringbuff) - 2, "%s", checkitem->ItemString);
            style = TextStyle;

            if (redraw) {
                style |= TPF_USE_BRIGHT;
                g_logicPage->Fill_Rect(x, y, x_max + x - 1, YSpacing + y - 1, GadgetClass::ColorScheme->WindowPalette[0]);
            } else if (!(style & TPF_USE_GRAD_PAL)) {
                style |= TPF_USE_MEDIUM;
            }

            Conquer_Clip_Text_Print(stringbuff, x, y, GadgetClass::ColorScheme, 0, style, x_max, Tabs);
        }
    }
}

void CheckListClass::Set_Selected_Index(int string_index)
{
    return ListClass::Set_Selected_Index(string_index);
}
