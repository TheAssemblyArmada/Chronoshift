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
#pragma once

#ifndef CHECKLIST_H
#define CHECKLIST_H

#include "always.h"
#include "list.h"
#include "stringex.h"

class CheckListClass : public ListClass
{
public:
    class CheckListItemClass
    {
    public:
        CheckListItemClass(const char *string, BOOL state = false) : ItemString(string), IsChecked(state) {}

        const char *ItemString;
        BOOL IsChecked;
    };

public:
    CheckListClass(
        unsigned id, int x, int y, int w, int h, TextPrintType style, void *up_btn_shape, void *down_btn_shape);
    CheckListClass(CheckListClass &that);
    virtual ~CheckListClass();

    virtual BOOL Action(unsigned flags, KeyNumType &key) override;
    virtual int Add_Item(const char *string) override;
    virtual int Add_Item(int string_index) override;
    virtual const char *Current_Item() const override;
    virtual const char *Get_Item(int index) const override;
    //virtual int Step_Selected_Index(int index);
    virtual void Remove_Item(const char *string) override;
    virtual void Remove_Item(int string_index) override;
    virtual void Set_Selected_Index(int string_index) override;
    virtual void Set_Selected_Index(const char *string) override;
    virtual void Draw_Entry(int index, int x, int y, int x_max, BOOL redraw) override;

    CheckListClass &operator=(CheckListClass &that);

    void Check_Item(int index, BOOL check_state);
    BOOL Is_Checked(int index) const;

protected:
    BOOL IgnoreInput;
};

inline CheckListClass &CheckListClass::operator=(CheckListClass &that)
{
    if (this != &that) {
        ListClass::operator=(that);
        IgnoreInput = that.IgnoreInput;
    }

    return *this;
}

#endif // CHECKLIST_H
