#include "image_io.h"
#include "bmp.h"
#include <stdlib.h>
#include <string.h>

// Read BMP file
int read_bmp(const char *filename, ImageData *img) {
    FILE *inptr = fopen(filename, "rb");
    if (inptr == NULL) {
        return 1;
    }
    BITMAPFILEHEADER bf;
    if (fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr) != 1) {
        fclose(inptr);
        return 1;
    }
    BITMAPINFOHEADER bi;
    if (fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr) != 1) {
        fclose(inptr);
        return 1;
    }
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||  bi.biBitCount != 24 || bi.biCompression != 0) {
        fclose(inptr);
        return 1;
    }
    img->height = abs(bi.biHeight);
    img->width = bi.biWidth;
    img->format = IMAGE_FORMAT_BMP;

    img->pixels = (RGBTRIPLE **)malloc(img->height * sizeof(RGBTRIPLE *));
    if (img->pixels == NULL) {
        fclose(inptr);
        return 1;
    }
    RGBTRIPLE *pixel_data = (RGBTRIPLE *)calloc(img->height * img->width, sizeof(RGBTRIPLE));
    if (pixel_data == NULL) {
        free(img->pixels);
        fclose(inptr);
        return 1;
    }
    for (int i = 0; i < img->height; i++) {
        img->pixels[i] = pixel_data + i * img->width;
    }
    int padding = (4 - (img->width * sizeof(RGBTRIPLE)) % 4) % 4;

    // Read pixels
    for (int i = 0; i < img->height; i++) {
        if (fread(img->pixels[i], sizeof(RGBTRIPLE), img->width, inptr) != (size_t)img->width) {
            free_image(img);
            fclose(inptr);
            return 1;
        }
        fseek(inptr, padding, SEEK_CUR);
    }

    fclose(inptr);
    return 0;
}

// Write BMP file
int write_bmp(const char *filename, ImageData *img) {
    FILE *outptr = fopen(filename, "wb");
    if (outptr == NULL) {
        return 1;
    }
    int padding = (4 - (img->width * sizeof(RGBTRIPLE)) % 4) % 4;
    int row_size = img->width * sizeof(RGBTRIPLE) + padding;
    int image_size = row_size * img->height;

    BITMAPFILEHEADER bf;
    bf.bfType = 0x4d42; 
    bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + image_size;
    bf.bfReserved1 = 0;
    bf.bfReserved2 = 0;
    bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = img->width;
    bi.biHeight = img->height;
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = 0;
    bi.biSizeImage = image_size;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    if (fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr) != 1 || fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr) != 1) {
        fclose(outptr);
        return 1;
    }

    for (int i = 0; i < img->height; i++) {
        if (fwrite(img->pixels[i], sizeof(RGBTRIPLE), img->width, outptr) != (size_t)img->width) {
            fclose(outptr);
            return 1;
        }
        for (int k = 0; k < padding; k++) {
            fputc(0x00, outptr);
        }
    }
    fclose(outptr);
    return 0;
}

