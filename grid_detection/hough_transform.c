#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include "helpers.h"

int **hough_transform(SDL_Surface* s)
{
    printf("        🔥 Hough Transform...\n");

    // Get pixels list of surface
    Uint32* pixels = s->pixels;

    // Get width and height of the image
    int w = s->w;
    int h = s->h;

    // Max distance equals to the diagonal
    int diag = sqrt(w*w + h*h);

    int rho_min = -diag;
    int rho_max = diag;
    int rho_num = diag * 2;
    int rho_step = (rho_max - rho_min) / rho_num;
    int *rhos = create_array(rho_num + 1, rho_min, rho_max, rho_step);

    int theta_min = -90;
    int theta_max = 90;
    int theta_num = diag * 2;
    int theta_step = (theta_max - theta_min) / theta_num;
    int *thetas = create_array(theta_num + 1, theta_min, theta_max, theta_step);

    int *cos_t = malloc(sizeof(int) * (theta_num + 1));
    int *sin_t = malloc(sizeof(int) * (theta_num + 1));
    for (int i = 0; i <= theta_num; i++)
    {
        thetas[i] = degrees_to_rad(thetas[i]);
        cos_t[i] = cos(thetas[i]);
        sin_t[i] = sin(thetas[i]);
    }

    int **accumulator = malloc(sizeof(int*) * (rho_num + 1) + 1);
    for (int r = 0; r <= rho_num; r++)
    {
        accumulator[r] = malloc(sizeof(int) * (theta_num + 1) + 1);
        for (int t = 0; t <= theta_num; t++)
            accumulator[r][t] = 0;
    }

    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            Uint8 r, g, b;
            SDL_GetRGB(pixels[x + y * w], s->format, &r, &g, &b);
            if(r != 255 || g != 255 || b != 255)
                continue;

            for (int t = 0; t <= theta_num; t++)
            {
                int rho = x * cos_t[t] + y * sin_t[t];
                int rho_i = rho + diag;

                accumulator[rho_i][t] += 1;
            }
        }
    }

    free(rhos);
    free(thetas);
    free(cos_t);
    free(sin_t);

    return accumulator;
}
