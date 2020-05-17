/**
 * @file
 *
 * @author CCHyper
 * @author tomsons26
 *
 * @brief Object communication layer of object class hierachy.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef RADIO_H
#define RADIO_H

#include "always.h"
#include "gametypes.h"
#include "mission.h"

class TechnoClass;
class FootClass;

class RadioClass : public MissionClass
{
public:
    RadioClass(RTTIType type, int id);
    RadioClass(const RadioClass &that);
    RadioClass(const NoInitClass &noinit) : MissionClass(noinit) {}
    virtual ~RadioClass();

#ifdef CHRONOSHIFT_DEBUG
#ifdef CHRONOSHIFT_STANDALONE
    virtual void Debug_Dump(MonoClass *mono) const override;
#else
    void Debug_Dump(MonoClass *mono) const;
#endif
#endif

    virtual BOOL Limbo() override;
    virtual RadioMessageType Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target) override;
    virtual void Code_Pointers() override;
    virtual void Decode_Pointers() override;
    virtual RadioMessageType Transmit_Message(
        RadioMessageType message, target_t &target = RadioClass::Get_LParam(), RadioClass *radio = nullptr);
    virtual RadioMessageType Transmit_Message(RadioMessageType message, RadioClass *radio);

    void Set_Contact(RadioClass *radio) { m_Radio = radio; }
    BOOL In_Contact() const { return m_Radio != nullptr; }
    void Break_Contact() { m_Radio = nullptr; }
    TechnoClass *Contact_With_Whom() const { return (TechnoClass *)m_Radio; }

    static target_t &Get_LParam();

    static const char *Message_From(RadioMessageType message);
    static RadioMessageType From_Message(const char *message);

private:
    static const char *s_Messages[RADIO_COUNT];

protected:
    RadioMessageType m_MessageHistory[3];
    RadioClass *m_Radio;
};

#endif // RADIO_H
