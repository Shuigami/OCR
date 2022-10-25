#pragma once

void grid_detection(SDL_Surface* s, double *angle);
int **find_lines(int **accumulator, SDL_Surface* s, double *rhos, 
        double *thetas, int **lines, int *len_lines);
