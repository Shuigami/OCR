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

    int t = 0;
    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j, ++t)
        {
            int val = (int) (sqrt((double)deltaX[t] * deltaX[t] 
                        + (double)deltaY[t] * deltaY[t]) + 0.5);

            pixels[t] = SDL_MapRGB(s->format, val, val, val);
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
                {
                    printf("t = %i\n", t);
                    nms[t] = pixels[t];
                }
            }
        }
    }
}

void canny_edge_detector(SDL_Surface *s)
{
    printf("Detecting edges...\n");
    int len = s->w * s->h;

    printf("    Bluring the image...\n");
    gaussian_blur(s, 10, 6.0);

    printf("    Generating gradients...\n");
    int *deltaX = malloc(sizeof(int) * len);
    int *deltaY = malloc(sizeof(int) * len);
    //get_gradients(s, deltaX, deltaY);
    //compute_magnitude(s, deltaX, deltaY);

    printf("    Applying non-maximal suppression algorithm...\n");
    // NMS(s, deltaX, deltaY);
}
