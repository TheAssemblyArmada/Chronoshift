/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Possibly meant to handle experience, veterancy, and kill count tracking.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */

#pragma once

#ifndef CREW_H
#define CREW_H

#include "noinit.h"

class CrewClass
{
public:
    CrewClass() : m_Kills(0) {}
    CrewClass(const NoInitClass &init) {}
    ~CrewClass() {}

    unsigned short Register_Kill() { return ++m_Kills; }

private:
    unsigned short m_Kills;
};

#endif // CREW_H
