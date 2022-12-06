#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

//______________________________________________________________________________

//the list of every space position in a file
size_t space[18] = {3,7,15,19,27,31,40,44,52,56,64,68,77,81,89,93,101,105};
//the list of every line break position in a file
size_t back[11] = {11,23,35,36,48,60,72,73,85,97,109};

//______________________________________________________________________________

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
}

void translate(char* board,char result[81][10])
{
  //translate the grid on the file in a empty list result
    char vboard[81];
    //delete every invalid character and verified the syntax
    verified(board,vboard);
    //put numbers of the grid to the list
    for(size_t i = 0;i<81;i++)
        if(vboard[i] != '.')
          result[i] = vboard[i]-48;
        else
          result[i] = 0;
}
