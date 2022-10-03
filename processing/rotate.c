#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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
        return 0;
    }
    printf("Pixel black : %i, %i, %i\n", r, g, b);
    return 1;
}

void find_line(SDL_Surface* s) 
{
    Uint32* pixels = s->pixels;
    int len = s->w * s->h;
    if(SDL_LockSurface(s) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_PixelFormat* format = s->format;

    for (int i = 0; i < len; i++)
    {
        is_pixel_black(pixels[i], format, 10, 200);
    }


    SDL_UnlockSurface(s);
}
