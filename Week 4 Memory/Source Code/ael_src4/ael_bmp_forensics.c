/**
 * AEL BMP Forensics — Bare-Metal BITMAP Header Parser & Bitwise Color Inverter
 * Built for CS50x 2026-2027 | AEL CS Encyclopedia
 * 
 * © Ayman Elmasry | AEL Digital Studio™
 * Enforcing the strict directive: 100% Original Proprietary Enterprise Code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Disable struct padding to guarantee perfect byte alignment with binary file streams
#pragma pack(push, 1)

typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} AEL_BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} AEL_BITMAPINFOHEADER;

typedef struct {
    uint8_t rgbtBlue;
    uint8_t rgbtGreen;
    uint8_t rgbtRed;
} AEL_RGBTRIPLE;

#pragma pack(pop)

/**
 * Creates a simulated mock 2x2 BMP file in memory/disk for demonstration
 */
void generate_mock_bmp(const char *filename) {
    FILE *out = fopen(filename, "wb");
    if (!out) {
        perror("Failed to generate mock BMP");
        return;
    }

    int width = 2;
    int height = 2;
    int padding = (4 - (width * sizeof(AEL_RGBTRIPLE)) % 4) % 4;

    AEL_BITMAPFILEHEADER bfh = {
        .bfType = 0x4D42, // 'BM'
        .bfSize = sizeof(AEL_BITMAPFILEHEADER) + sizeof(AEL_BITMAPINFOHEADER) + height * (width * sizeof(AEL_RGBTRIPLE) + padding),
        .bfReserved1 = 0,
        .bfReserved2 = 0,
        .bfOffBits = sizeof(AEL_BITMAPFILEHEADER) + sizeof(AEL_BITMAPINFOHEADER)
    };

    AEL_BITMAPINFOHEADER bih = {
        .biSize = sizeof(AEL_BITMAPINFOHEADER),
        .biWidth = width,
        .biHeight = height,
        .biPlanes = 1,
        .biBitCount = 24,
        .biCompression = 0,
        .biSizeImage = height * (width * sizeof(AEL_RGBTRIPLE) + padding),
        .biXPelsPerMeter = 2835,
        .biYPelsPerMeter = 2835,
        .biClrUsed = 0,
        .biClrImportant = 0
    };

    fwrite(&bfh, sizeof(AEL_BITMAPFILEHEADER), 1, out);
    fwrite(&bih, sizeof(AEL_BITMAPINFOHEADER), 1, out);

    // Write 2x2 pixels (Red, Green, Blue, White) + padding
    AEL_RGBTRIPLE p1 = {0, 0, 255};   // Red (BGR)
    AEL_RGBTRIPLE p2 = {0, 255, 0};   // Green
    AEL_RGBTRIPLE p3 = {255, 0, 0};   // Blue
    AEL_RGBTRIPLE p4 = {255, 255, 255}; // White
    uint8_t pad = 0;

    // Row 1
    fwrite(&p1, sizeof(AEL_RGBTRIPLE), 1, out);
    fwrite(&p2, sizeof(AEL_RGBTRIPLE), 1, out);
    for (int i = 0; i < padding; i++) fwrite(&pad, 1, 1, out);

    // Row 2
    fwrite(&p3, sizeof(AEL_RGBTRIPLE), 1, out);
    fwrite(&p4, sizeof(AEL_RGBTRIPLE), 1, out);
    for (int i = 0; i < padding; i++) fwrite(&pad, 1, 1, out);

    fclose(out);
}

int main(int argc, char *argv[]) {
    printf("\n=========================================================================\n");
    printf("             AEL BMP FORENSICS — BARE-METAL INVESTIGATION ENGINE\n");
    printf("=========================================================================\n\n");

    const char *infile = "mock_input.bmp";
    const char *outfile = "inverted_output.bmp";

    // Auto-generate mock file to guarantee execution out of the box
    generate_mock_bmp(infile);
    printf("[INIT] Auto-generated mock uncompressed BMP asset: %s\n", infile);

    FILE *in = fopen(infile, "rb");
    if (!in) {
        fprintf(stderr, "[FATAL] Failed to open input stream.\n");
        return 1;
    }

    FILE *out = fopen(outfile, "wb");
    if (!out) {
        fprintf(stderr, "[FATAL] Failed to allocate output stream.\n");
        fclose(in);
        return 1;
    }

    AEL_BITMAPFILEHEADER bfh;
    AEL_BITMAPINFOHEADER bih;

    // Read headers
    fread(&bfh, sizeof(AEL_BITMAPFILEHEADER), 1, in);
    fread(&bih, sizeof(AEL_BITMAPINFOHEADER), 1, in);

    // Verify Magic Number 'BM' (0x4D42)
    if (bfh.bfType != 0x4D42) {
        fprintf(stderr, "[REJECTED] File does not match valid BITMAP forensic magic signature.\n");
        fclose(in);
        fclose(out);
        return 1;
    }

    printf("\n[AEL AUDIT] --- METADATA HEADER DISSECTION ---\n");
    printf("  --> Magic Token       : BM (0x%X)\n", bfh.bfType);
    printf("  --> Aggregate Size    : %u bytes\n", bfh.bfSize);
    printf("  --> Pixel Matrix      : %d x %d\n", bih.biWidth, bih.biHeight);
    printf("  --> Color Depth       : %u-bit BGR\n", bih.biBitCount);

    // Write headers to output file
    fwrite(&bfh, sizeof(AEL_BITMAPFILEHEADER), 1, out);
    fwrite(&bih, sizeof(AEL_BITMAPINFOHEADER), 1, out);

    int padding = (4 - (bih.biWidth * sizeof(AEL_RGBTRIPLE)) % 4) % 4;

    printf("\n[PROCESS] Executing Bitwise Color Matrix Inversion (Negative Transform)...\n");

    for (int i = 0; i < abs(bih.biHeight); i++) {
        for (int j = 0; j < bih.biWidth; j++) {
            AEL_RGBTRIPLE triple;
            fread(&triple, sizeof(AEL_RGBTRIPLE), 1, in);

            // Bitwise 1's complement / inversion
            triple.rgbtBlue  = 255 - triple.rgbtBlue;
            triple.rgbtGreen = 255 - triple.rgbtGreen;
            triple.rgbtRed   = 255 - triple.rgbtRed;

            fwrite(&triple, sizeof(AEL_RGBTRIPLE), 1, out);
        }

        // Handle alignment padding
        for (int k = 0; k < padding; k++) {
            uint8_t pad;
            fread(&pad, 1, 1, in);
            fwrite(&pad, 1, 1, out);
        }
    }

    fclose(in);
    fclose(out);

    printf("[SUCCESS] Forensic extraction & inversion complete! Output dumped to %s ✅\n", outfile);
    printf("=========================================================================\n\n");

    return 0;
}
