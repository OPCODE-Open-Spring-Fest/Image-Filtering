#include "helpers.h"
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"

int min(int a, int b) {
    return (a < b) ? a : b;
}
int max(int a, int b) {
    return (a > b) ? a : b;
}

void grayscale(int height, int width, RGBTRIPLE image[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;
            int avg = (red + green + blue) / 3;
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
}

void invert(int height, int width, RGBTRIPLE image[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;

            int invRed = 255 - red;
            int invGreen = 255 - green;
            int invBlue = 255 - blue;

            int originalBrightness = 0.299 * red + 0.587 * green + 0.114 * blue;
            int invertedBrightness = 0.299 * invRed + 0.587 * invGreen + 0.114 * invBlue;
            int brightnessDiff = originalBrightness - invertedBrightness;

            invRed = min(max(invRed + brightnessDiff, 0), 255);
            invGreen = min(max(invGreen + brightnessDiff, 0), 255);
            invBlue = min(max(invBlue + brightnessDiff, 0), 255);

            image[i][j].rgbtRed = invRed;
            image[i][j].rgbtGreen = invGreen;
            image[i][j].rgbtBlue = invBlue;
        }
    }
}

void sepia(int height, int width, RGBTRIPLE image[height][width]) {
    // TODO: Implement sepia filter (currently empty)
}

void reflect(int height, int width, RGBTRIPLE image[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width / 2; j++) {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
}

void blur(int height, int width, RGBTRIPLE image[height][width]) {
    RGBTRIPLE **temp = malloc(height * sizeof(RGBTRIPLE *));
    for (int i = 0; i < height; i++)
        temp[i] = malloc(width * sizeof(RGBTRIPLE));

    int kernelSize = 21;
    int offset = kernelSize / 2;
    for (int repeat = 0; repeat < 3; repeat++) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int sumRed = 0, sumGreen = 0, sumBlue = 0, count = 0;
                for (int ki = -offset; ki <= offset; ki++) {
                    for (int kj = -offset; kj <= offset; kj++) {
                        int ni = i + ki;
                        int nj = j + kj;
                        if (ni >= 0 && ni < height && nj >= 0 && nj < width) {
                            sumRed += image[ni][nj].rgbtRed;
                            sumGreen += image[ni][nj].rgbtGreen;
                            sumBlue += image[ni][nj].rgbtBlue;
                            count++;
                        }
                    }
                }
                temp[i][j].rgbtRed = (uint8_t)(sumRed / count);
                temp[i][j].rgbtGreen = (uint8_t)(sumGreen / count);
                temp[i][j].rgbtBlue = (uint8_t)(sumBlue / count);
            }
        }
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                image[i][j] = temp[i][j];
    }
    for (int i = 0; i < height; i++)
        free(temp[i]);
    free(temp);
}

void brightness(int height, int width, RGBTRIPLE image[height][width], int value) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int r = image[i][j].rgbtRed + value;
            int g = image[i][j].rgbtGreen + value;
            int b = image[i][j].rgbtBlue + value;
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;
            if (r < 0) r = 0;
            if (g < 0) g = 0;
            if (b < 0) b = 0;
            image[i][j].rgbtRed = r;
            image[i][j].rgbtGreen = g;
            image[i][j].rgbtBlue = b;
        }
    }
}

void vignette(int height, int width, RGBTRIPLE image[height][width]) {
    float cx = width / 2.0;
    float cy = height / 2.0;
    float max_dis = sqrt(cx * cx + cy * cy);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float disx = j - cx;
            float disy = i - cy;
            float dist = sqrt(disx * disx + disy * disy);
            float vig = 1.0 - (dist / max_dis);
            if (vig < 0.0) vig = 0.0;
            if (vig > 1.0) vig = 1.0;
            image[i][j].rgbtRed = (int)(image[i][j].rgbtRed * vig);
            image[i][j].rgbtGreen = (int)(image[i][j].rgbtGreen * vig);
            image[i][j].rgbtBlue = (int)(image[i][j].rgbtBlue * vig);
        }
    }
}