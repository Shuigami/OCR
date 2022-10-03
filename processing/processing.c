#include <err.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "processing.h"

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
// colored: Texture that contains the colored image.
// grayscale: Texture that contains the grayscale image.
void event_loop(SDL_Renderer* renderer, SDL_Texture* grayscale)
{
    SDL_Event event;
    SDL_Texture* t = grayscale;

    while (1)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            // If the "quit" button is pushed, ends the event loop.
            case SDL_QUIT:
                return;

            // If the window is resized, updates and redraws the diagonals.
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    draw(renderer, t);
                break;
        }
    }
}

int processing_image(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    // - Initialize the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a window.
    SDL_Window* window = SDL_CreateWindow("Display Image", 0, 0, 1000, 1000,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a surface from the colored image.
    SDL_Surface* s = load_image(argv[1]);

    // - Resize the window according to the size of the image.
    SDL_SetWindowSize(window, s->w, s->h);

    // - Convert the surface into grayscale.
    surface_to_grayscale(s);

    // - Create a new texture from the grayscale surface.
    SDL_Texture* grayT = SDL_CreateTextureFromSurface(renderer, s);

    // - Free the surface.
    SDL_FreeSurface(s);

    // - Dispatch the events.
    event_loop(renderer, grayT);

    // - Destroy the objects.
    SDL_DestroyTexture(grayT);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

