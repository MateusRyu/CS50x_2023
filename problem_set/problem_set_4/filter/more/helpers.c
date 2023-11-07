#include <math.h>
#include <stdio.h>

#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int red = image[h][w].rgbtRed;
            int green = image[h][w].rgbtGreen;
            int blue = image[h][w].rgbtBlue;

            // take the average of the red, green, and blue values
            int average = round((red + green + blue) / (float) 3);

            // Determine what shade of grey based on the average
            image[h][w].rgbtRed = average;
            image[h][w].rgbtGreen = average;
            image[h][w].rgbtBlue = average;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        // Create an array to store the array reversed for each line
        RGBTRIPLE line[width];
        // The last pixel of an row is row[width-1]
        for (int w = 1; w <= width; w++)
        {
            line[width - w] = image[h][w - 1];
        }

        // Replace the current line with the reversed one
        for (int i = 0; i < width; i++)
        {
            image[h][i] = line[i];
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy to avoid losing data during image reconstruction
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
            copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
            copy[i][j].rgbtRed = image[i][j].rgbtRed;
        }
    }

    // Loop through each pixel of the image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Reset the data to calculate the new pixel later
            int box_size = 0;
            int blue = 0;
            int green = 0;
            int red = 0;

            // Loop through each pixel of the box blur
            for (int x = h - 1; x < h + 2; x++)
            {
                for (int y = w - 1; y < w + 2; y++)
                {
                    // If pixel of the box blur isn't outside of image
                    if (x >= 0 && x <= height - 1 && y >= 0 && y <= width - 1)
                    {
                        blue += copy[x][y].rgbtBlue;
                        green += copy[x][y].rgbtGreen;
                        red += copy[x][y].rgbtRed;
                        box_size++;
                    }
                }
            }

            image[h][w].rgbtBlue = round(blue / (float) box_size);
            image[h][w].rgbtGreen = round(green / (float) box_size);
            image[h][w].rgbtRed = round(red / (float) box_size);
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Gx and Gy are two "kernels"
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Create a copy to avoid losing data during image reconstruction
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
            copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
            copy[i][j].rgbtRed = image[i][j].rgbtRed;
        }
    }

    // Loop through each pixel of the image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Reset the data to calculate the new pixel later
            int blue[3] = {0, 0, 0};
            int green[3] = {0, 0, 0};
            int red[3] = {0, 0, 0};

            for (int x = h - 1; x < h + 2; x++)
            {
                for (int y = w - 1; y < w + 2; y++)
                {
                    // If pixel of the box blur isn't outside of image
                    if (x >= 0 && x <= height - 1 && y >= 0 && y <= width - 1)
                    {
                        blue[0] += copy[x][y].rgbtBlue * Gx[x - h + 1][y - w + 1];
                        green[0] += copy[x][y].rgbtGreen * Gx[x - h + 1][y - w + 1];
                        red[0] += copy[x][y].rgbtRed * Gx[x - h + 1][y - w + 1];

                        blue[1] += copy[x][y].rgbtBlue * Gy[x - h + 1][y - w + 1];
                        green[1] += copy[x][y].rgbtGreen * Gy[x - h + 1][y - w + 1];
                        red[1] += copy[x][y].rgbtRed * Gy[x - h + 1][y - w + 1];
                    }
                }
            }

            blue[2] = round(sqrt(blue[0] * blue[0] + blue[1] * blue[1]));
            green[2] = round(sqrt(green[0] * green[0] + green[1] * green[1]));
            red[2] = round(sqrt(red[0] * red[0] + red[1] * red[1]));

            if (blue[2] > 255)
            {
                image[h][w].rgbtBlue = 255;
            }
            else
            {
                image[h][w].rgbtBlue = blue[2];
            }

            if (green[2] > 255)
            {
                image[h][w].rgbtGreen = 255;
            }
            else
            {
                image[h][w].rgbtGreen = green[2];
            }

            if (red[2] > 255)
            {
                image[h][w].rgbtRed = 255;
            }
            else
            {
                image[h][w].rgbtRed = red[2];
            }
        }
    }
}