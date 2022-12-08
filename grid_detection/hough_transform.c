#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <err.h>
#include "helpers.h"
#include "grid_detection.h"

int **hough_transform(SDL_Surface* s)
{
    printf(" Hough Transform...\n");

    // Get pixels list of surface
    Uint32* pixels = s->pixels;

    // Get width and height of the image
    double w = s->w;
    double h = s->h;

    // Max distance equals to the diagonal
    double diag = sqrt(w*w + h*h);
    printf("   壟 Diagonal : %f\n", diag);

    double rho_min = -diag;
    double rho_max = diag;
    double rho_num = diag * 2;
    double rho_step = (rho_max - rho_min) / rho_num;
    double *rhos = create_array(rho_num + 1, rho_min, rho_max, rho_step);

    double theta_min = -90;
    double theta_max = 90;
    double theta_num = diag * 2;
    double theta_step = (theta_max - theta_min) / theta_num;
    double *thetas = create_array(theta_num + 1, theta_min, 
            theta_max, theta_step);

    // Creating caching arrays for faster computation
    double *cos_t = malloc(sizeof(double) * (theta_num + 1));
    double *sin_t = malloc(sizeof(double) * (theta_num + 1));
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

    fprintf(stderr, "\33[2K\r   🖨️ Done: %i %%", 0);

    int done = 0;
    float total = w * h;
    Uint8 c;
    // Filling the accumulator
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            done++;
            // Process pixel only if its an edge
            SDL_GetRGB(pixels[y * (int) w + x], s->format, &c, &c, &c);
            if (c != 255)
                continue;

            for (int t = 0; t <= theta_num; t++)
            {
                double rho = x * cos_t[t] + y * sin_t[t];
                int rho_i = rho + diag;

                accumulator[rho_i][t]++;
            }
        }
    }

    free(rhos);
    free(thetas);
    free(cos_t);
    free(sin_t);

    return accumulator;
}
