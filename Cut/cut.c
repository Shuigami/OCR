#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "cut.h"

void cut(SDL_Surface* surface)
{
	SDL_PixelFormat* format = surface->format;



    int length = (surface->w)/9;
    int pas = length*0.10;
    size_t count = 0;

    size_t i2 = 0;
    size_t j2 = 0;
    i2 += pas;
    j2 += pas;
    remove("split.txt");
    FILE *f;
    f = fopen("split.txt", "a");

    for(size_t i = 0; i < 9; i++)
    {
	    for(size_t j = 0; j < 9; j++)
	    {
		    SDL_Surface* surface_tmp = SDL_CreateRGBSurfaceWithFormat(0, 28, 28, 32, format->format);
		    SDL_Rect rect;
		    rect.x = j2;
		    rect.y = i2;
		    rect.w = length - pas;//nb_pixel;
		    rect.h = length - pas;//nb_pixel;

					  
		    if(SDL_BlitScaled(surface, &rect, surface_tmp, NULL)) // blitsSurface
		    {
			errx(1, "error");
		    }


		    Uint8 mat[784] = {};
		    SDL_Surface* surface2 = surface_tmp;
		    Uint32* pixels = surface2->pixels;
		    size_t len = 783;


		    Uint8 r,g,b;

		    for(size_t i = 0; i < len; i++)
		    {
			    SDL_GetRGB(pixels[i], format, &r, &g, &b);
			    if(r < 128)
				    mat[i] = 49;
			    else
				    mat[i] = 48;
			    fputc(mat[i], f);
			    fputc(44, f);
		    }
		    SDL_GetRGB(pixels[i], format, &r, &g, &b);
		    if(r < 128)
			    mat[i] = 49;
		    else
			    mat[i] = 48;
		    fputc(mat[i], f);


		    
		    fputc(10, f);

		    count++;

		    j2 += length;

	    }
	    j2 = pas;
	    i2 += length;
    }
}
