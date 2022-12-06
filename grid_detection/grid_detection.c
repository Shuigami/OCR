#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <err.h>
#include "helpers.h"
#include "hough_transform.h"
#include "rotate.h"
#include "blackwhite.h"
#include "square_detection.h"
#include "resize.h"

float **find_line_equations(int **lines, int len)
{
    printf(" Finding line equations...\n");
    float **line_eq = malloc(sizeof(float*) * len);

    for (int i = 0; i < len; i++)
    {
        line_eq[i] = malloc(sizeof(float) * 3);
        int x1 = lines[i][0];
        int y1 = lines[i][1];
        int x2 = lines[i][2];
        int y2 = lines[i][3];

        // Vertical
        if (x1 <= x2 + 5 && x1 >= x2 - 5)
        {
            line_eq[i][0] = x1;
            line_eq[i][1] = -1;
            line_eq[i][2] = 1.;
        }
        // Horizontal
        else
        {
            float m = (float) (y1 - y2) / (x1 - x2);
            float b = - m * x1 + y1;
            line_eq[i][0] = m;
            line_eq[i][1] = b;
            line_eq[i][2] = -1.;
        }
    }

    printf("     Found line equations\n\n");

    return line_eq;
}

int **find_lines(int **accumulator, SDL_Surface* s, double *rhos, 
        double *thetas, int *len_lines)
{
    printf(" Searching Lines...\n");
    // Get width and height of the image
    double w = s->w;
    double h = s->h;

    // Max distance equals to the diagonal
    double diag = sqrt(w*w + h*h);
    double num = diag * 2;

    int **lines = NULL;

    printf("     Finding max...\n");
    int max = 0;
    for (int r = 0; r <= (int) num; r++)
    {
        for (int t = 0; t <= (int) num; t++)
        {
            if (accumulator[r][t] > max)
                max = accumulator[r][t];
        }
    }
    printf("     Found max : %i\n", max);

    int threshold = max * .4;
    printf("    ﬕ Threshold : %i\n", threshold);

    int lines_index = 0;
    int last_t = 0;
    int last_r = 0;
    int step = num / 60;
    for (int r = 0; r <= num; r += step)
    {
        for (int t = 0; t <= num; t += step)
        {
            int val = accumulator[r][t];
            last_r = r;
            last_t = t;

            for (int i = 0; i < step; i++)
            {
                for (int j = 0; j < step; j++)
                {
                    if (r + i > num || t + j > num)
                        continue;

                    if (accumulator[r + i][t + j] > val)
                    {
                        val = accumulator[r + i][t + j];
                        last_r = r + i;
                        last_t = t + j;
                    }
                }
            }

            if (val < threshold)
                continue;

            double rho = rhos[last_r];
            double theta = thetas[last_t];

            double ax = cos(theta);
            double ay = sin(theta);

            int x1 = rho * ay + diag * ax;
            int y1 = rho * ax + diag * (-ay);

            int x2 = rho * ay - diag * ax;
            int y2 = rho * ax - diag * (-ay);

            int *coords = inside_coords(s, x1, y1, x2, y2);
            lines_index++;

            if (coords[0] != -1 || coords[1] != -1 || coords[2] != -1 || coords[3] != -1)
                lines = append_lines(lines, len_lines, coords[0], coords[1],
                    coords[2], coords[3]);
        }
    }
    printf("     Found %i line(s) :\n", *len_lines);

    for (int i = 0; i < *len_lines; i++)
        printf("        (%i) - Lines from (%i,%i) to (%i, %i)\n", i, lines[i][0], 
                lines[i][1], lines[i][2], lines[i][3]);

    printf("     Found lines\n\n");

    return lines;
}

void grid_detection(SDL_Surface* s, double *angle)
{
    int **hough_accumulator = hough_transform(s);

    // Get width and height of the image
    double w = s->w;
    double h = s->h;

    // Max distance equals to the diagonal
    double diag = sqrt(w*w + h*h);

    double rho_min = -diag;
    double rho_max = diag;
    int rho_num = diag * 2;
    double rho_step = (rho_max - rho_min) / rho_num;
    double *rhos = create_array(rho_num + 1, rho_min, rho_max, rho_step);

    double theta_min = -90;
    double theta_max = 90;
    int theta_num = diag * 2;
    double theta_step = (theta_max - theta_min) / theta_num;
    double *thetas = create_array(theta_num + 1, theta_min, 
            theta_max, theta_step);

    for (int i = 0; i <= theta_num; i++)
        thetas[i] = degrees_to_rad(thetas[i]);

    int len = 0;
    int **lines = find_lines(hough_accumulator, s, rhos, thetas, &len);

    free(rhos);
    free(thetas);

    if (*angle == -1)
        *angle = automatic_rotation(hough_accumulator, s);

    if (*angle > .5)
    {
        SDL_Surface *d = SDL_CreateRGBSurface(0, s->w, s->h, 32, 0, 255, 0, 0);
        rotate(s, d, *angle);
        *s = *d;

        lines = rotate_lines(s, *angle, lines, len);
    }

    float **lines_eq = find_line_equations(lines, len);

    for (int i = 0; i < len; i++)
        draw_line(s, lines_eq[i]);

    int *square = square_detection(lines_eq, len);

    resize(s, lines_eq, square);

    free(lines);
    free(lines_eq);
    free(square);
}
