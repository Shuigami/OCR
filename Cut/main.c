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
   printf("y");
	// Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    // - Initialize the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a window.
    SDL_Window* window = SDL_CreateWindow("Display Image", 0, 0, 100, 100,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a surface from the colored image.
    SDL_Surface* s = load_image(argv[1]);
    
    SDL_Surface* L[81];
    printf("oui");
    cut(s, L);
    printf("ok");

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, L[0]);

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    /*int i = 0;
    while(L[i] != 0)
    {
	    
    }*/


    // - Resize the window according to the size of the image.
    //SDL_SetWindowSize(window, s->w, s->h);

    // - Convert the surface into grayscale.
    //surface_to_grayscale(s);

    // - Create a new texture from the grayscale surface.
    //SDL_Texture* grayT = SDL_CreateTextureFromSurface(renderer, s);
    while(1)
    {

    }

    // - Free the surface.
    SDL_FreeSurface(s);

    // - Dispatch the events.
    //event_loop(renderer, grayT);

    // - Destroy the objects.
    //SDL_DestroyTexture(grayT);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    
    return EXIT_SUCCESS;
}

