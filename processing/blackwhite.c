#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "processing.h"

void surface_to_grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    if(SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_PixelFormat* format = surface->format;

    for (int i = 0; i < len; i++)
        pixels[i] = pixel_to_grayscale(pixels[i], format);

    SDL_UnlockSurface(surface);
}

void surface_to_simple_blackORwhite(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    if(SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_PixelFormat* format = surface->format;

    Uint8 black = 0 , white = 255;
    get_max_and_min(pixels,format,len,&white,&black);

    Uint8 mid = (black - white)/2 + white;

    for (int i = 0; i < len; i++)
    {
      if (get_gray(pixels[i],format) <= mid)
        pixels[i] = SDL_MapRGB(format, 0, 0, 0);
      else if (get_gray(pixels[i],format) > mid)
        pixels[i] = SDL_MapRGB(format, 255, 255, 255);
    }

    SDL_UnlockSurface(surface);
}


void black_or_white(Uint8 black,Uint8 white,
                    Uint32* pixels,Uint32* results,SDL_PixelFormat* format ,
                    int x,int y,int width, int height,
                    char (*limit)(*int,*int,int,int))
{
      Uint8 midgray = (black - white)/2 + white;
      Uint8 temp = get_gray(pixels[x+y*width],format);

      if(temp <= midgray)
        results[x+y*width] = SDL_MapRGB(format, 0, 0, 0),white = temp;
      else
        results[x+y*width] = SDL_MapRGB(format, 255, 255, 255),black = temp;

      if ((*limit)(&x,&y,height,width))
        black_or_white(black,white,pixels,format,x,y,width,height,(*limit)(*int,*int,int,int));

}

void surface_to_blackORwhite_Rec(SDL_Surface* surface)
{
  Uint32* pixels = surface->pixels;
  int width = surface->w;
  int height = surface->h;
  if(SDL_LockSurface(surface) != 0)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  SDL_PixelFormat* format = surface->format;

    Uint8 black = 0 , white = 255;
    get_max_and_min(pixels,format,width*height,&white,&black);

    char down(int *x,int *y,int width,int height)
    {
      y++;
      return y < height;
    }
    char up(int *x,int *y,int width,int height)
    {
      y--;
      return y >= 0;
    }
    char right(int *x,int *y,int width,int height)
    {
      x++;
      return x < width;
    }
    char left(int *x,int *y,int width,int height)
    {
      x--;
      return x >= 0;
    }

    for(int x = 0;x < width;x++)
      black_or_white(white,black,pixels,format,x,0,width,height,&down);
    for(int x = 0;x < width;x++)
      black_or_white(white,black,pixels,format,x,height-1,width,height,&up);

    for(int y = 0;x < height;y++)
      black_or_white(white,black,pixels,format,x,0,width,height,&down);
    for(int y = 0;x < height;y++)
      black_or_white(white,black,pixels,format,x,height-1,width,height,&up);

  SDL_UnlockSurface(surface);
}


void surface_to_blackORwhite(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int width = surface->w;
    int height = surface->h;
    if(SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_PixelFormat* format = surface->format;

    Uint8 black = 0 , white = 255;
    get_max_and_min(pixels,format,width*height,&white,&black);


    for (int x = 0; x < width-1; x++)
    {

      for (int y = 0; y < height-1; y++)
      {

        Uint8 midgray = (black - white)/2 + white;
        Uint8 save = get_gray(pixels[x+y*width],format);

        if(save <= midgray)
        {
          pixels[x+y*width] = SDL_MapRGB(format, 0, 0, 0);
          white = save;
        }
        else
        {
          pixels[x+y*width] = SDL_MapRGB(format, 255, 255, 255);
          black = save;
	}
      }
    }

    SDL_UnlockSurface(surface);
}
