#include <stdio.h>
#include <unistd.h>     // for close(), lseek(), and fileno
#include <fcntl.h>      // sometimes useful for open() flags, though not needed with FILE*
#include <stdlib.h>  // malloc, calloc, free
#include <string.h>  // strcmp
#include <sys/mman.h> // mmap, munmap, PROT_READ, MAP_PRIVATE


#include "ppm.h"

ppmimage* ppm_read_image(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error: could not open file");
        return NULL;
    }

    // get the file size
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);

    // get file descriptor
    int fd = fileno(file);

    // move the read pointer back to the beginning of the file
    rewind(file);

    ppmimage* result = (ppmimage*)malloc(sizeof(ppmimage));

    // verify that we have just opened a P6 file
    char type[3] = {0};
    fscanf(file, "%2s", type);
    if (strcmp(type, "P6") != 0) {
        perror("This is not a P6 PPM file");
        fclose(file);
        free(result);
        return NULL;
    }

    // get image size and color depth information
    fscanf(file, "%zu %zu", &(result->width), &(result->height));
    fscanf(file, "%hhu", &(result->maxval));

    off_t offset = ftell(file); // The beginning of the actual pixel data (ignoring header)

    uint8_t* rawdata = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);

    if (rawdata == MAP_FAILED) {
        perror("mmap error");
        fclose(file);
        free(result);
        return NULL;
    }

    // how many bytes per color value
    uint8_t bytes_per_value = 1;
    const int values_per_pixel = 3; // r, g, b

    uint8_t* pixeldata = rawdata + offset + 1; // + 1 for the \n

    result->pixels = (pixel**)calloc(result->height, sizeof(pixel*));
    for (int y = 0; y < result->height; ++y) {
        result->pixels[y] = calloc(result->width, sizeof(pixel));
        for (int x = 0; x < result->width; ++x) {
            uint8_t* pixel_start = pixeldata + (y * result->width * values_per_pixel * bytes_per_value)
                                    + (x * values_per_pixel * bytes_per_value);

            result->pixels[y][x].r = pixel_start[0];
            result->pixels[y][x].g = pixel_start[1];
            result->pixels[y][x].b = pixel_start[2];
        }
    }

    fclose(file);
    munmap(rawdata, file_size);

    return result;
}


void ppm_destroy(ppmimage* image) {
    for (int y = 0; y < image->height; ++y) {
        free(image->pixels[y]);
    }
    free(image->pixels);
    free(image);
}


bool ppm_write_image(const char* filename, const ppmimage* image) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error: could not open file");
        return false;
    }

    fprintf(file, "P6\n%zu %zu\n%hhu\n", image->width, image->height, image->maxval);

    uint8_t bytes_per_value = 1;
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            fwrite(&(image->pixels[y][x]), sizeof(uint8_t), 3, file);
        }
    }

    return true;
 }