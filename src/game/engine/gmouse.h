/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling mouse input.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
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
    GameMouseClass(const NoInitClass &noinit) : ScrollClass(noinit) {}

    virtual void One_Time() override;
    virtual void Init_Clear() override;
    virtual void AI(KeyNumType &key, int mouse_x, int mouse_y) override;
    virtual void Set_Default_Mouse(MouseType mouse, BOOL in_radar = false) override;
    virtual BOOL Override_Mouse_Shape(MouseType mouse, BOOL in_radar = false) override;
    virtual void Revert_Mouse_Shape() override { Override_Mouse_Shape(m_CurrentMouse); }
    virtual void Mouse_Small(BOOL use_small_frame) override;
    virtual MouseType Get_Mouse_Shape() const override { return m_DefaultMouse; }
    virtual BOOL Load(Straw &straw) override;
    virtual BOOL Save(Pipe &pipe) const override;

    BOOL Mouse_In_Radar() const { return m_MouseInRadar; }

#ifdef GAME_DLL
    BOOL Hook_Save(Pipe &pipe) { return GameMouseClass::Save(pipe); }
#endif

private:
    void *Get_Shape_File_For_Mouse(MouseType type) const;

protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_MouseInRadar : 1; // 1 Is the mouse in the radar map?
#else
    bool m_MouseInRadar; // Is the mouse in the radar map?
#endif
    MouseType m_CurrentMouse;
    MouseType m_DefaultMouse;
    unsigned m_MouseFrame; // this is the current frame index for the animated mouse.

private:
#ifdef GAME_DLL
    static void *&s_MouseShapes;
    static TCountDownTimerClass<SystemTimerClass> &s_AnimationTimer;
#else
    static void *s_MouseShapes;
    static TCountDownTimerClass<SystemTimerClass> s_AnimationTimer;
#endif
    static void *s_EditorMouseShapes;
    static MouseStruct s_MouseControl[MOUSE_COUNT];
};

#endif // GMOUSE_H
