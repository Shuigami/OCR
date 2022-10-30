#include <err.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Updates the display.
//
// renderer: Renderer to draw on.
// texture: Texture that contains the image.
void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_RenderCopyEx(renderer, texture, NULL, NULL, 0, NULL, SDL_FLIP_NONE);
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

void get_max_and_min(Uint32* pixels,SDL_PixelFormat *format,int len,Uint8 *min,Uint8 *max)
{
  for (int i = 0; i < len; i++)
  {
    Uint8 curr = get_gray(pixels[i],format);
    if (curr < *min)
      *min = curr;
    if (curr > *max)
      *max = curr;
  }
}

Uint8 get_average(Uint32* pixels, SDL_PixelFormat *format, int len)
{
  Uint8 average = 0;
  for (int i = 0; i < len; i++)
  {
      Uint8 curr = get_gray(pixels[i],format);
      average += curr;
  }
  return average/len;
}


void copy(Uint32 *src,Uint8 *dst,size_t len,SDL_PixelFormat* format)
{
  for(size_t i = 0; i < len; i++)
    dst[i] = get_gray(src[i],format);
}

void fill(Uint32* pixels,size_t len,SDL_PixelFormat* format)
{
  for(size_t i = 0; i < len; i++)
    pixels[i] = SDL_MapRGB(format, 127,  127,  127);
}
