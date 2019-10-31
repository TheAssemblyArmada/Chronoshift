/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Straw stream class implementation reading data from a file.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef FILESTRAW_H
#define FILESTRAW_H

#include "always.h"
#include "straw.h"

class FileClass;

class FileStraw : public Straw
{
public:
    FileStraw() : m_File(nullptr), m_HasOpened(false) {}
    FileStraw(FileClass &file) : m_File(&file), m_HasOpened(false) {}
    FileStraw(FileClass *file) : m_File(file), m_HasOpened(false) {}
    virtual ~FileStraw();

    virtual int Get(void *buffer, int length) override;

private:
    bool Valid_File() { return m_File != nullptr; }

private:
    FileClass *m_File;
    BOOL m_HasOpened;
};

#endif // FILESTRAW_H
