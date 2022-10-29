#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <helpers.h>

void corrected_rotation_matrix(double angle, double x, double y,
    double center_x, double center_y, double *rx, double *ry)
{
    *rx = (x - center_x) * cos(angle) - (y - center_y) * sin(angle) + center_x;
    *ry = (x - center_x) * sin(angle) + (y - center_y) * cos(angle) + center_y;
}

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
            if (hough_accumulator[r][t] >= 255)
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

    most_freq_theta = abs((int)most_freq_theta);

    printf("不 Automatic Rotation: %.2f\n", most_freq_theta);

    return most_freq_theta;
}

void rotate(SDL_Surface *s, SDL_Surface *d, float angle)
{
    angle = degrees_to_rad(angle);

    int w = s->w;
    int h = s->h;
    Uint32 *pixels = s->pixels;
    Uint32 *rPixels = d->pixels;

    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            double rx;
            double ry;
            corrected_rotation_matrix(
                angle, (double)x, (double)y, w/2, h/2, &rx, &ry);

            if (0 <= rx && rx < w && 0 <= ry && ry < h)
            {
                Uint8 r, g, b;
                SDL_GetRGB(pixels[(int)ry * w + (int)rx], s->format, &r, &g, &b);
                Uint32 color = SDL_MapRGB(d->format, r, g, b);
                rPixels[y * w + x] = color;
            }
            else
            {
                Uint32 black = SDL_MapRGB(d->format, 0, 0, 0);
                rPixels[y * w + x] = black;
            }
        }
    }
}
