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

Uint8 get_gray(Uint32 pixel_color, SDL_PixelFormat* format)
{
  Uint8 c;
  SDL_GetRGB(pixel_color, format, &c, &c, &c);
  return c;
}

void black_or_white(Uint8 black,Uint8 white,Uint32* pixels,SDL_PixelFormat* format ,int x,int y,int width, int height)
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

      if (x < width)
        black_or_white(black,white,pixels,format,x+1,y,width,height);
      if (y < height)
        black_or_white(black,white,pixels,format,x,y+1,width,height);
}

void surface_to_blackORwhite(SDL_Surface* surface)
{
  Uint32* pixels = surface->pixels;
  int width = surface->w;
  int height = surface->h;
  if(SDL_LockSurface(surface) != 0)
      errx(EXIT_FAILURE, "%s", SDL_GetError());

  SDL_PixelFormat* format = surface->format;

  black_or_white(0,255,pixels,format,0,0,width,height);


  SDL_UnlockSurface(surface);
}
