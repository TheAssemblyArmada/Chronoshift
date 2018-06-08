/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling mouse input.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef GMOUSE_H
#define GMOUSE_H

#include "always.h"
#include "scroll.h"

class GameMouseClass : public ScrollClass
{
private:
    struct MouseStruct
    {
        int Frame;
        int Count;
        int Rate; // Rate of animation, value of -1 means that it does not animate and has only 1 frame.
        int Small; // -1 means no mini frame
        int HotSpotX; // x pixel of image for the hotspot
        int HotSpotY; // y pixel of image for the hotspot
    };

public:
    GameMouseClass();
    GameMouseClass(NoInitClass &noinit) : ScrollClass(noinit) {}

    virtual void One_Time() override;
    virtual void Init_Clear() override;
    virtual void AI(KeyNumType &key, int mouse_x, int mouse_y) override;
    virtual void Set_Default_Mouse(MouseType mouse, BOOL in_radar = false) override;
    virtual BOOL Override_Mouse_Shape(MouseType mouse, BOOL in_radar = false) override;
    virtual void Revert_Mouse_Shape() override { Override_Mouse_Shape(PreviousMouseShape); }
    virtual void Mouse_Small(BOOL use_small_frame) override;
    virtual MouseType Get_Mouse_Shape() const override { return MouseShape; }
    virtual BOOL Load(Straw &straw) override;
    virtual BOOL Save(Pipe &pipe) const override;

#ifndef RAPP_STANDALONE
    static void Hook_Me();
#endif

protected:
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool MouseInRadar : 1; // 1 InRadar? RadarMouse?, seems there is only one bool here, TS only has one too.
        };
        int Bitfield;
    };
#else
    bool MouseInRadar;
#endif
    MouseType PreviousMouseShape;
    MouseType MouseShape;
    unsigned MouseFrame; // this is the current frame index for the animated mouse.

private:
#ifndef RAPP_STANDALONE
    static void *&MouseShapes;
    static TCountDownTimerClass<SystemTimerClass> &AnimationTimer;
#else
    static void *MouseShapes;
    static TCountDownTimerClass<SystemTimerClass> AnimationTimer;
#endif
    static MouseStruct MouseControl[MOUSE_COUNT];
};

#ifndef RAPP_STANALONE
#include "hooker.h"
inline void GameMouseClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    // Hook_Function(0x, *GameMouseClass::Init_Clear);
#endif
}
#endif

#endif // GMOUSE_H
