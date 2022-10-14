#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

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

int find_lines(SDL_Surface* s, int line[][2])
{
    Uint32* pixels = s->pixels;
    if(SDL_LockSurface(s) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_PixelFormat* format = s->format;

    int w = s->w;
    int h = s->h;
    int len = w * h;
    printf("len = %i\n", len);

    int R = sqrt(w*w + h*h);
    printf("R = %i\n", R);

    int tol = 10;
    int min = 10;

    int blackPixels[] = {};
    int blackIndex = 0;

    // Get all black pixels
    for(int i = 0; i < len; i++)
    {
        if(is_pixel_black(pixels[i], format, tol, min))
        {
            // printf("Pixels[%i] is black \n", i);
            blackPixels[blackIndex] = i;
            blackIndex++;
        }
    }

    int i = 0;

    int M[][180] = {};

    while(i <= blackIndex)
    {
        int x = i / w;
        int y = i % w;

        for(int t = 0; t < 180; t++)
        {
            int p = x * sin(t * (M_PI/180)) + y * cos(t * (M_PI/180));
            M[p+R][t] += 1;
        }

        i++;
    }

    int maxV = 0;

    for(int j = 0; j < 2*R; j++)
    {
        for(int k = 0; i < 180; k++)
        {
            if(M[j][k] > maxV)
            {
                maxV = M[j][k];
            }
        }
    }

    SDL_UnlockSurface(s);
    return 0;
}
