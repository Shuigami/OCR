#include <stddef.h>
#include <math.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

cut(SDL_Surface* surface, int length, int x, int y)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    if(SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_PixelFormat* format = surface->format;


    int L[len] = {};
    size_t = count = 0;

    int nb_pixel = sqrt(len)/length;

    for(size_t i = 0; i < nb_pixel; i++)
    {
	    for(size_t j = 0; j < nb_pixel; j++)
	    {
		    SDL_Surface* surface_tmp = SDL_CreateRGBSurfaceWithFormat(0, length, length, 32, format);
		    SDL_LockSurface(surface_tmp);
		    Uint32* pixels_tmp;

		    for(size_t i2 = i*length; i2 < i*length+length; i2++)
		    {
			    for(size_t j2 = j*length; j2 < j*length+length; j2++)
			    {
				if(i != 0 || j != 0)
					pixels_tmp[(i2/i)*nb_pixel + (j2/j)] = pixel[i2*nb_pixel + j2];//surement remplacer les nb_pixel par length a gauche
				else if (i != 0)
					pixels_tmp[(i2/i)*nb_pixel + j2] = pixel[i2*nb_pixel + j2];    //surement multiplier les i2 et j2 par i et j a droite
				else if  (j != 0)
					pixels_tmp[i2*nb_pixel + (j2/j)] = pixel[i2*nb_pixel + j2];    //changer a droite pour n'avoir que pixels_tmp[count]=
				else
					pixels_tmp[i2*nb_pixel + j2] = pixel[i2*nb_pixel + j2];


			    }
		    }

		    SDL_UnlockSurface(surface_tmp)
		    L[count] = surface_tmp;
		    count++;

	    }
    }

	}


    SDL_UnlockSurface(surface);

}
