// bmp.h
// Cross-platform BMP header definitions (CS50-style)

#ifndef BMP_H
#define BMP_H

#include <stdint.h>

// Use 1-byte alignment across all compilers
#ifdef _MSC_VER
#pragma pack(push, 1)
#else
#pragma pack(push, 1)
#endif

// --- Bitmap File Header (14 bytes) ---
typedef struct
{
    uint16_t bfType;         // File type ("BM")
    uint32_t bfSize;         // Size of the file in bytes
    uint16_t bfReserved1;    // Reserved; must be 0
    uint16_t bfReserved2;    // Reserved; must be 0
    uint32_t bfOffBits;      // Offset to start of pixel data
} BITMAPFILEHEADER;

// --- Bitmap Info Header (40 bytes for BITMAPINFOHEADER) ---
typedef struct
{
    uint32_t biSize;            // Header size (40 bytes)
    int32_t  biWidth;           // Image width in pixels
    int32_t  biHeight;          // Image height in pixels
    uint16_t biPlanes;          // Number of color planes (1)
    uint16_t biBitCount;        // Bits per pixel (24 for RGB)
    uint32_t biCompression;     // Compression type (0 = none)
    uint32_t biSizeImage;       // Image size (may be 0 if uncompressed)
    int32_t  biXPelsPerMeter;   // Horizontal resolution (pixels/meter)
    int32_t  biYPelsPerMeter;   // Vertical resolution (pixels/meter)
    uint32_t biClrUsed;         // Number of colors used
    uint32_t biClrImportant;    // Number of important colors
} BITMAPINFOHEADER;

// --- RGB Triple (3 bytes per pixel) ---
typedef struct
{
    uint8_t rgbtBlue;
    uint8_t rgbtGreen;
    uint8_t rgbtRed;
} RGBTRIPLE;

#pragma pack(pop)

#endif // BMP_H