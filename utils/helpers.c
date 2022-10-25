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
        r = r * 10 + (double) str[i];

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
