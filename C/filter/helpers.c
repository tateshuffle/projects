#include "helpers.h"

#include<math.h>

void swap(RGBTRIPLE *a, RGBTRIPLE *b);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // loop through image's rows
    for (int i = 0; i < height; i++)
    {
        // loop through row's pixels
        for (int j = 0; j < width; j++)
        {
            // calculate avg of pixel's RGB values
            int avg = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            // apply avg as value to each color channel of pixel
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // loop through image's rows
    for (int i = 0; i < height; i++)
    {
        // loop through row's pixels
        for (int j = 0; j < width - 1; j++)
        {
            // if width is odd value
            if (width % 2 == 1)
            {
                // swap all pixels
                swap(&image[i][j], &image[i][width - 1 - j]);
            }
            // if width is even value
            else
            {
                // split width
                int half = width / 2;
                //swap pixels in left half of width
                for (int k = 0; k < half; k++)
                {
                    swap(&image[i][k], &image[i][half - 1 - k]);
                }
                //swap pixels in right half of width
                for (int l = half; l < width; l++)
                {
                    swap(&image[i][l], &image[i][width - 1 - l]);
                }
            }
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // create copy of image
    RGBTRIPLE copy[height][width];
    // variables to hold color averages
    int avgRed, avgGreen, avgBlue;

    // loop through image's rows
    for (int i = 0; i < height; i++)
    {
        // loop through row's pixels
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];

            // first line
            if (i == 0)
            {
                // upper left corner
                if (j == 0)
                {
                    avgRed = round((image[i][j].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j].rgbtRed +
                                    image[i + 1][j + 1].rgbtRed) / 4.0);
                    avgGreen = round((image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j].rgbtGreen +
                                      image[i + 1][j + 1].rgbtGreen) / 4.0);
                    avgBlue = round((image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j].rgbtBlue +
                                     image[i + 1][j + 1].rgbtBlue) / 4.0);
                }
                // upper right corner
                if (j == width - 1)
                {
                    avgRed = round((image[i][j].rgbtRed + image[i][j - 1].rgbtRed + image[i + 1][j - 1].rgbtRed +
                                    image[i + 1][j].rgbtRed) / 4.0);
                    avgGreen = round((image[i][j].rgbtGreen + image[i][j - 1].rgbtGreen + image[i + 1][j - 1].rgbtGreen +
                                      image[i + 1][j].rgbtGreen) / 4.0);
                    avgBlue = round((image[i][j].rgbtBlue + image[i][j - 1].rgbtBlue + image[i + 1][j - 1].rgbtBlue +
                                     image[i + 1][j].rgbtBlue) / 4.0);
                }
                else if (i == 0 && j > 0 && j < width - 1)
                {
                    avgRed = round((image[i][j].rgbtRed + image[i][j - 1].rgbtRed + image[i + 1][j - 1].rgbtRed +
                                    image[i + 1][j].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j + 1].rgbtRed) / 6.0);
                    avgGreen = round((image[i][j].rgbtGreen + image[i][j - 1].rgbtGreen + image[i + 1][j - 1].rgbtGreen +
                                      image[i + 1][j].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 6.0);
                    avgBlue = round((image[i][j].rgbtBlue + image[i][j - 1].rgbtBlue + image[i + 1][j - 1].rgbtBlue +
                                     image[i + 1][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue) / 6.0);
                }
            }
            // first column
            else if (i > 0 && i < height - 1 && j == 0)
            {
                avgRed = round((image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i][j].rgbtRed +
                                image[i][j + 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed) / 6.0);
                avgGreen = round((image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j].rgbtGreen +
                                  image[i][j + 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 6.0);
                avgBlue = round((image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j].rgbtBlue +
                                 image[i][j + 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue) / 6.0);
            }
            // last column
            else if (i > 0 && i < height - 1 && j == width - 1)
            {
                avgRed = round((image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i][j - 1].rgbtRed +
                                image[i][j].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed) / 6.0);
                avgGreen = round((image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i][j - 1].rgbtGreen +
                                  image[i][j].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen) / 6.0);
                avgBlue = round((image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i][j - 1].rgbtBlue +
                                 image[i][j].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue) / 6.0);
            }
            // middle pixels
            else if (i > 0  && i < height - 1 && j > 0 && j < width - 1)
            {
                avgRed = round((image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed +
                                image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed +
                                image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed) / 9.0);
                avgGreen = round((image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen +
                                  image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen +
                                  image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 9.0);
                avgBlue = round((image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue +
                                 image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue +
                                 image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue) / 9.0);
            }
            // last line
            if (i == height - 1)
            {
                //upper right corner
                if (j == 0)
                {
                    avgRed = round((image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed +  image[i][j].rgbtRed +
                                    image[i][j + 1].rgbtRed) / 4.0);
                    avgGreen = round((image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen +  image[i][j].rgbtGreen +
                                      image[i][j + 1].rgbtGreen) / 4.0);
                    avgBlue = round((image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue +  image[i][j].rgbtBlue +
                                     image[i][j + 1].rgbtBlue) / 4.0);
                }
                //bottom right corner
                if (j == width - 1)
                {
                    avgRed = round((image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i][j - 1].rgbtRed +
                                    image[i][j].rgbtRed) / 4.0);
                    avgGreen = round((image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i][j - 1].rgbtGreen +
                                      image[i][j].rgbtGreen) / 4.0);
                    avgBlue = round((image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i][j - 1].rgbtBlue +
                                     image[i][j].rgbtBlue) / 4.0);
                }
                else if (j > 0 && j < width - 1)
                {
                    avgRed = round((image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed +
                                    image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed) / 6.0);
                    avgGreen = round((image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen +
                                      image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen) / 6.0);
                    avgBlue = round((image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue +
                                     image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue) / 6.0);
                }
            }
            // apply color averages to current pixel
            copy[i][j].rgbtRed = avgRed;
            copy[i][j].rgbtGreen = avgGreen;
            copy[i][j].rgbtBlue = avgBlue;
        }
    }

    // copy altered pixels to original image
    for (int row = 0; row < height; row++)
    {
        for (int pixel = 0; pixel < width; pixel++)
        {
            image[row][pixel] = copy[row][pixel];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // create copy of image with black border
    RGBTRIPLE copy[height + 2][width + 2];
    for (int i = 0; i < height + 2; i++)
    {
        for (int j = 0; j < width + 2; j++)
        {
            if (i == 0 || j == 0 || i == height + 1 || j == width + 1)
            {
                copy[i][j].rgbtRed = 0;
                copy[i][j].rgbtGreen = 0;
                copy[i][j].rgbtBlue = 0;
            }
            else
            {
                copy[i][j] = image[i - 1][j - 1];
            }
        }
    }

    // Gx and Gy kernels
    int Gx [3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy [3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // loop through pixels of original image
    for (int row = 1; row < height + 1; row++)
    {
        for (int pixel = 1; pixel < width + 1; pixel++)
        {
            int gxR = 0;
            int gyR = 0;
            int gxG = 0;
            int gyG = 0;
            int gxB = 0;
            int gyB = 0;

            // create 3 X 3 grid
            for (int grid_h = row - 1; grid_h <= row + 1; grid_h++)
            {
                for (int grid_w = pixel - 1; grid_w <= pixel + 1; grid_w++)
                {
                    // calculare Gx and Gy for each color channel and cap values at 255
                    gxR += copy[grid_h][grid_w].rgbtRed * Gx[grid_h - row + 1][grid_w - pixel + 1];
                    gyR += copy[grid_h][grid_w].rgbtRed * Gy[grid_h - row + 1][grid_w - pixel + 1];
                    if (gxR > 255)
                    {
                        gxR = 255;
                    }
                    if (gyR > 255)
                    {
                        gyR = 255;
                    }

                    gxG += copy[grid_h][grid_w].rgbtGreen * Gx[grid_h - row + 1][grid_w - pixel + 1];
                    gyG += copy[grid_h][grid_w].rgbtGreen * Gy[grid_h - row + 1][grid_w - pixel + 1];
                    if (gxG > 255)
                    {
                        gxG = 255;
                    }
                    if (gyG > 255)
                    {
                        gyG = 255;
                    }

                    gxB += copy[grid_h][grid_w].rgbtBlue * Gx[grid_h - row + 1][grid_w - pixel + 1];
                    gyB += copy[grid_h][grid_w].rgbtBlue * Gy[grid_h - row + 1][grid_w - pixel + 1];
                    if (gxB > 255)
                    {
                        gxB = 255;
                    }
                    if (gyB > 255)
                    {
                        gyB = 255;
                    }

                    // get sum of each color channel and cap values at 255
                    int red_sum = round(sqrt(pow(gxR, 2) + pow(gyR, 2)));
                    if (red_sum > 255)
                    {
                        red_sum = 255;
                    }
                    int green_sum = round(sqrt(pow(gxG, 2) + pow(gyG, 2)));
                    if (green_sum > 255)
                    {
                        green_sum = 255;
                    }
                    int blue_sum = round(sqrt(pow(gxB, 2) + pow(gyB, 2)));
                    if (blue_sum > 255)
                    {
                        blue_sum = 255;
                    }

                    // apply altered values to original image
                    image[row - 1][pixel - 1].rgbtRed = red_sum;
                    image[row - 1][pixel - 1].rgbtGreen = green_sum;
                    image[row - 1][pixel - 1].rgbtBlue = blue_sum;

                }
            }
        }
    }
    return;
}

void swap(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE temp = *a;
    *a = *b;
    *b = temp;
}