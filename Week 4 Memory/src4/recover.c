#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define BLOCK_SIZE 512

// Standard CS50 forensic memory recovery of JPEGs from raw card images

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./recover card.raw\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        printf("Could not open file.\n");
        return 1;
    }

    uint8_t buffer[BLOCK_SIZE];
    int file_count = 0;
    FILE *img = NULL;
    char filename[16];

    while (fread(buffer, 1, BLOCK_SIZE, file) == BLOCK_SIZE) {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) {
            if (img != NULL) {
                fclose(img);
            }
            sprintf(filename, "%03d.jpg", file_count++);
            img = fopen(filename, "wb");
        }
        if (img != NULL) {
            fwrite(buffer, 1, BLOCK_SIZE, img);
        }
    }

    if (img != NULL) {
        fclose(img);
    }
    fclose(file);
    return 0;
}
