#define NOGDI
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "image_io.h"
#include <stdlib.h>
#include <png.h>
// Read PNG file
int read_png(const char *filename, ImageData *img) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        return 1;
    }
    png_byte header[8];
    if (fread(header, 1, 8, fp) != 8 || png_sig_cmp(header, 0, 8)) {
        fclose(fp);
        return 1;
    }
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 1;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return 1;
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 1;
    }
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    img->width = png_get_image_width(png_ptr, info_ptr);
    img->height = png_get_image_height(png_ptr, info_ptr);
    img->format = IMAGE_FORMAT_PNG;

    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
    }
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    }
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_ptr);
    }
    if (bit_depth == 16) {
        png_set_strip_16(png_ptr);
    }
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png_ptr);
    }
    if (color_type == PNG_COLOR_TYPE_RGBA) {
        png_set_strip_alpha(png_ptr);  
    }

    png_read_update_info(png_ptr, info_ptr);

    img->pixels = (RGBTRIPLE **)malloc(img->height * sizeof(RGBTRIPLE *));
    if (!img->pixels) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 1;
    }
    RGBTRIPLE *pixel_data = (RGBTRIPLE *)calloc(img->height * img->width, sizeof(RGBTRIPLE));
    if (!pixel_data) {
        free(img->pixels);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 1;
    }
    for (int y = 0; y < img->height; y++) {
        img->pixels[y] = pixel_data + y * img->width;
    }
    png_bytep *row_pointers = (png_bytep *)malloc(img->height * sizeof(png_bytep));
    if (!row_pointers) {
        free(pixel_data);
        free(img->pixels);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 1;
    }
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    for (int y = 0; y < img->height; y++) {
        row_pointers[y] = (png_byte *)malloc(rowbytes);
        if (!row_pointers[y]) {
            for (int j = 0; j < y; j++) {
                free(row_pointers[j]);
            }
            free(row_pointers);
            free(pixel_data);
            free(img->pixels);
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
            return 1;
        }
    }

    // Read image
    png_read_image(png_ptr, row_pointers);
    for (int y = 0; y < img->height; y++) {
        png_bytep row = row_pointers[y];
        for (int x = 0; x < img->width; x++) {
            // PNG stores RGB, copy directly
            img->pixels[y][x].rgbtRed = row[x * 3];
            img->pixels[y][x].rgbtGreen = row[x * 3 + 1];
            img->pixels[y][x].rgbtBlue = row[x * 3 + 2];
        }
        free(row_pointers[y]);
    }

    free(row_pointers);
    png_read_end(png_ptr, NULL);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return 0;
}
// Write PNG file
int write_png(const char *filename, ImageData *img) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 1;
    }
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 1;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        return 1;
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 1;
    }
    png_init_io(png_ptr, fp);
    png_set_IHDR(png_ptr, info_ptr, img->width, img->height,
                 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);
    png_bytep row = (png_bytep)malloc(3 * img->width * sizeof(png_byte));
    if (!row) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 1;
    }
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            row[x * 3] = img->pixels[y][x].rgbtRed;
            row[x * 3 + 1] = img->pixels[y][x].rgbtGreen;
            row[x * 3 + 2] = img->pixels[y][x].rgbtBlue;
        }
        png_write_row(png_ptr, row);
    }
    free(row);
    png_write_end(png_ptr, NULL);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    return 0;
}

