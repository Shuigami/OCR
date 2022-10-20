#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Updates the display.
//
// renderer: Renderer to draw on.
// texture: Texture that contains the image.
void draw(SDL_Renderer* renderer, SDL_Texture* texture, int angle)
{
    SDL_RenderCopyEx(renderer, texture, NULL, NULL, angle, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(renderer);
}

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
    SDL_Surface* tmp = IMG_Load(path);
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(tmp);
    return surface;
}

// Converts a colored pixel into grayscale.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    Uint8 average = 0.3*r + 0.59*g + 0.11*b;
    Uint32 color = SDL_MapRGB(format, average, average, average);
    return color;
}

Uint8 get_gray(Uint32 pixel_color, SDL_PixelFormat* format)
{
  Uint8 c;
  SDL_GetRGB(pixel_color, format, &c, &c, &c);
  return c;
}

void get_max_and_min(Uint32* pixels,SDL_PixelFormat* format,int len,Uint8 *min,Uint8 *max)
{
  for (int i = 0; i < len; i++)
  {
    Uint8 curr = get_gray(pixels[i],format);
    if (curr < min)
      min = curr;
    if (curr > max)
      max = curr;
  }
}

void get_max_and_min(Uint32* source,Uint32* dest,int len)
{
  for (int i = 0; i < len; i++)
    dest[i] = const source[i];
}
