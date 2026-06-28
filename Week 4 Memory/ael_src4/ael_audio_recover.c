/**
 * AEL Audio Recover — Bare-Metal Raw Byte Scanner for Carving WAV/RIFF Files
 * Built for CS50x 2026-2027 | AEL CS Encyclopedia
 * 
 * © Ayman Elmasry | AEL Digital Studio™
 * Enforcing the strict directive: 100% Original Proprietary Enterprise Code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define BLOCK_SIZE 512

/**
 * Simulates a corrupted raw disk dump containing random byte streams + a hidden WAV magic signature
 */
void generate_corrupted_raw_dump(const char *filename) {
    FILE *dump = fopen(filename, "wb");
    if (!dump) {
        perror("Failed to allocate raw dump stream");
        return;
    }

    uint8_t noise[BLOCK_SIZE];
    for (int i = 0; i < BLOCK_SIZE; i++) noise[i] = (uint8_t)(rand() % 256);

    // Write 2 blocks of random garbage
    fwrite(noise, BLOCK_SIZE, 1, dump);
    fwrite(noise, BLOCK_SIZE, 1, dump);

    // Block 3: Inject valid WAV/RIFF Magic Numbers
    // Magic: 'RIFF' (0x52, 0x49, 0x46, 0x46) ... 'WAVE' at offset 8 (0x57, 0x41, 0x56, 0x45)
    uint8_t wav_header[BLOCK_SIZE];
    for (int i = 0; i < BLOCK_SIZE; i++) wav_header[i] = (uint8_t)(i % 256); // Mock payload

    wav_header[0] = 0x52; // 'R'
    wav_header[1] = 0x49; // 'I'
    wav_header[2] = 0x46; // 'F'
    wav_header[3] = 0x46; // 'F'

    // 4 bytes size (mocked)
    wav_header[4] = 0x00; wav_header[5] = 0x10; wav_header[6] = 0x00; wav_header[7] = 0x00;

    wav_header[8] = 0x57;  // 'W'
    wav_header[9] = 0x41;  // 'A'
    wav_header[10] = 0x56; // 'V'
    wav_header[11] = 0x45; // 'E'

    fwrite(wav_header, BLOCK_SIZE, 1, dump);

    // Write 1 more mock data block belonging to the WAV file
    fwrite(noise, BLOCK_SIZE, 1, dump);

    fclose(dump);
}

int main(int argc, char *argv[]) {
    printf("\n=========================================================================\n");
    printf("             AEL AUDIO RECOVER — BARE-METAL FILE CARVING ENGINE\n");
    printf("=========================================================================\n\n");

    const char *dumpfile = "corrupted_disk.raw";
    generate_corrupted_raw_dump(dumpfile);
    printf("[INIT] Corrupted raw memory dump allocated: %s\n", dumpfile);

    FILE *in = fopen(dumpfile, "rb");
    if (!in) {
        fprintf(stderr, "[FATAL] Failed to interface with raw storage block.\n");
        return 1;
    }

    uint8_t buffer[BLOCK_SIZE];
    FILE *out = NULL;
    int file_count = 0;
    char out_filename[64];

    printf("[PROCESS] Scanning raw unindexed memory blocks for WAV/RIFF Magic Numbers...\n\n");

    int block_num = 0;
    while (fread(buffer, 1, BLOCK_SIZE, in) == BLOCK_SIZE) {
        block_num++;
        
        // Audit first 12 bytes for RIFF ... WAVE signature
        bool is_riff = (buffer[0] == 0x52 && buffer[1] == 0x49 && buffer[2] == 0x46 && buffer[3] == 0x46);
        bool is_wave = (buffer[8] == 0x57 && buffer[9] == 0x41 && buffer[10] == 0x56 && buffer[11] == 0x45);

        if (is_riff && is_wave) {
            printf("[AEL CARVER] Magic Number Signature Match at Block %d! (0x52494646...0x57415645)\n", block_num);
            
            if (out != NULL) {
                fclose(out);
            }

            sprintf(out_filename, "recovered_audio_%03d.wav", file_count++);
            out = fopen(out_filename, "wb");
            if (!out) {
                fprintf(stderr, "[FATAL] Failed to establish output stream for carved audio.\n");
                fclose(in);
                return 1;
            }

            printf("  --> Allocated clean restoration stream: %s\n", out_filename);
        }

        // If an output stream is active, dump current block payload
        if (out != NULL) {
            fwrite(buffer, 1, BLOCK_SIZE, out);
        }
    }

    if (out != NULL) {
        fclose(out);
    }
    fclose(in);

    printf("\n[SUCCESS] Forensic File Carving Complete! Carved %d functional WAV asset(s). ✅\n", file_count);
    printf("=========================================================================\n\n");

    return 0;
}
