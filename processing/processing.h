#pragma once

int find_lines(SDL_Surface* s, int line[][2]);
int processing_image(int argc, char** argv);
void draw(SDL_Renderer* renderer, SDL_Texture* texture, int angle);
SDL_Surface* load_image(const char* path);
void surface_to_grayscale(SDL_Surface* surface);
void surface_to_blackORwhite(SDL_Surface* surface);
void event_loop(SDL_Renderer* renderer, SDL_Texture* grayscale, int angle);
