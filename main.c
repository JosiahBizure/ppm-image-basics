#include <stdio.h>
#include <libgen.h>
#include <math.h>
#include <stdlib.h>

#include "ppm.h"

#define COLOR_WHEEL_SEGMENTS 6
pixel hue_to_rgb(double hue) {
    pixel color;
    double r, g, b;

    // hue is in the 0 - 1 range
    double value = 1.0; // Full value/brightness

    int segment = floor(hue * COLOR_WHEEL_SEGMENTS);

    // how far into the segment is the hue
    double offset = (hue * COLOR_WHEEL_SEGMENTS) - segment;

    switch (segment % COLOR_WHEEL_SEGMENTS) {
        case 0: r = value; g = offset; b = 0; break;
        case 1: r = (1 - offset); g = value; b = 0; break;
        case 2: r = 0; g = value; b = offset; break;
        case 3: r = 0; g = (1 - offset); b = value; break;
        case 4: r = offset; g = 0; b = value; break;
        case 5: r = value; g = 0; b = (1 - offset); break;
    }

    // convert (0-1) RGB values to 0 - 255 integer range
    color.r = (int)(r * 255.0);
    color.g = (int)(g * 255.0);
    color.b = (int)(b * 255.0);

    return color;
}


int main(int argc, char** argv) {
    if (argc != 2) {
        printf("usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char* filename = argv[1];
    char* output_filename = NULL;
    asprintf(&output_filename, "rainbow_%s", basename(filename));

    printf("input:\t%s\noutput:\t%s\n\n", filename, output_filename);

    ppmimage* img = ppm_read_image(filename);
    if (!img) {
        free(output_filename);
        return 1;
    }

    const double weight = 0.3;

    for (int y = 0; y < img->height; y++) {
        double hue = ((double)y / (double)img->height);
        pixel color = hue_to_rgb(hue);

        for (int x = 0; x < img->width; x++) {
            pixel* p = &img->pixels[y][x];

            p->r = (uint8_t)((p->r * (1.0 - weight)) + (color.r * weight));
            p->g = (uint8_t)((p->g * (1.0 - weight)) + (color.g * weight));
            p->b = (uint8_t)((p->b * (1.0 - weight)) + (color.b * weight));
        }
    }

    ppm_write_image(output_filename, img);
    ppm_destroy(img);
    free(output_filename);

    return 0;
}