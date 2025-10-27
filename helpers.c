#include "helpers.h"
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"
#include <string.h>

#define LEVELS 16
#define RADIUS 8

int min(int a,int b){
    if(a<b) return a;
    return b;
}
int max(int a,int b){
    if(a>b) return a;
    return b;
}

void grayscale(int height, int width, RGBTRIPLE image[height][width]){
   
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
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


void invert(int height, int width, RGBTRIPLE image[height][width]){
    for(int i = 0; i<height; i++){
        for(int j = 0; j<width; j++){
            //Original RGB
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;

            //Normal inversion
            int invRed = 255 - red;
            int invGreen = 255 - green;
            int invBlue = 255 - blue;

            //for maintain average brightness
            int originalBrightness = 0.299*red + 0.587*green + 0.114*blue;
            int invertedBrightness = 0.299*invRed + 0.587*invGreen + 0.114*invBlue;
            int brightnessDiff = originalBrightness - invertedBrightness;

            invRed = min(max(invRed + brightnessDiff, 0), 255);
            invGreen = min(max(invGreen + brightnessDiff, 0), 255);
            invBlue = min(max(invBlue + brightnessDiff, 0), 255);

            //Assign
            image[i][j].rgbtRed = invRed;
            image[i][j].rgbtGreen = invGreen;
            image[i][j].rgbtBlue = invBlue;
        }
    }
    return;
    
// Convert image to grayscale

}
void sepia(int height, int width, RGBTRIPLE image[height][width]){

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int originalRed = image[i][j].rgbtRed;
            int originalGreen = image[i][j].rgbtGreen;
            int originalBlue = image[i][j].rgbtBlue;

            int sepiaRed = round(0.393 * originalRed + 0.769 * originalGreen + 0.189 * originalBlue);
            int sepiaGreen = round(0.349 * originalRed + 0.686 * originalGreen + 0.168 * originalBlue);
            int sepiaBlue = round(0.272 * originalRed + 0.534 * originalGreen + 0.131 * originalBlue);

            if (sepiaRed > 255)
                sepiaRed = 255;
            if (sepiaGreen > 255)
                sepiaGreen = 255;
            if (sepiaBlue > 255)
                sepiaBlue = 255;

            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }

}


void reflect(int height, int width, RGBTRIPLE image[height][width]){

    // Loop over each row
    for (int i = 0; i < height; i++)
    {
        // Swap pixels horizontally (mirror)
        for (int j = 0; j < width / 2; j++)
        {
            // Swap left pixel with right pixel
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }

}


void blur(int height, int width, RGBTRIPLE image[height][width]){
    // Allocate temporary array on heap
    RGBTRIPLE **temp = malloc(height * sizeof(RGBTRIPLE *));
    for (int i = 0; i < height; i++)
        temp[i] = malloc(width * sizeof(RGBTRIPLE));

    int kernelSize = 21;  // large kernel for heavy blur
    int offset = kernelSize / 2;
    // Repeating blur 2-3 times for ultra blur effect 
    //because in single time effect not much visible
    for(int repeat = 0; repeat < 3; repeat++){
        for(int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                int sumRed = 0, sumGreen = 0, sumBlue = 0;
                int count = 0;
                for (int ki = -offset; ki <= offset; ki++){
                    for(int kj = -offset; kj <= offset; kj++){
                        int ni = i + ki;
                        int nj = j + kj;
                        if(ni >= 0 && ni < height && nj >= 0 && nj < width){
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
        // Copy blurred array back to orig
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

void vignette(int height, int width, RGBTRIPLE image[height][width]){
    float cx = width / 2.0; //  center of the image
    float cy= height / 2.0;
    float max_dis= sqrt(cx * cx + cy * cy);
    for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
            float disx = j - cx;
            float disy = i - cy;
            float dist= sqrt(disx * disx + disy * disy);
            //  (0.0 = dark, 1.0 = og)
            float vig = 1.0 - (dist/ max_dis);
            if(vig< 0.0) vig = 0.0;
            if(vig > 1.0) vig = 1.0;
            image[i][j].rgbtRed = (int)(image[i][j].rgbtRed * vig);
            image[i][j].rgbtGreen = (int)(image[i][j].rgbtGreen * vig);
            image[i][j].rgbtBlue = (int)(image[i][j].rgbtBlue * vig);
        }
    }
}

//Threshold Filter(Black & White)
void threshold(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avg = round((image[i][j].rgbtRed +
                             image[i][j].rgbtGreen +
                             image[i][j].rgbtBlue) / 3.0);

            if (avg >= 128)
            {
                image[i][j].rgbtRed = 255;
                image[i][j].rgbtGreen = 255;
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtRed = 0;
                image[i][j].rgbtGreen = 0;
                image[i][j].rgbtBlue = 0;
            }
        }
    }
}
void detect_edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Temporary copy of the image
    RGBTRIPLE **copy = malloc(height * sizeof(RGBTRIPLE *));
    for (int i = 0; i < height; i++)
        copy[i] = malloc(width * sizeof(RGBTRIPLE));

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            copy[i][j] = image[i][j];

    // Sobel kernels
    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int Gy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumRx = 0, sumGx = 0, sumBx = 0;
            int sumRy = 0, sumGy = 0, sumBy = 0;

            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;

                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        RGBTRIPLE pixel = copy[ni][nj];
                        int kx = Gx[di + 1][dj + 1];
                        int ky = Gy[di + 1][dj + 1];

                        sumRx += pixel.rgbtRed * kx;
                        sumGx += pixel.rgbtGreen * kx;
                        sumBx += pixel.rgbtBlue * kx;

                        sumRy += pixel.rgbtRed * ky;
                        sumGy += pixel.rgbtGreen * ky;
                        sumBy += pixel.rgbtBlue * ky;
                    }
                }
            }

            // Calculate gradient magnitude and clamp
            int red   = min(max((int)round(sqrt(sumRx*sumRx + sumRy*sumRy)), 0), 255);
            int green = min(max((int)round(sqrt(sumGx*sumGx + sumGy*sumGy)), 0), 255);
            int blue  = min(max((int)round(sqrt(sumBx*sumBx + sumBy*sumBy)), 0), 255);

            image[i][j].rgbtRed   = red;
            image[i][j].rgbtGreen = green;
            image[i][j].rgbtBlue  = blue;
        }
    }

    // Free temporary array
    for (int i = 0; i < height; i++)
        free(copy[i]);
    free(copy);
}
void glow(int height, int width, RGBTRIPLE image[height][width])
{
    // Step 1: make a copy of the original
    RGBTRIPLE **original = malloc(height * sizeof(RGBTRIPLE *));
    RGBTRIPLE **blurred = malloc(height * sizeof(RGBTRIPLE *));
    for (int i = 0; i < height; i++)
    {
        original[i] = malloc(width * sizeof(RGBTRIPLE));
        blurred[i] = malloc(width * sizeof(RGBTRIPLE));
        for (int j = 0; j < width; j++)
        {
            original[i][j] = image[i][j];
        }
    }

    // Step 2: apply a *mild* blur to copy (smaller kernel)
    int kernelSize = 11;
    int offset = kernelSize / 2;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumRed = 0, sumGreen = 0, sumBlue = 0, count = 0;

            for (int ki = -offset; ki <= offset; ki++)
            {
                for (int kj = -offset; kj <= offset; kj++)
                {
                    int ni = i + ki, nj = j + kj;
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        sumRed += original[ni][nj].rgbtRed;
                        sumGreen += original[ni][nj].rgbtGreen;
                        sumBlue += original[ni][nj].rgbtBlue;
                        count++;
                    }
                }
            }

            blurred[i][j].rgbtRed = sumRed / count;
            blurred[i][j].rgbtGreen = sumGreen / count;
            blurred[i][j].rgbtBlue = sumBlue / count;
        }
    }

    // Step 3: blend original + blurred to produce glow
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float blend = 0.3; // glow intensity
            int newRed = (1 - blend) * original[i][j].rgbtRed + blend * blurred[i][j].rgbtRed;
            int newGreen = (1 - blend) * original[i][j].rgbtGreen + blend * blurred[i][j].rgbtGreen;
            int newBlue = (1 - blend) * original[i][j].rgbtBlue + blend * blurred[i][j].rgbtBlue;

            image[i][j].rgbtRed = min(255, newRed);
            image[i][j].rgbtGreen = min(255, newGreen);
            image[i][j].rgbtBlue = min(255, newBlue);
        }
    }

    // Step 4: cleanup
    for (int i = 0; i < height; i++)
    {
        free(original[i]);
        free(blurred[i]);
    }
    free(original);
    free(blurred);
}
// Oil Paint filter
void oilpaint(int height, int width, RGBTRIPLE image[height][width]){ 
     RGBTRIPLE **copy = malloc(height * sizeof(RGBTRIPLE *));
    for (int i = 0; i < height; i++){
        copy[i] = malloc(width * sizeof(RGBTRIPLE));
        for (int j = 0; j < width; j++){
            copy[i][j] = image[i][j];
        }
    }
    // logic
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            int intensityCount[LEVELS];
            long sumRed[LEVELS], sumGreen[LEVELS], sumBlue[LEVELS];
            memset(intensityCount, 0, sizeof(intensityCount));
            memset(sumRed, 0, sizeof(sumRed));
            memset(sumGreen, 0, sizeof(sumGreen));
            memset(sumBlue, 0, sizeof(sumBlue));

            // Analyze neighborhood
            for (int di = -RADIUS; di <= RADIUS; di++){
                for (int dj = -RADIUS; dj <= RADIUS; dj++){
                    int ni = i + di;
                    int nj = j + dj;
                    if (ni < 0 || ni >= height || nj < 0 || nj >= width)
                        continue;

                    RGBTRIPLE pixel = copy[ni][nj];
                    int intensity = ((pixel.rgbtRed + pixel.rgbtGreen + pixel.rgbtBlue) / 3) * LEVELS / 256;
                    if (intensity >= LEVELS)
                        intensity = LEVELS - 1;

                    intensityCount[intensity]++;
                    sumRed[intensity] += pixel.rgbtRed;
                    sumGreen[intensity] += pixel.rgbtGreen;
                    sumBlue[intensity] += pixel.rgbtBlue;
                }
            }

            int dominant = 0, maxCount = 0;
            for (int k = 0; k < LEVELS; k++){
                if (intensityCount[k] > maxCount){
                    maxCount = intensityCount[k];
                    dominant = k;
                }
            }
            if (maxCount > 0){
                int newRed = sumRed[dominant] / maxCount;
                int newGreen = sumGreen[dominant] / maxCount;
                int newBlue = sumBlue[dominant] / maxCount;

                image[i][j].rgbtRed = (newRed / 32) * 32;
                image[i][j].rgbtGreen = (newGreen / 32) * 32;
                image[i][j].rgbtBlue = (newBlue / 32) * 32;
            }
        }
    }
    for (int i = 0; i < height; i++)
        free(copy[i]);
    free(copy);
}
