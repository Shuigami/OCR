#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "helpers.h"

double **get_gaussian_kernel(int size, double sigma)
{
    double **kernel = malloc(sizeof(double*) * size);
    for (int i = 0; i < size; i++)
        kernel[i] = malloc(sizeof(double) * size);

    double s = 2.0 * sigma * sigma;

    double sum = 0.0;

    int mid = size / 2;

    for (int x = -mid; x < mid; x++)
    {
        for (int y = -mid; y < mid; y++)
        {
            kernel[x + mid][y + mid] = (exp(-(x * x + y * y) / s)) / (M_PI * s);
            sum += kernel[x + mid][y + mid];
        }
    }

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            kernel[i][j] /= sum;

    return kernel;
}

void gaussian_pixel(SDL_Surface *s, int x, int y, int size, double **kernel)
{
    Uint8 rF = 0;
    Uint8 gF = 0;
    Uint8 bF = 0;

    Uint8 r = 0;
    Uint8 g = 0;
    Uint8 b = 0;

    int mid = size / 2;

    Uint32 *pixels = s->pixels;
    SDL_PixelFormat *format = s->format;

    int width = s->w;
    int height = s->h;

    for (int i = -mid; i < mid; i++)
    {
        int xx = clamp(x + i, 0, width - 1);
        for (int j = -mid; j < mid; j++)
        {
            int yy = clamp(y + j, 0, height - 1);

            double factor = kernel[i + mid][j + mid];

            SDL_GetRGB(pixels[yy * width + xx], format, &r, &g, &b);

            r *= factor;
            g *= factor;
            b *= factor;

            rF += r;
            gF += g;
            bF += b;
        }
    }

    pixels[y * width + x] = SDL_MapRGB(format, rF, gF, bF);
}

void gaussian_blur(SDL_Surface *s, int size, double sigma)
{
    double **kernel = get_gaussian_kernel(size, sigma);

    for (int x = 0; x < s->w; x++)
        for (int y = 0; y < s->h; y++)
            gaussian_pixel(s, x, y, size, kernel);

    for (int i = 0; i < size; i++)
        free(kernel[i]);
    free(kernel);
}
