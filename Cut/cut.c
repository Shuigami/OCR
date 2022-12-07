#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "cut.h"

void cut(SDL_Surface* surface, SDL_Surface*** L)
{
    //Uint32* pixels = surface->pixels;
    //int len = surface->w * surface->h;
    /*if(SDL_LockSurface(surface) != 0)
       errx(EXIT_FAILURE, "%s", SDL_GetError());*/
    SDL_PixelFormat* format = surface->format;


    int length = (surface->w)/9;
    //length *= 0.95;
    int pas = length*0.10;
    int spas = length*0.05;
    //SDL_Surface* L[81] = {};
    size_t count = 0;

    int nb_pixel = surface->w;  
    size_t i2 = 0;
    size_t j2 = 0;
    //i2 += pas;
    //j2 += pas;

    for(size_t i = 0; i < 9; i++)
    {
	    for(size_t j = 0; j < 9; j++)
	    {
		    SDL_Surface* surface_tmp = SDL_CreateRGBSurfaceWithFormat(0, surface->w, surface->h, 28, format->format);
		    SDL_Rect rect;
		    rect.x = j2;
		    rect.y = i2;
		    rect.w = length;//nb_pixel;
		    rect.h = length;//nb_pixel;

		    /*if(i%3 == 0)
			    rect.w -= pas;
		    else
			    rect.w -= spas;
		    if(j%3 == 0)
			    rect.h -= pas;
		    else
			    rect.h -= spas;*/
					  
		    if(!SDL_BlitSurface(surface, &rect, surface_tmp, NULL)) // blitsSurface
			printf("error");

		    (*L)[count] = surface_tmp;
		    count++;

		    j2 += length;

	    }
	    //j2 = pas;
	    j2 = 0;
	    i2 += length;
    }

    /*for(size_t i = 0; i < (size_t)nb_pixel-1; i+=length) 
    {
	    for(size_t j = 0; j < (size_t)nb_pixel-1; j+=length)
	    {
		    SDL_Surface* surface_tmp = SDL_CreateRGBSurfaceWithFormat(0, surface->w, surface->h, 28, format->format);
		    SDL_Rect rect;
		    rect.x = j;
		    rect.y = i;
		    rect.w = length;//nb_pixel;
		    rect.h = length;//nb_pixel;
		    
		    if(!SDL_BlitSurface(surface, &rect, surface_tmp, NULL))
			printf("error");

		    (*L)[count] = surface_tmp;
		    count++;




		    //SDL_Surface* surface_tmp = (SDL_Surface*)malloc(sizeof(SDL_CreateRGBSurfaceWithFormat(0, surface->w, surface->h, 28, format->format)));//peut etre 32
		    //surface_tmp = malloc(sizeof(SDL_Surface));
		    //Uint32* pixels_tmp = surface_tmp->pixels;
		    //SDL_LockSurface(surface_tmp);
		    //size_t pixels_tmp_count = 0;
		    

		    //SDL_Rect dest;
		    //dest.x = 0;
		    //dest.y = 0;
		    //dest.w = nb_pixel;
		    //dest.h = nb_pixel;

		    //SDL_BlitSurface(surface, &rect, surface_tmp, NULL);
		    
		    //SDL_BlitScaled(surface, &rect, surface_tmp, NULL);



		    
		    //SDL_UnlockSurface(surface_tmp);
		    //printf("%zi\n",count);
		    *L = malloc(sizeof(SDL_Surface) * 82);
		    //if(*L == NULL)
		    //	    return;
		    //(*L)[count] = malloc(sizeof(SDL_Surface));
		    *(*L + count) = surface_tmp;
		    //L[count] = surface_tmp;



		    //j += length*0.05;
		}
	    //i += length*0.05;
	}*/

		    

    //SDL_UnlockSurface(surface);
    //return L;

}

/*SDL_Surface* load_image(const char* path)
{
    SDL_Surface* tmp = IMG_Load(path);
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(tmp);
    return surface;
}*/

