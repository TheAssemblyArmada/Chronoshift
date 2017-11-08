/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class for handling in game messages.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "msglist.h"
#include "globals.h"
#include "minmax.h"
#include "mouse.h"
#include "stringex.h"
#include "textprint.h"
#include "ttimer.h"
#include "txtlabel.h"
#include <stdio.h>

int MessageListClass::MaxMessageWidth = 640;

MessageListClass::MessageListClass() :
    LabelList(nullptr),
    XPos(0),
    YPos(0),
    FreeSlots(0),
    MaxChars(0),
    MessageHeight(0),
    Concatenate(false),
    Editing(false),
    EditAsMessage(false),
    EditXPos(0),
    EditYPos(0),
    EditingLabel(nullptr),
    EditPos(0),
    EditStart(0),
    EditCursor('\0'),
    EditTrimStart(0),
    EditTrimEnd(0),
    Width(0)
{
    for (int i = 0; i < MAX_MESSAGES; ++i) {
        SlotAvailable[i] = true;
    }
}

MessageListClass::~MessageListClass()
{
    Init(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 640);
}

/**
 * @brief Initialises a MessageListClass.
 */
void MessageListClass::Init(int x, int y, int max_lines, int max_chars, int height, int edit_x, int edit_y, BOOL concatenate,
    int trim_start, int trim_end, int width)
{
    Width = width;

    for (TextLabelClass *label = LabelList; label != nullptr; label = LabelList) {
        LabelList = (TextLabelClass *)label->Remove();
        delete label;
    }

    LabelList = nullptr;

    for (int i = 0; i < MAX_MESSAGES; ++i) {
        SlotAvailable[i] = true;
    }

    if (Editing && EditingLabel != nullptr) {
        delete EditingLabel;
    }

    EditingLabel = nullptr;
    XPos = x;
    YPos = y;
    FreeSlots = Min((int)MAX_MESSAGES, max_lines);
    MaxChars = Min(120, max_chars);
    MessageHeight = height;
    Concatenate = concatenate;

    if (edit_x != -1 && edit_y != -1) {
        EditXPos = edit_x;
        EditYPos = edit_y;
        EditAsMessage = false;
    } else {
        EditXPos = x;
        EditYPos = y;
        EditAsMessage = true;
    }

    EditBuffer[0] = '\0';
    ReserveBuffer[0] = '\0';
    EditPos = 0;
    EditStart = 0;
    EditCursor = '\0';
    EditTrimStart = trim_start;
    EditTrimEnd = trim_end >= MaxChars ? MaxChars - 1 : trim_end;
    EditTrimStart = trim_start >= EditTrimEnd ? EditTrimEnd - 1 : trim_start;
}

/**
 * @brief Resets a MessageListClass.
 */
void MessageListClass::Reset()
{
    for (TextLabelClass *label = LabelList; label != nullptr; label = LabelList) {
        LabelList = (TextLabelClass *)label->Remove();
        delete label;
    }

    LabelList = nullptr;

    for (int i = 0; i < FreeSlots; ++i) {
        SlotAvailable[i] = true;
        // MessageBuffers[i][0] = '\0';
    }

    if (Editing) {
        if (EditingLabel != nullptr) {
            delete EditingLabel;
        }

        EditingLabel = nullptr;
    }

    Editing = false;
}

/**
 * @brief Adds a message to be displayed.
 */
TextLabelClass *MessageListClass::Add_Message(
    char const *player, int id, char const *message, PlayerColorType color, TextPrintType style, int delay)
{
    char msgbuff[152];

    if (player != nullptr) {
        snprintf(msgbuff, sizeof(msgbuff), "%s:%s", player, message);
    } else {
        strlcpy(msgbuff, message, sizeof(msgbuff));
    }

    Fancy_Text_Print(nullptr, 0, 0, &ColorRemaps[color], COLOR_TBLACK, style);
    int break_pos = 0;
    char saved_char = '\0';

    if (String_Pixel_Width(msgbuff) >= (Width - 8)) {
        char longbuff[152];

        if (player != nullptr) {
            snprintf(longbuff, sizeof(longbuff), "%s:", player);
            break_pos = strlen(longbuff) + 1;
        }

        char *msgp = &longbuff[break_pos];
        unsigned to_copy = 1;
        int last_pos = break_pos + 1;

        while (to_copy < strlen(message)) {
            strncpy(msgp, message, to_copy);
            longbuff[last_pos] = '\0';

            if (String_Pixel_Width(longbuff) >= Width - 8) {
                break_pos = last_pos - 1;
                break;
            }

            ++last_pos;
            ++to_copy;
        }

        // Save the char from where we decided to break the string.
        if (break_pos != 0) {
            saved_char = msgbuff[break_pos];
            msgbuff[break_pos] = '\0';
        }
    }

    if (FreeSlots > 0) {
        // Make some space if we would go over the number of free buffers.
        if (Num_Messages() + 1 > FreeSlots) {
            if (LabelList == nullptr) {
                return nullptr;
            }

            TextLabelClass *last = LabelList;
            LabelList = reinterpret_cast<TextLabelClass *>(LabelList->Remove());

            // Find the index of the buffer we are freeing.
            for (int i = 0; i < MAX_MESSAGES; ++i) {
                if (MessageBuffers[i] == last->Get_Text()) {
                    SlotAvailable[i] = true;
                }
            }

            delete last;
        }
    }

    TextLabelClass *msglabel = new TextLabelClass(msgbuff, XPos, YPos, &ColorRemaps[color], style);

    if (delay == -1) {
        msglabel->Set_Delay(0);
    } else {
        msglabel->Set_Delay(TickCountTimer.Time() + delay);
    }

    msglabel->Set_ID(id);
    bool new_message = false;

    // Use next free slot.
    for (int i = 0; i < MAX_MESSAGES; ++i) {
        if (SlotAvailable[i]) {
            SlotAvailable[i] = false;
            memset(MessageBuffers[i], 0, sizeof(MessageBuffers[i]));
            strcpy(MessageBuffers[i], msgbuff);
            new_message = true;
            msglabel->Set_Text(MessageBuffers[i]);
            break;
        }
    }

    if (new_message) {
        // TODO Sound_Effect();

        if (LabelList != nullptr) {
            msglabel->Add_Tail(*LabelList);
        } else {
            LabelList = msglabel;
        }

        Compute_Y();

        if (saved_char != '\0') {
            msgbuff[break_pos] = saved_char;
            Add_Message(player, id, &msgbuff[break_pos], color, style, delay);
        }
    } else {
        delete msglabel;

        return nullptr;
    }

    return msglabel;
}

/**
 * @brief Get the contents of a message based on its ID.
 */
char *MessageListClass::Get_Message(int id)
{
    TextLabelClass *label = LabelList;

    if (label != nullptr) {
        while (label->Get_ID() != id) {
            label = reinterpret_cast<TextLabelClass *>(label->Get_Next());

            if (label == nullptr) {
                return nullptr;
            }
        }

        return label->Get_Text();
    }

    return nullptr;
}

/**
 * @brief Get the TextLabelClass gadget based on its ID.
 */
TextLabelClass *MessageListClass::Get_Label(int id)
{
    TextLabelClass *label = LabelList;

    if (label != nullptr) {
        while (label->Get_ID() != id) {
            label = reinterpret_cast<TextLabelClass *>(label->Get_Next());

            if (label == nullptr) {
                return nullptr;
            }
        }

        return label;
    }

    return nullptr;
}

/**
 * @brief Concatenate two messages.
 */
BOOL MessageListClass::Concat_Message(char *msg, int id, char *to_concat, int delay)
{
    if (msg == nullptr || !Concatenate) {
        return false;
    }

    bool found = false;
    TextLabelClass *label;

    for (label = LabelList; label != nullptr; label = reinterpret_cast<TextLabelClass *>(label->Get_Next())) {
        if (id == label->Get_ID() && memcmp(label->Get_Text(), msg, strlen(msg)) == 0) {
            found = true;

            break;
        }
    }

    if (found) {
        char *label_sub_str = &label->Get_Text()[strlen(msg) + 1];

        if (strlen(to_concat) + strlen(label_sub_str) >= (unsigned)MaxChars) {
            int required = Max(strlen(to_concat) + strlen(label_sub_str) - MaxChars, strlen(label_sub_str));

            Trim_Message(nullptr, label_sub_str, strlen(to_concat) + strlen(label_sub_str) - MaxChars, required, 0);
        } else {
            char *new_str = new char[MaxChars + 1];
            int width_diff = String_Pixel_Width(label->Get_Text()) - String_Pixel_Width(label_sub_str);

            Fancy_Text_Print(nullptr, 0, 0, label->Get_Remap(), COLOR_TBLACK, label->Get_Style());
            strcpy(new_str, label_sub_str);
            strcpy(&new_str[strlen(new_str)], to_concat);

            for (int i = width_diff + String_Pixel_Width(new_str); i < Width - 8;
                 i = width_diff + String_Pixel_Width(new_str)) {
                int sub_st_len = Min(strlen(label_sub_str), 10u);
                Trim_Message(nullptr, label_sub_str, 10, sub_st_len, 0);
                strcpy(new_str, label_sub_str);
                strcpy(&new_str[strlen(new_str)], to_concat);
            }

            delete[] new_str;
        }

        strcpy(&label_sub_str[strlen(label_sub_str)], to_concat);

        if (delay == -1) {
            label->Set_Delay(0);
        } else {
            label->Set_Delay(TickCountTimer.Time() + delay);
        }

        return true;
    }

    return false;
}

/**
 * @brief Give the TextLabelClass used for editing focus.
 */
void MessageListClass::Set_Edit_Focus()
{
    if (Editing) {
        EditingLabel->Set_Focus();
    }
}

/**
 * @brief Check if the TextLabelClass used for editing has focus.
 */
bool MessageListClass::Has_Edit_Focus()
{
    if (Editing) {
        return EditingLabel->Has_Focus();
    }

    return false;
}

/**
 * @brief Add an editable message.
 */
TextLabelClass *MessageListClass::Add_Edit(PlayerColorType player, TextPrintType style, char *string, char cursor, int width)
{
    if (Editing) {
        EditingLabel->Set_Focus();

        return nullptr;
    }

    if (EditAsMessage) {
        if (Num_Messages() + 1 > FreeSlots) {
            if (LabelList == nullptr) {
                return nullptr;
            }

            TextLabelClass *last = LabelList;
            LabelList = reinterpret_cast<TextLabelClass *>(LabelList->Remove());

            // Find the index of the buffer we are freeing.
            for (int i = 0; i < MAX_MESSAGES; ++i) {
                if (MessageBuffers[i] == last->Get_Text()) {
                    SlotAvailable[i] = true;
                }
            }

            delete last;
        }
    }

    const char *existing = string != nullptr ? string : "";
    EditCursor = cursor;
    memset(EditBuffer, 0, sizeof(EditBuffer));
    strlcpy(EditBuffer, existing, sizeof(EditBuffer));
    ReserveBuffer[0] = '\0';
    EditStart = strlen(existing);
    EditPos = strlen(existing);

    EditingLabel = new TextLabelClass(EditBuffer, EditXPos, EditYPos, &ColorRemaps[player], style);
    Width = width;

    if (EditingLabel != nullptr) {
        Editing = true;
        EditingLabel->Set_Focus();
    } else {
        Editing = false;
    }

    if (EditAsMessage) {
        Compute_Y();
        --FreeSlots;
    }

    return EditingLabel;
}

/**
 * @brief Remove the editable message.
 */
void MessageListClass::Remove_Edit()
{
    if (!Editing) {
        return;
    }

    Editing = false;

    if (EditingLabel != nullptr) {
        delete EditingLabel;
    }

    if (EditAsMessage) {
        Compute_Y();
        ++FreeSlots;
    }
}

/**
 * @brief Get the contents of the editable message.
 */
char *MessageListClass::Get_Edit_Buf()
{
    return &EditBuffer[EditStart];
}

/**
 * @brief Set the color for the editable message.
 */
void MessageListClass::Set_Edit_Color(PlayerColorType player)
{
    if (Editing) {
        EditingLabel->Set_Remap(&ColorRemaps[player]);
    }
}

/**
 * @brief Perform update tasks for the message list such as removing messages that have displayed long enough.
 */
BOOL MessageListClass::Manage()
{
    bool message_removed = false;

    // Iterate through the message list and remove any messages that have past their delay.
    for (TextLabelClass *label = LabelList; label != nullptr;) {
        if (label->Get_Delay() != 0 && TickCountTimer.Time() > label->Get_Delay()) {
            TextLabelClass *next = reinterpret_cast<TextLabelClass *>(label->Get_Next());
            LabelList = reinterpret_cast<TextLabelClass *>(label->Remove());

            // Mark shot being freed up as available.
            for (int i = 0; i < MAX_MESSAGES; ++i) {
                if (label->Get_Text() == MessageBuffers[i]) {
                    SlotAvailable[i] = true;
                }
            }

            if (label != nullptr) {
                delete label;
            }

            message_removed = true;
            label = next;
        } else {
            label = reinterpret_cast<TextLabelClass *>(label->Get_Next());
        }
    }

    if (message_removed) {
        Compute_Y();
    }

    return message_removed;
}

/**
 * @brief Handle input to the message list.
 */
int MessageListClass::Input(KeyNumType &key)
{
    if (key == KN_NONE) {
        return 0;
    }

    if ((key & (~KN_RLSE_BIT)) == KN_LMOUSE || (key & (~KN_RLSE_BIT)) == KN_RMOUSE) {
        return 0;
    }

    if (!Editing) {
        return 0;
    }

    char ascii = g_keyboard->To_ASCII(key);

    if (key & 0x1000 && isdigit(ascii)) {
        key = (KeyNumType)(key & ~0x1000);
    } else if (key & 0x1000 || key & KN_BUTTON || !isprint(ascii)) {
        int raw_key = key & 0xFF;

        if (raw_key != KN_KEYPAD_RETURN && raw_key != KN_BACKSPACE && raw_key != KN_ESC) {
            key = KN_NONE;

            return 0;
        }
    }

    int ret_val = 0;

    switch (ascii) {
        case KA_BACKSPACE:
            if (EditPos > EditStart) {
                EditBuffer[--EditPos] = '\0';
                ret_val = 2;
            }

            key = KN_NONE;
            EditingLabel->Set_Focus();

            break;
        case KA_RETURN:
            if (EditPos == EditStart) {
                key = KN_NONE;
                ret_val = 0;
            } else {
                if (EditPos - EditStart < MaxChars - 1) {
                    EditBuffer[EditPos++] = ' ';
                    EditBuffer[EditPos] = '\0';
                }

                Remove_Edit();
                key = KN_NONE;
                ret_val = 3;
            }

            break;

        case KA_ESC:
            ret_val = 2;
            Remove_Edit();
            key = KN_NONE;

            break;
        default:
            EditingLabel->Set_Focus();

            if (isprint(ascii)) {
                bool too_long = false;

                if (EditPos - EditStart >= MaxChars - 1) {
                    too_long = true;
                } else {
                    EditBuffer[EditPos++] = ascii;
                    EditBuffer[EditPos] = '\0';
                    Fancy_Text_Print(nullptr, 0, 0, EditingLabel->Get_Remap(), COLOR_TBLACK, EditingLabel->Get_Style());
                    ret_val = 1;

                    if (String_Pixel_Width(EditBuffer) >= Width - 10) {
                        too_long = true;
                        ret_val = 0;
                        EditBuffer[--EditPos] = '\0';
                    }

                    if (too_long) {
                        EditPos -= Trim_Message(ReserveBuffer, &EditBuffer[EditStart], EditTrimStart, EditTrimEnd, true);
                        EditBuffer[EditPos++] = ascii;
                        EditBuffer[EditPos] = '\0';
                        ret_val = 4;
                    }
                }
            }

            key = KN_NONE;

            break;
    }

    return ret_val;
}

/**
 * @brief Draw the list.
 */
void MessageListClass::Draw()
{
    if (Editing) {
        if (g_logicPage == &g_seenBuff) {
            g_mouse->Hide_Mouse();
        }

        EditingLabel->Draw_Me(true);

        // Handle if we have an edit cursor to draw.
        if (EditCursor != '\0' && EditPos - EditStart < MaxChars - 1) {
            if (EditingLabel->Has_Focus()) {
                char cursor[2] = { EditCursor, '\0' };
                Fancy_Text_Print(cursor,
                    EditingLabel->Get_XPos() + String_Pixel_Width(EditingLabel->Get_Text()),
                    EditingLabel->Get_YPos(),
                    EditingLabel->Get_Remap(),
                    COLOR_TBLACK,
                    EditingLabel->Get_Style());
            }
        }

        if (g_logicPage == &g_seenBuff) {
            g_mouse->Show_Mouse();
        }
    }

    if (LabelList != nullptr) {
        if (g_logicPage == &g_seenBuff) {
            g_mouse->Hide_Mouse();
        }

        LabelList->Draw_All(true);

        if (g_logicPage == &g_seenBuff) {
            g_mouse->Show_Mouse();
        }
    }
}

/**
 * @brief Get the number of messages currently in the list.
 */
int MessageListClass::Num_Messages()
{
    int count = 0;
    TextLabelClass *label = LabelList;

    while (label != nullptr) {
        ++count;
        label = reinterpret_cast<TextLabelClass *>(label->Get_Next());
    }

    if (Editing && EditAsMessage) {
        ++count;
    }

    return count;
}

/**
 * @brief Give the TextLabelClass used for editing focus.
 */
void MessageListClass::Set_Width(int width)
{
    for (TextLabelClass *label = LabelList; label != nullptr;
         label = reinterpret_cast<TextLabelClass *>(label->Get_Next())) {
        label->Set_Max_Width(width);
    }

    if (Editing) {
        EditingLabel->Set_Max_Width(width);
    }
}

/**
 * @brief Trim a message down.
 */
int MessageListClass::Trim_Message(char *reserve, char *msg, int trim_start, int trim_end, BOOL trim_right)
{
    if (trim_start <= 0) {
        return 0;
    }

    trim_end = Min(strlen(msg), (unsigned)trim_end);

    bool trim_adjusted = true;
    int trim_pos;

    // Trim from the right or left of the string.
    if (trim_right) {
        trim_pos = trim_end;

        if (trim_end < trim_start) {
            trim_adjusted = false;
        } else {
            char *check_ptr = &msg[trim_end - 1];

            while (isspace(*check_ptr--)) {
                if (trim_start > --trim_pos) {
                    trim_adjusted = false;
                    break;
                }
            }
        }
    } else {
        trim_pos = trim_start;

        if (trim_start > trim_end) {
            trim_adjusted = false;
        } else {
            char *check_ptr = &msg[trim_start - 1];

            while (isspace(*check_ptr++)) {
                if (trim_end < ++trim_pos) {
                    trim_adjusted = false;
                    break;
                }
            }
        }
    }

    if (!trim_adjusted) {
        trim_pos = trim_end;
    }

    if (reserve != nullptr) {
        memcpy(reserve, msg, trim_pos);
        reserve[trim_pos] = '\0';
    }

    memmove(msg, &msg[trim_pos], strlen(msg) - trim_pos + 1);

    return trim_pos;
}

/**
 * @brief Compute the position for each element in the list.
 */
void MessageListClass::Compute_Y()
{
    int ypos = 0;

    if (Editing && EditAsMessage) {
        ypos = MessageHeight + YPos;
    } else {
        ypos = YPos;
    }

    for (TextLabelClass *label = LabelList; label != nullptr;
         label = reinterpret_cast<TextLabelClass *>(label->Get_Next())) {
        label->Set_YPos(ypos);
        ypos += MessageHeight;
    }
}
