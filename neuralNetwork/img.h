#pragma once

#include "matrix.h"

typedef struct {
	Matrix* img_data;
	int label;
} Img;

Img** csv_to_imgs2(char* file_string, int number_of_imgs);
Img** csv_to_imgs(char* file_string, int number_of_imgs);
void writeee(int grid[81], char *filename);
void img_print(Img* img);
void img_free(Img *img);
void imgs_free(Img **imgs, int n);
