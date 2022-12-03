#include <err.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "blackwhite.h"
#include "grid_detection.h"
#include "helpers.h"
#include "rotate.h"
#include "tools.h"
#include "filter.h"
#include "morph.h"
#include "edge.h"

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
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    draw(renderer, t);
                }
                break;
        }
    }
}

int processing_image(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc < 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    // - Initialize the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a surface from the colored image.
    SDL_Surface* s = load_image(argv[1]);

    double w = s->w;
    double h = s->h;

    // - Convert the surface into grayscale.
    surface_to_grayscale(s);
    double average = 0;
    Uint32 *pixels = s->pixels;
    Uint8 c;
    for (int i = 0; i < w * h; i++)
    {
        SDL_GetRGB(pixels[i], s->format, &c, &c, &c);
        average += (double)c / (double)(w * h);
    }

    if (average > 175)
        filter_normalize(s);
    filter_gamma(s);
    filter_contrast(s);
    morph(s);
    otsu(s);
    //canny_edge_detector(s);

    /*
    // - Grid Detection
    double angle = -1;
    for (int i = 1; i < argc - 1; i++)
        if (argv[i][0] == '-' && argv[i][1] == 'a')
            angle = str_to_double(argv[i+1]);

    grid_detection(s, &angle);

    int stop = 0;
    for (int i = 1; i < argc - 1; i++)
    {
        if (argv[i][0] == '-')
        {
            if (argv[i][1] == 'd')
                stop = 0;
            if (argv[i][1] == 's' && i + 2 < argc)
            {
                w = str_to_double(argv[i+1]);
                h = str_to_double(argv[i+2]);
            }
        }
    }

    if (stop)
        return EXIT_SUCCESS;
    */
    // - Create a window.
    SDL_Window* window = SDL_CreateWindow("Display Image", 0, 0, 1, 1,
            SDL_WINDOW_SHOWN);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Resize the window according to the size of the image.
    SDL_SetWindowSize(window, w, h);

    // - Create a new texture from the grayscale surface.
    SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);

    // - Free the surface.
    SDL_FreeSurface(s);

    // - Dispatch the events.
    event_loop(renderer, t);

    // - Destroy the objects.
    SDL_DestroyTexture(t);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
