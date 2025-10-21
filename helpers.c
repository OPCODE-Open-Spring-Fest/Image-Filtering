#include "helpers.h"

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

// Convert image to sepia

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

// Blur image

}