# ppm-image-basics

Basic image manipulation and I/O utilities in C using the raw PPM (P6) image format.  
I realized I had no knowledge of graphics so I wanted to start building that up through hands-on pixel-level operations. This serves as a warmup to my raycasting project.

## Features

- Read and write PPM (P6) binary image files
- Apply vertical rainbow gradient effect with simple color blending
- Memory-mapped file reading for fast pixel access
- No external libraries except `libgen` for `basename()`

## File Structure

ppm-image-basics/
├── main.c    # Applies rainbow effect
├── ppm.c     # Read/write PPM files
├── ppm.h    
├── README.md


## Usage

Convert any image to PPM (P6) format using ImageMagick:

```bash
magick input.jpg output.ppm
```

To run the rainbow effect:
gcc -o rainbow main.c ppm.c
./rainbow input.ppm

This will create a new image:
rainbow_input.ppm

## Sample Effect

Every row of the image is tinted by a hue based on vertical position, blended with a weight factor.