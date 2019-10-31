/**
 * @file
 *
 * @author CCHyper
 *
 * @brief Class holding information on static house info.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef HOUSESTATIC_H
#define HOUSESTATIC_H

#include "always.h"
#include "gametypes.h"
#include "heap.h"
#include "source.h"

class GameINIClass;

class HouseStaticClass
{
public:
    HouseStaticClass();
    HouseStaticClass(const HouseStaticClass &that);
    HouseStaticClass(const NoInitClass &noinit) {}
    ~HouseStaticClass() {}

    bool Read_INI(GameINIClass &ini, const char *section);
    void Write_INI(GameINIClass &ini, const char *section);

    int Get_IQ() const { return m_IQ; }
    int Get_TechLevel() const { return m_TechLevel; }
    int32_t Get_Allies() const { return m_Allies; }
    int Get_MaxUnit() const { return m_MaxUnit; }
    int Get_MaxBuilding() const { return m_MaxBuilding; }
    int Get_MaxInfantry() const { return m_MaxInfantry; }
    int Get_MaxVessel() const { return m_MaxVessel; }
    int Get_MaxAircraft() const { return m_MaxAircraft; }
    int Get_Credits() const { return m_Credits; }
    SourceType Get_Edge() const { return m_Edge; }
    void Add_Ally(HousesType house) { m_Allies |= 1 << house; }
    void Remove_Ally(HousesType house) { m_Allies &= ~(1 << house); }
    void Set_Credits(int credits) { m_Credits = credits; }

private:
    // The initial IQ for this house.
    int m_IQ;

    // The tech level of this house, which in turn defines which techno
    // objects the side may produce. Any object with a tech level higher than
    // this value will not be available to the house.
    int m_TechLevel;

    // A a bit list of sides this house is allied with.
    // (This only means the side can not attack these sides, not the other way
    // around unless they both define each other as allies!)
    // NOTE: This only means that this house is allied with the houses set here.
    uint32_t m_Allies;

    // Maximum number of vessels this house can produce.
    int m_MaxUnit;

    // Maximum number of buildings this house can produce.
    int m_MaxBuilding;

    // Maximum number of infantry this house can produce.
    int m_MaxInfantry;

    // Maximum number of vessels this house can produce.
    int m_MaxVessel;

    // Maximum number of aircraft this house can produce.
    int m_MaxAircraft;

    // Defines the inital amount of credits this house starts with.
    // NOTE: Multiplied by 100 [a value of 10 would mean 1000 credits].
    int m_Credits;

    // The closest edge of the map from which reinforcment triggers, superweapons
    // and other possible actions start from. [see SourceType enum].
    SourceType m_Edge;
};

#endif // HOUSESTATIC_H
