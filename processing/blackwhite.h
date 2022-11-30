#pragma once

void surface_to_grayscale(SDL_Surface* surface);
void surface_to_blackORwhite(SDL_Surface* surface);
void surface_to_blackORwhite_Rec(SDL_Surface* surface);
void surface_to_simple_blackORwhite(SDL_Surface* surface);
void filter_contrast(SDL_Surface *s);
void filter_gamma(SDL_Surface *s);
void otsu(SDL_Surface* surface);
