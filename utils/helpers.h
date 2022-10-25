#pragma once

double *create_array(int size, double min, double max, double step);
double degrees_to_rad(double degrees);
void print_2d_array(int **array, int size_x, int size_y);
int *draw_line(SDL_Surface *s, int x0, int y0, int x1, int y1);
double str_to_double(char *str);
int **append_lines(int **L, int *len, int x1, int y1, int x2, int y2);
