#pragma once

int *square_detection(SDL_Surface *s, float **lines, int len);
int is_perpendicular(float *line1, float *line2);
float *intersection_point(float **lines, int i, int j);
