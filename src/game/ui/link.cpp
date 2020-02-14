/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Base class for UI elements linking them together for processing.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "link.h"
#include <captainslog.h>

/**
 * @brief Add another LinkClass instance as the next link in the chain after this instance.
 */
LinkClass &LinkClass::Add(LinkClass &that)
{
    captainslog_assert(this != &that);
    LinkClass *next = that.m_Next;
    that.m_Next = this;
    m_Prev = &that;
    m_Next = next;

    if (next != nullptr) {
        next->m_Prev = this;
    }

    return Head_Of_List();
}

/**
 * @brief Add another LinkClass instance as the last link in the chain.
 */
LinkClass &LinkClass::Add_Tail(LinkClass &that)
{
    captainslog_assert(this != &that);
    LinkClass *tail = &that.Tail_Of_List();
    tail->m_Next = this;
    m_Next = nullptr;
    m_Prev = tail;

    return Head_Of_List();
}

/**
 * @brief Add another LinkClass instance as the first link in the chain.
 */
LinkClass &LinkClass::Add_Head(LinkClass &that)
{
    captainslog_assert(this != &that);
    LinkClass *head = &that.Head_Of_List();
    head->m_Prev = this;
    m_Prev = nullptr;
    m_Next = head;

    return *this;
}

/**
 * @brief Fetch the first entry in the list from were this instance is.
 */
LinkClass &LinkClass::Head_Of_List()
{
    // Check we aren't at the head node already.
    if (m_Prev != nullptr) {
        LinkClass *entry;

        // Search back through the list until we find a node without a previous
        // node.
        for (entry = m_Prev; entry->m_Prev != nullptr; entry = entry->m_Prev)
            ;

        return *entry;
    }

    return *this;
}

/**
 * @brief Fetch the last entry in the list from where this instance is.
 */
LinkClass &LinkClass::Tail_Of_List()
{
    // Check we aren't at the tail node already.
    if (m_Next != nullptr) {
        LinkClass *entry;

        // Search forward through the list until we find a node without a
        // previous node.
        for (entry = m_Next; entry->m_Next != nullptr; entry = entry->m_Next)
            ;

        return *entry;
    }

    return *this;
}

/**
 * @brief Null both the Next and Prev pointers for this instance.
 */
void LinkClass::Zap()
{
    m_Prev = nullptr;
    m_Next = nullptr;
}

/**
 * @brief Unlink this instance from the chain and return a pointer to the head node or nullptr if this was the only entry.
 */
LinkClass *LinkClass::Remove()
{
    LinkClass *head = &Head_Of_List();
    LinkClass *tail = &Tail_Of_List();

    Unlink();

    // Check if we were the head or tail node.
    if (this == head) {
        // If we are both head and tail, then the list is now empty.
        if (this == tail) {
            return nullptr;
        }

        // If we were the head then the new head can only be reached from
        // starting at the tail node.
        return &tail->Head_Of_List();
    }

    // If we were in the middle, return the head.
    return head;
}

/**
 * @brief Unlink this instance from the list, closing the gap left behind.
 */
void LinkClass::Unlink()
{
    // Remove self from linked list and closes the gap?
    if (m_Prev != nullptr) {
        m_Prev->m_Next = m_Next;
    }

    if (m_Next != nullptr) {
        m_Next->m_Prev = m_Prev;
    }

    // Zap the pointers.
    Zap();
}
