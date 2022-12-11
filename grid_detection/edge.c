#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "helpers.h"
#include "blur.h"
#define M_PI 3.141592653589793

void get_gradients(SDL_Surface *s, int *deltaX, int *deltaY)
{
    int w = s->w;
    int h = s->h;

    int t;

    Uint32 *pixels = s->pixels;

    for (int i = 0; i < h; i++)
    {
        t = i * w;
        deltaX[t] = pixels[t + 1] - pixels[t];

        for (int j = 1; j < w - 1; ++j)
        {
            t++;
            deltaX[t] = pixels[t + 1] - pixels[t - 1];
        }

        t++;
        deltaX[t] = pixels[t] - pixels[t - 1];
    }

    for (int j = 0; j < w; j++)
    {
        t = j;
        deltaY[t] = pixels[t + w] - pixels[t];

        for (int i = 1; i < h - 1; ++i)
        {
            t+=w;
            deltaY[t] = pixels[t + w] - pixels[t - w];
        }

        t+=w;
        deltaY[t] = pixels[t] - pixels[t - w];
    }
}

void compute_magnitude(SDL_Surface *s, int *deltaX, int *deltaY)
{
    Uint32 *pixels = s->pixels;

    int w = s->w;
    int h = s->h;

    for (int i = 0; i < w * h; i++)
    {
        int val = (int) (sqrt((double)deltaX[i] * deltaX[i] + (double)deltaY[i] * deltaY[i]) + 0.5);
        pixels[i] = SDL_MapRGB(s->format, val, val, val);
    }
}

void sobel_filtering(SDL_Surface *s, int *dX, int *dY)
{
    int w = s->w;
    int h = s->h;

    int Gx[3][3] = 
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gy[3][3] =
    {
        { 1,  2,  1},
        { 0,  0,  0},
        {-1, -2, -1}
    };

    Uint32 *old_pixels = copy_pixels(s);
    Uint32 *pixels = s->pixels;
    int x = 0;
    int y = 0;
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < h - 1; j++)
        {
            Uint8 c0;
            Uint8 c1;
            Uint8 c2;
            Uint8 c3;
            Uint8 c4;
            Uint8 c5;
            Uint8 c6;
            Uint8 c7;
            Uint8 c8;
            SDL_GetRGB(old_pixels[(j - 1) * w + i - 1], s->format, &c0, &c0, &c0);
            SDL_GetRGB(old_pixels[(j - 1) * w + i], s->format, &c1, &c1, &c1);
            SDL_GetRGB(old_pixels[(j - 1) * w + i + 1], s->format, &c2, &c2, &c2);
            SDL_GetRGB(old_pixels[j * w + i - 1], s->format, &c3, &c3, &c3);
            SDL_GetRGB(old_pixels[j * w + i], s->format, &c4, &c4, &c4);
            SDL_GetRGB(old_pixels[j * w + i + 1], s->format, &c5, &c5, &c5);
            SDL_GetRGB(old_pixels[(j + 1) * w + i - 1], s->format, &c6, &c6, &c6);
            SDL_GetRGB(old_pixels[(j + 1) * w + i], s->format, &c7, &c7, &c7);
            SDL_GetRGB(old_pixels[(j + 1) * w + i + 1], s->format, &c8, &c8, &c8);

            x = (Gx[0][0] * c0 + Gx[0][1] * c1 + Gx[0][2] * c2) +
                (Gx[1][0] * c3 + Gx[1][1] * c4 + Gx[1][2] * c5) +
                (Gx[2][0] * c6 + Gx[2][1] * c7 + Gx[2][2] * c8);

            y = (Gy[0][0] * c0 + Gy[0][1] * c1 + Gy[0][2] * c2) +
                (Gy[1][0] * c3 + Gy[1][1] * c4 + Gy[1][2] * c5) +
                (Gy[2][0] * c6 + Gy[2][1] * c7 + Gy[2][2] * c8);

            dX[j * w + i] = x;
            dY[j * w + i] = y;

            int val = sqrt(x * x + y * y);
            pixels[j * w + i] = SDL_MapRGB(s->format, val, val, val);
        }
    }
}

void NMS(SDL_Surface *s, int *angle)
{
    int w = s->w;
    int h = s->h;
    int len = w * h;

    Uint32 *pixels = copy_pixels(s);
    Uint32 *nms = s->pixels;

    for(int i = 0; i < len; i++)
    {
        angle[i] = angle[i] * 180 / M_PI;
        if (angle[i] < 0)
            angle[i] += 180;
    }

    for (int x = 1; x < w - 1; x++)
    {
        for (int y = 1; y < h - 1; y++)
        {
            Uint8 q = 255;
            Uint8 r = 255;

            int i = y * w + x;

            if ((angle[i] >= 0 && angle[i] < 22.5) || (angle[i] >= 157.5 && angle[i] <= 180))
            {
                SDL_GetRGB(pixels[(y+1) * w + x], s->format, &q, &q, &q);
                SDL_GetRGB(pixels[(y-1) * w + x], s->format, &r, &r, &r);
            }

            else if (angle[i] >= 22.5 && angle[i] < 67.5)
            {
                SDL_GetRGB(pixels[(y-1) * w + x + 1], s->format, &q, &q, &q);
                SDL_GetRGB(pixels[(y+1) * w + x - 1], s->format, &r, &r, &r);
            }

            else if (angle[i] >= 67.5 && angle[i] < 112.5)
            {
                SDL_GetRGB(pixels[y * w + x + 1], s->format, &q, &q, &q);
                SDL_GetRGB(pixels[y * w + x - 1], s->format, &r, &r, &r);
            }

            else if (angle[i] >= 112.5 && angle[i] < 157.5)
            {
                SDL_GetRGB(pixels[(y-1) * w + x - 1], s->format, &q, &q, &q);
                SDL_GetRGB(pixels[(y+1) * w + x + 1], s->format, &r, &r, &r);
            }

            Uint8 c;
            SDL_GetRGB(pixels[i], s->format, &c, &c, &c);
            if (c >= q && c >= r)
                nms[i] = pixels[i];
            else
                nms[i] = 0;
        }
    }
    free(pixels);
}

void threshold(SDL_Surface *s)
{
    int w = s->w;
    int h = s->h;
    int len = w * h;

    Uint32 *threshold = s->pixels;
    Uint32 *pixels = copy_pixels(s);
    SDL_PixelFormat * format = s->format;

    Uint8 max = 0;
    Uint8 c;
    for (int i = 0; i < len; i++)
    {
        SDL_GetRGB(pixels[i], format, &c, &c, &c);
        if (c > max)
            max = c;
    }

    int highThreshold = max * 0.09;
    int lowThreshold = highThreshold * 0.1;

    Uint8 weak = 25;
    Uint8 strong = 255;

    for (int i = 0; i < len; i++)
    {
        SDL_GetRGB(pixels[i], s->format, &c, &c, &c);
        if (c >= highThreshold)
            threshold[i] = SDL_MapRGB(format, strong, strong, strong);
        else if (c < lowThreshold)
            threshold[i] = SDL_MapRGB(format, 0, 0, 0);
        else
            threshold[i] = SDL_MapRGB(format, weak, weak, weak);
    }
}

void hysteresis(SDL_Surface *s)
{
    Uint8 weak = 25;
    Uint8 strong = 255;

    int w = s->w;
    int h = s->h;

    Uint32 *hysteresis = s->pixels;
    Uint32 *pixels = copy_pixels(s);
    SDL_PixelFormat * format = s->format;

    Uint8 c;
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < h - 1; j++)
        {
            SDL_GetRGB(pixels[j * w + i], format, &c, &c, &c);
            if (c == weak)
            {
                Uint8 c0;
                Uint8 c1;
                Uint8 c2;
                Uint8 c3;
                Uint8 c5;
                Uint8 c6;
                Uint8 c7;
                Uint8 c8;
                SDL_GetRGB(pixels[(j - 1) * w + i - 1], s->format, &c0, &c0, &c0);
                SDL_GetRGB(pixels[(j - 1) * w + i], s->format, &c1, &c1, &c1);
                SDL_GetRGB(pixels[(j - 1) * w + i + 1], s->format, &c2, &c2, &c2);
                SDL_GetRGB(pixels[j * w + i - 1], s->format, &c3, &c3, &c3);
                SDL_GetRGB(pixels[j * w + i + 1], s->format, &c5, &c5, &c5);
                SDL_GetRGB(pixels[(j + 1) * w + i - 1], s->format, &c6, &c6, &c6);
                SDL_GetRGB(pixels[(j + 1) * w + i], s->format, &c7, &c7, &c7);
                SDL_GetRGB(pixels[(j + 1) * w + i + 1], s->format, &c8, &c8, &c8);

                if (c0 == strong || c1 == strong || c2 == strong 
                        || c3 == strong || c5 == strong || c6 == strong
                        || c7 == strong || c8 == strong)
                    hysteresis[j * w + i] = SDL_MapRGB(s->format, strong, strong, strong);
                else
                    hysteresis[j * w + i] = SDL_MapRGB(s->format, 0, 0, 0);
            }
        }
    }
}

void canny_edge_detector(SDL_Surface *s)
{
    printf("Detecting edges...\n");
    int len = s->w * s->h;

    printf("    Bluring the image...\n");
    gaussian_blur(s, 5, 1.4);

    printf("    Generating gradients...\n");
    int *deltaX = calloc(len, sizeof(int));
    int *deltaY = calloc(len, sizeof(int));
    sobel_filtering(s, deltaX, deltaY);

    int *angle = calloc(len, sizeof(int));
    for (int i = 0; i < len; i++)
        angle[i] = (int)atan((double)deltaX[i] / (double)deltaY[i]);

    printf("    Applying non-maximal suppression algorithm...\n");
    NMS(s, angle);

    printf("    Double Threshold...\n");
    threshold(s);

    printf("    Hysteresis...\n");
    hysteresis(s);
}
