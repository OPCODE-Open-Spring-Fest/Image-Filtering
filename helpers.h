#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <math.h>

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

//Threshold Filter(Black & White)
void threshold(int height, int width, RGBTRIPLE image[height][width]);

// *New: Edge Detection filter*
void detect_edges(int height, int width, RGBTRIPLE image[height][width]);

// Brightness adjustment filter
void brightness(int height, int width, RGBTRIPLE image[height][width], int value);

// Vignette filter
void vignette(int height, int width, RGBTRIPLE image[height][width]);

// Glow filter
void glow(int height, int width, RGBTRIPLE image[height][width]);
// Oil Paint filter
void oilpaint(int height, int width, RGBTRIPLE image[height][width]);
// Pixelate filter
void pixelate(int height, int width, RGBTRIPLE image[height][width]);
#endif
