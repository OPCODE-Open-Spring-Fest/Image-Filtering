// Define NOGDI BEFORE any Windows headers to prevent GDI types
#define NOGDI
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "bmp.h"
#include <setjmp.h>
#include <stdlib.h>
#include <jpeglib.h>
#include "image_io.h"

struct jpeg_error_mgr_wrapper {
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};

static void jpeg_error_exit(j_common_ptr cinfo) {
    struct jpeg_error_mgr_wrapper *myerr = (struct jpeg_error_mgr_wrapper *)cinfo->err;
    (*cinfo->err->output_message)(cinfo);
    longjmp(myerr->setjmp_buffer, 1);
}
// Read JPEG file
int read_jpeg(const char *filename, ImageData *img) {
    FILE *infile = fopen(filename, "rb");
    if (!infile) {
        return 1;
    }
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr_wrapper jerr;

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = jpeg_error_exit;
    if (setjmp(jerr.setjmp_buffer)) {
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        return 1;
    }
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    img->width = cinfo.output_width;
    img->height = cinfo.output_height;
    img->format = IMAGE_FORMAT_JPEG;

    // Allocate contiguous memory for pixels
    img->pixels = (RGBTRIPLE **)malloc(img->height * sizeof(RGBTRIPLE *));
    if (!img->pixels) {
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        return 1;
    }
    RGBTRIPLE *pixel_data = (RGBTRIPLE *)calloc(img->height * img->width, sizeof(RGBTRIPLE));
    if (!pixel_data) {
        free(img->pixels);
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        return 1;
    }
    for (int i = 0; i < img->height; i++) {
        img->pixels[i] = pixel_data + i * img->width;
    }

    // Read scanlines
    JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE,
                                                   cinfo.output_width * cinfo.output_components, 1);
    int row = 0;
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        JSAMPROW ptr = buffer[0];
        for (int col = 0; col < img->width; col++) {
            img->pixels[row][col].rgbtRed = ptr[col * 3];
            img->pixels[row][col].rgbtGreen = ptr[col * 3 + 1];
            img->pixels[row][col].rgbtBlue = ptr[col * 3 + 2];
        }
        row++;
    }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return 0;
}
// Write JPEG file
int write_jpeg(const char *filename, ImageData *img) {
    FILE *outfile = fopen(filename, "wb");
    if (!outfile) {
        return 1;
    }
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr_wrapper jerr;

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = jpeg_error_exit;
    if (setjmp(jerr.setjmp_buffer)) {
        jpeg_destroy_compress(&cinfo);
        fclose(outfile);
        return 1;
    }
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = img->width;
    cinfo.image_height = img->height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 90, TRUE);  
    jpeg_start_compress(&cinfo, TRUE);

    // Write scanlines
    JSAMPROW row_pointer[1];
    unsigned char *row_buffer = (unsigned char *)malloc(img->width * 3);
    if (!row_buffer) {
        jpeg_destroy_compress(&cinfo);
        fclose(outfile);
        return 1;
    }
    while (cinfo.next_scanline < cinfo.image_height) {
        int row = cinfo.next_scanline;
        for (int col = 0; col < img->width; col++) {
            row_buffer[col * 3] = img->pixels[row][col].rgbtRed;
            row_buffer[col * 3 + 1] = img->pixels[row][col].rgbtGreen;
            row_buffer[col * 3 + 2] = img->pixels[row][col].rgbtBlue;
        }
        row_pointer[0] = row_buffer;
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
    free(row_buffer);
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(outfile);
    return 0;
}

