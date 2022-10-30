#include <err.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "processing.h"
#include "NNXOR.h"


int main(int argc, char **argv)
{
    if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'n')
        NNXOR();
    else
        processing_image(argc, argv);

    return EXIT_SUCCESS;
}
