#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "cut.h"

/*SDL_Surface* load_image(const char* path)
{
    SDL_Surface* tmp = IMG_Load(path);
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(tmp);
    return surface;
}*/

int main(int argc, char **argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    // - Initialize the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a window.
    SDL_Window* window = SDL_CreateWindow("Display Image", 0, 0, 0, 0,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0); //SDL_RENDERER_ACCELERATED
    if(renderer == NULL) 
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    // - Create a surface from the colored image.
    SDL_Surface* s = load_image(argv[1]);
    
    SDL_Surface* L[81];
    //SDL_Surface **L = malloc(sizeof(SDL_Surface) * 81);
    cut(s, L);

    SDL_Rect rect;
    rect.x = 888;
    rect.y = 888;
    rect.w = 111;
    rect.h = 111;

    SDL_Rect rect2;
    rect2.x = 777;
    rect2.y = 888;
    rect2.w = 111;
    rect2.h = 111;

    SDL_Surface* p[2];

    SDL_Surface* tmp = SDL_CreateRGBSurfaceWithFormat(0, s->w, s->h, 28, s->format->format);
    SDL_Surface* tmp2 = SDL_CreateRGBSurfaceWithFormat(0, s->w, s->h, 28, s->format->format);


    SDL_BlitSurface(s, &rect, tmp, NULL);
    SDL_BlitSurface(s, &rect2, tmp2, NULL);


    p[0] = tmp;
    p[1] = tmp2;


    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, L[80]);

    int w, h;
    if(SDL_QueryTexture(texture, NULL, NULL, &w, &h) != 0)
	    errx(EXIT_FAILURE, "%s", SDL_GetError());
		    
    // - Resize the window according to the size of the image.	   
    SDL_SetWindowSize(window, w, h);


    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    /*int i = 0;
    while(L[i] != 0)
    {
	    
    }*/

       
    // - Convert the surface into grayscale.
    //surface_to_grayscale(s);

    // - Create a new texture from the grayscale surface.
    //SDL_Texture* grayT = SDL_CreateTextureFromSurface(renderer, s)
    
    SDL_Event event;


    while(1)
    {
	SDL_WaitEvent(&event);
	if(event.type == SDL_QUIT)
		break;
    }

    // - Free the surface.
    SDL_FreeSurface(s);
    size_t k = 0;
    while(L[k] != 0)
    {
	    SDL_FreeSurface(L[k]);
    }

    // - Dispatch the events.
    //event_loop(renderer, grayT);

    // - Destroy the objects.
    //SDL_DestroyTexture(grayT);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    
    return EXIT_SUCCESS;
}

