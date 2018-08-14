/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Message box class to elicit input from the user.
 *
 * @copyright RedAlert++ is free software: you can redistribute it and/or
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

enum MessageBoxReturn
{
    MSGBOX_IDFAIL = 0,
    MSGBOX_IDOK = 1,
    MSGBOX_IDCANCEL = 2,
    MSGBOX_IDABORT = 3,
    MSGBOX_IDRETRY = 4,
    MSGBOX_IDIGNORE = 5,
    MSGBOX_IDYES = 6,
    MSGBOX_IDNO = 7
};

class MessageBoxClass
{
public:
    MessageBoxClass(int caption = TXT_NULL) : CaptionText(caption) {}
    ~MessageBoxClass() {}

    int Process(const int body_text, const int button_1_text = TXT_OK, const int button_2_text = TXT_NONE,
        const int button_3_text = TXT_NONE, BOOL shadow_seen = false);
    int Process(const char *body_text, const int button_1_text = TXT_OK, const int button_2_text = TXT_NONE,
        const int button_3_text = TXT_NONE, BOOL shadow_seen = false);
    int Process(const char *body_text, const char *button_1_text, const char *button_2_text, const char *button_3_text,
        BOOL shadow_seen = false);

private:
    int CaptionText;
};

#endif // MSGBOX_H
