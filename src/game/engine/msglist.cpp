/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class for handling in game messages.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include <cstdio>
#include <cstring>
#include <cctype>
#include "msglist.h"
#include "gbuffer.h"
#include "mouse.h"
#include "rules.h"
#include "textprint.h"
#include "ttimer.h"
#include "txtlabel.h"
#include <algorithm>

using std::snprintf;
using std::strlen;
using std::strcpy;
using std::strncpy;

int MessageListClass::s_MaxMessageWidth = 640;

MessageListClass::MessageListClass() :
    m_LabelList(nullptr),
    m_XPos(0),
    m_YPos(0),
    m_FreeSlots(0),
    m_MaxChars(0),
    m_MessageHeight(0),
    m_Concatenate(false),
    m_Editing(false),
    m_EditAsMessage(false),
    m_EditXPos(0),
    m_EditYPos(0),
    m_EditingLabel(nullptr),
    m_EditPos(0),
    m_EditStart(0),
    m_EditCursor('\0'),
    m_EditTrimStart(0),
    m_EditTrimEnd(0),
    m_Width(0)
{
    for (int i = 0; i < MAX_MESSAGES; ++i) {
        m_SlotAvailable[i] = true;
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
    m_Width = width;

    for (TextLabelClass *label = m_LabelList; label != nullptr; label = m_LabelList) {
        m_LabelList = (TextLabelClass *)label->Remove();
        delete label;
    }

    m_LabelList = nullptr;

    for (int i = 0; i < MAX_MESSAGES; ++i) {
        m_SlotAvailable[i] = true;
    }

    if (m_Editing && m_EditingLabel != nullptr) {
        delete m_EditingLabel;
    }

    m_EditingLabel = nullptr;
    m_XPos = x;
    m_YPos = y;
    m_FreeSlots = std::min((int)MAX_MESSAGES, max_lines);
    m_MaxChars = std::min(120, max_chars);
    m_MessageHeight = height;
    m_Concatenate = concatenate;

    if (edit_x != -1 && edit_y != -1) {
        m_EditXPos = edit_x;
        m_EditYPos = edit_y;
        m_EditAsMessage = false;
    } else {
        m_EditXPos = x;
        m_EditYPos = y;
        m_EditAsMessage = true;
    }

    m_EditBuffer[0] = '\0';
    m_ReserveBuffer[0] = '\0';
    m_EditPos = 0;
    m_EditStart = 0;
    m_EditCursor = '\0';
    m_EditTrimStart = trim_start;
    m_EditTrimEnd = trim_end >= m_MaxChars ? m_MaxChars - 1 : trim_end;
    m_EditTrimStart = trim_start >= m_EditTrimEnd ? m_EditTrimEnd - 1 : trim_start;
}

/**
 * @brief Resets a MessageListClass.
 */
void MessageListClass::Reset()
{
    for (TextLabelClass *label = m_LabelList; label != nullptr; label = m_LabelList) {
        m_LabelList = (TextLabelClass *)label->Remove();
        delete label;
    }

    m_LabelList = nullptr;

    for (int i = 0; i < m_FreeSlots; ++i) {
        m_SlotAvailable[i] = true;
        // m_MessageBuffers[i][0] = '\0';
    }

    if (m_Editing) {
        if (m_EditingLabel != nullptr) {
            delete m_EditingLabel;
        }

        m_EditingLabel = nullptr;
    }

    m_Editing = false;
}

/**
 * @brief Adds a message to be displayed.
 */
TextLabelClass *MessageListClass::Add_Message(
    const char *player, int id, const char *message, PlayerColorType color, TextPrintType style, int lifetime)
{
    char msgbuff[152];

    if (player != nullptr) {
        snprintf(msgbuff, sizeof(msgbuff), "%s:%s", player, message);
    } else {
        strlcpy(msgbuff, message, sizeof(msgbuff));
    }

    Fancy_Text_Print(nullptr, 0, 0, &g_ColorRemaps[color], COLOR_TBLACK, style);
    int break_pos = 0;
    char saved_char = '\0';

    if (String_Pixel_Width(msgbuff) >= (m_Width - 8)) {
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

            if (String_Pixel_Width(longbuff) >= m_Width - 8) {
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

    if (m_FreeSlots > 0) {
        // Make some space if we would go over the number of free buffers.
        if (Num_Messages() + 1 > m_FreeSlots) {
            if (m_LabelList == nullptr) {
                return nullptr;
            }

            TextLabelClass *last = m_LabelList;
            m_LabelList = reinterpret_cast<TextLabelClass *>(m_LabelList->Remove());

            // Find the index of the buffer we are freeing.
            for (int i = 0; i < MAX_MESSAGES; ++i) {
                if (m_MessageBuffers[i] == last->Get_Text()) {
                    m_SlotAvailable[i] = true;
                }
            }

            delete last;
        }
    }

    TextLabelClass *msglabel = new TextLabelClass(msgbuff, m_XPos, m_YPos, &g_ColorRemaps[color], style);

    if (lifetime == -1) {
        msglabel->Set_Lifetime(0);
    } else {
        msglabel->Set_Lifetime(g_TickCountTimer.Time() + lifetime);
    }

    msglabel->Set_Label_ID(id);
    bool new_message = false;

    // Use next free slot.
    for (int i = 0; i < MAX_MESSAGES; ++i) {
        if (m_SlotAvailable[i]) {
            m_SlotAvailable[i] = false;
            memset(m_MessageBuffers[i], 0, sizeof(m_MessageBuffers[i]));
            strcpy(m_MessageBuffers[i], msgbuff);
            new_message = true;
            msglabel->Set_Text(m_MessageBuffers[i]);
            break;
        }
    }

    if (new_message) {

        // Play notification of new message.
        Sound_Effect(VOC_INCOMING_MESSAGE);

        if (m_LabelList != nullptr) {
            msglabel->Add_Tail(*m_LabelList);
        } else {
            m_LabelList = msglabel;
        }

        Compute_Y();

        if (saved_char != '\0') {
            msgbuff[break_pos] = saved_char;
            Add_Message(player, id, &msgbuff[break_pos], color, style, lifetime);
        }
    } else {
        delete msglabel;

        return nullptr;
    }

    return msglabel;
}

/**
* @brief Adds a simple message to be displayed.
*/
TextLabelClass *MessageListClass::Add_Simple_Message(const char *player, const char *message, PlayerColorType color)
{
    return Add_Message(player, 0, message, color, TPF_6PT_GRAD | TPF_OUTLINE | TPF_USE_GRAD_PAL, g_Rule.Message_Delay() * 900);
}

/**
 * @brief Get the contents of a message based on its ID.
 */
char *MessageListClass::Get_Message(int id)
{
    TextLabelClass *label = m_LabelList;

    if (label != nullptr) {
        while (label->Get_Label_ID() != id) {
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
    TextLabelClass *label = m_LabelList;

    if (label != nullptr) {
        while (label->Get_Label_ID() != id) {
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
BOOL MessageListClass::Concat_Message(char *msg, int id, char *to_concat, int lifetime)
{
    if (msg == nullptr || !m_Concatenate) {
        return false;
    }

    bool found = false;
    TextLabelClass *label;

    for (label = m_LabelList; label != nullptr; label = reinterpret_cast<TextLabelClass *>(label->Get_Next())) {
        if (id == label->Get_Label_ID() && memcmp(label->Get_Text(), msg, strlen(msg)) == 0) {
            found = true;

            break;
        }
    }

    if (found) {
        char *label_sub_str = &label->Get_Text()[strlen(msg) + 1];

        if (strlen(to_concat) + strlen(label_sub_str) >= (unsigned)m_MaxChars) {
            int required = std::max(strlen(to_concat) + strlen(label_sub_str) - m_MaxChars, strlen(label_sub_str));

            Trim_Message(nullptr, label_sub_str, strlen(to_concat) + strlen(label_sub_str) - m_MaxChars, required, 0);
        } else {
            char *new_str = new char[m_MaxChars + 1];
            int width_diff = String_Pixel_Width(label->Get_Text()) - String_Pixel_Width(label_sub_str);

            Fancy_Text_Print(nullptr, 0, 0, label->Get_Remap(), COLOR_TBLACK, label->Get_Style());
            strcpy(new_str, label_sub_str);
            strcpy(&new_str[strlen(new_str)], to_concat);

            for (int i = width_diff + String_Pixel_Width(new_str); i < m_Width - 8;
                 i = width_diff + String_Pixel_Width(new_str)) {
                int sub_st_len = std::min(strlen(label_sub_str), (size_t)10u);
                Trim_Message(nullptr, label_sub_str, 10, sub_st_len, 0);
                strcpy(new_str, label_sub_str);
                strcpy(&new_str[strlen(new_str)], to_concat);
            }

            delete[] new_str;
        }

        strcpy(&label_sub_str[strlen(label_sub_str)], to_concat);

        if (lifetime == -1) {
            label->Set_Lifetime(0);
        } else {
            label->Set_Lifetime(g_TickCountTimer.Time() + lifetime);
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
    if (m_Editing) {
        m_EditingLabel->Set_Focus();
    }
}

/**
 * @brief Check if the TextLabelClass used for editing has focus.
 */
bool MessageListClass::Has_Edit_Focus()
{
    if (m_Editing) {
        return m_EditingLabel->Has_Focus();
    }

    return false;
}

/**
 * @brief Add an editable message.
 */
TextLabelClass *MessageListClass::Add_Edit(PlayerColorType player, TextPrintType style, char *string, char cursor, int width)
{
    if (m_Editing) {
        m_EditingLabel->Set_Focus();

        return nullptr;
    }

    if (m_EditAsMessage) {
        if (Num_Messages() + 1 > m_FreeSlots) {
            if (m_LabelList == nullptr) {
                return nullptr;
            }

            TextLabelClass *last = m_LabelList;
            m_LabelList = reinterpret_cast<TextLabelClass *>(m_LabelList->Remove());

            // Find the index of the buffer we are freeing.
            for (int i = 0; i < MAX_MESSAGES; ++i) {
                if (m_MessageBuffers[i] == last->Get_Text()) {
                    m_SlotAvailable[i] = true;
                }
            }

            delete last;
        }
    }

    const char *existing = string != nullptr ? string : "";
    m_EditCursor = cursor;
    memset(m_EditBuffer, 0, sizeof(m_EditBuffer));
    strlcpy(m_EditBuffer, existing, sizeof(m_EditBuffer));
    m_ReserveBuffer[0] = '\0';
    m_EditStart = strlen(existing);
    m_EditPos = strlen(existing);

    m_EditingLabel = new TextLabelClass(m_EditBuffer, m_EditXPos, m_EditYPos, &g_ColorRemaps[player], style);
    m_Width = width;

    if (m_EditingLabel != nullptr) {
        m_Editing = true;
        m_EditingLabel->Set_Focus();
    } else {
        m_Editing = false;
    }

    if (m_EditAsMessage) {
        Compute_Y();
        --m_FreeSlots;
    }

    return m_EditingLabel;
}

/**
 * @brief Remove the editable message.
 */
void MessageListClass::Remove_Edit()
{
    if (!m_Editing) {
        return;
    }

    m_Editing = false;

    if (m_EditingLabel != nullptr) {
        delete m_EditingLabel;
    }

    if (m_EditAsMessage) {
        Compute_Y();
        ++m_FreeSlots;
    }
}

/**
 * @brief Get the contents of the editable message.
 */
char *MessageListClass::Get_Edit_Buf()
{
    return &m_EditBuffer[m_EditStart];
}

/**
 * @brief Set the color for the editable message.
 */
void MessageListClass::Set_Edit_Color(PlayerColorType player)
{
    if (m_Editing) {
        m_EditingLabel->Set_Remap(&g_ColorRemaps[player]);
    }
}

/**
 * @brief Perform update tasks for the message list such as removing messages that have displayed long enough.
 */
BOOL MessageListClass::Manage()
{
    bool message_removed = false;

    // Iterate through the message list and remove any messages that have past their delay.
    for (TextLabelClass *label = m_LabelList; label != nullptr;) {
        if (label->Get_Lifetime() != 0 && g_TickCountTimer > label->Get_Lifetime()) {
            TextLabelClass *next = reinterpret_cast<TextLabelClass *>(label->Get_Next());
            m_LabelList = reinterpret_cast<TextLabelClass *>(label->Remove());

            // Mark shot being freed up as available.
            for (int i = 0; i < MAX_MESSAGES; ++i) {
                if (label->Get_Text() == m_MessageBuffers[i]) {
                    m_SlotAvailable[i] = true;
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

    if (!m_Editing) {
        return 0;
    }

    KeyASCIIType ascii_char = g_Keyboard->To_ASCII(key);
    char raw_char = ascii_char & 0xFF;

    if (key & KEY_VK_BIT && std::isdigit(raw_char)) {
        key = (KeyNumType)(key & ~KEY_VK_BIT);
    } else if (key & KEY_VK_BIT || key & KN_BUTTON || !std::isprint(raw_char)) {
        int raw_key = key & 0xFF;

        if (raw_key != KN_KEYPAD_RETURN && raw_key != KN_BACKSPACE && raw_key != KN_ESC) {
            key = KN_NONE;

            return 0;
        }
    }

    int ret_val = 0;

    switch (ascii_char) {
        case KA_BACKSPACE:
            if (m_EditPos > m_EditStart) {
                m_EditBuffer[--m_EditPos] = '\0';
                ret_val = 2;
            }

            key = KN_NONE;
            m_EditingLabel->Set_Focus();

            break;
        case KA_RETURN:
            if (m_EditPos == m_EditStart) {
                key = KN_NONE;
                ret_val = 0;
            } else {
                if (m_EditPos - m_EditStart < m_MaxChars - 1) {
                    m_EditBuffer[m_EditPos++] = ' ';
                    m_EditBuffer[m_EditPos] = '\0';
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
            m_EditingLabel->Set_Focus();

            if (isprint(raw_char)) {
                bool too_long = false;

                if (m_EditPos - m_EditStart >= m_MaxChars - 1) {
                    too_long = true;
                } else {
                    m_EditBuffer[m_EditPos++] = raw_char;
                    m_EditBuffer[m_EditPos] = '\0';
                    Fancy_Text_Print(nullptr, 0, 0, m_EditingLabel->Get_Remap(), COLOR_TBLACK, m_EditingLabel->Get_Style());
                    ret_val = 1;

                    if (String_Pixel_Width(m_EditBuffer) >= m_Width - 10) {
                        too_long = true;
                        ret_val = 0;
                        m_EditBuffer[--m_EditPos] = '\0';
                    }

                    if (too_long) {
                        m_EditPos -= Trim_Message(m_ReserveBuffer, &m_EditBuffer[m_EditStart], m_EditTrimStart, m_EditTrimEnd, true);
                        m_EditBuffer[m_EditPos++] = raw_char;
                        m_EditBuffer[m_EditPos] = '\0';
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
    if (m_Editing) {
        if (g_LogicPage == &g_SeenBuff) {
            g_Mouse->Hide_Mouse();
        }

        m_EditingLabel->Draw_Me(true);

        // Handle if we have an edit cursor to draw.
        if (m_EditCursor != '\0' && m_EditPos - m_EditStart < m_MaxChars - 1) {
            if (m_EditingLabel->Has_Focus()) {
                char cursor[2] = { m_EditCursor, '\0' };
                Fancy_Text_Print(cursor,
                    m_EditingLabel->Get_XPos() + String_Pixel_Width(m_EditingLabel->Get_Text()),
                    m_EditingLabel->Get_YPos(),
                    m_EditingLabel->Get_Remap(),
                    COLOR_TBLACK,
                    m_EditingLabel->Get_Style());
            }
        }

        if (g_LogicPage == &g_SeenBuff) {
            g_Mouse->Show_Mouse();
        }
    }

    if (m_LabelList != nullptr) {
        if (g_LogicPage == &g_SeenBuff) {
            g_Mouse->Hide_Mouse();
        }

        m_LabelList->Draw_All(true);

        if (g_LogicPage == &g_SeenBuff) {
            g_Mouse->Show_Mouse();
        }
    }
}

/**
 * @brief Get the number of messages currently in the list.
 */
int MessageListClass::Num_Messages()
{
    int count = 0;
    TextLabelClass *label = m_LabelList;

    while (label != nullptr) {
        ++count;
        label = reinterpret_cast<TextLabelClass *>(label->Get_Next());
    }

    if (m_Editing && m_EditAsMessage) {
        ++count;
    }

    return count;
}

/**
 * @brief Give the TextLabelClass used for editing focus.
 */
void MessageListClass::Set_Width(int width)
{
    for (TextLabelClass *label = m_LabelList; label != nullptr;
         label = reinterpret_cast<TextLabelClass *>(label->Get_Next())) {
        label->Set_Max_Width(width);
    }

    if (m_Editing) {
        m_EditingLabel->Set_Max_Width(width);
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

    trim_end = std::min(strlen(msg), (size_t)trim_end);

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

    if (m_Editing && m_EditAsMessage) {
        ypos = m_MessageHeight + m_YPos;
    } else {
        ypos = m_YPos;
    }

    for (TextLabelClass *label = m_LabelList; label != nullptr;
         label = reinterpret_cast<TextLabelClass *>(label->Get_Next())) {
        label->Set_YPos(ypos);
        ypos += m_MessageHeight;
    }
}
