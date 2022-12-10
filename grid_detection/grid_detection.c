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

    for (int i = 0; i < len; i++)
    {
        if (line_eq[i][2] != 1. && abs((int)line_eq[i][0]) > 60)
        {
            line_eq[i][0] = - line_eq[i][1] / line_eq[i][0];
            line_eq[i][1] = -1.;
            line_eq[i][2] = 1.;
        }
    }

    printf("     Found line equations\n");

    /*
    for (int i = 0; i < len; i++)
    {
        if (line_eq[i][2] == 1)
            printf("    Lines[%i] : x = %.1f\n", i, line_eq[i][0]);
        else
            printf("    Lines[%i] : y = %.1f * x + %.1f\n", i, line_eq[i][0], line_eq[i][1]);
    }
    */

    printf("\n");

    return line_eq;
}

int **find_lines(int **accumulator, SDL_Surface* s, double *rhos, 
        double *thetas, int *len_lines)
{
    double w = s->w, h = s->h;
    // int w2 = w / 2, h2 = h / 2;
    double diag = sqrt(w * w + h * h);

    // Creating the rho values array
    double rho_num = (diag * 2);

    // Creating the theta array
    double theta_num = rho_num;
    
    printf(" Searching Lines...\n");

    // 1. Fiding the maximum
    double max = 0;
    for (int r = 0; r <= rho_num; r++)
    {
        for (int t = 0; t <= theta_num; t++)
        {
            int val = accumulator[r][t];

            if (val == 0)
                continue;

            if (val > max)
                max = val;
        }
    }

    printf("     Found max : %0f\n", max);

    // 2. Computing threshold
    int line_threshold = max * (30 / 100.0);

    printf("    ﬕ Threshold : %i\n", line_threshold);

    int **lines = malloc(sizeof(int*));
    // 3. Fiding coordinates of the edges in the accumulator using the
    // threshold

    int prev_t = 0, prev_r = 0;
    int step = rho_num / 60;

    int edges = 0;

    for (int r = 0; r <= rho_num; r += step)
    {
        for (int t = 0; t <= theta_num; t += step)
        {
            int val = accumulator[r][t];
            prev_r = r;
            prev_t = t;
;
            // Looking for the maximum in a 10*10 window
            for (int i = 0; i < step; i++)
            {
                for (int j = 0; j < step; j++)
                {
                    int x = r + i;
                    int y = t + j;

                    if (x > rho_num || y > theta_num)
                        continue;

                    if (accumulator[x][y] > val)
                    {
                        val = accumulator[x][y];
                        prev_r = x;
                        prev_t = y;
                    }
                }
            }

            if (val < line_threshold)
                continue;

            double rho = rhos[prev_r];
            double theta = thetas[prev_t];

            double ax = cos(theta);
            double ay = sin(theta);

            int x0 = rho * ax;
            int y0 = rho * ay;

            int x1 = x0 + diag * (-ay);
            int y1 = y0 + diag * (ax);

            int x2 = x0 - diag * (-ay);
            int y2 = y0 - diag * (ax);

            int coords[4] = {x1, y1, x2, y2};
            edges++;

            if (coords[0] != -1 || coords[1] != -1 || coords[2] != -1 || coords[3] != -1)
                lines = append_lines(lines, len_lines, coords[0], coords[1], coords[2], coords[3]);
        }
    }

    printf("     Found %i line(s) :\n", *len_lines);

    for (int i = 0; i < *len_lines; i++)
        printf("        (%i) - Lines from (%i,%i) to (%i, %i)\n", i, lines[i][0], 
                lines[i][1], lines[i][2], lines[i][3]);

    printf("     Found lines\n\n");

    return lines;
}

void lines_averaging(float **lines, int *len_lines)
{
    int *lines_deleted = malloc(sizeof(int) * *len_lines);
    int len_lines_deleted = 0;

    float threshold = .5;
    float delta = 40;
    for (int i = 0; i < *len_lines; i++)
    {
        // Check if already deleted
        int found = 0;
        for (int k = 0; k < len_lines_deleted; k++)
            if (lines_deleted[k] == i)
                found = 1;
        if (found)
            continue;

        float m1 = lines[i][0];
        float b1 = lines[i][1];
        float v1 = lines[i][2];

        for (int j = 0; j < *len_lines; j++)
        {
            if (j == i)
                continue;

            // Check if already deleted
            int found = 0;
            for (int k = 0; k < len_lines_deleted; k++)
                if (lines_deleted[k] == j)
                    found = 1;
            if (found)
                continue;
    
            float m2 = lines[j][0];
            float b2 = lines[j][1];
            float v2 = lines[j][2];

            if ((v1 == 1. && v2 == 1.))
            {
                if (m1 <= m2 + delta && m1 >= m2 - delta)
                {
                    int found = 0;
                    for (int k = 0; k < len_lines_deleted; k++)
                        if (lines_deleted[k] == j)
                            found = 1;
                    
                    if (!found)
                        lines_deleted[len_lines_deleted++] = j;
                }
            }

            else if (v1 == 1. && abs((int)m2) >= 50)
            {
                if (m1 <= b2 + delta && m1 >= b2 - delta)
                {
                    int found = 0;
                    for (int k = 0; k < len_lines_deleted; k++)
                        if (lines_deleted[k] == j)
                            found = 1;
                    
                    if (!found)
                        lines_deleted[len_lines_deleted++] = j;
                }
            }

            else if (v2 == 1. && abs((int)m1) >= 50)
            {
                if (m2 <= b1 + delta && m2 >= b1 - delta)
                {
                    int found = 0;
                    for (int k = 0; k < len_lines_deleted; k++)
                        if (lines_deleted[k] == j)
                            found = 1;
                    
                    if (!found)
                        lines_deleted[len_lines_deleted++] = j;
                }
            }

            else if (m1 <= m2 + threshold && m1 >= m2 - threshold && b1 <= b2 + delta && b1 >= b2 - delta)
            {
                int found = 0;
                for (int k = 0; k < len_lines_deleted; k++)
                    if (lines_deleted[k] == j)
                        found = 1;
                
                if (!found)
                    lines_deleted[len_lines_deleted++] = j;
            }
        }
    }

    printf("%i lines deleted\n", len_lines_deleted);
    
    int new_len = *len_lines - len_lines_deleted;
    int d = 0;
    for (int i = 0; i < *len_lines; i++)
    {
        int supp = 0;
        for (int j = 0; j < len_lines_deleted; j++)
        {
            if (lines_deleted[j] == i)
                supp = 1;
        }
        if (!supp){
            lines[i - d][0] = lines[i][0];
            lines[i - d][1] = lines[i][1];
            lines[i - d][2] = lines[i][2];
        }
        else
            d++;
    }

    for (int i = 0; i < new_len; i++)
    {
        if (lines[i][2] == 1)
            printf("    Lines[%i] : x = %.1f\n", i, lines[i][0]);
        else
            printf("    Lines[%i] : y = %.1f * x + %.1f\n", i, lines[i][0], lines[i][1]);
    }
    printf("\n");

    *len_lines = new_len;
}

void grid_detection(SDL_Surface* s, Uint32 *old, double *angle)
{
    int **hough_accumulator = hough_transform(s);

    Uint32 *pixels = s->pixels;

    // Get width and height of the image
    double w = s->w;
    double h = s->h;

    Uint8 r, g, b;
    for (int i = 0; i < w * h; i++){
        SDL_GetRGB(old[i], s->format, &r, &g, &b);
        pixels[i] = SDL_MapRGB(s->format, r, g, b);
    }

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
        if(*angle > 45)
            *angle = 90 - *angle;

        SDL_Surface *d = SDL_CreateRGBSurface(0, s->w, s->h, 32, 0, 255, 0, 0);
        rotate(s, d, *angle);
        *s = *d;

        lines = rotate_lines(s, -*angle, lines, len);
        for (int i = 0; i < len; i++)
            printf("        (%i) - Lines from (%i,%i) to (%i, %i)\n", i, lines[i][0], lines[i][1], lines[i][2], lines[i][3]);

    }

    float **lines_eq = find_line_equations(lines, len);
    lines_averaging(lines_eq, &len);

    // for (int i = 0; i < len; i++)
        // draw_line(s, lines_eq[i]);

    int *square = square_detection(s, lines_eq, len);

    resize(s, lines_eq, square);
    free(lines);
    free(lines_eq);
    // free(square);
}
