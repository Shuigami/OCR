#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <err.h>
#include "helpers.h"

int is_perpendicular(float *line1, float *line2)
{
    if (line1[2] == 1. && line2[2] == 1.)
        return 0;

    float threshold = .4;
    // Line 1 is vertical
    if (line1[2] == 1.)
    {
        // Line 2 is horizontal
        if (line2[2] == -1. && line2[0] <= threshold && line2[0] >= -threshold)
            return 1;
        // Line 2 is not horizontal
        else
            return 0;
    }

    // Line 2 is vertical
    if (line2[2] == 1.)
    {
        // Line 1 is horizontal
        if (line1[2] == -1. && line1[0] <= threshold && line1[0] >= -threshold)
            return 1;
        // Line 1 is not horizontal
        else
            return 0;
    }

    if (line1[0] * line2[0] <= -1 + threshold 
            && line1[0] * line2[0] >= -1 - threshold)
        return 1;

    return 0;
}

float *intersection_point(float **lines, int i, int j)
{
    if (lines[i][2] == 1 && lines[j][2] == 1)
        errx(1, "No intersection point with these lines");

    float x = 0;
    float y = 0;
    if (lines[i][2] == 1.)
    {
        x = lines[i][0];
        y = lines[j][0] * x + lines[j][1];
    }
    else if (lines[j][2] == 1.)
    {
        x = lines[j][0];
        y = lines[i][0] * x + lines[i][1];
    }
    else
    {
        x = (- lines[j][1] + lines[i][1]) / (- lines[i][0] + lines[j][0]);
        y = (lines[i][1] * lines[j][0] - lines[j][1] * lines[i][0])
            / (- lines[i][0] + lines[j][0]);
    }

    float *point = malloc(sizeof(float) * 2);
    point[0] = x;
    point[1] = y;
    return point;
}

float square_perimeter(float **lines, int i, int j, int k, int l)
{
    float *p1 = intersection_point(lines, i, j);
    float *p2 = intersection_point(lines, j, k);
    float *p3 = intersection_point(lines, k, l);
    float *p4 = intersection_point(lines, l, i);

    float d = 0;
    d += sqrt((p1[0] - p2[0]) * (p1[0] - p2[0]) 
            + (p1[1] - p2[1]) * (p1[1] - p2[1]));
    d += sqrt((p2[0] - p3[0]) * (p2[0] - p3[0]) 
            + (p2[1] - p3[1]) * (p2[1] - p3[1]));
    d += sqrt((p3[0] - p4[0]) * (p3[0] - p4[0]) 
            + (p3[1] - p4[1]) * (p3[1] - p4[1]));
    d += sqrt((p4[0] - p1[0]) * (p4[0] - p1[0]) 
            + (p4[1] - p1[1]) * (p4[1] - p1[1]));

    free(p1);
    free(p2);
    free(p3);
    free(p4);
    return d;
}

int *square_detection(SDL_Surface *s, float **lines, int len)
{
    printf(" Square detection...\n");

    // Perimeter of the largest square
    float max_length = -1;

    // Index of the lines in **lines making the square
    int *square_lines = malloc(sizeof(int) * 4);
    memset(square_lines, -1, 4);

    if (len < 4)
    {
        errx(1, "    No square found");
        return square_lines;
    }

    float **points = NULL;

    for (int i = 0; i < len; i++)
    {
        float *line1 = lines[i];
        for (int j = 0; j < len; j++)
        {
            float *line2 = lines[j];
            if (i == j || !is_perpendicular(line1, line2))
                continue;
            
            for (int k = 0; k < len; k++)
            {
                float *line3 = lines[k];
                if (i == k || j == k || !is_perpendicular(line2, line3))
                    continue;

                for (int l = 0; l < len; l++)
                {
                    float *line4 = lines[l];
                    if (i == l || j == l || k == l 
                            || !is_perpendicular(line3, line4)
                            || !is_perpendicular(line4, line1))
                        continue;


                    float perimeter = square_perimeter(lines, i, j, k, l);

                    if (perimeter > max_length)
                    {
                        float **tmp_points = get_points(lines, &i, &j, &k, &l);

                        float h = dist(tmp_points[0], tmp_points[1]);
                        float w = dist(tmp_points[0], tmp_points[2]);

                        if (abs((int)w - (int)h) > 30)
                            continue;

                        int min = 120;
                        if ((tmp_points[0][0] >= min && tmp_points[2][0] > s->w - min)
                            || (tmp_points[0][0] < min && tmp_points[2][0] <= s->w - min))
                            continue;

                        min = 40;
                        if ((tmp_points[0][1] >= min && tmp_points[1][1] > s->h - min)
                            || (tmp_points[0][1] < min && tmp_points[1][1] <= s->h - min))
                            continue;

                        points = tmp_points;

                        // draw_line(s, lines[i]);
                        // draw_line(s, lines[j]);
                        // draw_line(s, lines[k]);
                        // draw_line(s, lines[l]);
                        
                        max_length = perimeter;
                        square_lines[0] = i;
                        square_lines[1] = j;
                        square_lines[2] = k;
                        square_lines[3] = l;
                    }
                }
            }
        }
    }

    if (points == NULL)
        errx(1, "    No square found");

    int i = square_lines[0];
    int j = square_lines[1];
    int k = square_lines[2];
    int l = square_lines[3];
    
    float *p1 = points[0];
    float *p2 = points[1];
    float *p3 = points[2];
    float *p4 = points[3];

    // draw_line(s, lines[i]);
    // draw_line(s, lines[j]);
    // draw_line(s, lines[k]);
    // draw_line(s, lines[l]);

    printf("     Largest square :\n");
    printf("        - p1 = (%.0f, %.0f)\n", p1[0], p1[1]);
    printf("        - p2 = (%.0f, %.0f)\n", p2[0], p2[1]);
    printf("        - p3 = (%.0f, %.0f)\n", p3[0], p3[1]);
    printf("        - p4 = (%.0f, %.0f)\n", p4[0], p4[1]);

    printf("     Perimeter of largest square : %.2f\n\n", 
            square_perimeter(lines, i, j, k, l));

    return square_lines;
}
