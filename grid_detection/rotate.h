#pragma once

int **rotate_lines(SDL_Surface *s, float angle, int **lines, int len);
int *rotate_line(SDL_Surface *s, float angle, int *line);
double automatic_rotation(int **hough_accumulator, SDL_Surface *s);
void rotate(SDL_Surface *s, SDL_Surface *d, float angle);
