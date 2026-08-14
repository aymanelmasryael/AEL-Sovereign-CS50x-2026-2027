/**
 * @file bmp.h
 * @brief Compact, packed type definitions for 24-bit uncompressed BMP files.
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027, Problem Set 4
 *
 * Every struct mirrors the on-disk, little-endian layout of the classic
 * Windows BMP format. __attribute__((__packed__)) guarantees the compiler
 * performs no padding, so a single fread() reads the header byte-for-byte.
 */

#include <stdint.h>

typedef uint8_t BYTE;
typedef uint32_t DWORD;
typedef int32_t LONG;
typedef uint16_t WORD;

typedef struct
{
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} __attribute__((__packed__)) BITMAPFILEHEADER;

typedef struct
{
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} __attribute__((__packed__)) BITMAPINFOHEADER;

typedef struct
{
    BYTE rgbtBlue;
    BYTE rgbtGreen;
    BYTE rgbtRed;
} __attribute__((__packed__)) RGBTRIPLE;
