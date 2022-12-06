#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "helpers.h"
#include "blur.h"

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
        {1, 2, 1},
        {0, 0, 0},
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

            deltaX[y * w + x] = x;
            deltaY[y * w + x] = y;

            int val = sqrt(x * x + y * y);
            pixels[j * w + i] = SDL_MapRGB(s->format, val, val, val);
        }
    }
}

void NMS(SDL_Surface *s, int *dX, int *dY)
{
    int t = 0;
    float alpha;
    float mag1, mag2;

    int w = s->w;
    int h = s->h;
    int len = w * h;

    Uint8 *nms = s->pixels;
    Uint8 pixels[len];
    for (int i = 0; i < len; i++)
        pixels[i] = nms[i];

    for (int i = 0; i < w; i++)
        nms[i] = 0;

    for (int i = (h - 1) * w; i < len; i++)
        nms[i] = 0;

    for (int i = w; i < len; i += w)
    {
        nms[i] = 0;
        nms[i + w - 1] = 0;
    }

    t = w + 1;
    for (int i = 1; i < h - 1; i++, t += 2)
    {
        for (int j = 1; j < w - 1; j++, t++)
        {
            if (pixels[t] == 0)
                nms[t] = 0;
            else
            {
                if (dX[t] >= 0)
                {
                    if (dY[t] >= 0)
                    {
                        if (dX[t] - dY[t] >= 0)
                        {
                            alpha = (float) dY[t] / dX[t];
                            mag1 = (1 - alpha) * pixels[t + 1] + alpha * pixels[t + w + 1];
                            mag2 = (1 - alpha) * pixels[t - 1] + alpha * pixels[t - w - 1];
                        }
                        else
                        {
                            alpha = (float) dX[t] / dY[t];
                            mag1 = (1 - alpha) * pixels[t + w] + alpha * pixels[t + w + 1];
                            mag2 = (1 - alpha) * pixels[t - w] + alpha * pixels[t - w - 1];
                        }
                    }
                    else
                    {
                        if (dX[t] + dY[t] >= 0)
                        {
                            alpha = (float) - dY[t] / dX[t];
                            mag1 = (1 - alpha) * pixels[t + 1] + alpha * pixels[t - w + 1];
                            mag2 = (1 - alpha) * pixels[t - 1] + alpha * pixels[t + w - 1];
                        }
                        else
                        {
                            alpha = (float) - dX[t] / dY[t];
                            mag1 = (1 - alpha) * pixels[t + w] + alpha * pixels[t + w + 1];
                            mag2 = (1 - alpha) * pixels[t - w] + alpha * pixels[t - w - 1];
                        }
                    }
                }
                else
                {
                    if (dY[t] >= 0)
                    {
                        if (dX[t] + dY[t] >= 0)
                        {
                            alpha = (float) - dX[t] / dY[t];
                            mag1 = (1 - alpha) * pixels[t + w] + alpha * pixels[t + w - 1];
                            mag2 = (1 - alpha) * pixels[t - w] + alpha * pixels[t - w + 1];
                        }
                        else
                        {
                            alpha = (float) - dY[t] / dX[t];
                            mag1 = (1 - alpha) * pixels[t - 1] + alpha * pixels[t + w - 1];
                            mag2 = (1 - alpha) * pixels[t + 1] + alpha * pixels[t - w + 1];
                        }
                    }
                    else
                    {
                        if (-dX[t] + dY[t] >= 0)
                        {
                            alpha = (float) dY[t] / dX[t];
                            mag1 = (1 - alpha) * pixels[t - 1] + alpha * pixels[t - w - 1];
                            mag2 = (1 - alpha) * pixels[t + 1] + alpha * pixels[t + w + 1];
                        }
                        else
                        {
                            alpha = (float) dX[t] / dY[t];
                            mag1 = (1 - alpha) * pixels[t - w] + alpha * pixels[t - w - 1];
                            mag2 = (1 - alpha) * pixels[t + w] + alpha * pixels[t + w + 1];
                        }
                    }
                }

                if (pixels[t] < mag1 || pixels[t] < mag2)
                    nms[t] = 0;
                else
                    nms[t] = pixels[t];
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
    int *deltaX = calloc(len * sizeof(int));
    int *deltaY = calloc(len * sizeof(int));
    sobel_filtering(s);

    printf("    Applying non-maximal suppression algorithm...\n");
    //NMS(s, deltaX, deltaY);
}
