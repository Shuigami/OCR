#include "img.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <unistd.h>
#include <limits.h>

#define MAXCHAR 10000


Img** csv_to_imgs2(char* file_string, int number_of_imgs) {
	int k = 0;
	FILE *fp;
	char row[MAXCHAR];
	fp = fopen(file_string, "r");
	if (fp == NULL)
		fprintf(stderr, "Error opening file %s\n", file_string);
	Img** imgs = malloc(number_of_imgs * sizeof(Img*));
	int i = 0;
	while(feof(fp) != 1 && i < number_of_imgs)
	{
		int j = 0;
		fgets(row,MAXCHAR,fp);
		char* token = strtok(row,"\n");
		while(token != NULL)
		{
			imgs[i] = malloc(sizeof(Img));
			imgs[i]->label = 0;
			imgs[i]->img_data = matrix_create(28,28);
			char* token2 =strtok(token,",");
			while(token2 != NULL)
			{
				if(j == 0)
				{
					imgs[i]->label = atoi(token2);
				}
				if(atoi(token2) > 0)
					imgs[i]->img_data->entries[(j-1) / 28][(j-1) % 28] = 255 / 256.0;
				if(atoi(token2) == 0)
					imgs[i]->img_data->entries[(j-1) / 28][(j-1) % 28] =0 /256.0;
				token2= strtok(NULL,",");
				j++;
			}
			token = strtok(NULL,"\n");
			i++;
			k++;
		}
	}
	fclose(fp);
	return imgs;
}
Img** csv_to_imgs(char* file_string, int number_of_imgs) {
	int k = 0;
	FILE *fp;
	char row[MAXCHAR];
	fp = fopen(file_string, "r");
	Img** imgs = malloc(number_of_imgs * sizeof(Img*));
	int i = 0;
	while(i < number_of_imgs)
	{
		if(feof(fp) == 1)
		{
			fclose(fp);
			FILE *fp;
			char row[MAXCHAR];
			fp = fopen(file_string,"r");
		}
		int j = 0;
		fgets(row,MAXCHAR,fp);
		char* token = strtok(row,"\n");
		while(token != NULL)
		{
			imgs[i] = malloc(sizeof(Img));
			imgs[i]->label = 0;
			imgs[i]->img_data = matrix_create(28,28);
			char* token2 =strtok(token,",");
			while(token2 != NULL)
			{
				if(j == 0)
				{
					imgs[i]->label = atoi(token2);
				}
				if(atoi(token2) > 0)
					imgs[i]->img_data->entries[(j-1) / 28][(j-1) % 28] = 255 / 256.0;
				if(atoi(token2) == 0)
					imgs[i]->img_data->entries[(j-1) / 28][(j-1) % 28] =0 /256.0;
				token2= strtok(NULL,",");
				j++;
			}
			token = strtok(NULL,"\n");
			i++;
			k++;
		}
	}
	fclose(fp);
	return imgs;
}
void writeee(int grid[81])
{
  size_t j = 0;
  char *save = calloc(111,sizeof(char));
  for(size_t i = 0; i<81 ;i++)
  {
    if(!grid[i])
      save[j] = '.';
    else
      save[j] = grid[i] + 48;
    j++;
    if(i == 26 || i == 53)
    {
        save[j] = '\n';
        j++;
    }
    if((i+1)%9 == 0)
    {
        save[j] = '\n';
        j++;
    }
    else if((i+1)%3 == 0 && i != 0)
    {
        save[j] = ' ';
        j++;
    }
  }
  printf("%s",save);
  //filename = realloc(filename,strlen(filename+6)*sizeof(char));
  //strcat(filename,".save");

  FILE* out = fopen("../../grid_result/grid.save", "w+");
  if (out == NULL)
		fprintf(stderr, "Error opening file\n");

  fwrite(save,1,110,out);
  fclose(out);
}
void img_print(Img* img) {
	matrix_print(img->img_data);
	printf("Img Label: %d\n", img->label);
}

void img_free(Img* img) {
	matrix_free(img->img_data);
	free(img);
	img = NULL;
}

void imgs_free(Img** imgs, int n) {
	for (int i = 0; i < n; i++) {
		img_free(imgs[i]);
	}
}
