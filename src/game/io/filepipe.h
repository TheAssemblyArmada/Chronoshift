/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Pipe stream class implementation writing data to a file.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef FILEPIPE_H
#define FILEPIPE_H

#include "always.h"
#include "pipe.h"

class FileClass;

class FilePipe : public Pipe
{
public:
    FilePipe() : m_File(nullptr), m_HasOpened(false) {}
    FilePipe(FileClass &file) : m_File(&file), m_HasOpened(false) {}
    FilePipe(FileClass *file) : m_File(file), m_HasOpened(false) {}
    virtual ~FilePipe();

    virtual int Put(const void *buffer, int length) override;
    virtual int End() override;

private:
    bool Valid_File() { return m_File != nullptr; };

private:
    FileClass *m_File;
    BOOL m_HasOpened;
};

#endif // FILEPIPE_H
