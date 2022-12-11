#include <err.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "blackwhite.h"
#include "grid_detection.h"
#include "helpers.h"
#include "rotate.h"
#include "tools.h"
#include "filter.h"
#include "morph.h"
#include "edge.h"

int processing_image(SDL_Surface *s, double angle)
{
    int w = s->w;
    int h = s->h;

    surface_to_grayscale(s);

    double average = 0;
    Uint32 *pixels = s->pixels;
    Uint8 c;
    for (int i = 0; i < w * h; i++)
    {
        SDL_GetRGB(pixels[i], s->format, &c, &c, &c);
        average += (double)c / (double)(w * h);
    }

    // Uint32 *old = copy_pixels(s);

    if(SDL_SaveBMP(s, "result/1.0-black_and_white.bmp") != 0)
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());

    if (average > 175)
        filter_normalize(s);

    if(SDL_SaveBMP(s, "result/1.1-normalize.bmp") != 0)
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    
    filter_gamma(s);

    if(SDL_SaveBMP(s, "result/1.2-gamma.bmp") != 0)
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());

    filter_contrast(s);

    if(SDL_SaveBMP(s, "result/1.3-contrast.bmp") != 0)
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
        
    morph(s);   
    otsu(s);

    Uint32 *bow = copy_pixels(s);

    if(SDL_SaveBMP(s, "result/1.6-otsu.bmp") != 0)
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
        
    canny_edge_detector(s);

    if(SDL_SaveBMP(s, "result/1.7-canny.bmp") != 0)
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
        
    // - Grid Detection
    grid_detection(s, bow, &angle);

    return EXIT_SUCCESS;
}
