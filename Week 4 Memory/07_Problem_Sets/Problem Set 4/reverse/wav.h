/**
 * @file wav.h
 * @brief Packed, little-endian layout of a 44-byte PCM WAV header.
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027, Problem Set 4
 *
 * The struct is byte-for-byte identical to the RIFF/WAVE container written
 * on disk, so a single fread() into WAVHEADER captures the full header.
 * __attribute__((__packed__)) forbids padding insertion between fields.
 */

#include <stdint.h>

typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t LONG;

typedef struct
{
    BYTE chunkID[4];
    DWORD chunkSize;
    BYTE format[4];
    BYTE subchunk1ID[4];
    DWORD subchunk1Size;
    WORD audioFormat;
    WORD numChannels;
    DWORD sampleRate;
    DWORD byteRate;
    WORD blockAlign;
    WORD bitsPerSample;
    BYTE subchunk2ID[4];
    DWORD subchunk2Size;
} __attribute__((__packed__)) WAVHEADER;