/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 * @author tomsons26
 *
 * @brief Engine initialisation functions.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef INIT_H
#define INIT_H

#include "always.h"
#include "ccfileclass.h"
#include "mixfile.h"

void Init_Expansion_Files();
void Init_Bootstrap_Mixfiles();
void Init_Secondary_Mixfiles();
//void Init_Bulk_Data();
void Init_Keys();
void Init_Fonts();

#ifndef CHRONOSHIFT_STANDALONE
extern MixFileClass<CCFileClass> *&MainMix;
extern MixFileClass<CCFileClass> *&ConquerMix;
extern MixFileClass<CCFileClass> *&GeneralMix;
extern MixFileClass<CCFileClass> *&MoviesMix;
extern MixFileClass<CCFileClass> *&ScoreMix;
#else
extern MixFileClass<CCFileClass> *MainMix;
extern MixFileClass<CCFileClass> *ConquerMix;
extern MixFileClass<CCFileClass> *GeneralMix;
extern MixFileClass<CCFileClass> *MoviesMix;
extern MixFileClass<CCFileClass> *ScoreMix;
#endif

#endif // INIT_H
