#include <err.h>
#include <stdio.h>
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

    Uint8 mid = 127; //(black - white)/2 + white;

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
                    Uint8* pixels,Uint32* results,SDL_PixelFormat* format ,
                    int x,int y,int width, int height,
                    char (*limit)(int*,int*,int,int))
{

      printf("[%u=%u]",pixels[10],get_gray(results[10],format));
      Uint8 curpix,midgray,gray;
      Uint32 nextpix;
      while((*limit)(&x,&y,height,width))
      {
	//printf("/%i/",y);
        curpix = pixels[x + y * width];
        nextpix = results[x + y * width];
	//printf("/%u|%u/",get_gray(curpix,format),results[x+y*width]);
	printf("/%u/",nextpix);

        midgray = (black - white)/2 + white;
        gray = curpix;

	//printf("/%u/",temp);
        if(curpix <= midgray)
	{
          gray = gray - 31,white = curpix;
	}
        else
        {
          gray = gray + 31,black = curpix;
	}
       // nextpix = SDL_MapRGB(format,  gray, gray, gray);
      }
}

void surface_to_blackORwhite_Rec(SDL_Surface* surface)
{
  Uint32* pixels = surface->pixels;
  int width = surface->w;
  int height = surface->h;
  int len = width * height;
  SDL_PixelFormat* format = surface->format;

  //SDL_Surface *surfaceTEMP = SDL_CreateRGBSurfaceFrom(pixels,width,height,32,4*width,0,0,0,0);
  //if(surfaceTEMP == NULL)
    //errx(EXIT_FAILURE,"%s",SDL_GetError());
  /*SDL_Surface *surfaceTEMP = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);
  if(SDL_BlitSurface(surface,NULL,surfaceTEMP,NULL) != 0)
    errx(EXIT_FAILURE,"%s",SDL_GetError());
  Uint32 *pixelsTEMP = surfaceTEMP->pixels;*/
  

  Uint8 *save = NULL;
  save = (Uint8 *) malloc(len);
  copy(pixels,save,len,format);

  //if(SDL_LockSurface(surfaceTEMP) != 0)
    //errx(EXIT_FAILURE,"%s",SDL_GetError());

  if(SDL_LockSurface(surface) != 0)
    errx(EXIT_FAILURE,"%s",SDL_GetError());

  Uint8 black = 0 , white = 255;
  get_max_and_min(pixels,format,len,&white,&black);

  SDL_FillRect(surface,NULL,SDL_MapRGB(format,127,127,127));
  SDL_UnlockSurface(surface);
  //SDL_LockSurface(surface);

    char down(int *x,int *y,int width,int height)
    {
      (void)(x);
      (void)(width);
      *y=*y+1;
      return *y < height;
    }
    char up(int *x,int *y,int width,int height)
    {
      (void)(x);
      (void)(width);
      (void)(height);
      *y=*y-1;
      return *y >= 0;
    }
    char right(int *x,int *y,int width,int height)
    {
      (void)(y);
      (void)(height);
      *x=*x+1;
      return *x < width;
    }
    char left(int *x,int *y,int width,int height)
    {
      (void)(y);
      (void)(height);
      (void)(width);
      *x=*x-1;
      return *x >= 0;
    }

    
    //for(int x = 0;x < width;x++)
    black_or_white(white,black,save,pixels,format,0,0,width,height,&down);
    printf("%u/%u\n",black,white);/*
    for(int x = 0;x < width;x++)
      black_or_white(white,black,pixelsTEMP,pixels,format,x,height-1,width,height,&up);
    printf("mid");
    for(int y = 0;y < height;y++)
      black_or_white(white,black,pixelsTEMP,pixels,format,0,y,width,height,&left);
    for(int y = 0;y < height;y++)
      black_or_white(white,black,pixelsTEMP,pixels,format,width-1,y,width,height,&right);

    surface_to_simple_blackORwhite(surface);

    */
  //SDL_UnlockSurface(surfaceTEMP);
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
