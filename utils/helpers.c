#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

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

int degrees_to_rad(int degrees)
{
    double r = (double) degrees * (3.14149265359 / 180);
    return (int) r;
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
