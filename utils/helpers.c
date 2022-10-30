#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

double *create_array(int size, double min, double max, double step)
{
    double *array = malloc(sizeof(double) * size + 1);
    if (array == NULL)
        errx(1, "Not enough memory!");

    double cur = min;
    for (int i = 0; i < size && cur <= max + 1; i++)
    {
        array[i] = cur;
        cur += step;
    }

    return array;
}

double degrees_to_rad(double degrees)
{
    double rad = degrees * (3.14149265359 / 180);
    return rad;
}

void print_2d_array(int **array, int size_x, int size_y)
{
    for (int x = 0; x < size_x; x++)
    {
        for (int y = 0; y < size_y; y++)
            printf("%4i ", array[x][y]);
        printf("\n");
    }
}

int *draw_line(SDL_Surface *s, int x0, int y0, int x1, int y1)
{
    int w = s->w;
    int h = s->h;

    int *coordinates = malloc(sizeof(int) * 4 + 1);
    memset(coordinates, -1, sizeof(int) * 4);

    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;

    int err = dx + dy;

    while (true)
    {
        if (0 <= x0 && x0 < w && 0 <= y0 && y0 < h)
        {
            if (coordinates[0] == -1 && coordinates[1] == -1)
            {
                coordinates[0] = x0;
                coordinates[1] = y0;
            }
            else
            {
                coordinates[2] = x0;
                coordinates[3] = y0;
            }
        }

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;

        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }

    return coordinates;
}

double str_to_double(char *str)
{
    double r = 0;
    size_t n = strlen(str);

    for (size_t i = 0; i < n; i++)
        r = r * 10 + (double) (str[i] - '0');

    return r;
}

int **append_lines(int **L, int *len, int x1, int y1, int x2, int y2)
{
    int found = 0;
    int **M = malloc(sizeof(int*) * (*len + 1));
    for (int i = 0; i < *len; i++)
    {
        if ((L[i][0] == x1 && L[i][1] == y1 && L[i][2] == x2 && L[i][3] == y2) 
                || (L[i][0] == x2 && L[i][1] == y2 
                    && L[i][2] == x1 && L[i][3] == y1))
            found = 1;
        M[i] = malloc(sizeof(int) * 4);
        M[i][0] = L[i][0];
        M[i][1] = L[i][1];
        M[i][2] = L[i][2];
        M[i][3] = L[i][3];
    }

    if (found == 1)
        return M;

    *len += 1;
    M[*len - 1] = malloc(sizeof(int) * 4);
    M[*len - 1][0] = x1;
    M[*len - 1][1] = y1;
    M[*len - 1][2] = x2;
    M[*len - 1][3] = y2;

    return M;
}

int comp(int sign, int x, int y, int threshold)
{
    if (sign == 0)
    {
        if (x <= y + threshold)
            return 1;
        else
            return 0;
    }
    else
    {
        printf("    %i >= %i\n\n", x, y - y * threshold);
        if (x >= y - threshold)
            return 1;
        else
            return 0;
    }
}

int is_min(int i, int j, int k, int l)
{
    if (i <= j && i <= k && i <= l)
        return 1;
    return 0;
}

int is_max(int i, int j, int k, int l)
{
    if (i >= j && i >= k && i >= l)
        return 1;
    return 0;
}

int get_tl(float **points)
{
    float *p1 = points[0];
    float *p2 = points[1];
    float *p3 = points[2];
    float *p4 = points[3];

    int *top = malloc(sizeof(int) * 3);
    int top_index = 0;

    if (is_min(p1[1], p2[1], p3[1], p4[1]))
    {
        top[top_index] = 0;
        top_index++;
    }
    if (is_min(p2[1], p1[1], p3[1], p4[1]))
    {
        top[top_index] = 1;
        top_index++;
    }
    if (is_min(p3[1], p2[1], p1[1], p4[1]))
    {
        top[top_index] = 2;
        top_index++;
    }
    if (is_min(p4[1], p2[1], p3[1], p1[1]))
    {
        top[top_index] = 3;
        top_index++;
    }

    int t0 = top[0];
    int t1 = top[1];
    free(top);

    if (top_index == 1)
        return t0;
    else
    {
        if (points[t0][0] < points[t1][0])
            return t0;
        return t1;
    }
}

int get_bl(float **points)
{
    float *p1 = points[0];
    float *p2 = points[1];
    float *p3 = points[2];
    float *p4 = points[3];

    int *bottom = malloc(sizeof(int) * 3);
    int bottom_index = 0;

    if (is_min(p1[0], p2[0], p3[0], p4[0]))
    {
        bottom[bottom_index] = 0;
        bottom_index++;
    }
    if (is_min(p2[0], p1[0], p3[0], p4[0]))
    {
        bottom[bottom_index] = 1;
        bottom_index++;
    }
    if (is_min(p3[0], p2[0], p1[0], p4[0]))
    {
        bottom[bottom_index] = 2;
        bottom_index++;
    }
    if (is_min(p4[0], p2[0], p3[0], p1[0]))
    {
        bottom[bottom_index] = 3;
        bottom_index++;
    }

    int b0 = bottom[0];
    int b1 = bottom[1];
    free(bottom);

    if (bottom_index == 1)
        return b0;
    else
    {
        if (points[b0][1] < points[b1][1])
            return b1;
        return b0;
    }
}

int get_tr(float **points)
{
    float *p1 = points[0];
    float *p2 = points[1];
    float *p3 = points[2];
    float *p4 = points[3];

    int *top = malloc(sizeof(int) * 3);
    int top_index = 0;

    if (is_max(p1[0], p2[0], p3[0], p4[0]))
    {
        top[top_index] = 0;
        top_index++;
    }
    if (is_max(p2[0], p1[0], p3[0], p4[0]))
    {
        top[top_index] = 1;
        top_index++;
    }
    if (is_max(p3[0], p2[0], p1[0], p4[0]))
    {
        top[top_index] = 2;
        top_index++;
    }
    if (is_max(p4[0], p2[0], p3[0], p1[0]))
    {
        top[top_index] = 3;
        top_index++;
    }

    int t0 = top[0];
    int t1 = top[1];
    free(top);

    if (top_index == 1)
        return t0;
    else
    {
        if (points[t0][1] > points[t1][1])
            return t1;
        return t0;
    }
}

int get_br(float **points)
{
    float *p1 = points[0];
    float *p2 = points[1];
    float *p3 = points[2];
    float *p4 = points[3];

    int *bottom = malloc(sizeof(int) * 3);
    int bottom_index = 0;

    if (is_max(p1[1], p2[1], p3[1], p4[1]))
    {
        bottom[bottom_index] = 0;
        bottom_index++;
    }
    if (is_max(p2[1], p1[1], p3[1], p4[1]))
    {
        bottom[bottom_index] = 1;
        bottom_index++;
    }
    if (is_max(p3[1], p2[1], p1[1], p4[1]))
    {
        bottom[bottom_index] = 2;
        bottom_index++;
    }
    if (is_max(p4[1], p2[1], p3[1], p1[1]))
    {
        bottom[bottom_index] = 3;
        bottom_index++;
    }

    int b0 = bottom[0];
    int b1 = bottom[1];
    free(bottom);

    if (bottom_index == 1)
        return b0;
    else
    {
        if (points[b0][0] > points[b1][0])
            return b0;
        return b1;
    }
}
