#pragma once

double *create_array(int size, double min, double max, double step);
double degrees_to_rad(double degrees);
void print_2d_array(int **array, int size_x, int size_y);
int *draw_line(SDL_Surface *s, int x0, int y0, int x1, int y1);
double str_to_double(char *str);
int **append_lines(int **L, int *len, int x1, int y1, int x2, int y2);
int comp(int sign, int x, int y, int threshold);
int is_min(int i, int j, int k, int l);
int is_max(int i, int j, int k, int l);
int get_tl(float **points);
int get_bl(float **points);
int get_tr(float **points);
int get_br(float **points);
int clamp(int val, int min, int max);
Uint32 *copy_pixels(SDL_Surface *s);
