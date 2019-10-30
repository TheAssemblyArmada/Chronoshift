/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Message box class to elicit input from the user.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef MSGBOX_H
#define MSGBOX_H

#include "always.h"
#include "language.h"

// TODO: This needs refactoring
enum MsgBoxReturnType
{
    ID_BUTTON_1,
    ID_BUTTON_2,
    ID_BUTTON_3,

    MSGBOX_IDFAIL = 0,
    MSGBOX_IDOK = 1,
    MSGBOX_IDCANCEL = 2,
    MSGBOX_IDABORT = 3,
    MSGBOX_IDRETRY = 4,
    MSGBOX_IDIGNORE = 5,
    MSGBOX_IDYES = 6,
    MSGBOX_IDNO = 7,
};

class MessageBoxClass
{
public:
    MessageBoxClass(int caption = TXT_NULL) : m_CaptionText(caption) {}
    ~MessageBoxClass() {}

    int Process(const int body_text, const int button_1_text = TXT_OK, const int button_2_text = TXT_NULL,
        const int button_3_text = TXT_NULL, BOOL shadow_seen = false);
    int Process(const char *body_text, const int button_1_text = TXT_OK, const int button_2_text = TXT_NULL,
        const int button_3_text = TXT_NULL, BOOL shadow_seen = false);
    int Process(const char *body_text, const char *button_1_text, const char *button_2_text, const char *button_3_text = nullptr,
        BOOL shadow_seen = false);

    static int Simple_OK_Message_Box(const int body_text, const int button_1_text = TXT_OK);
    static int Simple_OK_Message_Box(const char *body_text, const int button_1_text = TXT_OK);
    static int Simple_YesNo_Message_Box(const int body_text, const int button_1_text = TXT_YES, const int button_2_text = TXT_NO);
    static int Simple_YesNo_Message_Box(const char *body_text, const int button_1_text = TXT_YES, const int button_2_text = TXT_NO);

#ifdef GAME_DLL
    int Hook_Process(const char *body_text, const char *button_1_text, const char *button_2_text, const char *button_3_text,
        BOOL shadow_seen = false)
    {
        return Process(body_text, button_1_text, button_2_text, button_3_text, shadow_seen);
    }
#endif

private:
    int m_CaptionText;
};

#endif // MSGBOX_H
