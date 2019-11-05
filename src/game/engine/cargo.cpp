/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Handles tracking units currently stored as cargo.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "cargo.h"
#include "gamedebug.h"
#include "foot.h"
#include "target.h"

/**
 * @brief
 *
 * @address 0x004623D0
 */
void CargoClass::Attach(FootClass *object)
{
    if (object != nullptr) {

        object->Limbo();
        ObjectClass *obj = nullptr;
        for (obj = object->Get_Next(); obj != nullptr && obj->Get_Next(); obj = obj->Get_Next());
        if (obj != nullptr) {
            obj->Set_Next(m_Object);
        } else {
            object->Set_Next(m_Object);
        }

        m_Object = object;
        m_Count = 0;
        for (obj = object; obj != nullptr; obj = obj->Get_Next()) {
            ++m_Count;
        }
    }
}

/**
 * @brief
 *
 * @address 0x0046247C
 */
FootClass *CargoClass::Attached_Object() const
{
    if (Has_Cargo()) {
        return m_Object;
    }

    return nullptr;
}

/**
 * @brief
 *
 * @address 0x00462448
 */
FootClass *CargoClass::Detach_Object()
{
    FootClass *object = Attached_Object();
    if (object != nullptr) {
        m_Object = reinterpret_cast<FootClass *>(object->Get_Next());
        object->Set_Next(nullptr);
        --m_Count;
        return object;
    }
    return nullptr;
}

/**
 * @brief
 *
 * @address 0x004F9884
 */
void CargoClass::Code_Pointers()
{
    if (m_Object != nullptr) {
        m_Object = reinterpret_cast<FootClass *>(As_Target(m_Object));
    }
}

/**
 * @brief
 *
 * @address 0x004F98C0
 */
void CargoClass::Decode_Pointers()
{
    if (m_Object != nullptr) {
        m_Object = reinterpret_cast<FootClass *>(As_Techno((uintptr_t)m_Object));
    }
}
