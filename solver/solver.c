#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include <time.h>
#include "solverprog.h"
#include "tools_solver.h"

int solver(char *filename)
{
	char grid[111];
	char sudoku[81][10] = {{0}};

	/*
	clock_t start,stop,initstart,initstop;
	unsigned long time,inittime;
	char success;
	*/

	read_solver(filename,grid);
	translate_solver(grid,sudoku);


	//initstart = clock();
	init(sudoku);
	//initstop = clock();

	//start = clock();
	int success = force(sudoku,0);
	//stop = clock();

	print_solver(sudoku);
	/*time = (unsigned long) difftime (stop, start);
	inittime = (unsigned long) difftime (initstop, initstart);
	printf("Init finished in %6.3ld millisec\n Solve finished in %6.3ld millisec\n\n",inittime,time);
	*/
	translateback(sudoku,grid);

	char *result = malloc(sizeof(char) * strlen(filename) + 8);
	sprintf(result, "%s.result",filename);
	write_solver(result,grid);

	if(success)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
