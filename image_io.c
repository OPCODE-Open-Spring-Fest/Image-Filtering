#include "image_io.h"
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>

// Detect image format by reading file signature
ImageFormat detect_image_format(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        return IMAGE_FORMAT_UNKNOWN;
    }
    uint8_t header[8];
    size_t bytes_read = fread(header, 1, 8, file);
    fclose(file);

    if (bytes_read < 2)  return IMAGE_FORMAT_UNKNOWN;

    //BMP: "BM"(0x42 0x4D)
    if (header[0] == 0x42 && header[1] == 0x4D) {
        return IMAGE_FORMAT_BMP;
    }
    //PNG: \x89PNG\r\n\x1a\n
    if (bytes_read >= 8 &&   header[0] == 0x89 && header[1] == 0x50 &&   header[2] == 0x4E && header[3] == 0x47 && header[4] == 0x0D && header[5] == 0x0A &&  header[6] == 0x1A && header[7] == 0x0A) {
        return IMAGE_FORMAT_PNG;
    }
    //JPEG: \xFF\xD8\xFF
    if (bytes_read >= 3 && header[0] == 0xFF && header[1] == 0xD8 && header[2] == 0xFF) {
        return IMAGE_FORMAT_JPEG;
    }
    return IMAGE_FORMAT_UNKNOWN;
}
// Get format from file extension
ImageFormat get_format_from_extension(const char *filename) {
    const char *ext = strrchr(filename, '.');
    if (!ext) {
        return IMAGE_FORMAT_UNKNOWN;
    }
    ext++; 
    char lower_ext[10];
    int i = 0;
    while (ext[i] && i < 9) {
        lower_ext[i] = tolower(ext[i]);
        i++;
    }
    lower_ext[i] = '\0';
    if (strcmp(lower_ext, "bmp") == 0) {
        return IMAGE_FORMAT_BMP;
    } else if (strcmp(lower_ext, "png") == 0) {
        return IMAGE_FORMAT_PNG;
    } else if (strcmp(lower_ext, "jpg") == 0 || strcmp(lower_ext, "jpeg") == 0) {
        return IMAGE_FORMAT_JPEG;
    }
    return IMAGE_FORMAT_UNKNOWN;
}
extern int read_bmp(const char *filename, ImageData *img);
extern int read_png(const char *filename, ImageData *img);
extern int read_jpeg(const char *filename, ImageData *img);
extern int write_bmp(const char *filename, ImageData *img);
extern int write_png(const char *filename, ImageData *img);
extern int write_jpeg(const char *filename, ImageData *img);
// Read image from file
int read_image(const char *filename, ImageData *img) {
    if (!filename || !img) {
        return 1;
    }
    ImageFormat format = detect_image_format(filename);
    if (format == IMAGE_FORMAT_UNKNOWN) {
        format = get_format_from_extension(filename);
    }
    img->format = format;
    switch (format) {
        case IMAGE_FORMAT_BMP:
            return read_bmp(filename, img);
        case IMAGE_FORMAT_PNG:
            return read_png(filename, img);
        case IMAGE_FORMAT_JPEG:
            return read_jpeg(filename, img);
        default:
            return 1; 
    }
}
// Write image to file
int write_image(const char *filename, ImageData *img, ImageFormat output_format) {
    if (!filename || !img || !img->pixels) {
        return 1;
    }
    if (output_format == IMAGE_FORMAT_UNKNOWN) {
        output_format = get_format_from_extension(filename);
    }
    if (output_format == IMAGE_FORMAT_UNKNOWN) {
        output_format = img->format;
    }
    switch (output_format) {
        case IMAGE_FORMAT_BMP:
            return write_bmp(filename, img);
        case IMAGE_FORMAT_PNG:
            return write_png(filename, img);
        case IMAGE_FORMAT_JPEG:
            return write_jpeg(filename, img);
        default:
            return 1;
    }
}
// Free image data
void free_image(ImageData *img) {
    if (img && img->pixels) {
        if (img->height > 0 && img->pixels[0]) {
            free(img->pixels[0]);  
        }
        free(img->pixels); 
        img->pixels = NULL;
        img->width = 0;
        img->height = 0;
    }
}

