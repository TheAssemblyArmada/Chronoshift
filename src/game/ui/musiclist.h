/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class specialised for the list of music tracks.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef MUSICLIST_H
#define MUSICLIST_H

#include "always.h"
#include "dialog.h"
#include "list.h"

class MusicListClass : public ListClass
{
public:
    MusicListClass(int id, int x, int y, int w, int h, TextPrintType style, void *up_btn_shape, void *down_btn_shape);
    MusicListClass(MusicListClass &that) : ListClass(that) {}
    virtual ~MusicListClass(void) {}

    virtual void Draw_Entry(int index, int x, int y, int x_max, BOOL selected = false) override;

    MusicListClass &operator=(MusicListClass &that);
};

inline MusicListClass &MusicListClass::operator=(MusicListClass &that)
{
    if (this != &that) {
        ListClass::operator=(that);
    }

    return *this;
}

#endif // MUSICLIST_H
