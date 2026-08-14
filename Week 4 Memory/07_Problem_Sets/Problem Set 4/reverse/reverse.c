#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 3)
    {
        printf("Usage: reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    // Read header
    WAVHEADER header;
    fread(&header, sizeof(WAVHEADER), 1, input);

    // Use check_format to ensure WAV format
    if (check_format(header) == 0)
    {
        printf("Not a  Wave File\n");
        return 1;
    }

    // Open output file for writing
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open %s.\n", argv[2]);
        return 1;
    }

    // Write header to file
    fwrite(&header, sizeof(WAVHEADER), 1, output);

    // Use get_block_size to calculate size of block
    int size = get_block_size(header);

    // Write reversed audio to file
    if (fseek(input, size, SEEK_END))
    {
        return 1;
    }
    BYTE buffer[size];
    while (ftell(input) - size > sizeof(header))
    {
        if (fseek(input, -2 * size, SEEK_CUR))
        {
            return 1;
        }
        fread(buffer, size, 1, input);
        fwrite(buffer, size, 1, output);
    }

    fclose(input);
    fclose(output);
}

int check_format(WAVHEADER header)
{
    if (header.format[0] == 'W' && header.format[1] == 'A' && header.format[2] == 'V' && header.format[3] == 'E')
    {
        return 1;
    }
    return 0;
}

int get_block_size(WAVHEADER header)
{
    int block_size = header.numChannels * header.bitsPerSample / 8;  // (bits / 8) = bytes
    return block_size;
}