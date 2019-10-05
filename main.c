#include<stdio.h>
#include <stdlib.h>
#include"parser.h"
#include"initstruct.h"
#include <time.h> 
extern int mode;

int main() {
	setbuf(stdout, NULL);
	srand((time(NULL)));
	mode = 0; /* start game in init mode */
	markErrors = 1;
	erroneousBoard = 0;
	printf("Please enter your command to start the game:\n");
	getCommand();
	return 0;
}
