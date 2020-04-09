/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief  Decoders for the IMA ADPCM and Westwood ADPCM formats.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef ADPCM_H
#define ADPCM_H

#include "always.h"

struct ADPCMStreamType
{
    void *m_Source; // pointer to compressed data
    void *m_Dest; // pointer to uncompressed data
    int32_t m_CompSize; // compressed size
    int32_t m_UnCompSize; // uncompressed size
    int32_t m_LeftSampleIndex; // index to next sample
    int32_t m_LeftPredicted; // next predicted value
    int32_t m_LeftDifference; // difference from last sample
    int16_t m_LeftCodeBuf; // holds 2 nybbles for decomp
    int16_t m_LeftCode; // current 4bit code
    int16_t m_LeftStep; // step value in table
    int16_t m_LeftIndex; // bit size for decompression
    int32_t m_RightSampleIndex; // These duplicate entries are for a second channel
    int32_t m_RightPredicted;
    int32_t m_RightDifference;
    int16_t m_RightCodeBuf;
    int16_t m_RightCode;
    int16_t m_RightStep;
    int16_t m_RightIndex;
    int16_t m_BitsPerSample; // bits per sample, 8 or 16 bit are valid
    int16_t m_Channels; // number of channels, 1 or 2 are valid.
};

void __cdecl ADPCM_Stream_Init(ADPCMStreamType *stream);
unsigned __cdecl ADPCM_Decompress(ADPCMStreamType *stream, unsigned bytes);
unsigned __cdecl ADPCM_Compress(ADPCMStreamType *stream, unsigned bytes);
unsigned __cdecl Audio_Unzap(void *input, void *output, int length);

#endif // ADPCM_H
