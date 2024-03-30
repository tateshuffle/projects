#include <stdlib.h>

#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    // Change all black pixels to a color of your choosing
    // create red pixel
    RGBTRIPLE *pixel = malloc(sizeof(RGBTRIPLE));
    pixel[0].rgbtRed = 0xff;
    pixel[0].rgbtGreen = 0x00;
    pixel[0].rgbtBlue = 0x00;

    // loop through image height
    for (int i = 0; i < height; i++)
    {
        // loop through image width
        for (int j = 0; j < width; j++)
        {
            // find black pixels
            if (image[i][j].rgbtRed == 0x00 && image[i][j].rgbtGreen == 0x00 && image[i][j].rgbtBlue == 0x00)
            {
                // switch black to red
                image[i][j] = pixel[0];
            }
        }
    }
    free(pixel);
}
