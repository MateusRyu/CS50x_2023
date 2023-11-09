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
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Read header
    WAVHEADER header;
    fread(&header.chunkID, 1, 4, input);
    fread(&header.chunkSize, 4, 1, input);
    fread(&header.format, 1, 4, input);
    fread(&header.subchunk1ID, 1, 4, input);
    fread(&header.subchunk1Size, 4, 1, input);
    fread(&header.audioFormat, 2, 1, input);
    fread(&header.numChannels, 2, 1, input);
    fread(&header.sampleRate, 4, 1, input);
    fread(&header.byteRate, 4, 1, input);
    fread(&header.blockAlign, 2, 1, input);
    fread(&header.bitsPerSample, 2, 1, input);
    fread(&header.subchunk2ID, 1, 4, input);
    fread(&header.subchunk2Size, 4, 1, input);

    // Use check_format to ensure WAV format
    if (check_format(header) == 0)
    {
        printf("Input is not a WAV file.\n");
        return 1;
    }

    // Open output file for writing
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Write header to file
    fwrite(&header.chunkID, 1, 4, output);
    fwrite(&header.chunkSize, 4, 1, output);
    fwrite(&header.format, 1, 4, output);
    fwrite(&header.subchunk1ID, 1, 4, output);
    fwrite(&header.subchunk1Size, 4, 1, output);
    fwrite(&header.audioFormat, 2, 1, output);
    fwrite(&header.numChannels, 2, 1, output);
    fwrite(&header.sampleRate, 4, 1, output);
    fwrite(&header.byteRate, 4, 1, output);
    fwrite(&header.blockAlign, 2, 1, output);
    fwrite(&header.bitsPerSample, 2, 1, output);
    fwrite(&header.subchunk2ID, 1, 4, output);
    fwrite(&header.subchunk2Size, 4, 1, output);

    // Use get_block_size to calculate size of block
    int block_size = get_block_size(header);

    // Write reversed audio to file
    BYTE buffer[block_size];

    // Move input Pointer to the end
    fseek(input, 0, SEEK_END);

    // Finding the Audio Size (excluding the header)
    int header_size = sizeof(WAVHEADER);
    long audio_size = ftell(input) - header_size;
    int max_samples = (int) audio_size / block_size;

    // Loop in through moving backwards,
    for (int i = max_samples - 1; i >= 0; i--)
    {
        // Set pointer of input file to the start of sample i
        fseek(input, header_size + i * block_size, SEEK_SET);
        // Read sample i into buffer
        fread(buffer, block_size, 1, input);
        // Copy sample i into output file
        fwrite(buffer, block_size, 1, output);
    }

    // close files
    fclose(input);
    fclose(output);
}

int check_format(WAVHEADER header)
{
    int check = 1;
    char *format = "WAVE";
    for (int i = 0; i < 4; i++)
    {
        if (header.format[i] != format[i])
        {
            check = 0;
        }
    }

    return check;
}

int get_block_size(WAVHEADER header)
{
    // 1 bit = 1/8 byte
    int block_size = header.numChannels * header.bitsPerSample / 8;
    return block_size;
}