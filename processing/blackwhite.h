#pragma once

void draw(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Window *window, 
    const SDL_Rect * srcrect, const SDL_Rect * dstrect);
SDL_Surface* load_image(const char* path);
void surface_to_grayscale(SDL_Surface* surface);
