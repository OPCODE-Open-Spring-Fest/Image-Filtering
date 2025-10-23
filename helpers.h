#include "bmp.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]);

// invert image
void invert(int height, int width, RGBTRIPLE image[height][width]);

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width]);

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]);

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]);

// Brightness adjustment filter
void brightness(int height, int width, RGBTRIPLE image[height][width], int value);

// Vignette filter
void vignette(int height, int width, RGBTRIPLE image[height][width]);
