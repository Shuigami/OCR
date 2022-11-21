#include <err.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "processing.h"
#include "tools.h"

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

/*
void otsu(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    if(SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_PixelFormat* format = surface->format;

    Uint8 mid = 0;
    double uT = 0;
    double uTT[256];
    double w[256] = {0};
    double u[256] = {0};

    double histo[256] = {0};
    for (int i = 0; i < len; i++)
        histo[get_gray(pixels[i],format)] += (1/len);

    w[0] = histo[0];
    for (int i = 1; i < 256; i++) {
        w[i] = w[i - 1] + histo[i];
        u[i] = u[i - 1] + i * histo[i];
    }

    for(int i = 0; i < 255; i++) {
        if(w[i] != 0.0 && w[i] != 1.0)
            uTT[i] = pow(u[255] * w[i], 2) / (w[i] * (1.0 - w[i]));
        else
            uTT[i] = 0.0;
        if(uTT[i] > uT) {
            uT = uTT[i];
            mid = i;
        }
    }

    printf("mid : %i\n", mid);

    for (int i = 0; i < len; i++)
    {
      if (get_gray(pixels[i],format) <= mid)
        pixels[i] = SDL_MapRGB(format, 0, 0, 0);
      else if (get_gray(pixels[i],format) > mid)
        pixels[i] = SDL_MapRGB(format, 255, 255, 255);
    }

    SDL_UnlockSurface(surface);
}*/

void otsu(SDL_Surface* surface)
{
    surface_to_grayscale(surface);
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

    int level = 0;

    int top = 256;
    int sumB = 0;
    int wB = 0;
    int maximum = 0;

    int colorArray[256];
    for (int i = 0; i < 256; i++)
        colorArray[i] = i;
    int sum1 = 0;
    for (int i = 0; i < 256; i++)
        sum1 += colorArray[i] * histogramCounts[i];

    for (int i = 1; i < top; i++)
    {
        int wF = total - wB;
        if (wB > 0 && wF > 0)
        {
            int mF = (sum1 - sumB) / wF;
            int val = wB * wF * ((sumB / wB) - mF) * ((sumB / wB) - mF);
            if (val >= maximum)
            {
                level = i;
                maximum = val;
            }
        }
        wB += histogramCounts[i];
        sumB += (i - 1) * histogramCounts[i];
    }

    printf("level = %i\n", level);

    Uint32 black = SDL_MapRGB(format, 255, 255, 255);
    Uint32 white = SDL_MapRGB(format, 0, 0, 0);

    for (int i = 0; i < total; i++)
    {
        SDL_GetRGB(pixels[i], format, &color, &color, &color);
        if (color > level)
            pixels[i] = black;
        else
            pixels[i] = white;
    }
}
