#ifndef IMAGE_IO_H
#define IMAGE_IO_H
#include "bmp.h"
#include <stdio.h>

// Image format types
typedef enum {IMAGE_FORMAT_BMP, IMAGE_FORMAT_PNG, IMAGE_FORMAT_JPEG, IMAGE_FORMAT_UNKNOWN} ImageFormat;
// Image data structure
typedef struct { int width; int height; RGBTRIPLE **pixels; ImageFormat format;} ImageData;
// detect image format from file
ImageFormat detect_image_format(const char *filename);

int read_image(const char *filename, ImageData *img);
int write_image(const char *filename, ImageData *img, ImageFormat output_format);
void free_image(ImageData *img);
ImageFormat get_format_from_extension(const char *filename);
#endif 

