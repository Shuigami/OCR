#include <stdio.h>
#include <stdlib.h>
#include "solver.h"

size_t x_of(size_t i);
size_t y_of(size_t i);
size_t i_of(size_t i);
size_t valid_numb(char s[81][10],size_t i,size_t j);

//_____________________________initialisation___________________________________

void fill(char s[81][10])
{
    //for each empty box, init possible numbers
    for(size_t i = 0;i<81;i++)
        if(s[i][0] == (char)0)
            for(short j = 0;j<10;j++)
                s[i][j] = j;
}

void del_h(char s[81][10],size_t y)
{
    //delete impossible numbers for the line "y"
    char notpermit[9] = {1,2,3,4,5,6,7,8,9};
    //find every impossible numbers
    for(size_t i = y;i<y+9;i++)
        if(s[i][0] != 0)
            notpermit[s[i][0]-1] = 0;
    //delete for the empty box
    for(size_t i = y;i<y+9;i++)
        if(s[i][0] == 0)
            for(size_t j = 1;j<10;j++)
                s[i][j] = notpermit[j-1];
}

void del_v(char s[81][10],size_t x)
{  
    //delete impossible numbers for the row "x"
    char notpermit[9] = {0};
    //find every impossible numbers
    for(size_t i = x;i<=81;i+=9)
        if(s[i][0] != 0)
            notpermit[s[i][0]-1] = s[i][0];
    //delete for the empty box
    for(size_t i = x;i<=81;i+=9)
        if(s[i][0] == 0)
            for(size_t j = 1;j<10;j++)
                if(s[i][j] == notpermit[j-1])
                    s[i][j] = 0;
}

void del_s(char s[81][10],size_t p)
{
    //delete impossible numbers for the 3x3 square at "p"
    char notpermit[9] = {0};
    //find every impossible numbers
    for(size_t i = 0;i<3;i++)
        for(size_t j=0;j<3;j++)
            if(s[p + i*9 +j][0] != 0)
                notpermit[s[p+i*9+j][0]-1] = s[p+i*9+j][0];
    //delete for the empty box
    for(size_t i = 0;i<3;i++)
        for(size_t j=0;j<3;j++)
            if(s[p+i*9+j][0] == 0)
                for(size_t k = 1;k<10;k++)
                    if(s[p+i*9+j][k] == notpermit[k-1])
                        s[p+i*9+j][k] = 0;
}

void one(char s[81][10])
{
    //init every empty box with only one possible number
    for(size_t i = 0;i<81;i++)
        if(s[i][0] == 0)
        {
            char nbr = 0;
            size_t j = 1;
            while(j < 10 && !(s[i][j] && nbr))
            {
                if(s[i][j])
                    nbr = s[i][j];
                j++;
            }
            if(j == 10)
                s[i][0] = nbr;
        }
            
}

void init(char s[81][10])
{
    //init sudoku board
    fill(s);
    for(size_t i = 0;i<81;i+=9)
        del_h(s,i);
    for(size_t i = 0;i<9;i++)
        del_v(s,i);
    for(size_t i = 0;i<81;i+=27)
        for(size_t j = 0;j<9;j+=3)
            del_s(s,i+j);
    one(s);
}

//___________________________________check______________________________________

char valid_h(char s[81][10], size_t y,char n)
{
    //check if the line is valid
    size_t i = 0;
    while(i < 9 && s[y*9+i][0] != n)
        i++;
    return i == 9;
}

char valid_v(char s[81][10], size_t x,char n)
{
    //check if the row is valid
    size_t i = 0;
    while(i < 81 && s[x+i][0] != n)
        i+=9;
    return i >= 81;
}

char valid_s(char s[81][10], size_t p,char n)
{
    //check if the 3x3 square is valid
    for(size_t i = 0;i<3;i++)
        for(size_t j=0;j<3;j++)
            if(s[p + i*9 +j][0] == n)
                return 0;
    return 1;
}

char valid(char s[81][10], size_t i, char n)
{
    return valid_h(s,y_of(i),n) && valid_v(s,x_of(i),n) && valid_s(s,i_of(i),n);
}

//________________________________solver________________________________________

char force(char s[81][10], size_t i)
{
    //the board is complete
    if(i >= 81)
        return 1;
    //if the box is not empty go to the next
    if(s[i][0])
        return force(s,i+1);
    //try every possible numbers
    size_t j = 0;
    j = valid_numb(s,i,j);
    while(j < 10)
    {
        //if the number is valid go to the next
        if(valid(s,i,s[i][j]))
        {
            s[i][0] = s[i][j];
            if(!force(s,i+1))
                s[i][0] = 0;
            else
                return 1;
        }
        j = valid_numb(s,i,j);
    }
    return 0;
}

//______________________________________________________________________________
