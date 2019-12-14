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
#include "radio.h"
#include "target.h"

// A invalid target instance parsed into targets as a default param.
static target_t LParam = 0;

target_t &RadioClass::Get_LParam()
{
    return LParam;
}

const char *RadioClass::s_Messages[RADIO_COUNT] = {
    "static (no message)",
    "Roger.",
    "Come in.",
    "Over and out.",
    "Requesting transport.",
    "Attach to transport.",
    "I've got a delivery for you.",
    "I'm performing load/unload maneuver. Be careful.",
    "I'm clear.",
    "You are clear to unload. Driving away now.",
    "Am unable to comply.",
    "I'm starting construction now... act busy.",
    "I've finished construction. You are free.",
    "We bumped, redraw yourself please.",
    "I'm trying to load up now.",
    "May I become a passenger?",
    "Are you ready to receive shipment?",
    "Are you trying to become a passenger?",
    "Move to location X.",
    "Do you need to move?",
    "All right already. Now what?",
    "I'm a passenger now.",
    "Backup into refinery now.",
    "Run away!",
    "Tether established.",
    "Tether broken.",
    "Repair one step.",
    "Are you prepared to fight?",
    "Attack this target please.",
    "Reload one step.",
    "Circumstances prevent success.",
    "All done with the request.",
    "Do you need service depot work?",
    "Are you sitting on service depot?"
};

RadioClass::RadioClass(RTTIType type, int id) :
    MissionClass(type, id),
    /*m_ReceivedMessage(RADIO_STATIC),
    m_TransmittedMessage(RADIO_STATIC),
    m_LastMessage(RADIO_STATIC),*/
    //m_MessageHistory(), // TODO: When fully standalone, uncomment this line and remove the 'for' loop below.
    m_Radio(nullptr)
{
    for (int i = 0; i < ARRAY_SIZE(m_MessageHistory); ++i){
        m_MessageHistory[i] = RADIO_STATIC;
    }
}

RadioClass::RadioClass(const RadioClass &that) :
    MissionClass(that),
    /*m_ReceivedMessage(that.m_ReceivedMessage),
    m_TransmittedMessage(that.m_TransmittedMessage),
    m_LastMessage(that.m_LastMessage),*/
    m_Radio(that.m_Radio)
{
}

RadioClass::~RadioClass()
{
    m_Radio = nullptr;
}

#ifdef CHRONOSHIFT_DEBUG
void RadioClass::Debug_Dump(MonoClass *mono) const
{
    MissionClass::Debug_Dump(mono);
}
#endif

BOOL RadioClass::Limbo()
{
    if (!m_InLimbo) {
        Transmit_Message(RADIO_OVER_AND_OUT);
    }

    return ObjectClass::Limbo();
}

RadioMessageType RadioClass::Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target)
{
    DEBUG_ASSERT(radio != nullptr);
    DEBUG_ASSERT(message != RADIO_NONE);
    DEBUG_ASSERT(message < RADIO_COUNT);

#ifdef GAME_DLL
    RadioMessageType (*func)(RadioClass *, RadioMessageType, target_t &) =
        reinterpret_cast<RadioMessageType (*)(RadioClass *, RadioMessageType, target_t &)>(0x00532A70);
    return func(radio, message, target);
#elif 0 // TODO Needs HouseClass.

    // Basic circular tracker of received message history.
    if (message != m_MessageHistory[0]) {
        m_MessageHistory[2] = m_MessageHistory[1];
        m_MessageHistory[1] = m_MessageHistory[0];
        m_MessageHistory[0] = message;
    }

    if (m_Radio != radio || message != RADIO_OVER_AND_OUT) {
        if (message == RADIO_HELLO && m_Health > 0) {
            if ((m_Radio == nullptr || m_Radio == radio) && radio != nullptr) {
                TechnoClass *t_radioptr = reinterpret_cast<TechnoClass *>(radio);
                TechnoClass *t_this = reinterpret_cast<TechnoClass *>(this);

                if (t_radioptr != nullptr && t_this != nullptr) {
                    if (t_radioptr->OwnerHouse->Is_Ally(this) && t_this->OwnerHouse->Is_Ally(t_radioptr)) {
                        m_Radio = radio;
                        return RADIO_ROGER;
                    }
                }
            }

            return RADIO_UNABLE_TO_COMPLY;
        }

        return ObjectClass::Receive_Message(radio, RADIO_OVER_AND_OUT, target);
    }

    ObjectClass::Receive_Message(radio, RADIO_OVER_AND_OUT, target);

    m_Radio = nullptr;

    return RADIO_ROGER;
#else
    return RADIO_ROGER;
#endif
}

RadioMessageType RadioClass::Transmit_Message(RadioMessageType message, target_t &target, RadioClass *radio)
{
    DEBUG_ASSERT(message != RADIO_NONE);
    DEBUG_ASSERT(message < RADIO_COUNT);

    RadioClass *radioptr = radio == nullptr ? reinterpret_cast<RadioClass *>(m_Radio) : radio;

    if (radioptr == nullptr) {
        return RADIO_STATIC;
    }

    if (m_Radio == radioptr && message == RADIO_OVER_AND_OUT) {
        m_Radio = nullptr;
    }

    if (message != RADIO_HELLO) {
        return radioptr->Receive_Message(this, message, target);
    }

    Transmit_Message(RADIO_OVER_AND_OUT);

    if (radioptr->Receive_Message(this, message, target) == RADIO_ROGER) {
        m_Radio = radioptr;
        return RADIO_ROGER;
    }

    return RADIO_UNABLE_TO_COMPLY;
}

RadioMessageType RadioClass::Transmit_Message(RadioMessageType message, RadioClass *radio)
{
    // DEBUG_ASSERT(radio != nullptr);
    DEBUG_ASSERT(message != RADIO_NONE);
    DEBUG_ASSERT(message < RADIO_COUNT);

    return Transmit_Message(message, Get_LParam(), radio);
}

void RadioClass::Code_Pointers()
{
    if (m_Radio != nullptr) {
        m_Radio = reinterpret_cast<ObjectClass *>(m_Radio->As_Target());
    }

    MissionClass::Code_Pointers();
}

void RadioClass::Decode_Pointers()
{
    if (m_Radio != nullptr) {
        m_Radio = reinterpret_cast<ObjectClass *>(As_Techno((uintptr_t)(m_Radio)));
        DEBUG_ASSERT(m_Radio != nullptr);
    }

    MissionClass::Decode_Pointers();
}

const char *RadioClass::Message_From(RadioMessageType message)
{
    // DEBUG_ASSERT(message != RADIO_NONE);
    DEBUG_ASSERT(message < RADIO_COUNT);

    if (message != RADIO_NONE && message < RADIO_COUNT) {
        return s_Messages[message];
    }
    return "Invalid message";
}

RadioMessageType RadioClass::From_Message(const char *message)
{
    DEBUG_ASSERT(message != nullptr);

    if (strcasecmp(message, "Invalid message") == 0 || strcasecmp(message, "<none>") == 0
        || strcasecmp(message, "none") == 0) {
        return RADIO_NONE;
    }

    if (message != nullptr) {
        for (RadioMessageType type = RADIO_FIRST; type < RADIO_COUNT; ++type) {
            if (strcasecmp(message, Message_From(type)) == 0) {
                return type;
            }
        }
    }
    return RADIO_NONE;
}
