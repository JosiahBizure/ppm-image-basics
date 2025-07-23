#ifndef PPM_H
#define PPM_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Represents a single RGB pixel using standard 8-bit color depth (0–255)
typedef struct {
    uint8_t r, g, b;
} pixel;

/*
    Represents a PPM image stored in memory.

    pixels is a 2D array allocated as pixel[height][width],
    stored in row-major order: pixels[row][col]

    width:  number of columns (horizontal resolution)
    height: number of rows (vertical resolution)
    maxval: maximum color value (I'll use 255)
*/
typedef struct {
    size_t width;
    size_t height;
    uint8_t maxval;
    pixel** pixels;
} ppmimage;

/*
    Reads a PPM (only supports P6) image from the given file.
    Returns a pointer to a newly allocated ppmimage struct on success, or NULL on failure.
*/
ppmimage* ppm_read_image(const char* filename);

// Frees the memory allocated for a ppmimage and its internal pixel data.
void ppm_destroy(ppmimage* image);

/*
    Writes the given ppmimage to a file in P6 format (binary).
    Returns true if write was successful, false otherwise.
*/
bool ppm_write_image(const char* filename, const ppmimage* image);

#endif // PPM_H