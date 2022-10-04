#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <time.h>
#include <solver.h>

int main(int argc,char** argv)
{
	if(argc == 1)
		printf("solver(char* filename) => void\nsolve the sudoku of filename and save it in filename.result");
	else if (argc == 2)
		errx("\033[0;31m main:\n-invalid argument: solver must have just 1 argument (try ./solver to know more about) \033[0m");
	
	char grid[111];
	char sudoku[81][10] = {{0}};
	
	clock_t start,stop,initstart,initstop;
	unsigned long time,inittime;
	char success;
	
	read(argv[1],grid);
	translate(grid,sudoku);
	
	initstart = clock();
	init(sudoku);
	initstop = clock();
	
	start = clock();
	success = force(sudoku);
	stop = clock();
	
	print(sudoku);
	time = (unsigned long) difftime (stop, start);
	inittime = (unsigned long) difftime (initstop, initstart);
	printf("Init finished in %6.3ld millisec\n Solve finished in %6.3ld millisec\n",inittime,time);
	
	translateback(sudoku,grid);
	write(argv[1],grid);
	
	if(success)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}