#include <err.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "processing/processing.h"


int main(int argc, char **argv)
{
    processing_image(argc, argv);

    return EXIT_SUCCESS;
}
