#include <err.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "processing.h"
#include "tools.h"
#include "helpers.h"

void surface_to_grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    if(SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_PixelFormat* format = surface->format;

    for (int i = 0; i < len; i++)
        pixels[i] = pixel_to_grayscale(pixels[i], format);

    SDL_UnlockSurface(surface);
}

void surface_to_simple_blackORwhite(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    if(SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_PixelFormat* format = surface->format;

    Uint8 black = 0 , white = 255;
    get_max_and_min(pixels,format,len,&white,&black);

    // Uint8 mid = (black - white)/2 + white;
    Uint8 mid = 110;
    // Uint8 mid = get_average(pixels,format,len);

    for (int i = 0; i < len; i++)
    {
      if (get_gray(pixels[i],format) <= mid)
        pixels[i] = SDL_MapRGB(format, 0, 0, 0);
      else if (get_gray(pixels[i],format) > mid)
        pixels[i] = SDL_MapRGB(format, 255, 255, 255);
    }

    SDL_UnlockSurface(surface);
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
        r = clamp(3 * (r - 128) + 128, 0, 255);
        g = clamp(3 * (g - 128) + 128, 0, 255);
        b = clamp(3 * (b - 128) + 128, 0, 255);

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
        r = clamp((int)(((double)r / 255.0) * (double)r), 0, 255);
        g = clamp((int)(((double)g / 255.0) * (double)g), 0, 255);
        b = clamp((int)(((double)b / 255.0) * (double)b), 0, 255);

        pixels[i] = SDL_MapRGB(format, r, g, b);
    }
}

void otsu(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    SDL_PixelFormat* format = surface->format;
    int w = surface->w;
    int h = surface->h;
    int total = w * h;
    if(SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    Uint8 color;

    int histogramCounts[256] = {0};
    for (int i = 0; i < total; i++)
    {
        SDL_GetRGB(pixels[i], format, &color, &color, &color);
        histogramCounts[color]++;
    }

    Uint8 threshold = 0;

    int sum1 = 0, sum2 = 0;
    int w1 = 0, w2 = 0;
    int m1 = 0, m2 = 0;
    double mid = 0, max = 0;

    for (int i = 0; i < 256; i++)
        sum1 += i * histogramCounts[i];

    for (int i = 1; i < 256; i++)
    {
        w1 += histogramCounts[i];
        if (w1 == 0)
            continue;

        w2 = total - w1;

        if (w2 == 0)
            break;

        sum2 += i * histogramCounts[i];
        m1 = sum2 / w1;
        m2 = (sum1 - sum2) / w2;
        mid = w1 * w2 * (m1 - m2) * (m1 - m2);

        if (mid >= max)
        {
            threshold = i;
            max = mid;
        }
    }

    Uint32 black = SDL_MapRGB(format, 255, 255, 255);
    Uint32 white = SDL_MapRGB(format, 0, 0, 0);

    for (int i = 0; i < total; i++)
    {
        SDL_GetRGB(pixels[i], format, &color, &color, &color);
        if (color >= threshold)
            pixels[i] = white;
        else
            pixels[i] = black;
    }
}
