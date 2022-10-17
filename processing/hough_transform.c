#include <math.h>

int* hough_transform(SDL_Surface* s)
{
    // Get width and height of the image
    int Nx = s->w;
    int Ny = s->h;

    // Max distance equals to the diagonal
    int Maxdist = sqrt(Nx*Nx + Ny*Ny);

    int thetas[180] = {};
    for (int i = -90; i < 90; i++)
        thetas[i + 90] = i;

    int rhos[2*Maxdist] = {};
    for (int i = -Maxdist; i < Maxdist; i++)
        rhos[i + Maxdist] = i;

    int accumulator[2*Maxdist][180] = {};

    for (int y = 0; y < Ny; y++)
    {
        for (int x = 0; x < Nx; x++)
        {
            Uint8 r = 0;
            Uint8 g = 0;
            Uint8 b = 0;
            SDL_GetRGB(pixel, format, &r, &g, &b);

            // Check if it is an edge pixel
            // NB: y -> rows , x -> columns
            if (r == 255 && g == 255 && b == 255)
            {
                for (int k = 0; k < 180; k++)
                {
                    // Calculate space parameter
                    int r = x * cos(thetas[k]) + y * sin(thetas[k]);

                    // Update the accumulator
                    // N.B: r has value -max to max
                    // map r to its idx 0 : 2*max
                    accumulator[r + Maxdist][k] += 1;
                }
            }
        }
    }

    return [&accumulator, &thetas, &rhos, 2*Maxdist];
}
