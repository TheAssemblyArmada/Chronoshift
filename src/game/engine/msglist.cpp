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
#include "stringex.h"
#include "textprint.h"
#include "ttimer.h"
#include "txtlabel.h"
#include <stdio.h>

int MessageListClass::MaxMessageWidth = 640;

MessageListClass::MessageListClass() :
    CurrentMessage(nullptr),
    XPos(0),
    YPos(0),
    FreeSlots(0),
    MaxChars(0),
    MessageHeight(0),
    MessageListBool1(false),
    Editing(false),
    MessageListBool3(false),
    EditXPos(0),
    EditYPos(0),
    EditingLabel(nullptr),
    MessageSize(0),
    EditPosition(0),
    EditCursor('\0'),
    field_15D(0),
    field_161(0),
    Width(0)
{
    for (int i = 0; i < MAX_MESSAGES; ++i) {
        BufferAvailable[i] = true;
    }
}

MessageListClass::~MessageListClass()
{
    Init(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 640);
}

void MessageListClass::Init(int x, int y, int max_lines, int max_chars, int max_msg, int x_offset, int y_offset, BOOL a8, int a9, int a10, int width)
{
    Width = width;

    for (TextLabelClass *label = CurrentMessage; label != nullptr; label = CurrentMessage) {
        CurrentMessage = (TextLabelClass *)label->Remove();
        delete label;
    }

    CurrentMessage = nullptr;

    for (int i = 0; i < MAX_MESSAGES; ++i) {
        BufferAvailable[i] = true;
    }

    if (Editing && EditingLabel != nullptr) {
        delete EditingLabel;
    }

    EditingLabel = nullptr;
    XPos = x;
    YPos = y;
    FreeSlots = Min((int)MAX_MESSAGES, max_lines);
    MaxChars = Min(120, max_chars);
    MessageHeight = max_msg;
    MessageListBool1 = a8;

    if (x_offset != -1 && y_offset != -1) {
        EditXPos = x_offset;
        EditYPos = y_offset;
        MessageListBool3 = false;
    } else {
        EditXPos = x;
        EditYPos = y;
        MessageListBool3 = true;
    }

    EditBuffer[0] = '\0';
    ReserveBuffer[0] = '\0';
    MessageSize = 0;
    EditPosition = 0;
    EditCursor = '\0';
    field_15D = a9;
    field_161 = a10 >= MaxChars ? MaxChars - 1 : a10;
    field_15D = a9 >= field_161 ? field_161 - 1 : a9;
}

void MessageListClass::Reset()
{
    for (TextLabelClass *label = CurrentMessage; label != nullptr; label = CurrentMessage) {
        CurrentMessage = (TextLabelClass *)label->Remove();
        delete label;
    }

    CurrentMessage = nullptr;

    for (int i = 0; i < FreeSlots; ++i) {
        BufferAvailable[i] = true;
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
            if (CurrentMessage == nullptr) {
                return nullptr;
            }

            TextLabelClass *last = CurrentMessage;
            CurrentMessage = reinterpret_cast<TextLabelClass *>(CurrentMessage->Remove());

            // Find the index of the buffer we are freeing.
            for (int i = 0; i < MAX_MESSAGES; ++i) {
                if (MessageBuffers[i] == last->Get_Text()) {
                    BufferAvailable[i] = true;
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

    for (int i = 0; i < MAX_MESSAGES; ++i) {
        if (BufferAvailable[i]) {
            BufferAvailable[i] = false;
            memset(MessageBuffers[i], 0, sizeof(MessageBuffers[i]));
            strcpy(MessageBuffers[i], msgbuff);
            new_message = true;
            break;
        }
    }
    
    if (new_message) {
        // TODO Sound_Effect();

        if (CurrentMessage != nullptr) {
            msglabel->Add_Tail(*CurrentMessage);
        } else {
            CurrentMessage = msglabel;
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

char *MessageListClass::Get_Message(int id)
{
    TextLabelClass *label = CurrentMessage;

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

TextLabelClass *MessageListClass::Get_Label(int id)
{
    TextLabelClass *label = CurrentMessage;

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

BOOL MessageListClass::Concat_Message(char *msg, int id, char *to_concat, int delay)
{
    if (msg == nullptr || !MessageListBool1) {
        return false;
    }

    bool found = false;
    TextLabelClass *label;

    for (label = CurrentMessage; label != nullptr; label = reinterpret_cast<TextLabelClass *>(label->Get_Next())) {
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
            
            Fancy_Text_Print(nullptr, 0, 0, label->Get_Remap(), 0, label->Get_Style());
            strcpy(new_str, label_sub_str);
            strcpy(&new_str[strlen(new_str)], to_concat);

            for (int i = width_diff + String_Pixel_Width(new_str); i < Width - 8; i = width_diff + String_Pixel_Width(new_str)) {
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

void MessageListClass::Set_Edit_Focus()
{
    if (Editing) {
        EditingLabel->Set_Focus();
    }
}

bool MessageListClass::Has_Edit_Focus()
{
    if (Editing) {
        return EditingLabel->Has_Focus();
    }

    return false;
}

TextLabelClass *MessageListClass::Add_Edit(PlayerColorType player, TextPrintType style, char *string, char cursor, int width)
{
    if (Editing) {
        EditingLabel->Set_Focus();

        return nullptr;
    }

    if (MessageListBool3) {
        if (Num_Messages() + 1 > FreeSlots) {
            if (CurrentMessage == nullptr) {
                return nullptr;
            }

            TextLabelClass *last = CurrentMessage;
            CurrentMessage = reinterpret_cast<TextLabelClass *>(CurrentMessage->Remove());

            // Find the index of the buffer we are freeing.
            for (int i = 0; i < MAX_MESSAGES; ++i) {
                if (MessageBuffers[i] == last->Get_Text()) {
                    BufferAvailable[i] = true;
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
    EditPosition = strlen(existing);
    MessageSize = strlen(existing);
    
    EditingLabel = new TextLabelClass(EditBuffer, EditXPos, EditYPos, &ColorRemaps[player], style);
    Width = width;

    if (EditingLabel != nullptr) {
        Editing = true;
        EditingLabel->Set_Focus();
    } else {
        Editing = false;
    }

    if (MessageListBool3) {
        Compute_Y();
        --FreeSlots;
    }

    return EditingLabel;
}

void MessageListClass::Remove_Edit()
{
    if (!Editing) {
        return;
    }

    Editing = false;

    if (EditingLabel != nullptr) {
        delete EditingLabel;
    }

    if (MessageListBool3) {
        Compute_Y();
        ++FreeSlots;
    }
}

char *MessageListClass::Get_Edit_Buf()
{
    return &EditBuffer[EditPosition];
}

void MessageListClass::Set_Edit_Color(PlayerColorType player)
{
    if (Editing) {
        EditingLabel->Set_Remap(&ColorRemaps[player]);
    }
}

BOOL MessageListClass::Manage()
{
    bool message_removed = false;

    // Iterate through the message list and remove any messages that have past their delay.
    for (TextLabelClass *label = CurrentMessage; label != nullptr;) {
        if (label->Get_Delay() != 0 && TickCountTimer.Time() > label->Get_Delay()) {
            TextLabelClass *next =  reinterpret_cast<TextLabelClass *>(label->Get_Next());
            CurrentMessage = reinterpret_cast<TextLabelClass *>(label->Remove());

            // Mark shot being freed up as available.
            for (int i = 0; i < MAX_MESSAGES; ++i) {
                if (label->Get_Text() == MessageBuffers[i]) {
                    BufferAvailable[i] = true;
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

int MessageListClass::Input(KeyNumType &key)
{
    return 0;
}

void MessageListClass::Draw() {}

int MessageListClass::Num_Messages()
{
    int count = 0;
    TextLabelClass *label = CurrentMessage;

    while (label != nullptr) {
        ++count;
        label = reinterpret_cast<TextLabelClass *>(label->Get_Next());
    }

    if (Editing && MessageListBool3) {
        ++count;
    }

    return count;
}

void MessageListClass::Set_Width(int width)
{
    for (TextLabelClass *label = CurrentMessage; label != nullptr; reinterpret_cast<TextLabelClass *>(label->Get_Next())) {
        label->Set_XPos_Max(width);
    }

    if (Editing) {
        EditingLabel->Set_XPos_Max(width);
    }
}

int MessageListClass::Trim_Message(char *a1, char *a2, int a3, int a4, int a5)
{
    return 0;
}

void MessageListClass::Compute_Y()
{
    int ypos = 0;

    if (Editing && MessageListBool3) {
        ypos = MessageHeight + YPos;
    } else {
        ypos = YPos;
    }

    for (TextLabelClass *label = CurrentMessage; label != nullptr; reinterpret_cast<TextLabelClass *>(label->Get_Next())) {
        label->Set_YPos(ypos);
        ypos += MessageHeight;
    }
}
