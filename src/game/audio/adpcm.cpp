
#include "adpcm.h"
#include "endiantype.h"
#include <algorithm>
#include <cstring>

using std::memset;

// clang-format off
static const int16_t g_ADPCMStepTab[89] = {
    7,     8,     9,     10,    11,    12,     13,    14,    16,
    17,    19,    21,    23,    25,    28,     31,    34,    37,
    41,    45,    50,    55,    60,    66,     73,    80,    88,
    97,    107,   118,   130,   143,   157,    173,   190,   209,
    230,   253,   279,   307,   337,   371,    408,   449,   494,
    544,   598,   658,   724,   796,   876,    963,   1060,  1166,
    1282,  1411,  1552,  1707,  1878,  2066,   2272,  2499,  2749,
    3024,  3327,  3660,  4026,  4428,  4871,   5358,  5894,  6484,
    7132,  7845,  8630,  9493,  10442, 11487,  12635, 13899, 15289,
    16818, 18500, 20350, 22385, 24623, 27086,  29794, 32767
};

static const int16_t g_ADPCMIndexTab[16] = {
    -1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8
};

static const int8_t g_ZapTabTwo[4] = {-2, -1, 0, 1};

static const int8_t g_ZapTabFour[16] = {
    -9, -8, -6, -5, -4, -3, -2, -1, 0, 1,  2,  3,  4,  5,  6,  8
};
// clang-format on

void __cdecl ADPCM_Stream_Init(ADPCMStreamType *stream)
{
    stream->m_LeftCode = 0;
    stream->m_LeftCodeBuf = 0;
    stream->m_LeftIndex = 0;
    stream->m_LeftStep = g_ADPCMStepTab[stream->m_LeftIndex];
    stream->m_LeftPredicted = 0;
    stream->m_LeftSampleIndex = 0;
    stream->m_RightCode = 0;
    stream->m_RightCodeBuf = 0;
    stream->m_RightIndex = 0;
    stream->m_RightStep = g_ADPCMStepTab[stream->m_RightIndex];
    stream->m_RightPredicted = 0;
    stream->m_RightSampleIndex = 0;
}

unsigned __cdecl ADPCM_Decompress(ADPCMStreamType *stream, unsigned bytes)
{
    int16_t current_nybble;
    unsigned step;
    int sample;
    unsigned full_length;

    full_length = bytes;
    stream->m_LeftSampleIndex = 0;
    stream->m_RightSampleIndex = 0;

    if (stream->m_BitsPerSample == 16) {
        bytes /= 2;
    }

    uint8_t *src = static_cast<uint8_t *>(stream->m_Source);
    int16_t *dst = static_cast<int16_t *>(stream->m_Dest);

    // Handle stereo
    if (stream->m_Channels == 2) {
        current_nybble = 0;
        for (int i = bytes; i > 0; i -= 2) {
            if ((stream->m_LeftSampleIndex & 1) != 0) {
                current_nybble = stream->m_LeftCodeBuf >> 4;
                stream->m_LeftCode = current_nybble;
            } else {
                stream->m_LeftCodeBuf = *src;
                // Stereo is interleaved so skip a byte for this channel
                src += 2;
                current_nybble = stream->m_LeftCodeBuf & 0xF;
                stream->m_LeftCode = current_nybble;
            }

            step = stream->m_LeftStep;
            stream->m_LeftDifference = step >> 3;

            if ((current_nybble & 4) != 0) {
                stream->m_LeftDifference += step;
            }

            if ((current_nybble & 2) != 0) {
                stream->m_LeftDifference += step >> 1;
            }

            if ((current_nybble & 1) != 0) {
                stream->m_LeftDifference += step >> 2;
            }

            if ((current_nybble & 8) != 0) {
                stream->m_LeftDifference = -stream->m_LeftDifference;
            }

            sample = std::clamp(stream->m_LeftDifference + stream->m_LeftPredicted, -32768, 32767);
            stream->m_LeftPredicted = sample;

            if (stream->m_BitsPerSample == 16) {
                *dst = sample;
                // Stereo is interleaved so skip a sample for this channel
                dst += 2;
            } else {
                *dst++ = ((sample & 0xFF00) >> 8) ^ 0x80;
            }

            stream->m_LeftIndex += g_ADPCMIndexTab[stream->m_LeftCode & 0x7];
            stream->m_LeftIndex = std::clamp<int16_t>(stream->m_LeftIndex, 0, 88);
            ++stream->m_LeftSampleIndex;
            stream->m_LeftStep = g_ADPCMStepTab[stream->m_LeftIndex];
        }

        src = static_cast<uint8_t *>(stream->m_Source) + 1;
        dst = reinterpret_cast<int16_t *>(static_cast<char *>(stream->m_Dest) + 1);

        if (stream->m_BitsPerSample == 16) {
            dst = static_cast<int16_t *>(stream->m_Dest) + 1;
        }

        for (int i = bytes; i > 0; i -= 2) {
            if ((stream->m_RightSampleIndex & 1) != 0) {
                current_nybble = stream->m_RightCodeBuf >> 4;
                stream->m_RightCode = current_nybble;
            } else {
                stream->m_RightCodeBuf = *src;
                // Stereo is interleaved so skip a byte for this channel
                src += 2;
                current_nybble = stream->m_RightCodeBuf & 0xF;
                stream->m_RightCode = current_nybble;
            }

            step = stream->m_RightStep;
            stream->m_RightDifference = step >> 3;

            if ((current_nybble & 4) != 0) {
                stream->m_RightDifference += step;
            }

            if ((current_nybble & 2) != 0) {
                stream->m_RightDifference += step >> 1;
            }

            if ((current_nybble & 1) != 0) {
                stream->m_RightDifference += step >> 2;
            }

            if ((current_nybble & 8) != 0) {
                stream->m_RightDifference = -stream->m_RightDifference;
            }

            sample = std::clamp(stream->m_RightDifference + stream->m_RightPredicted, -32768, 32767);
            stream->m_RightPredicted = sample;

            if (stream->m_BitsPerSample == 16) {
                *dst = sample;
                // Stereo is interleaved so skip a sample for this channel
                dst += 2;
            } else {
                *dst++ = ((sample & 0xFF00) >> 8) ^ 0x80;
            }

            stream->m_RightIndex += g_ADPCMIndexTab[stream->m_RightCode & 0x7];
            stream->m_RightIndex = std::clamp<int16_t>(stream->m_RightIndex, 0, 88);
            ++stream->m_RightSampleIndex;
            stream->m_RightStep = g_ADPCMStepTab[stream->m_RightIndex];
        }
    } else {
        for (int i = bytes; i > 0; --i) {
            if ((stream->m_LeftSampleIndex & 1) != 0) {
                current_nybble = stream->m_LeftCodeBuf >> 4;
                stream->m_LeftCode = current_nybble;
            } else {
                stream->m_LeftCodeBuf = *src++;
                current_nybble = stream->m_LeftCodeBuf & 0xF;
                stream->m_LeftCode = current_nybble;
            }

            step = stream->m_LeftStep;
            stream->m_LeftDifference = step >> 3;

            if ((current_nybble & 4) != 0) {
                stream->m_LeftDifference += step;
            }

            if ((current_nybble & 2) != 0) {
                stream->m_LeftDifference += step >> 1;
            }

            if ((current_nybble & 1) != 0) {
                stream->m_LeftDifference += step >> 2;
            }

            if ((current_nybble & 8) != 0) {
                stream->m_LeftDifference = -stream->m_LeftDifference;
            }

            sample = std::clamp(stream->m_LeftDifference + stream->m_LeftPredicted, -32768, 32767);
            stream->m_LeftPredicted = sample;

            if (stream->m_BitsPerSample == 16) {
                *dst++ = sample;
            } else {
                *dst = ((sample & 0xFF00) >> 8) ^ 0x80;
                dst = reinterpret_cast<int16_t *>(reinterpret_cast<char *>(dst) + 1);
            }

            stream->m_LeftIndex += g_ADPCMIndexTab[stream->m_LeftCode & 0x7];
            stream->m_LeftIndex = std::clamp<int16_t>(stream->m_LeftIndex, 0, 88);
            ++stream->m_LeftSampleIndex;
            stream->m_LeftStep = g_ADPCMStepTab[stream->m_LeftIndex];
        };
    }

    return full_length;
}

unsigned __cdecl ADPCM_Compress(ADPCMStreamType *stream, unsigned bytes)
{
    int delta;
    int tmp_step;
    int16_t code;
    unsigned tmp;
    unsigned step;
    int16_t current_samp;

    int samples = stream->m_BitsPerSample == 16 ? bytes >> 1 : bytes;
    stream->m_LeftSampleIndex = 0;
    stream->m_RightSampleIndex = 0;
    int16_t *src = static_cast<int16_t *>(stream->m_Source);
    uint8_t *dst = static_cast<uint8_t *>(stream->m_Dest);

    if (stream->m_Channels == 2) {
        // Compress a stereo data stream.
        for (int i = samples; i > 0; i -= 2) {
            if (stream->m_BitsPerSample == 16) {
                current_samp = le16toh(*src);
                src += 2;
            } else {
                current_samp = le16toh(*src);
                ++src;
                current_samp = (current_samp & 0xFF00) ^ 0x8000;
            }

            delta = current_samp - (stream->m_LeftPredicted);
            code = 0;

            if (delta < 0) {
                delta = -delta;
                code = 8;
            }

            stream->m_LeftCode = code;
            tmp_step = stream->m_LeftStep;
            tmp = 4;

            for (int i = 3; i > 0; --i) {
                if (delta >= tmp_step) {
                    stream->m_LeftCode |= tmp;
                    delta -= tmp_step;
                }

                tmp_step = tmp_step >> 1;
                tmp >>= 1;
            };

            stream->m_LeftDifference = delta;

            if (stream->m_LeftSampleIndex & 1) {
                *dst = stream->m_LeftCodeBuf | (stream->m_LeftCode << 4);
                dst += 2;
            } else {
                stream->m_LeftCodeBuf = stream->m_LeftCode & 0xF;
            }

            step = stream->m_LeftStep;
            code = stream->m_LeftCode;
            stream->m_LeftDifference = step >> 3;

            if (code & 4) {
                stream->m_LeftDifference += step;
            }

            if (code & 2) {
                stream->m_LeftDifference += step >> 1;
            }

            if (code & 1) {
                stream->m_LeftDifference += step >> 2;
            }

            if (code & 8) {
                stream->m_LeftDifference = -stream->m_LeftDifference;
            }

            stream->m_LeftPredicted = std::clamp(stream->m_LeftDifference + stream->m_LeftPredicted, -32768, 32767);
            stream->m_LeftIndex += g_ADPCMIndexTab[stream->m_LeftCode];
            stream->m_LeftIndex = std::clamp<int16_t>(stream->m_LeftIndex, 0, 88);
            ++stream->m_LeftSampleIndex;
            stream->m_LeftStep = g_ADPCMStepTab[stream->m_LeftIndex];
        }

        src = reinterpret_cast<int16_t *>(static_cast<char *>(stream->m_Source) + 1);
        dst = static_cast<uint8_t *>(stream->m_Dest) + 1;

        if (stream->m_BitsPerSample == 16) {
            src = reinterpret_cast<int16_t *>(stream->m_Source) + 1;
        }

        for (int i = samples; i > 0; i -= 2) {
            if (stream->m_BitsPerSample == 16) {
                current_samp = le16toh(*src);
                src += 2;
            } else {
                current_samp = le16toh(*src);
                ++src;
                current_samp = (current_samp & 0xFF00) ^ 0x8000;
            }

            delta = current_samp - (stream->m_RightPredicted);
            code = 0;

            if (delta < 0) {
                delta = -delta;
                code = 8;
            }

            stream->m_RightCode = code;
            tmp_step = stream->m_RightStep;
            tmp = 4;

            for (int i = 3; i > 0; --i) {
                if (delta >= tmp_step) {
                    stream->m_RightCode |= tmp;
                    delta -= tmp_step;
                }

                tmp_step >>= 1;
                tmp >>= 1;
            };

            stream->m_RightDifference = delta;

            if (stream->m_RightSampleIndex & 1) {
                *dst = stream->m_RightCodeBuf | (stream->m_RightCode << 4);
                dst += 2;
            } else {
                stream->m_RightCodeBuf = stream->m_RightCode & 0xF;
            }

            step = stream->m_RightStep;
            code = stream->m_RightCode;
            stream->m_RightDifference = step >> 3;

            if (code & 4) {
                stream->m_RightDifference += step;
            }

            if (code & 2) {
                stream->m_RightDifference += step >> 1;
            }

            if (code & 1) {
                stream->m_RightDifference += step >> 2;
            }

            if (code & 8) {
                stream->m_RightDifference = -stream->m_RightDifference;
            }

            stream->m_RightPredicted = std::clamp(stream->m_RightDifference + stream->m_RightPredicted, -32768, 32767);
            stream->m_RightIndex += g_ADPCMIndexTab[stream->m_RightCode];
            stream->m_RightIndex = std::clamp<int16_t>(stream->m_RightIndex, 0, 88);
            ++stream->m_RightSampleIndex;
            stream->m_RightStep = g_ADPCMStepTab[stream->m_RightIndex];
        }
    } else {
        // Compress a mono data stream.
        for (int i = samples; i > 0; --i) {
            if (stream->m_BitsPerSample == 16) {
                current_samp = *src;
                ++src;
            } else {
                current_samp = *src;
                src = reinterpret_cast<int16_t *>(reinterpret_cast<char *>(src) + 1);
                current_samp = (current_samp & 0xFF00) ^ 0x8000;
            }

            delta = current_samp - (stream->m_LeftPredicted);
            code = 0;

            if (delta < 0) {
                delta = -delta;
                code = 8;
            }

            stream->m_LeftCode = code;
            tmp_step = stream->m_LeftStep;
            tmp = 4;

            for (int i = 3; i > 0; --i) {
                if (delta >= tmp_step) {
                    stream->m_LeftCode |= tmp;
                    delta -= tmp_step;
                }

                tmp_step >>= 1;
                tmp >>= 1;
            }

            stream->m_LeftDifference = delta;

            if (stream->m_LeftSampleIndex & 1) {
                *dst++ = stream->m_LeftCodeBuf | (stream->m_LeftCode << 4);
            } else {
                stream->m_LeftCodeBuf = stream->m_LeftCode & 0xF;
            }

            step = stream->m_LeftStep;
            code = stream->m_LeftCode;
            stream->m_LeftDifference = step >> 3;

            if (code & 4) {
                stream->m_LeftDifference += step;
            }

            if (code & 2) {
                stream->m_LeftDifference += step >> 1;
            }

            if (code & 1) {
                stream->m_LeftDifference += step >> 2;
            }

            if (code & 8) {
                stream->m_LeftDifference = -stream->m_LeftDifference;
            }

            stream->m_LeftPredicted = std::clamp(stream->m_LeftDifference + stream->m_LeftPredicted, -32768, 32767);
            stream->m_LeftIndex += g_ADPCMIndexTab[stream->m_LeftCode];
            stream->m_LeftIndex = std::clamp<int16_t>(stream->m_LeftIndex, 0, 88);
            ++stream->m_LeftSampleIndex;
            stream->m_LeftStep = g_ADPCMStepTab[stream->m_LeftIndex];
        }
    }

    return stream->m_BitsPerSample == 16 ? bytes >> 2 : bytes >> 1;
}

unsigned __cdecl Audio_Unzap(void *input, void *output, int length)
{
    int16_t sample;
    uint8_t code;
    int8_t count;
    uint16_t shifted;

    sample = 0x80; //-128
    uint8_t *src = static_cast<uint8_t *>(input);
    uint8_t *dst = static_cast<uint8_t *>(output);
    uint16_t remaining = length;

    while (remaining > 0) { // expecting more output
        shifted = *src++;
        shifted <<= 2;
        code = (shifted & 0xFF00) >> 8;
        count = (shifted & 0x00FF) >> 2;

        switch (code) {
            case 2: // no compression...
                if (count & 0x20) {
                    count <<= 3; // here it's significant that (count) is signed:
                    sample += count >> 3; // the sign bit will be copied by these shifts!
                    *dst++ = std::clamp<int16_t>(sample, 0, 255);
                    remaining--; // one byte added to output
                } else {
                    for (++count; count > 0; --count) {
                        --remaining;
                        *dst++ = *src++;
                    }

                    sample = *(src - 1); // set (sample) to the last byte sent to output
                }
                break;

            case 1: // ADPCM 8-bit -> 4-bit
                for (++count; count > 0; --count) { // decode (count+1) bytes
                    code = *src++;
                    sample += g_ZapTabFour[(code & 0x0F)]; // lower nibble
                    *dst++ = std::clamp<int16_t>(sample, 0, 255);
                    sample += g_ZapTabFour[(code >> 4)]; // higher nibble
                    *dst++ = std::clamp<int16_t>(sample, 0, 255);
                    remaining -= 2; // two bytes added to output
                }
                break;

            case 0: // ADPCM 8-bit -> 2-bit
                for (++count; count > 0; --count) { // decode (count+1) bytes
                    code = *src++;
                    sample += g_ZapTabTwo[(code & 0x03)]; // lower 2 bits
                    *dst++ = std::clamp<int16_t>(sample, 0, 255);
                    sample += g_ZapTabTwo[((code >> 2) & 0x03)]; // lower middle 2 bits
                    *dst++ = std::clamp<int16_t>(sample, 0, 255);
                    sample += g_ZapTabTwo[((code >> 4) & 0x03)]; // higher middle 2 bits
                    *dst++ = std::clamp<int16_t>(sample, 0, 255);
                    sample += g_ZapTabTwo[((code >> 6) & 0x03)]; // higher 2 bits
                    *dst++ = std::clamp<int16_t>(sample, 0, 255);
                    remaining -= 4; // 4 bytes sent to output
                }
                break;

            default: // just copy (sample) (count+1) times to output
                memset(dst, std::clamp<int16_t>(sample, 0, 255), ++count);
                remaining -= count;
                dst += count;
                break;
        }
    }

    return length - remaining;
}
