#include <err.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "processing.h"
#include "tools.h"
#include "helpers.h"

void filter_normalize(SDL_Surface *s)
{
    int w = s->w;
    int h = s->h;
    int len = w * h;

    Uint8 max = 0;
    Uint8 min = 255;

    Uint32 *pixels = s->pixels;
    SDL_PixelFormat* format = s->format;

    Uint8 c;

    for (int i = 0; i < len; i++)
    {
        SDL_GetRGB(pixels[i], format, &c, &c, &c);

        if (c > max)
            max = c;
        if (c < min)
            min = c;
    }

    for (int i = 0; i < len; i++)
    {
        SDL_GetRGB(pixels[i], format, &c, &c, &c);

        Uint8 r = clamp(((double)(c - min - 30) / (double)(max - min)) * 255, 0, 255);
        pixels[i] = SDL_MapRGB(format, r, r, r);
    }
}

void filter_contrast(SDL_Surface *s)
{
    int w = s->w;
    int h = s->h;
    int len = w * h;

    SDL_PixelFormat* format = s->format;

    Uint8 r;
    Uint8 g;
    Uint8 b;

    Uint32 *pixels = s->pixels;

    for (int i = 0; i < len; i++)
    {
        SDL_GetRGB(pixels[i], format, &r, &g, &b);
        int c = 3 * (r - 128) + 128;
        r = clamp(c, 0, 255);
        g = clamp(c, 0, 255);
        b = clamp(c, 0, 255);

        pixels[i] = SDL_MapRGB(format, r, g, b);
    }
}

void filter_gamma(SDL_Surface *s)
{
    int w = s->w;
    int h = s->h;
    int len = w * h;

    SDL_PixelFormat* format = s->format;

    Uint8 r;
    Uint8 g;
    Uint8 b;

    Uint32 *pixels = s->pixels;

    for (int i = 0; i < len; i++)
    {
        SDL_GetRGB(pixels[i], format, &r, &g, &b);

        double c = pow((double)r / 255.0, .9);

        r = clamp(c * 255, 0, 255);
        g = clamp(c * 255, 0, 255);
        b = clamp(c * 255, 0, 255);

        pixels[i] = SDL_MapRGB(format, r, g, b);
    }
}
