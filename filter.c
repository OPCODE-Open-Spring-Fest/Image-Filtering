#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"
#include "image_io.h"

int main(int argc, char *argv[])
{
    // Define allowable filters
    char *filters = "bgrsivtmdGoPB:";

    // Allocate filter array
    char *filterArr = (char *)malloc((argc - 2) * sizeof(char));
    if (!filterArr) {
        printf("Memory allocation error.\n");
        return 1;
    }
    int filterCount = 0;
    int brightness_value = 0;
    
    // gets all filter flags and checks validity
    int opt;
    while ((opt = getopt(argc, argv, filters)) != -1) {
        if (opt == '?') {
            printf("Invalid filter option\n");
            free(filterArr);
            return 1;
        }
        if (opt == 'B') {
            if (optarg) {
                brightness_value = atoi(optarg);
            } else {
                printf("Brightness filter requires a value. Usage: -B <value>\n");
                free(filterArr);
                return 1;
            }
            filterArr[filterCount++] = opt;
        } else if (opt == 'm') {
            printf("\nAvailable filters:\n");
            printf("  -b   Blur\n");
            printf("  -g   Grayscale\n");
            printf("  -r   Reflect\n");
            printf("  -s   Sepia\n");
            printf("  -i   Invert\n");
            printf("  -v   Vignette\n");
            printf("  -t   Threshold\n");
            printf("  -d   Detect edges\n");
            printf("  -B <value>  Adjust brightness\n");
            printf("  -G   Glow\n");
            printf("  -o   Oil paint\n");
            printf("  -P   Pixelate (mosaic effect)\n");
            printf("  -m   Show this menu\n\n");
            free(filterArr);
            return 0;
        } else {
            filterArr[filterCount++] = opt;
        }
    }

    if (argc < optind + 2)
    {
        printf("Usage: ./filter [flag] infile outfile\n");
        printf("Filters: -g (grayscale), -s (sepia), -r (reflect), -b (blur), -i (invert), -v (vignette), -G (glow), -t (threshold), -d (edge detection), -o (oil paint), -P (pixelate), -B <value> (brightness)\n");
        free(filterArr);
        return 3;
    }

    // Remember filenames
    char *infile = argv[optind];
    char *outfile = argv[optind + 1];

    // Read image using new I/O system
    ImageData img;
    img.pixels = NULL;
    img.width = 0;
    img.height = 0;
    img.format = IMAGE_FORMAT_UNKNOWN;
    
    if (read_image(infile, &img) != 0) {
        printf("Could not read %s. Unsupported file format or file not found.\n", infile);
        printf("Supported formats: BMP, PNG, JPEG\n");
        return 4;
    }

    // Validate image was read correctly
    if (img.pixels == NULL || img.width <= 0 || img.height <= 0 || img.pixels[0] == NULL) {
        printf("Error: Invalid image data after reading.\n");
        free_image(&img);
        return 4;
    }

    int height = img.height;
    int width = img.width;
    
    // Cast pixels to match filter function

    RGBTRIPLE(*image)[width] = (RGBTRIPLE(*)[width])img.pixels[0];

    // Filter image
    for(int i=0; i<filterCount; i++){
    switch (filterArr[i])
    {
        // Blur
        case 'b':
            blur(height, width, image);
            break;

        // Grayscale
        case 'g':
            grayscale(height, width, image);
            break;

        // Reflection
        case 'r':
            reflect(height, width, image);
            break;

        // Sepia
        case 's':
            sepia(height, width, image);
            break;
        //invert
        case 'i':
            invert(height,width,image);
            break;

        // Vignette
        case 'v':
            vignette(height, width, image);
            break;

        case 't':
            threshold(height, width, image);
            break;
        case 'd':  // Edge Detection
            detect_edges(height, width, image);
            break;
            
        // Brightness Adjust
        case 'B':
            brightness(height, width, image, brightness_value);
            break;
        case 'G': 
            glow(height, width, image);
            break;
        case 'o':
            oilpaint(height, width, image);
            break;

        case 'P':  // Pixelate
            pixelate(height, width, image);
            break;
        default:
            printf("Unknown filter: %c\n", filterArr[i]);
            free_image(&img);
            free(filterArr);
            return 7;
        
    }
    }
    
    // Free filter array
    free(filterArr);
    
    // Write image using I/O system
    ImageFormat output_format = get_format_from_extension(outfile);
    if (output_format == IMAGE_FORMAT_UNKNOWN) {
        output_format = img.format;  
    }
    
    if (write_image(outfile, &img, output_format) != 0) {
        printf("Could not write %s.\n", outfile);
        free_image(&img);
        return 5;
    }

    // Free memory for image
    free_image(&img);
    return 0;
}
