#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

//______________________________________________________________________________

size_t space[18] = {3,7,15,19,27,31,40,44,52,56,64,68,77,81,89,93,101,105};
size_t back[11] = {11,23,35,36,48,60,72,73,85,97,109};

//______________________________________________________________________________
 
size_t x_of(size_t i)
{
    return i % 9;
}
 
size_t y_of(size_t i)
{
    return i / 9;
}

size_t i_of(size_t i)
{
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
    strcat(filename,".result");
    FILE* output_file = fopen(filename, "w+");
    if (!output_file) {
        errx(1,"fopen");
    }

    fwrite(result, 1, strlen(result), output_file);
    printf("Done Writing!\n");

    fclose(output_file);
}

void read(char* filename,char* buffer)
{
    
    FILE* input_file = fopen(filename , "r+");
    if (!input_file) {
        errx(1,"fopen");
    }
    
    fread(buffer, strlen(buffer)+1, 1, input_file);
    fclose(input_file);
}

void verified(char *board,char *output)
{
    size_t j = 0 ; size_t ispace = 0 ; size_t iback = 0;
    for(size_t i = 0; i < 110; i++)
    {
        if(i == space[ispace] )
		{
            if(board[i] != ' ')
                errx(2,"\033[0;31m verified:\n-invalid syntax: the input don't match \033[0m");
		}
        else if(i == back[iback] )
		{
            if(board[i] != '\n')
                errx(2,"\033[0;31m verified:\n-invalid syntax: the input don't match \033[0m");
		}
        else if(board[i] == ' ' || board[i] ==  '\n')
            errx(2,"\033[0;31m verified:\n-invalid syntax: the input don't match \033[0m");
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
    char vboard[82];
    verified(board,vboard);
    for(size_t i = 0;i<81;i++)
        if(vboard[i] != '.')
            result[i][0] = vboard[i]-48;
}

void translateback(char board[81][10], char* result)
{
    size_t j = 0 ; size_t ispace = 0 ; size_t iback = 0;
    for(size_t i = 0; i<81;i++)
    {
        if(i == 35 || i == 72)
        {
            result[j] = ' ';
            j++;
        }
        if(i == space[ispace] )
        {
            result[j] = ' ';
            j++;
        }
        else if(i == back[iback] )
        {
            result[j] = '\n';
            j++;
        }
        result[j] = board[i][0] + 48;
        j++;
    }
    result[j] = 0;
}

