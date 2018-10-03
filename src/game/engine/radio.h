/**
 * @file
 *
 * @author CCHyper
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
#include "smartptr.h"

class TechnoClass;
class FootClass;

// A invalid target instance parsed into Transmit_Message() as a default param.
extern target_t NullRadioTarget;

class RadioClass : public MissionClass
{
public:
    RadioClass(RTTIType type, int id);
    RadioClass(RadioClass const &that);
    RadioClass(NoInitClass const &noinit) : MissionClass(noinit) {}
    virtual ~RadioClass();

    virtual BOOL Limbo() override;
    virtual RadioMessageType Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target) override;
    virtual void Code_Pointers() override;
    virtual void Decode_Pointers() override;
    virtual RadioMessageType Transmit_Message(
        RadioMessageType message, target_t &target = NullRadioTarget, RadioClass *radio = nullptr);
    virtual RadioMessageType Transmit_Message(RadioMessageType message, RadioClass *radio);

    // TODO, rename!
    bool const Radio_Valid() const { return Radio != nullptr; }

    TechnoClass *Radio_As_Techno() const
    {
        if (Radio_Valid()) {
            return (TechnoClass *)&Radio;
        }
        return nullptr;
    }

    FootClass *Radio_As_Foot() const
    {
        if (Radio_Valid()) {
            return (FootClass *)&Radio;
        }
        return nullptr;
    }

    static const char *Message_From(RadioMessageType message);
    static RadioMessageType From_Message(const char *message);

private:
    static const char *Messages[RADIO_COUNT];

public:
    RadioMessageType ReceivedMessage; // 0x37	//name subject to change!
    RadioMessageType TransmittedMessage; // 0x38	//name subject to change!
    RadioMessageType LastMessage; // 0x39
    SmartPtr<ObjectClass> Radio; // 0x3A
};

#endif // RADIO_H
