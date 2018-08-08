/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base class for toggleable UI elements.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef TOGGLE_H
#define TOGGLE_H

#include "always.h"
#include "controlc.h"

class ToggleClass : public ControlClass
{
public:
    ToggleClass(unsigned id, int x, int y, int w, int h);
    ToggleClass(ToggleClass &that);
    virtual ~ToggleClass() {}

    ToggleClass &operator=(ToggleClass &that);

    virtual BOOL Action(unsigned flags, KeyNumType &key) override;

    void Turn_On();
    void Turn_Off();

    void Set_Toggle_Bool1(BOOL unk) { Toggle_Boolean1 = unk; }
    void Set_Toggle_Disabled(BOOL disabled) { ToggleDisabled = disabled; }

#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
#endif

protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool Toggle_Boolean1 : 1; // & 1
            bool ToggleState : 1; // & 2
            bool ToggleDisabled : 1; // & 4
        };
        int m_toggleFlags;
    };
#else
    bool Toggle_Boolean1;
    bool ToggleState;
    bool ToggleDisabled;
#endif
};

inline ToggleClass &ToggleClass::operator=(ToggleClass &that)
{
    if (this != &that) {
        ControlClass::operator=(that);
        Toggle_Boolean1 = that.Toggle_Boolean1;
        ToggleState = that.ToggleState;
        ToggleDisabled = that.ToggleDisabled;
    }

    return *this;
}

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"

inline void ToggleClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x0056C4C8, *ToggleClass::Action);
#endif
}
#endif

#endif // TOGGLE_H
