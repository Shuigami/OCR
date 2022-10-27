#pragma once

void draw(SDL_Renderer* renderer, SDL_Texture* texture);
SDL_Surface* load_image(const char* path);
void surface_to_grayscale(SDL_Surface* surface);
