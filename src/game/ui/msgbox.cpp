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
#include "msgbox.h"
#include "callback.h"
#include "dialog.h"
#include "gbuffer.h"
#include "globals.h"
#include "keyboard.h"
#include "mouse.h"
#include "ostimer.h"
#include "stringex.h"
#include "surfacemonitor.h"
#include "textbtn.h"
#include "textprint.h"
#include <algorithm>
#include <cstring>

using std::memset;

/**
 * Creates message box windows and retrieves all strings from the string table.
 *
 * 0x0050500C
 */
int MessageBoxClass::Process(
    const int body_text, const int button_1_text, const int button_2_text, const int button_3_text, BOOL shadow_seen)
{
    return Process(Text_String(body_text),
        Text_String(button_1_text),
        Text_String(button_2_text),
        Text_String(button_3_text),
        shadow_seen);
}

/**
 * Creates message box windows and retrieves all strings from the string table except for the message body.
 *
 * 0x0050507C
 */
int MessageBoxClass::Process(
    const char *body_text, const int button_1_text, const int button_2_text, const int button_3_text, BOOL shadow_seen)
{
    return Process(
        body_text, Text_String(button_1_text), Text_String(button_2_text), Text_String(button_3_text), shadow_seen);
}

/**
 * Creates message box windows where all strings are specified directly.
 *
 * 0x005043D0
 */
int MessageBoxClass::Process(
    const char *body_text, const char *button_1_text, const char *button_2_text, const char *button_3_text, BOOL shadow_seen)
{
    char buffer[510];
    GraphicBufferClass shadow_buffer(g_VisiblePage.Get_Width(), g_VisiblePage.Get_Height());
    int ret_val = 0;

    if (button_1_text != nullptr && *button_1_text == '\0') {
        button_1_text = nullptr;
    }

    if (button_2_text != nullptr && *button_2_text == '\0') {
        button_2_text = nullptr;
    }

    if (button_3_text != nullptr && *button_3_text == '\0') {
        button_3_text = nullptr;
    }

    int button_count = 0;

    // Null call sets font and style so text size maths uses correct font for calcs.
    Fancy_Text_Print(nullptr, 0, 0, 0, 0, TPF_6PT_GRAD | TPF_NOSHADOW);

    int b1_ypos = 0;
    int b_width = 0;

    if (button_1_text != nullptr) {
        b1_ypos = g_FontYSpacing + g_FontHeight + 4;
        b_width = std::max(60, String_Pixel_Width(button_1_text) + 16);

        if (button_2_text != nullptr) {
            button_count = 2;
            b_width = std::max(b_width, String_Pixel_Width(button_2_text) + 16);

            if (button_3_text != nullptr) {
                button_count = 3;
            }
        } else {
            button_count = 1;
        }
    }

    strlcpy(buffer, body_text, sizeof(buffer));

    // Null call sets font and style so text size maths uses correct font for calcs.
    Fancy_Text_Print(nullptr, 0, 0, 0, 0, TPF_6PT_GRAD | TPF_NOSHADOW);
    int formatted_width;
    int formatted_height;
    int format_result = Format_Window_String(buffer, sizeof(buffer), formatted_width, formatted_height);
    TextPrintType style = TPF_6PT_GRAD | TPF_NOSHADOW;
    formatted_width = std::max(formatted_width, 180) + 80;
    formatted_height += button_count == 0 ? 80 : 120;
    int xpos = (g_SeenBuff.Get_Width() - formatted_width) / 2;
    int ypos = (g_SeenBuff.Get_Height() - formatted_height) / 2;
    int adj_xpos = xpos + 40;

    if (format_result == 1) {
        adj_xpos = (formatted_width / 2) + xpos;
        style |= TPF_CENTER;
    }

    Set_Logic_Page(&g_SeenBuff);
    // Perform a full blit of contents of visible to shadow buffer to allow removal of message later.
    g_VisiblePage.Blit(shadow_buffer, 0, 0, 0, 0, g_VisiblePage.Get_Width(), g_VisiblePage.Get_Height(), false);
    TextButtonClass button1(MSGBOX_IDOK,
        button_1_text,
        TPF_6PT_GRAD | TPF_NOSHADOW | TPF_CENTER,
        xpos + (button_count == 1 ? (formatted_width - b_width) / 2 : 40),
        formatted_height + ypos - (b1_ypos + 30),
        b_width,
        -1,
        false);

    TextButtonClass button2(MSGBOX_IDCANCEL,
        button_2_text,
        TPF_6PT_GRAD | TPF_NOSHADOW | TPF_CENTER,
        formatted_width + xpos - (b_width + 40),
        formatted_height + ypos - (b1_ypos + 30),
        b_width,
        -1,
        false);

    TextButtonClass button3(MSGBOX_IDABORT,
        button_3_text,
        TPF_6PT_GRAD | TPF_NOSHADOW | TPF_CENTER,
        0,
        formatted_height + ypos - (b1_ypos + 30),
        -1,
        -1,
        false);

    button3.Set_XPos(xpos + (formatted_width - button3.Get_Width()) / 2);

    TextButtonClass *button_head = nullptr;
    TextButtonClass *buttons[3];
    unsigned button_idx[3];
    memset(buttons, 0, sizeof(buttons));

    // Sort out linking the active gadgets to each other.
    if (button_count > 0) {
        button_head = &button1;
        buttons[0] = &button1;
        button_idx[0] = MSGBOX_IDOK;

        if (button_count > 2) {
            button3.Add(button1);
            buttons[1] = &button3;
            button_idx[1] = MSGBOX_IDABORT;

            button2.Add(button1);
            buttons[2] = &button2;
            button_idx[2] = MSGBOX_IDCANCEL;
        } else if (button_count == 2) {
            button2.Add(button1);
            buttons[1] = &button2;
            button_idx[1] = MSGBOX_IDCANCEL;
        }

        button_head->Turn_On();
    }

    g_Mouse->Hide_Mouse();
    uint8_t *seen_shadown_buff = nullptr;

    // Bool seems to decide if we shadow the seenBuff viewport as well as the underlying visiblePage buffer.
    if (shadow_seen) {
        seen_shadown_buff = new uint8_t[formatted_height * formatted_width];
        g_SeenBuff.To_Buffer(
            xpos, ypos, formatted_width, formatted_height, seen_shadown_buff, formatted_height * formatted_width);
    }

    Dialog_Box(xpos, ypos, formatted_width, formatted_height);
    Draw_Caption(m_CaptionText, xpos, ypos, formatted_width);
    Fancy_Text_Print(buffer, adj_xpos, ypos + 40, GadgetClass::Get_Color_Scheme(), COLOR_TBLACK, style);

    if (button_head) {
        button_head->Draw_All(true);
    }

    g_Mouse->Show_Mouse();

    if (button_head != nullptr) {
        bool process = true;
        bool redraw = false;
        bool process_action = false;
        int starting_button = button_count - 1;
        int current_button = starting_button;

        while (process) {
            if (g_AllSurfaces.Surfaces_Restored()) {
                g_AllSurfaces.Clear_Surfaces_Restored();
                shadow_buffer.Blit(g_VisiblePage, 0, 0, 0, 0, shadow_buffer.Get_Width(), shadow_buffer.Get_Height(), false);
                redraw = true;
            }

            if (redraw) {
                g_Mouse->Hide_Mouse();
                Dialog_Box(xpos, ypos, formatted_width, formatted_height);
                Draw_Caption(m_CaptionText, xpos, ypos, formatted_width);
                Fancy_Text_Print(buffer, adj_xpos, ypos + 30, GadgetClass::Get_Color_Scheme(), COLOR_TBLACK, style);

                if (button_head) {
                    button_head->Draw_All(true);
                }

                g_Mouse->Show_Mouse();
            }

            Call_Back();
            KeyNumType input = button_head->Input();
            unsigned click_button = 0;

            if (g_CancelCurrentMsgBox) {
                input = KN_ESC;
                g_CancelCurrentMsgBox = false;
            }

            switch ((int)input) {
                case KN_RETURN:
                    process_action = true;
                    click_button = button_idx[current_button];
                    break;
                case KN_ESC:
                    process_action = true;
                    click_button = button_idx[button_count - 1];
                    break;
                case KN_LEFT:
                    if (button_count > 1) {
                        buttons[current_button]->Turn_Off();
                        buttons[current_button]->Flag_To_Redraw();
                        if (--current_button < 0) {
                            current_button = starting_button;
                        }

                        buttons[current_button]->Turn_On();
                        buttons[current_button]->Flag_To_Redraw();
                    }
                    break;
                case KN_RIGHT:
                    if (button_count > 1) {
                        buttons[current_button]->Turn_Off();
                        buttons[current_button]->Flag_To_Redraw();
                        if (++current_button > starting_button) {
                            current_button = 0;
                        }

                        buttons[current_button]->Turn_On();
                        buttons[current_button]->Flag_To_Redraw();
                    }
                    break;
                case KN_BUTTON | MSGBOX_IDOK:
                    click_button = button_idx[0];
                    process_action = true;
                    break;
                case KN_BUTTON | MSGBOX_IDCANCEL:
                    if (button_count <= 2) {
                        click_button = button_idx[1];
                    } else {
                        click_button = button_idx[2];
                    }

                    process_action = true;
                    break;
                case KN_BUTTON | MSGBOX_IDABORT:
                    click_button = button_idx[1];
                    process_action = true;
                    break;
                default:
                    break;
            }

            if (process_action) {
                TextButtonClass *extract = (TextButtonClass *)button_head->Extract_Gadget(MSGBOX_IDOK);

                if (extract != nullptr) {
                    extract->Turn_Off();
                    extract->Set_Toggle_Bool1(false);
                }

                extract = (TextButtonClass *)button_head->Extract_Gadget(MSGBOX_IDCANCEL);

                if (extract != nullptr) {
                    extract->Turn_Off();
                    extract->Set_Toggle_Bool1(false);
                }

                extract = (TextButtonClass *)button_head->Extract_Gadget(MSGBOX_IDABORT);

                if (extract != nullptr) {
                    extract->Turn_Off();
                    extract->Set_Toggle_Bool1(false);
                }

                if (click_button == MSGBOX_IDOK || click_button == MSGBOX_IDCANCEL || click_button == MSGBOX_IDABORT) {
                    extract = (TextButtonClass *)button_head->Extract_Gadget(click_button);

                    if (extract != nullptr) {
                        extract->Turn_On();
                        extract->Set_Toggle_Bool1(true);
                    }
                }

                g_Mouse->Hide_Mouse();
                button_head->Draw_All(true);
                g_Mouse->Show_Mouse();

                switch (click_button) {
                    case MSGBOX_IDOK:
                        process = false;
                        ret_val = 0;
                        break;
                    case MSGBOX_IDCANCEL:
                        process = false;
                        ret_val = 1;
                        break;
                    case MSGBOX_IDABORT:
                        process = false;
                        ret_val = 2;
                        break;
                    default:
                        break;
                }

                process_action = false;
            }

            // BUGFIX: Stops high CPU load in this menu.
            PlatformTimerClass::Sleep(1);
        }
    } else {
        g_Keyboard->Clear();
    }

    if (shadow_seen) {
        g_Mouse->Hide_Mouse();
        g_SeenBuff.From_Buffer(xpos, ypos, formatted_width, formatted_height, seen_shadown_buff);
        delete[] seen_shadown_buff;
        g_Mouse->Show_Mouse();
    }

    return ret_val;
}

int MessageBoxClass::Simple_OK_Message_Box(const int body_text, const int button_1_text)
{
    MessageBoxClass msgbox;
    return msgbox.Process(body_text, button_1_text);
}

int MessageBoxClass::Simple_OK_Message_Box(const char *body_text, const int button_1_text)
{
    MessageBoxClass msgbox;
    return msgbox.Process(body_text, button_1_text);
}

int MessageBoxClass::Simple_YesNo_Message_Box(const int body_text, const int button_1_text, const int button_2_text)
{
    MessageBoxClass msgbox;
    return msgbox.Process(body_text, button_1_text, button_2_text);
}

int MessageBoxClass::Simple_YesNo_Message_Box(const char *body_text, const int button_1_text, const int button_2_text)
{
    MessageBoxClass msgbox;
    return msgbox.Process(body_text, button_1_text, button_2_text);
}
