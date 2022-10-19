#include <err.h>
#include <stdlib.h>
#include <stdio.h>

int *create_array(int size, int min, int max, int step)
{
    int *array = malloc(sizeof(int) * size + 1);

    int cur = min;
    for (int i = 0; i < size && cur <= max; i++)
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
        {
            if (array[x][y] != 0)
                printf("array[%i][%i] = %i\n", x, y, array[x][y]);
        }
    }
}
