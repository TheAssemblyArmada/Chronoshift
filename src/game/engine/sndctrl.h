/**
 * @file
 *
 * @author tomsons26
 *
 * @brief
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef SNDCTRL_H
#define SNDCTRL_H

class SoundControlsClass
{
public:
    SoundControlsClass() {}
    void Adjust_Variables_For_Resolution();
    void Process();
};
#endif //SNDCTRL_H
