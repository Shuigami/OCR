#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "cut.h"

void cut(SDL_Surface* surface, SDL_Surface* L[])
{
    //Uint32* pixels = surface->pixels;
    //int len = surface->w * surface->h;
    if(SDL_LockSurface(surface) != 0)
       errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_PixelFormat* format = surface->format;


    int length = (surface->w)/9;
    //SDL_Surface* L[81] = {};
    size_t count = 0;

    int nb_pixel = surface->w;  

    for(size_t i = 0; i < (size_t)nb_pixel-1; i+=length) 
    {
	    for(size_t j = 0; j < (size_t)nb_pixel-1; j+=length)
	    {
		    SDL_Surface* surface_tmp = SDL_CreateRGBSurfaceWithFormat(0, surface->w, surface->h, 28, format->format);//peut etre 32
		    //Uint32* pixels_tmp = surface_tmp->pixels;
		    //SDL_LockSurface(surface_tmp);
		    //size_t pixels_tmp_count = 0;
		    
		    SDL_Rect rect;
		    rect.x = i;
		    rect.y = j;
		    rect.w = nb_pixel;
		    rect.h = nb_pixel;

		    //SDL_BlitSurface(surface, &rect, surface_tmp, NULL);

		    if(!SDL_BlitSurface(surface, &rect, surface_tmp, NULL))
			printf("error");

		    /*for(size_t i2 = i; i2 < i+length; i2++)//utiliser blitsurface(surface, rect(i, j, length, length), surface_tmp, NULL) pour copier chaque		    {
			    for(size_t j2 = j; j2 < j+length; j2++)
			    {
				pixels_tmp[pixels_tmp_count] = pixels[i2*nb_pixel + j2];    
				pixels_tmp_count++;*/


				/*if(i != 0 || j != 0)
					pixels_tmp[(i2/i)*nb_pixel + (j2/j)] = pixel[i2*nb_pixel + j2];//surement remplacer les nb_pixel par length a gauche
				else if (i != 0)
					pixels_tmp[(i2/i)*nb_pixel + j2] = pixel[i2*nb_pixel + j2];    //surement multiplier les i2 et j2 par i et j a droite
				else if  (j != 0)
					pixels_tmp[i2*nb_pixel + (j2/j)] = pixel[i2*nb_pixel + j2];    //changer a droite pour n'avoir que pixels_tmp[count]=
				else
					pixels_tmp[i2*nb_pixel + j2] = pixel[i2*nb_pixel + j2];*/
		    
		    
		    //SDL_UnlockSurface(surface_tmp);
		    //printf("%zi\n",count);
		    //*(L + count) = surface_tmp;
		    L[count] = surface_tmp;
		    count++;



		}
	}

		    

    SDL_UnlockSurface(surface);
    //return L;

}

SDL_Surface* load_image(const char* path)
{
    SDL_Surface* tmp = IMG_Load(path);
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(tmp);
    return surface;
}

