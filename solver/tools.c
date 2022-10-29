#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include "solver.h"

//______________________________________________________________________________

//the list of every space position in a file
size_t space[18] = {3,7,15,19,27,31,40,44,52,56,64,68,77,81,89,93,101,105};
//the list of every line break position in a file
size_t back[11] = {11,23,35,36,48,60,72,73,85,97,109};

//______________________________________________________________________________

size_t x_of(size_t i)
{
  //return the x coordonate of i
    return i % 9;
}

size_t y_of(size_t i)
{
  //return the y coordonate of i
    return i / 9;
}

size_t i_of(size_t i)
{
  //return the area coordonate of i
    return (y_of(i) / 3)*27 + (x_of(i) / 3)*3;
}

//______________________________________________________________________________

size_t valid_numb(char s[81][10], size_t i, size_t j)
{
    //return the next possible number
    j++;
    while(s[i][j] == 0)
        j++;
    return j;
}

//______________________________________________________________________________

void print(char s[81][10])
{
    //print the grid
    for(size_t i = 0; i < 9; i++)
    {
        for(size_t j = 0; j < 9; j++)
            printf("%hhi ",s[i*9+j][0]);
        printf("\n");
    }
    printf("\n");
}

//______________________________________________________________________________

void write(char* filename,char* result)
{
  //add ".result" after the filename
    strcat(filename,".result");
    FILE* output_file = fopen(filename, "w+");
    if (!output_file) {
        errx(1,"fopen");
    }

    //write the grid in the file
    fwrite(result, 1, 110, output_file);
    printf("Done Writing!\n");

    fclose(output_file);
}

void read(char* filename,char* buffer)
{

    FILE* input_file = fopen(filename , "r+");
    if (!input_file) {
        errx(1,"fopen");
    }

    //get the grid from the file to buffer
    fread(buffer, 110, 1, input_file);
    fclose(input_file);
}

//_____________________________________________________________________________

void verified(char *board,char *output)
{
    //verified if the grid on the file have a correct syntax and delete every space and line break
    size_t j = 0 ; size_t ispace = 0 ; size_t iback = 0;
    for(size_t i = 0; i < 110; i++)
    {
      if(i == space[ispace] )
	      {
          if(board[i] != ' ')
            errx(2,"\033[0;31m verified:\n-invalid syntax: the input don't match (unexpect number, must be ' ' at %lu) \033[0m",i);
	        ispace++;
	      }
      else if(i == back[iback] )
	     {
          if(board[i] != '\n')
            errx(2,"\033[0;31m verified:\n-invalid syntax: the input don't match (unexpect number, must be \\n at %lu) \033[0m",i);
	        iback++;
	      }
      else if(board[i] == ' ' || board[i] ==  '\n')
        errx(2,"\033[0;31m verified:\n-invalid syntax: the input don't match (unexpect space, must be a number at %lu)\033[0m",i);
      else
        {
          output[j] = board[i];
          j++;
        }
    }
    output[j] = 0;
}

void translate(char* board,char result[81][10])
{
  //translate the grid on the file in a empty list result
    char vboard[82];
    //delete every invalid character and verified the syntax
    verified(board,vboard);
    //put numbers of the grid to the list
    for(size_t i = 0;i<81;i++)
        if(vboard[i] != '.')
            result[i][0] = vboard[i]-48;
}

void translateback(char board[81][10], char* result)
{
  //translate a list with the file syntax to a string
    size_t j = 0 ;
    for(size_t i = 0; i<81;i++)
    {
  	result[j] = board[i][0] + 48;
  	j++;
        if(i == 26 || i == 53)
        {
            result[j] = '\n';
            j++;
        }
        if((i+1)%9 == 0)
        {
            result[j] = '\n';
	    j++;
        }
        else if((i+1)%3 == 0 && i != 0)
        {
            result[j] = ' ';
	    j++;
        }
    }
    result[j] = 0;
}
