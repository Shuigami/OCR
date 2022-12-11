#include <err.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "processing.h"
#include "helpers.h"
#include "rotate.h"
#include "tools.h"
#include "cut.h"
#include "nn_main.h"

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

void print_help()
{
    printf("Usage: ./main file [options]\n\n");
    printf("Options:\n");
    printf("\t-r <angle>        Rotate the image by the specified <angle>\n");
}

int main(int argc, char **argv)
{
    if (argc < 2)
        errx(EXIT_FAILURE, "Usage: image-file");
    
    double angle = -1;
    char *filename = NULL;

    if (argv[1][0] == '-' && argv[1][1] == 'h') {
        print_help();
        return EXIT_SUCCESS;
    } else {
        filename = argv[1];
    }

    for (int i = 2; i < argc; i++){
        if (i < argc - 1 && argv[i][0] == '-' && argv[i][1] == 'r')
            angle = strtod(argv[i+1], NULL);
    }


    // - Initialize the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a surface from the colored image.
    SDL_Surface* s = load_image(filename);

    double w = s->w;
    double h = s->h;

    printf("Size : %0f * %0f\n", w, h);

    if(SDL_SaveBMP(s, "result/0.0-original.bmp") != 0)
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());

    if (angle != -1){
        SDL_Surface *d = SDL_CreateRGBSurface(0, s->w, s->h, 32, 0, 255, 0, 0);
        rotate(s, d, angle);
        *s = *d;
        angle = 0;
    }
    
    // - Convert the surface into grayscale.
    processing_image(s, angle);
    cut(s);
    nn_function();

    // - Create a window.
    SDL_Window* window_sdl = SDL_CreateWindow("Display Image", 0, 0, 1, 1,
            SDL_WINDOW_SHOWN);
    if (window_sdl == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window_sdl, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Resize the window according to the size of the image.
    SDL_SetWindowSize(window_sdl, s->w, s->h);

    // - Create a new texture from the grayscale surface.
    SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);

    // - Free the surface.
    SDL_FreeSurface(s);

    // - Dispatch the events.
    event_loop(renderer, t);

    // - Destroy the objects.
    SDL_DestroyTexture(t);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window_sdl);
    SDL_Quit();

    return EXIT_SUCCESS;
}
