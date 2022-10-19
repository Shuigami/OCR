#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <helpers.h>

int is_pixel_black(Uint32 pixel, SDL_PixelFormat* format, int tol, int min)
{
    Uint8 r = 0;
    Uint8 g = 0;
    Uint8 b = 0;
    SDL_GetRGB(pixel, format, &r, &g, &b);

    if (r < g - tol || r > g + tol ||
        r < b - tol || r > b + tol ||
        g < r - tol || g > r + tol ||
        g < b - tol || g > b + tol ||
        b < r - tol || b > r + tol ||
        b < g - tol || b > g + tol ||
        r < min || g < min || b < min)
    {
        // printf("Pixel black : %i, %i, %i\n", r, g, b);
        return 1;
    }

    return 0;
}

double automatic_rotation(int **hough_accumulator, SDL_Surface *s)
{
    double w = s->w;
    double h = s->h;
    double diag = sqrt(w*w + h*h);
    double size = diag * 2;

    double theta_min = -90;
    double theta_max = 90;
    int theta_num = diag * 2;
    double theta_step = (theta_max - theta_min) / theta_num;
    double *thetas = create_array(theta_num + 1, theta_min, theta_max, theta_step);

    int *thetas_v = malloc(sizeof(int) * (size + 1) + 1);
    for (int r = 0; r <= size; r++)
    {
        thetas_v[r] = 0;
    }

    for (int r = 0; r <= size; r++)
    {
        for (int t = 0; t <= size; t++)
        {
            if (hough_accumulator[r][t] >= 9)
                thetas_v[t] += hough_accumulator[r][t];
        }

        free(hough_accumulator[r]);
    }

    double most_freq_theta = 0;
    double most_freq_votes = thetas_v[0];

    for (int i = 1; i <= size; i++)
    {
        int votes = thetas_v[i];
        if (votes > most_freq_votes)
        {
            most_freq_theta = thetas[i];
            most_freq_votes = votes;
        }
    }

    free(hough_accumulator);
    free(thetas);
    free(thetas_v);

    if (most_freq_theta < 0)
        most_freq_theta = most_freq_theta + 90;

    printf("automatic rotation: %.2f\n", most_freq_theta);

    return most_freq_theta;
}
