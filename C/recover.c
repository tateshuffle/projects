#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// create type for byte
typedef uint8_t byte;
// define block size
const int block = 512;

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./recover filename\n");
        return 1;
    }

    // open memory card
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        return 2;
    }

    // create block-sized buffer
    byte *buffer = malloc(block * sizeof(byte));
    if (buffer == NULL)
    {
        free(buffer);
        return 3;
    }

    // allocate memory for file names
    char *filename = malloc(8 * sizeof(char));
    if (filename == NULL)
    {
        free(filename);
        return 4;
    }

    // create empty file to store JPEGs
    FILE *img = NULL;

    // keep track of JPEGs
    int jpg = 0;

    // read memory card in blocks of 512B
    while (fread(buffer, 1, block, card) == block)
    {
        // look for JPEG header
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (img != NULL)
            {
                fclose(img);
            }
            // name JPEG file
            sprintf(filename, "%03i.jpg", jpg);
            // create JPEG
            img = fopen(filename, "w");
            // write to JPEG
            fwrite(buffer, 1, block, img);
            
            jpg++;
        }
        else
        {
            if (jpg != 0)
            {
                // keep writing to current JPEG
                fwrite(buffer, 1, block, img);
            }
        }
    }
    fclose(img);
    free(filename);
    free(buffer);
    fclose(card);
}
