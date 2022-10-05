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
        return 1;
    }
    // printf("Pixel black : %i, %i, %i\n", r, g, b);
    return 0;
}

// dir= 1 => left
//      2 => bottom right
//      3 => bottom
//      4 => bottom left

int[] find_line(Uint32* pixels, int i, int dir)
{
    int j = i;

    switch (dir)
    {
        case 1:
            while(is_pixel_black)
    }

}

void find_lines(SDL_Surface* s) 
{
    Uint32* pixels = s->pixels;
    int len = s->w * s->h;
    if(SDL_LockSurface(s) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_PixelFormat* format = s->format;

    int w = s->w;
    int h = s->h;

    int tol = 10;
    int min = 100;

    for (int i = 0; i < len; i++)
    {
        if(is_pixel_black(pixels[i], format, tol, min))
        {
            printf("Pixels[%i] black\n", i);

            if(is_pixel_black(pixels[i+1], format, tol, min))


                    || is_pixel_black(pixels[i+w-1], format, tol, min)
                    || is_pixel_black(pixels[i+w], format, tol, min)
                    || is_pixel_black(pixels[i+w+1], format, tol, min))
        }
    }

    SDL_UnlockSurface(s);
}
