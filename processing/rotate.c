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
        // printf("Pixel black : %i, %i, %i\n", r, g, b);
        return 1;
    }

    return 0;
}

int find_line(Uint32* pixels, int i, int dir, SDL_PixelFormat* format, int tol, int min, int w, int len)
{
    switch (dir)
    {
        // Right
        case 1:
            printf("Going right\n");
            while(i+1 < len && is_pixel_black(pixels[i+1], format, tol, min))
                i += 1;
            printf("Stop at %i\n\n", i);
            if(i+w < len && is_pixel_black(pixels[i+w], format, tol, min))
                return find_line(pixels, i, 3, format, tol, min, w, len);
            break;

        // Left
        case 2:
            printf("Going left\n");
            while(i-1 > 0 && is_pixel_black(pixels[i-1], format, tol, min))
                i -= 1;
            printf("Stop at %i\n\n", i);
            if(i+w < len && is_pixel_black(pixels[i+w], format, tol, min))
                return find_line(pixels, i, 4, format, tol, min, w, len);
            break;

        // Bottom to right
        case 3:
            printf("Going bottom to right\n");
            while(i+w < len && is_pixel_black(pixels[i+w], format, tol, min))
                i += w;
            printf("Stop at %i\n\n", i);
            if(i+1 < len && is_pixel_black(pixels[i+1], format, tol, min))
                return find_line(pixels, i, 1, format, tol, min, w, len);
            if(i+w+1 < len && is_pixel_black(pixels[i+w+1], format, tol, min))
                return find_line(pixels, i+w+1, 3, format, tol, min, w, len);

            break;

        // Bottom to left
        case 4:
            printf("Going bottom to left\n");
            while(i+w < len && is_pixel_black(pixels[i+w], format, tol, min))
                i += w;
            printf("Stop at %i\n\n", i);
            if(i-1 > 0 && is_pixel_black(pixels[i-1], format, tol, min))
                return find_line(pixels, i, 2, format, tol, min, w, len);
            break;

        // Bottom
        case 5:
            printf("Going bottom\n");
            while(i+w < len && is_pixel_black(pixels[i+w], format, tol, min))
                i += w;
            int left = 0;
            int right = 0;
            if(i-1 > 0 && is_pixel_black(pixels[i-1], format, tol, min))
                left = find_line(pixels, i, 2, format, tol, min, w, len);
            if(i+1 < len && is_pixel_black(pixels[i+1], format, tol, min))
                right = find_line(pixels, i, 1, format, tol, min, w, len);

            if(left > right)
                return left;
            else
                return right;
    }

    return i;
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

    int tol = 10;
    int min = 10;

    int i = 0;
    int k = 0;

    while(i < len)
    {
        while(i < len && is_pixel_black(pixels[i], format, tol, min) != 1)
            i += 1;

        if(i < len - 2)
        {
            printf("Pixels[%i] black\n", i);

            int dir = 0;

            int j = i;

            // Bottom left
            if(is_pixel_black(pixels[i+w-1], format, tol, min))
            {
                printf("Going right for left\n");
                while(i+1 < len && is_pixel_black(pixels[i+1], format, tol, min))
                    i += 1;
                printf("Stop at %i\n\n", i);
                dir = 4;
                i = find_line(pixels, i, dir, format, tol, min, w, len);
                printf("Line from %i to %i\n", j, i);
                line[k][0] = j;
                line[k][1] = i;
                k += 1;
                printf("k = %i\n", k);
                while(is_pixel_black(pixels[i+1], format, tol, min))
                    i+=1;
            }

            // Right
            else if(is_pixel_black(pixels[i+1], format, tol, min))
            {
                dir = 1;
                i = find_line(pixels, i, dir, format, tol, min, w, len);
                printf("Line from %i to %i\n", j, i);
                line[k][0] = j;
                line[k][1] = i;
                k += 1;
                printf("k = %i\n", k);
            }

            // Bottom
            else if(is_pixel_black(pixels[i+w], format, tol, min))
            {
                dir = 5;
                i = find_line(pixels, i, dir, format, tol, min, w, len);
                printf("Line from %i to %i\n", j, i);
                line[k][0] = j;
                line[k][1] = i;
                k += 1;
                printf("k = %i\n", k);
            }

            // Bottom right
            else if(is_pixel_black(pixels[i+w+1], format, tol, min))
            {
                i += w+1;
                dir = 1;
                i = find_line(pixels, i, dir, format, tol, min, w, len);
                printf("Line from %i to %i\n", j, i);
                line[k][0] = j;
                line[k][1] = i;
                k += 1;
                printf("k = %i\n", k);
            }

            if(dir == 0)
                printf("Not a line\n");
        }
        i += 1;
    }


    SDL_UnlockSurface(s);
    return k;
}
