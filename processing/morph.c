#include <err.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "processing.h"
#include "tools.h"
#include "helpers.h"

Uint8 erode(SDL_Surface *s, Uint32 *old_pixels, int size, int x, int y)
{
    int w = s->w;
    int h = s->h;

    SDL_PixelFormat* format = s->format;

    Uint8 min = 0;
    Uint8 c = 0;
    int mid = size / 2;

    for (int i = -mid; i <= mid; i++)
    {
        int xx = clamp(x + i, 0, w - 1);
        for (int j = -mid; j <= mid; j++)
        {
            int yy = clamp(y + j, 0, h - 1);

            SDL_GetRGB(old_pixels[yy * w + xx], format, &c, &c, &c);

            if (c > min)
                min = c;
        }
    }

    return min;
}

Uint8 dilate(SDL_Surface *s, Uint32 *old_pixels, int size, int x, int y)
{
    int w = s->w;
    int h = s->h;

    SDL_PixelFormat* format = s->format;

    Uint8 max = 255;
    Uint8 c = 0;
    int mid = size / 2;

    for (int i = -mid; i <= mid; i++)
    {
        int xx = clamp(x + i, 0, w - 1);
        for (int j = -mid; j <= mid; j++)
        {
            int yy = clamp(y + j, 0, h - 1);

            SDL_GetRGB(old_pixels[yy * w + xx], format, &c, &c, &c);

            if (c < max)
                max = c;
        }
    }

    return max;
}


void morph(SDL_Surface *s)
{
    int w = s->w;
    int h = s->h;

    int size = 0;
    if (w > h)
        size = w / 300;
    else
        size = h / 300;

    Uint32 *old_pixels = copy_pixels(s);
    Uint32 *pixels = s->pixels;
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            Uint8 c = dilate(s, old_pixels, size, x, y);
            pixels[y * w + x] = SDL_MapRGB(s->format, c, c, c);
        }
    }

    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            Uint8 c = erode(s, old_pixels, size / 2.5, x, y);
            pixels[y * w + x] = SDL_MapRGB(s->format, c, c, c);
        }
    }

}
