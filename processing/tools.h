#pragma once

void draw(SDL_Renderer* renderer, SDL_Texture* texture);
SDL_Surface* load_image(const char* path);
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);
Uint8 get_gray(Uint32 pixel_color, SDL_PixelFormat* format);
void get_max_and_min(Uint32* pixels,SDL_PixelFormat* f,int len,Uint8 *min,Uint8 *max);
Uint8 get_average(Uint32* pixels, SDL_PixelFormat *format, int len);
void copy(Uint32 *pixels,Uint8 copy[],size_t len,SDL_PixelFormat* f);
void fill(Uint32* pixels,size_t len,SDL_PixelFormat* format);
