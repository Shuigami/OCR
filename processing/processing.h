#pragma once

int processing_image(int argc, char** argv);
void draw(SDL_Renderer* renderer, SDL_Texture* texture);
SDL_Surface* load_image(const char* path);
void surface_to_grayscale(SDL_Surface* surface);
void event_loop(SDL_Renderer* renderer, SDL_Texture* grayscale);
