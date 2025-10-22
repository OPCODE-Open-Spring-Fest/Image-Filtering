#include "helpers.h"
#include <stdint.h>
#include <stdlib.h>
#include "bmp.h"
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

// Blur image


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
