#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <err.h>
#include "helpers.h"
#include "square_detection.h"

float **sort_points(float **lines, int *square_corners)
{
    float threshold = .5;

    int i = square_corners[0];
    int j = square_corners[1];
    int k = square_corners[2];
    int l = square_corners[3];

    float *p1 = intersection_point(lines, i, j);
    float *p2 = intersection_point(lines, j, k);
    float *p3 = intersection_point(lines, k, l);
    float *p4 = intersection_point(lines, l, i);

    int max = 0;
    if (p1[0] > max)
        max = p1[0];
    if (p1[1] > max)
        max = p1[1];
    if (p2[0] > max)
        max = p2[0];
    if (p2[1] > max)
        max = p2[1];
    if (p3[0] > max)
        max = p3[0];
    if (p3[1] > max)
        max = p3[1];
    if (p4[0] > max)
        max = p4[0];
    if (p4[1] > max)
        max = p4[1];

    threshold *= max;

    printf("    ﬆ Finding corners...\n");
    float **points = malloc(sizeof(float*) * 4);
    for (int i = 0; i < 4; i++)
    {
        points[i] = malloc(sizeof(float) * 2);
        for (int j = 0; j < 2; j++)
            points[i][j] = -1.;
    }

    float **comp_points = malloc(sizeof(float*) * 4);
    comp_points[0] = p1;
    comp_points[1] = p2;
    comp_points[2] = p3;
    comp_points[3] = p4;

    points[0] = comp_points[get_tl(comp_points)];
    points[1] = comp_points[get_bl(comp_points)];
    points[2] = comp_points[get_tr(comp_points)];
    points[3] = comp_points[get_br(comp_points)];

    return points;
}

void resize(SDL_Surface *s, float **lines, int *square_corners)
{
    printf("ﬕ Resizing...\n");
    float **points = sort_points(lines, square_corners);

    float *tl = points[0];
    printf("        - Top left corner = (%.0f,%.0f)\n", tl[0], tl[1]);
    float *bl = points[1];
    printf("        - Bottom left corner = (%.0f,%.0f)\n", bl[0], bl[1]);
    float *tr = points[2];
    printf("        - Top right corner = (%.0f,%.0f)\n", tr[0], tr[1]);
    float *br = points[3];
    printf("        - Bottom right corner = (%.0f,%.0f)\n", br[0], br[1]);

    printf("     Finding dimensions...\n");
    float w = sqrt((tl[0] - tr[0]) * (tl[0] - tr[0]) + (tl[1] - tr[1]) * (tl[1] - tr[1]));
    float h = sqrt((tl[0] - bl[0]) * (tl[0] - bl[0]) + (tl[1] - bl[1]) * (tl[1] - bl[1]));
    printf("        - Width of square : %f\n", w);
    printf("        - Height of square : %f\n", h);

    int sizeLine = 1;
    w += sizeLine;
    h += sizeLine;
    SDL_Rect newRect;
    int x = tl[0];
    int y = tl[1];
    newRect.x = x;
    newRect.y = y;
    newRect.w = w;
    newRect.h = h;
    const SDL_Rect newRectC = newRect;

    SDL_Rect pastRect;
    pastRect.x = 0;
    pastRect.y = 0;
    pastRect.w = s->w;
    pastRect.h = s->h;

    SDL_BlitSurface(s, &newRectC, s, &pastRect);
    s->w = w;
    s->h = h;

    printf("\n");
}

