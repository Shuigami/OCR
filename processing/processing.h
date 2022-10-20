#pragma once

int find_lines(SDL_Surface* s, int line[][2]);
int processing_image(int argc, char** argv);

void draw(SDL_Renderer* renderer, SDL_Texture* texture, int angle);
SDL_Surface* load_image(const char* path);
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);
Uint8 get_gray(Uint32 pixel_color, SDL_PixelFormat* format);
void get_max_and_min(Uint32* pixels,SDL_PixelFormat* format,int len,Uint8 *min,Uint8 *max);
void get_max_and_min(Uint32* source,Uint32* dest,int len);

void surface_to_grayscale(SDL_Surface* surface);
void surface_to_blackORwhite(SDL_Surface* surface);
void surface_to_blackORwhite_Rec(SDL_Surface* surface);
void surface_to_simple_blackORwhite(SDL_Surface* surface);

void event_loop(SDL_Renderer* renderer, SDL_Texture* grayscale, int angle);
