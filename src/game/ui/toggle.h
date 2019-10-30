/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base class for toggleable UI elements.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
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

    void Set_Toggle_Bool1(BOOL unk) { m_Toggle_Boolean1 = unk; }
    void Set_Toggle_Disabled(BOOL disabled) { m_ToggleDisabled = disabled; }

    bool Get_Toggle_State() { return m_ToggleState; }

protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_Toggle_Boolean1 : 1; // & 1
    BOOL m_ToggleState : 1; // & 2
    BOOL m_ToggleDisabled : 1; // & 4
#else
    bool m_Toggle_Boolean1;
    bool m_ToggleState;
    bool m_ToggleDisabled;
#endif
};

inline ToggleClass &ToggleClass::operator=(ToggleClass &that)
{
    if (this != &that) {
        ControlClass::operator=(that);
        m_Toggle_Boolean1 = that.m_Toggle_Boolean1;
        m_ToggleState = that.m_ToggleState;
        m_ToggleDisabled = that.m_ToggleDisabled;
    }

    return *this;
}

#endif // TOGGLE_H
